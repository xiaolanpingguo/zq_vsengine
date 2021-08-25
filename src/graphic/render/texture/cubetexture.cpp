#include "graphic/render/texture/cubetexture.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/render/outputresource/rendertarget.h"
using namespace zq;
IMPLEMENT_RTTI(VSCubeTexture,VSTexture)
BEGIN_ADD_PROPERTY(VSCubeTexture,VSTexture)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCubeTexture)
IMPLEMENT_INITIAL_END
VSCubeTexture::VSCubeTexture(uint32 uiWidth,uint32 uiFormatType,
							 uint32 uiMipLevel,bool bIsStatic, bool bSRGB)
						 :VS2DTextureArray(uiWidth, uiWidth,uiFormatType, uiMipLevel, F_MAX,bIsStatic, bSRGB)
{
	
}
VSCubeTexture::~VSCubeTexture()
{

}
VSCubeTexture::VSCubeTexture()
{

}
