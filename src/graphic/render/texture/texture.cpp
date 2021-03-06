#include "graphic/render/texture/texture.h"
#include "graphic/material/shaderstringfactory.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI_NoCreateFun(VSTexture,VSBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSTexture)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSTexture,VSBind)
REGISTER_PROPERTY(m_uiFormatType,FormatType,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiWidth,Width,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiHeight,Height,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiLength,Lenght,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiArraySize, ArraySize, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiMipLevel,MipLevel,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bSRGB, bSRGB, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_DataBufferArray, DataBufferArray, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSTexture::VSTexture(uint32 uiFormatType,uint32 uiWidth,
					 uint32 uiHeight,uint32 uiLength, uint32 uiArraySize,
					uint32 uiMipLevel,bool bIsStatic, bool bSRGB)
{
	VSMAC_ASSERT(uiWidth && uiHeight);
	VSMAC_ASSERT(uiFormatType < VSRenderer::SFT_MAX);
	m_uiFormatType = uiFormatType;
	m_uiHeight = uiHeight;
	m_uiWidth = uiWidth;
	m_uiLength = uiLength;
	for (uint32 i = 0 ; i < MAX_MIP_LEVEL ; i++)
	{
		for (uint32 j = 0 ; j < 6 ; j++)
		{
			m_pLockData[i][j] = NULL;
		}
	}

	m_bIsStatic = bIsStatic;
	m_uiMipLevel = uiMipLevel;
	m_bSRGB = bSRGB;
	m_uiArraySize = uiArraySize;
}
VSTexture::VSTexture()
{
	for (uint32 i = 0 ; i < MAX_MIP_LEVEL ; i++)
	{
		for (uint32 j = 0 ; j < 6 ; j++)
		{
			m_pLockData[i][j] = NULL;
		}
	}
	m_bSRGB = false;
	m_uiArraySize = 1;
	m_bIsStatic = true;
	m_uiFormatType = VSRenderer::SFT_A8R8G8B8;
	m_uiHeight = 1;
	m_uiWidth = 1;
	m_uiLength = 1;
	m_uiMipLevel = 1;
}
VSTexture::~VSTexture()
{
	ReleaseResource();
}

void VSTexture::ClearInfo()
{
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
	{
		if (m_uiMemType == MT_VRAM)
		{
			m_DataBufferArray.Destroy();
		}
	}

}
bool VSTexture::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadTexture(this,pID))
		return 0;
	return 1;

}
void *VSTexture::Lock(uint32 uiLevel,uint32 uiFace)
{

	if (m_bIsStatic || !m_pUser)
	{
		return NULL;
	}

	VSMAC_ASSERT(m_pLockData[uiLevel][uiFace] == NULL);
	m_pLockData[uiLevel][uiFace] = m_pUser->Lock(this,uiLevel,uiFace);


	return m_pLockData[uiLevel][uiFace];
}
void VSTexture::UnLock(uint32 uiLevel,uint32 uiFace)
{
	if (m_bIsStatic || !m_pUser)
	{
		return;
	}

	VSMAC_ASSERT(m_pLockData[uiLevel][uiFace]);
	m_pUser->UnLock(this,uiLevel,uiFace);

	m_pLockData[uiLevel][uiFace] = NULL;
}
void VSTexture::CopyRAMData(const VSTexture* pSourceTexture)
{	
	uint32 MipDelta = pSourceTexture->GetMipLevel() - GetMipLevel();
	m_DataBufferArray.SetBufferNum(GetMipLevel());
	for (uint32 i = 0; i < GetMipLevel(); i++)
	{
		m_DataBufferArray[i] = pSourceTexture->m_DataBufferArray[i + MipDelta];
	}
}
void VSTexture::CreateRAMData()
{
	m_DataBufferArray.Destroy();
	if (m_uiOutputType == VSOutputResource::OT_NONE)
	{
		m_DataBufferArray.SetBufferNum(m_uiMipLevel);
		for (uint32 i = 0; i < m_uiMipLevel; i++)
		{
			uint32 uiByteSize = GetByteSize(i);
			if (uiByteSize == 0)
			{
				continue;
			}
			m_DataBufferArray[i].SetBufferNum(uiByteSize * m_uiArraySize);
		}
	}
	
}
unsigned char * VSTexture::GetBuffer(uint32 uiLevel, uint32 uiFace)const
{
	if (uiLevel < m_uiMipLevel && uiFace < m_uiArraySize)
	{
		if (uiLevel >= m_DataBufferArray.GetNum() || !m_DataBufferArray[uiLevel].GetBuffer())
		{
			return NULL;
		}
		if (IsCompress())
		{
			return m_DataBufferArray[uiLevel].GetBuffer() + VSRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel))
				* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetHeight(uiLevel))
				* uiFace * VSRenderer::GetBytesPerPixel(m_uiFormatType);
		}
		else
		{
			return m_DataBufferArray[uiLevel].GetBuffer() + GetWidth(uiLevel) * GetHeight(uiLevel) * uiFace * VSRenderer::GetBytesPerPixel(m_uiFormatType);
		}

	}
	else
	{
		VSMAC_ASSERT(false);
		return NULL;
	}
}
uint32 VSTexture::GetByteSize()const
{
	uint32 uiSize = 0;

	for (uint32 i = 0; i < m_uiMipLevel; i++)
	{
		uiSize += GetByteSize(i);
	}
	return uiSize * m_uiArraySize;
}
uint32 VSTexture::GetByteSize(uint32 uiLevel)const
{
	if (uiLevel > m_uiMipLevel)
	{
		return 0;
	}
	if (IsCompress())
	{
		return VSRenderer::GetBytesPerPixel(m_uiFormatType)
			* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel))
			* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetHeight(uiLevel))
			* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetLength(uiLevel));
	}
	return VSRenderer::GetBytesPerPixel(m_uiFormatType) * GetWidth(uiLevel) * GetHeight(uiLevel) * GetLength(uiLevel);
}
uint32 VSTexture::GetMemPitch(uint32 uiLevel)const
{
	if (uiLevel > m_uiMipLevel)
	{
		return 0;
	}
	if (IsCompress())
	{
		return VSRenderer::GetBytesPerPixel(m_uiFormatType)
			* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel));
	}
	return VSRenderer::GetBytesPerPixel(m_uiFormatType) * GetWidth(uiLevel);
}
uint32 VSTexture::GetMemSlicePitch(uint32 uiLevel)const
{
	if (uiLevel > m_uiMipLevel)
	{
		return 0;
	}
	if (IsCompress())
	{
		return VSRenderer::GetBytesPerPixel(m_uiFormatType)
			* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel))
			* VSRenderer::ms_pRenderer->GetTextureCompressLen(GetHeight(uiLevel));
	}
	return VSRenderer::GetBytesPerPixel(m_uiFormatType) * GetWidth(uiLevel) * GetHeight(uiLevel);
}
uint32 VSTexture::GetWidth(uint32 uiLevel)const
{
	uint32 uiWidth = m_uiWidth >> uiLevel;
	uiWidth = Max(uiWidth, VSRenderer::GetMinDimension(m_uiFormatType));
	return uiWidth;
}
uint32 VSTexture::GetHeight(uint32 uiLevel)const
{
	uint32 uiHeight = m_uiHeight >> uiLevel;
	uiHeight = Max(uiHeight, VSRenderer::GetMinDimension(m_uiFormatType));

	return uiHeight;

}
uint32 VSTexture::GetLength(uint32 uiLevel)const
{
	uint32 uiLength = m_uiLength >> uiLevel;
	uiLength = Max(uiLength, VSRenderer::GetMinDimension(m_uiFormatType));
	return uiLength;
}
VSTextureOutputInfo * VSTexture::GetTextureOutputInfo(VSOutputResource * pOutputResource)
{
	if (pOutputResource->GetOutputType() == VSOutputResource::OT_RENDER_TARGET)
	{
		return ((VSTextureOutputInfo *)DynamicCast<VSRenderTarget>(pOutputResource));
	}
	else if (pOutputResource->GetOutputType() == VSOutputResource::OT_DEPTH_STENCIL)
	{
		return ((VSTextureOutputInfo *)DynamicCast<VSDepthStencil>(pOutputResource));
	}
	else if (pOutputResource->GetOutputType() == VSOutputResource::OT_TEXTURE_UNORDER_ACCESS)
	{
		return ((VSTextureOutputInfo *)DynamicCast<VSTextureUnorderAccess>(pOutputResource));
	}
	else
	{
		return NULL;
	}

}
bool VSTexture::SetOutput(class VSOutputResource *pOutputResource)
{
	VSMAC_ASSERT(pOutputResource && pOutputResource->GetOutputType() != VSOutputResource::OT_BUFFER_UNORDER_ACCESS);
	if (IsBindResource())
	{
		return false;
	}
	if (m_uiOutputType == VSOutputResource::OT_NONE)
	{
 		m_uiOutputType = pOutputResource->GetOutputType();
	}
	else if (m_uiOutputType != ((VSOutputResource *)pOutputResource)->GetOutputType())
	{
		return false;
	}
	const VSTextureOutputInfo * pTextureOutputInfo = GetTextureOutputInfo(pOutputResource);
	VSMAC_ASSERT(!m_pOutputResource[pTextureOutputInfo->GetLevel()]);
	m_pOutputResource[pTextureOutputInfo->GetLevel()] = pOutputResource;

	return true;
}
IMPLEMENT_RTTI(VSTextureCache, VSCacheResource)
BEGIN_ADD_PROPERTY(VSTextureCache, VSCacheResource)
REGISTER_PROPERTY(m_pTex, Tex, VSProperty::F_SAVE_LOAD)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTextureCache)
IMPLEMENT_INITIAL_END
VSTextureCache::VSTextureCache(VSObject * pCacheObject)
{
	m_pTex = DynamicCast<VSTexture>(pCacheObject);
}
VSTextureCache::~VSTextureCache()
{

}
bool VSTextureCache::SetCacheResource(VSObject * pOwner)
{
	VSTexAllState * pTex = DynamicCast<VSTexAllState>(pOwner);
	VSMAC_ASSERT(pTex);
	if (!m_pTex)
	{
		return false;
	}
	pTex->m_pTex = m_pTex;
	return true;
}
bool VSTextureCache::SetStreamResouce(VSResourceProxyBase* pOwner)
{
	pOwner->CreateToStreamObject(this);
	VSResourceProxy<VSTexAllState>* pRTexAll = (VSResourceProxy<VSTexAllState> *)(pOwner);
	VSMAC_ASSERT(pRTexAll);
	if (!m_pTex)
	{
		return false;
	}
	return true;
}