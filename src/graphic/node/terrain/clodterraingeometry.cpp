#include "graphic/node/terrain/clodterraingeometry.h"
#include "graphic/node/terrain/terrainnode.h"
#include "graphic/node/mesh/triangleset.h"
#include "graphic/render/buffer/vertexbuffer.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI_NoCreateFun(VSCLodTerrainGeometry,VSGeometry)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSCLodTerrainGeometry)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSCLodTerrainGeometry,VSGeometry)
REGISTER_PROPERTY(m_pNeighbor[NT_LEFT],NeighborLeft,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pNeighbor[NT_RIGHT],NeighborRight,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pNeighbor[NT_TOP],NeighborTFop,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pNeighbor[NT_BOTTOM],NeighborBottom,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiIndexXInTerrain,IndexXInTerrain,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiIndexZInTerrain,IndexZInTerrain,VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSCLodTerrainGeometry::VSCLodTerrainGeometry()
{
	for (uint32 i = 0 ; i < NT_MAX ; i++)
	{
		m_pNeighbor[i] = NULL;
	}
	m_uiIndexXInTerrain = 0;
	m_uiIndexZInTerrain = 0;
	m_bIsStatic = false;
	
}
VSCLodTerrainGeometry::~VSCLodTerrainGeometry()
{

}
bool VSCLodTerrainGeometry::CreateMeshData(uint32 uiIndexXInTerrain, uint32 uiIndexZInTerrain,
									   uint32 uiTessellationLevel)
{

	VSTerrainNode * pTerrainNode = DynamicCast<VSTerrainNode>(m_pParent);
	VSMAC_ASSERT(pTerrainNode);


	VSDataBuffer * pVertexData = NULL ;
	pVertexData = MEM_NEW VSDataBuffer();
	uint32 uiVertexLenght= (1 << uiTessellationLevel) + 1;
	if (!pVertexData->CreateEmptyBuffer(uiVertexLenght * uiVertexLenght,VSDataBuffer::DT_FLOAT32_3))
	{
		return 0;
	}
	

	Vector3 * pVer = (Vector3 *)pVertexData->GetData();
	uint32 uiBiasX = uiIndexXInTerrain * (1 << uiTessellationLevel);
	uint32 uiBiasZ = uiIndexZInTerrain * (1 << uiTessellationLevel);
	for (uint32 i = 0 ; i < uiVertexLenght ; i++)
	{
		for(uint32 j = 0 ; j < uiVertexLenght ; j++)
		{
			uint32 uiIndex = i * uiVertexLenght + j;
			pVer[uiIndex].x = 1.0f * (uiBiasX + i) * VSTerrainNode::WIDTH_SCALE;
			pVer[uiIndex].y = pTerrainNode->GetHeight(uiBiasX + i,uiBiasZ + j);
			pVer[uiIndex].z = 1.0f * (uiBiasZ + j) * VSTerrainNode::WIDTH_SCALE;
		}
	}
	
	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	pVertexBuffer->SetMemType(VSMemBind::MT_BOTH);
	VSMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetData(pVertexData,VSVertexFormat::VF_POSITION);


 	uint32 uiCurRenderTriNum = (uiVertexLenght - 1) * (uiVertexLenght - 1) * 2 * 3;

	VSIndexBuffer * pIndexBuffer = NULL;
	pIndexBuffer = MEM_NEW VSIndexBuffer(uiCurRenderTriNum);
	pIndexBuffer->SetMemType(VSMemBind::MT_BOTH);
	VSMAC_ASSERT(pIndexBuffer);

	//test mul thread render
	if (GetTerrainGeometryType() == TGT_ROAM)
	{
		pIndexBuffer->SetStatic(false,true);
	}
	else
	{
		pIndexBuffer->SetStatic(false);
	}
	
	pIndexBuffer->SetLockFlag(VSInheritBind::LF_DISCARD);

	VSTriangleSet * pMeshData = NULL;
	pMeshData = MEM_NEW VSTriangleSet();

	if (!pMeshData)
	{
		return 0;
	}

	pMeshData->SetVertexBuffer(pVertexBuffer);
	pMeshData->SetIndexBuffer(pIndexBuffer);
	SetMeshData(pMeshData);
	m_uiIndexXInTerrain = uiIndexXInTerrain;
	m_uiIndexZInTerrain = uiIndexZInTerrain;
	return 1;
}
bool VSCLodTerrainGeometry::AddNeighbor(VSCLodTerrainGeometry * pTerrainGemetry,uint32 uiNeighbor)
{
	VSMAC_ASSERT(pTerrainGemetry && uiNeighbor < NT_MAX);

	VSTerrainNode * pTerrainNode = DynamicCast<VSTerrainNode>(m_pParent);
	VSMAC_ASSERT(pTerrainNode);

	VSMAC_ASSERT(m_pNeighbor[uiNeighbor] == NULL);
	
	m_pNeighbor[uiNeighbor] = pTerrainGemetry;

	return 1;
}
void VSCLodTerrainGeometry::UpdateView(VSCuller & Culler,double dAppTime)
{
	VSGeometry::UpdateView(Culler,dAppTime);
	VSCamera * pCamera = Culler.GetCamera();
	VSMAC_ASSERT(pCamera);

	if (Culler.GetCullerType() == VSCuller::CUT_MAIN)
	{
		VSTerrainNode * pTerrainNode = DynamicCast<VSTerrainNode>(m_pParent);
		if (!pTerrainNode)
		{
			return ;
		}
		VSTransform Tran = pTerrainNode->GetWorldTransform();
		Vector3 Loc = pCamera->GetWorldTranslate() * Tran.GetCombineInverse();


		Tessellate(Loc);
	}
}

