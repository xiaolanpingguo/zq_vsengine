#include "graphic/node/terrain/roamterraingemotry.h"
#include "graphic/node/terrain/clodterrainnode.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/asyn/renderthread.h"
using namespace zq;
IMPLEMENT_RTTI(VSRoamTerrainGeometry,VSCLodTerrainGeometry)
BEGIN_ADD_PROPERTY(VSRoamTerrainGeometry,VSCLodTerrainGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSRoamTerrainGeometry)
IMPLEMENT_INITIAL_END
VSRoamTerrainGeometry::VSRoamTerrainGeometry()
{
	m_Variance[0].Clear();
	m_Variance[1].Clear();

	m_TriTreeNode[0].pOwner = this;
	m_TriTreeNode[1].pOwner = this;
	m_TriBuffer.Clear();
	m_uiCurBufferNum =0;
}

VSRoamTerrainGeometry::~VSRoamTerrainGeometry()
{

}
float VSRoamTerrainGeometry::RecursiveComputeVariance(VSArray<float>& Variance,uint32 uiIndex,
													   uint32 A,uint32 B,uint32 C)
{
	
	if (uiIndex >= Variance.GetNum())
	{
		return 0;
	}
	Vector3 * pVer = (Vector3 *)m_pMeshData->GetVertexBuffer()->GetPositionData(0)->GetData();
	VSMAC_ASSERT(pVer);

	uint32 uiMiddle = (B + C) >> 1;
	float fVariance = ABS(pVer[uiMiddle].y - (pVer[B].y + pVer[C].y) * 0.5f);
	float fLeftChildVariance = RecursiveComputeVariance(Variance,(uiIndex << 1) + 1,uiMiddle,A,B);
	float fRightChildVariance = RecursiveComputeVariance(Variance,(uiIndex << 1) + 2,uiMiddle,C,A);
	fVariance = Max(fVariance,fLeftChildVariance);
	fVariance = Max(fVariance,fRightChildVariance);

	Variance[uiIndex] = fVariance;
	return fVariance;
	
}
void VSRoamTerrainGeometry::ComputeVariance(uint32 uiCurLevel)
{
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	uint32 uiLevel = 1 << pTerrainNode->GetTessellationLevel();
	uint32 uiVarianceNum = (1 << (pTerrainNode->GetTessellationLevel() * 2 + 1)) - 1;

	m_Variance[0].SetBufferNum(uiVarianceNum);
	m_Variance[1].SetBufferNum(uiVarianceNum);
	m_TriBuffer.SetBufferNum(uiLevel * uiLevel * 2 * 2);
	for (uint32 i = 0; i < m_TriBuffer.GetNum();i++)
	{
		m_TriBuffer[i].pOwner = this;
	}
	m_uiCurBufferNum = 0;
	uint32 uiTri1A = uiLevel;
	uint32 uiTri1B = uiLevel * (uiLevel + 1) + uiLevel;
	uint32 uiTri1C = 0;

	uint32 uiTri2A = uiLevel * (uiLevel + 1);
	uint32 uiTri2B = 0;
	uint32 uiTri2C = uiLevel * (uiLevel + 1) + uiLevel;
	

	RecursiveComputeVariance(m_Variance[0],0,uiTri1A,uiTri1B,uiTri1C);
	RecursiveComputeVariance(m_Variance[1],0,uiTri2A,uiTri2B,uiTri2C);
}
void VSRoamTerrainGeometry::LinkNeighbor()
{
	if (m_pNeighbor[NT_LEFT])
	{
		m_TriTreeNode[0].pLeftNeighbor = &((VSRoamTerrainGeometry *)m_pNeighbor[NT_LEFT])->m_TriTreeNode[1];
	}
	if (m_pNeighbor[NT_RIGHT])
	{
		m_TriTreeNode[1].pLeftNeighbor = &((VSRoamTerrainGeometry *)m_pNeighbor[NT_RIGHT])->m_TriTreeNode[0];
	}
	if (m_pNeighbor[NT_TOP])
	{
		m_TriTreeNode[0].pRightNeighbor = &((VSRoamTerrainGeometry *)m_pNeighbor[NT_TOP])->m_TriTreeNode[1];
	}
	if (m_pNeighbor[NT_BOTTOM])
	{
		m_TriTreeNode[1].pRightNeighbor = &((VSRoamTerrainGeometry *)m_pNeighbor[NT_BOTTOM])->m_TriTreeNode[0];
	}

	m_TriTreeNode[0].pBaseNeighbor = &m_TriTreeNode[1];
	m_TriTreeNode[1].pBaseNeighbor = &m_TriTreeNode[0];

	m_TriTreeNode[0].pLeftChild = NULL;
	m_TriTreeNode[0].pRightChild = NULL;

	m_TriTreeNode[1].pLeftChild = NULL;
	m_TriTreeNode[1].pRightChild = NULL;

	m_TriTreeNode[0].pOwner = this;
	m_TriTreeNode[1].pOwner = this;
}
void VSRoamTerrainGeometry::RecursiveTessellate(VSTriTreeNode * pTri,const Vector3 & CameraPos,VSArray<float>& Variance,
												uint32 uiIndex,uint32 A,uint32 B,uint32 C)
{
	
	
	//���㹻�Ŀռ䱣�ּ������֣���Ϊ�ռ������Ǹ�������޶���������ȷ���ġ�
	if (((uiIndex << 1) + 2) < Variance.GetNum())
	{
		Vector3 * pVer = (Vector3 *)m_pMeshData->GetVertexBuffer()->GetPositionData(0)->GetData();
		if (!pVer)
		{
			return ;
		}

		VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
		

		uint32 uiMiddle = (B + C) >> 1;
		Vector3 Dist = CameraPos - pVer[uiMiddle];

		if (!pTri->pLeftChild)
		{
			float fRatio = ( Dist.GetSqrLength() ) / ( Max(Variance[uiIndex],1.0f) * pTerrainNode->GetCLODScale() );
			if (fRatio < 1.0f)
			{
				Split(pTri);
			}
		}
		if (pTri->pLeftChild)
		{


			RecursiveTessellate(pTri->pLeftChild,CameraPos,Variance,
				(uiIndex << 1) + 1,uiMiddle,C,A);

			RecursiveTessellate(pTri->pRightChild,CameraPos,Variance,
				(uiIndex << 1) + 2,uiMiddle,A,B);

		}
	}
}
void VSRoamTerrainGeometry::Split(VSTriTreeNode * pTri)
{
	if (pTri->pRightChild)
		return;

	if ( pTri->pBaseNeighbor && (pTri->pBaseNeighbor->pBaseNeighbor != pTri) )
		pTri->pBaseNeighbor->pOwner->Split(pTri->pBaseNeighbor);


	pTri->pLeftChild = GetBuffer();
	pTri->pRightChild = GetBuffer();

	if ( !pTri->pLeftChild || !pTri->pRightChild)
	{
		pTri->pLeftChild = NULL;
		pTri->pRightChild = NULL;
		return;
	}


	pTri->pLeftChild->pBaseNeighbor  = pTri->pLeftNeighbor;
	pTri->pLeftChild->pLeftNeighbor  = pTri->pRightChild;

	pTri->pRightChild->pBaseNeighbor  = pTri->pRightNeighbor;
	pTri->pRightChild->pRightNeighbor = pTri->pLeftChild;

	if (pTri->pLeftNeighbor != NULL)
	{
		if (pTri->pLeftNeighbor->pBaseNeighbor == pTri)
			pTri->pLeftNeighbor->pBaseNeighbor = pTri->pLeftChild;
		else if (pTri->pLeftNeighbor->pLeftNeighbor == pTri)
			pTri->pLeftNeighbor->pLeftNeighbor = pTri->pLeftChild;
		else if (pTri->pLeftNeighbor->pRightNeighbor == pTri)
			pTri->pLeftNeighbor->pRightNeighbor = pTri->pLeftChild;
		else
		{
			VSMAC_ASSERT(0);
		}
	}

	if (pTri->pRightNeighbor != NULL)
	{
		if (pTri->pRightNeighbor->pBaseNeighbor == pTri)
			pTri->pRightNeighbor->pBaseNeighbor = pTri->pRightChild;
		else if (pTri->pRightNeighbor->pRightNeighbor == pTri)
			pTri->pRightNeighbor->pRightNeighbor = pTri->pRightChild;
		else if (pTri->pRightNeighbor->pLeftNeighbor == pTri)
			pTri->pRightNeighbor->pLeftNeighbor = pTri->pRightChild;
		else
		{
			VSMAC_ASSERT(0);
		}
	}

	if (pTri->pBaseNeighbor != NULL)
	{
		if ( pTri->pBaseNeighbor->pLeftChild )
		{
			pTri->pBaseNeighbor->pLeftChild->pRightNeighbor = pTri->pRightChild;
			pTri->pBaseNeighbor->pRightChild->pLeftNeighbor = pTri->pLeftChild;
			pTri->pLeftChild->pRightNeighbor = pTri->pBaseNeighbor->pRightChild;
			pTri->pRightChild->pLeftNeighbor = pTri->pBaseNeighbor->pLeftChild;
		}
		else
			pTri->pBaseNeighbor->pOwner->Split(pTri->pBaseNeighbor);
	}
	else
	{
		// An edge triangle, trivial case.
		pTri->pLeftChild->pRightNeighbor = NULL;
		pTri->pRightChild->pLeftNeighbor = NULL;
	}
}
VSRoamTerrainGeometry::VSTriTreeNode * VSRoamTerrainGeometry::GetBuffer()
{
// 	if (m_uiCurBufferNum >= m_TriBuffer.GetNum())
// 	{
// 		return NULL;
// 	}
// 	else
	{
		VSTriTreeNode * pTriTree = &m_TriBuffer[m_uiCurBufferNum];
		m_uiCurBufferNum++;
		
		pTriTree->Clear();
		
		return pTriTree;
	}
}
void VSRoamTerrainGeometry::TessellateEx(const Vector3 & CameraPos, uint32 uiLevel)
{
	uint32 uiTri1A = uiLevel;
	uint32 uiTri1B = uiLevel * (uiLevel + 1) + uiLevel;
	uint32 uiTri1C = 0;

	uint32 uiTri2A = uiLevel * (uiLevel + 1);
	uint32 uiTri2B = 0;
	uint32 uiTri2C = uiLevel * (uiLevel + 1) + uiLevel;


	RecursiveTessellate(&m_TriTreeNode[0], CameraPos, m_Variance[0],
		0, uiTri1A, uiTri1B, uiTri1C);
	RecursiveTessellate(&m_TriTreeNode[1], CameraPos, m_Variance[1],
		0, uiTri2A, uiTri2B, uiTri2C);
}
void VSRoamTerrainGeometry::Tessellate(const Vector3 & CameraPos)
{
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	uint32 uiLevel = 1 << pTerrainNode->GetTessellationLevel();
	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_MulBufferSwtich))
	{
		TessellateEx(CameraPos, uiLevel);
	}
	else
	{
		struct VSTessellatePara
		{
			Vector3  CameraPos;
			uint32 uiLevel;
		};
		VSTessellatePara TessellatePara;
		TessellatePara.CameraPos = CameraPos;
		TessellatePara.uiLevel = uiLevel;
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSTessellateCommand,
			VSTessellatePara, TessellatePara, TessellatePara, VSRoamTerrainGeometry *,pRoamTerrainGeometry,this,
		{
			pRoamTerrainGeometry->TessellateEx(TessellatePara.CameraPos, TessellatePara.uiLevel);
		})
	}
	
}
void VSRoamTerrainGeometry::ClearInfoEx()
{
	LinkNeighbor();
	m_uiCurBufferNum = 0;
}
void VSRoamTerrainGeometry::ClearInfo()
{
	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_MulBufferSwtich))
	{
		ClearInfoEx();
	}
	else
	{

		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(VSClearInfoCommand,
			VSRoamTerrainGeometry *, pRoamTerrainGeometry, this,
		{
			pRoamTerrainGeometry->ClearInfoEx();
		})
	}
}
void VSRoamTerrainGeometry::UpdateOtherEx(uint32 uiLevel)
{
	uint32 uiTri1A = uiLevel;
	uint32 uiTri1B = uiLevel * (uiLevel + 1) + uiLevel;
	uint32 uiTri1C = 0;

	uint32 uiTri2A = uiLevel * (uiLevel + 1);
	uint32 uiTri2B = 0;
	uint32 uiTri2C = uiLevel * (uiLevel + 1) + uiLevel;
	MATH_USHORT_INDEX * pIndexData = (MATH_USHORT_INDEX *)m_pMeshData->GetIndexBuffer()->Lock();

	if (!pIndexData)
	{
		return;
	}

	uint32 uiCurRenderTriNum = 0;
	RecursiveBuildRenderTriange(&m_TriTreeNode[0], uiTri1A, uiTri1B, uiTri1C, pIndexData, uiCurRenderTriNum);
	RecursiveBuildRenderTriange(&m_TriTreeNode[1], uiTri2A, uiTri2B, uiTri2C, pIndexData, uiCurRenderTriNum);
	m_pMeshData->GetIndexBuffer()->UnLock();
	SetActiveNum(uiCurRenderTriNum);
}
void VSRoamTerrainGeometry::UpdateOther(double dAppTime)
{
	VSMAC_ASSERT(m_pMeshData);
	if (!GetMeshData()->GetIndexBuffer()->IsBindResource())
	{
		return;
	}
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	uint32 uiLevel = 1 << pTerrainNode->GetTessellationLevel();
	if (VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_MulBufferSwtich))
	{
		UpdateOtherEx(uiLevel);
	}
	else
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(VSTessellateCommand,
			uint32 ,uiLevel, uiLevel, VSRoamTerrainGeometry *, pRoamTerrainGeometry, this,
		{
			pRoamTerrainGeometry->UpdateOtherEx(uiLevel);;
		})
	}
	
}
void VSRoamTerrainGeometry::RecursiveBuildRenderTriange(VSTriTreeNode * pTri,
								 uint32 A,uint32 B,uint32 C,MATH_USHORT_INDEX * &pIndexData,uint32 &uiCurRenderTriNum)
{
	if (pTri->pLeftChild)
	{
		uint32 uiMiddle = (B + C) >> 1;
		RecursiveBuildRenderTriange(pTri->pLeftChild,uiMiddle,C,A,pIndexData,uiCurRenderTriNum);

		RecursiveBuildRenderTriange(pTri->pRightChild,uiMiddle,A,B,pIndexData,uiCurRenderTriNum);
	}
	else
	{
		*pIndexData = A;
		pIndexData++;

		*pIndexData = B;
		pIndexData++;

		*pIndexData = C;
		pIndexData++;
		uiCurRenderTriNum++;
	}

}
