#pragma once
#include "graphic/render/texture/texture.h"
namespace zq
{
	class GRAPHIC_API VS3DTexture:public VSTexture
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VS3DTexture(uint32 uiWidth, uint32 uiHeight,uint32 uiLength,
			uint32 uiFormatType,uint32 uiMipLevel = 0,bool bIsStatic = true, bool bSRGB = false);
		virtual ~VS3DTexture();

		virtual uint32 GetTexType()const{return TT_3D;}
		virtual void SetMipLevel();
		friend class VSRenderTarget;
		friend class VSDepthStencil;
		friend class VSTextureUnorderAccess;
	protected:
		VS3DTexture();
		virtual bool SetOutput(class VSOutputResource *pOutputResource);
	};
	DECLARE_Ptr(VS3DTexture);
	VSTYPE_MARCO(VS3DTexture);
	
}