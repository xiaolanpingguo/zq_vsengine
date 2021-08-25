#pragma once


#include "graphic/render/scenemanager/spatial.h"


namespace zq{


class VSSort;
class VSGeometry;
class VSStream;
class VSNodeComponent;
class GRAPHIC_API VSNode : public VSSpatial
{
	//PRIORITY
		
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	friend class VSMeshComponent;
	VSNode();
	virtual ~VSNode() = 0;

    inline uint32 GetNodeNum()const
    {
        return m_pChild.GetNum();
    }
    inline VSArray<VSSpatialPtr>* GetChildList()
    {
        return &m_pChild;
    }

	virtual uint32 AddChild(VSSpatial * pChild);
	virtual uint32 DeleteChild(VSSpatial *pChild);
	virtual bool DeleteChild(uint32 i);
	VSSpatial * GetChild(uint32 i)const;
	virtual void		DeleteAllChild();

protected:
		
	virtual void UpdateWorldBound(double dAppTime);//更新世界边界体 

	virtual void UpdateNodeAll(double dAppTime);
	virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
protected:
	VSArray<VSSpatialPtr>m_pChild;
		
		
public:
	virtual void SetIsVisibleUpdate(bool bIsVisibleUpdate);
	virtual void CreateLocalAABB();
};

DECLARE_Ptr(VSNode);
VSTYPE_MARCO(VSNode);
}
