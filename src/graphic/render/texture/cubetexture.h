#pragma once
#include "graphic/render/texture/texture.h"
#include "graphic/render/texture/texturearray2d.h"
namespace zq
{
	class VSRenderTarget;
	class GRAPHIC_API VSCubeTexture : public VS2DTextureArray
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum	//FACE
		{
			F_RIGHT,
			F_LEFT,	
			F_TOP,
			F_BOTTOM,
			F_FRONT,
			F_BACK,				
			F_MAX
		};
		VSCubeTexture(uint32 uiWidth,uint32 uiFormatType,
					uint32 uiMipLevel = 0,bool bIsStatic = true, bool bSRGB = false);
		virtual ~VSCubeTexture();
		virtual uint32 GetTexType()const { return TT_CUBE; }
	protected:
		VSCubeTexture();

	};
	DECLARE_Ptr(VSCubeTexture);
	VSTYPE_MARCO(VSCubeTexture);
}