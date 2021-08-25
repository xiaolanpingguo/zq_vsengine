#pragma once


#include "graphic/node/node.h"


namespace zq{


class VSRenderer;
class VSStream;
class VSQuadNode : public VSNode
{
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	VSQuadNode();
	~VSQuadNode();
	virtual void ComputeNodeVisibleSet(VSCuller & Culler, bool bNoCull, double dAppTime);
	bool RecursiveBuild(const VSArray<VSSpatial *> &pObjectArray);
	virtual uint32 AddChild(VSSpatial * pChild);
	virtual uint32 DeleteChild(VSSpatial *pChild);
	virtual bool DeleteChild(uint32 i);
	virtual void		DeleteAllChild();
	VSNodeComponent *GetNeedDrawNode(uint32 uiIndex)const;
	uint32 GetNeedDrawNodeNum()const;
protected:
	void DeleteNeedDrawNode(VSNodeComponent *pNeedDrawNode);
	void AddNeedDrawNode(VSNodeComponent *pNeedDrawNode);
	VSArray<VSNodeComponent *> m_pNeedDrawNode;

};
DECLARE_Ptr(VSQuadNode);
VSTYPE_MARCO(VSQuadNode);
	
class GRAPHIC_API VSScene : public VSObject
{
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	friend class VSSceneManager;
	void Update(double dAppTime);
	void CollectUpdateInfo();
	VSScene();
	~VSScene();
		

    inline VSLight* GetAllLight(uint32 i)const
    {
        VSMAC_ASSERT(i < m_pAllLight.GetNum());
        return m_pAllLight[i];
    }

    inline uint32 GetAllLightNum()const
    {
        return m_pAllLight.GetNum();
    }

    inline bool IsBuild()
    {
        return m_bIsBuild;
    }

	virtual void AddObject(VSNode * pObject);
	virtual void DeleteObject(VSNode *pObject);
	virtual void DeleteAllObject();	

	virtual void ComputeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
	bool Build();

	void GetStreamResourceWithPos(const Vector3& CameraPos, const Vector3W& ProjectInfo);
protected:
	VSArray<VSLight *> m_pAllLight;
	VSArray<VSCamera *> m_pAllCamera;

	VSArray<VSNode *> m_pDynamic;
	VSQuadNodePtr m_pStaticRoot;
	VSArray<VSNode *> m_ObjectNodes;
	bool m_bIsBuild;
};

DECLARE_Ptr(VSScene);
VSTYPE_MARCO(VSScene);


}