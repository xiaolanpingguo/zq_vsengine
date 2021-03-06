#include "graphic/node/model/staticmeshnode.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/node/model/modelswitchnode.h"
using namespace zq;
IMPLEMENT_RTTI(VSStaticMeshNode,VSModelMeshNode)
BEGIN_ADD_PROPERTY(VSStaticMeshNode,VSModelMeshNode)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSStaticMeshNode)
ADD_PRIORITY(VSGeometry)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
IMPLEMENT_RESOURCE(VSStaticMeshNode)
VSStaticMeshNode::VSStaticMeshNode()
{
}
VSStaticMeshNode::~VSStaticMeshNode()
{

}
bool VSStaticMeshNode::InitialDefaultState()
{
	ms_Default = MEM_NEW VSStaticMeshNode();
	VSGeometryNodePtr GeometryNode = MEM_NEW VSGeometryNode();
	ms_Default->AddChild(GeometryNode);
	VSGeometryPtr Geometry = (VSGeometry *)VSObject::CloneCreateObject(VSGeometry::GetDefaultRenderCube());
	GeometryNode->AddChild(Geometry);
	ms_Default->CreateLocalAABB();
	GeometryNode->SetLocalScale(Vector3(100.0f,100.0f,100.0f));
	ms_DefaultResource = VSStaticMeshNodeR::Create(ms_Default);
	return true;
}
bool VSStaticMeshNode::TerminalDefaultState()
{
	ms_DefaultResource = NULL;
	ms_Default = NULL;
	return true;
}
void VSStaticMeshNode::AddLodMesh(VSStaticMeshNodeR * pStaticMeshResource)
{
	if (pStaticMeshResource)
	{
		while (!pStaticMeshResource->IsLoaded())
		{
		}
		VSGeometryNode* pGeoNode = pStaticMeshResource->GetResource()->GetGeometryNode(0);
		AddLODGeometryNode(pGeoNode);
	}
}
