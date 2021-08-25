#pragma once


#include "graphic/node/geometry.h"


/*
	顶点排列顺序
j	4 9 14 19 24
	3 8 13 18 23
	2 7 12 17 22
	1 6 11 16 21
	0 5 10 15 20 i
*/
namespace zq{


class VSStream;
class GRAPHIC_API VSCLodTerrainGeometry : public VSGeometry
{
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL_NO_CLASS_FACTORY;
public:
	enum	// TerrainGeometry Type
	{
		TGT_ROAM,
		TGT_QUAD,
		TGT_MAX
	};
	enum	// Neightbor Type
	{
		NT_LEFT,
		NT_RIGHT,
		NT_TOP,
		NT_BOTTOM,
		NT_MAX
	};
	VSCLodTerrainGeometry();
	virtual ~VSCLodTerrainGeometry() = 0;
public:

	virtual bool CreateMeshData(uint32 uiIndexXInTerrain, uint32 uiIndexZInTerrain,
								uint32 uiTessellationLevel);
	virtual uint32 GetTerrainGeometryType() = 0;
	virtual bool AddNeighbor(VSCLodTerrainGeometry * pTerrainGemetry,uint32 uiNeighbor);
	virtual void LinkNeighbor() = 0;
	//每个节点自己不允许变换，以免相邻的邻居和当前节点LOD出现问题
	virtual void SetLocalScale(const Vector3& Scale){};
	virtual void SetLocalTranslate(const Vector3& Translate){};
	virtual void SetLocalRotate(const Matrix3X3 & Rotate){};
	virtual void SetLocalTransform(const VSTransform & LocalTransform){};
	virtual void SetLocalMat(const Matrix3X3W VSMat){};

	virtual void ComputeVariance(uint32 uiCurLevel = 0) = 0;
	virtual void ClearInfo() = 0;
		
	virtual void UpdateView(VSCuller & Culler,double dAppTime);
protected:
	VSCLodTerrainGeometry * m_pNeighbor[NT_MAX];
	uint32 m_uiIndexXInTerrain;
	uint32 m_uiIndexZInTerrain;
	virtual void Tessellate(const Vector3 & CameraPos) = 0;
};
DECLARE_Ptr(VSCLodTerrainGeometry);
VSTYPE_MARCO(VSCLodTerrainGeometry);
}
