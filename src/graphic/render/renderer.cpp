#include "graphic/render/renderer.h"
#include "graphic/render/font.h"
#include "graphic/material/shader/vshader.h"
#include "graphic/material/shader/pshader.h"
#include "graphic/material/shader/gshader.h"
#include "graphic/material/shader/dshader.h"
#include "graphic/material/shader/hshader.h"
#include "graphic/material/shader/cshader.h"
#include "graphic/render/buffer/vertexbuffer.h"
#include "graphic/node/geometry.h"
#include "graphic/render/texture/texture.h"
#include "graphic/render/texture/texture2d.h"
#include "graphic/render/texture/texallstate.h"
#include "graphic/material/material.h"
#include "graphic/render/depthstencil.h"
#include "graphic/render/texture/cubetexture.h"
#include "graphic/render/buffer/vertexformat.h"
#include "graphic/node/mesh/meshdata.h"
#include "graphic/material/shaderstringfactory.h"
#include "math/aabb.h"
#include "math/obb.h"
#include "math/sphere.h"
#include "graphic/node/mesh/triangleset.h"
#include "graphic/node/mesh/lineset.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/render/outputresource/rendertarget.h"
#include "graphic/core/profiler.h"
#include "graphic/render/buffer/bufferresource.h"
#include "graphic/render/outputresource/unorderaccess.h"
#include "graphic/render/query.h"
#include "graphic/asyn/renderthread.h"


namespace zq {


//DECLEAR_TIME_PROFILENODE(DrawMesh, )
DECLEAR_COUNT_PROFILENODE(DrawPrimitiveNum, )
DECLEAR_COUNT_PROFILENODE(DrawGeometryNum, )


VSRenderer * VSRenderer::ms_pRenderer = NULL;
VSString VSRenderer::ms_ShaderTypeString[] = { _T("None") ,_T("Dx9") ,_T("Dx11") };
VSString VSRenderer::ms_ShaderTypePathString[] = { _T("None") ,_T("Dx9/"),_T("Dx11/") };
VSString VSRenderer::ms_ShaderProgramMain[] = { _T("VSMain"), _T("PSMain"), _T("GSMain"),_T("HSMain"),_T("DSMain"),_T("CSMain") };
uint32 VSRenderer::ms_uiBytesPerPixel[SFT_MAX] =
{
	4,  //SFT_A8R8G8B8
	4,	//SFT_X8R8G8B8
	2,	//SFT_D16
	16,	//SFT_A32B32G32R32F
	8,	//SFT_A16B16G16R16F
	4,	//SFT_G16R16F
	2,	//SFT_R16F
	4,	//SFT_R32F
	0,	//SFT_UNKNOWN
	4,	//SFT_D24S8
	8,	//SFT_G32R32F
	2,	//SFT_R5G6B5
	1,	//SFT_A8
	4,	//SFT_G16R16
	8,	//SFT_A16B16G16R16
	1,	//SFT_R8
	4,	//SFT_A8R8G8B8_S
	4,	//SFT_G16R16_S
	2,	//SFT_R16
	8,	//SFT_A16B16G16R16_S,
	8,	//SFT_BC1
	16,	//SFT_BC2
	16,	//SFT_BC3
	8,	//SFT_BC4
	16	//SFT_BC5
};
uint32 VSRenderer::ms_uiChannelPerPixel[SFT_MAX] =
{
	4,  //SFT_A8R8G8B8
	4,	//SFT_X8R8G8B8
	1,	//SFT_D16
	4,	//SFT_A32B32G32R32F
	4,	//SFT_A16B16G16R16F
	2,	//SFT_G16R16F
	1,	//SFT_R16F
	1,	//SFT_R32F
	0,	//SFT_UNKNOWN
	2,	//SFT_D24S8
	2,	//SFT_G32R32F
	3,	//SFT_R5G6B5
	1,	//SFT_A8
	2,	//SFT_G16R16
	4,	//SFT_A16B16G16R16
	1,	//SFT_R8
	4,	//SFT_A8R8G8B8_S
	2,	//SFT_G16R16_S
	1,	//SFT_R16
	4,	//SFT_A16B16G16R16_S
	4,	//SFT_BC1
	4,	//SFT_BC2
	4,	//SFT_BC3
	1,	//SFT_BC4
	2	//SFT_BC5
};
uint32 VSRenderer::ms_uiMinDimension[SFT_MAX] =
{
	1,  //SFT_A8R8G8B8
	1,	//SFT_X8R8G8B8
	1,	//SFT_D16
	1,	//SFT_A32B32G32R32F
	1,	//SFT_A16B16G16R16F
	1,	//SFT_G16R16F
	1,	//SFT_R16F
	1,	//SFT_R32F
	0,	//SFT_UNKNOWN
	1,	//SFT_D24S8
	1,	//SFT_G32R32F
	1,	//SFT_R5G6B5
	1,	//SFT_A8
	1,	//SFT_G16R16
	1,	//SFT_A16B16G16R16
	1,	//SFT_R8
	1,	//SFT_A8R8G8B8_S
	1,	//SFT_G16R16_S
	1,	//SFT_R16
	1,	//SFT_A16B16G16R16_S,
	4,	//SFT_BC1
	4,	//SFT_BC2
	4,	//SFT_BC3
	4,	//SFT_BC4
	4	//SFT_BC5
};
VSRenderer::SCREEN_QUAD_TYPE VSRenderer::ms_FullScreen[4] = 
{
	{Vector3W(-1.0f,  1.0f, 0.0f,1.0f),Vector2(0.0f,0.0f)},
	{Vector3W( 1.0f,  1.0f, 0.0f,1.0f),Vector2(1.0f,0.0f)},
	{Vector3W( 1.0f, -1.0f, 0.0f,1.0f),Vector2(1.0f,1.0f)},
	{Vector3W(-1.0f, -1.0f, 0.0f,1.0f),Vector2(0.0f,1.0f)}
	
};
MATH_USHORT_INDEX VSRenderer::ms_FullScreenI[6] = { 0, 1, 3, 1, 2, 3 };
const VSString & VSRenderer::GetRenderTypeShaderPath(uint32 RenderTypeAPI)
{
	return ms_ShaderTypePathString[RenderTypeAPI];
}
const VSString & VSRenderer::GetRenderTypeString(uint32 RenderTypeAPI)
{
	return ms_ShaderTypeString[RenderTypeAPI];
}
uint32 VSRenderer::GetBytesPerPixel(uint32 uiFormatType)
{
	VSMAC_ASSERT(uiFormatType < SFT_MAX)
	return ms_uiBytesPerPixel[uiFormatType];
}
uint32 VSRenderer::GetChannelPerPixel(uint32 uiFormatType)
{
	VSMAC_ASSERT(uiFormatType < SFT_MAX);
	return ms_uiChannelPerPixel[uiFormatType];
}
uint32 VSRenderer::GetMinDimension(uint32 uiFormatType)
{
	VSMAC_ASSERT(uiFormatType < SFT_MAX);
	return ms_uiMinDimension[uiFormatType];
}
const VSString & VSRenderer::GetVShaderProgramMain()
{
	return ms_ShaderProgramMain[VSEngineFlag::ST_VERTEX];
}
const VSString & VSRenderer::GetPShaderProgramMain()
{
	return ms_ShaderProgramMain[VSEngineFlag::ST_PIXEL];
}
const VSString & VSRenderer::GetGShaderProgramMain()
{
	return ms_ShaderProgramMain[VSEngineFlag::ST_GEOMETRY];
}
const VSString & VSRenderer::GetHShaderProgramMain()
{
	return ms_ShaderProgramMain[VSEngineFlag::ST_HULL];
}
const VSString & VSRenderer::GetDShaderProgramMain()
{
	return ms_ShaderProgramMain[VSEngineFlag::ST_DOMAIN];
}
const VSString & VSRenderer::GetRenderTypeShaderPath()const
{
	return GetRenderTypeShaderPath(GetRendererType());
}
const VSString & VSRenderer::GetRenderTypeString()const
{
	return GetRenderTypeString(GetRendererType());
}
VSRenderer::~VSRenderer()
{
	SAFE_DELETEA(m_pChildWindowInfo);
	for (uint32 i = 0; i < STREAM_LEVEL;i++)
	{
		m_pVertexBuffer[i] = NULL;
	}
	
	m_pVertexFormat = NULL;
	m_pIndexBuffer = NULL;
	m_pQuery = NULL;
	m_pVShader = NULL;
	m_pPShader = NULL;
	m_pGShader = NULL;
	m_pDShader = NULL;
	m_pHShader = NULL;
	m_pCShader = NULL;
	m_pFont		= NULL;
	m_pQuadVertexFormat = NULL;
	m_pFontVertexFormat = NULL;

	m_pBlendState = NULL;
	m_pDepthStencilState = NULL;
	m_pRasterizerState = NULL;

}
void VSRenderer::RenderStateToDefault()
{
	m_pPTex.SetBufferNum(m_uiMaxBindResource[VSEngineFlag::ST_PIXEL]);
	for (uint32 i = 0; i < m_uiMaxBindResource[VSEngineFlag::ST_PIXEL]; i++)
	{
		m_pPTex[i] = NULL;
	}
	m_pSamplerState[VSEngineFlag::ST_PIXEL].SetBufferNum(m_uiMaxSampler[VSEngineFlag::ST_PIXEL]);
	for (uint32 i = 0; i < m_uiMaxSampler[VSEngineFlag::ST_PIXEL]; i++)
	{
		m_pSamplerState[VSEngineFlag::ST_PIXEL][i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetPSamplerState(NULL, i, true);
	}

	m_pVTex.SetBufferNum(m_uiMaxBindResource[VSEngineFlag::ST_VERTEX]);
	for (uint32 i = 0; i < m_uiMaxBindResource[VSEngineFlag::ST_VERTEX]; i++)
	{
		m_pVTex[i] = NULL;
	}
	m_pSamplerState[VSEngineFlag::ST_VERTEX].SetBufferNum(m_uiMaxSampler[VSEngineFlag::ST_VERTEX]);
	for (uint32 i = 0; i < m_uiMaxSampler[VSEngineFlag::ST_VERTEX]; i++)
	{
		m_pSamplerState[VSEngineFlag::ST_VERTEX][i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetVSamplerState(NULL, i, true);
	}

	m_pGTex.SetBufferNum(m_uiMaxBindResource[VSEngineFlag::ST_GEOMETRY]);
	for (uint32 i = 0; i < m_uiMaxBindResource[VSEngineFlag::ST_GEOMETRY]; i++)
	{
		m_pGTex[i] = NULL;
	}
	m_pSamplerState[VSEngineFlag::ST_GEOMETRY].SetBufferNum(m_uiMaxSampler[VSEngineFlag::ST_GEOMETRY]);
	for (uint32 i = 0; i < m_uiMaxSampler[VSEngineFlag::ST_GEOMETRY]; i++)
	{
		m_pSamplerState[VSEngineFlag::ST_GEOMETRY][i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetGSamplerState(NULL, i, true);
	}

	m_pDTex.SetBufferNum(m_uiMaxBindResource[VSEngineFlag::ST_DOMAIN]);
	for (uint32 i = 0; i < m_uiMaxBindResource[VSEngineFlag::ST_DOMAIN]; i++)
	{
		m_pDTex[i] = NULL;
	}
	m_pSamplerState[VSEngineFlag::ST_DOMAIN].SetBufferNum(m_uiMaxSampler[VSEngineFlag::ST_DOMAIN]);
	for (uint32 i = 0; i < m_uiMaxSampler[VSEngineFlag::ST_DOMAIN]; i++)
	{
		m_pSamplerState[VSEngineFlag::ST_DOMAIN][i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetDSamplerState(NULL, i, true);
	}

	m_pHTex.SetBufferNum(m_uiMaxBindResource[VSEngineFlag::ST_HULL]);
	for (uint32 i = 0; i < m_uiMaxBindResource[VSEngineFlag::ST_HULL]; i++)
	{
		m_pHTex[i] = NULL;
	}
	m_pSamplerState[VSEngineFlag::ST_HULL].SetBufferNum(m_uiMaxSampler[VSEngineFlag::ST_HULL]);
	for (uint32 i = 0; i < m_uiMaxSampler[VSEngineFlag::ST_HULL]; i++)
	{
		m_pSamplerState[VSEngineFlag::ST_HULL][i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetHSamplerState(NULL, i, true);
	}

	m_pCTex.SetBufferNum(m_uiMaxBindResource[VSEngineFlag::ST_COMPUTE]);
	for (uint32 i = 0; i < m_uiMaxBindResource[VSEngineFlag::ST_COMPUTE]; i++)
	{
		m_pCTex[i] = NULL;
	}
	m_pSamplerState[VSEngineFlag::ST_COMPUTE].SetBufferNum(m_uiMaxSampler[VSEngineFlag::ST_COMPUTE]);
	for (uint32 i = 0; i < m_uiMaxSampler[VSEngineFlag::ST_COMPUTE]; i++)
	{
		m_pSamplerState[VSEngineFlag::ST_COMPUTE][i] = (VSSamplerState *)VSSamplerState::GetDefault();
		SetCSamplerState(NULL, i, true);
	}
	m_pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	m_pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	m_pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
	SetDepthStencilState(NULL, true);
	SetBlendState(NULL, true);
	SetRasterizerState(NULL, true);
	m_bClipPlaneEnable = false;
	m_bScissorRectEnable = false;
 	m_pVertexFormat = NULL;
	for (uint32 i = 0; i < STREAM_LEVEL; i++)
	{
		m_pVertexBuffer[i] = NULL;
	}
	m_pIndexBuffer = NULL;
	m_pQuery = NULL;
	m_pPShader = NULL;
	m_pVShader = NULL;
	m_pGShader = NULL;
	m_pDShader = NULL;
	m_pHShader = NULL;
	m_pCShader = NULL;
	LoadTexture(VSTexAllState::GetDefault()->m_pTex);
}
bool VSRenderer::SetDefaultValue()
{
	RenderStateToDefault();

	m_pFont = VSFont::ms_DefaultResource;

	VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> FormatArray;
	VSVertexFormat::VERTEXFORMAT_TYPE Pos;
	Pos.DataType = VSDataBuffer::DT_FLOAT32_4;
	Pos.Offset = 0;
	Pos.Semantics = VSVertexFormat::VF_POSITION;
	Pos.SemanticsIndex = 0;
	FormatArray.AddElement(Pos);

	VSVertexFormat::VERTEXFORMAT_TYPE TextureCode;
	TextureCode.DataType = VSDataBuffer::DT_FLOAT32_2;
	TextureCode.Offset = 16;
	TextureCode.Semantics = VSVertexFormat::VF_TEXCOORD;
	TextureCode.SemanticsIndex = 0;
	FormatArray.AddElement(TextureCode);
	m_pQuadVertexFormat = VSResourceManager::LoadVertexFormat(NULL, &FormatArray);

	VSVertexFormat::VERTEXFORMAT_TYPE ColorCode;
	ColorCode.DataType = VSDataBuffer::DT_COLOR;
	ColorCode.Offset = 24;
	ColorCode.Semantics = VSVertexFormat::VF_COLOR;
	ColorCode.SemanticsIndex = 0;
	FormatArray.AddElement(ColorCode);
	m_pFontVertexFormat = VSResourceManager::LoadVertexFormat(NULL, &FormatArray);
	VSResourceManager::LoadDefaultDeviceResource(GetRendererType());
	VSCustomMaterial::LoadAllDeviceShader();
	VSCustomCSMaterial::LoadAllDeviceShader();
	return 1;
}
VSString VSRenderer::GetValueElement(const VSPutNode * pPutNode,unsigned char uiVE)
{
	VSMAC_ASSERT(pPutNode);
	VSString Temp = pPutNode->GetNodeName().GetString();
	if (uiVE > 0)
	{
		if (pPutNode->GetValueType() == VSPutNode::VT_1 && (uiVE & VE_R))
		{
			return Temp;
		}
		else if (pPutNode->GetValueType() == VSPutNode::VT_1)
		{
			VSMAC_ASSERT(0);
			return "";
		}
		VSString Value[4];
		Value[0] = _T("r");
		Value[1] = _T("g");
		Value[2] = _T("b");
		Value[3] = _T("a");
		uint32 Mask[4];
		Mask[0] = VE_R;
		Mask[1] = VE_G;
		Mask[2] = VE_B;
		Mask[3] = VE_A;
		Temp += _T(".");

		for (uint32 i = 0; i < 4; i++)
		{
			if (i <= pPutNode->GetValueType())
			{
				if (uiVE & Mask[i])
				{
					Temp += Value[i];
				}
			}

		}

	}


	return Temp;
}
VSString VSRenderer::GetValueElement(const VSString & InputString,unsigned char uiVE)
{
	VSString Temp = InputString;
	if (uiVE > 0)
	{
		VSString Value[4];
		Value[0] = _T("r");
		Value[1] = _T("g");
		Value[2] = _T("b");
		Value[3] = _T("a");
		uint32 Mask[4];
		Mask[0] = VE_R;
		Mask[1] = VE_G;
		Mask[2] = VE_B;
		Mask[3] = VE_A;
		Temp += _T(".");

		for (uint32 i = 0; i < 4; i++)
		{
			if (uiVE & Mask[i])
			{
				Temp += Value[i];
			}
		}
	}
	return Temp;
}
bool VSRenderer::SetRenderTargets(VSRenderTarget * pRenderTargets[], uint32 uiNum)
{
	VSMAC_ASSERT(uiNum > 0);

	if (pRenderTargets[0] == NULL)
	{
		return false;
	}
	for (uint32 i = 0; i < uiNum; i++)
	{
		SetRenderTarget(pRenderTargets[i], i);
	}
	return true;
}
bool VSRenderer::EndRenderTargets(VSRenderTarget * pRenderTargets[], uint32 uiNum)
{
	VSMAC_ASSERT(uiNum > 0);

	if (pRenderTargets[0] == NULL)
	{
		return false;
	}
	for (uint32 i = 0; i < uiNum; i++)
	{
		EndRenderTarget(pRenderTargets[i], i);
	}
	return true;

}
bool VSRenderer::SetCSUnorderAccesses(VSUnorderAccess * pUnorderAccesses[], uint32 uiNum)
{

	if (pUnorderAccesses[0] == NULL)
	{
		return false;
	}
	for (uint32 i = 0; i < uiNum; i++)
	{
		SetCSUnorderAccess(pUnorderAccesses[i], i);
	}
	return true;

}
bool VSRenderer::EndCSUnorderAccesses(VSUnorderAccess * pUnorderAccesses[], uint32 uiNum)
{
	VSMAC_ASSERT(uiNum > 0);

	if (pUnorderAccesses[0] == NULL)
	{
		return false;
	}
	for (uint32 i = 0; i < uiNum; i++)
	{
		EndCSUnorderAccess(pUnorderAccesses[i], i);
	}
	return true;

}
VSRenderer::ChildWindowInfo * VSRenderer::GetChildWindowInfo(int32 uiID)
{
	VSMAC_ASSERT(m_bWindowed);
	VSMAC_ASSERT(uiID < m_iNumChildWindow);
	if (uiID < 0)
	{
		return NULL;
	}
	return &m_pChildWindowInfo[uiID];
}

bool VSRenderer::ReleaseDefaultValue()
{
	ReleaseBindObjectResource();
	VSCullerManager::GetCullerManager().ClearAllOcclusionQuery();
	VSResourceManager::CacheResource();
	return 1;
}
bool VSRenderer::ReleaseBindObjectResource()
{
	for (uint32 i = 0; i < VSBind::ms_BindArray.GetNum(); i++)
	{
		VSBind * pBind = VSBind::ms_BindArray[i];
		if (!pBind)
		{
			continue;
		}
		if (!pBind->ReleaseResource())
		{
			VSMAC_ASSERT(0);
			return false;
		}
	}
	return true;
}

void VSRenderer::SetUseState(VSRenderState & RenderState, uint32 uiRenderStateInheritFlag)
{
	m_UseState.GetAll(&RenderState);
	m_uiRenderStateInheritFlag = uiRenderStateInheritFlag;
}
void VSRenderer::ClearUseState()
{
	m_uiRenderStateInheritFlag = 0;
}
void VSRenderer::SetRenderState(VSRenderState & RenderState)
{

	SetBlendState(RenderState.m_pBlendState);

	SetDepthStencilState(RenderState.m_pDepthStencilState);

	SetRasterizerState(RenderState.m_pRasterizerState);

	if (RenderState.m_RasterizerDesc.m_bClipPlaneEnable && RenderState.m_Plane.GetNum() > 0)
	{
		m_bClipPlaneEnable = true;
		SetClipPlane(RenderState.m_Plane, m_bClipPlaneEnable);
	}
	else
	{
		if (m_bClipPlaneEnable)
		{
			m_bClipPlaneEnable = false;
			SetClipPlane(RenderState.m_Plane, m_bClipPlaneEnable);
		}
	}
	if (RenderState.m_RasterizerDesc.m_bScissorTestEnable && RenderState.m_RectArray.GetNum() > 0)
	{
		m_bScissorRectEnable = true;
		SetScissorRect(RenderState.m_RectArray, m_bScissorRectEnable);
	}
	else
	{
		if (m_bScissorRectEnable)
		{
			m_bScissorRectEnable = false;
			SetScissorRect(RenderState.m_RectArray, m_bScissorRectEnable);
		}
	}


}
bool VSRenderer::SetVShaderProgram(VSVShader*	pVShaderProgram)
{
	if (!pVShaderProgram)
		return 0;
	if (!LoadVShaderProgram(pVShaderProgram))
		return 0;

	return 1;
}
bool VSRenderer::SetGShaderProgram(VSGShader* pGShaderProgram)
{
	if (!pGShaderProgram)
		return 0;
	if (!LoadGShaderProgram(pGShaderProgram))
		return 0;
	return 1;
}
bool VSRenderer::SetDShaderProgram(VSDShader* pDShaderProgram)
{
	if (!pDShaderProgram)
		return 0;
	if (!LoadDShaderProgram(pDShaderProgram))
		return 0;
	return 1;
}
bool VSRenderer::SetHShaderProgram(VSHShader* pHShaderProgram)
{
	if (!pHShaderProgram)
		return 0;
	if (!LoadHShaderProgram(pHShaderProgram))
		return 0;
	return 1;
}
bool VSRenderer::SetCShaderProgram(VSCShader*	pCShaderProgram)
{
	if (!pCShaderProgram)
		return 0;
	if (!LoadCShaderProgram(pCShaderProgram))
		return 0;
	return 1;
}
bool VSRenderer::SetPShaderProgram(VSPShader*	pPShaderProgram)
{
	if (!pPShaderProgram)
		return 0;
	if (!LoadPShaderProgram(pPShaderProgram))
		return 0;
	return 1;
}
void VSRenderer::EnableBindResourceUsed(VSSlot * pSlotResource)
{
	if (!pSlotResource)
	{
		return;
	}
	pSlotResource->m_bBindResourceUse = true;
}
void VSRenderer::DisableBindResourceUsed(VSSlot * pSlotResource)
{
	if (!pSlotResource)
	{
		return;
	}
	pSlotResource->m_bBindResourceUse = false;
}
bool VSRenderer::SetRenderTarget(VSRenderTarget *pRenderTarget, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxRTNum);
	if (!pRenderTarget)
	{
		return 0;
	}
	if (!LoadRenderTarget(pRenderTarget))
		return 0;
	VSTexture * pTexture = (VSTexture *)pRenderTarget->GetCreateBy();

	if (pTexture)
	{
		pTexture->ClearAllSlot();
	}


	
	if (m_uiCurRTWidth != pRenderTarget->GetWidth()
		|| m_uiCurRTHeight != pRenderTarget->GetHeight()
		|| m_uiCurRTMultisample != pRenderTarget->GetMulSample())
	{
		if (i == 0 && m_uiBackMacthType == BMT_NONE)
		{
			m_uiBackMacthType = BMT_RENDER_TARGET;
			m_uiCurRTWidth = pRenderTarget->GetWidth();
			m_uiCurRTHeight = pRenderTarget->GetHeight();
			m_uiCurRTMultisample = pRenderTarget->GetMulSample();
		}
		else
		{
			VSMAC_ASSERT(false);
		}
		
	}
	
	EnableBindResourceUsed(pTexture);
	return 1;
}
bool VSRenderer::EndRenderTarget(VSRenderTarget *pRenderTarget, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxRTNum);
	if (!pRenderTarget)
		return 0;
	VSTexture * pTexture = (VSTexture *)pRenderTarget->GetCreateBy();
// 	if (pTexture)
// 	{
// 		pTexture->ClearAllSlot();
// 	}
// 	if (!LoadRenderTarget(pRenderTarget))
// 		return 0;
	m_uiCurRTWidth = m_uiScreenWidth;
	m_uiCurRTHeight = m_uiScreenHeight;
	m_uiCurRTMultisample = m_uiCurMultisample;
	DisableBindResourceUsed(pTexture);
	if (m_uiBackMacthType == BMT_RENDER_TARGET)
	{
		m_uiBackMacthType = BMT_NONE;
	}
	return 1;
}
bool VSRenderer::SetDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer)
{
	if (!pDepthStencilBuffer)
		return 0;
#ifdef _DEBUG
	if (m_uiCurRTWidth != pDepthStencilBuffer->GetWidth()
		|| m_uiCurRTHeight != pDepthStencilBuffer->GetHeight()
		|| m_uiCurRTMultisample != pDepthStencilBuffer->GetMulSample())
	{
		VSMAC_ASSERT(false);	
	}
#endif
	if (!LoadDepthStencil(pDepthStencilBuffer))
		return 0;
	VSTexture * pTexture = (VSTexture *)pDepthStencilBuffer->GetCreateBy();
	EnableBindResourceUsed(pTexture);
	return 1;
}
bool VSRenderer::SetCSUnorderAccess(VSUnorderAccess * pUnorderAccess, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxUAVNum);
	if (!pUnorderAccess)
	{
		return false;
	}
	if (!LoadUnorderAccess(pUnorderAccess))
		return 0;
	VSBind * pBindResource = pUnorderAccess->GetCreateBy();
	VSSlot *pSlotResource = (VSSlot *)pBindResource;
	if (pSlotResource)
	{
		pSlotResource->ClearAllSlot();
	}
	EnableBindResourceUsed(pSlotResource);
	return 1;
}
bool VSRenderer::EndCSUnorderAccess(VSUnorderAccess * pUnorderAccess, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxUAVNum);
	if (!pUnorderAccess)
	{
		return false;
	}
	VSBind * pBindResource = pUnorderAccess->GetCreateBy();
	VSSlot *pSlotResource = (VSSlot *)pBindResource;
// 	if (!LoadUnorderAccess(pUnorderAccess))
// 		return 0;
	DisableBindResourceUsed(pSlotResource);
	return 1;
}
bool VSRenderer::EndDepthStencilBuffer(VSDepthStencil * pDepthStencilBuffer)
{
	if (!pDepthStencilBuffer)
		return 0;
	VSTexture * pTexture = (VSTexture *)pDepthStencilBuffer->GetCreateBy();
// 	if (!LoadDepthStencil(pDepthStencilBuffer))
// 		return 0;
	DisableBindResourceUsed(pTexture);
	return 1;
}
uint32 VSRenderer::SetVBuffer(VSVertexBuffer * pVBuffer, uint32 uiStream)
{
	if (m_pVertexBuffer[uiStream] == pVBuffer)
	{
		if (!pVBuffer)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pVBuffer  && pVBuffer->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}
	}

	if (!pVBuffer)
	{
		m_pVertexBuffer[uiStream] = NULL;
		return FRI_FAIL;
	}
	if (!LoadVBuffer(pVBuffer))
	{
		m_pVertexBuffer[uiStream] = NULL;
		return FRI_FAIL;
	}
	m_pVertexBuffer[uiStream] = pVBuffer;

	return FRI_SUCCESS;
}
uint32 VSRenderer::SetVertexFormat(VSVertexFormat * pVertexFormat)
{
	if (m_pVertexFormat == pVertexFormat)
		return FRI_SAMERESOURCE;
	m_pVertexFormat = pVertexFormat;
	if (!pVertexFormat)
	{
		return FRI_FAIL;
	}
	return FRI_SUCCESS;
}
bool VSRenderer::CopyResourceBuffer(VS2DTexture * pSource, VSCubeTexture * pDest, uint32 uiFace)
{
#ifdef _DEBUG
	if (!pSource || !pDest || uiFace >= VSCubeTexture::F_MAX)
	{
		VSMAC_ASSERT(0);
	}
	if (pSource->GetWidth(0) != pSource->GetHeight(0) && pSource->GetWidth(0) != pDest->GetWidth(0))
	{
		VSMAC_ASSERT(0);
	}
	if (pSource->GetMipLevel() != pDest->GetMipLevel())
	{
		VSMAC_ASSERT(0);
	}
	if (pSource->GetFormatType() != pDest->GetFormatType())
	{
		VSMAC_ASSERT(0);
	}
#endif
	return true;
}
uint32 VSRenderer::BeginQuery(VSQuery * pQuery)
{
	VSMAC_ASSERT(!m_pQuery);
	if (m_pQuery)
	{
		EndQuery(m_pQuery);
		m_pQuery = NULL;
		return FRI_FAIL;
	}
	if (!pQuery)
	{
		return FRI_FAIL;
	}
	if (!LoadQuery(pQuery))
	{
		return FRI_FAIL;
	}
	m_pQuery = pQuery;
	return FRI_SUCCESS;
}
uint32 VSRenderer::EndQuery(VSQuery * pQuery)
{
	VSMAC_ASSERT(m_pQuery && m_pQuery == pQuery);
	if (!m_pQuery)
	{
		return FRI_FAIL;
	}
	if (m_pQuery != pQuery)
	{
		return FRI_FAIL;
	}
	m_pQuery = NULL;
	return FRI_SUCCESS;
}
uint32 VSRenderer::SetIBuffer(VSIndexBuffer* pIBuffer)
{
	if (m_pIndexBuffer == pIBuffer)
	{
		if (!pIBuffer)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pIBuffer && pIBuffer->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pIBuffer)
	{
		m_pIndexBuffer = NULL;
		return FRI_FAIL;
	}
	if (!LoadIBuffer(pIBuffer))
	{
		m_pIndexBuffer = NULL;
		return FRI_FAIL;
	}

	m_pIndexBuffer = pIBuffer;

	return FRI_SUCCESS;
}

bool VSRenderer::LoadVShaderProgram(VSVShader * pVShaderProgram)
{
	if (!pVShaderProgram)
		return 0;

	return pVShaderProgram->LoadResource(this);
}
bool VSRenderer::ReleaseVShaderProgram(VSVShader * pVShaderProgram)
{
	if (!pVShaderProgram)
		return 0;
	return pVShaderProgram->ReleaseResource();
}

bool VSRenderer::LoadPShaderProgram(VSPShader* pPShaderProgram)
{
	if (!pPShaderProgram)
		return 0;

	return pPShaderProgram->LoadResource(this);

}
bool VSRenderer::ReleasePShaderProgram(VSPShader* pPShaderProgram)
{

	if (!pPShaderProgram)
		return 0;

	return pPShaderProgram->ReleaseResource();

}
bool VSRenderer::LoadGShaderProgram(VSGShader * pGShaderProgram)
{
	if (!pGShaderProgram)
		return 0;

	return pGShaderProgram->LoadResource(this);
}
bool VSRenderer::ReleaseGShaderProgram(VSGShader * pGShaderProgram)
{
	if (!pGShaderProgram)
		return 0;

	return pGShaderProgram->ReleaseResource();
}
bool VSRenderer::LoadDShaderProgram(VSDShader * pDShaderProgram)
{
	if (!pDShaderProgram)
		return 0;

	return pDShaderProgram->LoadResource(this);
}
bool VSRenderer::ReleaseDShaderProgram(VSDShader * pDShaderProgram)
{
	if (!pDShaderProgram)
		return 0;

	return pDShaderProgram->ReleaseResource();
}
bool VSRenderer::LoadHShaderProgram(VSHShader * pHShaderProgram)
{
	if (!pHShaderProgram)
		return 0;

	return pHShaderProgram->LoadResource(this);
}
bool VSRenderer::ReleaseHShaderProgram(VSHShader * pHShaderProgram)
{
	if (!pHShaderProgram)
		return 0;

	return pHShaderProgram->ReleaseResource();
}
bool VSRenderer::LoadCShaderProgram(VSCShader * pCShaderProgram)
{
	if (!pCShaderProgram)
		return 0;

	return pCShaderProgram->LoadResource(this);
}
bool VSRenderer::ReleaseCShaderProgram(VSCShader * pCShaderProgram)
{
	if (!pCShaderProgram)
		return 0;

	return pCShaderProgram->ReleaseResource();
}
bool VSRenderer::LoadTexture(VSTexture * pTexture)
{
	if (!pTexture)
		return 0;

	return pTexture->LoadResource(this);
}
bool VSRenderer::ReleaseTexture(VSTexture * pTexture)
{

	if (!pTexture)
		return 0;

	return pTexture->ReleaseResource();

}
bool VSRenderer::LoadBufferResource(VSBufferResource * pBufferResource)
{
	if (!pBufferResource)
		return 0;

	return pBufferResource->LoadResource(this);
}
bool VSRenderer::ReleaseBufferResource(VSBufferResource * pBufferResource)
{
	if (!pBufferResource)
		return 0;

	return pBufferResource->ReleaseResource();
}
bool VSRenderer::LoadDepthStencil(VSDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
		return 0;
	VSTexture * pTexture = (VSTexture *)pDepthStencil->GetCreateBy();
	if (pTexture)
	{
		pTexture->LoadResource(this);
	}
	return pDepthStencil->LoadResource(this);
}
bool VSRenderer::ReleaseDepthStencil(VSDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
		return 0;

	return pDepthStencil->ReleaseResource();
}
bool VSRenderer::LoadRenderTarget(VSRenderTarget  * pRenderTarget)
{
	if (!pRenderTarget)
	{
		return 0;
	}
	VSTexture * pTexture = (VSTexture *)pRenderTarget->GetCreateBy();
	if (pTexture)
	{
		pTexture->LoadResource(this);
	}
	pRenderTarget->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseRenderTarget(VSRenderTarget * pRenderTarget)
{
	if (!pRenderTarget)
	{
		return 0;
	}
	pRenderTarget->ReleaseResource();
	return 1;
}
bool VSRenderer::LoadUnorderAccess(VSUnorderAccess  * pUnorderAccess)
{
	if (!pUnorderAccess)
	{
		return 0;
	}
	VSBind * pBindResource = pUnorderAccess->GetCreateBy();
	if (pBindResource)
	{
		pBindResource->LoadResource(this);
	}
	pUnorderAccess->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseUnorderAccess(VSUnorderAccess  * pUnorderAccess)
{
	if (!pUnorderAccess)
	{
		return 0;
	}
	pUnorderAccess->ReleaseResource();
	return 1;
}
bool VSRenderer::LoadBlendState(VSBlendState * pBlendState)
{
	if (!pBlendState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pBlendState->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseBlendState(VSBlendState * pBlendState)
{
	if (!pBlendState)
	{
		return 0;
	}
	pBlendState->ReleaseResource();
	return 1;
}

bool VSRenderer::LoadDepthStencilState(VSDepthStencilState * pDepthStencilState)
{
	if (!pDepthStencilState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pDepthStencilState->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseDepthStencilState(VSDepthStencilState * pDepthStencilState)
{
	if (!pDepthStencilState)
	{
		return 0;
	}
	pDepthStencilState->ReleaseResource();
	return 1;
}

bool VSRenderer::LoadRasterizerState(VSRasterizerState * pRasterizerState)
{
	if (!pRasterizerState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pRasterizerState->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseRasterizerState(VSRasterizerState * pRasterizerState)
{
	if (!pRasterizerState)
	{
		return 0;
	}
	pRasterizerState->ReleaseResource();
	return 1;
}
bool VSRenderer::LoadSamplerState(VSSamplerState * pSamplerState)
{
	if (!pSamplerState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pSamplerState->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseSamplerState(VSSamplerState * pSamplerState)
{
	if (!pSamplerState)
	{
		return 0;
	}
	pSamplerState->ReleaseResource();
	return 1;
}
bool VSRenderer::LoadQuery(VSQuery * pQuery)
{
	if (!pQuery)
	{
		return 0;
	}

	pQuery->LoadResource(this);
	return 1;
}
bool VSRenderer::ReleaseQuery(VSQuery * pQuery)
{
	if (!pQuery)
	{
		return 0;
	}
	pQuery->ReleaseResource();
	return 1;
}
bool VSRenderer::LoadVBuffer(VSVertexBuffer * pVBuffer)
{
	if (!pVBuffer)
		return 0;

	return pVBuffer->LoadResource(this);
}
bool VSRenderer::ReleaseVBuffer(VSVertexBuffer * pVBuffer)
{

	if (!pVBuffer)
		return 0;

	return pVBuffer->ReleaseResource();
}
bool VSRenderer::LoadIBuffer(VSIndexBuffer* pIBuffer)
{
	if (!pIBuffer)
		return 0;

	return pIBuffer->LoadResource(this);

}
bool VSRenderer::ReleaseIBuffer(VSIndexBuffer* pIBuffer)
{
	if (!pIBuffer)
		return 0;

	return pIBuffer->ReleaseResource();

}
uint32 VSRenderer::SetInstanceMesh(VSInstanceGeometry *pInstanceGeometry)
{
	VSMAC_ASSERT(pInstanceGeometry);

	VSMeshData * pMeshData = pInstanceGeometry->GetMeshData();

	if (!SetVBuffer(pMeshData->GetVertexBuffer()))
	{
		SetVertexFormat(NULL);
		return 0;
	}
	if (!SetIBuffer(pMeshData->GetIndexBuffer()))
		return 0;
	if (!SetVBuffer(pInstanceGeometry->GetInstanceVertexBuffer(),1))
	{
		SetVertexFormat(NULL);
		return 0;
	}
	if (!SetVertexFormat(pInstanceGeometry->GetInstanceVertexFormat()))
		return 0;
	return FRI_SUCCESS;
}
uint32 VSRenderer::SetNormalMesh(VSGeometry *pGeometry)
{

	VSMAC_ASSERT(pGeometry);
	VSMeshData * pMeshData = pGeometry->GetMeshData();

	if (!SetVBuffer(pMeshData->GetVertexBuffer()))
	{
		SetVertexFormat(NULL);
		return 0;
	}
	if (!SetIBuffer(pMeshData->GetIndexBuffer()))
		return 0;
	if (!SetVertexFormat(pMeshData->GetVertexBuffer()->GetVertexFormat()))
		return 0;
	return FRI_SUCCESS;

}
bool VSRenderer::DrawMesh(VSGeometry * pGeometry, VSRenderState * pRenderState, VSVShader * pVShader, VSPShader * pPShader,
	VSGShader * pGShader,VSHShader * pHShader, VSDShader * pDShader)
{

	VSMAC_ASSERT(pGeometry && pGeometry->GetMeshData());
	m_LocalRenderState.GetAll(pRenderState);

	if (pGeometry->IsSwapCull())
	{
		m_LocalRenderState.SwapCull();
	}
	if (m_uiRenderStateInheritFlag)
	{

		m_LocalRenderState.Inherit(&m_UseState, m_uiRenderStateInheritFlag);


	}
	SetRenderState(m_LocalRenderState);
	if (!SetVShader(pVShader))
	{
		return 0;
	}
	SetHShader(pHShader);
	SetDShader(pDShader);
	SetGShader(pGShader);
	if (!SetPShader(pPShader))
	{
		return 0;
	}
	
	if (pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		if (SetInstanceMesh((VSInstanceGeometry *)pGeometry) == FRI_FAIL)
			return 0;
	}
	else
	{
		if (SetNormalMesh(pGeometry) == FRI_FAIL)
			return 0;
	}
	

	return 1;
}
void VSRenderer::SetVTexAllState(VSTexAllState * pTexAllState, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_VERTEX]);
	uint32 iMinShaderSampler = m_pVShader->GetMinShareSampler();
	bool bSetSampler = true;
	if ((iMinShaderSampler != INVALID_SIMPLAR_REGISTER && i >= iMinShaderSampler) || i >= m_uiMaxSampler[VSEngineFlag::ST_VERTEX])
	{
		bSetSampler = false;
	}
	if (pTexAllState)
	{
		SetVTexture(pTexAllState->m_pTex, i);		
		if (bSetSampler)
		{
			SetVSamplerState(pTexAllState->m_pSamplerState, i);
		}
	}
	else
	{
		SetVTexture(NULL, i);
		if (bSetSampler)
		{
			SetVSamplerState(NULL, i);
		}
	}
}
uint32 VSRenderer::SetGSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet)
{
	VSMAC_ASSERT(i < m_uiMaxSampler[VSEngineFlag::ST_GEOMETRY]);
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pSamplerState[VSEngineFlag::ST_GEOMETRY][i])
	{
		m_pSamplerState[VSEngineFlag::ST_GEOMETRY][i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pSamplerState[VSEngineFlag::ST_GEOMETRY][i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
uint32 VSRenderer::SetDSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet)
{
	VSMAC_ASSERT(i < m_uiMaxSampler[VSEngineFlag::ST_DOMAIN]);
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pSamplerState[VSEngineFlag::ST_DOMAIN][i])
	{
		m_pSamplerState[VSEngineFlag::ST_DOMAIN][i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pSamplerState[VSEngineFlag::ST_DOMAIN][i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
uint32 VSRenderer::SetHSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet)
{
	VSMAC_ASSERT(i < m_uiMaxSampler[VSEngineFlag::ST_HULL]);
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pSamplerState[VSEngineFlag::ST_HULL][i])
	{
		m_pSamplerState[VSEngineFlag::ST_HULL][i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pSamplerState[VSEngineFlag::ST_HULL][i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
uint32 VSRenderer::SetCSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet)
{
	VSMAC_ASSERT(i < m_uiMaxSampler[VSEngineFlag::ST_COMPUTE]);
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pSamplerState[VSEngineFlag::ST_COMPUTE][i])
	{
		m_pSamplerState[VSEngineFlag::ST_COMPUTE][i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pSamplerState[VSEngineFlag::ST_COMPUTE][i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
uint32 VSRenderer::SetVSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet)
{
	VSMAC_ASSERT(i < m_uiMaxSampler[VSEngineFlag::ST_VERTEX]);
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pSamplerState[VSEngineFlag::ST_VERTEX][i])
	{
		m_pSamplerState[VSEngineFlag::ST_VERTEX][i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pSamplerState[VSEngineFlag::ST_VERTEX][i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
void VSRenderer::SetGTexAllState(VSTexAllState * pTexAllState, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_GEOMETRY]);
	uint32 iMinShaderSampler = m_pGShader->GetMinShareSampler();
	bool bSetSampler = true;
	if ((iMinShaderSampler != INVALID_SIMPLAR_REGISTER && i >= iMinShaderSampler) || i >= m_uiMaxSampler[VSEngineFlag::ST_GEOMETRY])
	{
		bSetSampler = false;
	}
	if (pTexAllState)
	{
		SetGTexture(pTexAllState->m_pTex, i);
		
		if (bSetSampler)
		{
			SetGSamplerState(pTexAllState->m_pSamplerState, i);
		}
	}
	else
	{
		SetGTexture(NULL, i);
		if (bSetSampler)
		{
			SetGSamplerState(NULL, i);
		}
	}
}
void VSRenderer::SetDTexAllState(VSTexAllState * pTexAllState, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_DOMAIN]);
	uint32 iMinShaderSampler = m_pDShader->GetMinShareSampler();
	bool bSetSampler = true;
	if ((iMinShaderSampler != INVALID_SIMPLAR_REGISTER && i >= iMinShaderSampler) || i >= m_uiMaxSampler[VSEngineFlag::ST_DOMAIN])
	{
		bSetSampler = false;
	}
	if (pTexAllState)
	{
		SetDTexture(pTexAllState->m_pTex, i);
		
		if (bSetSampler)
		{
			SetDSamplerState(pTexAllState->m_pSamplerState, i);
		}
	}
	else
	{
		SetDTexture(NULL, i);
		if (bSetSampler)
		{
			SetDSamplerState(NULL, i);
		}
	}
}
void VSRenderer::SetHTexAllState(VSTexAllState * pTexAllState, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_HULL]);
	uint32 iMinShaderSampler = m_pHShader->GetMinShareSampler();
	bool bSetSampler = true;
	if ((iMinShaderSampler != INVALID_SIMPLAR_REGISTER && i >= iMinShaderSampler) || i >= m_uiMaxSampler[VSEngineFlag::ST_HULL])
	{
		bSetSampler = false;
	}
	if (pTexAllState)
	{
		SetHTexture(pTexAllState->m_pTex, i);
		
		if (bSetSampler)
		{
			SetHSamplerState(pTexAllState->m_pSamplerState, i);
		}
	}
	else
	{
		SetHTexture(NULL, i);
		if (bSetSampler)
		{
			SetHSamplerState(NULL, i);
		}
	}
}
void VSRenderer::SetCTexAllState(VSTexAllState * pTexAllState, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_COMPUTE]);
	bool bSetSampler = true;
	uint32 iMinShaderSampler = m_pCShader->GetMinShareSampler();
	if ((iMinShaderSampler != INVALID_SIMPLAR_REGISTER && i >= iMinShaderSampler) || i >= m_uiMaxSampler[VSEngineFlag::ST_COMPUTE])
	{
		bSetSampler = false;
	}
	if (pTexAllState)
	{
		SetCTexture(pTexAllState->m_pTex, i);		
		if (bSetSampler)
		{
			SetCSamplerState(pTexAllState->m_pSamplerState, i);
		}
	}
	else
	{
		SetCTexture(NULL, i);
		if (bSetSampler)
		{
			SetCSamplerState(NULL, i);
		}
	}
}
void VSRenderer::SetPTexAllState(VSTexAllState * pTexAllState, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_PIXEL]);
	bool bSetSampler = true;
	uint32 iMinShaderSampler = m_pPShader->GetMinShareSampler();
	if ((iMinShaderSampler != INVALID_SIMPLAR_REGISTER && i >= iMinShaderSampler) || i >= m_uiMaxSampler[VSEngineFlag::ST_PIXEL])
	{
		bSetSampler = false;
	}
	if (pTexAllState)
	{
		SetPTexture(pTexAllState->m_pTex, i);
		
		if (bSetSampler)
		{
			SetPSamplerState(pTexAllState->m_pSamplerState, i);
		}
	}
	else
	{
		SetPTexture(NULL, i);
		if (bSetSampler)
		{
			SetPSamplerState(NULL, i);
		}
	}
}
void VSRenderer::SetVSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b)
{
	if (!pSlotResource)
	{
		return;
	}
	if (b)
	{
		pSlotResource->m_VSTexSlot.AddElement(uiSlot);
	}
	else
	{
		pSlotResource->m_VSTexSlot.Erase(pSlotResource->m_VSTexSlot.FindElement(uiSlot));
	}

}
void VSRenderer::SetPSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b)
{
	if (!pSlotResource)
	{
		return;
	}
	if (b)
	{
		pSlotResource->m_PSTexSlot.AddElement(uiSlot);
	}
	else
	{
		pSlotResource->m_PSTexSlot.Erase(pSlotResource->m_PSTexSlot.FindElement(uiSlot));
	}
}
void VSRenderer::SetGSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b)
{
	if (!pSlotResource)
	{
		return;
	}
	if (b)
	{
		pSlotResource->m_GSTexSlot.AddElement(uiSlot);
	}
	else
	{
		pSlotResource->m_GSTexSlot.Erase(pSlotResource->m_GSTexSlot.FindElement(uiSlot));
	}
}
void VSRenderer::SetDSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b)
{
	if (!pSlotResource)
	{
		return;
	}
	if (b)
	{
		pSlotResource->m_DSTexSlot.AddElement(uiSlot);
	}
	else
	{
		pSlotResource->m_DSTexSlot.Erase(pSlotResource->m_DSTexSlot.FindElement(uiSlot));
	}
}
void VSRenderer::SetHSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b)
{
	if (!pSlotResource)
	{
		return;
	}
	if (b)
	{
		pSlotResource->m_HSTexSlot.AddElement(uiSlot);
	}
	else
	{
		pSlotResource->m_HSTexSlot.Erase(pSlotResource->m_HSTexSlot.FindElement(uiSlot));
	}
}
void VSRenderer::SetCSResourceSlotUsed(VSSlot * pSlotResource, uint32 uiSlot, bool b)
{
	if (!pSlotResource)
	{
		return;
	}
	if (b)
	{
		pSlotResource->m_CSTexSlot.AddElement(uiSlot);
	}
	else
	{
		pSlotResource->m_CSTexSlot.Erase(pSlotResource->m_CSTexSlot.FindElement(uiSlot));
	}
}
bool VSRenderer::CheckIsResourceCanSet(VSSlot * pSlotResource)
{
	if (!pSlotResource)
	{
		return true;
	}
	if (pSlotResource->GetOutputType() != VSOutputResource::OT_NONE)
	{
		if (pSlotResource->m_bBindResourceUse == true)
		{
			VSMAC_ASSERT(0);
			return false;
		}
		else
		{
			return true;
		}

	}
	return true;
}
uint32 VSRenderer::SetVTexture(VSTexture* pTexture, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_VERTEX]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pTexture))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pVTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}
	}
	if (!pTexture)
	{
		SetVSResourceSlotUsed(m_pVTex[i],i,false);
		m_pVTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pVTex[i] = NULL;
		return FRI_FAIL;
	}
	SetVSResourceSlotUsed(m_pVTex[i],i,false);
	SetVSResourceSlotUsed(pTexture,i,true);
	m_pVTex[i] = pTexture;
	return 1;
}
uint32 VSRenderer::SetDTexture(VSTexture* pTexture, uint32 i)
{

	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_DOMAIN]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pTexture))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pDTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetDSResourceSlotUsed(m_pDTex[i], i, false);
		m_pDTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pDTex[i] = NULL;
		return FRI_FAIL;
	}

	SetDSResourceSlotUsed(m_pDTex[i], i, false);
	SetDSResourceSlotUsed(pTexture, i, true);

	m_pDTex[i] = pTexture;
	return 1;
}
uint32 VSRenderer::SetHTexture(VSTexture* pTexture, uint32 i)
{

	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_HULL]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pTexture))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pHTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetHSResourceSlotUsed(m_pHTex[i], i, false);
		m_pHTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pHTex[i] = NULL;
		return FRI_FAIL;
	}

	SetHSResourceSlotUsed(m_pHTex[i], i, false);
	SetHSResourceSlotUsed(pTexture, i, true);

	m_pHTex[i] = pTexture;
	return 1;
}
void VSRenderer::Dispath(uint32 uiThreadGroupCountX, uint32 uiThreadGroupCountY,
	uint32 uiThreadGroupCountZ)
{

}
uint32 VSRenderer::SetCBufferResource(VSBufferResource* pBufferResource, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_COMPUTE]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pBufferResource))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pCTex[i] == pBufferResource)
	{
		if (!pBufferResource)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pBufferResource && pBufferResource->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pBufferResource)
	{
		SetCSResourceSlotUsed(m_pCTex[i], i, false);
		m_pCTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadBufferResource(pBufferResource))
	{
		m_pCTex[i] = NULL;
		return FRI_FAIL;
	}

	SetCSResourceSlotUsed(m_pCTex[i], i, false);
	SetCSResourceSlotUsed(pBufferResource, i, true);

	m_pCTex[i] = pBufferResource;
	return 1;
}
uint32 VSRenderer::SetVBufferResource(VSBufferResource* pBufferResource, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_VERTEX]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pBufferResource))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pVTex[i] == pBufferResource)
	{
		if (!pBufferResource)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pBufferResource && pBufferResource->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pBufferResource)
	{
		SetVSResourceSlotUsed(m_pVTex[i], i, false);
		m_pVTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadBufferResource(pBufferResource))
	{
		m_pVTex[i] = NULL;
		return FRI_FAIL;
	}

	SetVSResourceSlotUsed(m_pVTex[i], i, false);
	SetVSResourceSlotUsed(pBufferResource, i, true);

	m_pVTex[i] = pBufferResource;
	return 1;
}
uint32 VSRenderer::SetPBufferResource(VSBufferResource* pBufferResource, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_PIXEL]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pBufferResource))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pPTex[i] == pBufferResource)
	{
		if (!pBufferResource)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pBufferResource && pBufferResource->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pBufferResource)
	{
		SetPSResourceSlotUsed(m_pPTex[i], i, false);
		m_pPTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadBufferResource(pBufferResource))
	{
		m_pPTex[i] = NULL;
		return FRI_FAIL;
	}

	SetPSResourceSlotUsed(m_pPTex[i], i, false);
	SetPSResourceSlotUsed(pBufferResource, i, true);

	m_pPTex[i] = pBufferResource;
	return 1;
}
uint32 VSRenderer::SetGBufferResource(VSBufferResource* pBufferResource, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_GEOMETRY]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pBufferResource))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pGTex[i] == pBufferResource)
	{
		if (!pBufferResource)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pBufferResource && pBufferResource->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pBufferResource)
	{
		SetGSResourceSlotUsed(m_pGTex[i], i, false);
		m_pGTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadBufferResource(pBufferResource))
	{
		m_pGTex[i] = NULL;
		return FRI_FAIL;
	}

	SetGSResourceSlotUsed(m_pGTex[i], i, false);
	SetGSResourceSlotUsed(pBufferResource, i, true);

	m_pGTex[i] = pBufferResource;
	return 1;
}
uint32 VSRenderer::SetHBufferResource(VSBufferResource* pBufferResource, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_HULL]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pBufferResource))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pHTex[i] == pBufferResource)
	{
		if (!pBufferResource)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pBufferResource && pBufferResource->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pBufferResource)
	{
		SetHSResourceSlotUsed(m_pHTex[i], i, false);
		m_pHTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadBufferResource(pBufferResource))
	{
		m_pHTex[i] = NULL;
		return FRI_FAIL;
	}

	SetHSResourceSlotUsed(m_pHTex[i], i, false);
	SetHSResourceSlotUsed(pBufferResource, i, true);

	m_pHTex[i] = pBufferResource;
	return 1;
}
uint32 VSRenderer::SetDBufferResource(VSBufferResource* pBufferResource, uint32 i)
{
	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_DOMAIN]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pBufferResource))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pDTex[i] == pBufferResource)
	{
		if (!pBufferResource)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pBufferResource && pBufferResource->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pBufferResource)
	{
		SetDSResourceSlotUsed(m_pDTex[i], i, false);
		m_pDTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadBufferResource(pBufferResource))
	{
		m_pDTex[i] = NULL;
		return FRI_FAIL;
	}

	SetDSResourceSlotUsed(m_pDTex[i], i, false);
	SetDSResourceSlotUsed(pBufferResource, i, true);

	m_pDTex[i] = pBufferResource;
	return 1;
}
uint32 VSRenderer::SetCTexture(VSTexture* pTexture, uint32 i)
{

	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_COMPUTE]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pTexture))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pCTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetCSResourceSlotUsed(m_pCTex[i], i, false);
		m_pCTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pCTex[i] = NULL;
		return FRI_FAIL;
	}

	SetCSResourceSlotUsed(m_pCTex[i], i, false);
	SetCSResourceSlotUsed(pTexture, i, true);

	m_pCTex[i] = pTexture;
	return 1;
}
uint32 VSRenderer::SetGTexture(VSTexture* pTexture, uint32 i)
{

	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_GEOMETRY]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pTexture))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pGTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetGSResourceSlotUsed(m_pGTex[i], i, false);
		m_pGTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pGTex[i] = NULL;
		return FRI_FAIL;
	}

	SetGSResourceSlotUsed(m_pGTex[i], i, false);
	SetGSResourceSlotUsed(pTexture, i, true);

	m_pGTex[i] = pTexture;
	return 1;
}
uint32 VSRenderer::SetPTexture(VSTexture* pTexture, uint32 i)
{

	VSMAC_ASSERT(i < m_uiMaxBindResource[VSEngineFlag::ST_PIXEL]);
#ifdef _DEBUG
	if (!CheckIsResourceCanSet(pTexture))
	{
		return FRI_FAIL;
	}
#endif
	if (m_pPTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetPSResourceSlotUsed(m_pPTex[i],i,false);
		m_pPTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pPTex[i] = NULL;
		return FRI_FAIL;
	}

	SetPSResourceSlotUsed(m_pPTex[i],i,false);
	SetPSResourceSlotUsed(pTexture,i,true);
	
	m_pPTex[i] = pTexture;
	return 1;
}
uint32 VSRenderer::SetPSamplerState(VSSamplerState * pSamplerState, uint32 i, bool bForceSet)
{
	VSMAC_ASSERT(i < m_uiMaxSampler[VSEngineFlag::ST_PIXEL]);
	if (!pSamplerState)
	{
		pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (!m_pSamplerState[VSEngineFlag::ST_PIXEL][i])
	{
		m_pSamplerState[VSEngineFlag::ST_PIXEL][i] = (VSSamplerState *)VSSamplerState::GetDefault();
	}
	if (m_pSamplerState[VSEngineFlag::ST_PIXEL][i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
uint32 VSRenderer::SetBlendState(VSBlendState * pBlendState, bool bForceSet)
{
	if (!pBlendState)
	{
		pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	}
	if (!m_pBlendState)
	{
		m_pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	}
	if (m_pBlendState == pBlendState && !bForceSet)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadBlendState(pBlendState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
uint32 VSRenderer::SetDepthStencilState(VSDepthStencilState * pDepthStencilState, bool bForceSet)
{
	if (!pDepthStencilState)
	{
		pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	}
	if (!m_pDepthStencilState)
	{
		m_pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	}
	if (m_pDepthStencilState == pDepthStencilState && !bForceSet)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadDepthStencilState(pDepthStencilState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
uint32 VSRenderer::SetRasterizerState(VSRasterizerState * pRasterizerState, bool bForceSet)
{
	if (!pRasterizerState)
	{
		pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
	}
	if (!m_pRasterizerState)
	{
		m_pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
	}
	if (m_pRasterizerState == pRasterizerState && !bForceSet)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadRasterizerState(pRasterizerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
void VSRenderer::SetClipPlane(VSArray<Plane> &Plane, bool bEnable)
{

}
void VSRenderer::SetScissorRect(VSArray<VSRect2> &Rect, bool bEnable)
{

}
VSRenderer::VSRenderer()
{

	m_pChildWindowInfo = NULL;
	m_ClearColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
	m_fClearDepth = 1.0f;
	m_uiClearStencil = 0;

	m_iNumChildWindow = 0;
	m_iCurWindowID = -1;
	m_bIsRendering = false;



	m_pBlendState = (VSBlendState *)VSBlendState::GetDefault();
	m_pDepthStencilState = (VSDepthStencilState *)VSDepthStencilState::GetDefault();
	m_pRasterizerState = (VSRasterizerState *)VSRasterizerState::GetDefault();
	m_bClipPlaneEnable = false;
	m_bScissorRectEnable = false;
	m_pFont = NULL;//VSFont::ms_Default;

	for (uint32 i = 0; i < STREAM_LEVEL; i++)
	{
		m_pVertexBuffer[i] = NULL;
	}
	m_pVertexFormat = NULL;
	m_pIndexBuffer = NULL;
	m_pQuery = NULL;
	m_pVShader = NULL;
	m_pPShader = NULL;
	m_pGShader = NULL;
	m_pDShader = NULL;
	m_pHShader = NULL;
	m_pCShader = NULL;
	m_pQuadVertexFormat = NULL;
	m_pFontVertexFormat = NULL;

	m_uiRenderStateInheritFlag = 0;
	VSRenderer::ms_pRenderer = this;
	m_uiBackMacthType = BMT_NONE;

	for (uint32 i = 0; i < SupportFeatureType::SF_MAX; i++)
	{
		m_SupportFeature[i] = false;
	}
}
bool VSRenderer::UseWindow(int32 uiWindowID)
{
	VSMAC_ASSERT(uiWindowID < m_iNumChildWindow);
	VSMAC_ASSERT(uiWindowID >= -1);

	m_iCurWindowID = uiWindowID;
	return 1;
}
bool VSRenderer::BeginRendering()
{
	ClearBuffers();
	m_FontVertex.Clear();
	m_bIsRendering = true;
	return true;
}
bool VSRenderer::EndRendering()
{
	if (m_FontVertex.GetNum() > 0)
	{
		SetViewPort(NULL);
		VSFont * pUseFont = m_pFont->GetResource();
		VSCustomMaterial *pFontMaterial = VSCustomMaterial::GetFont();
		pFontMaterial->SetPShaderTexture(VSUsedName::ms_cPostInputTexture[0], pUseFont->GetTexAllState());
		pFontMaterial->UseMaterial();
		uint32 uiLength = m_FontVertex.GetNum() >> 2;
		DrawScreenFont(m_FontVertex.GetBuffer(), uiLength * 4, m_FontIndex.GetBuffer(), uiLength * 2 * 3);
	}	
	m_bIsRendering = false;
	VSMAC_ASSERT(m_uiBackMacthType == BMT_NONE);
	m_uiBackMacthType = BMT_NONE;
	return true;
}
void VSRenderer::DeviceLost()
{

}
void VSRenderer::ResetDevice()
{

}
bool VSRenderer::ChangeScreenSize(uint32 uiWidth, uint32 uiHeight, bool bWindow)
{
	if (m_uiScreenHeight == uiHeight && m_uiScreenWidth == uiWidth && m_bWindowed == bWindow)
	{


		return false;
	}
	if (!bWindow)
	{
		uint32 uiAdpterInfoNum = 0;
		const VSAdapterInfo * pAdapterInfo = GetAdapterInfo(uiAdpterInfoNum);
		if (!pAdapterInfo || !uiAdpterInfoNum)
		{
			return false;
		}
		bool bFlag = false;
		for (uint32 i = 0; i < pAdapterInfo[0].DisplayMode.GetNum(); i++)
		{
			if (pAdapterInfo[0].DisplayMode[i].uiHeight == uiHeight
				&& pAdapterInfo[0].DisplayMode[i].uiWidth == uiWidth
				&& pAdapterInfo[0].DisplayMode[i].uiBitFormat == VSRenderer::SFT_A8R8G8B8)
			{
				bFlag = true;
				break;
			}
		}
		if (!bFlag)
		{
			return false;
		}
	}


	m_bWindowed = bWindow;
	m_uiScreenHeight = uiHeight;
	m_uiScreenWidth = uiWidth;
	m_uiCurRTWidth = uiWidth;
	m_uiCurRTHeight = uiHeight;
	return true;
}
bool VSRenderer::DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4])
{
	SetVertexFormat(m_pQuadVertexFormat);
	m_pVertexBuffer[0] = NULL;
	m_pIndexBuffer = NULL;
	return 1;
}
bool VSRenderer::DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, uint32 uiVertexNum,
	MATH_USHORT_INDEX * pIndexBuffer, uint32 uiIndexNum)
{
	VSMAC_ASSERT(pScreenBuffer && uiVertexNum && pIndexBuffer && uiIndexNum);
	SetVertexFormat(m_pQuadVertexFormat);
	m_pVertexBuffer[0] = NULL;
	m_pIndexBuffer = NULL;
	return 1;
}
bool VSRenderer::DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, uint32 uiVertexNum,
	MATH_USHORT_INDEX * pIndexBuffer, uint32 uiIndexNum)
{
	VSMAC_ASSERT(pScreenFontBuffer && uiVertexNum && pIndexBuffer && uiIndexNum);
	SetVertexFormat(m_pFontVertexFormat);
	m_pVertexBuffer[0] = NULL;
	m_pIndexBuffer = NULL;
	return 1;
}
VSString VSRenderer::Float(uint32 uiIndex)const
{
	if (uiIndex == FI_1)
	{
		return Float();
	}
	else if (uiIndex == FI_2)
	{
		return Float2();
	}
	else if (uiIndex == FI_3)
	{
		return Float3();
	}
	else if (uiIndex == FI_4)
	{
		return Float4();
	}
	else
		VSMAC_ASSERT(1);
	return VSString();


}
bool VSRenderer::SetViewPort(VSViewPort *pViewPort)
{
	if (!pViewPort)
	{
		m_CurViewPort.Reset();
	}
	return true;
}
void VSRenderer::DrawText(int32 iX, int32 iY, const uint32 rColor,
	const char* acText, ...)
{
	VSMAC_ASSERT(acText && m_pFont);
	VSFont * pUseFont = m_pFont->GetResource();
	char cch[1024];
	char *pArgs;
	pArgs = (char*)&acText + sizeof(acText);
	VSSprintf(cch, 1024, acText, pArgs);
	VSString VSString = cch;
	
	uint32 uiLength = VSString.GetLength();
	uint32 uiRTWidth = GetCurRTWidth();
	uint32 uiRTHeight = GetCurRTHeight();
	
	float InvRTWidth = 1.0f / uiRTWidth;
	float InvRTHeight = 1.0f / uiRTHeight;
	float fLeft = iX  * InvRTWidth;
	float fTop = 1.0f - iY * InvRTHeight;

	fLeft = fLeft * 2.0f - 1.0f;
	fTop = fTop * 2.0f - 1.0f;

	float fOriginalLeft = fLeft;
	float fGlyphSizeY = pUseFont->GetTexAllState()->m_pTex->GetHeight(0) * InvRTHeight/* * 2.0f*/;

	float fTextureWidth = pUseFont->GetTexAllState()->m_pTex->GetWidth(0) * 1.0f;
	
	for (uint32 i = 0; i < uiLength; i++)
	{
		if (VSString[i] < VSFont::ASCII_START || VSString[i] > VSFont::ASCII_END)
		{
			continue;
		}
		if (VSString[i] == '\n')
		{
			fLeft = fOriginalLeft;
			fTop -= fGlyphSizeY;
			continue;
		}
		unsigned char CharIndex = VSString[i] - VSFont::ASCII_START;

		SCREEN_FONT_TYPE Vertex[4];
		float Left = pUseFont->GetFontData(CharIndex);
		float Right = pUseFont->GetFontData(CharIndex + 1);
		float fGlyphSizeX = (Right - Left) * fTextureWidth * InvRTWidth /**2.0f*/;  // in pixels
		Vertex[0].Point = Vector3W(fLeft, fTop, 0.0f,1.0f);
		Vertex[1].Point = Vector3W(fLeft + fGlyphSizeX, fTop, 0.0f, 1.0f);
		Vertex[2].Point = Vector3W(fLeft + fGlyphSizeX, fTop - fGlyphSizeY, 0.0f, 1.0f);
		Vertex[3].Point = Vector3W(fLeft, fTop - fGlyphSizeY, 0.0f, 1.0f);

		
		fLeft += fGlyphSizeX;

		Vertex[0].UV = Vector2(Left,0.0f);
		Vertex[1].UV = Vector2(Right, 0.0f);
		Vertex[2].UV = Vector2(Right, 1.0f);
		Vertex[3].UV = Vector2(Left, 1.0f);;

		Vertex[0].Color = rColor;
		Vertex[1].Color = rColor;
		Vertex[2].Color = rColor;
		Vertex[3].Color = rColor;

		m_FontVertex.AddElement(Vertex[0]);
		m_FontVertex.AddElement(Vertex[1]);
		m_FontVertex.AddElement(Vertex[2]);
		m_FontVertex.AddElement(Vertex[3]);
		
	}
	uint32 IndexFontNum = m_FontIndex.GetNum() / 6;
	if (IndexFontNum < (m_FontVertex.GetNum() >> 2))
	{
		for (uint32 i = IndexFontNum; i < (m_FontVertex.GetNum() >> 2); i++)
		{
			uint32 Index = i * 4;
			m_FontIndex.AddElement(Index);
			m_FontIndex.AddElement(Index + 1);
			m_FontIndex.AddElement(Index + 3);

			m_FontIndex.AddElement(Index + 1);
			m_FontIndex.AddElement(Index + 2);
			m_FontIndex.AddElement(Index + 3);
		}
	}
	
}
void VSRenderer::SetFont(VSFontR * pFont)
{
	VSMAC_ASSERT(pFont);
	m_pFont = pFont;
}
uint32 VSRenderer::SetVShader(VSVShader * pVShader)
{
	if (!pVShader)
		return FRI_FAIL;

	if (pVShader == m_pVShader)
	{

		SetVShaderConstant(pVShader);
		SetVShaderBindResource(pVShader);

		return FRI_SUCCESS;
	}
	if (!SetVShaderProgram(pVShader))
		return FRI_FAIL;

	m_pVShader = pVShader;
	SetVShaderConstant(pVShader);
	SetVShaderBindResource(pVShader);
	return FRI_SUCCESS;
}
uint32 VSRenderer::SetDShader(VSDShader * pDShader)
{
	if (m_pDShader == pDShader)
	{
		if (pDShader == NULL)
		{
			return FRI_SUCCESS;
		}
		SetDShaderConstant(pDShader);
		SetDShaderBindResource(pDShader);
		return FRI_SUCCESS;
	}
	SetDShaderProgram(pDShader);

	m_pDShader = pDShader;
	if (pDShader == NULL)
	{
		return FRI_SUCCESS;
	}
	SetDShaderConstant(pDShader);
	SetDShaderBindResource(pDShader);
	return FRI_SUCCESS;
}
uint32 VSRenderer::SetCShader(VSCShader * pCShader)
{

	if (m_pCShader == pCShader)
	{
		if (pCShader == NULL)
		{
			return FRI_SUCCESS;
		}
		SetCShaderConstant(pCShader);
		SetCShaderBindResource(pCShader);
		return FRI_SUCCESS;
	}
	SetCShaderProgram(pCShader);

	m_pCShader = pCShader;
	if (pCShader == NULL)
	{
		return FRI_SUCCESS;
	}
	SetCShaderConstant(pCShader);
	SetCShaderBindResource(pCShader);
	return FRI_SUCCESS;
}
uint32 VSRenderer::SetHShader(VSHShader * pHShader)
{
	if (m_pHShader == pHShader)
	{
		if (pHShader == NULL)
		{
			return FRI_SUCCESS;
		}
		SetHShaderConstant(pHShader);
		SetHShaderBindResource(pHShader);
		return FRI_SUCCESS;
	}
	SetHShaderProgram(pHShader);

	m_pHShader = pHShader;
	if (pHShader == NULL)
	{
		return FRI_SUCCESS;
	}
	SetHShaderConstant(pHShader);
	SetHShaderBindResource(pHShader);
	return FRI_SUCCESS;
}
uint32 VSRenderer::SetGShader(VSGShader * pGShader)
{
	if (m_pGShader == pGShader)
	{
		if (pGShader == NULL)
		{
			return FRI_SUCCESS;
		}
		SetGShaderConstant(pGShader);
		SetGShaderBindResource(pGShader);
		return FRI_SUCCESS;
	}
	SetGShaderProgram(pGShader);

	m_pGShader = pGShader;
	if (pGShader == NULL)
	{
		return FRI_SUCCESS;
	}
	SetGShaderConstant(pGShader);
	SetGShaderBindResource(pGShader);
	return FRI_SUCCESS;
}
uint32 VSRenderer::SetPShader(VSPShader * pPShader)
{
	if (!pPShader)
		return FRI_FAIL;

	if (m_pPShader == pPShader)
	{
		SetPShaderConstant(pPShader);
		SetPShaderBindResource(pPShader);
		return FRI_SUCCESS;
	}
	if (!SetPShaderProgram(pPShader))
		return FRI_FAIL;

	m_pPShader = pPShader;
	SetPShaderConstant(pPShader);
	SetPShaderBindResource(pPShader);
	return FRI_SUCCESS;
}
void VSRenderer::SetVShaderBindResource(VSVShader * pShader)
{
	m_pTexAllStateBuffer.Clear();
	m_pBufferResource.Clear();
	if (pShader->m_bCreatePara)
	{
		for (uint32 uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			VSMAC_ASSERT(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_VERTEX])
			{
				for (uint32 i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{

					m_pTexAllStateBuffer.AddElement(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i, pShader->m_pUserSampler[uiTexid]->GetTex(i));
				}
			}

		}
		for (uint32 uiBufferid = 0; uiBufferid < pShader->m_pUserBuffer.GetNum(); uiBufferid++)
		{
			VSMAC_ASSERT(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_VERTEX])
			{
				for (uint32 i = 0; i < pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum(); i++)
				{
					m_pBufferResource.AddElement(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + i, pShader->m_pUserBuffer[uiBufferid]->GetBufferResource(i));
				}
			}
		}
	}
	for (uint32 i = 0; i < m_pTexAllStateBuffer.GetNum(); i++)
	{
		SetVTexAllState(m_pTexAllStateBuffer[i].Value, m_pTexAllStateBuffer[i].Key);
	}
	for (uint32 i = 0; i < pShader->m_uiShareSamplerRegister.GetNum(); i++)
	{
		SetVSamplerState((VSSamplerState *)VSSamplerState::GetShareSampler(pShader->m_uiShareSamplerRegister[i].Value), pShader->m_uiShareSamplerRegister[i].Key);
	}
	for (uint32 i = 0; i < m_pBufferResource.GetNum(); i++)
	{
		SetVBufferResource(m_pBufferResource[i].Value, m_pBufferResource[i].Key);
	}
}

void VSRenderer::SetPShaderBindResource(VSPShader * pShader)
{
	m_pTexAllStateBuffer.Clear();
	m_pBufferResource.Clear();
	if (pShader->m_bCreatePara)
	{
		for (uint32 uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			VSMAC_ASSERT(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_PIXEL])
			{
				for (uint32 i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer.AddElement(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i, pShader->m_pUserSampler[uiTexid]->GetTex(i));
				}
			}

		}
		for (uint32 uiBufferid = 0; uiBufferid < pShader->m_pUserBuffer.GetNum(); uiBufferid++)
		{
			VSMAC_ASSERT(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_PIXEL])
			{
				for (uint32 i = 0; i < pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum(); i++)
				{
					m_pBufferResource.AddElement(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + i, pShader->m_pUserBuffer[uiBufferid]->GetBufferResource(i));
				}
			}
		}
	}
	for (uint32 i = 0; i < m_pTexAllStateBuffer.GetNum(); i++)
	{
		SetPTexAllState(m_pTexAllStateBuffer[i].Value, m_pTexAllStateBuffer[i].Key);
	}
	for (uint32 i = 0; i < pShader->m_uiShareSamplerRegister.GetNum(); i++)
	{
		SetPSamplerState((VSSamplerState *)VSSamplerState::GetShareSampler(pShader->m_uiShareSamplerRegister[i].Value), pShader->m_uiShareSamplerRegister[i].Key);
	}
	for (uint32 i = 0; i < m_pBufferResource.GetNum(); i++)
	{
		SetPBufferResource(m_pBufferResource[i].Value, m_pBufferResource[i].Key);
	}
}
void VSRenderer::SetDShaderBindResource(VSDShader * pShader)
{
	m_pTexAllStateBuffer.Clear();
	m_pBufferResource.Clear();
	if (pShader->m_bCreatePara)
	{
		for (uint32 uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			VSMAC_ASSERT(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_DOMAIN])
			{
				for (uint32 i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer.AddElement(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i, pShader->m_pUserSampler[uiTexid]->GetTex(i));
				}
			}

		}
		for (uint32 uiBufferid = 0; uiBufferid < pShader->m_pUserBuffer.GetNum(); uiBufferid++)
		{
			VSMAC_ASSERT(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_DOMAIN])
			{
				for (uint32 i = 0; i < pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum(); i++)
				{
					m_pBufferResource.AddElement(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + i, pShader->m_pUserBuffer[uiBufferid]->GetBufferResource(i));
				}
			}
		}
	}
	for (uint32 i = 0; i < m_pTexAllStateBuffer.GetNum(); i++)
	{
		SetDTexAllState(m_pTexAllStateBuffer[i].Value, m_pTexAllStateBuffer[i].Key);
	}
	for (uint32 i = 0; i < pShader->m_uiShareSamplerRegister.GetNum(); i++)
	{
		SetDSamplerState((VSSamplerState *)VSSamplerState::GetShareSampler(pShader->m_uiShareSamplerRegister[i].Value), pShader->m_uiShareSamplerRegister[i].Key);
	}
	for (uint32 i = 0; i < m_pBufferResource.GetNum(); i++)
	{
		SetDBufferResource(m_pBufferResource[i].Value, m_pBufferResource[i].Key);
	}
}
void VSRenderer::SetCShaderBindResource(VSCShader * pShader)
{
	m_pTexAllStateBuffer.Clear();
	m_pBufferResource.Clear();
	if (pShader->m_bCreatePara)
	{
		for (uint32 uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			VSMAC_ASSERT(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_COMPUTE])
			{
				for (uint32 i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer.AddElement(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i, pShader->m_pUserSampler[uiTexid]->GetTex(i));
				}
			}

		}
		for (uint32 uiBufferid = 0; uiBufferid < pShader->m_pUserBuffer.GetNum(); uiBufferid++)
		{
			VSMAC_ASSERT(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_COMPUTE])
			{
				for (uint32 i = 0; i < pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum(); i++)
				{
					m_pBufferResource.AddElement(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + i, pShader->m_pUserBuffer[uiBufferid]->GetBufferResource(i));
				}
			}
		}
	}
	for (uint32 i = 0; i < m_pTexAllStateBuffer.GetNum(); i++)
	{
		SetCTexAllState(m_pTexAllStateBuffer[i].Value, m_pTexAllStateBuffer[i].Key);
	}
	for (uint32 i = 0; i < pShader->m_uiShareSamplerRegister.GetNum(); i++)
	{
		SetCSamplerState((VSSamplerState *)VSSamplerState::GetShareSampler(pShader->m_uiShareSamplerRegister[i].Value), pShader->m_uiShareSamplerRegister[i].Key);
	}
	for (uint32 i = 0; i < m_pBufferResource.GetNum(); i++)
	{
		SetCBufferResource(m_pBufferResource[i].Value, m_pBufferResource[i].Key);
	}
}
void VSRenderer::SetHShaderBindResource(VSHShader * pShader)
{
	m_pTexAllStateBuffer.Clear();
	m_pBufferResource.Clear();
	if (pShader->m_bCreatePara)
	{
		for (uint32 uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			VSMAC_ASSERT(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_HULL])
			{
				for (uint32 i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer.AddElement(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i, pShader->m_pUserSampler[uiTexid]->GetTex(i));
				}
			}

		}
		for (uint32 uiBufferid = 0; uiBufferid < pShader->m_pUserBuffer.GetNum(); uiBufferid++)
		{
			VSMAC_ASSERT(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_HULL])
			{
				for (uint32 i = 0; i < pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum(); i++)
				{
					m_pBufferResource.AddElement(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + i, pShader->m_pUserBuffer[uiBufferid]->GetBufferResource(i));
				}
			}
		}
	}
	for (uint32 i = 0; i < m_pTexAllStateBuffer.GetNum(); i++)
	{
		SetHTexAllState(m_pTexAllStateBuffer[i].Value, m_pTexAllStateBuffer[i].Key);
	}
	for (uint32 i = 0; i < pShader->m_uiShareSamplerRegister.GetNum(); i++)
	{
		SetHSamplerState((VSSamplerState *)VSSamplerState::GetShareSampler(pShader->m_uiShareSamplerRegister[i].Value), pShader->m_uiShareSamplerRegister[i].Key);
	}
	for (uint32 i = 0; i < m_pBufferResource.GetNum(); i++)
	{
		SetHBufferResource(m_pBufferResource[i].Value, m_pBufferResource[i].Key);
	}
}
void VSRenderer::SetGShaderBindResource(VSGShader * pShader)
{
	m_pTexAllStateBuffer.Clear();
	m_pBufferResource.Clear();
	if (pShader->m_bCreatePara)
	{
		for (uint32 uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			VSMAC_ASSERT(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_GEOMETRY])
			{
				for (uint32 i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer.AddElement(pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i, pShader->m_pUserSampler[uiTexid]->GetTex(i));
				}
			}

		}
		for (uint32 uiBufferid = 0; uiBufferid < pShader->m_pUserBuffer.GetNum(); uiBufferid++)
		{
			VSMAC_ASSERT(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum() <= m_uiMaxBindResource[VSEngineFlag::ST_GEOMETRY])
			{
				for (uint32 i = 0; i < pShader->m_pUserBuffer[uiBufferid]->GetRegisterNum(); i++)
				{
					m_pBufferResource.AddElement(pShader->m_pUserBuffer[uiBufferid]->GetRegisterIndex() + i, pShader->m_pUserBuffer[uiBufferid]->GetBufferResource(i));
				}
			}
		}
	}
	for (uint32 i = 0; i < m_pTexAllStateBuffer.GetNum(); i++)
	{
		SetGTexAllState(m_pTexAllStateBuffer[i].Value, m_pTexAllStateBuffer[i].Key);
	}
	for (uint32 i = 0; i < pShader->m_uiShareSamplerRegister.GetNum(); i++)
	{
		SetGSamplerState((VSSamplerState *)VSSamplerState::GetShareSampler(pShader->m_uiShareSamplerRegister[i].Value), pShader->m_uiShareSamplerRegister[i].Key);
	}
	for (uint32 i = 0; i < m_pBufferResource.GetNum(); i++)
	{
		SetGBufferResource(m_pBufferResource[i].Value, m_pBufferResource[i].Key);
	}
}
void VSRenderer::GetShareSamplerDeclare(VSShader * pShader, VSString & OutString)
{
	if (!IsSupportFeature(SupportFeatureType::SF_ShareSampler))
	{
		return;
	}
	for (uint32 i = 0; i < VSEngineFlag::ms_uiShaderSamplerNum; i++)
	{
		if (VSShaderStringFactory::ms_uiShareSamplerRegisterID[i] != INVALID_SIMPLAR_REGISTER)
		{
			pShader->AddShareSampler(VSShaderStringFactory::ms_uiShareSamplerRegisterID[i], i);
		}
	}
	for (uint32 i = 0 ; i < pShader->m_uiShareSamplerRegister.GetNum() ; i++)
	{
		GetDeclareShareSampler(pShader->m_uiShareSamplerRegister[i].Value, pShader->m_uiShareSamplerRegister[i].Key, OutString);
	}
}
bool VSRenderer::OnReleaseResouce(VSResourceIdentifier * pResourceResourceIdentifier)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(VSOnReleaseResourceIdentifier,
		VSResourceIdentifier *, pResourceResourceIdentifier, pResourceResourceIdentifier,
		{
		SAFE_DELETE(pResourceResourceIdentifier);
		})
		return true;
}

}