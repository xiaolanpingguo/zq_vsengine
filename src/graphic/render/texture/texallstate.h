#pragma once
#include "graphic/render/texture/texture.h"
#include "graphic/core/resource.h"
#include "graphic/render/renderstate/samplerstate.h"
//#include "VSString.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSTexAllState : public VSObject , public VSResource
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;

	public:
		virtual ~VSTexAllState();
		VSTexAllState();
		DECLARE_INITIAL

		static bool InitialDefaultState();
		static bool TerminalDefaultState();

		void SetTexture(VSTexture * pTexture);
		inline VSTexture * GetTexture()const
		{
			return m_pTex;
		}
		virtual uint32 GetCurStreamLevel()const;
	protected:
		VSTexturePtr		m_pTex;
		VSSamplerDesc		m_SamplerDesc;
		VSSamplerStatePtr	m_pSamplerState;
		bool				m_bSRGB;
		bool				m_bNormal;
		bool				m_bMip;
		uint32 m_uiArraySize;
		uint32 m_uiFormatType;
		uint32 m_uiWidth;
		uint32 m_uiHeight;
		uint32 m_uiLength;
		VSArray<unsigned char> m_SourceData;
	protected:
		static VSPointer<VSTexAllState> ms_pOrenNayarLookUpTable;
	public:
		friend class VSResourceManager;
		friend class VSRenderer;
		inline bool GetSRGB()const
		{
			return m_bSRGB;
		}
		inline uint32 GetWidth()const
		{
			return m_uiWidth;
		}
		inline uint32 GetHeight()const
		{
			return m_uiHeight;
		}
		inline uint32 GetLength()const
		{
			return m_uiLength;
		}
		inline uint32 GetArraySize()const
		{
			return m_uiArraySize;
		}
		inline uint32 GetFormatType()const
		{
			return m_uiFormatType;
		}
		void SetSRGBEable(bool bEnable);
	
		static const VSTexAllState * GetOrenNayarLookUpTable()
		{
			return ms_pOrenNayarLookUpTable;
		}
		void SetSamplerState(VSSamplerState * pSamplerState)
		{
			if (!pSamplerState)
			{
				pSamplerState = (VSSamplerState *)VSSamplerState::GetDefault();
			}
			if (m_pSamplerState != pSamplerState)
			{
				m_pSamplerState = pSamplerState;
				m_SamplerDesc = pSamplerState->GetSamplerDesc();
			}
		}
		DECLARE_RESOURCE(VSTexAllState, RA_ASYN_LOAD | RA_ASYN_POSTLOAD | RA_ENABLE_GC | RA_NEED_CACHE, RT_TEXTURE,TEXTURE,Resource/Texture, VSTextureCache)
		const VSSamplerState * GetSamplerState()const
		{
			return m_pSamplerState;
		}
		virtual bool BeforeSave(VSStream* pStream);
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual bool IsCanSave()
		{
			if (!m_SourceData.GetNum())
			{
				return false;
			}
			return true;
		}
		virtual VSObject* CreateToStreamObject(uint32 uiWantSteamLevel, const VSCacheResource* pCacheResouce)const;
		virtual void StreamEnd(VSObject* pStreamResource);
	};
	DECLARE_Ptr(VSTexAllState);
	DECLARE_Proxy(VSTexAllState);
	VSTYPE_MARCO(VSTexAllState);
}
