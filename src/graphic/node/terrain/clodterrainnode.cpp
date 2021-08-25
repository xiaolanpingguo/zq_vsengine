#include "graphic/node/terrain/clodterrainnode.h"
#include "graphic/node/terrain/clodterraingeometry.h"
#include "graphic/node/terrain/roamterraingemotry.h"
#include "graphic/node/terrain/quadterraingeometry.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSCLodTerrainNode,VSTerrainNode)
BEGIN_ADD_PROPERTY(VSCLodTerrainNode,VSTerrainNode)
REGISTER_PROPERTY(m_uiTerrainNodeType,TerrainNodeType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_fCLODScale, CLODScale, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCLodTerrainNode)
IMPLEMENT_INITIAL_END
VSCLodTerrainNode::VSCLodTerrainNode()
{
	
	m_fCLODScale = 50000000.0f;

	m_uiTerrainNodeType = TNT_ROAM;
}
void VSCLodTerrainNode::SetTerrainNodeType(uint32 uiTerrainNodeType)
{
	VSMAC_ASSERT(uiTerrainNodeType < TNT_MAX);
	m_uiTerrainNodeType = uiTerrainNodeType;

}
VSCLodTerrainNode::~VSCLodTerrainNode()
{

}
uint32 VSCLodTerrainNode::AddChild(VSSpatial * pChild)
{
	VSMAC_ASSERT(DynamicCast<VSCLodTerrainGeometry>(pChild));

	return VSTerrainNode::AddChild(pChild);
}

bool VSCLodTerrainNode::CreateChild()
{
	m_pChild.Clear();
	uint32 uiChildNumX = GetTileNumX();
	uint32 uiChildNumZ = GetTileNumZ();
	for (uint32 i = 0 ; i < uiChildNumX ; i++)
	{
		for ( uint32 j = 0 ; j < uiChildNumZ ; j++)
		{
			VSCLodTerrainGeometry * pTerrainGeo = NULL;
			if (m_uiTerrainNodeType == TNT_ROAM)
			{
				pTerrainGeo = MEM_NEW VSRoamTerrainGeometry();
			}
			else if (m_uiTerrainNodeType == TNT_QUAD)
			{
				pTerrainGeo = MEM_NEW VSQuadTerrainGeometry();
			}

			if (!pTerrainGeo)
			{
				return 0;
			}
			AddChild(pTerrainGeo);
			if (!pTerrainGeo->CreateMeshData(i,j,m_uiTessellationLevel))
			{
				return 0;
			}
			pTerrainGeo->AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultOnlyColorResource());
			float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			pTerrainGeo->GetMaterialInstance(0)->SetShaderValue(_T("EmissiveColor"), green, 4);
			

		}
	}

	LinkNeighbor();
	ComputeVariance();
	return 1;
}


void VSCLodTerrainNode::ComputeVariance()
{
	VSCLodTerrainGeometry * pTerrainGeo = NULL;
	uint32 uiCurIndex;
	uint32 uiChildNumX = GetTileNumX();
	uint32 uiChildNumZ = GetTileNumZ();
	if (m_uiTerrainNodeType == TNT_ROAM)
	{
		for (uint32 i = 0 ; i < uiChildNumX ; i++)
		{
			for ( uint32 j = 0 ; j < uiChildNumZ ; j++)
			{

				uiCurIndex = i * uiChildNumZ + j;
				pTerrainGeo = (VSCLodTerrainGeometry *)(GetChild(uiCurIndex));		
				pTerrainGeo->ComputeVariance();
			}
		}
	}
	else if(m_uiTerrainNodeType == TNT_QUAD)
	{
		for (int32 uiLevel = m_uiTessellationLevel - 1 ; uiLevel >= 0 ; uiLevel--)
		{
			for (uint32 i = 0 ; i < uiChildNumX ; i++)
			{
				for ( uint32 j = 0 ; j < uiChildNumZ ; j++)
				{

					uiCurIndex = i * uiChildNumZ + j;
					pTerrainGeo = (VSCLodTerrainGeometry *)(GetChild(uiCurIndex));		
					pTerrainGeo->ComputeVariance(uiLevel);
				}
			}
		}
		
		
	}
	
}
void VSCLodTerrainNode::LinkNeighbor()
{
	uint32 uiLeft;
	uint32 uiRight;
	uint32 uiTop;
	uint32 uiBottom;
	uint32 uiChildNumX = GetTileNumX();
	uint32 uiChildNumZ = GetTileNumZ();
	uint32 uiCurIndex;
	VSCLodTerrainGeometry * pTerrainGeo = NULL;
	for (uint32 i = 0 ; i < uiChildNumX ; i++)
	{
		for ( uint32 j = 0 ; j < uiChildNumZ ; j++)
		{
			
			uiCurIndex = i * uiChildNumZ + j;
			if (i == 0)
			{
				uiLeft = MATH_MAX_UINT32;

			}
			else
			{
				uiLeft = (i - 1) * uiChildNumZ + j;

			}

			if (i == uiChildNumX - 1)
			{
				uiRight = MATH_MAX_UINT32;

			}
			else
			{
				uiRight = (i + 1) * uiChildNumZ + j;

			}

			if (j == 0)
			{
				uiBottom = MATH_MAX_UINT32;

			}
			else
			{
				uiBottom = i * uiChildNumZ + j - 1;

			}

			if (j == uiChildNumZ - 1)
			{
				uiTop = MATH_MAX_UINT32;
			}
			else
			{
				uiTop = i * uiChildNumZ + j + 1;
			}

			 
			pTerrainGeo = (VSCLodTerrainGeometry *)(GetChild(uiCurIndex));
			if (uiTop != MATH_MAX_UINT32)
			{
				pTerrainGeo->AddNeighbor((VSCLodTerrainGeometry *)GetChild(uiTop),VSCLodTerrainGeometry::NT_TOP);
			}
			
			if (uiBottom != MATH_MAX_UINT32)
			{
				pTerrainGeo->AddNeighbor((VSCLodTerrainGeometry *)GetChild(uiBottom),VSCLodTerrainGeometry::NT_BOTTOM);
			}

			if (uiLeft != MATH_MAX_UINT32)
			{
				pTerrainGeo->AddNeighbor((VSCLodTerrainGeometry *)GetChild(uiLeft),VSCLodTerrainGeometry::NT_LEFT);
			}

			if (uiRight != MATH_MAX_UINT32)
			{
				pTerrainGeo->AddNeighbor((VSCLodTerrainGeometry *)GetChild(uiRight),VSCLodTerrainGeometry::NT_RIGHT);
			}
			pTerrainGeo->LinkNeighbor();
		}
	}

}
void VSCLodTerrainNode::SetCLODScale(float fCLODScale)
{
	if (fCLODScale <= 0.0f)
	{
		return ;
	}
	m_fCLODScale = fCLODScale;
}


void VSCLodTerrainNode::UpdateNodeAll(double dAppTime)
{
	VSTerrainNode::UpdateNodeAll(dAppTime);
	if (dAppTime > 0.0f)
	{
		for (uint32 i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			VSSpatial * pChild = m_pChild[i];
			((VSCLodTerrainGeometry *)pChild)->ClearInfo();
		}
	}
}

