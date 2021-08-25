#include "graphic/node/terrain/dlodterrainnode.h"
#include "graphic/node/terrain/dlodterraingeometry.h"
#include "graphic/node/terrain/dlodterrainswitchnode.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSDLodTerrainNode,VSTerrainNode)
BEGIN_ADD_PROPERTY(VSDLodTerrainNode,VSTerrainNode)
REGISTER_PROPERTY(m_uiDLodExtend, DLodExtend, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fDLodScale, DLodScale, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDLodTerrainNode)
IMPLEMENT_INITIAL_END
VSDLodTerrainNode::VSDLodTerrainNode()
{
	m_uiDLodExtend = 500;
	m_fDLodScale = 50000000.0f;
}

VSDLodTerrainNode::~VSDLodTerrainNode()
{
}
bool VSDLodTerrainNode::CreateChild()
{
	m_pChild.Clear();
	uint32 uiChildNumX = GetTileNumX();
	uint32 uiChildNumZ = GetTileNumZ();
	for (uint32 i = 0 ; i < uiChildNumX ; i++)
	{
		for ( uint32 j = 0 ; j < uiChildNumZ ; j++)
		{

			VSDLodTerrainSwitchNode * pDTS = NULL;
			pDTS = MEM_NEW VSDLodTerrainSwitchNode(i,j);
			VSMAC_ASSERT(pDTS);

			AddChild(pDTS);
			for (uint32 k = 0 ; k < m_uiTessellationLevel ; k++)
			{
				VSDLodTerrainGeometry * pChild = NULL;
				pChild = MEM_NEW VSDLodTerrainGeometry();
				VSMAC_ASSERT(pChild);

				pDTS->AddChild(pChild);
				pChild->CreateMesh(i,j,k,m_uiTessellationLevel);

				pChild->AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultOnlyColorResource());
				float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
				pChild->GetMaterialInstance(0)->SetShaderValue(_T("EmissiveColor"), green, 4);
			}

		}
	}

	return 1;
}

void VSDLodTerrainNode::UpdateNodeAll(double dAppTime)
{
	
	VSTerrainNode::UpdateNodeAll(dAppTime);

}