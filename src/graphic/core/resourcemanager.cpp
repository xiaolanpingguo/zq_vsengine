#include "graphic/core/resourcemanager.h"
#include "graphic/node/geometry.h"
#include "graphic/node/nodecomponent/light/light.h"
#include "graphic/material/material.h"
#include "graphic/material/shader/vshader.h"
#include "graphic/material/shader/pshader.h"
#include "graphic/material/shader/gshader.h"
#include "graphic/material/shader/dshader.h"
#include "graphic/material/shader/hshader.h"
#include "graphic/material/shader/cshader.h"
#include "graphic/core/name.h"
#include "graphic/render/texture/texallstate.h"
#include "core/image.h"
#include "graphic/material/shaderstringfactory.h"
#include "graphic/core/nvcompression.h"
#include "graphic/render/texture/texture2d.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/render/outputresource/rendertarget.h"
#include "graphic/render/depthstencil.h"
#include "graphic/render/texture/cubetexture.h"
#include "graphic/core/stream/stream.h"
#include "graphic/posteffect/posteffectset.h"
#include "graphic/controller/animtree/animtree.h"
#include "graphic/render/texture/capturetexallstate.h"
#include "graphic/core/profiler.h"
#include "graphic/world/actor/actor.h"
#include "graphic/world/scenemap.h"
#include "graphic/world/world.h"
#include "graphic/node/dynamicbuffergeometry.h"
#include "graphic/node/instancegeometry.h"
#include "graphic/render/outputresource/unorderaccess.h"
#include "graphic/render/buffer/bufferresource.h"
#include "graphic/core/config.h"
using namespace zq;
namespace zq
{

	class VSGCTask :public MemObject
	{
	public:
		VSGCTask(VSArray<VSObject *>& CanGCObject, uint32 CanGCNum = 50)
		{
			m_CanGCNum = CanGCNum;
			m_CanGCObject = CanGCObject;
			CurClearIndex = 0;
			CurDeleteIndex = 0;
			GCStream.SetStreamFlag(VSStream::AT_CLEAR_OBJECT_PROPERTY_GC);
			m_pNextTask = NULL;
		}
		void Run()
		{
			if (CurClearIndex >= m_CanGCObject.GetNum())
			{
				uint32 MaxDeleteIndex = CurDeleteIndex + m_CanGCNum;
				for (; CurDeleteIndex < MaxDeleteIndex && CurDeleteIndex < m_CanGCObject.GetNum(); CurDeleteIndex++)
				{
					SAFE_DELETE(m_CanGCObject[CurDeleteIndex]);
				}
			}
			else
			{
				uint32 MaxClearIndex = CurClearIndex + m_CanGCNum;
				for (; CurClearIndex < MaxClearIndex && CurClearIndex < m_CanGCObject.GetNum(); CurClearIndex++)
				{
					GCStream.ArchiveAll(m_CanGCObject[CurClearIndex]);
				}
			}
		}
		VSGCTask * m_pNextTask;
		bool IsEnd()
		{
			return CurDeleteIndex >= m_CanGCObject.GetNum();
		}
	private:
		VSArray<VSObject *> m_CanGCObject;
		uint32 m_CanGCNum;
		uint32 CurClearIndex;
		uint32 CurDeleteIndex;
		VSStream GCStream;

	};
}

namespace zq
{
	bool operator ==(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2)
	{
		return DBI1.pVertexFormat == DBI2.pVertexFormat && DBI1.uiMeshDataType == DBI2.uiMeshDataType;
	}
	bool operator >(const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2)
	{
		if (DBI1.pVertexFormat > DBI2.pVertexFormat)
		{
			return true;
		}
		else if (DBI1.pVertexFormat == DBI2.pVertexFormat)
		{
			return DBI1.uiMeshDataType > DBI2.uiMeshDataType;
		}
		else
		{
			return false;
		}
	}
	bool operator < (const DynamicBufferIndex & DBI1, const DynamicBufferIndex & DBI2)
	{
		if (DBI1.pVertexFormat < DBI2.pVertexFormat)
		{
			return true;
		}
		else if (DBI1.pVertexFormat == DBI2.pVertexFormat)
		{
			return DBI1.uiMeshDataType < DBI2.uiMeshDataType;
		}
		else
		{
			return false;
		}
	}
}
VSArray<VSDynamicBufferGeometryPtr> VSResourceManager::ms_SaveDelete;
uint32 VSResourceManager::ms_uiGpuSkinBoneNum = 70;
uint32 VSResourceManager::ms_uiGpuMorphTargetNum = 40;
uint32 VSResourceManager::ms_uiGpuMorphTextureSize = 512;
bool VSResourceManager::ms_bLerpSimpleInstanceAnim = true;
bool VSResourceManager::ms_bActiveDebugDraw = true;
bool VSResourceManager::ms_bDrawSkeleton = true;
bool VSResourceManager::ms_bRenderThread = false;
bool VSResourceManager::ms_bUpdateThread = false;
bool VSResourceManager::ms_bIsCacheShader = true;
VSCriticalSection VSResourceManager::ms_NameCri;
uint32 VSResourceManager::ms_CurRenderAPIType = 0;
uint32 VSResourceManager::ms_uiSimpleInstanceAnimFloat4Num = 0;
VSArray<VSObject *>VSResourceManager::ms_pRootObject;
VSArrayOrder<VSObject *>VSResourceManager::ms_pGCObject;
VSGCTask * VSResourceManager::ms_pCurGCTask = NULL;
VSGCTask * VSResourceManager::ms_pEndGCTask = NULL;
IMPLEMENT_PRIORITY(VSResourceManager)
IMPLEMENT_INITIAL_ONLY_BEGIN(VSResourceManager);
ADD_PRIORITY(VSTexAllState);
ADD_PRIORITY(VSMaterial);
ADD_PRIORITY(VSGeometry);
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_ONLY_END
bool VSResourceManager::TerminalDefaultState()
{
	return true;
}
bool VSResourceManager::InitialDefaultState()
{
	ms_uiSimpleInstanceAnimFloat4Num = (unsigned int)CEIL((MAX_SIMPLE_INSTANCE_ANIM * 2 + 1) / 4.0f);
	return 1;
}
VSResourceManager::VSResourceManager()
{
	
}
VSResourceManager::~VSResourceManager()
{

}

VSDVGeometry *VSResourceManager::GetDVGeometry(VSVertexFormat * pVertexFormat, uint32 MeshDataType, uint32 VertexNum)
{
	if (!pVertexFormat || !VertexNum || MeshDataType>= VSMeshData::MDT_MAX)
	{
		return NULL;
	}
	VSDVGeometryPtr  pBuffer = NULL;
	DynamicBufferIndex DBI;
	DBI.pVertexFormat = pVertexFormat;
	DBI.uiMeshDataType = MeshDataType;
	pBuffer = GetDVGeometryArray().CheckIsHaveTheResource(DBI);
	if (pBuffer == NULL)
	{
		pBuffer = NULL;
		pBuffer = MEM_NEW VSDVGeometry();
		pBuffer->Create(pVertexFormat->m_FormatArray, MeshDataType, VertexNum);
		GetDVGeometryArray().AddResource(DBI, pBuffer);
	}
	else
	{	
		if (pBuffer->GetMaxRenderVertexNum() < VertexNum)
		{		
			ms_SaveDelete.AddElement(pBuffer.GetObject());
			pBuffer = MEM_NEW VSDVGeometry();
			pBuffer->Create(pVertexFormat->m_FormatArray, MeshDataType, VertexNum);
			MapElement<DynamicBufferIndex, VSDVGeometryPtr> * PTemp = (MapElement<DynamicBufferIndex, VSDVGeometryPtr> *)(GetDVGeometryArray().GetResource(GetDVGeometryArray().GetResourceIndexByKey(DBI)));
			PTemp->Value = pBuffer;
		}
	}

	return pBuffer;
}
void VSResourceManager::LoadDefaultDeviceResource(uint32 RenderTypeAPI)
{
	InitCacheShader(RenderTypeAPI);
}
void VSResourceManager::InitCacheShader(uint32 RenderTypeAPI)
{
	ms_CurRenderAPIType = RenderTypeAPI;
	if (ms_CurRenderAPIType == VSRenderer::RAT_NULL)
	{
		return;
	}
	if (!VSResourceManager::IsCacheShader())
	{
		return;
	}
#if _DEBUG 
	VSString Suffix = _T("_d");
#else
	VSString Suffix;
#endif

#define LOAD_SHADER_CACHE_INNER(ShaderMapName) \
	{\
		VSStream LoadStream;\
		VSString FileName = VSShaderMapCache::GetCachePath() + Get##ShaderMapName().m_ShaderMapName + Suffix; \
		FileName += VSShaderMapCache::ms_PointFileSuffix; \
		LoadStream.Load(FileName.GetBuffer());\
		VSShaderMapCache * pShaderMapLoadSave = (VSShaderMapCache *)LoadStream.GetObjectByRtti(VSShaderMapCache::ms_Type);\
		if (pShaderMapLoadSave)\
		{\
			Get##ShaderMapName().GetShaderMap() = pShaderMapLoadSave->m_ShaderMap; \
			SAFE_DELETE(pShaderMapLoadSave); \
		}\
	}

#define LOAD_SHADER_CACHE(Name) \
	LOAD_SHADER_CACHE_INNER(Name##VertexShaderMap);\
	LOAD_SHADER_CACHE_INNER(Name##PixelShaderMap);\
	LOAD_SHADER_CACHE_INNER(Name##GeometryShaderMap);\
	LOAD_SHADER_CACHE_INNER(Name##HullShaderMap);\
	LOAD_SHADER_CACHE_INNER(Name##DomainShaderMap);\
	LOAD_SHADER_CACHE_INNER(Name##ComputeShaderMap);\

	LOAD_SHADER_CACHE(Custom);

	LOAD_SHADER_CACHE(Material);

	LOAD_SHADER_CACHE(Indirect);

	LOAD_SHADER_CACHE(NormalDepth);

	LOAD_SHADER_CACHE(CubShadow);

	LOAD_SHADER_CACHE(VolumeShadow);

	LOAD_SHADER_CACHE(Shadow);

	LOAD_SHADER_CACHE(DualParaboloidShadow);

	LOAD_SHADER_CACHE(PreZ);

	LOAD_SHADER_CACHE(Inner);

}
void VSResourceManager::AddCanGCObject(VSArray<VSObject *>& CanGCObject)
{
	if (CanGCObject.GetNum() == 0)
	{
		return;
	}
	if (!ms_pCurGCTask)
	{
		ms_pCurGCTask = MEM_NEW VSGCTask(CanGCObject);
		ms_pEndGCTask = ms_pCurGCTask;
	}
	else
	{
		ms_pEndGCTask->m_pNextTask = MEM_NEW VSGCTask(CanGCObject);
		ms_pEndGCTask = ms_pEndGCTask->m_pNextTask;
	}
}
void VSResourceManager::RunGCTask()
{
	if (ms_pCurGCTask)
	{
		ms_pCurGCTask->Run();
		if (ms_pCurGCTask->IsEnd())
		{
			VSGCTask * Temp = ms_pCurGCTask;
			ms_pCurGCTask = ms_pCurGCTask->m_pNextTask;
			SAFE_DELETE(Temp);
		}
	}
}
void VSResourceManager::RunAllGCTask()
{
	ms_pRootObject.Clear();
	while (ms_pCurGCTask)
	{
		RunGCTask();
	}
	VSMAC_ASSERT(ms_pGCObject.GetNum() == 0);
}
DECLEAR_NOCLEAR_COUNT_PROFILENODE(RootGCObjectNum, )
void VSResourceManager::AddRootObject(VSObject * p)
{
	p->SetFlag(VSObject::OF_RootObject);
	ms_pRootObject.AddElement(p);
	ADD_COUNT_PROFILE(RootGCObjectNum, 1)
}
void VSResourceManager::DeleteRootObject(VSObject * p)
{
	for (uint32 i = 0; i < ms_pRootObject.GetNum(); i++)
	{
		if (ms_pRootObject[i] == p)
		{
			p->ClearFlag(VSObject::OF_RootObject);
			ms_pRootObject.Erase(i);
			ADD_COUNT_PROFILE(RootGCObjectNum, -1)
			return;
		}
	}
}
DECLEAR_NOCLEAR_COUNT_PROFILENODE(GCObjectNum, )
void VSResourceManager::AddGCObject(VSObject * p)
{
	ms_pGCObject.AddElement(p);
	p->SetFlag(VSObject::OF_GCObject);
	ADD_COUNT_PROFILE(GCObjectNum, 1)
}
void VSResourceManager::GCObject()
{
	{
		for (uint32 i = 0; i < ms_pRootObject.GetNum(); i++)
		{
			ms_pRootObject[i]->ClearFlag(VSObject::OF_REACH);
			ms_pRootObject[i]->SetFlag(VSObject::OF_UNREACH);
		}
		for (uint32 i = 0; i < ms_pGCObject.GetNum();i++)
		{
			ms_pGCObject[i]->ClearFlag(VSObject::OF_REACH);
			ms_pGCObject[i]->SetFlag(VSObject::OF_UNREACH);
		}
	}
	// collect not reachable object
	{
		VSStream GCCollectStream;
		GCCollectStream.SetStreamFlag(VSStream::AT_OBJECT_COLLECT_GC);
		for (uint32 i = 0; i < ms_pRootObject.GetNum(); i++)
		{
			GCCollectStream.ArchiveAll(ms_pRootObject[i]);
		}
	}

	{
		VSArray<VSObject *> CanGCObject;
		for (uint32 i = 0; i < ms_pGCObject.GetNum();)
		{
			VSObject * p = ms_pGCObject[i];
			if (p->IsHasFlag(VSObject::OF_UNREACH))
			{
				CanGCObject.AddElement(p);
				ms_pGCObject.Erase(i);
				ADD_COUNT_PROFILE(GCObjectNum, -1)
			}
			else
			{
				i++;
			}
		}

		AddCanGCObject(CanGCObject);
		RunGCTask();
	}

}
bool VSResourceManager::CacheShader()
{
	if (ms_CurRenderAPIType == VSRenderer::RAT_NULL)
	{
		return true;
	}
#if _DEBUG 
	VSString Suffix = _T("_d");
#else
	VSString Suffix;
#endif
#define SAVE_SHADER_CACHE_INNER(ShaderMapName)\
	{\
		VSStream SaveStream;\
		SaveStream.SetStreamFlag(VSStream::AT_REGISTER);\
		VSShaderMapCache * pShaderMapLoadSave = MEM_NEW VSShaderMapCache();\
		pShaderMapLoadSave->m_ShaderMap = Get##ShaderMapName().GetShaderMap(); \
		SaveStream.ArchiveAll(pShaderMapLoadSave);\
		VSString FileName = VSShaderMapCache::GetCachePath() + Get##ShaderMapName().m_ShaderMapName + Suffix; \
		FileName += VSShaderMapCache::ms_PointFileSuffix;\
		SaveStream.Save(FileName.GetBuffer());\
		SAFE_DELETE(pShaderMapLoadSave);\
	}
#define SAVE_SHADER_CACHE(Name)\
	SAVE_SHADER_CACHE_INNER(Name##VertexShaderMap);\
	SAVE_SHADER_CACHE_INNER(Name##PixelShaderMap);\
	SAVE_SHADER_CACHE_INNER(Name##GeometryShaderMap);\
	SAVE_SHADER_CACHE_INNER(Name##HullShaderMap);\
	SAVE_SHADER_CACHE_INNER(Name##DomainShaderMap);\
	SAVE_SHADER_CACHE_INNER(Name##ComputeShaderMap);

	SAVE_SHADER_CACHE(Custom);
	SAVE_SHADER_CACHE(Material);
	SAVE_SHADER_CACHE(Indirect);
	SAVE_SHADER_CACHE(NormalDepth);
	SAVE_SHADER_CACHE(CubShadow);
	SAVE_SHADER_CACHE(VolumeShadow);
	SAVE_SHADER_CACHE(Shadow);
	SAVE_SHADER_CACHE(DualParaboloidShadow);
	SAVE_SHADER_CACHE(PreZ);
	SAVE_SHADER_CACHE(Inner);

	return 1;
}
void VSResourceManager::CacheResource()
{
	VSResourceControl::AllClear();
	
	if (VSResourceManager::IsCacheShader())
	{
		CacheShader();

	}
#define CLEAR_SHADER_CACHE_INNER(ShaderMapName) 	Get##ShaderMapName().Clear(); 
#define CLEAR_SHADER_CACHE(Name)\
	CLEAR_SHADER_CACHE_INNER(Name##VertexShaderMap);\
	CLEAR_SHADER_CACHE_INNER(Name##PixelShaderMap);\
	CLEAR_SHADER_CACHE_INNER(Name##GeometryShaderMap);\
	CLEAR_SHADER_CACHE_INNER(Name##HullShaderMap);\
	CLEAR_SHADER_CACHE_INNER(Name##DomainShaderMap);\
	CLEAR_SHADER_CACHE_INNER(Name##ComputeShaderMap);

	CLEAR_SHADER_CACHE(Custom);
	CLEAR_SHADER_CACHE(Material);
	CLEAR_SHADER_CACHE(Indirect);
	CLEAR_SHADER_CACHE(NormalDepth);
	CLEAR_SHADER_CACHE(CubShadow);
	CLEAR_SHADER_CACHE(VolumeShadow);
	CLEAR_SHADER_CACHE(Shadow);
	CLEAR_SHADER_CACHE(DualParaboloidShadow);
	CLEAR_SHADER_CACHE(PreZ);
	CLEAR_SHADER_CACHE(Inner);	

	GetVertexFormatSet().ClearAll();
	GetBlendStateSet().ClearAll();
	GetDepthStencilStateSet().ClearAll();
	GetRasterizerStateSet().ClearAll();
	GetSamplerStateSet().ClearAll();
	GetNameSet().ClearAll();
	GetRenderTargetArray().ClearAll();
	GetDepthStencilArray().ClearAll();
	GetUnorderAccessArray().ClearAll();
	GetRenderTargetBufferArray().ClearAll();
	GetDepthStencilBufferArray().ClearAll();
	GetUnorderAccessBufferArray().ClearAll();
	GetDVGeometryArray().ClearAll();
	GetDynamicInstanceGeometryArray().ClearAll();
}
DECLEAR_TIME_PROFILENODE(GCTime,ApplicationUpdate)
void VSResourceManager::GC()
{
	ADD_TIME_PROFILE(GCTime)

	VSResourceControl::GCAllResource();

	GetRenderTargetBufferArray().GCResource();
	GetDepthStencilBufferArray().GCResource();
	GetUnorderAccessBufferArray().GCResource();
	GetDynamicInstanceGeometryArray().GCResource();
	GCObject();
}
bool VSResourceManager::IsReleaseAll()
{
#define IS_RELEASE_ALL(MapName) 	if(!Get##MapName().IsReleseAll()){return false;}


#define IS_RELEASE_ALL_SHADER(Name)\
	IS_RELEASE_ALL(Name##VertexShaderMap);\
	IS_RELEASE_ALL(Name##PixelShaderMap);\
	IS_RELEASE_ALL(Name##GeometryShaderMap);\
	IS_RELEASE_ALL(Name##HullShaderMap);\
	IS_RELEASE_ALL(Name##DomainShaderMap);\
	IS_RELEASE_ALL(Name##ComputeShaderMap);

	if (!VSResourceControl::IsAllRelese())
	{
		return false;
	}

	IS_RELEASE_ALL(VertexFormatSet);


	IS_RELEASE_ALL_SHADER(Custom);

	IS_RELEASE_ALL_SHADER(Material);

	IS_RELEASE_ALL_SHADER(Indirect);

	IS_RELEASE_ALL_SHADER(NormalDepth);

	IS_RELEASE_ALL_SHADER(CubShadow);

	IS_RELEASE_ALL_SHADER(VolumeShadow);

	IS_RELEASE_ALL_SHADER(Shadow);

	IS_RELEASE_ALL_SHADER(DualParaboloidShadow);

	IS_RELEASE_ALL_SHADER(PreZ);

	IS_RELEASE_ALL_SHADER(Inner);


	IS_RELEASE_ALL(NameSet);

	IS_RELEASE_ALL(BlendStateSet);

	IS_RELEASE_ALL(SamplerStateSet);

	IS_RELEASE_ALL(DepthStencilStateSet);

	IS_RELEASE_ALL(RasterizerStateSet);

	IS_RELEASE_ALL(DVGeometryArray);


	VSMAC_ASSERT(ms_pRootObject.GetNum() == 0);
	VSMAC_ASSERT(ms_pGCObject.GetNum() == 0);
	return true;
}
VSCaptureTexAllState * VSResourceManager::CreateCaptureTexture(const VSString & ViewFamilyName,uint32 uiMipLevel)
{
	VSCaptureTexAllState * pCaptureTexture = MEM_NEW VSCaptureTexAllState();
	pCaptureTexture->SetViewCapture(ViewFamilyName);

	pCaptureTexture->SetMipLevel(uiMipLevel);

	return pCaptureTexture;
}
uint32 GetEngineCompressFormat(VSResourceManager::CompressType uiCompressType)
{
	uint32 uiVSTextureType = VSRenderer::SFT_A8R8G8B8;
	if (uiCompressType == VSResourceManager::CT_NONE)
	{
		uiVSTextureType = VSRenderer::SFT_A8R8G8B8;
	}
	else if (uiCompressType == VSResourceManager::CT_BC1)
	{
		uiVSTextureType = VSRenderer::SFT_BC1;
	}
	else if (uiCompressType == VSResourceManager::CT_BC2)
	{
		uiVSTextureType = VSRenderer::SFT_BC2;
	}
	else if (uiCompressType == VSResourceManager::CT_BC3)
	{
		uiVSTextureType = VSRenderer::SFT_BC3;
	}
	else if (uiCompressType == VSResourceManager::CT_BC4)
	{
		uiVSTextureType = VSRenderer::SFT_BC4;
	}
	else if (uiCompressType == VSResourceManager::CT_BC5)
	{
		uiVSTextureType = VSRenderer::SFT_BC5;
	}
	return uiVSTextureType;
}
VSTexAllState * VSResourceManager::Load2DTexture(const char *pFileName,VSSamplerStatePtr pSamplerState,
	CompressType uiCompressType,bool bIsNormal,bool bSRGB, bool bMip)
{
	if (bIsNormal)
	{
		bSRGB = false;
	}
	
	if (!pFileName)
	{
		
		return NULL;
	}
	
	if (uiCompressType > CT_MAX)
	{
		
		return NULL;
	}

	VSFileName FileName = pFileName;
	VSString Extension; 

	if (!FileName.GetExtension(Extension))
	{
		
		return NULL;
	}
	VSTexAllState * pTexAllState = NULL;

	Image *pImage = NULL;
	uint32 uiFormatType = VSRenderer::SFT_A8R8G8B8;
	if (Extension == Image::ms_ImageFormat[Image::IF_BMP])
	{
		pImage = MEM_NEW VSBMPImage();
	}
	else if (Extension == Image::ms_ImageFormat[Image::IF_TGA])
	{
		pImage = MEM_NEW VSTGAImage();
	}
	else 
	{
		
		return NULL;
	}

	if(!pImage->Load(FileName.GetBuffer()))
	{
		SAFE_DELETE(pImage);
		
		return NULL;
	}

	uint32 uiWidth = pImage->GetWidth();
	uint32 uiHeight = pImage->GetHeight();
	if (!uiWidth || !uiHeight)
	{
		SAFE_DELETE(pImage);
		
		return NULL;
	}
	if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
	{
		SAFE_DELETE(pImage);
		
		return NULL;
	}

	pTexAllState = MEM_NEW VSTexAllState();
	pTexAllState->m_uiWidth = uiWidth;
	pTexAllState->m_uiHeight = uiHeight;
	pTexAllState->m_bNormal = bIsNormal;
	pTexAllState->m_bSRGB = bSRGB;
	pTexAllState->m_bMip = bMip;
	pTexAllState->m_uiFormatType = GetEngineCompressFormat(uiCompressType);
	pTexAllState->m_SourceData.SetBufferNum(uiWidth * uiHeight * VSRenderer::GetBytesPerPixel(uiFormatType));

	for (uint32 cy = 0; cy < uiHeight; cy++) 
	{
		for (uint32 cx = 0; cx < uiWidth; cx++) 
		{

			uint32 uiIndex = cy * uiWidth + cx;
			unsigned char *pBuffer = pTexAllState->m_SourceData.GetBuffer() + uiIndex * VSRenderer::GetBytesPerPixel(uiFormatType);
			const unsigned char * pImageBuffer = pImage->GetPixel(cx,cy);
			if (pImage->GetBPP() == 8)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[0];
				pBuffer[2] = pImageBuffer[0];
				pBuffer[3] = pImageBuffer[0];
			}
			else if (pImage->GetBPP() == 24)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 32)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = pImageBuffer[3];
			}
		} // for
	} // for

	if (pImage)
	{
		SAFE_DELETE(pImage);
	}

	VS2DTexture * pNewTexture = CreateTextureCache(pTexAllState->m_SourceData.GetBuffer(), uiWidth, uiHeight, 
		pTexAllState->m_uiFormatType, bIsNormal, bSRGB, bMip);
	pTexAllState->m_pTex = pNewTexture;
	
	if (pSamplerState)
	{
		pTexAllState->SetSamplerState(pSamplerState);
	}
	return pTexAllState;
}
VS2DTexture * VSResourceManager::CreateTextureCache(void* SourceData, uint32 uiWidth, uint32 uiHeight,
	uint32 uiFormatType, bool bIsNormal, bool bSRGB, bool bMip)
{
	uint32 uiCompressFormat = VSNVCompression::NV_ARGB;
	if (uiFormatType == VSRenderer::SFT_A8R8G8B8)
	{
		uiCompressFormat = VSNVCompression::NV_ARGB;
	}
	else if (uiFormatType == VSRenderer::SFT_BC1)
	{
		uiCompressFormat = VSNVCompression::NV_BC1;
	}
	else if (uiFormatType == VSRenderer::SFT_BC2)
	{
		uiCompressFormat = VSNVCompression::NV_BC2;
	}
	else if (uiFormatType == VSRenderer::SFT_BC3)
	{
		uiCompressFormat = VSNVCompression::NV_BC3;
	}
	else if (uiFormatType == VSRenderer::SFT_BC4)
	{
		uiCompressFormat = VSNVCompression::NV_BC4;
	}
	else if (uiFormatType == VSRenderer::SFT_BC5)
	{
		uiCompressFormat = VSNVCompression::NV_BC5;
	}
	VS2DTexture * pNewTexture = MEM_NEW VS2DTexture(uiWidth, uiHeight, uiFormatType, bMip ? 0 : 1, 1, bSRGB);
	pNewTexture->CreateRAMData();

	VSNVCompression Help;
	Help.Setup(SourceData, uiCompressFormat, uiWidth, uiHeight, 1, 1, pNewTexture->GetMipLevel(), bIsNormal);
	Help.Process();
	VSMAC_ASSERT(pNewTexture->GetMipLevel() == Help.OutputHandler.m_DataBufferArray.GetNum());
	for (uint32 i = 0; i < pNewTexture->GetMipLevel(); i++)
	{
		VSMemcpy(pNewTexture->GetBuffer(i), Help.OutputHandler.m_DataBufferArray[i].GetBuffer(), Help.OutputHandler.m_DataBufferArray[i].GetNum());
	}
	return pNewTexture;
}
VSResourceProxyBase * VSResourceManager::LoadResource(const char *pFileName,bool IsAsyn)
{
	VSFileName FileName = pFileName;
	VSString Extension; 
	FileName.GetExtension(Extension);
	VSResourceProxyBase * pResource = NULL;
	if (Extension == VSTexAllState::ms_FileSuffix)
	{
		pResource = LoadASYNResource<VSTexAllState>(pFileName,IsAsyn);
	}
	else if (Extension == VSMaterial::ms_FileSuffix)
	{
		pResource = LoadASYNResource<VSMaterial>(pFileName,IsAsyn);
	}
	else if (Extension == VSAnim::ms_FileSuffix)
	{
		pResource = LoadASYNResource<VSAnim>(pFileName,IsAsyn);
	}
	else if (Extension == VSStaticMeshNode::ms_FileSuffix)
	{
		pResource = LoadASYNResource<VSStaticMeshNode>(pFileName,IsAsyn);
	}
	else if (Extension == VSSkeletonMeshNode::ms_FileSuffix)
	{
		pResource = LoadASYNResource<VSSkeletonMeshNode>(pFileName,IsAsyn);
	}
	else if (Extension == VSPostEffectSet::ms_FileSuffix)
	{
		pResource = LoadASYNResource<VSPostEffectSet>(pFileName,IsAsyn);
	}
	else if (Extension == VSAnimTree::ms_FileSuffix)
	{
		pResource = LoadASYNResource<VSAnimTree>(pFileName, IsAsyn);
	}
	else if (Extension == VSActor::ms_FileSuffix)
	{
		pResource = LoadASYNResource<VSActor>(pFileName, IsAsyn);
	}
	else if (Extension == VSFont::ms_FileSuffix)
	{
		pResource = LoadASYNResource<VSFont>(pFileName, IsAsyn);
	}
	return pResource;
}
void VSResourceManager::DeleteAllMapResource()
{
	VSCriticalSection::Locker Temp(VSSceneMap::ms_LoadResourceCriticalSection);
	for (uint32 i = 0; i < VSSceneMap::GetASYNResourceSet().GetResourceNum(); )
	{
		VSSceneMapRPtr pMapR = VSSceneMap::GetASYNResourceSet().GetResource(i)->Value;
		if (pMapR->IsLoaded())
		{
			if (VSWorld::ms_pWorld)
			{
				VSWorld::ms_pWorld->DestroyScene(pMapR->GetResource()->m_Name);
			}
		}
		else
		{
			VSFileName FileName = VSSceneMap::ms_ResourcePath + pMapR->GetResourceName().GetString();
			VSASYNLoadManager::ms_pASYNLoadManager->DeleteLoadResource(FileName);
		}
		VSSceneMap::GetASYNResourceSet().DeleteResource(pMapR->GetResourceName());
	}
	
}
void VSResourceManager::DeleteMapResource(const char *  pFileName)
{
	VSFileName FileName = pFileName;
	VSString Extension;
	VSUsedName ResourceName;
	if (FileName.GetExtension(Extension))
	{
		if (Extension != VSSceneMap::ms_FileSuffix)
		{
			return;
		}
		else
		{
			ResourceName = FileName;
			FileName = VSSceneMap::ms_ResourcePath + FileName;
		}
	}
	else
	{
		ResourceName = FileName + VSSceneMap::ms_PointFileSuffix;
		FileName = VSSceneMap::ms_ResourcePath + FileName + VSSceneMap::ms_PointFileSuffix;
	}

	VSCriticalSection::Locker Temp(VSSceneMap::ms_LoadResourceCriticalSection);
	VSSceneMapRPtr pMapR = (VSSceneMapR *)VSSceneMap::GetASYNResourceSet().CheckIsHaveTheResource(ResourceName);
	if (pMapR == NULL)
	{
		return;
	}
	if (pMapR->IsLoaded())
	{
		if (VSWorld::ms_pWorld)
		{
			VSWorld::ms_pWorld->DestroyScene(pMapR->GetResource()->m_Name);
		}	
	}
	else
	{
		VSASYNLoadManager::ms_pASYNLoadManager->DeleteLoadResource(FileName);
	}	
	VSSceneMap::GetASYNResourceSet().DeleteResource(ResourceName);

}
VSVertexFormat *VSResourceManager::LoadVertexFormat(VSVertexBuffer * pVertexBuffer,VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> *pFormatArray)
{
	if(!pVertexBuffer && !pFormatArray)
		return NULL;
	if (pVertexBuffer)
	{
		if(pVertexBuffer->m_pVertexFormat)
			return pVertexBuffer->m_pVertexFormat;
	}
	
	VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> FormatArray;
	if (!pFormatArray)
	{	
		if(!pVertexBuffer->GetVertexFormat(FormatArray))
			return	NULL;
		pFormatArray = &FormatArray;
	}
	else
	{
		if (!pFormatArray->GetNum())
		{
			return	NULL;
		}
	}

	uint32 lVertexFormatCode = VSMathInstance::GetMathInstance().CRC32Compute(pFormatArray->GetBuffer(), sizeof(VSVertexFormat::VERTEXFORMAT_TYPE) * pFormatArray->GetNum());
	
	VSVertexFormat * pVertexFormat = NULL;
	pVertexFormat = (VSVertexFormat *)VSResourceManager::GetVertexFormatSet().CheckIsHaveTheResource(lVertexFormatCode);
	if(pVertexFormat)
	{
		if (pVertexBuffer)
		{
			pVertexBuffer->m_pVertexFormat = pVertexFormat;
		}
		return pVertexFormat;
	}
	pVertexFormat = MEM_NEW VSVertexFormat();

	VSResourceManager::GetVertexFormatSet().AddResource(lVertexFormatCode,pVertexFormat);
	if (pVertexBuffer)
	{
		pVertexBuffer->m_pVertexFormat = pVertexFormat;
	}
	pVertexFormat->m_FormatArray = *pFormatArray;
	pVertexFormat->m_uiVertexFormatCode = lVertexFormatCode;
	pVertexFormat->LoadResource(VSRenderer::ms_pRenderer);
	return pVertexFormat;

}
VSName * VSResourceManager::CreateName(const char * pChar)
{
	if (!pChar)
	{
		return NULL;
	}
	VSCriticalSection::Locker Temp(ms_NameCri);
	uint32 uiCRCCode  = VSMathInstance::GetMathInstance().CRC32Compute(pChar,(unsigned int)VSStrLen(pChar));
	VSName * pName = NULL;
	pName = VSResourceManager::GetNameSet().CheckIsHaveTheResource(uiCRCCode);
	if (!pName)
	{
		pName = MEM_NEW VSName(pChar,uiCRCCode);
		if (!pName)
		{
			return NULL;
		}

		VSResourceManager::GetNameSet().AddResource(pName->m_uiID,pName);
	}
	return pName;
}
VSName * VSResourceManager::CreateName(const VSString & VSString)
{

	VSCriticalSection::Locker Temp(ms_NameCri);
	uint32 uiCRCCode  = VSMathInstance::GetMathInstance().CRC32Compute(VSString.GetBuffer(),VSString.GetLength());
	VSName * pName = NULL;
	pName = VSResourceManager::GetNameSet().CheckIsHaveTheResource(uiCRCCode);
	if (!pName)
	{
		pName = MEM_NEW VSName(VSString,uiCRCCode);
		if (!pName)
		{
			return NULL;
		}

		VSResourceManager::GetNameSet().AddResource(pName->m_uiID,pName);
	}
	return pName;
}
VSVShader * VSResourceManager::CreateVShader(MaterialShaderPara &MSPara,uint32 uiShaderID)
{
	VSMAC_ASSERT(VSIsMainThread());
	VSMAC_ASSERT(MSPara.pGeometry && MSPara.pMaterialInstance);
	
	bool bCreateShaderString = false;
	VSVShader * pVShader = NULL;
	if (MSPara.VSShaderPath.GetLength())
	{
		if (!MSPara.VMainFunName.GetLength())
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pVShader = MEM_NEW VSVShader(MSPara.VSShaderPath.GetBuffer(),MSPara.VMainFunName.GetBuffer(),true);
		}
	}
	else
	{	
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			return NULL;
		}
		pVShader = MEM_NEW VSVShader();
		bCreateShaderString = true;
	}

	VSMAC_ASSERT(pVShader)
	if (bCreateShaderString)
	{
		VSString VShaderString;
	#if _DEBUG 
		VSString ShaderFileName;
		if(!VSShaderStringFactory::CreateVShaderString(pVShader,MSPara,uiShaderID,VShaderString,ShaderFileName))
	#else
		if(!VSShaderStringFactory::CreateVShaderString(pVShader,MSPara,uiShaderID,VShaderString))
	#endif
		{
			SAFE_DELETE(pVShader);
			return NULL;
		}
	#if _DEBUG 
		pVShader->SetFileName(ShaderFileName);
	#endif
		pVShader->SetShaderString(VShaderString,VSRenderer::GetVShaderProgramMain());
	}
	VSShaderKey::SetMaterialVShaderKey(&pVShader->m_ShaderKey,MSPara);
	VSRenderer::ms_pRenderer->LoadVShaderProgram(pVShader);
	return pVShader;
}
VSHShader * VSResourceManager::CreateHShader(MaterialShaderPara &MSPara, uint32 uiShaderID)
{
	VSMAC_ASSERT(VSIsMainThread());
	VSMAC_ASSERT(MSPara.pGeometry && MSPara.pMaterialInstance);

	bool bCreateShaderString = false;
	VSHShader * pHShader = NULL;
	if (MSPara.HSShaderPath.GetLength())
	{
		if (!MSPara.HMainFunName.GetLength())
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pHShader = MEM_NEW VSHShader(MSPara.HSShaderPath.GetBuffer(), MSPara.HMainFunName.GetBuffer(), true);
		}
	}
	else
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			return NULL;
		}
		pHShader = MEM_NEW VSHShader();
		bCreateShaderString = true;
	}

	VSMAC_ASSERT(pHShader);

	if (bCreateShaderString)
	{
		VSString HShaderString;
#if _DEBUG 
		VSString ShaderFileName;
		if (!VSShaderStringFactory::CreateHShaderString(pHShader, MSPara, uiShaderID, HShaderString, ShaderFileName))
#else
		if (!VSShaderStringFactory::CreateHShaderString(pHShader, MSPara, uiShaderID, HShaderString))
#endif
		{
			SAFE_DELETE(pHShader);
			return NULL;
		}
#if _DEBUG 
		pHShader->SetFileName(ShaderFileName);
#endif
		pHShader->SetShaderString(HShaderString, VSRenderer::GetHShaderProgramMain());
	}


	VSShaderKey::SetMaterialHShaderKey(&pHShader->m_ShaderKey, MSPara);
	VSRenderer::ms_pRenderer->LoadHShaderProgram(pHShader);
	return pHShader;
}
VSDShader * VSResourceManager::CreateDShader(MaterialShaderPara &MSPara, uint32 uiShaderID)
{
	VSMAC_ASSERT(VSIsMainThread());
	VSMAC_ASSERT(MSPara.pGeometry && MSPara.pMaterialInstance);

	bool bCreateShaderString = false;
	VSDShader * pDShader = NULL;
	if (MSPara.DSShaderPath.GetLength())
	{
		if (!MSPara.DMainFunName.GetLength())
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pDShader = MEM_NEW VSDShader(MSPara.DSShaderPath.GetBuffer(), MSPara.DMainFunName.GetBuffer(), true);
		}
	}
	else
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			return NULL;
		}
		pDShader = MEM_NEW VSDShader();
		bCreateShaderString = true;
	}

	VSMAC_ASSERT(pDShader);


	if (bCreateShaderString)
	{
		VSString DShaderString;
#if _DEBUG 
		VSString ShaderFileName;
		if (!VSShaderStringFactory::CreateDShaderString(pDShader, MSPara, uiShaderID, DShaderString, ShaderFileName))
#else
		if (!VSShaderStringFactory::CreateDShaderString(pDShader, MSPara, uiShaderID, DShaderString))
#endif
		{
			SAFE_DELETE(pDShader);
			return NULL;
		}
#if _DEBUG 
		pDShader->SetFileName(ShaderFileName);
#endif
		pDShader->SetShaderString(DShaderString, VSRenderer::GetDShaderProgramMain());
	}


	VSShaderKey::SetMaterialDShaderKey(&pDShader->m_ShaderKey, MSPara);
	VSRenderer::ms_pRenderer->LoadDShaderProgram(pDShader);
	return pDShader;
}
VSGShader * VSResourceManager::CreateGShader(MaterialShaderPara &MSPara,uint32 uiShaderID)
{
	VSMAC_ASSERT(VSIsMainThread());
	VSMAC_ASSERT(MSPara.pGeometry && MSPara.pMaterialInstance);

	bool bCreateShaderString = false;
	VSGShader * pGShader = NULL;
	if (MSPara.GSShaderPath.GetLength())
	{
		if (!MSPara.GMainFunName.GetLength())
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pGShader = MEM_NEW VSGShader(MSPara.GSShaderPath.GetBuffer(), MSPara.GMainFunName.GetBuffer(), true);
		}
	}
	else
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			return NULL;
		}
		pGShader = MEM_NEW VSGShader();
		bCreateShaderString = true;
	}

	VSMAC_ASSERT(pGShader);

	if (bCreateShaderString)
	{
		VSString GShaderString;
#if _DEBUG 
		VSString ShaderFileName;
		if (!VSShaderStringFactory::CreateGShaderString(pGShader, MSPara,uiShaderID, GShaderString, ShaderFileName))
#else
		if (!VSShaderStringFactory::CreateGShaderString(pGShader, MSPara, uiShaderID, GShaderString))
#endif
		{
			SAFE_DELETE(pGShader);
			return NULL;
		}
#if _DEBUG 
		pGShader->SetFileName(ShaderFileName);
#endif
		pGShader->SetShaderString(GShaderString, VSRenderer::GetGShaderProgramMain());
	}


	VSShaderKey::SetMaterialGShaderKey(&pGShader->m_ShaderKey, MSPara);
	VSRenderer::ms_pRenderer->LoadGShaderProgram(pGShader);
	return pGShader;
}
VSPShader * VSResourceManager::CreatePShader(MaterialShaderPara &MSPara,uint32 uiShaderID)
{
	VSMAC_ASSERT(VSIsMainThread());
	VSMAC_ASSERT(MSPara.pGeometry && MSPara.pMaterialInstance);
	
	bool bCreateShaderString = false;
	VSPShader * pPShader = NULL;
	if (MSPara.PSShaderPath.GetLength())
	{
		if (!MSPara.PMainFunName.GetLength())
		{
			VSMAC_ASSERT(0);
			return NULL;
		}
		else
		{
			pPShader = MEM_NEW VSPShader(MSPara.PSShaderPath.GetBuffer(),MSPara.PMainFunName.GetBuffer(),true);
		}
	}
	else
	{
		VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
		if (!pMaterial)
		{
			return NULL;
		}
		pPShader = MEM_NEW VSPShader();
		bCreateShaderString = true;
	}
	
	VSMAC_ASSERT(pPShader);


	if (bCreateShaderString)
	{
		VSString PShaderString;
#if _DEBUG 
		VSString ShaderFileName;
		if (!VSShaderStringFactory::CreatePShaderString(pPShader,MSPara,uiShaderID,PShaderString,ShaderFileName))
#else
		if (!VSShaderStringFactory::CreatePShaderString(pPShader,MSPara,uiShaderID,PShaderString))
#endif
		{
			SAFE_DELETE(pPShader);
			return NULL;
		}
#if _DEBUG 
		pPShader->SetFileName(ShaderFileName);
#endif
		pPShader->SetShaderString(PShaderString,VSRenderer::GetPShaderProgramMain());
	}
	
	
	VSShaderKey::SetMaterialPShaderKey(&pPShader->m_ShaderKey,MSPara);
	VSRenderer::ms_pRenderer->LoadPShaderProgram(pPShader);
	return pPShader;
}


VSBlendState * VSResourceManager::CreateBlendState(const VSBlendDesc & BlendDesc)
{
	uint32 uiDataSize = 0;
	void * pData = BlendDesc.GetCRC32Data(uiDataSize);
	uint32 uiHashCode = VSMathInstance::GetMathInstance().CRC32Compute(pData, uiDataSize);

	VSBlendState * pBlendState = NULL;
	pBlendState = (VSBlendState *)VSResourceManager::GetBlendStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pBlendState)
	{

		return pBlendState;
	}
	pBlendState = MEM_NEW VSBlendState();
	pBlendState->m_BlendDesc = BlendDesc;
	VSResourceManager::GetBlendStateSet().AddResource(uiHashCode,pBlendState);
	return pBlendState;
}
VSDepthStencilState * VSResourceManager::CreateDepthStencilState(const VSDepthStencilDesc & DepthStencilDesc)
{
	uint32 uiDataSize = 0;
	void * pData = DepthStencilDesc.GetCRC32Data(uiDataSize);
	uint32 uiHashCode = VSMathInstance::GetMathInstance().CRC32Compute(pData, uiDataSize);

	VSDepthStencilState * pDepthStencilState = NULL;
	pDepthStencilState = (VSDepthStencilState *)VSResourceManager::GetDepthStencilStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pDepthStencilState)
	{

		return pDepthStencilState;
	}
	pDepthStencilState = MEM_NEW VSDepthStencilState();
	pDepthStencilState->m_DepthStencilDesc = DepthStencilDesc;
	VSResourceManager::GetDepthStencilStateSet().AddResource(uiHashCode,pDepthStencilState);
	return pDepthStencilState;
}
VSRasterizerState * VSResourceManager::CreateRasterizerState(const VSRasterizerDesc &RasterizerDesc)
{
	uint32 uiDataSize = 0;
	void * pData = RasterizerDesc.GetCRC32Data(uiDataSize);
	uint32 uiHashCode = VSMathInstance::GetMathInstance().CRC32Compute(pData, uiDataSize);

	VSRasterizerState * pRasterizerState = NULL;
	pRasterizerState = (VSRasterizerState *)VSResourceManager::GetRasterizerStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pRasterizerState)
	{

		return pRasterizerState;
	}
	pRasterizerState = MEM_NEW VSRasterizerState();
	pRasterizerState->m_RasterizerDesc = RasterizerDesc;
	VSResourceManager::GetRasterizerStateSet().AddResource(uiHashCode,pRasterizerState);
	return pRasterizerState;
}

VSSamplerState * VSResourceManager::CreateSamplerState(const VSSamplerDesc &SamplerDesc)
{
	uint32 uiDataSize = 0;
	void * pData = SamplerDesc.GetCRC32Data(uiDataSize);
	uint32 uiHashCode = VSMathInstance::GetMathInstance().CRC32Compute(pData, uiDataSize);

	VSSamplerState * pSamplerState = NULL;
	pSamplerState = (VSSamplerState *)VSResourceManager::GetSamplerStateSet().CheckIsHaveTheResource(uiHashCode);
	if(pSamplerState)
	{

		return pSamplerState;
	}
	pSamplerState = MEM_NEW VSSamplerState();
	pSamplerState->m_SamplerDesc = SamplerDesc;
	VSResourceManager::GetSamplerStateSet().AddResource(uiHashCode,pSamplerState);
	return pSamplerState;
}
VSRenderTarget * VSResourceManager::CreateRenderTarget(uint32 uiWidth, uint32 uiHeight,uint32 uiFormatType,
									uint32 uiMulSample, bool CPURead)
{
	if (uiMulSample != VSRenderer::MS_NONE && CPURead && !VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_MSBufferRead))
	{
		return NULL;
	}
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	VSRenderTarget *Temp = MEM_NEW VSRenderTarget(uiWidth,uiHeight,uiFormatType,uiMulSample, CPURead);
	GetRenderTargetArray().AddResource(Temp);
	return Temp;
}
VSUnorderAccess * VSResourceManager::CreateBufferUnorderAccess(VSBufferResource * pCreateBy, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		return NULL;
	}
	VSUnorderAccess *Temp = MEM_NEW VSBufferUnorderAccess(pCreateBy, CPURead);
	GetUnorderAccessArray().AddResource(Temp);
	return Temp;
}
VSDepthStencil * VSResourceManager::CreateDepthStencil(VS2DTexture * pCreateBy, uint32 uiMulSample
	, uint32 uiLevel,bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_DepthStensilToTexture))
	{
		return NULL;
	}
	if (CPURead && !VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_DepthStensilBufferRead))
	{
		return NULL;
	}
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	VSMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}

	VSDepthStencil *Temp = MEM_NEW VSDepthStencil(pCreateBy, uiMulSample, uiLevel, CPURead);
	GetDepthStencilArray().AddResource(Temp);
	return Temp;
}
VSDepthStencil * VSResourceManager::CreateDepthStencil(VS2DTextureArray * pCreateBy, uint32 uiMulSample
	, uint32 uiLevel, uint32 First, uint32 m_uiArraySize, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_DepthStensilToTexture))
	{
		return NULL;
	}
	if (CPURead && !VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_DepthStensilBufferRead))
	{
		return NULL;
	}
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	VSMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}
	if ((First + m_uiArraySize) > pCreateBy->GetArraySize())
	{
		return NULL;
	}
	VSDepthStencil *Temp = MEM_NEW VSDepthStencil(pCreateBy, uiMulSample, uiLevel,First, m_uiArraySize, CPURead);
	GetDepthStencilArray().AddResource(Temp);
	return Temp;
}
VSDepthStencil * VSResourceManager::CreateDepthStencil(VS3DTexture * pCreateBy, uint32 uiMulSample
	, uint32 uiLevel, uint32 First, uint32 m_uiArraySize, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_DepthStensilToTexture))
	{
		return NULL;
	}
	if (CPURead && !VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_DepthStensilBufferRead))
	{
		return NULL;
	}
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)

	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}

	VSMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}
	if ((First + m_uiArraySize) > pCreateBy->GetLength(uiLevel))
	{
		return NULL;
	}
	VSDepthStencil *Temp = MEM_NEW VSDepthStencil(pCreateBy, uiMulSample, uiLevel, First, m_uiArraySize, CPURead);
	GetDepthStencilArray().AddResource(Temp);
	return Temp;
}
VSRenderTarget * VSResourceManager::GetRenderTarget(uint32 uiWidth, uint32 uiHeight,
	uint32 uiFormatType, uint32 uiMulSample, bool TextureUse, bool CPURead)
{
	if (TextureUse)
	{
		return GetRenderTargetTexture(uiWidth, uiHeight, uiFormatType, uiMulSample, CPURead);
	}
	else
	{
		return GetRenderTargetNoTexture(uiWidth, uiHeight, uiFormatType, uiMulSample, CPURead);
	}
}
VSDepthStencil * VSResourceManager::GetDepthStencil(uint32 uiWidth, uint32 uiHeight,
	uint32 uiFormatType, uint32 uiMulSample, bool TextureUse, bool CPURead)
{
	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_DepthStensilToTexture) && TextureUse)
	{
		return GetDepthStencilTexture(uiWidth, uiHeight, uiFormatType, uiMulSample, CPURead);
	}
	else
	{
		return GetDepthStencilNoTexture(uiWidth, uiHeight,uiFormatType,uiMulSample,CPURead);
	}
}
VSUnorderAccess * VSResourceManager::GetBufferUnorderAccess(uint32 uiNum, uint32 uiDT,
	uint32 uiStructStride, bool BufferResource, bool CPURead)
{
	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		if (BufferResource)
		{
			return GetUnorderAccessBuffer(uiNum, uiDT, uiStructStride, CPURead);
		}
		else
		{
			return GetUnorderAccessNoBuffer(uiNum, uiDT, uiStructStride, CPURead);
		}
	}
	else
	{
		return NULL;
	}
}
VSRenderTarget * VSResourceManager::CreateRenderTarget(VS2DTexture * pCreateBy,uint32 uiMulSample
									,uint32 uiLevel,bool CPURead)
{
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	VSMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}
	VSRenderTarget *Temp = MEM_NEW VSRenderTarget(pCreateBy,uiMulSample,uiLevel,CPURead);
	GetRenderTargetArray().AddResource(Temp);
	return Temp;
}
VSRenderTarget * VSResourceManager::CreateRenderTarget(VS2DTextureArray * pCreateBy, uint32 uiMulSample
	, uint32 uiLevel, uint32 First, uint32 m_uiArraySize, bool CPURead)
{
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if(uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	VSMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}
	if ((First + m_uiArraySize) > pCreateBy->GetArraySize())
	{
		return NULL;
	}
	VSRenderTarget *Temp = MEM_NEW VSRenderTarget(pCreateBy, uiMulSample, uiLevel, First, m_uiArraySize,CPURead);
	GetRenderTargetArray().AddResource(Temp);
	return Temp;
}
VSRenderTarget * VSResourceManager::CreateRenderTarget(VS3DTexture * pCreateBy, uint32 uiMulSample
	, uint32 uiLevel, uint32 First, uint32 m_uiArraySize, bool CPURead)
{
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	VSMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}
	if ((First + m_uiArraySize) > pCreateBy->GetLength(uiLevel))
	{
		return NULL;
	}
	VSRenderTarget *Temp = MEM_NEW VSRenderTarget(pCreateBy, uiMulSample, uiLevel, First, m_uiArraySize, CPURead);
	GetRenderTargetArray().AddResource(Temp);
	return Temp;
}
VSUnorderAccess * VSResourceManager::CreateTextureUnorderAccess(uint32 uiWidth, uint32 uiHeight,
	uint32 uiFormatType, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		return NULL;
	}
	VSUnorderAccess *Temp = MEM_NEW VSTextureUnorderAccess(uiWidth, uiHeight,uiFormatType, CPURead);
	GetUnorderAccessArray().AddResource(Temp);
	return Temp;
}
VSUnorderAccess * VSResourceManager::CreateTextureUnorderAccess(VS2DTexture * pCreateBy, uint32 uiLevel, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		return NULL;
	}
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}
	VSMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}

	VSUnorderAccess *Temp = MEM_NEW VSTextureUnorderAccess(pCreateBy, uiLevel, CPURead);
	GetUnorderAccessArray().AddResource(Temp);
	return Temp;
}
VSUnorderAccess * VSResourceManager::CreateTextureUnorderAccess(VS2DTextureArray * pCreateBy, uint32 uiLevel,
	uint32 First, uint32 m_uiArraySize, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		return NULL;
	}
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}
	VSMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}
	if ((First + m_uiArraySize) > pCreateBy->GetArraySize())
	{
		return NULL;
	}
	VSUnorderAccess *Temp = MEM_NEW VSTextureUnorderAccess(pCreateBy,uiLevel, First, m_uiArraySize, CPURead);
	GetUnorderAccessArray().AddResource(Temp);
	return Temp;
}
VSUnorderAccess * VSResourceManager::CreateTextureUnorderAccess(VS3DTexture * pCreateBy, uint32 uiLevel,
	uint32 First, uint32 m_uiArraySize, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		return NULL;
	}
	if (pCreateBy->IsCompress())
	{
		return NULL;
	}

	VSMAC_ASSERT(uiLevel < pCreateBy->GetMipLevel());
	if (uiLevel >= pCreateBy->GetMipLevel())
	{
		return NULL;
	}
	if ((First + m_uiArraySize) > pCreateBy->GetLength(uiLevel))
	{
		return NULL;
	}
	VSUnorderAccess *Temp = MEM_NEW VSTextureUnorderAccess(pCreateBy,uiLevel, First, m_uiArraySize, CPURead);
	GetUnorderAccessArray().AddResource(Temp);
	return Temp;
}
VSDepthStencil * VSResourceManager::CreateDepthStencil(uint32 uiWidth,uint32 uiHeight,
									uint32 uiMulSample,uint32 uiFormatType, bool CPURead)
{
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)

	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	if (CPURead && !VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_DepthStensilBufferRead))
	{
		return NULL;
	}
	VSDepthStencil *Temp = MEM_NEW VSDepthStencil(uiWidth,uiHeight,uiMulSample,uiFormatType, CPURead);
	GetDepthStencilArray().AddResource(Temp);
	return Temp;
}
VSUnorderAccess * VSResourceManager::CreateBufferUnorderAccess(uint32 uiNum, uint32 uiDT,
	uint32 uiStructStride, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		return NULL;
	}
	VSUnorderAccess *Temp = MEM_NEW VSBufferUnorderAccess(uiNum, uiDT, uiStructStride, CPURead);
	GetUnorderAccessArray().AddResource(Temp);
	return Temp;
}
VSRenderTarget * VSResourceManager::GetRenderTargetNoTexture(uint32 uiWidth, uint32 uiHeight,
	uint32 uiFormatType, uint32 uiMulSample, bool CPURead)
{
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	VSResourceArrayControl<VSRenderTargetPtr> & RenderTargetArray = GetRenderTargetBufferArray();
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	for (uint32 i = 0; i < RenderTargetArray.GetResourceNum(); i++)
	{
		VSRenderTarget* pRt = RenderTargetArray.GetResource(i);
		if (pRt->m_bUsed || pRt->GetCreateBy())
		{
			continue;
		}

		if (pRt->m_uiWidth == uiWidth && pRt->m_uiHeight == uiHeight &&
			pRt->m_uiFormatType == uiFormatType && pRt->m_uiMulSample == uiMulSample
			&& pRt->GetLockFlag() == LockFlag)
		{
			pRt->m_bUsed = true;
			RenderTargetArray.ClearTimeCount(i);
			return pRt;
		}

	}

	

	VSRenderTarget *  pNewRt = MEM_NEW VSRenderTarget(uiWidth, uiHeight, uiFormatType,uiMulSample,CPURead);
	GetRenderTargetBufferArray().AddResource(pNewRt);

	pNewRt->m_bUsed = true;
	return pNewRt;

}
VSRenderTarget * VSResourceManager::GetRenderTargetTexture(uint32 uiWidth, uint32 uiHeight,
	uint32 uiFormatType,uint32 uiMulSample, bool CPURead)
{
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	VSResourceArrayControl<VSRenderTargetPtr> & RenderTargetArray = GetRenderTargetBufferArray();
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	for (uint32 i = 0 ; i < RenderTargetArray.GetResourceNum() ; i++)
	{
		VSRenderTarget* pRt = RenderTargetArray.GetResource(i);
		if (pRt->m_bUsed)
		{
			continue;
		}
		if (pRt->GetCreateBy() && ((VSTexture *)pRt->GetCreateBy())->GetTexType() == VSTexture::TT_2D)
		{
			VS2DTexture * p2DTex = (VS2DTexture *)pRt->GetCreateBy();
			if (p2DTex->HasAnySlot())
			{
				continue;
			}
			if (pRt->m_uiWidth == uiWidth && pRt->m_uiHeight == uiHeight && 
				pRt->m_uiFormatType == uiFormatType && pRt->m_uiMulSample == uiMulSample
				&& pRt->GetLockFlag() == LockFlag)
			{
				pRt->m_bUsed = true;
				RenderTargetArray.ClearTimeCount(i);
				return pRt;
			}
		}
	}
	VS2DTexture * pTexture = MEM_NEW VS2DTexture(uiWidth,uiHeight,uiFormatType, 1, true);

	VSRenderTarget *  pNewRt = MEM_NEW VSRenderTarget(pTexture,uiMulSample,0,CPURead);
	GetRenderTargetBufferArray().AddResource(pNewRt);

	pNewRt->m_bUsed = true;
	return pNewRt;

}
VSUnorderAccess * VSResourceManager::GetTextureUnorderAccess(uint32 uiWidth, uint32 uiHeight,
	uint32 uiFormatType, bool TextureUse, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		return NULL;
	}

	if (TextureUse)
	{
		return GetUnorderAccessTexture(uiWidth, uiHeight, uiFormatType, CPURead);
	}
	else
	{
		return GetUnorderAccessNoTexture(uiWidth, uiHeight, uiFormatType, CPURead);
	}

}
bool VSResourceManager::GetCubeUnorderAccess(uint32 uiWidth, uint32 uiFormatType,
	VSUnorderAccess * OutUA[VSCubeTexture::F_MAX], bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		return false;
	}
	VSResourceArrayControl<VSUnorderAccessPtr> & UnorderAccessArray = GetUnorderAccessBufferArray();
	VSCubeTexture * IndexCubTexture = NULL;
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	for (uint32 k = 0; k < VSCubeTexture::F_MAX; k++)
	{
		OutUA[k] = NULL;
	}
	for (uint32 i = 0; i < UnorderAccessArray.GetResourceNum(); i++)
	{
		VSUnorderAccess* pUA = UnorderAccessArray.GetResource(i);
		if (pUA->m_bUsed || pUA->GetOutputType() != VSOutputResource::OT_TEXTURE_UNORDER_ACCESS)
		{
			continue;
		}
		if (pUA->GetCreateBy() && ((VSTexture *)pUA->GetCreateBy())->GetTexType() == VSTexture::TT_CUBE)
		{
			VSCubeTexture * pCubDTex = (VSCubeTexture *)pUA->GetCreateBy();
			if (pCubDTex->HasAnySlot())
			{
				continue;
			}
			VSTextureUnorderAccess* pTUA = (VSTextureUnorderAccess*)pUA;
			if (pTUA->m_uiWidth == uiWidth && pTUA->m_uiFormatType == uiFormatType
				&& pTUA->GetLockFlag() == LockFlag)
			{
				VSMAC_ASSERT(pTUA->m_uiFirst < VSCubeTexture::F_MAX);

				if (!IndexCubTexture)
				{
					IndexCubTexture = pCubDTex;
					if (!OutUA[pTUA->m_uiFirst])
					{
						OutUA[pTUA->m_uiFirst] = pUA;
						UnorderAccessArray.ClearTimeCount(i);
					}
					else
					{
						for (uint32 k = 0; k < VSCubeTexture::F_MAX; k++)
						{
							OutUA[k] = NULL;
						}
						return false;
					}

				}
				else
				{
					if (IndexCubTexture == pCubDTex)
					{
						if (!OutUA[pTUA->m_uiFirst])
						{
							UnorderAccessArray.ClearTimeCount(i);
							OutUA[pTUA->m_uiFirst] = pUA;
						}
						else
						{
							for (uint32 k = 0; k < VSCubeTexture::F_MAX; k++)
							{
								OutUA[k] = NULL;
							}
							return false;
						}
					}
				}


			}
		}
	}
	uint32 uiFindNum = 0;
	for (uint32 k = 0; k < VSCubeTexture::F_MAX; k++)
	{
		if (OutUA[k])
		{
			uiFindNum++;
		}
	}

	if (uiFindNum == 0)
	{
		VSCubeTexture *pCubTexture = MEM_NEW VSCubeTexture(uiWidth, uiFormatType, 1, true);
		for (uint32 i = 0; i < VSCubeTexture::F_MAX; i++)
		{
			OutUA[i] = MEM_NEW VSTextureUnorderAccess(pCubTexture, 0, i, CPURead);
			UnorderAccessArray.AddResource(OutUA[i]);
			OutUA[i]->m_bUsed = true;
		}
		return true;
	}
	else if (uiFindNum == VSCubeTexture::F_MAX)
	{
		for (uint32 i = 0; i < VSCubeTexture::F_MAX; i++)
		{
			OutUA[i]->m_bUsed = true;
		}
		return true;
	}
	else // GC will delete some ,so no longer  use any rt
	{
		//set other rt used , so you cat't find it
		for (uint32 i = 0; i < VSCubeTexture::F_MAX; i++)
		{
			if (OutUA[i])
			{
				OutUA[i]->m_bUsed = true;
			}
		}

		// create new one
		VSCubeTexture *pCubTexture = MEM_NEW VSCubeTexture(uiWidth, uiFormatType, 1, true);
		for (uint32 i = 0; i < VSCubeTexture::F_MAX; i++)
		{
			OutUA[i] = MEM_NEW VSTextureUnorderAccess(pCubTexture, 0, i, CPURead);
			UnorderAccessArray.AddResource(OutUA[i]);
			OutUA[i]->m_bUsed = true;
		}
		return true;
	}
}
bool VSResourceManager::Get2DTextureArrayUnorderAccess(uint32 uiWidth, uint32 uiHeight, uint32 uiFormatType,
	VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSUnorderAccess *> &UAArray, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		return false;
	}
	if (!CheckTextureBindInfo(TextureBindInfo))
	{
		return false;
	}
	uint32 uiArraySize = TextureBindInfo[TextureBindInfo.GetNum() - 1].uiFirst +
		TextureBindInfo[TextureBindInfo.GetNum() - 1].uiArraySize;
	UAArray.Clear();
	VSResourceArrayControl<VSUnorderAccessPtr> & UnorderAccessArray = GetUnorderAccessBufferArray();
	VS2DTextureArray * Index2DTextureArray = NULL;
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;

	for (uint32 i = 0; i < UnorderAccessArray.GetResourceNum(); i++)
	{
		VSUnorderAccess* pUA = UnorderAccessArray.GetResource(i);
		if (pUA->m_bUsed || pUA->GetOutputType() != VSOutputResource::OT_TEXTURE_UNORDER_ACCESS)
		{
			continue;
		}
		if (pUA->GetCreateBy() && ((VSTexture *)pUA->GetCreateBy())->GetTexType() == VSTexture::TT_2DARRAY)
		{
			VS2DTextureArray * p2DTextureArray = (VS2DTextureArray *)pUA->GetCreateBy();
			if (p2DTextureArray->HasAnySlot() || p2DTextureArray->GetArraySize() != uiArraySize)
			{
				continue;
			}
			VSTextureUnorderAccess* pTUA = (VSTextureUnorderAccess*)pUA;
			if (pTUA->m_uiWidth == uiWidth && pTUA->m_uiFormatType == uiFormatType && pTUA->m_uiHeight == uiHeight
				&& pTUA->GetLockFlag() == LockFlag)
			{

				if (!Index2DTextureArray)
				{
					if (CheckTextureBindInfo(TextureBindInfo, pTUA))
					{
						Index2DTextureArray = p2DTextureArray;
						UAArray.AddElement(pTUA);
						UnorderAccessArray.ClearTimeCount(i);
					}
				}
				else
				{
					if (Index2DTextureArray == p2DTextureArray)
					{
						if (CheckTextureBindInfo(TextureBindInfo, pTUA))
						{
							UAArray.AddElement(pTUA);
							UnorderAccessArray.ClearTimeCount(i);
						}
						else
						{
							Index2DTextureArray = NULL;
						}
					}
				}
			}
		}
	}
	if (UAArray.GetNum() == TextureBindInfo.GetNum())
	{
		return true;
	}
	else
	{
		UAArray.Clear();
		VS2DTextureArray *p2DTextureArray = MEM_NEW VS2DTextureArray(uiWidth, uiHeight, uiFormatType, 1, uiArraySize, true);
		for (uint32 i = 0; i < TextureBindInfo.GetNum(); i++)
		{
			VSUnorderAccess* pUnorderAccess =
				MEM_NEW VSTextureUnorderAccess(p2DTextureArray,0, TextureBindInfo[i].uiFirst, TextureBindInfo[i].uiArraySize, CPURead);
			UnorderAccessArray.AddResource(pUnorderAccess);
			pUnorderAccess->m_bUsed = true;
			UAArray.AddElement(pUnorderAccess);
		}
		return true;
	}
}
bool VSResourceManager::Get3DTextureUnorderAccess(uint32 uiWidth, uint32 uiHeight, uint32 uiFormatType,
	 VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSUnorderAccess *> &UAArray, bool CPURead)
{
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		return false;
	}
	if (!CheckTextureBindInfo(TextureBindInfo))
	{
		return false;
	}
	uint32 uiArraySize = TextureBindInfo[TextureBindInfo.GetNum() - 1].uiFirst +
		TextureBindInfo[TextureBindInfo.GetNum() - 1].uiArraySize;
	UAArray.Clear();
	VSResourceArrayControl<VSUnorderAccessPtr> & UnorderAccessArray = GetUnorderAccessBufferArray();
	VS3DTexture * Index3DTexture = NULL;
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;

	for (uint32 i = 0; i < UnorderAccessArray.GetResourceNum(); i++)
	{
		VSUnorderAccess* pUA = UnorderAccessArray.GetResource(i);
		if (pUA->m_bUsed || pUA->GetOutputType() != VSOutputResource::OT_TEXTURE_UNORDER_ACCESS)
		{
			continue;
		}
		if (pUA->GetCreateBy() && ((VSTexture *)pUA->GetCreateBy())->GetTexType() == VSTexture::TT_3D)
		{
			VS3DTexture * p3DTexture = (VS3DTexture *)pUA->GetCreateBy();
			if (p3DTexture->HasAnySlot() || p3DTexture->GetLength(0) != uiArraySize)
			{
				continue;
			}
			VSTextureUnorderAccess* pTUA = (VSTextureUnorderAccess*)pUA;
			if (pTUA->m_uiWidth == uiWidth && pTUA->m_uiFormatType == uiFormatType && pTUA->m_uiHeight == uiHeight
				&& pTUA->GetLockFlag() == LockFlag)
			{

				if (!Index3DTexture)
				{
					if (CheckTextureBindInfo(TextureBindInfo, pTUA))
					{
						Index3DTexture = p3DTexture;
						UAArray.AddElement(pTUA);
						UnorderAccessArray.ClearTimeCount(i);
					}
				}
				else
				{
					if (Index3DTexture == p3DTexture)
					{
						if (CheckTextureBindInfo(TextureBindInfo, pTUA))
						{
							UAArray.AddElement(pTUA);
							UnorderAccessArray.ClearTimeCount(i);
						}
						else
						{
							Index3DTexture = NULL;
						}
					}
				}
			}
		}
	}

	if (UAArray.GetNum() == TextureBindInfo.GetNum())
	{
		return true;
	}
	else
	{
		UAArray.Clear();
		VS3DTexture *p3DTexture = MEM_NEW VS3DTexture(uiWidth, uiHeight, uiArraySize, uiFormatType, 1, true);
		for (uint32 i = 0; i < TextureBindInfo.GetNum(); i++)
		{
			VSUnorderAccess* pUnorderAccess =
				MEM_NEW VSTextureUnorderAccess(p3DTexture, 0, TextureBindInfo[i].uiFirst, TextureBindInfo[i].uiArraySize, CPURead);
			UnorderAccessArray.AddResource(pUnorderAccess);
			pUnorderAccess->m_bUsed = true;
			UAArray.AddElement(pUnorderAccess);
		}
		return true;
	}
}
bool VSResourceManager::GetCubDepthStencil(uint32 uiWidth, uint32 uiFormatType,
	uint32 uiMulSample, VSDepthStencil * OutDS[VSCubeTexture::F_MAX], bool CPURead)
{
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_DepthStensilToTexture))
	{
		return NULL;
	}
	VSResourceArrayControl<VSDepthStencilPtr> & DepthStencilArray = GetDepthStencilBufferArray();
	VSCubeTexture * IndexCubTexture = NULL;
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	for (uint32 k = 0; k < VSCubeTexture::F_MAX; k++)
	{
		OutDS[k] = NULL;
	}
	for (uint32 i = 0; i < DepthStencilArray.GetResourceNum(); i++)
	{
		VSDepthStencil* pDS = DepthStencilArray.GetResource(i);
		if (pDS->m_bUsed)
		{
			continue;
		}
		if (pDS->GetCreateBy() && ((VSTexture *)pDS->GetCreateBy())->GetTexType() == VSTexture::TT_CUBE)
		{
			VSCubeTexture * pCubDTex = (VSCubeTexture *)pDS->GetCreateBy();
			if (pCubDTex->HasAnySlot())
			{
				continue;
			}
			if (pDS->m_uiWidth == uiWidth && pDS->m_uiFormatType == uiFormatType
				&& pDS->m_uiMulSample == uiMulSample && pDS->GetLockFlag() == LockFlag)
			{
				VSMAC_ASSERT(pDS->m_uiFirst < VSCubeTexture::F_MAX);

				if (!IndexCubTexture)
				{
					IndexCubTexture = pCubDTex;
					if (!OutDS[pDS->m_uiFirst])
					{
						OutDS[pDS->m_uiFirst] = pDS;
						DepthStencilArray.ClearTimeCount(i);
					}
					else
					{
						for (uint32 k = 0; k < VSCubeTexture::F_MAX; k++)
						{
							OutDS[k] = NULL;
						}
						return false;
					}

				}
				else
				{
					if (IndexCubTexture == pCubDTex)
					{
						if (!OutDS[pDS->m_uiFirst])
						{
							DepthStencilArray.ClearTimeCount(i);
							OutDS[pDS->m_uiFirst] = pDS;
						}
						else
						{
							for (uint32 k = 0; k < VSCubeTexture::F_MAX; k++)
							{
								OutDS[k] = NULL;
							}
							return false;
						}
					}
				}


			}
		}
	}
	uint32 uiFindNum = 0;
	for (uint32 k = 0; k < VSCubeTexture::F_MAX; k++)
	{
		if (OutDS[k])
		{
			uiFindNum++;
		}
	}

	if (uiFindNum == 0)
	{
		VSCubeTexture *pCubTexture = MEM_NEW VSCubeTexture(uiWidth, uiFormatType, 1, true);
		for (uint32 i = 0; i < VSCubeTexture::F_MAX; i++)
		{
			OutDS[i] = MEM_NEW VSDepthStencil(pCubTexture, uiMulSample, 0, i, CPURead);
			DepthStencilArray.AddResource(OutDS[i]);
			OutDS[i]->m_bUsed = true;
		}
		return true;
	}
	else if (uiFindNum == VSCubeTexture::F_MAX)
	{
		for (uint32 i = 0; i < VSCubeTexture::F_MAX; i++)
		{
			OutDS[i]->m_bUsed = true;
		}
		return true;
	}
	else // GC will delete some ,so no longer  use any rt
	{
		//set other rt used , so you cat't find it
		for (uint32 i = 0; i < VSCubeTexture::F_MAX; i++)
		{
			if (OutDS[i])
			{
				OutDS[i]->m_bUsed = true;
			}
		}

		// create new one
		VSCubeTexture *pCubTexture = MEM_NEW VSCubeTexture(uiWidth, uiFormatType, 1, true);
		for (uint32 i = 0; i < VSCubeTexture::F_MAX; i++)
		{
			OutDS[i] = MEM_NEW VSDepthStencil(pCubTexture, uiMulSample, 0, i, CPURead);
			DepthStencilArray.AddResource(OutDS[i]);
			OutDS[i]->m_bUsed = true;
		}
		return true;
	}
}
bool VSResourceManager::CheckTextureBindInfo(VSArray<TextureBindInfoType> & TextureBindInfo)
{
	if (!TextureBindInfo.GetNum())
	{
		return false;
	}
	uint32 uiFirst = 0;
	for (uint32 i = 0 ; i < TextureBindInfo.GetNum() ; i++)
	{
		if (uiFirst == TextureBindInfo[i].uiFirst)
		{
			uiFirst = TextureBindInfo[i].uiFirst + TextureBindInfo[i].uiArraySize;
		}
		else
		{
			return false;
		}
	}
	return false;
}
bool VSResourceManager::CheckTextureBindInfo(VSArray<TextureBindInfoType> & TextureBindInfo, VSTextureOutputInfo * pTextureOutputInfo)
{
	for (uint32 i = 0; i < TextureBindInfo.GetNum(); i++)
	{
		if (pTextureOutputInfo->GetFirst() == TextureBindInfo[i].uiFirst && pTextureOutputInfo->GetArraySize() == TextureBindInfo[i].uiArraySize)
		{
			return true;
		}
		else
		{
			continue;
		}
	}
	return false;
}
bool VSResourceManager::Get2DTextureArrayDepthStencil(uint32 uiWidth, uint32 uiHeight, uint32 uiFormatType,
	uint32 uiMulSample, VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSDepthStencil *> &DSArray, bool CPURead)
{
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return false;
	}
	if (!CheckTextureBindInfo(TextureBindInfo))
	{
		return false;
	}
	uint32 uiArraySize = TextureBindInfo[TextureBindInfo.GetNum() - 1].uiFirst +
		TextureBindInfo[TextureBindInfo.GetNum() - 1].uiArraySize;
	DSArray.Clear();
	VSResourceArrayControl<VSDepthStencilPtr> & DepthStencilArray = GetDepthStencilBufferArray();
	VS2DTextureArray * Index2DTextureArray = NULL;
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;

	for (uint32 i = 0; i < DepthStencilArray.GetResourceNum(); i++)
	{
		VSDepthStencil* pDS = DepthStencilArray.GetResource(i);
		if (pDS->m_bUsed)
		{
			continue;
		}
		if (pDS->GetCreateBy() && ((VSTexture *)pDS->GetCreateBy())->GetTexType() == VSTexture::TT_2DARRAY)
		{
			VS2DTextureArray * p2DTextureArray = (VS2DTextureArray *)pDS->GetCreateBy();
			if (p2DTextureArray->HasAnySlot() || p2DTextureArray->GetArraySize() != uiArraySize)
			{
				continue;
			}
			if (pDS->m_uiWidth == uiWidth && pDS->m_uiFormatType == uiFormatType && pDS->m_uiHeight == uiHeight
				&& pDS->m_uiMulSample == uiMulSample && pDS->GetLockFlag() == LockFlag)
			{

				if (!Index2DTextureArray)
				{
					if (CheckTextureBindInfo(TextureBindInfo, pDS))
					{
						Index2DTextureArray = p2DTextureArray;
						DSArray.AddElement(pDS);
						DepthStencilArray.ClearTimeCount(i);
					}
				}
				else
				{
					if (Index2DTextureArray == p2DTextureArray)
					{
						if (CheckTextureBindInfo(TextureBindInfo, pDS))
						{
							DSArray.AddElement(pDS);
							DepthStencilArray.ClearTimeCount(i);
						}
						else
						{
							Index2DTextureArray = NULL;
						}
					}
				}
			}
		}
	}
	if (DSArray.GetNum() == TextureBindInfo.GetNum())
	{
		return true;
	}
	else
	{
		DSArray.Clear();
		VS2DTextureArray *p2DTextureArray = MEM_NEW VS2DTextureArray(uiWidth, uiHeight, uiFormatType, 1, uiArraySize, true);
		for (uint32 i = 0; i < TextureBindInfo.GetNum(); i++)
		{
			VSDepthStencil* pRenderTarget =
				MEM_NEW VSDepthStencil(p2DTextureArray, uiMulSample, 0, TextureBindInfo[i].uiFirst, TextureBindInfo[i].uiArraySize, CPURead);
			DepthStencilArray.AddResource(pRenderTarget);
			pRenderTarget->m_bUsed = true;
			DSArray.AddElement(pRenderTarget);
		}
		return true;
	}
}
bool VSResourceManager::Get3DTextureDepthStencil(uint32 uiWidth, uint32 uiHeight, uint32 uiFormatType,
	uint32 uiMulSample, VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSDepthStencil *> &DSArray, bool CPURead)
{


	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return false;
	}
	if (!CheckTextureBindInfo(TextureBindInfo))
	{
		return false;
	}
	uint32 uiArraySize = TextureBindInfo[TextureBindInfo.GetNum() - 1].uiFirst +
		TextureBindInfo[TextureBindInfo.GetNum() - 1].uiArraySize;
	DSArray.Clear();
	VSResourceArrayControl<VSDepthStencilPtr> & DepthStencilArray = GetDepthStencilBufferArray();
	VS3DTexture * Index3DTexture = NULL;
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;

	for (uint32 i = 0; i < DepthStencilArray.GetResourceNum(); i++)
	{
		VSDepthStencil* pDS = DepthStencilArray.GetResource(i);
		if (pDS->m_bUsed)
		{
			continue;
		}
		if (pDS->GetCreateBy() && ((VSTexture *)pDS->GetCreateBy())->GetTexType() == VSTexture::TT_3D)
		{
			VS3DTexture * p3DTexture = (VS3DTexture *)pDS->GetCreateBy();
			if (p3DTexture->HasAnySlot() || p3DTexture->GetLength(0) != uiArraySize)
			{
				continue;
			}
			if (pDS->m_uiWidth == uiWidth && pDS->m_uiFormatType == uiFormatType && pDS->m_uiHeight == uiHeight
				&& pDS->m_uiMulSample == uiMulSample && pDS->GetLockFlag() == LockFlag)
			{

				if (!Index3DTexture)
				{
					if (CheckTextureBindInfo(TextureBindInfo, pDS))
					{
						Index3DTexture = p3DTexture;
						DSArray.AddElement(pDS);
						DepthStencilArray.ClearTimeCount(i);
					}
				}
				else
				{
					if (Index3DTexture == p3DTexture)
					{
						if (CheckTextureBindInfo(TextureBindInfo, pDS))
						{
							DSArray.AddElement(pDS);
							DepthStencilArray.ClearTimeCount(i);
						}
						else
						{
							Index3DTexture = NULL;
						}
					}
				}
			}
		}
	}

	if (DSArray.GetNum() == TextureBindInfo.GetNum())
	{
		return true;
	}
	else
	{
		DSArray.Clear();
		VS3DTexture *p3DTexture = MEM_NEW VS3DTexture(uiWidth, uiHeight, uiArraySize, uiFormatType, 1, true);
		for (uint32 i = 0; i < TextureBindInfo.GetNum(); i++)
		{
			VSDepthStencil* pDepthStencil =
				MEM_NEW VSDepthStencil(p3DTexture, uiMulSample, 0, TextureBindInfo[i].uiFirst, TextureBindInfo[i].uiArraySize, CPURead);
			DepthStencilArray.AddResource(pDepthStencil);
			pDepthStencil->m_bUsed = true;
			DSArray.AddElement(pDepthStencil);
		}
		return true;
	}
}
bool VSResourceManager::Get3DTextureRenderTarget(uint32 uiWidth, uint32 uiHeight, uint32 uiFormatType,
	uint32 uiMulSample, VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSRenderTarget *> &RTArray, bool CPURead)
{
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return false;
	}
	if (!CheckTextureBindInfo(TextureBindInfo))
	{
		return false;
	}
	uint32 uiArraySize = TextureBindInfo[TextureBindInfo.GetNum() - 1].uiFirst +
		TextureBindInfo[TextureBindInfo.GetNum() - 1].uiArraySize;
	RTArray.Clear();
	VSResourceArrayControl<VSRenderTargetPtr> & RenderTargetArray = GetRenderTargetBufferArray();
	VS3DTexture * Index3DTexture = NULL;
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;

	for (uint32 i = 0; i < RenderTargetArray.GetResourceNum(); i++)
	{
		VSRenderTarget* pRt = RenderTargetArray.GetResource(i);
		if (pRt->m_bUsed)
		{
			continue;
		}
		if (pRt->GetCreateBy() && ((VSTexture *)pRt->GetCreateBy())->GetTexType() == VSTexture::TT_3D)
		{
			VS3DTexture * p3DTexture = (VS3DTexture *)pRt->GetCreateBy();
			if (p3DTexture->HasAnySlot() || p3DTexture->GetLength(0) != uiArraySize)
			{
				continue;
			}
			if (pRt->m_uiWidth == uiWidth && pRt->m_uiFormatType == uiFormatType && pRt->m_uiHeight == uiHeight
				&& pRt->m_uiMulSample == uiMulSample && pRt->GetLockFlag() == LockFlag)
			{

				if (!Index3DTexture)
				{
					if (CheckTextureBindInfo(TextureBindInfo, pRt))
					{
						Index3DTexture = p3DTexture;
						RTArray.AddElement(pRt);
						RenderTargetArray.ClearTimeCount(i);
					}
				}
				else
				{
					if (Index3DTexture == p3DTexture)
					{
						if (CheckTextureBindInfo(TextureBindInfo, pRt))
						{
							RTArray.AddElement(pRt);
							RenderTargetArray.ClearTimeCount(i);
						}
						else
						{
							Index3DTexture = NULL;
						}
					}
				}
			}
		}
	}

	if (RTArray.GetNum() == TextureBindInfo.GetNum())
	{
		return true;
	}
	else
	{
		RTArray.Clear();
		VS3DTexture *p3DTexture = MEM_NEW VS3DTexture(uiWidth, uiHeight, uiArraySize, uiFormatType, 1, true);
		for (uint32 i = 0; i < TextureBindInfo.GetNum(); i++)
		{
			VSRenderTarget* pRenderTarget =
				MEM_NEW VSRenderTarget(p3DTexture, uiMulSample, 0, TextureBindInfo[i].uiFirst, TextureBindInfo[i].uiArraySize, CPURead);
			RenderTargetArray.AddResource(pRenderTarget);
			pRenderTarget->m_bUsed = true;
			RTArray.AddElement(pRenderTarget);
		}
		return true;
	}
}
bool VSResourceManager::Get2DTextureArrayRenderTarget(uint32 uiWidth, uint32 uiHeight,uint32 uiFormatType,
	uint32 uiMulSample, VSArray<TextureBindInfoType> & TextureBindInfo, VSArray<VSRenderTarget *> &RTArray, bool CPURead)
{
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return false;
	}
	if (!CheckTextureBindInfo(TextureBindInfo))
	{
		return false;
	}
	uint32 uiArraySize = TextureBindInfo[TextureBindInfo.GetNum() - 1].uiFirst +
		TextureBindInfo[TextureBindInfo.GetNum() - 1].uiArraySize;
	RTArray.Clear();
	VSResourceArrayControl<VSRenderTargetPtr> & RenderTargetArray = GetRenderTargetBufferArray();
	VS2DTextureArray * Index2DTextureArray = NULL;
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;

	for (uint32 i = 0; i < RenderTargetArray.GetResourceNum(); i++)
	{
		VSRenderTarget* pRt = RenderTargetArray.GetResource(i);
		if (pRt->m_bUsed)
		{
			continue;
		}
		if (pRt->GetCreateBy() && ((VSTexture *)pRt->GetCreateBy())->GetTexType() == VSTexture::TT_2DARRAY)
		{
			VS2DTextureArray * p2DTextureArray = (VS2DTextureArray *)pRt->GetCreateBy();
			if (p2DTextureArray->HasAnySlot() || p2DTextureArray->GetArraySize() != uiArraySize)
			{
				continue;
			}
			if (pRt->m_uiWidth == uiWidth && pRt->m_uiFormatType == uiFormatType && pRt->m_uiHeight == uiHeight
				&& pRt->m_uiMulSample == uiMulSample && pRt->GetLockFlag() == LockFlag)
			{

				if (!Index2DTextureArray)
				{
					if (CheckTextureBindInfo(TextureBindInfo,pRt))
					{
						Index2DTextureArray = p2DTextureArray;
						RTArray.AddElement(pRt);
						RenderTargetArray.ClearTimeCount(i);
					}
				}
				else
				{
					if (Index2DTextureArray == p2DTextureArray)
					{
						if (CheckTextureBindInfo(TextureBindInfo, pRt))
						{
							RTArray.AddElement(pRt);
							RenderTargetArray.ClearTimeCount(i);
						}
						else
						{
							Index2DTextureArray = NULL;
						}
					}
				}
			}
		}
	}
	if (RTArray.GetNum() == TextureBindInfo.GetNum())
	{
		return true;
	}
	else
	{
		RTArray.Clear();
		VS2DTextureArray *p2DTextureArray = MEM_NEW VS2DTextureArray(uiWidth, uiHeight, uiFormatType, 1, uiArraySize, true);
		for (uint32 i = 0; i < TextureBindInfo.GetNum(); i++)
		{		
			VSRenderTarget* pRenderTarget = 
				MEM_NEW VSRenderTarget(p2DTextureArray, uiMulSample, 0, TextureBindInfo[i].uiFirst, TextureBindInfo[i].uiArraySize, CPURead);
			RenderTargetArray.AddResource(pRenderTarget);
			pRenderTarget->m_bUsed = true;
			RTArray.AddElement(pRenderTarget);
		}
		return true;
	}
}
bool VSResourceManager::GetCubRenderTarget(uint32 uiWidth,uint32 uiFormatType,
	uint32 uiMulSample,VSRenderTarget * OutRT[VSCubeTexture::F_MAX], bool CPURead)
{
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}

	VSResourceArrayControl<VSRenderTargetPtr> & RenderTargetArray = GetRenderTargetBufferArray();
	VSCubeTexture * IndexCubTexture = NULL;
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	for(uint32 k = 0 ; k < VSCubeTexture::F_MAX ; k++)
	{
		OutRT[k] = NULL;
	}
	for (uint32 i = 0 ; i < RenderTargetArray.GetResourceNum() ; i++)
	{
		VSRenderTarget* pRt = RenderTargetArray.GetResource(i);
		if (pRt->m_bUsed)
		{
			continue;
		}
		if (pRt->GetCreateBy() && ((VSTexture *)pRt->GetCreateBy())->GetTexType() == VSTexture::TT_CUBE)
		{
			VSCubeTexture * pCubDTex = (VSCubeTexture *)pRt->GetCreateBy();
			if (pCubDTex->HasAnySlot())
			{
				continue;
			}
			if (pRt->m_uiWidth == uiWidth  && pRt->m_uiFormatType == uiFormatType 
				&& pRt->m_uiMulSample == uiMulSample && pRt->GetLockFlag() == LockFlag)
			{
				VSMAC_ASSERT(pRt->m_uiFirst < VSCubeTexture::F_MAX);

				if (!IndexCubTexture)
				{
					IndexCubTexture = pCubDTex;
					if (!OutRT[pRt->m_uiFirst])
					{
						OutRT[pRt->m_uiFirst] = pRt;
						RenderTargetArray.ClearTimeCount(i);
					}
					else
					{
						for(uint32 k = 0 ; k < VSCubeTexture::F_MAX ; k++)
						{
							OutRT[k] = NULL;
						}
						return false;
					}
					
				}
				else
				{
					if (IndexCubTexture == pCubDTex)
					{
						if (!OutRT[pRt->m_uiFirst])
						{
							RenderTargetArray.ClearTimeCount(i);
							OutRT[pRt->m_uiFirst] = pRt;
						}
						else
						{
							for(uint32 k = 0 ; k < VSCubeTexture::F_MAX ; k++)
							{
								OutRT[k] = NULL;
							}
							return false;
						}
					}
				}
				
				
			}
		}
	}
	uint32 uiFindNum = 0;
	for(uint32 k = 0 ; k < VSCubeTexture::F_MAX ; k++)
	{
		if (OutRT[k])
		{
			uiFindNum++;
		}
	}

	if (uiFindNum == 0)
	{
		VSCubeTexture *pCubTexture = MEM_NEW VSCubeTexture(uiWidth,uiFormatType, 1, true);
		for (uint32 i = 0 ; i < VSCubeTexture::F_MAX ; i++)
		{
			OutRT[i] = MEM_NEW VSRenderTarget(pCubTexture,uiMulSample,0,i,1, CPURead);
			RenderTargetArray.AddResource(OutRT[i]);
			OutRT[i]->m_bUsed = true;
		}
		return true;
	}
	else if (uiFindNum == VSCubeTexture::F_MAX)
	{
		for (uint32 i = 0 ; i < VSCubeTexture::F_MAX ; i++)
		{
			OutRT[i]->m_bUsed = true;
		}
		return true;
	}
	else // GC will delete some ,so no longer  use any rt
	{
		//set other rt used , so you cat't find it
		for (uint32 i = 0 ; i < VSCubeTexture::F_MAX ; i++)
		{
			if (OutRT[i])
			{
				OutRT[i]->m_bUsed = true;
			}	
		}

		// create new one
		VSCubeTexture *pCubTexture = MEM_NEW VSCubeTexture(uiWidth,uiFormatType, 1, true);
		for (uint32 i = 0 ; i < VSCubeTexture::F_MAX ; i++)
		{
			OutRT[i] = MEM_NEW VSRenderTarget(pCubTexture,uiMulSample,0,i,1,CPURead);
			RenderTargetArray.AddResource(OutRT[i]);
			OutRT[i]->m_bUsed = true;
		}
		return true;
	}
}
VSDepthStencil * VSResourceManager::GetDepthStencilTexture(uint32 uiWidth, uint32 uiHeight,
	uint32 uiFormatType, uint32 uiMulSample, bool CPURead)
{
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	VSResourceArrayControl<VSDepthStencilPtr> & DepthStencilArray = GetDepthStencilBufferArray();
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	for (uint32 i = 0; i < DepthStencilArray.GetResourceNum(); i++)
	{
		VSDepthStencil* pDS = DepthStencilArray.GetResource(i);
		if (pDS->m_bUsed)
		{
			continue;
		}
		if (pDS->GetCreateBy() && ((VSTexture *)pDS->GetCreateBy())->GetTexType() == VSTexture::TT_2D)
		{
			VS2DTexture * p2DTex = (VS2DTexture *)pDS->GetCreateBy();
			if (p2DTex->HasAnySlot())
			{
				continue;
			}
			if (pDS->m_uiWidth == uiWidth && pDS->m_uiHeight == uiHeight &&
				pDS->m_uiFormatType == uiFormatType && pDS->m_uiMulSample == uiMulSample
				&& pDS->GetLockFlag() == LockFlag)
			{
				pDS->m_bUsed = true;
				DepthStencilArray.ClearTimeCount(i);
				return pDS;
			}
		}
	}
	VS2DTexture * pTexture = MEM_NEW VS2DTexture(uiWidth, uiHeight, uiFormatType, 1, true);

	

	VSDepthStencil *  pNewDS = MEM_NEW VSDepthStencil(pTexture, uiMulSample, 0, CPURead);
	GetDepthStencilBufferArray().AddResource(pNewDS);

	pNewDS->m_bUsed = true;
	return pNewDS;
}
VSUnorderAccess * VSResourceManager::GetUnorderAccessBuffer(uint32 uiNum, uint32 uiDT,
	uint32 uiStructStride, bool CPURead)
{
	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		VSResourceArrayControl<VSUnorderAccessPtr> & UnorderAccessArray = GetUnorderAccessBufferArray();
		uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
		for (uint32 i = 0; i < UnorderAccessArray.GetResourceNum(); i++)
		{
			VSUnorderAccess* pUnorderAccess = UnorderAccessArray.GetResource(i);
			if (pUnorderAccess->GetOutputType() != VSOutputResource::OT_BUFFER_UNORDER_ACCESS)
			{
				continue;
			}
			VSBufferUnorderAccess * pBufferUnorderAccess = (VSBufferUnorderAccess *)pUnorderAccess;
			VSBufferResource * pBufferResource = (VSBufferResource *)pUnorderAccess->GetCreateBy();
			if (pBufferUnorderAccess->m_bUsed || !pBufferResource)
			{
				continue;;
			}
			if (pBufferResource->HasAnySlot())
			{
				continue;
			}
			if (pBufferUnorderAccess->m_uiNum == uiNum && pBufferUnorderAccess->m_uiDT == uiDT &&
				pBufferUnorderAccess->m_uiStructStride == uiStructStride &&
				pBufferUnorderAccess->GetLockFlag() == LockFlag)
			{
				pBufferUnorderAccess->m_bUsed = true;
				UnorderAccessArray.ClearTimeCount(i);
				return pBufferUnorderAccess;
			}

		}
		VSBufferResource * pNewBufferResource = MEM_NEW VSBufferResource(uiNum, uiDT, uiStructStride);

		VSUnorderAccess* pNewUnorderAccess = MEM_NEW VSBufferUnorderAccess(pNewBufferResource,CPURead);
		GetUnorderAccessBufferArray().AddResource(pNewUnorderAccess);
		pNewUnorderAccess->m_bUsed = true;
		return pNewUnorderAccess;
	}
	else
	{
		return NULL;
	}
}
VSUnorderAccess * VSResourceManager::GetUnorderAccessNoBuffer(uint32 uiNum, uint32 uiDT,
	uint32 uiStructStride, bool CPURead)
{
	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_UnorderAccess))
	{
		VSResourceArrayControl<VSUnorderAccessPtr> & UnorderAccessArray = GetUnorderAccessBufferArray();
		uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
		for (uint32 i = 0; i < UnorderAccessArray.GetResourceNum(); i++)
		{
			VSUnorderAccess* pUnorderAccess = UnorderAccessArray.GetResource(i);
			if (pUnorderAccess->GetOutputType() != VSOutputResource::OT_BUFFER_UNORDER_ACCESS)
			{
				continue;
			}
			VSBufferUnorderAccess * pBufferUnorderAccess = (VSBufferUnorderAccess *)pUnorderAccess;
			if (pBufferUnorderAccess->m_bUsed || pBufferUnorderAccess->GetCreateBy())
			{
				continue;;
			}
			if (pBufferUnorderAccess->m_uiNum == uiNum && pBufferUnorderAccess->m_uiDT == uiDT &&
				pBufferUnorderAccess->m_uiStructStride == uiStructStride &&
				pBufferUnorderAccess->GetLockFlag() == LockFlag)
			{
				pBufferUnorderAccess->m_bUsed = true;
				UnorderAccessArray.ClearTimeCount(i);
				return pBufferUnorderAccess;
			}

		}

		VSUnorderAccess* pNewUnorderAccess = MEM_NEW VSBufferUnorderAccess(uiNum,uiDT,uiStructStride,CPURead);
		GetUnorderAccessBufferArray().AddResource(pNewUnorderAccess);
		pNewUnorderAccess->m_bUsed = true;
		return pNewUnorderAccess;
	}
	else
	{
		return NULL;
	}
}
VSUnorderAccess * VSResourceManager::GetUnorderAccessNoTexture(uint32 uiWidth, uint32 uiHeight,
	uint32 uiFormatType, bool CPURead)
{
	VSResourceArrayControl<VSUnorderAccessPtr> & UnorderAccessArray = GetUnorderAccessBufferArray();
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	for (uint32 i = 0; i < UnorderAccessArray.GetResourceNum(); i++)
	{
		VSUnorderAccess* pUA = UnorderAccessArray.GetResource(i);
		if (pUA->m_bUsed || pUA->GetCreateBy() || pUA->GetOutputType() != VSOutputResource::OT_TEXTURE_UNORDER_ACCESS)
		{
			continue;;
		}
		VSTextureUnorderAccess * pTUA = (VSTextureUnorderAccess *)pUA;
		if (pTUA->m_uiWidth == uiWidth && pTUA->m_uiHeight == uiHeight &&
			pTUA->m_uiFormatType == uiFormatType && pTUA->GetLockFlag() == LockFlag)
		{
			pTUA->m_bUsed = true;
			UnorderAccessArray.ClearTimeCount(i);
			return pTUA;
		}

	}

	VSUnorderAccess* pNewUA = MEM_NEW VSTextureUnorderAccess(uiWidth, uiHeight, uiFormatType, CPURead);
	UnorderAccessArray.AddResource(pNewUA);
	pNewUA->m_bUsed = true;
	return pNewUA;
}
VSUnorderAccess * VSResourceManager::GetUnorderAccessTexture(uint32 uiWidth, uint32 uiHeight,
	uint32 uiFormatType, bool CPURead)
{
	VSResourceArrayControl<VSUnorderAccessPtr> & UnorderAccessArray = GetUnorderAccessBufferArray();
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	for (uint32 i = 0; i < UnorderAccessArray.GetResourceNum(); i++)
	{
		VSUnorderAccess* pUA = UnorderAccessArray.GetResource(i);
		if (pUA->m_bUsed || pUA->GetOutputType() != VSOutputResource::OT_TEXTURE_UNORDER_ACCESS)
		{
			continue;
		}
		if (pUA->GetCreateBy() && ((VSTexture *)pUA->GetCreateBy())->GetTexType() == VSTexture::TT_2D)
		{
			VS2DTexture * p2DTex = (VS2DTexture *)pUA->GetCreateBy();
			if (p2DTex->HasAnySlot())
			{
				continue;
			}
			VSTextureUnorderAccess * pTUA = (VSTextureUnorderAccess *)pUA;
			if (pTUA->m_uiWidth == uiWidth && pTUA->m_uiHeight == uiHeight &&
				pTUA->m_uiFormatType == uiFormatType
				&& pTUA->GetLockFlag() == LockFlag)
			{
				pUA->m_bUsed = true;
				UnorderAccessArray.ClearTimeCount(i);
				return pUA;
			}
		}
	}
	VS2DTexture * pTexture = MEM_NEW VS2DTexture(uiWidth, uiHeight, uiFormatType, 1, true);

	VSUnorderAccess *  pNewUA = MEM_NEW VSTextureUnorderAccess(pTexture,0, CPURead);
	UnorderAccessArray.AddResource(pNewUA);

	pNewUA->m_bUsed = true;
	return pNewUA;
}
VSDepthStencil * VSResourceManager::GetDepthStencilNoTexture(uint32 uiWidth,uint32 uiHeight,
	uint32 uiFormatType,uint32 uiMulSample, bool CPURead)
{
	VSMAC_ASSERT(uiMulSample < VSRenderer::MS_MAX)
	if (uiMulSample >= VSRenderer::MS_MAX)
	{
		return NULL;
	}
	if (CPURead && !VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_DepthStensilBufferRead))
	{
		return NULL;
	}
	VSResourceArrayControl<VSDepthStencilPtr> & DepthStencilArray = GetDepthStencilBufferArray();
	uint32 LockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	for (uint32 i = 0 ; i < DepthStencilArray.GetResourceNum() ; i++)
	{
		VSDepthStencil* pDS = DepthStencilArray.GetResource(i);
		if (pDS->m_bUsed || pDS->GetCreateBy())
		{
			continue;;
		}
		if (pDS->m_uiWidth == uiWidth && pDS->m_uiHeight == uiHeight &&
			pDS->m_uiFormatType == uiFormatType && pDS->m_uiMulSample == uiMulSample &&
			pDS->GetLockFlag() == LockFlag)
		{
			pDS->m_bUsed = true;
			DepthStencilArray.ClearTimeCount(i);
			return pDS;
		}

	}

	VSDepthStencil* pNewDS= MEM_NEW VSDepthStencil(uiWidth,uiHeight,uiMulSample,uiFormatType,CPURead);
	GetDepthStencilBufferArray().AddResource(pNewDS);
	pNewDS->m_bUsed = true;
	return pNewDS;
}
VSAnim * VSResourceManager::CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR)
{
	if (!pSourceAnim || !pTargetAnim || !pBlendAnimR)
	{
		return NULL;
	}

	while (!pBlendAnimR->IsLoaded())
	{

	}
	VSAnim * pBlendAnim = pBlendAnimR->GetResource();

	if (pSourceAnim->IsAdditive() || pTargetAnim->IsAdditive() || pBlendAnim->IsAdditive())
	{
		return NULL;
	}



	VSAnim * pAddAnim = MEM_NEW VSAnim();
	pAddAnim->m_pBlendAnim = pBlendAnimR;
	pAddAnim->SetRootMotion(pBlendAnim->IsRootAnim());


	float fSourceTimeScale = pSourceAnim->GetAnimLength() / pBlendAnim->GetAnimLength();

	float fTargetTimeScale = pTargetAnim->GetAnimLength() / pBlendAnim->GetAnimLength();

	for (uint32 i = 0; i < pBlendAnim->GetBoneKeyNum(); i++)
	{
		VSBoneKey * pBlendBoneKey = pSourceAnim->GetBoneKey(i);
		
		if (!pBlendBoneKey)
		{
			continue;
		}
		VSBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pTargetBoneKey = pTargetAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pAddBoneKey = MEM_NEW VSBoneKey();
		pAddBoneKey->m_cName = pSourceBoneKey->m_cName;
		if (!pTargetBoneKey || !pSourceBoneKey)
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			pAddBoneKey->m_TranslationArray[0].m_Vector = Vector3::ms_Zero;
			pAddBoneKey->m_ScaleArray[0].m_Vector = Vector3::ms_One;
			pAddBoneKey->m_RotatorArray[0].m_Quat = Quat();
			return false;
		}
		else
		{
			

			pAddBoneKey->m_TranslationArray.SetBufferNum(pBlendBoneKey->m_TranslationArray.GetNum());
			for (uint32 j = 0; j < pBlendBoneKey->m_TranslationArray.GetNum(); j++)
			{
				float fSourceTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime * fSourceTimeScale;
				float fTargetTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime * fTargetTimeScale;

				pAddBoneKey->m_TranslationArray[j].m_dKeyTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime;
				Vector3 SourceTranslation = pSourceAnim->GetTranslation(pBlendBoneKey->m_cName, fSourceTime);
				Vector3 TargetTranslation = pTargetAnim->GetTranslation(pBlendBoneKey->m_cName, fTargetTime);
				pAddBoneKey->m_TranslationArray[j].m_Vector = SourceTranslation - TargetTranslation;
					

			}

			pAddBoneKey->m_ScaleArray.SetBufferNum(pBlendBoneKey->m_ScaleArray.GetNum());
			for (uint32 j = 0; j < pBlendBoneKey->m_ScaleArray.GetNum(); j++)
			{
				float fSourceTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime * fSourceTimeScale;
				float fTargetTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime * fTargetTimeScale;

				pAddBoneKey->m_ScaleArray[j].m_dKeyTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime;

				Vector3 SourceScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fSourceTime);
				Vector3 TargetScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fTargetTime);
				VSMAC_ASSERT(SourceScale.x > EPSILON_E4 && SourceScale.y > EPSILON_E4 && SourceScale.z > EPSILON_E4);
				VSMAC_ASSERT(TargetScale.x > EPSILON_E4 && TargetScale.y > EPSILON_E4 && TargetScale.z > EPSILON_E4);
				pAddBoneKey->m_ScaleArray[j].m_Vector = SourceScale / TargetScale;


			}


			pAddBoneKey->m_RotatorArray.SetBufferNum(pBlendBoneKey->m_RotatorArray.GetNum());
			for (uint32 j = 0; j < pBlendBoneKey->m_RotatorArray.GetNum();j++)
			{
				float fSourceTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime * fSourceTimeScale;
				float fTargetTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime * fTargetTimeScale;

				pAddBoneKey->m_RotatorArray[j].m_dKeyTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime;
				Quat TargetRoator = pTargetAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime).GetInverse();
				Quat SourceRoator = pSourceAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime);
				pAddBoneKey->m_RotatorArray[j].m_Quat = SourceRoator * TargetRoator;

			}
		}
		

		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	
	return pAddAnim;
}

VSAnim * VSResourceManager::CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR,float fTargetTime)
{

	if (!pSourceAnim || !pTargetAnim)
	{
		return NULL;
	}


	while (!pBlendAnimR->IsLoaded())
	{

	}
	VSAnim * pBlendAnim = pBlendAnimR->GetResource();

	if (pSourceAnim->IsAdditive() || pTargetAnim->IsAdditive() || pBlendAnim->IsAdditive())
	{
		return NULL;
	}

	VSAnim * pAddAnim = MEM_NEW VSAnim();
	pAddAnim->m_pBlendAnim = pBlendAnimR;
	pAddAnim->SetRootMotion(pBlendAnim->IsRootAnim());

	float fSourceTimeScale = pSourceAnim->GetAnimLength() / pBlendAnim->GetAnimLength();

	for (uint32 i = 0; i < pBlendAnim->GetBoneKeyNum(); i++)
	{
		VSBoneKey * pBlendBoneKey = pBlendAnim->GetBoneKey(i);
		if (!pBlendBoneKey)
		{
			continue;
		}
		VSBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pTargetBoneKey = pTargetAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pAddBoneKey = MEM_NEW VSBoneKey();
		pAddBoneKey->m_cName = pSourceBoneKey->m_cName;

		if (!pTargetBoneKey || !pSourceBoneKey)
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			pAddBoneKey->m_TranslationArray[0].m_Vector = Vector3::ms_Zero;
			pAddBoneKey->m_ScaleArray[0].m_Vector = Vector3::ms_One;
			pAddBoneKey->m_RotatorArray[0].m_Quat = Quat();
			return false;
		}
		else
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(pBlendBoneKey->m_TranslationArray.GetNum());
			Vector3 TargetTranslation = pTargetAnim->GetTranslation(pBlendBoneKey->m_cName, fTargetTime);
			for (uint32 j = 0; j < pBlendBoneKey->m_TranslationArray.GetNum(); j++)
			{
				float fSourceTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime * fSourceTimeScale;

				pAddBoneKey->m_TranslationArray[j].m_dKeyTime = pBlendBoneKey->m_TranslationArray[j].m_dKeyTime;
				Vector3 SourceTranslation = pSourceAnim->GetTranslation(pBlendBoneKey->m_cName, fSourceTime);
				pAddBoneKey->m_TranslationArray[j].m_Vector = SourceTranslation - TargetTranslation;


			}

			pAddBoneKey->m_ScaleArray.SetBufferNum(pBlendBoneKey->m_ScaleArray.GetNum());
			Vector3 TargetScale = pTargetAnim->GetScale(pSourceBoneKey->m_cName, fTargetTime);
			VSMAC_ASSERT(TargetScale.x > EPSILON_E4 && TargetScale.y > EPSILON_E4 && TargetScale.z > EPSILON_E4);
			for (uint32 j = 0; j < pBlendBoneKey->m_ScaleArray.GetNum(); j++)
			{
				float fSourceTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime * fSourceTimeScale;


				pAddBoneKey->m_ScaleArray[j].m_dKeyTime = pBlendBoneKey->m_ScaleArray[j].m_dKeyTime;

				Vector3 SourceScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fSourceTime);

				VSMAC_ASSERT(SourceScale.x > EPSILON_E4 && SourceScale.y > EPSILON_E4 && SourceScale.z > EPSILON_E4);

				pAddBoneKey->m_ScaleArray[j].m_Vector = SourceScale / TargetScale;


			}

			Quat TargetRoator = pTargetAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime).GetInverse();
			pAddBoneKey->m_RotatorArray.SetBufferNum(pBlendBoneKey->m_RotatorArray.GetNum());
			for (uint32 j = 0; j < pBlendBoneKey->m_RotatorArray.GetNum(); j++)
			{
				float fSourceTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime * fSourceTimeScale;


				pAddBoneKey->m_RotatorArray[j].m_dKeyTime = pBlendBoneKey->m_RotatorArray[j].m_dKeyTime;

				Quat SourceRoator = pSourceAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime);
				pAddBoneKey->m_RotatorArray[j].m_Quat = SourceRoator * TargetRoator;

			}
		}
		



		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	return pAddAnim;
}
VSAnim * VSResourceManager::CreateAdditiveAnim(VSAnim * pSourceAnim, VSAnim * pTargetAnim, VSAnimR * pBlendAnimR,float fSourceTime, float fTargetTime)
{
	if (!pSourceAnim || !pTargetAnim)
	{
		return NULL;
	}


	while (!pBlendAnimR->IsLoaded())
	{

	}
	VSAnim * pBlendAnim = pBlendAnimR->GetResource();

	if (pSourceAnim->IsAdditive() || pTargetAnim->IsAdditive() || pBlendAnim->IsAdditive())
	{
		return NULL;
	}

	VSAnim * pAddAnim = MEM_NEW VSAnim();
	pAddAnim->m_pBlendAnim = pBlendAnimR;
	pAddAnim->SetRootMotion(pSourceAnim->IsRootAnim());


	for (uint32 i = 0; i < pBlendAnim->GetBoneKeyNum(); i++)
	{
		VSBoneKey * pBlendBoneKey = pBlendAnim->GetBoneKey(i);
		if (!pBlendBoneKey)
		{
			continue;
		}
		VSBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pTargetBoneKey = pTargetAnim->GetBoneKey(pBlendBoneKey->m_cName);
		VSBoneKey * pAddBoneKey = MEM_NEW VSBoneKey();
		pAddBoneKey->m_cName = pBlendBoneKey->m_cName;

		if (!pTargetBoneKey || !pSourceBoneKey)
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			pAddBoneKey->m_TranslationArray[0].m_Vector = Vector3::ms_Zero;
			pAddBoneKey->m_ScaleArray[0].m_Vector = Vector3::ms_One;
			pAddBoneKey->m_RotatorArray[0].m_Quat = Quat();
			return false;
		}
		else
		{
			pAddBoneKey->m_TranslationArray.SetBufferNum(1);
			Vector3 TargetTranslation = pTargetAnim->GetTranslation(pBlendBoneKey->m_cName, fTargetTime);
			pAddBoneKey->m_TranslationArray[0].m_dKeyTime = 0.0f;
			Vector3 SourceTranslation = pSourceAnim->GetTranslation(pBlendBoneKey->m_cName, fSourceTime);
			pAddBoneKey->m_TranslationArray[0].m_Vector = SourceTranslation - TargetTranslation;



			pAddBoneKey->m_ScaleArray.SetBufferNum(1);
			Vector3 TargetScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fTargetTime);
			VSMAC_ASSERT(TargetScale.x > EPSILON_E4 && TargetScale.y > EPSILON_E4 && TargetScale.z > EPSILON_E4);
			pAddBoneKey->m_ScaleArray[0].m_dKeyTime = 0.0f;
			Vector3 SourceScale = pTargetAnim->GetScale(pBlendBoneKey->m_cName, fSourceTime);
			VSMAC_ASSERT(SourceScale.x > EPSILON_E4 && SourceScale.y > EPSILON_E4 && SourceScale.z > EPSILON_E4);
			pAddBoneKey->m_ScaleArray[0].m_Vector = SourceScale / TargetScale;



			Quat TargetRoator = pTargetAnim->GetQuat(pBlendBoneKey->m_cName, fTargetTime).GetInverse();
			pAddBoneKey->m_RotatorArray.SetBufferNum(1);
			pAddBoneKey->m_RotatorArray[0].m_dKeyTime = 0.0f;
			Quat SourceRoator = pSourceAnim->GetQuat(pBlendBoneKey->m_cName, fSourceTime);
			pAddBoneKey->m_RotatorArray[0].m_Quat = SourceRoator * TargetRoator;


		}




		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	return pAddAnim;
}
VSInstanceGeometry *  VSResourceManager::GetDynamicInstanceGeometry(VSMeshData * pSourceMeshData, VSMaterialR * pMaterial)
{
	if (!pSourceMeshData || !pMaterial)
	{
		return NULL;
	}
// 	if (pSourceMeshData->GetVertexBuffer()->HaveBlendIndicesInfo())
// 	{
// 		return NULL;
// 	}

	VSResourceArrayControl<VSInstanceGeometryPtr> & DynamicInstanceGeometryArray = GetDynamicInstanceGeometryArray();
	for (uint32 i = 0; i < DynamicInstanceGeometryArray.GetResourceNum(); i++)
	{
		VSInstanceGeometry* pInstanceGeometry = DynamicInstanceGeometryArray.GetResource(i);
		if (pInstanceGeometry->IsUsed())
		{
			continue;
		}
		else
		{
			if (pInstanceGeometry->GetMeshData() == pSourceMeshData
				&& pInstanceGeometry->GetMaterialInstance(0)->GetMaterialR() == pMaterial)
			{
				//use for gc
				DynamicInstanceGeometryArray.ClearTimeCount(i);
				return pInstanceGeometry;
			}
		}
	}

	VSInstanceGeometry * pInstanceGeometry = MEM_NEW VSInstanceGeometry();
	pInstanceGeometry->CreateInstanceBuffer(pSourceMeshData, pMaterial);
	GetDynamicInstanceGeometryArray().AddResource(pInstanceGeometry);
	return pInstanceGeometry;
}
VSAnim * VSResourceManager::CreateAnim(VSAnim * pSourceAnim, float fSourceTime)
{
	if (!pSourceAnim)
	{
		return NULL;
	}



	VSAnim * pAddAnim = MEM_NEW VSAnim();
	pAddAnim->SetRootMotion(pSourceAnim->IsRootAnim());


	for (uint32 i = 0; i < pSourceAnim->GetBoneKeyNum(); i++)
	{
		VSBoneKey * pSourceBoneKey = pSourceAnim->GetBoneKey(i);
		if (!pSourceBoneKey)
		{
			continue;
		}


		VSBoneKey * pAddBoneKey = MEM_NEW VSBoneKey();
		pAddBoneKey->m_cName = pSourceBoneKey->m_cName;

		pAddBoneKey->m_TranslationArray.SetBufferNum(1);

		Vector3 Translate = pSourceAnim->GetTranslation(pSourceBoneKey->m_cName, fSourceTime);
		pAddBoneKey->m_TranslationArray[0].m_dKeyTime = 0.0F;
		pAddBoneKey->m_TranslationArray[0].m_Vector = Translate;

		

		pAddBoneKey->m_ScaleArray.SetBufferNum(1);
		Vector3 Scale = pSourceAnim->GetScale(pSourceBoneKey->m_cName, fSourceTime);

		VSMAC_ASSERT(Scale.x > EPSILON_E4 && Scale.y > EPSILON_E4 && Scale.z > EPSILON_E4);

		pAddBoneKey->m_ScaleArray[0].m_dKeyTime = 0.0f;
		pAddBoneKey->m_ScaleArray[0].m_Vector = Scale;



		pAddBoneKey->m_RotatorArray.SetBufferNum(1);
		Quat Rotator = pSourceAnim->GetQuat(pSourceBoneKey->m_cName, fSourceTime);


		pAddBoneKey->m_RotatorArray[0].m_dKeyTime = 0.0f;
		pAddBoneKey->m_RotatorArray[0].m_Quat = Rotator;


		pAddAnim->AddBoneKey(pAddBoneKey);
	}
	return pAddAnim;
}
