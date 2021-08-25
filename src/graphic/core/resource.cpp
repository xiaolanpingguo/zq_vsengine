#include "graphic/core/resource.h"
#include "graphic/core/stream/property.h"
using namespace zq;
VSResourceInterface::VSResourceInterface()
{

}
VSResourceInterface::~VSResourceInterface()
{

}
VSResource::VSResource()
{

}
VSResource::~VSResource()
{

}
IMPLEMENT_RTTI_NoCreateFun(VSCacheResource, VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSCacheResource)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSCacheResource, VSObject)
END_ADD_PROPERTY
VSCacheResource::VSCacheResource()
{

}
VSCacheResource::~VSCacheResource()
{

}
VSResourceProxyBase::VSResourceProxyBase()
{

}
VSResourceProxyBase::~VSResourceProxyBase()
{

}
VSResourceControl::VSResourceControl(uint32 uiGCMaxTimeCount)
{
	m_uiGCMaxTimeCount = uiGCMaxTimeCount;
}
VSResourceControl::~VSResourceControl()
{

}
