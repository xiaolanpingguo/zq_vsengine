#include "graphic/render/texture/texture3d.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VS3DTexture,VSTexture)
BEGIN_ADD_PROPERTY(VS3DTexture,VSTexture)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VS3DTexture)
IMPLEMENT_INITIAL_END
VS3DTexture::VS3DTexture(uint32 uiWidth, uint32 uiHeight,uint32 uiLength,
						 uint32 uiFormatType,uint32 uiMipLevel,bool bIsStatic, bool bSRGB)
						 :VSTexture(uiFormatType,uiWidth,uiHeight,uiLength,1,uiMipLevel,bIsStatic, bSRGB)
{

	SetMipLevel();

}
VS3DTexture::~VS3DTexture()
{

}
VS3DTexture::VS3DTexture()
{

}
void VS3DTexture::SetMipLevel()
{
	VSMAC_ASSERT(m_uiWidth && m_uiHeight && m_uiLength);
	VSMAC_ASSERT(IsTwoPower(m_uiWidth) && IsTwoPower(m_uiHeight) && IsTwoPower(m_uiLength));
	uint32 uiWidthLevel = FastLog2(m_uiWidth);
	uint32 uHeightLevel = FastLog2(m_uiHeight);
	uint32 uiLengthLevel = FastLog2(m_uiLength);;
	uint32 uiCurMipLevel = Max(Max(uiWidthLevel,uHeightLevel),uiLengthLevel) + 1;
	if (!m_uiMipLevel || uiCurMipLevel < m_uiMipLevel)
	{
		m_uiMipLevel = uiCurMipLevel;
	}

}
bool VS3DTexture::SetOutput(class VSOutputResource *pOutputResource)
{
	VSMAC_ASSERT(pOutputResource&& pOutputResource->GetOutputType() != VSOutputResource::OT_BUFFER_UNORDER_ACCESS);
	if (IsBindResource())
	{
		return false;
	}
	if (m_uiOutputType == VSOutputResource::OT_NONE)
	{
		m_uiOutputType = ((VSOutputResource *)pOutputResource)->GetOutputType();
		m_pOutputResource.AddElement(pOutputResource);
	}
	else if (m_uiOutputType != ((VSOutputResource *)pOutputResource)->GetOutputType())
	{
		return false;
	}
	else
	{
		bool IsOk = true;
		
		for (uint32 i = 0; i < m_pOutputResource.GetNum(); i++)
		{
			VSTextureOutputInfo * pTextureOutputInfo_i = GetTextureOutputInfo(m_pOutputResource[i]);
			VSTextureOutputInfo * pTextureOutputInfo = GetTextureOutputInfo(pOutputResource);
			if (pTextureOutputInfo_i->GetLevel() == pTextureOutputInfo->GetLevel())
			{
				uint32 uiMin1 = pTextureOutputInfo_i->GetFirst();
				uint32 uiMax1 = pTextureOutputInfo_i->GetFirst() + pTextureOutputInfo_i->GetArraySize() - 1;

				uint32 uiMin2 = pTextureOutputInfo->GetFirst();
				uint32 uiMax2 = pTextureOutputInfo->GetFirst() + pTextureOutputInfo->GetArraySize() - 1;

				if (uiMin1 <= uiMax2 && uiMin2 <= uiMax1)
				{
					IsOk = false;
					break;
				}
			}
		}

		if (IsOk)
		{
			m_pOutputResource.AddElement(pOutputResource);
		}
		else
		{
			VSMAC_ASSERT(0);
			return false;
		}
	}
	return true;
}
