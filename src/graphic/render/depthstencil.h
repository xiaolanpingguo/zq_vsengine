#pragma once


#include "graphic/render/outputresource/outputresource.h"
#include "graphic/render/outputresource/textureoutputinfo.h"
#include "graphic/render/texture/texture.h"


namespace zq
{
	class VSStream;
	class GRAPHIC_API VSDepthStencil : public VSOutputResource,public VSTextureOutputInfo
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class VSResourceManager;
		virtual uint32 GetOutputType()const { return OT_DEPTH_STENCIL; }
		virtual ~VSDepthStencil();
		virtual void *Lock();
		virtual void UnLock();
	protected:
		VSDepthStencil();
		VSDepthStencil(uint32 uiWidth,uint32 uiHeight,
			uint32 uiMulSample,uint32 uiFormatType, bool CPURead = false);
		VSDepthStencil(class VS2DTexture * pCreateBy, uint32 uiMulSample = VSRenderer::MS_NONE
			, uint32 uiLevel = 0, bool CPURead = false);
		VSDepthStencil(class VS2DTextureArray * pCreateBy, uint32 uiMulSample = VSRenderer::MS_NONE
			, uint32 uiLevel = 0, uint32 First = 0, uint32 uiArraySize = 1, bool CPURead = false);
		VSDepthStencil(class VS3DTexture * pCreateBy, uint32 uiMulSample = VSRenderer::MS_NONE
			, uint32 uiLevel = 0, uint32 First = 0, uint32 uiArraySize = 1, bool CPURead = false);
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);		

	};
	DECLARE_Ptr(VSDepthStencil);
	VSTYPE_MARCO(VSDepthStencil);
}
