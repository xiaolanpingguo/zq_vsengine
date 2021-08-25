#pragma once
#include "graphic/render/texture/texture.h"
namespace zq
{
	class GRAPHIC_API VS1DTexture:public VSTexture
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VS1DTexture(uint32 uiWidth,uint32 uiFormatType,
					uint32 uiMipLevel = 0,bool bIsStatic = true, bool bSRGB = false);
		virtual ~VS1DTexture();

		virtual uint32 GetTexType()const{return TT_1D;}
	public:
		friend class VSRenderer;
		friend class VSTexAllState;
		virtual void SetMipLevel();
	protected:
		VS1DTexture();

	};
	VSTYPE_MARCO(VS1DTexture);
	DECLARE_Ptr(VS1DTexture);


}