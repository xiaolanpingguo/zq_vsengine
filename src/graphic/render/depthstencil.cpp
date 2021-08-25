#include "graphic/render/depthstencil.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/render/texture/texture2d.h"
#include "graphic/render/texture/texturearray2d.h"
using namespace zq;
IMPLEMENT_RTTI(VSDepthStencil, VSOutputResource)
BEGIN_ADD_PROPERTY(VSDepthStencil, VSOutputResource)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDepthStencil)
IMPLEMENT_INITIAL_END
VSDepthStencil::VSDepthStencil()
{
	m_uiWidth = 512;
	m_uiHeight = 512;
	m_uiFormatType = VSRenderer::SFT_D24S8;
	m_uiMulSample = VSRenderer::MS_NONE;
	m_bUsed = false;
	m_pLockData = NULL;
	m_pCreateBy = NULL;
	m_uiFirst = 0;
	m_uiLevel = 0;
	m_uiArraySize = 1;
}
VSDepthStencil::VSDepthStencil(uint32 uiWidth,uint32 uiHeight,
							   uint32 uiMulSample,uint32 uiFormatType, bool CPURead)
{
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	m_uiMulSample = uiMulSample;
	m_uiFormatType = uiFormatType;
	m_pCreateBy = NULL;
	m_uiLevel = 0;
	m_uiFirst = 0;
	m_bIsStatic = !CPURead;
	m_bUsed = false;
	m_uiLockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	m_pLockData = NULL;
	m_uiArraySize = 1;
}
VSDepthStencil::VSDepthStencil(VS2DTexture * pCreateBy, uint32 uiMulSample, uint32 uiLevel,bool CPURead)
{	
	m_uiFirst = 0;
	m_uiFormatType = pCreateBy->GetFormatType();
	m_uiMulSample = uiMulSample;
	m_pCreateBy = pCreateBy;
	m_uiLevel = uiLevel;
	m_uiWidth = pCreateBy->GetWidth(m_uiLevel);
	m_uiHeight = pCreateBy->GetHeight(m_uiLevel);
	m_bUsed = false;
	m_bIsStatic = !CPURead;
	m_uiLockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	m_pLockData = NULL;
	m_uiArraySize = 1;
	if (!pCreateBy->SetOutput(this))
	{
		VSMAC_ASSERT(0);
	}
}
VSDepthStencil::VSDepthStencil(VS2DTextureArray * pCreateBy, uint32 uiMulSample, uint32 uiLevel,
	uint32 First, uint32 uiArraySize, bool CPURead)
{
	if (pCreateBy->GetTexType() == VSTexture::TT_CUBE && uiArraySize > VSCubeTexture::F_MAX)
	{
		m_uiArraySize = VSCubeTexture::F_MAX;
	}
	
	m_uiFirst = First;

	m_uiFormatType = pCreateBy->GetFormatType();
	m_uiMulSample = uiMulSample;
	m_pCreateBy = pCreateBy;
	m_uiLevel = uiLevel;
	m_uiWidth = pCreateBy->GetWidth(m_uiLevel);
	m_uiHeight = pCreateBy->GetHeight(m_uiLevel);
	m_bUsed = false;
	m_bIsStatic = !CPURead;
	m_uiLockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	m_pLockData = NULL;
	m_uiArraySize = uiArraySize;
	if (!pCreateBy->SetOutput(this))
	{
		VSMAC_ASSERT(0);
	}
}
VSDepthStencil::VSDepthStencil(VS3DTexture * pCreateBy, uint32 uiMulSample, uint32 uiLevel,
	uint32 First, uint32 uiArraySize, bool CPURead)
{
	m_uiFirst = First;

	m_uiFormatType = pCreateBy->GetFormatType();
	m_uiMulSample = uiMulSample;
	m_pCreateBy = pCreateBy;
	m_uiLevel = uiLevel;
	m_uiWidth = pCreateBy->GetWidth(m_uiLevel);
	m_uiHeight = pCreateBy->GetHeight(m_uiLevel);
	m_bUsed = false;
	m_bIsStatic = !CPURead;
	m_uiLockFlag = CPURead ? VSInheritBind::LF_READONLY : VSInheritBind::LF_NOOVERWRITE;
	m_pLockData = NULL;
	m_uiArraySize = uiArraySize;
	if (!pCreateBy->SetOutput(this))
	{
		VSMAC_ASSERT(0);
	}
}
VSDepthStencil::~VSDepthStencil()
{
	ReleaseResource();
}
bool VSDepthStencil::OnLoadResource(VSResourceIdentifier *&pID)
{

	if(!m_pUser->OnLoadDepthStencil (this,pID))
		return 0;
	return 1;
}
void *VSDepthStencil::Lock()
{
	if (m_pLockData)
	{
		return NULL;
	}

	m_pLockData = m_pUser->Lock(this);


	return m_pLockData;
}
void VSDepthStencil::UnLock()
{
	if (!m_pLockData)
	{
		return;
	}

	m_pUser->UnLock(this);

	m_pLockData = NULL;
}