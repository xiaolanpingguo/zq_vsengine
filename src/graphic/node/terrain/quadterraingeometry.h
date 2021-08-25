#pragma once
#include "graphic/node/terrain/clodterraingeometry.h"
namespace zq
{
	/*
		|Z j
		|
	   B|____C
		|   /|
		|  / |
		| /  |
	   A|/___|D______X i
	*/
	class VSStream;
	class GRAPHIC_API VSQuadTerrainGeometry : public VSCLodTerrainGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSQuadTerrainGeometry();
		virtual ~VSQuadTerrainGeometry();

	public:
		virtual uint32 GetTerrainGeometryType() {return TGT_QUAD;}
		virtual void ComputeVariance(uint32 uiCurLevel = 0);
		virtual void LinkNeighbor();	
		virtual void ClearInfo();
	protected:
		enum
		{
			AT_ALL		= 0x0000,
			AT_NLEFT	= 0x0001,
			AT_NRIGHT	= 0x0002,
			AT_NTOP		= 0x0004,
			AT_NBOTTOM	= 0x0008
			
		};

		virtual void Tessellate(const Vector3 & CameraPos);
		bool RecursiveComputeVariance(uint32 uiLevel);	
		void RecursiveTessellate(const Vector3 & CameraPos,uint32 uiLevel,uint32 A,uint32 B,
			uint32 C,uint32 D);	
		static inline void AddTriangle(MATH_USHORT_INDEX * &pIndexData,uint32 A,uint32 B,
							uint32 C,uint32 D,uint32 uiATFlag,unsigned int& uiCurRenderTriNum);
		virtual void UpdateOther(double dAppTime);
	protected:

		uint32 m_uiCurLevel;
		VSArray<float> m_Variance;
	};
	DECLARE_Ptr(VSQuadTerrainGeometry);
	VSTYPE_MARCO(VSQuadTerrainGeometry);


}
