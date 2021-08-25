#pragma once


#include "math/plane.h"
#include "graphic/core/object.h"
#include "math/sphere.h"
#include "datastruct/VSStack.h"


namespace zq{


class VSSpatial;
class VSMaterialInstance;
class VSMaterial;
class VSMeshNode;
class VSGeometry;
class VSCamera;
class VSLight;
class VSRenderContext
{
public:
	VSRenderContext()
	{
		m_pGeometry = NULL;
		m_pMaterialInstance = NULL;
		m_uiPassId = 0;
		m_pMaterial = NULL;
		m_pMeshNode = NULL;
		m_uiInstanceID = -1;
		bOcclusion = false;
	}
	~VSRenderContext()
	{

	}
	static VSRenderContext ms_RenderContextNULL;
	VSGeometry * m_pGeometry;
	VSMaterialInstance * m_pMaterialInstance;
	VSMaterial * m_pMaterial;
	uint32 m_uiPassId;
	VSMeshNode * m_pMeshNode;
	VSArray<VSLight *> m_pIndirectLight;
	VSArray<VSLight *> m_pDirectLight;
	int32 m_uiInstanceID;
	bool bOcclusion;
	inline bool IsInstanceData()const
	{
		return m_pMeshNode == NULL && m_pMaterialInstance == NULL;
	}

	bool IsCanDraw()const;	
};


class GRAPHIC_API VSCuller : public MemObject
{
public:
	enum //CULLER TYPE
	{
		CUT_MAIN,
		CUT_SHADOW,
		CUT_MAX
	};

	enum
	{
		VS_MAX_PLANE_NUM = 32
	};

	enum //VisibleSet Type
	{
		VST_BASE,
		VST_ALPHABLEND,	
		VST_COMBINE,
		VST_MAX
	};

	enum	//Render Group
	{
		RG_BACK,
		RG_NORMAL,
		RG_FRONT,		
		RG_MAX
	};

	enum	// VisibleSet Flag
	{
		VSF_NONE,
		VSF_ALL,
		VSF_PARTIAL,
		VSF_MAX
			
	};

    VSCuller();
    virtual ~VSCuller();


    inline uint32 GetPlaneNum()const
    {
        return m_uiPlaneNum;

    }

    inline void SetPlaneState(uint32 uiPlaneState)
    {
        m_uiPlaneState = uiPlaneState;
    }

    inline uint32 GetPlaneState()const
    {

        return m_uiPlaneState;
    }

    inline uint32 GetAllVisibleNum()const
    {
        uint32 uiNum = 0;
        for (uint32 j = 0; j < RG_MAX; j++)
        {
            for (uint32 i = 0; i < VST_MAX; i++)
            {
                uiNum += m_VisibleSet[j][i].GetNum();
            }
        }

        return uiNum;
    }

    inline uint32 GetVisibleNum(unsigned int uiVisibleSetType = VST_BASE, unsigned int uiRenderGroup = RG_NORMAL)const
    {
        if (uiVisibleSetType >= VST_MAX || uiRenderGroup >= RG_MAX)
        {
            return 0;
        }

        return m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum();
    }

    inline uint32 GetRenderGroupVisibleNum(uint32 uiRenderGroup)const
    {
        uint32 uiNum = 0;
        for (uint32 i = 0; i < VST_MAX; i++)
        {
            uiNum += m_VisibleSet[uiRenderGroup][i].GetNum();
        }

        return uiNum;
    }

    inline void ClearVisibleSet()
    {
        for (uint32 j = 0; j < RG_MAX; j++)
        {
            for (uint32 i = 0; i < VST_MAX; i++)
            {
                m_VisibleSet[j][i].Clear();
            }
        }
    }

    inline void ClearPlaneState()
    {
        m_uiPlaneState = 0xffffffff;
    }

    inline void ClearAllPlane()
    {
        m_uiPlaneNum = 0;
    }

    inline void SetCamera(VSCamera &Camera)
    {
        m_pCamera = &Camera;
    }

    inline VSRenderContext& GetVisibleSpatial(uint32 i, uint32 uiVisibleSetType = VST_BASE, uint32 uiRenderGroup = RG_NORMAL)const
    {
        if (uiVisibleSetType >= VST_MAX || uiRenderGroup > RG_MAX || i >= m_VisibleSet[uiRenderGroup][uiVisibleSetType].GetNum())
        {
            VSMAC_ASSERT(0);
            return VSRenderContext::ms_RenderContextNULL;
        }

        return m_VisibleSet[uiRenderGroup][uiVisibleSetType][i];
    }

    inline uint32 GetLightNum()const
    {
        return m_LightSet.GetNum();
    }

    inline VSLight* GetLight(uint32 i)const
    {
        if (i >= m_LightSet.GetNum())
        {
            return NULL;
        }

        return m_LightSet[i];
    }

    inline void ClearLight()
    {
        m_LightSet.Clear();
    }

    inline VSCamera* GetCamera()const
    {
        return m_pCamera;
    }

    inline void SetUseMaterialIndex(int32 iUseMaterialIndex)
    {
        m_iUseMaterialIndex = iUseMaterialIndex;
    }

    inline int32 GetUseMaterialIndex()const
    {
        return m_iUseMaterialIndex;
    }

    inline  void SetBeignDynamicInstanceNum(uint32 uiBeignDynamicInstanceNum)
    {
        m_uiBeignDynamicInstanceNum = uiBeignDynamicInstanceNum;
    }

    inline void ClearAll()
    {
        ClearPlaneState();
        ClearAllPlane();
        ClearVisibleSet();
        ClearLight();
    }

	virtual uint32 IsVisible(const AABB &BV,bool bClearState = false);
	virtual uint32 IsVisible(const Sphere &S,bool bClearState = false);
	virtual uint32 IsVisible(const Vector3 & Point,bool bClearState = false);
	virtual bool ForceNoCull(const VSSpatial * pSpatial);
	virtual bool CullConditionNode(const VSMeshNode * pMeshNode);
    virtual uint32 GetCullerType()const
    {
        return CUT_MAIN;
    }

    bool PushPlane(const Plane & Plane);
    bool PushCameraPlane(VSCamera &Camera);
    bool PopPlane(Plane &Plane);

    bool InsertObject(VSRenderContext &VisibleContext, uint32 uiVisibleSetType = VST_BASE, uint32 uiRenderGroup = RG_NORMAL);
    void InsertLight(VSLight * pLight);
    bool HasLight(VSLight * pLight);

	class RenderPriority
	{
	public:
		RenderPriority(){}
		~RenderPriority(){}
		bool operator()(VSRenderContext & p1,VSRenderContext & p2);
	};

	class AlphaPriority
	{
	public:
		AlphaPriority(VSCamera * pCamera){m_pCamera = pCamera;}
		~AlphaPriority(){}
		bool operator()(VSRenderContext & p1,VSRenderContext & p2);
	protected:
		VSCamera *m_pCamera;
	};


	friend class VSShadowCuller;
	friend class VSVolumeShadowMapCuller;
	virtual void Sort();
	virtual void CollectDynamicInstance();
	virtual bool CullGeometry(VSGeometry * pGeometry);

	void GetAllVisibleAABB(uint32 uiVisibleSetType, uint32 uiRenderGroup, VSArray<AABB> & AABBArray);

	void GetAndSortLight();

	void GetSceneContent(VSCamera & pCamera, VSArray<class VSScene *> & pSceneArray, double dAppTime,bool bSort = true);

protected:
	void GetSceneGeometryContent(VSCamera & pCamera, VSArray<class VSScene *> & pSceneArray, double dAppTime,bool bSort);
	void GetSceneLightContent(VSCamera & pCamera, VSArray<class VSScene *> & pSceneArray, double dAppTime);
	virtual void CollectDynamicInstance(VSArray<VSRenderContext> & inContext);
	uint32 m_uiPlaneNum;
	Plane	m_Plane[VS_MAX_PLANE_NUM];
	uint32 m_uiPlaneState;
	VSArray<VSRenderContext> m_VisibleSet[RG_MAX][VST_MAX];
	VSArray<VSLight *> m_LightSet;
	VSCamera *m_pCamera;
	int32 m_iUseMaterialIndex;	
	uint32 m_uiBeignDynamicInstanceNum;
};

class VSLocalLight;
class GRAPHIC_API VSShadowCuller : public VSCuller
{
public:
	// use light Camera
	VSShadowCuller();
	virtual ~VSShadowCuller();
	virtual uint32 GetCullerType()const
	{
		return CUT_SHADOW;
	}
	virtual bool CullConditionNode(const VSMeshNode * pMeshNode);
	virtual void Sort();
	class RenderPriority
	{
	public:
		RenderPriority(){}
		~RenderPriority(){}
		bool operator()(VSRenderContext & p1, VSRenderContext & p2);
	};
	bool TestObjectWithCameraIntersectOnLightDirPlane(const VSSpatial * pSpatial);
	void GetSceneContent(VSCamera & pCamera, VSArray<class VSScene *> & pSceneArray, VSLocalLight * pLocalLight,double dAppTime,bool bCollectDynamicInstance = true);
protected:
	VSLocalLight * m_pLocalLight;
};


class GRAPHIC_API VSVolumeShadowMapCuller : public VSShadowCuller
{
public://use Culler Camera to Cull
	VSVolumeShadowMapCuller();
	virtual ~VSVolumeShadowMapCuller();
	virtual bool ForceNoCull(const VSSpatial * pSpatial);	
	virtual bool CullGeometry(VSGeometry * pGeometry);
};


class GRAPHIC_API VSDirShadowMapCuller : public VSShadowCuller
{
public:

	VSDirShadowMapCuller();
	virtual ~VSDirShadowMapCuller();
	virtual bool ForceNoCull(const VSSpatial * pSpatial);
	
};


class VSOcclusionQueryType
{
public:
	VSOcclusionQueryType()
	{
		pQuery = NULL;
	}
	~VSOcclusionQueryType()
	{
		pQuery = NULL;
	}
	bool GetQueryData(void * pData, uint32 uiDataSize);
	class VSQuery * pQuery;
	friend class VSOcclusionQueryPool;
private:
	int32 ID;
};


class VSGeometryOcclusionQueryData
{
public:
	VSGeometryOcclusionQueryData()
	{
		pGeometry = NULL;
		uiPixedNum = MATH_MAX_UINT32;
		bVisible = true;
		pCuller = NULL;
	}
	VSGeometryOcclusionQueryData(VSGeometry * pInGeometry,VSCuller * pInCuller)
	{
		pGeometry = pInGeometry;
		uiPixedNum = MATH_MAX_UINT32;
		bVisible = true;
		pCuller = pInCuller;
	}
	void GetOcclusionQuery(const VSOcclusionQueryType& InOcclusionQueryType)
	{
		OcclusionQueryType = InOcclusionQueryType;
	}

	bool operator > (const VSGeometryOcclusionQueryData& e)const
	{
		return pGeometry < e.pGeometry;
	}

	bool operator < (const VSGeometryOcclusionQueryData& e)const
	{
		return pGeometry > e.pGeometry;
	}

	bool operator == (const VSGeometryOcclusionQueryData& e)const
	{
		return pGeometry == e.pGeometry;
	}

	void ComputeVisible();
	VSGeometry * pGeometry;
	VSCuller * pCuller;
	VSOcclusionQueryType OcclusionQueryType;
	uint32 uiPixedNum;
	bool bVisible;	
};


inline uint32 GetTypeHash(const VSGeometryOcclusionQueryData & Element)
{
	return GetTypeHash(Element.pGeometry);
}


class VSOcclusionQueryPool
{
public:
	VSOcclusionQueryPool();
	~VSOcclusionQueryPool();
	VSOcclusionQueryType GetOcclusionQuery();
	void ReleaseAllOcclusionQuery();
	void DeleteAllReleaseOcclusionQuery();
private:
	VSArray<VSOcclusionQueryType> m_ElementArray;
	VSStack<unsigned int> m_FreeElement;
};


class GRAPHIC_API VSCullerManager
{
public: 
	VSCullerManager();
	~VSCullerManager();
	static VSCullerManager & GetCullerManager()
	{
		static VSCullerManager s_CullerManager;
		return s_CullerManager;
	}
	bool ProcessOcclusionCull(VSCuller * pCuller);

	void FillGeometryOcclusionQueryData();
	friend class VSOcclusionQuerySceneRender;
	void Exchange();
	void RegisterCuller(VSCuller * pCuller);
	void UnRegisterCuller(VSCuller * pCuller);
	bool IsRegister(VSCuller * pCuller);
	void ClearAllOcclusionQuery();
protected:
	VSArray<VSGeometryOcclusionQueryData> & GetGroupGeometryOcclusionQueryData(VSCuller * pCuller, uint32 uiGroup);
		
	
	VSArray<VSCuller *> m_CullerArray;
	VSArray<VSArrayOrder<VSGeometry *>> m_NoVisibleGeometry[VSCuller::RG_MAX];
	typedef VSArray<VSArray<VSGeometryOcclusionQueryData>> GeometryOcclusionQueryDataType;
	GeometryOcclusionQueryDataType m_GeometryOcclusionQueryBuffer[VSCuller::RG_MAX][3];
	GeometryOcclusionQueryDataType *m_GOQBufferPointer[VSCuller::RG_MAX][3];

	VSOcclusionQueryPool m_OcclusionQueryPool;

	void GetQueryData();
	VSArray<VSCuller * > m_PendingRegister;
	VSArray<VSCuller * > m_PendingDelete; 
	void OnRegisterCuller(VSCuller * pCuller);
	void OnUnRegisterCuller(VSCuller * pCuller);
};


}