#pragma once
#include "graphic/node/geometry.h"
namespace zq
{

	class VSDLodTerrainNode;
	class GRAPHIC_API VSDLodTerrainGeometry : public VSGeometry
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSDLodTerrainGeometry();
		virtual ~VSDLodTerrainGeometry();
		bool CreateMesh(uint32 uiIndexXInTerrain, uint32 uiIndexZInTerrain,
			uint32 uiLevel,uint32 uiTotalLevel);
	protected:
		VSDLodTerrainNode * GetParentTerrainNode()const;
	};
	DECLARE_Ptr(VSDLodTerrainGeometry);
	VSTYPE_MARCO(VSDLodTerrainGeometry);
};
