#include "graphic/material/shader/usersampler.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSUserSampler,VSObject)
BEGIN_ADD_PROPERTY(VSUserSampler,VSObject)
REGISTER_PROPERTY(m_uiTexType,TexType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Name,Name,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterIndex,RegisterIndex,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiRegisterNum,RegisterNum,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSUserSampler)
IMPLEMENT_INITIAL_END
VSUserSampler::VSUserSampler()
{
	m_uiTexType = VSTexture::TT_2D;
	m_pTextureArray.Clear();
	m_uiRegisterNum = 1;
}
VSUserSampler::~VSUserSampler()
{
	m_pTextureArray.Clear();

}
VSUserSampler::VSUserSampler(const VSUsedName & Name,uint32 uiTexType,uint32 uiRegisterIndex,uint32 uiRegisterNum)
{
	m_Name = Name;
	m_uiTexType = uiTexType;
	m_uiRegisterIndex = uiRegisterIndex;
	m_uiRegisterNum = uiRegisterNum;
	m_pTextureArray.SetBufferNum(uiRegisterNum);
}
bool VSUserSampler::PostLoad(VSStream* pStream)
{
	m_pTextureArray.SetBufferNum(m_uiRegisterNum);
	return true;
}
bool VSUserSampler::PostClone(VSObject * pObjectSrc)
{
	m_pTextureArray.SetBufferNum(m_uiRegisterNum);
	return true;
}
