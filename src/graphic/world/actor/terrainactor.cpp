#include "graphic/world/actor/terrainactor.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/node/terrain/clodterrainnode.h"
#include "graphic/node/terrain/dlodterrainnode.h"
#include "graphic/node/terrain/gpulodterrainnode.h"
using namespace zq;

IMPLEMENT_RTTI_NoCreateFun(VSTerrainActor,VSActor)
BEGIN_ADD_PROPERTY(VSTerrainActor,VSActor)
ADD_PROPERTY_FLAG(pNode,VSProperty::F_NO_USE_GC)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSTerrainActor)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
VSTerrainActor::VSTerrainActor()
{

}
VSTerrainActor::~VSTerrainActor()
{

}
IMPLEMENT_RTTI(VSCLodTerrainActor,VSTerrainActor)
BEGIN_ADD_PROPERTY(VSCLodTerrainActor,VSTerrainActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCLodTerrainActor)
IMPLEMENT_INITIAL_END
VSCLodTerrainActor::VSCLodTerrainActor()
{
	
}
VSCLodTerrainActor::~VSCLodTerrainActor()
{

}
void VSCLodTerrainActor::CreateDefaultComponentNode()
{
	m_pNode = MEM_NEW VSCLodTerrainNode();
	GetTypeNode()->CreateTerrain(10, 10, 7);
}

IMPLEMENT_RTTI(VSDLodTerrainActor,VSTerrainActor)
BEGIN_ADD_PROPERTY(VSDLodTerrainActor,VSTerrainActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDLodTerrainActor)
IMPLEMENT_INITIAL_END
VSDLodTerrainActor::VSDLodTerrainActor()
{
}
VSDLodTerrainActor::~VSDLodTerrainActor()
{

}
void VSDLodTerrainActor::CreateDefaultComponentNode()
{
	m_pNode = MEM_NEW VSDLodTerrainNode();
	GetTypeNode()->CreateTerrain(10, 10, 7);
}

IMPLEMENT_RTTI(VSGPULodTerrainActor, VSTerrainActor)
BEGIN_ADD_PROPERTY(VSGPULodTerrainActor, VSTerrainActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGPULodTerrainActor)
IMPLEMENT_INITIAL_END
VSGPULodTerrainActor::VSGPULodTerrainActor()
{
}
VSGPULodTerrainActor::~VSGPULodTerrainActor()
{

}
void VSGPULodTerrainActor::CreateDefaultComponentNode()
{
	m_pNode = MEM_NEW VSGPULodTerrainNode();
	GetTypeNode()->CreateTerrain(10, 10, 7);
}