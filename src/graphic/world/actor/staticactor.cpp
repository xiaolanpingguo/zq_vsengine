#include "graphic/world/actor/staticactor.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSStaticActor,VSActor)
BEGIN_ADD_PROPERTY(VSStaticActor,VSActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSStaticActor)
IMPLEMENT_INITIAL_END
VSStaticActor::VSStaticActor()
{
	
}
VSStaticActor::~VSStaticActor()
{

}
void VSStaticActor::CreateDefaultComponentNode()
{
	m_pNode = VSNodeComponent::CreateComponent<VSStaticMeshComponent>();
}
