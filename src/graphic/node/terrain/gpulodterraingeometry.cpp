#include "graphic/node/terrain/gpulodterraingeometry.h"
#include "graphic/node/terrain/gpulodterrainnode.h"
#include "graphic/node/mesh/triangleset.h"
#include "graphic/render/buffer/vertexbuffer.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/node/mesh/controlpointpatch.h"
using namespace zq;
IMPLEMENT_RTTI(VSGPULodTerrainGeometry, VSGeometry)
BEGIN_ADD_PROPERTY(VSGPULodTerrainGeometry, VSGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGPULodTerrainGeometry)
IMPLEMENT_INITIAL_END
VSGPULodTerrainGeometry::VSGPULodTerrainGeometry()
{

}
VSGPULodTerrainGeometry::~VSGPULodTerrainGeometry()
{

}
VSGPULodTerrainNode * VSGPULodTerrainGeometry::GetParentTerrainNode()const
{
	if (m_pParent)
	{
		return (VSGPULodTerrainNode *)m_pParent->GetParent();
	}
	return NULL;
}
bool VSGPULodTerrainGeometry::CreateMesh(uint32 uiTileNumX,uint32 uiTileNumZ)
{
	VSGPULodTerrainNode * pTerrainNode = GetParentTerrainNode();


	VSDataBuffer * pVertexData = NULL;
	pVertexData = MEM_NEW VSDataBuffer();
	if (!pVertexData->CreateEmptyBuffer((uiTileNumX + 1) * (uiTileNumZ + 1), VSDataBuffer::DT_FLOAT32_3))
	{
		return 0;
	}


	Vector3 * pVer = (Vector3 *)pVertexData->GetData();
	Vector3 * pVerTemp = pVer;

	for (uint32 i = 0; i < (uiTileNumX + 1); i++)
	{
		for (uint32 j = 0; j < (uiTileNumZ + 1); j++)
		{
			uint32 uiIndex = i * (uiTileNumZ + 1) + j;
			pVer[uiIndex].x = (i * 1.0f)/ uiTileNumX;
			pVer[uiIndex].y = 0;
			pVer[uiIndex].z = (j * 1.0f)/ uiTileNumZ;
			pVerTemp++;
		}
	}

	



	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	VSMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetData(pVertexData, VSVertexFormat::VF_POSITION);

	VSDataBuffer* pIndexData = NULL;
	pIndexData = MEM_NEW VSDataBuffer();
	VSMAC_ASSERT(pIndexData);

	if (!pIndexData->CreateEmptyBuffer(uiTileNumX * uiTileNumZ * 4, VSDataBuffer::DT_USHORT))
	{
		return 0;
	}

	MATH_USHORT_INDEX * pIndex = (MATH_USHORT_INDEX *)pIndexData->GetData();

	for (uint32 i = 0; i < uiTileNumX; i++)
	{
		for (uint32 j = 0; j < uiTileNumZ; j++)
		{
			*pIndex = i * uiTileNumZ + j;
			pIndex++;

			*pIndex = i * uiTileNumZ + j + 1;
			pIndex++;

			*pIndex = (i + 1) * uiTileNumZ + j;
			pIndex++;

			*pIndex = (i + 1) * uiTileNumZ + j + 1;
			pIndex++;

		}
	}



	


	VSIndexBuffer * pIndexBuffer = NULL;
	pIndexBuffer = MEM_NEW VSIndexBuffer();
	VSMAC_ASSERT(pIndexBuffer);

	pIndexBuffer->SetData(pIndexData);

	VSControlPointPatch * pMeshData = NULL;
	pMeshData = MEM_NEW VSControlPointPatch();

	VSMAC_ASSERT(pMeshData);

	pMeshData->SetVertexBuffer(pVertexBuffer);
	pMeshData->SetIndexBuffer(pIndexBuffer);
	SetMeshData(pMeshData);
	return 1;
}