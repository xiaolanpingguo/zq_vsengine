#pragma once
#include "graphic/core/object.h"
#include "graphic/render/texture/texallstate.h"
#include "graphic/render/texture/texture2d.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSFont : public VSObject, public VSResource
	{
		//PRIORITY
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum
		{
			ASCII_START = 32,
			ASCII_END = 126,
			ASCII_NUM = ASCII_END - ASCII_START + 1
		};
		VSFont(const char* acFace, uint32 uiSize, bool bBold,
				bool bItalic);
		virtual ~VSFont();
	protected:
		VSFont();
	public:


		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	public:
		friend class VSRenderer;

		inline const VSString & GetFace()const {return m_Face;}
		inline uint32 GetSize()const {return m_uiSize;}
		inline bool GetBold()const {return m_bBold;}
		inline bool GetItalic()const {return m_bItalic;}
		inline bool IsValid()const
		{
			return m_pFontTex != NULL && !m_pCharacterData && m_pFontTexAllState != NULL;
		}
		void SetFontData(VS2DTexture * pTexture,float * pCharacterData);
		inline float GetFontData(unsigned char Index)const
		{
			return m_pCharacterData[Index];
		}
		VSTexAllState * GetTexAllState()const
		{
			return m_pFontTexAllState;
		}
		DECLARE_RESOURCE(VSFont, RA_ASYN_LOAD | RA_ASYN_POSTLOAD | RA_ENABLE_GC, RT_FONTTYPE,FONT, Resource/Font, VSCacheResource)
		virtual bool PostLoad(VSStream* pStream);
		virtual bool PostClone(VSObject * pObjectSrc);
	protected:
		VSString m_Face;
		uint32 m_uiSize;
		bool m_bBold;
		bool m_bItalic;
		VS2DTexturePtr m_pFontTex;
		float *m_pCharacterData;
		VSTexAllStatePtr m_pFontTexAllState;
	};
	DECLARE_Ptr(VSFont);
	DECLARE_Proxy(VSFont);
	VSTYPE_MARCO(VSFont);


}
