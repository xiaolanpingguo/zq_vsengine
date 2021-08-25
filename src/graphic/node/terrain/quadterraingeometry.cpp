#include "graphic/node/terrain/quadterraingeometry.h"
#include "graphic/node/terrain/clodterrainnode.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/asyn/renderthread.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSQuadTerrainGeometry,VSCLodTerrainGeometry)
BEGIN_ADD_PROPERTY(VSQuadTerrainGeometry,VSCLodTerrainGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSQuadTerrainGeometry)
IMPLEMENT_INITIAL_END
VSQuadTerrainGeometry::VSQuadTerrainGeometry()
{
	m_uiCurLevel = 0;
}
VSQuadTerrainGeometry::~VSQuadTerrainGeometry()
{

}
void VSQuadTerrainGeometry::ComputeVariance(uint32 uiCurLevel)
{
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	uint32 uiVarianceLength = (1 << pTerrainNode->GetTessellationLevel()) + 1;
	if (m_Variance.GetNum() != uiVarianceLength * uiVarianceLength)
	{
		m_Variance.SetBufferNum(uiVarianceLength * uiVarianceLength);
		memset(&m_Variance[0],0,m_Variance.GetNum());
	}
	
	RecursiveComputeVariance(uiCurLevel);
}
bool VSQuadTerrainGeometry::RecursiveComputeVariance(uint32 uiLevel)
{
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	if (uiLevel >= pTerrainNode->GetTessellationLevel())
	{
		VSMAC_ASSERT(0);
		return 0;
	}
	Vector3 * pVer = (Vector3 *)m_pMeshData->GetVertexBuffer()->GetPositionData(0)->GetData();
	if (!pVer)
	{
		return 0;
	}
	float fVariance = 0;
	uint32 uiBias = pTerrainNode->GetTessellationLevel() - uiLevel;
	uint32 uiVarianceLength = (1 << pTerrainNode->GetTessellationLevel()) + 1;
	//遍历刚层次的节点 1 << uiLevel 个
	for (uint32 i = 0 ; i < (unsigned int)(1 << uiLevel) ; i++)
	{
		for(uint32 j = 0 ; j < (unsigned int)(1 << uiLevel) ; j++)
		{
			//i方向上的(i << uiBias)，j方向上的(j << uiBias)，宽度(1 << pTerrainNode->GetTessellationLevel() + 1)
			uint32 A = (j << uiBias) + (i << uiBias) * uiVarianceLength;
			uint32 B = ((j + 1) << uiBias) + (i << uiBias) * uiVarianceLength;
			uint32 C = ((j + 1) << uiBias) + ((i + 1) << uiBias) * uiVarianceLength;
			uint32 D = (j << uiBias) + ((i + 1) << uiBias) * uiVarianceLength;

			if (uiLevel == pTerrainNode->GetTessellationLevel() - 1)
			{
				float fVarianceI[6] = {0};
				fVarianceI[0] = ABS(pVer[(A + B) >> 1].y - (pVer[A].y + pVer[B].y) * 0.5f);
				fVarianceI[1] = ABS(pVer[(B + C) >> 1].y - (pVer[B].y + pVer[C].y) * 0.5f);
				fVarianceI[2] = ABS(pVer[(C + D) >> 1].y - (pVer[C].y + pVer[D].y) * 0.5f);
				fVarianceI[3] = ABS(pVer[(D + A) >> 1].y - (pVer[D].y + pVer[A].y) * 0.5f);
				fVarianceI[4] = ABS(pVer[(A + C) >> 1].y - (pVer[A].y + pVer[C].y) * 0.5f);
				fVarianceI[5] = ABS(pVer[(B + D) >> 1].y - (pVer[B].y + pVer[D].y) * 0.5f);

				for( uint32 i = 0 ; i < 6 ; i++)
				{
					if(fVariance < fVarianceI[i])
						fVariance = fVarianceI[i];
				}
				m_Variance[(A + C) >> 1] = fVariance;
			}
			else
			{
				float fVarianceI[14] = {0};
				fVarianceI[0] = ABS(pVer[(A + B) >> 1].y - (pVer[A].y + pVer[B].y) * 0.5f);
				fVarianceI[1] = ABS(pVer[(B + C) >> 1].y - (pVer[B].y + pVer[C].y) * 0.5f);
				fVarianceI[2] = ABS(pVer[(C + D) >> 1].y - (pVer[C].y + pVer[D].y) * 0.5f);
				fVarianceI[3] = ABS(pVer[(D + A) >> 1].y - (pVer[D].y + pVer[A].y) * 0.5f);
				fVarianceI[4] = ABS(pVer[(A + C) >> 1].y - (pVer[A].y + pVer[C].y) * 0.5f);
				fVarianceI[5] = ABS(pVer[(B + D) >> 1].y - (pVer[B].y + pVer[D].y) * 0.5f);
				//CL Child Level 
				uint32 uiCLBegin = 0;
				uint32 uiCLEnd = ((1 << (uiLevel + 1)) - 1);
				uint32 uiCLBias = uiBias - 1;
				//left two child
				if (m_pNeighbor[NT_LEFT])
				{
					uint32 uiLNTopA = (uiCLEnd << uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
					uint32 uiLNTopC = ((uiCLEnd + 1 ) << uiCLBias) * uiVarianceLength + ((uiCLEnd + 1) << uiCLBias);

					uint32 uiLNBottomA = (uiCLEnd << uiCLBias) * uiVarianceLength + ((uiCLEnd - 1) << uiCLBias);
					uint32 uiLNBottomC = ((uiCLEnd + 1 )<< uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
					fVarianceI[6] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_LEFT])->m_Variance[(uiLNTopA + uiLNTopC) >> 1];
					fVarianceI[7] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_LEFT])->m_Variance[(uiLNBottomA + uiLNBottomC) >> 1];
				}
				//top two child
				if (m_pNeighbor[NT_TOP])
				{
					uint32 uiTNLeftA = (uiCLBegin << uiCLBias) * uiVarianceLength + (uiCLBegin << uiCLBias);
					uint32 uiTNLeftC = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + ((uiCLBegin + 1) << uiCLBias);

					uint32 uiTNRightA = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + (uiCLBegin << uiCLBias);
					uint32 uiTNRightC = ((uiCLBegin + 2) << uiCLBias) * uiVarianceLength + ((uiCLBegin + 1) << uiCLBias);
					fVarianceI[8] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_TOP])->m_Variance[(uiTNLeftA + uiTNLeftC) >> 1];
					fVarianceI[9] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_TOP])->m_Variance[(uiTNRightA + uiTNRightC) >> 1];
				}
				//right two child
				if (m_pNeighbor[NT_RIGHT])
				{
					uint32 uiLNTopA = (uiCLBegin << uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
					uint32 uiLNTopC = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + ((uiCLEnd + 1) << uiCLBias);

					uint32 uiLNBottomA = (uiCLBegin << uiCLBias) * uiVarianceLength + ((uiCLEnd - 1) << uiCLBias);
					uint32 uiLNBottomC = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
 					fVarianceI[10] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_RIGHT])->m_Variance[(uiLNTopA + uiLNTopC) >> 1];
					fVarianceI[11] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_RIGHT])->m_Variance[(uiLNBottomA + uiLNBottomC) >> 1];
				}
				//bottom two child
				if (m_pNeighbor[NT_BOTTOM])
				{
					uint32 uiTNLeftA = (uiCLBegin << uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
					uint32 uiTNLeftC = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + ((uiCLEnd + 1) << uiCLBias);

					uint32 uiTNRightA = ((uiCLBegin + 1) << uiCLBias) * uiVarianceLength + (uiCLEnd << uiCLBias);
					uint32 uiTNRightC = ((uiCLBegin + 2) << uiCLBias) * uiVarianceLength + ((uiCLEnd + 1) << uiCLBias);
					fVarianceI[12] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_BOTTOM])->m_Variance[(uiTNLeftA + uiTNLeftC) >> 1];
					fVarianceI[13] = ((VSQuadTerrainGeometry *)m_pNeighbor[NT_BOTTOM])->m_Variance[(uiTNRightA + uiTNRightC) >> 1];
				}
				for( uint32 i = 0 ; i < 14 ; i++)
				{
					if(fVariance < fVarianceI[i])
						fVariance = fVarianceI[i];
				}
				m_Variance[(A + C) >> 1] = fVariance * pTerrainNode->GetCLODScale() / (pTerrainNode->GetCLODScale() - 1);
			}
		}
	}
	
	return 1;
}
void VSQuadTerrainGeometry::LinkNeighbor()
{

}
void VSQuadTerrainGeometry::ClearInfo()
{
	m_uiCurLevel = 0;
}
void VSQuadTerrainGeometry::Tessellate(const Vector3 & CameraPos)
{
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	uint32 uiLevel = 1 << pTerrainNode->GetTessellationLevel();

	uint32 A = 0;
	uint32 B = uiLevel;
	uint32 C = uiLevel * (uiLevel + 1) + uiLevel;

	uint32 D = uiLevel * (uiLevel + 1);

	RecursiveTessellate(CameraPos,0,A,B,C,D);
}
void VSQuadTerrainGeometry::RecursiveTessellate(const Vector3 & CameraPos,uint32 uiLevel,uint32 A,uint32 B,
						   uint32 C,uint32 D)
{
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	if (uiLevel >= pTerrainNode->GetTessellationLevel())
	{
		return ;
	}
	Vector3 * pVer = (Vector3 *)m_pMeshData->GetVertexBuffer()->GetPositionData(0)->GetData();
	VSMAC_ASSERT(pVer);

	if (m_uiCurLevel < uiLevel)
	{
		m_uiCurLevel = uiLevel;
	}


	uint32 uiMiddle = (B + D) >> 1;
	Vector3 Dist = CameraPos - pVer[uiMiddle];
	Vector3 Edge = pVer[A] - pVer[B];
	float fRatio = Dist.GetSqrLength() / (Edge.GetSqrLength() * Max(m_Variance[uiMiddle],1.0f) * pTerrainNode->GetCLODScale());
	if (fRatio < 1.0f)
	{
		RecursiveTessellate(CameraPos,uiLevel + 1,A,(A + B) >> 1,(B + D) >> 1,(A + D) >> 1);
		RecursiveTessellate(CameraPos,uiLevel + 1,(A + B) >> 1,B,(B + C) >> 1,(B + D) >> 1);
		RecursiveTessellate(CameraPos,uiLevel + 1,(B + D) >> 1,(B + C) >> 1,C,(C + D) >> 1);
		RecursiveTessellate(CameraPos,uiLevel + 1,(A + D) >> 1,(B + D) >> 1,(C + D) >> 1,C);
	}
	
}

/*
_________
|\  |  /|
|6\5|4/3|
|__\|/__|
|  /|\ 2|
|7/0|1\ | 
|/__|__\|



*/
void VSQuadTerrainGeometry::UpdateOther(double dAppTime)
{
	VSMAC_ASSERT(m_pMeshData);
	if (!GetMeshData()->GetIndexBuffer()->IsBindResource())
	{
		return;
	}
	struct VSQuadTerrainUpdateGeometryPara
	{
		uint32 uiTessellationLevel;
		VSQuadTerrainGeometry * pTG;
		uint32 m_uiCurLevel;
		uint32 m_NeighborLevel[VSCLodTerrainGeometry::NT_MAX];
		VSQuadTerrainGeometry * pNeightbor[VSCLodTerrainGeometry::NT_MAX];
	};
	VSQuadTerrainUpdateGeometryPara QuadTerrainUpdateGeometryPara;
	VSCLodTerrainNode * pTerrainNode = (VSCLodTerrainNode *)m_pParent;
	QuadTerrainUpdateGeometryPara.uiTessellationLevel = pTerrainNode->GetTessellationLevel();
	QuadTerrainUpdateGeometryPara.pTG = this;
	QuadTerrainUpdateGeometryPara.m_uiCurLevel = m_uiCurLevel;
	for (uint32 i = 0 ; i < VSCLodTerrainGeometry::NT_MAX ;i++)
	{
		QuadTerrainUpdateGeometryPara.pNeightbor[i] = ((VSQuadTerrainGeometry *)m_pNeighbor[i]);
		if (QuadTerrainUpdateGeometryPara.pNeightbor[i])
		{
			QuadTerrainUpdateGeometryPara.m_NeighborLevel[i] = ((VSQuadTerrainGeometry *)m_pNeighbor[i])->m_uiCurLevel;
		}
	}
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(VSQuadTerrainUpdateGeometryCommand,
		VSQuadTerrainUpdateGeometryPara,QuadTerrainUpdateGeometryPara,QuadTerrainUpdateGeometryPara,
	{
		int32 iLevelDelt = 0; 

		MATH_USHORT_INDEX * pIndexData = (MATH_USHORT_INDEX *)QuadTerrainUpdateGeometryPara.pTG->GetMeshData()->GetIndexBuffer()->Lock();

		if (!pIndexData)
		{
			return UGRI_FAIL;
		}
		uint32 uiCurRenderTriNum = 0;
		uint32 uiLeftTopFanFlag = 0;
		uint32 uiLeftBottomFanFlag = 0;
		uint32 uiRightTopFanFlag = 0;
		uint32 uiRightBottomFanFlag = 0;

		uint32 uiBias = QuadTerrainUpdateGeometryPara.uiTessellationLevel - QuadTerrainUpdateGeometryPara.m_uiCurLevel;
		uint32 uiVarianceLength = (1 << QuadTerrainUpdateGeometryPara.uiTessellationLevel) + 1;
		uint32 uiLength = (1 << QuadTerrainUpdateGeometryPara.m_uiCurLevel);
		for (uint32 i = 1 ; i < uiLength - 1; i++)
		{
			for(uint32 j = 1 ; j < uiLength - 1; j++)
			{

				uint32 A = (i << uiBias) * uiVarianceLength + (j << uiBias);
				uint32 B = ((i + 1) << uiBias) * uiVarianceLength + (j << uiBias);
				uint32 C = ((i + 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				uint32 D = (i << uiBias) * uiVarianceLength + ((j + 1) << uiBias);

				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}
		if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_LEFT])
		{
			iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_LEFT];
			if (iLevelDelt >= 0 && iLevelDelt == 1)//写成 == 1 其实就可以
			{

				for (uint32 j = 1 ; j < uiLength - 1 ; j ++)
				{
					uint32 A = (j << uiBias);
					uint32 B = ((1) << uiBias) * uiVarianceLength + (j << uiBias);
					uint32 C = ((1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					uint32 D = ((j + 1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NLEFT,uiCurRenderTriNum);
				}
				uiLeftTopFanFlag |= 0x01;
				uiLeftBottomFanFlag |= 0x01;


			}
			else
			{

				for (uint32 j = 1 ; j < uiLength - 1 ; j ++)
				{
					uint32 A = (j << uiBias);
					uint32 B = ((1) << uiBias) * uiVarianceLength + (j << uiBias);
					uint32 C = ((1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					uint32 D = ((j + 1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
				}

			}
		}
		else
		{
			for (uint32 j = 1 ; j < uiLength - 1 ; j ++)
			{
				uint32 A = (j << uiBias);
				uint32 B = ((1) << uiBias) * uiVarianceLength + (j << uiBias);
				uint32 C = ((1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				uint32 D = ((j + 1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}

		}
		if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_RIGHT])
		{
			iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_RIGHT];
			if (iLevelDelt >= 0 && iLevelDelt == 1)//写成 == 1 其实就可以
			{

				for (uint32 j = 1 ; j < uiLength - 1 ; j ++)
				{
					uint32 A = ((uiLength - 1) << uiBias) * uiVarianceLength + (j << uiBias);
					uint32 B = (uiLength << uiBias) * uiVarianceLength + (j << uiBias);
					uint32 C = (uiLength << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					uint32 D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NRIGHT,uiCurRenderTriNum);
				}
				uiRightTopFanFlag |= 0x01;
				uiRightBottomFanFlag |= 0x01;


			}
			else
			{

				for (uint32 j = 1 ; j < uiLength - 1 ; j ++)
				{
					uint32 A = ((uiLength - 1) << uiBias) * uiVarianceLength + (j << uiBias);
					uint32 B = (uiLength << uiBias) * uiVarianceLength + (j << uiBias);
					uint32 C = (uiLength << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					uint32 D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
				}

			}
		}
		else
		{
			for (uint32 j = 1 ; j < uiLength - 1 ; j ++)
			{
				uint32 A = ((uiLength - 1) << uiBias) * uiVarianceLength + (j << uiBias);
				uint32 B = (uiLength << uiBias) * uiVarianceLength + (j << uiBias);
				uint32 C = (uiLength << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				uint32 D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((j + 1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}

		}
		if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_TOP])
		{
			iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_TOP];
			if (iLevelDelt >= 0 && iLevelDelt == 1)//写成 == 1 其实就可以
			{

				for (uint32 i = 1 ; i < uiLength - 1 ; i ++)
				{
					uint32 A = (i << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
					uint32 B = ((i + 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
					uint32 C = ((i + 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
					uint32 D = (i << uiBias) * uiVarianceLength + (uiLength << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP,uiCurRenderTriNum);
				}
				uiLeftTopFanFlag |= 0x02;
				uiRightTopFanFlag |= 0x02;


			}
			else
			{
				for (uint32 i = 1 ; i < uiLength - 1 ; i ++)
				{
					uint32 A = (i << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
					uint32 B = ((i + 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
					uint32 C = ((i + 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
					uint32 D = (i << uiBias) * uiVarianceLength + (uiLength << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
				}

			}
		}
		else
		{
			for (uint32 i = 1 ; i < uiLength - 1 ; i ++)
			{
				uint32 A = (i << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
				uint32 B = ((i + 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
				uint32 C = ((i + 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
				uint32 D = (i << uiBias) * uiVarianceLength + (uiLength << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}
		if (QuadTerrainUpdateGeometryPara.pNeightbor[NT_BOTTOM])
		{
			iLevelDelt = QuadTerrainUpdateGeometryPara.m_uiCurLevel - QuadTerrainUpdateGeometryPara.m_NeighborLevel[NT_BOTTOM];
			if (iLevelDelt >= 0 && iLevelDelt == 1) //写成 == 1 其实就可以
			{

				for (uint32 i = 1 ; i < uiLength - 1 ; i ++)
				{
					uint32 A = (i << uiBias) * uiVarianceLength;
					uint32 B = ((i + 1) << uiBias) * uiVarianceLength;
					uint32 C = ((i + 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
					uint32 D = (i << uiBias) * uiVarianceLength + ((1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM,uiCurRenderTriNum);
				}
				uiLeftBottomFanFlag |= 0x02;
				uiRightBottomFanFlag |= 0x02;					

			}
			else
			{
				for (uint32 i = 1 ; i < uiLength - 1 ; i ++)
				{
					uint32 A = (i << uiBias) * uiVarianceLength;
					uint32 B = ((i + 1) << uiBias) * uiVarianceLength;
					uint32 C = ((i + 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
					uint32 D = (i << uiBias) * uiVarianceLength + ((1) << uiBias);
					VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
				}

			}
		}
		else
		{
			for (uint32 i = 1 ; i < uiLength - 1 ; i ++)
			{
				uint32 A = (i << uiBias) * uiVarianceLength;
				uint32 B = ((i + 1) << uiBias) * uiVarianceLength;
				uint32 C = ((i + 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
				uint32 D = (i << uiBias) * uiVarianceLength + ((1) << uiBias);
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}

		}
		//处理4个角
		//left top
		{
			uint32 A = ((uiLength - 1) << uiBias);
			uint32 B = ((1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
			uint32 C = ((1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
			uint32 D = (uiLength << uiBias);
			if (uiLeftTopFanFlag == 3)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP | AT_NLEFT,uiCurRenderTriNum);
			}
			else if (uiLeftTopFanFlag == 2)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP,uiCurRenderTriNum);
			}
			else if (uiLeftTopFanFlag == 1)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NLEFT,uiCurRenderTriNum);
			}
			else
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}

		//right top
		{
			uint32 A = ((uiLength - 1) << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
			uint32 B = (uiLength << uiBias) * uiVarianceLength + ((uiLength - 1) << uiBias);
			uint32 C = (uiLength << uiBias) * uiVarianceLength + (uiLength << uiBias);
			uint32 D = ((uiLength - 1) << uiBias) * uiVarianceLength + (uiLength << uiBias);
			if (uiRightTopFanFlag == 3)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP | AT_NRIGHT,uiCurRenderTriNum);
			}
			else if (uiRightTopFanFlag == 2)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NTOP,uiCurRenderTriNum);
			}
			else if (uiRightTopFanFlag == 1)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NRIGHT,uiCurRenderTriNum);
			}
			else
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}

		//left bottom
		{
			uint32 A = 0;
			uint32 B = ((1) << uiBias) * uiVarianceLength;
			uint32 C = ((1) << uiBias) * uiVarianceLength + ((1) << uiBias);
			uint32 D = ((1) << uiBias);
			if (uiLeftBottomFanFlag == 3)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM | AT_NLEFT,uiCurRenderTriNum);
			}
			else if (uiLeftBottomFanFlag == 2)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM,uiCurRenderTriNum);
			}
			else if (uiLeftBottomFanFlag == 1)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NLEFT,uiCurRenderTriNum);
			}
			else
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}


		//rigth bottom
		{
			uint32 A = ((uiLength - 1) << uiBias) * uiVarianceLength;
			uint32 B = (uiLength << uiBias) * uiVarianceLength;
			uint32 C = (uiLength << uiBias) * uiVarianceLength + ((1) << uiBias);
			uint32 D = ((uiLength - 1) << uiBias) * uiVarianceLength + ((1) << uiBias);
			if (uiRightBottomFanFlag == 3)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM | AT_NRIGHT,uiCurRenderTriNum);
			}
			else if (uiRightBottomFanFlag == 2)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NBOTTOM,uiCurRenderTriNum);
			}
			else if (uiRightBottomFanFlag == 1)
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_NRIGHT,uiCurRenderTriNum);
			}
			else
			{
				VSQuadTerrainGeometry::AddTriangle(pIndexData,A,B,C,D,AT_ALL,uiCurRenderTriNum);
			}
		}

		QuadTerrainUpdateGeometryPara.pTG->GetMeshData()->GetIndexBuffer()->UnLock();
		QuadTerrainUpdateGeometryPara.pTG->SetActiveNum(uiCurRenderTriNum);
	})
}
void VSQuadTerrainGeometry::AddTriangle(MATH_USHORT_INDEX * &pIndexData,uint32 A,uint32 B,
										uint32 C,uint32 D,uint32 uiATFlag,uint32 &uiCurRenderTriNum)
{
	uint32 M = (A + C) >> 1;
	uint32 AB = (A + B) >> 1;
	uint32 BC = (C + B) >> 1;
	uint32 CD = (C + D) >> 1;
	uint32 DA = (D + A) >> 1;
	if (uiATFlag & AT_NBOTTOM)
	{
		*pIndexData = A;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = B;
		pIndexData++;
		uiCurRenderTriNum += 1;	
	}
	else
	{
		//0
		*pIndexData = A;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = AB;
		pIndexData++;

		//1
		*pIndexData = AB;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = B;
		pIndexData++;

		uiCurRenderTriNum += 2;
		
	}
	
	if (uiATFlag & AT_NRIGHT)
	{
		*pIndexData = B;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = C;
		pIndexData++;
		uiCurRenderTriNum += 1;
	}
	else
	{
		//2
		*pIndexData = B;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = BC;
		pIndexData++;

		//3
		*pIndexData = BC;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = C;
		pIndexData++;
		uiCurRenderTriNum += 2;
		
	}
	if (uiATFlag & AT_NTOP)
	{
		*pIndexData = C;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = D;
		pIndexData++;
		uiCurRenderTriNum += 1;
	}
	else
	{
		//4
		*pIndexData = C;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = CD;
		pIndexData++;

		//5
		*pIndexData = CD;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = D;
		pIndexData++;
		uiCurRenderTriNum += 2;
		
	}
	if (uiATFlag & AT_NLEFT)
	{
		*pIndexData = D;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = A;
		pIndexData++;
		uiCurRenderTriNum += 1;
	}
	else
	{

		//6
		*pIndexData = D;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = DA;
		pIndexData++;

		//7
		*pIndexData = DA;
		pIndexData++;
		*pIndexData = M;
		pIndexData++;
		*pIndexData = A;
		pIndexData++;
		uiCurRenderTriNum += 2;
		

	}
	

}
