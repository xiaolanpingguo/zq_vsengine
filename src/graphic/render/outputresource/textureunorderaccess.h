#pragma once
#include "graphic/render/outputresource/unorderaccess.h"
#include "graphic/render/outputresource/textureoutputinfo.h"
#include "graphic/render/texture/texture.h"
namespace zq
{
	class GRAPHIC_API VSTextureUnorderAccess :public VSUnorderAccess, public VSTextureOutputInfo
	{

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		friend class VSResourceManager;
		virtual ~VSTextureUnorderAccess();
		virtual uint32 GetOutputType()const { return OT_TEXTURE_UNORDER_ACCESS; }
		virtual void *Lock();
		virtual void UnLock();
	protected:
		VSTextureUnorderAccess(uint32 uiWidth, uint32 uiHeight, uint32 uiFormatType,
			bool CPURead = false);
		VSTextureUnorderAccess(class VS2DTexture * pCreateBy,uint32 uiLevel = 0, bool CPURead = false);
		VSTextureUnorderAccess(class VS2DTextureArray * pCreateBy,uint32 uiLevel = 0, 
			uint32 First = 0, uint32 uiArraySize = 1, bool CPURead = false);
		VSTextureUnorderAccess(class VS3DTexture * pCreateBy,uint32 uiLevel = 0, 
			uint32 First = 0, uint32 uiArraySize = 1, bool CPURead = false);
		VSTextureUnorderAccess();

		virtual bool OnLoadResource(VSResourceIdentifier *&pID);
	};
	DECLARE_Ptr(VSTextureUnorderAccess);
	VSTYPE_MARCO(VSTextureUnorderAccess);
}

