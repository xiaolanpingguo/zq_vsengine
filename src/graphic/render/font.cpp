#include "graphic/render/font.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/core/config.h"
using namespace zq;
IMPLEMENT_RTTI(VSFont,VSBind)
BEGIN_ADD_PROPERTY(VSFont,VSBind)
REGISTER_PROPERTY(m_Face,Face,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiSize,Size,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bBold,Bold,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bItalic,Italic,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pFontTex, FontTex, VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY_FIXED_DATA(m_pCharacterData, ASCII_NUM + 1, CharacterData, true)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSFont)
ADD_PRIORITY(VSResourceManager)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
IMPLEMENT_RESOURCE(VSFont)
bool VSFont::InitialDefaultState()
{
	ms_DefaultResource = VSResourceManager::LoadASYNResource<VSFont>(_T("Arial"), false);
	ms_Default = ms_DefaultResource->GetResource();
	if (!ms_Default)
	{
		return false;
	}
	return 1;
}
bool VSFont::TerminalDefaultState()
{
	ms_Default = NULL;
	ms_DefaultResource = NULL;
	return 1;
}
VSFont::VSFont(const char* acFace, uint32 uiSize, bool bBold,
				bool bItalic)
{
	m_bItalic = bItalic;
	if(acFace)
	{
		m_Face = acFace;
	}
	else
	{
		m_Face = _T("Arial");
	}
	m_uiSize = uiSize;
	m_bBold = bBold;
	m_pFontTex = NULL;
	m_pCharacterData = NULL;
	m_pFontTexAllState = NULL;
}
VSFont::~VSFont()
{
	m_pFontTex = NULL;
	SAFE_DELETEA(m_pCharacterData);
	m_pFontTexAllState = NULL;
}
VSFont::VSFont()
{
	m_bItalic = 1;
	m_Face = _T("Arial");
	m_uiSize = 40;
	m_bBold = 0;
	m_pCharacterData = NULL;
	m_pFontTexAllState = NULL;
	m_pFontTex = NULL;
}
void VSFont::SetFontData(VS2DTexture * pTexture, float * pCharacterData)
{
	if (!pTexture || !pCharacterData)
	{
		return;
	}
	m_pFontTex = pTexture;
	SAFE_DELETEA(m_pCharacterData);
	m_pCharacterData = MEM_NEW float[ASCII_NUM + 1];
	VSMemcpy(m_pCharacterData, pCharacterData, sizeof(float)* (ASCII_NUM + 1));
	return;
}
bool VSFont::PostLoad(VSStream* pStream)
{
	if (m_pFontTex)
	{
		m_pFontTexAllState = MEM_NEW VSTexAllState();
		m_pFontTexAllState->SetTexture(m_pFontTex);
		m_pFontTexAllState->SetSamplerState((VSSamplerState *)VSSamplerState::GetTwoLine());
	}
	return true;
}
bool VSFont::PostClone(VSObject * pObjectSrc)
{
	if (m_pFontTex)
	{
		m_pFontTexAllState = MEM_NEW VSTexAllState();
		m_pFontTexAllState->SetTexture(m_pFontTex);
		m_pFontTexAllState->SetSamplerState((VSSamplerState *)VSSamplerState::GetTwoLine());
	}
	return true;
}