#include "graphic/node/terrain/gpulodterrainnode.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/node/terrain/gpulodterraingeometry.h"
using namespace zq;
IMPLEMENT_RTTI(VSGPULodTerrainNode, VSTerrainNode)
BEGIN_ADD_PROPERTY(VSGPULodTerrainNode, VSTerrainNode)
// REGISTER_PROPERTY(m_uiDLodExtend, DLodExtend, VSProperty::F_SAVE_LOAD_CLONE);
// REGISTER_PROPERTY(m_fDLodScale, DLodScale, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGPULodTerrainNode)
IMPLEMENT_INITIAL_END
VSGPULodTerrainNode::VSGPULodTerrainNode()
{
// 	m_uiDLodExtend = 500;
// 	m_fDLodScale = 50000000.0f;
}

VSGPULodTerrainNode::~VSGPULodTerrainNode()
{
}
bool VSGPULodTerrainNode::CreateChild()
{
	m_pChild.Clear();

	VSGPULodTerrainGeometry * pGLG = NULL;
	pGLG = MEM_NEW VSGPULodTerrainGeometry();
	if (!pGLG->CreateMesh(GetTileNumX(), GetTileNumZ()))
	{
		return false;
	}
	AddChild(pGLG);
	pGLG->AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultOnlyColorResource());
	float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	pGLG->GetMaterialInstance(0)->SetShaderValue(_T("EmissiveColor"), green, 4);
	
	return 1;
}

void VSGPULodTerrainNode::UpdateNodeAll(double dAppTime)
{

	VSTerrainNode::UpdateNodeAll(dAppTime);

}
bool VSGPULodTerrainNode::CreateTerrain(uint32 uiNumX, uint32 uiNumZ, uint32 uiTessellationLevel)
{
	SetNum(uiNumX, uiNumZ);
	SetTessellationLevel(uiTessellationLevel,MAX_GPU_TESSELLATION_LEVEL);
	SAFE_DELETEA(m_pHeight);
	m_pHeight = MEM_NEW unsigned char[m_uiTotalNum];
	VSMAC_ASSERT(m_pHeight);

	memset(m_pHeight, 0, m_uiTotalNum * sizeof(unsigned char));
	m_LodTexture = VSResourceManager::Create2DTexture(GetSizeX(), GetSizeZ(), VSRenderer::SFT_R16F, 1, m_pHeight);
	if (!CreateChild())
	{
		return 0;
	}
	m_bIsChanged = true;
	CreateLocalAABB();
	UpdateAll(0.0f);
	return true;
}