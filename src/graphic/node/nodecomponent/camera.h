#pragma once


#include "graphic/node/nodecomponent/nodecomponent.h"
#include "math/vector3.h"
#include "math/matrix3X3.h"
#include "math/matrix3X3w.h"
#include "math/plane.h"


namespace zq{


class VSRenderer;
struct VSViewPort
{
	float       XMin;
	float       YMin;            
	float       XMax;
	float       YMax;
	float		ZMin;
	float		ZMax;
	bool		bChangeAspect;
	void Reset()
	{
		XMin = 0.0f;
		YMin = 0.0f;
		XMax = 1.0f;
		YMax = 1.0f;
		ZMin = 0.0f;
		ZMax = 1.0f;
		bChangeAspect = false;
	}
    VSViewPort()
	{
		Reset();
	}
};


class VSStream;
class GRAPHIC_API VSCamera : public VSNodeComponent
{
	//PRIORITY
	
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	friend class VSViewFamily;
	VSCamera();
	virtual ~VSCamera();
	enum //Camera Plane
	{	
		CP_RIGHT,
		CP_LEFT,
		CP_TOP,
		CP_BOTTOM,
		CP_FAR,
		CP_NEAR,
		CP_MAX = 6
	};


    inline const Matrix3X3W& VSCamera::GetViewMatrix()const
    {
        return m_ViewMat;
    }

    // inline const Vector3& GetWorldPos()const
    // {
    // 	return m_WorldPos;
    // }
    // inline void GetLocalDirection(Vector3 &LocalDir,Vector3 &LocalUp,Vector3 & LocalRight)const
    // {
    // 	LocalDir = m_LocalDir;
    // 	LocalUp = m_LocalUp;
    // 	LocalRight = m_LocalRight;
    // 
    // }
    // inline void GetWorldDirection(Vector3 &WorldDir,Vector3 &WorldUp,Vector3 & WorldRight)const
    // {
    // 	WorldDir = m_WorldDir;
    // 	WorldUp = m_WorldUp;
    // 	WorldRight = m_WorldRight;
    // 
    // }

    inline const Matrix3X3W& GetProjMatrix()const
    {
        return m_ProjMat;
    }

    inline VSViewPort* GetViewPort(uint32 i)const
    {
        return &m_ViewPort[i];
    }

    inline bool AddViewPort(const VSViewPort &ViewPort)//视口		
    {
        if (ViewPort.XMin < 0 || ViewPort.XMax > 1.0f || ViewPort.XMin > ViewPort.XMax ||
            ViewPort.YMin < 0 || ViewPort.YMax > 1.0f || ViewPort.YMin > ViewPort.YMax ||
            ViewPort.ZMax > 1.0f || ViewPort.ZMin < 0.0f || ViewPort.ZMin > ViewPort.ZMax)
        {
            return false;
        }

        m_ViewPort.AddElement(ViewPort);
        return true;
    }

    // inline const Vector3 &VSCamera::GetLocalDir()const
    // {
    // 	return m_LocalDir;
    // }

    // inline const Vector3 &VSCamera::GetLocalUp()const
    // {
    // 	return m_LocalUp;
    // }

    // inline const Vector3 &VSCamera::GetLocalRight()const
    // {
    // 	return m_LocalRight;
    // }

    // inline const Vector3 &VSCamera::GetWorldDir()const
    // {
    // 	return m_WorldDir;
    // }

    // inline const Vector3 &VSCamera::GetWorldUp()const
    // {
    // 	return m_WorldUp;
    // }

    // inline const Vector3 &VSCamera::GetWorldRight()const
    // {
    // 	return m_WorldRight;
    // }


	void CreateFromEuler(const Vector3 &Pos,float RotX,float RotY , float RotZ);
	void CreateFromLookDir(const Vector3 &Pos,
							const Vector3 &vcDir,
							const Vector3 &vcUp = Vector3(0,1,0));
	void CreateFromLookAt(const Vector3 &vcPos,									
							const Vector3 &vcLookAt,							
							const Vector3 &vcUp = Vector3(0,1,0));	
	void CreateFromLookDirWorld(const Vector3 &Pos,
		const Vector3 &vcDir,
		const Vector3 &vcUp = Vector3(0, 1, 0));
	void CreateFromLookAtWorld(const Vector3 &vcPos,
		const Vector3 &vcLookAt,
		const Vector3 &vcUp = Vector3(0, 1, 0));
	bool SetPerspectiveFov(float fFov ,		//X方向张角
						float Aspect,	//宽高比
						float fZN ,		//近剪裁面
						float fZF);		//远剪裁面
	bool SetAspect(float Aspect);
	
	bool SetOrthogonal(float fW ,				//宽
						float fH,					//高
						float fZN ,				//近剪裁面
						float fZF)	;				//远剪裁面

	inline uint32 GetViewPortNum()const
	{
		return m_ViewPort.GetNum();
	}
	void GetFrustumPoint(Vector3 Point[8]);
	AABB GetFrustumAABB();
	void GetPlane(Plane Plane[VSCamera::CP_MAX])const;
	inline float GetZFar()const
	{
		return m_ZFar;
	}
	inline float GetZNear()const
	{
		return m_ZNear;
	}
	inline float GetAspect()const
	{
		return m_Aspect;
	}
	inline float GetFov()const
	{
		return m_Fov;
	}
	inline VSViewFamily * GetViewFamily(uint32 uiID)
	{
		if (uiID < m_ViewFamilyArray.GetNum())
		{
			return m_ViewFamilyArray[uiID];
		}
		return NULL;
	}
	inline uint32 GetViewFamilyNum()
	{
		return m_ViewFamilyArray.GetNum();
	}
	inline void ClearProject()
	{
		m_ProjMat.Identity();
	}
	void AddCustomCullPlane(const Plane& P);
	void ClearCustomCullPlane();
	VSArray<Plane> m_CustomCullPlane;
	virtual void UpdateCameraState(double dAppTime);
	float GetProjectScreenSize(const AABB & WorldAABB);
public:
	float		m_RotX;							//在局部坐标系下的EULER角(旋转角)
	float		m_RotY;
	float		m_RotZ;

	bool		m_bEnableOcclusionQuery;
	bool		m_bEnableStreamResource;
	float		m_fStreamBias;
protected :
	friend class VSFreeCameraController;
	friend class VS1stCameraController;
	friend class VSSpatial;
	friend class VSScene;
	friend class VSSceneManager;
	friend class VSViewFamily;
	friend class VSLight;
	VSArray<VSViewPort>		m_ViewPort;
	Matrix3X3W	m_ViewMat;					//相机矩阵
	Matrix3X3W	m_ProjMat;
	

	float		m_Fov;
	float		m_Aspect;
	float		m_ZFar;
	float		m_ZNear;
	VSArrayOrder<VSViewFamily *> m_ViewFamilyArray;
	
	void AddViewFamily(VSViewFamily * pViewFamily);

	
	void DeleteViewFamily(VSViewFamily * pViewFamily);

	
public:
	virtual void UpdateTransform(double dAppTime);//更新变换信息 
public:

	static bool InitialDefaultState();
	static bool TerminalDefaultState();

protected:
	static VSPointer<VSCamera> ms_Default;
	
public:
	friend class VSProjLight;
	static const VSCamera * GetDefault()
	{
		return ms_Default;
	}
};


DECLARE_Ptr(VSCamera);
VSTYPE_MARCO(VSCamera);
};
