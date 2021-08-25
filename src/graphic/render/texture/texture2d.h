#pragma once
#include "graphic/render/texture/texture.h"
namespace zq
{
	class VSRenderTarget;
	class GRAPHIC_API VS2DTexture:public VSTexture
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		VS2DTexture(uint32 uiWidth, uint32 uiHeight,uint32 uiFormatType,
					uint32 uiMipLevel = 0,bool bIsStatic = true, bool bSRGB = false);
		virtual ~VS2DTexture();

		virtual uint32 GetTexType()const{return TT_2D;}
		virtual void SetMipLevel();
		//bool SaveToFile(char * pSaveName);

		DECLARE_INITIAL
	public:
		friend class VSRenderer;
		friend class VSTexAllState;
		friend class VSRenderTarget;
		friend class VSDepthStencil;
		friend class VSTextureUnorderAccess;
		class VSOutputResource *GetOutputResource(uint32 uiMipLevel = 0)const
		{
			return m_pOutputResource[uiMipLevel];
		}
	protected:
		VS2DTexture();
		
	};
	DECLARE_Ptr(VS2DTexture);
	VSTYPE_MARCO(VS2DTexture);
}