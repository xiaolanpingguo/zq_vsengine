#pragma once
#include "graphic/node/geometry.h"
namespace zq
{

	class VSGPULodTerrainNode;
	class GRAPHIC_API VSGPULodTerrainGeometry : public VSGeometry
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSGPULodTerrainGeometry();
		virtual ~VSGPULodTerrainGeometry();
		bool CreateMesh(uint32 uiTileNumX, uint32 uiTileNumZ);
	protected:
		VSGPULodTerrainNode * GetParentTerrainNode()const;
	};
	DECLARE_Ptr(VSGPULodTerrainGeometry);
	VSTYPE_MARCO(VSGPULodTerrainGeometry);
};
