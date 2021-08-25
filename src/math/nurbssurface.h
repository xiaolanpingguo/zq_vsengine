#pragma once


#include "math/b_splinesurface.h"


namespace zq{


class MATH_API NURBSSurface:public BSplineSurface
{
public:
	NURBSSurface();
	~NURBSSurface();

    inline bool ChangeWeight(uint32 U, uint32 V, float Weight)
    {
        VSMAC_ASSERT(U <= m_uiRow && V <= m_uiColumn);
        m_pWeight[U * m_uiColumn + V] = Weight;
        return 1;
    }

    inline const float* GetWeight()const
    {
        return m_pWeight;
    }

    inline float GetWeight(uint32 U, uint32 V)const
    {
        VSMAC_ASSERT(U <= m_uiRow && V <= m_uiColumn);
        return m_pWeight[U * m_uiColumn + V];
    }


	//设置权重
	bool Set(const float * pWeight,uint32 uiRow,uint32 uiColumn);
	//拷贝
	bool Copy(const NURBSSurface & NURBSSurface3);
	//取得点
	virtual Vector3 GetPoint(float U,float V);
	//得到偏导
	virtual Vector3 GetUPartialDerivative(float U,float V);
	virtual Vector3 GetVPartialDerivative(float U,float V);

protected:
	bool Get(uint32 Order,float U,float V ,Vector3 *D0,Vector3 *DU,Vector3 *DV);
private:
	float *m_pWeight;
};

}