#pragma once


#include "math/controlcurve.h"


namespace zq{


class MATH_API BezierCurve3 : public  ControlCurve
{
public:
	BezierCurve3();
	virtual ~BezierCurve3();

	//���ÿ��Ƶ�
	bool Set(const Vector3 * pControlPoint,uint32 uiControlPointNum);
	//����
	bool Copy(const BezierCurve3 & BezierCurve3);
	//��
	virtual Vector3	GetFirstDerivative(float t);
	virtual Vector3	GetSecondDerivative(float t);
	virtual Vector3	GetThirdDerivative(float t);
	//virtual float		GetLength(float t1 ,float t2);
	//���
	virtual Vector3	GetPoint(float t);

private:
    inline float GetC(int32 i, int32 j)const
    {
        return m_C[m_uiControlPointNum * i + j];
    }
    inline float SetC(int32 i, int32 j, float Value)
    {
        return m_C[m_uiControlPointNum * i + j] = Value;
    }

private:


	float * m_C;
	Vector3 * m_FDValue;
	Vector3 * m_SDValue;
	Vector3 * m_TDValue;

};

}