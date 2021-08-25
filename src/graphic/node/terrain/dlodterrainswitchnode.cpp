#include "graphic/node/terrain/dlodterrainswitchnode.h"
#include "graphic/node/terrain/dlodterrainnode.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSDLodTerrainSwitchNode,VSSwitchNode)
BEGIN_ADD_PROPERTY(VSDLodTerrainSwitchNode,VSSwitchNode)
REGISTER_PROPERTY(m_uiIndexXInTerrain,IndexXInTerrain,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiIndexZInTerrain,IndexZInTerrain,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDLodTerrainSwitchNode)
IMPLEMENT_INITIAL_END
VSDLodTerrainSwitchNode::VSDLodTerrainSwitchNode(uint32 uiIndexXInTerrain,uint32 uiIndexZInTerrain)
{
	m_uiIndexXInTerrain = uiIndexXInTerrain;
	m_uiIndexZInTerrain = uiIndexZInTerrain;
}
VSDLodTerrainSwitchNode::VSDLodTerrainSwitchNode()
{
	
}
VSDLodTerrainSwitchNode::~VSDLodTerrainSwitchNode()
{

}
void VSDLodTerrainSwitchNode::UpdateView(VSCuller & Culler,double dAppTime)
{
	VSSwitchNode::UpdateView(Culler,dAppTime);
	VSCamera * pCamera = Culler.GetCamera();
	VSMAC_ASSERT(pCamera);

	if (Culler.GetCullerType() == VSCuller::CUT_MAIN)
	{
		VSDLodTerrainNode * pTerrainNode = DynamicCast<VSDLodTerrainNode>(m_pParent);
		if (!pTerrainNode)
		{
			return ;
		}
		VSTransform Tran = pTerrainNode->GetWorldTransform();
		Vector3 Loc = pCamera->GetWorldTranslate() * Tran.GetCombineInverse();
		uint32 uiLength = 1 << pTerrainNode->GetTessellationLevel();
		Vector3 Pos;
		uint32 uiIndenX = uiLength * m_uiIndexXInTerrain;
		uint32 uiIndenZ = uiLength * m_uiIndexZInTerrain;
		Pos.x = VSTerrainNode::WIDTH_SCALE * (uiIndenX + (uiLength >> 1)) * 1.0f;
		Pos.z = VSTerrainNode::WIDTH_SCALE * (uiIndenZ + (uiLength >> 1)) * 1.0f;
		Pos.y = pTerrainNode->GetHeight(uiIndenX,uiIndenZ);

		Vector3 Length = Loc - Pos;
		float fSqrLen = Length.GetSqrLength();
		float fDLodScale = pTerrainNode->GetDLodScale();
		for (uint32 i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			m_uiActiveNode = i;
			if (fSqrLen < (1 << i) * fDLodScale)
			{		
				break;
			}
		}
	}


}
