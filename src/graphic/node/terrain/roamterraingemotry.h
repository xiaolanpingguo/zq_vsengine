#pragma once
#include "graphic/node/terrain/clodterraingeometry.h"
namespace zq
{

	/*
			    A
				 /\
				/  \
			  C/____\ B
	顺时针排列，可见

	 |Z
	 |
	A|____B
	 |   /|
	 |  / |
	 | /  |
	C|/___|_______X

	*/
	class VSStream;
	class GRAPHIC_API VSRoamTerrainGeometry : public VSCLodTerrainGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSRoamTerrainGeometry();
		virtual ~VSRoamTerrainGeometry();


	public:
		virtual uint32 GetTerrainGeometryType() {return TGT_ROAM;}
		class VSTriTreeNode
		{
		public:
			VSTriTreeNode *pBaseNeighbor;
			VSTriTreeNode *pLeftNeighbor;
			VSTriTreeNode *pRightNeighbor;
			VSTriTreeNode *pLeftChild;
			VSTriTreeNode *pRightChild;

			VSRoamTerrainGeometry * pOwner;
			VSTriTreeNode()
			{
				pOwner = NULL;
				pBaseNeighbor = NULL;
				pLeftNeighbor = NULL;
				pRightNeighbor = NULL;
				pLeftChild = NULL;
				pRightChild = NULL;
			}
			inline void Clear()
			{
				//pOwner = NULL;
				pBaseNeighbor = NULL;
				pLeftNeighbor = NULL;
				pRightNeighbor = NULL;
				pLeftChild = NULL;
				pRightChild = NULL;
			}
			
		};
		virtual void ComputeVariance(uint32 uiCurLevel = 0);
		virtual void LinkNeighbor();	
		virtual void ClearInfo();
	protected:
		
		virtual void Tessellate(const Vector3 & CameraPos);
		float RecursiveComputeVariance(VSArray<float>& Variance,uint32 uiIndex,
			uint32 A,uint32 B,uint32 C);	
		void RecursiveTessellate(VSTriTreeNode * pTri,const Vector3 & CameraPos,VSArray<float>& Variance,
			uint32 uiIndex,uint32 A,uint32 B,uint32 C);
		void Split(VSTriTreeNode * pTri);
		VSTriTreeNode * GetBuffer();
		
		void RecursiveBuildRenderTriange(VSTriTreeNode * pTri,
			uint32 A,uint32 B,uint32 C,MATH_USHORT_INDEX * &pIndexData,uint32 &uiCurRenderTriNum);
		virtual void UpdateOther(double dAppTime);

		void TessellateEx(const Vector3 & CameraPos, uint32 uiLevel);
		void ClearInfoEx();
		void UpdateOtherEx(uint32 uiLevel);
	protected:
		VSArray<float> m_Variance[2];
		VSTriTreeNode	m_TriTreeNode[2];
		VSArray<VSTriTreeNode>	m_TriBuffer;
		uint32 m_uiCurBufferNum;
	};
	DECLARE_Ptr(VSRoamTerrainGeometry);
	VSTYPE_MARCO(VSRoamTerrainGeometry);
}
