#pragma once


#include "math/surface.h"


namespace zq{


class MATH_API ControlSurface : public Surface
{
public:
	ControlSurface();
	virtual ~ControlSurface() = 0;

    inline Vector3 GetControlPoint(uint32 U, uint32 V)const
    {
        VSMAC_ASSERT(U <= m_uiRow && V <= m_uiColumn);
        return m_pControlPoint[U * m_uiColumn + V];
    }

    inline bool ChangeControlPoint(uint32 U, uint32 V, const Vector3 &Value)
    {
        VSMAC_ASSERT(U <= m_uiRow && V <= m_uiColumn);
        m_pControlPoint[U * m_uiColumn + V] = Value;
        return 1;
    }

    inline const Vector3* GetControlPoint()const
    {
        return m_pControlPoint;
    }

    inline uint32 GetControlPointNum()const
    {
        return m_uiControlPointNum;
    }

    inline uint32 GetRow()const
    {
        return m_uiRow;
    }

    inline uint32 GetColumn()const
    {
        return m_uiColumn;
    }

	//拷贝
	bool Copy(const ControlSurface& ControlSurface);
	//得到点
	virtual Vector3 GetPoint(float U,float V) = 0;
	//求偏导
	virtual Vector3 GetUPartialDerivative(float U,float V) = 0;
	virtual Vector3 GetVPartialDerivative(float U,float V) = 0;

	float GetULength(float V,float U1, float U2,uint32 uiIterations = 32);
	float GetVLength(float U,float V1, float V2,uint32 uiIterations = 32);
	float GetUTime (float V,float fLength, uint32 uiIterations = 32,float fTolerance = EPSILON_E4);
	float GetVTime (float U,float fLength, uint32 uiIterations = 32,float fTolerance = EPSILON_E4);

protected:
	Vector3 * m_pControlPoint;
	uint32 m_uiControlPointNum;
	uint32 m_uiRow;
	uint32 m_uiColumn;
};


}