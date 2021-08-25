#pragma once
#include "graphic/render/texture/texture.h"
#include "graphic/render/texture/texture2d.h"
namespace zq
{
	class VSRenderTarget;
	class GRAPHIC_API VS2DTextureArray : public VSTexture
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VS2DTextureArray(uint32 uiWidth, uint32 uiHeight, uint32 uiFormatType,
			uint32 uiMipLevel, uint32 uiArraySize,bool bIsStatic, bool bSRGB = false);
		VS2DTextureArray(VSArray<VS2DTexture *>& pTextureArray);
		virtual ~VS2DTextureArray();

		virtual uint32 GetTexType()const { return TT_2DARRAY; }

		virtual void SetMipLevel();
		friend class VSRenderTarget;
		friend class VSDepthStencil;
		friend class VSTextureUnorderAccess;
		class VSOutputResource *GetOutputResource(uint32 uiIndex)const
		{
			return m_pOutputResource[uiIndex];
		}
		virtual uint32 GetArraySize()const
		{
			return m_uiArraySize;
		}
	protected:
		VS2DTextureArray();	
		virtual bool SetOutput(class VSOutputResource *pOutputResource);

	};
	DECLARE_Ptr(VS2DTextureArray);
	VSTYPE_MARCO(VS2DTextureArray);
}
