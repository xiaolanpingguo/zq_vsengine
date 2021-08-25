#pragma once
#include "graphic/node/terrain/terrainnode.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSCLodTerrainNode : public VSTerrainNode
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //Terrain Node Type
		{
			TNT_ROAM,
			TNT_QUAD,
			TNT_MAX
		};
		virtual ~VSCLodTerrainNode();
		VSCLodTerrainNode();
	protected:
		

		
		float	m_fCLODScale;

		uint32 m_uiTerrainNodeType;
		void LinkNeighbor();
		void ComputeVariance();

		virtual bool CreateChild();
		virtual void UpdateNodeAll(double dAppTime);
	public:
		virtual uint32 AddChild(VSSpatial * pChild);

		void SetCLODScale(float fCLODScale);

		inline float GetCLODScale()const { return m_fCLODScale; }

		void SetTerrainNodeType(uint32 uiTerrainNodeType);
	};
	DECLARE_Ptr(VSCLodTerrainNode);
	VSTYPE_MARCO(VSCLodTerrainNode);
}
