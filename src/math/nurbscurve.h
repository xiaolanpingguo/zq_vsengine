#pragma once


#include "math/b_splinecurve.h"


namespace zq{


class MATH_API NURBSCurve : public BSplineCurve
{
public:
	NURBSCurve();
	virtual ~NURBSCurve();

    inline bool ChangeWeight(uint32 i, float Weight)
    {
        VSMAC_ASSERT(m_pWeight);
        VSMAC_ASSERT(i < m_uiControlPointNum);
        m_pWeight[i] = Weight;
        return 1;
    }

    inline const float* GetWeight()const
    {
        return m_pWeight;
    }

    inline float GetWeight(uint32 i)const
    {
        VSMAC_ASSERT(m_pWeight);
        VSMAC_ASSERT(i < m_uiControlPointNum);
        return m_pWeight[i];
    }


	//����Ȩ��
	bool Set(const float * pWeight,uint32 uiControlPointNum);
	//����
	bool Copy(const NURBSCurve &NURBSCurve3);

	//ȡ�õ���
	virtual Vector3	GetFirstDerivative(float t);
	virtual Vector3	GetSecondDerivative(float t);
	virtual Vector3	GetThirdDerivative(float t);
	//virtual float		GetLength(float t1 ,float t2);
	//�õ���
	virtual Vector3	GetPoint(float t);
protected:
	bool Get(uint32 Order,float t ,Vector3 *D0,Vector3 *D1,Vector3 *D2,Vector3 *D3);
private:
	float *m_pWeight;

};

}