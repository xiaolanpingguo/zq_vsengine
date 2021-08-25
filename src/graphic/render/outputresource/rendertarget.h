#pragma once


#include "graphic/render/outputresource/outputresource.h"
#include "graphic/render/outputresource/textureoutputinfo.h"
#include "graphic/render/texture/texture.h"


namespace zq{


class GRAPHIC_API VSRenderTarget : public VSOutputResource,public VSTextureOutputInfo
{

	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL
		
public:
	friend class VSResourceManager;	
	virtual ~VSRenderTarget();
	virtual uint32 GetOutputType()const {return OT_RENDER_TARGET;}
	virtual void *Lock();
	virtual void UnLock();
protected:
	VSRenderTarget(uint32 uiWidth, uint32 uiHeight,uint32 uiFormatType,
		uint32 uiMulSample, bool CPURead = false);
	VSRenderTarget(class VS2DTexture * pCreateBy, uint32 uiMulSample = VSRenderer::MS_NONE
		, uint32 uiLevel = 0, bool CPURead = false);
	VSRenderTarget(class VS2DTextureArray * pCreateBy, uint32 uiMulSample = VSRenderer::MS_NONE
		, uint32 uiLevel = 0, uint32 First = 0, uint32 uiArraySize = 1,bool CPURead = false);
	VSRenderTarget(class VS3DTexture * pCreateBy, uint32 uiMulSample = VSRenderer::MS_NONE
		, uint32 uiLevel = 0, uint32 First = 0, uint32 uiArraySize = 1, bool CPURead = false);
	VSRenderTarget();

	virtual bool OnLoadResource(VSResourceIdentifier *&pID);		

};
DECLARE_Ptr(VSRenderTarget);
VSTYPE_MARCO(VSRenderTarget);
}
