#include "graphic/node/terrain/dlodterraingeometry.h"
#include "graphic/node/terrain/dlodterrainnode.h"
#include "graphic/node/mesh/triangleset.h"
#include "graphic/render/buffer/vertexbuffer.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSDLodTerrainGeometry,VSGeometry)
BEGIN_ADD_PROPERTY(VSDLodTerrainGeometry,VSGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDLodTerrainGeometry)
IMPLEMENT_INITIAL_END
VSDLodTerrainGeometry::VSDLodTerrainGeometry()
{

}
VSDLodTerrainGeometry::~VSDLodTerrainGeometry()
{

}
VSDLodTerrainNode * VSDLodTerrainGeometry::GetParentTerrainNode()const
{
	if (m_pParent)
	{
		return (VSDLodTerrainNode *)m_pParent->GetParent();
	}
	return NULL;
}
bool VSDLodTerrainGeometry::CreateMesh(uint32 uiIndexXInTerrain, uint32 uiIndexZInTerrain,
									   uint32 uiLevel,uint32 uiTessellationLevel)
{
	VSDLodTerrainNode * pTerrainNode = GetParentTerrainNode();
	if (uiLevel >= uiTessellationLevel || !pTerrainNode)
	{
		return 0;
	}
	uint32 uiVertexLenght = (1 << ( uiTessellationLevel - uiLevel) ) + 1;

	VSDataBuffer * pVertexData = NULL ;
	pVertexData = MEM_NEW VSDataBuffer();
	if (!pVertexData->CreateEmptyBuffer(uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 4,VSDataBuffer::DT_FLOAT32_3))
	{
		return 0;
	}


	Vector3 * pVer = (Vector3 *)pVertexData->GetData();
	Vector3 * pVerTemp = pVer;
	uint32 uiBiasX = uiIndexXInTerrain * (1 << uiTessellationLevel);
	uint32 uiBiasZ = uiIndexZInTerrain * (1 << uiTessellationLevel);
	for (uint32 i = 0 ; i < uiVertexLenght ; i++)
	{
		for(uint32 j = 0 ; j < uiVertexLenght ; j++)
		{
			uint32 uiIndex = i * uiVertexLenght + j;
			pVer[uiIndex].x = VSTerrainNode::WIDTH_SCALE * (uiBiasX + (i << uiLevel)) * 1.0f;
			pVer[uiIndex].y = pTerrainNode->GetHeight(uiBiasX + (i << uiLevel),uiBiasZ + (j << uiLevel));
			pVer[uiIndex].z = VSTerrainNode::WIDTH_SCALE * (uiBiasZ + (j << uiLevel)) * 1.0f;
			pVerTemp++;
		}
	}

	for(uint32 i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		uint32 uiIndex = i;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}

	for(uint32 i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		uint32 uiIndex = uiVertexLenght * i + uiVertexLenght - 1;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}

	for(int32 i = uiVertexLenght - 1 ; i > 0 ; i--)
	{
		uint32 uiIndex = uiVertexLenght * (uiVertexLenght - 1 )+ i;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}

	for(int32 i = uiVertexLenght - 1 ; i > 0 ; i--)
	{
		uint32 uiIndex = uiVertexLenght * i;
		pVerTemp->x = pVer[uiIndex].x;
		pVerTemp->y = pVer[uiIndex].y - pTerrainNode->GetHeightScale() * pTerrainNode->GetDLodExtend();
		pVerTemp->z = pVer[uiIndex].z;
		pVerTemp++;
	}



	VSVertexBuffer * pVertexBuffer = NULL;
	pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	VSMAC_ASSERT(pVertexBuffer);

	pVertexBuffer->SetData(pVertexData,VSVertexFormat::VF_POSITION);

	VSDataBuffer* pIndexData = NULL;
	pIndexData = MEM_NEW VSDataBuffer();
	VSMAC_ASSERT(pIndexData);

	if (!pIndexData->CreateEmptyBuffer(
		(uiVertexLenght - 1) * (uiVertexLenght - 1) * 2 * 3 + (uiVertexLenght - 1) * 4 * 2 * 3,VSDataBuffer::DT_USHORT))
	{
		return 0;
	}
	
	MATH_USHORT_INDEX * pIndex = (MATH_USHORT_INDEX *)pIndexData->GetData();

	for (uint32 i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		for (uint32 j = 0 ; j < uiVertexLenght - 1; j++)
		{
			*pIndex = i * uiVertexLenght + j;
			pIndex++;

			*pIndex = i * uiVertexLenght + j + 1;
			pIndex++;

			*pIndex = (i + 1) * uiVertexLenght + j + 1;
			pIndex++;





			*pIndex = i * uiVertexLenght + j;
			pIndex++;

			*pIndex = (i + 1) * uiVertexLenght + j + 1;
			pIndex++;

			*pIndex = (i + 1) * uiVertexLenght + j;
			pIndex++;






		}
	}



	for(uint32 i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		uint32 uiIndex = i;
		uint32 uiIndex1= i + 1;
		uint32 uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 0 + i;
		uint32 uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 0 + i + 1;
		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex3;
		pIndex++;
	}

	for(uint32 i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		uint32 uiIndex = uiVertexLenght * i + uiVertexLenght - 1;
		uint32 uiIndex1 = uiVertexLenght * (i + 1) + uiVertexLenght - 1;
		uint32 uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 1 + i;
		uint32 uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 1 + i + 1;

		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex3;
		pIndex++;
	}

	for(uint32 i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		uint32 uiIndex = uiVertexLenght * (uiVertexLenght - 1 ) + uiVertexLenght - 1 - i;
		uint32 uiIndex1 = uiVertexLenght * (uiVertexLenght - 1 )+ uiVertexLenght - 1 - i - 1;
		uint32 uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 2 + i;
		uint32 uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 2 + i + 1;

		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex3;
		pIndex++;
	}

	for(uint32 i = 0 ; i < uiVertexLenght - 1 ; i++)
	{
		uint32 uiIndex = uiVertexLenght * (uiVertexLenght - 1 - i);
		uint32 uiIndex1 = uiVertexLenght * (uiVertexLenght - 1 - i - 1);
		uint32 uiIndex2= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 3 + i;
		uint32 uiIndex3= uiVertexLenght * uiVertexLenght + (uiVertexLenght - 1) * 3 + i + 1;

		*pIndex = uiIndex;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex1;
		pIndex++;

		*pIndex = uiIndex2;
		pIndex++;

		if (i == uiVertexLenght - 2)
		{
			*pIndex = uiVertexLenght * uiVertexLenght;
			pIndex++;
		}
		else
		{
			*pIndex = uiIndex3;
			pIndex++;
		}
	}

	
	VSIndexBuffer * pIndexBuffer = NULL;
	pIndexBuffer = MEM_NEW VSIndexBuffer();
	VSMAC_ASSERT(pIndexBuffer);

// 	pIndexBuffer->SetStatic(false);
// 	pIndexBuffer->SetLockFlag(VSInheritBind::LF_DISCARD);
	pIndexBuffer->SetData(pIndexData);

	VSTriangleSet * pMeshData = NULL;
	pMeshData = MEM_NEW VSTriangleSet();

	VSMAC_ASSERT(pMeshData);

	pMeshData->SetVertexBuffer(pVertexBuffer);
	pMeshData->SetIndexBuffer(pIndexBuffer);
	SetMeshData(pMeshData);
	return 1;
}