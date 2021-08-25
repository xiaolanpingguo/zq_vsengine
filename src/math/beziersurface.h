#pragma once


#include "math/controlsurface.h"


namespace zq{


class MATH_API BezierSurface : ControlSurface
{
public:
	BezierSurface();
	virtual ~BezierSurface();
	//设置控制点
	bool Set(const Vector3 * pControlPoint,uint32 uiRow,uint32 uiColumn);
	//拷贝
	bool Copy(const BezierSurface & BezierSurface3);
	//求点
	virtual Vector3 GetPoint(float U,float V);
	//求偏导
	virtual Vector3 GetUPartialDerivative(float U,float V);
	virtual Vector3 GetVPartialDerivative(float U,float V);
private:
	
    inline float GetUC(int32 i, int32 j)const
    {
        return m_UC[i * m_uiRow + j];
    }

    inline float SetUC(int32 i, int32 j, float Value)
    {
        return m_UC[i * m_uiRow + j] = Value;
    }

    inline float GetVC(int32 i, int32 j)const
    {
        return m_VC[i * m_uiColumn + j];
    }

    inline float SetVC(int32 i, int32 j, float Value)
    {
        return m_VC[i * m_uiColumn + j] = Value;
    }

    inline void SetUPD(int32 i, int32 j, const Vector3 & Value)
    {
        m_UPDValue[i * m_uiColumn + j] = Value;
    }

    inline Vector3 BezierSurface::GetUPD(int32 i, int32 j)
    {
        return m_UPDValue[i * m_uiColumn + j];
    }

    inline void SetVPD(int32 i, int32 j, const Vector3 & Value)
    {
        m_VPDValue[i * (m_uiColumn - 1) + j] = Value;
    }

    inline Vector3 GetVPD(int32 i, int32 j)
    {
        return m_VPDValue[i * (m_uiColumn - 1) + j];
    }

private:
	float * m_UC;
	float * m_VC;
	Vector3 * m_UPDValue;
	Vector3 * m_VPDValue;
};

}
