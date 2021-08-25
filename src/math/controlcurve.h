#pragma once


#include "math/curve.h"


namespace zq{


class MATH_API ControlCurve:public Curve
{
public:
	enum {B_SPLINE,NURBS,BEZIER};
	ControlCurve();
	virtual ~ControlCurve() = 0;
	virtual uint32 GetCurveType(){return CT_CONTROL;}

    inline bool ChangeControlPoint(uint32 i, const Vector3 & ControlPoint)
    {
        VSMAC_ASSERT(m_pControlPoint);
        VSMAC_ASSERT(i < m_uiControlPointNum);
        m_pControlPoint[i] = ControlPoint;
        return 1;
    }

    inline const Vector3* GetpControlPoint()const
    {

        return m_pControlPoint;
    }

    inline Vector3 GetControlPoint(uint32 i)const
    {
        VSMAC_ASSERT(m_pControlPoint);
        VSMAC_ASSERT(i < m_uiControlPointNum);
        return m_pControlPoint[i];
    }

    inline uint32 GetControlPointNum()const
    {
        return m_uiControlPointNum;
    }

    inline uint32 GetType()const
    {
        return m_CurveType;
    }

	//����
	bool Copy(const ControlCurve & ControlCurve3);
	//���FRENET3������
	virtual void	GetFrenetFrameBNT(float t,Vector3 & B,Vector3 & N,Vector3 &T);
	//����
	virtual Vector3	GetFirstDerivative(float t)= 0 ;
	virtual Vector3	GetSecondDerivative(float t)= 0;
	virtual Vector3	GetThirdDerivative(float t)= 0;
	//�󳤶�
	virtual float		GetLength(float t1 ,float t2,uint32 uiIterations = 32);
	//�õ���
	virtual Vector3	GetPoint(float t) = 0;
	//�õ��ܳ���
	virtual float		GetTotalLength(uint32 uiIterations);
	//���ݳ�����ʱ��
	virtual float GetTime (float fLength, uint32 uiIterations = 32,
						float fTolerance = EPSILON_E4);
	//�õ�����
	virtual float GetCurvature(float fTime);
	//�õ�ת��
	virtual float GetTorsion(float fTime);

protected:
	uint32 m_CurveType;
	Vector3 * m_pControlPoint;
	uint32 m_uiControlPointNum;
};

}