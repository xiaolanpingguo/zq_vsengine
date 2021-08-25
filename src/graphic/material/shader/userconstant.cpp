#include "graphic/material/shader/userconstant.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSUserConstant,VSObject)
BEGIN_ADD_PROPERTY(VSUserConstant,VSObject)
REGISTER_PROPERTY(m_uiValueType,ValueType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterIndex,RegisterIndex,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterNum,RegisterNum,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Name,Name,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_NameInShader, NameInShader, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiSize, Size, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSUserConstant)
IMPLEMENT_INITIAL_END
VSUserConstant::VSUserConstant()
{
	m_pData = NULL;
	m_uiRegisterIndex = 0;
	m_uiValueType = MATH_MAX_UINT32;
	m_uiRegisterNum = 0;
	m_uiSize = 0;
}
VSUserConstant::VSUserConstant(const VSUsedName & Name, const VSUsedName & NameInShader, uint32 uiSize,
	uint32 uiRegisterIndex, uint32 uiRegisterNum, uint32 uiValueType)
{
	VSMAC_ASSERT(uiValueType < VT_MAX && uiSize > 0 && uiRegisterNum > 0);
	m_Name = Name;
	m_uiValueType = uiValueType;
	m_uiSize = uiSize;
	m_pData = new unsigned char[m_uiSize];
	m_uiRegisterIndex = uiRegisterIndex;
	m_uiRegisterNum = uiRegisterNum;
	VSMAC_ASSERT(m_pData);
	memset(m_pData, 0, m_uiSize);
	m_NameInShader = NameInShader;
}
VSUserConstant::VSUserConstant(const VSUsedName & Name,
							   uint32 uiSize,
							  uint32 uiRegisterIndex,uint32 uiRegisterNum,
							   uint32 uiValueType)
{
	VSMAC_ASSERT(uiValueType < VT_MAX && uiSize > 0 && uiRegisterNum > 0);
	m_Name = Name;
	m_uiValueType = uiValueType;
	m_uiSize = uiSize;
	m_pData = new unsigned char[m_uiSize];
	m_uiRegisterIndex = uiRegisterIndex;
	m_uiRegisterNum = uiRegisterNum;
	VSMAC_ASSERT(m_pData);
	memset(m_pData, 0, m_uiSize);
	m_NameInShader = Name;
}
VSUserConstant::~VSUserConstant()
{
	SAFE_DELETEA(m_pData);

}
bool VSUserConstant::PostLoad(VSStream* pStream)
{
	SAFE_DELETEA(m_pData);
	if (m_uiSize)
	{
		m_pData = MEM_NEW unsigned char[m_uiSize];
		memset(m_pData, 0, m_uiSize);
	}
	return true;
}
bool VSUserConstant::PostClone(VSObject * pObjectSrc)
{
	SAFE_DELETEA(m_pData);
	if (m_uiSize)
	{
		m_pData = MEM_NEW unsigned char[m_uiSize];
		memset(m_pData, 0, m_uiSize);
	}
	return true;
}
bool VSUserConstant::SetData(void *pElementData)
{
	VSMAC_ASSERT(pElementData);
	void * pDest =(unsigned char * )m_pData;
	VSMemcpy(pDest,pElementData,m_uiSize);

	return 1;
}
