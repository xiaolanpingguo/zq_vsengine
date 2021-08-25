#include "graphic/render/inheritbind.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI_NoCreateFun(VSInheritBind, VSBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSInheritBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSInheritBind, VSBind)
REGISTER_PROPERTY(m_bIsStatic, IsStatic, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiLockFlag, LockFlag, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSInheritBind::VSInheritBind()
{
	m_bIsStatic = 1;
	m_uiLockFlag = LF_NOOVERWRITE;
}
VSInheritBind::~VSInheritBind()
{
	
}