#pragma once


#include "graphic/core/object.h"
#include "graphic/core/transform.h"
#include "graphic/controller/controller.h"
#include "math/aabb.h"
#include "graphic/core/resource.h"


namespace zq{


class VSStream;
class VSCuller;
DECLARE_Ptr(VSMaterialInstance)
DECLARE_Ptr(VSLight);
DECLARE_Ptr(VSCamera);
DECLARE_Proxy(VSMaterial);
class GRAPHIC_API VSSpatial :public VSObject
{
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL_NO_CLASS_FACTORY;
protected:
    friend class VSNode;
    friend class VSSwitchNode;
    friend class VSScene;
    friend class VSGeometry;
    friend class VSMeshComponent;
    friend class VSSkeleton;
    friend class VSNodeComponent;
    friend class VSSkeletonMeshNode;
public:

    enum //Cull Mode
    {
        CM_DYNAMIC,
        CM_ALAWAYS,
        CM_NEVER
    };

	virtual ~VSSpatial() = 0;


    inline const AABB& GetWorldAABB()const
    {
        return m_WorldBV;
    }

    inline void SetParent(VSSpatial * pParent)
    {
        m_pParent = pParent;
        m_bIsChanged = true;
    }

    inline VSSpatial* GetParent()const
    {
        return m_pParent;
    }

    inline const Vector3& GetLocalScale()const
    {
        return m_Local.GetScale();
    }

    inline const Vector3& GetLocalTranslate()const
    {
        return m_Local.GetTranslate();
    }

    inline const Matrix3X3& GetLocalRotate()const
    {
        return m_Local.GetRotate();
    }

    inline void GetLocalDir(Vector3& Dir, Vector3& Up, Vector3& Right)const
    {
        m_Local.GetDir(Dir, Up, Right);
    }

    inline const VSTransform& GetLocalTransform()
    {
        m_Local.GetCombine();
        return m_Local;
    }

    inline uint32 GetControllerNum()const 
    {
        return m_ControllerArray.GetNum(); 
    }

    inline VSArray<VSPointer<VSController>>* GetControllerList()
    {
        return &m_ControllerArray;
    }

    inline void SetDynamic(bool bIsDynamic);
    inline bool IsDynamic()const { return !m_bIsStatic; }

	virtual void ComputeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);
	virtual void UpdateView(VSCuller & Culler,double dAppTime);
	virtual void CreateLocalAABB(){}
	virtual void SetIsVisibleUpdate(bool bIsVisibleUpdate);
	virtual void UpdateTransform(double dAppTime);//更新变换信息 
	virtual void UpdateAll(double dAppTime);

	virtual const Vector3 &GetWorldScale();
	virtual const Vector3 & GetWorldTranslate();
	virtual const Matrix3X3 & GetWorldRotate();
	virtual void GetWorldDir(Vector3 &Dir, Vector3 &Up, Vector3 & Right);
	virtual const VSTransform & GetWorldTransform();
	virtual void SetWorldScale(const Vector3 & fScale);
	virtual void SetWorldTranslate(const Vector3& Translate);
	virtual void SetWorldRotate(const Matrix3X3 & Rotate);
	virtual void SetWorldTransform(const VSTransform & LocalTransform);
	virtual void SetWorldMat(const Matrix3X3W VSMat);

	virtual void SetLocalScale(const Vector3 & fScale);
	virtual void SetLocalTranslate(const Vector3& Translate);
	virtual void SetLocalRotate(const Matrix3X3 & Rotate);
	virtual void SetLocalTransform(const VSTransform & LocalTransform);
	virtual void SetLocalMat(const Matrix3X3W VSMat);


	virtual void UpdateController(double dAppTime);
	bool AddController(VSController * pController);
	VSController * DeleteController(uint32 ID);
	uint32 DeleteController(VSController *pController);
	VSController *GetController(uint32 ID)const;
	void DeleteAllController();
	void ClearAllTime();

protected:
    VSSpatial();

    virtual void UpdateWorldBound(double dAppTime) = 0;//更新边界体 
    virtual void UpdateNodeAll(double dAppTime) = 0;
    virtual void ComputeNodeVisibleSet(VSCuller & Culler, bool bNoCull, double dAppTime) = 0;

public:
    bool m_bIsVisibleUpdate;
    bool m_bInheritScale;
    bool m_bInheritRotate;
    bool m_bInheritTranslate;
    bool m_bEnable;

protected:
    uint32 m_uiCullMode;
	bool m_bIsStatic;
	bool m_bIsChanged;
    AABB m_WorldBV;
    VSArray<VSControllerPtr>	m_ControllerArray;
    VSTransform m_World;
    VSTransform m_Local;
    VSSpatial* m_pParent;
    VSArray<VSLight *> m_pAllLight;
    VSArray<VSCamera *> m_pAllCamera;
};


DECLARE_Ptr(VSSpatial);
VSTYPE_MARCO(VSSpatial);


}