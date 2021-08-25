#pragma once
#include "graphic/render/texture/texallstate.h"
#include "graphic/render/scenemanager/viewfamily.h"
#include "graphic/render/texture/cubetexture.h"
namespace zq
{
	class GRAPHIC_API VSCaptureTexAllState : public VSTexAllState
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	public:
		VSCaptureTexAllState();

		virtual ~VSCaptureTexAllState();

		bool SetViewCapture(const VSString & ViewCaptureName);

		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual void ForceUpdate(bool OnlyUpdateOneTime);
		virtual void NotifyEndDraw();
		virtual bool BeforeSave(VSStream* pStream);
		virtual bool PostSave(VSStream* pStream);
		void SetMipLevel(uint32 uiMipLevel);
		VSCaptureViewFamily * GetViewFamily();
	protected:
		VSString  m_ViewCaptureName;
		bool m_bDynamic;
		VSTexturePtr m_pStaticTexture;
		uint32 m_uiMipLevel;
		void CreateStaticTexture();
	};
	DECLARE_Ptr(VSCaptureTexAllState);
	VSTYPE_MARCO(VSCaptureTexAllState);
	DECLARE_Proxy(VSCaptureTexAllState);
}