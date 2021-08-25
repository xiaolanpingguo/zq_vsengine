#pragma once


#include "math/controlcurve.h"
/*
	B_Spline曲线，属于控制点类型曲线一种，由控制点，节向量，曲线的度，多项式组成,参数t在0-1之间

*/
namespace zq{


class MATH_API BSplineCurve : public ControlCurve
{
public:
	BSplineCurve();
	virtual ~BSplineCurve();

    inline bool SetKnotVector(uint32 i, float Value)
    {
        VSMAC_ASSERT(m_pKnotVector);

        VSMAC_ASSERT(i < m_uiKnotNum);

        m_pKnotVector[i] = Value;
        return 1;
    }

    inline const float* GetpKnotVector()const
    {
        return m_pKnotVector;
    }

    inline float GetKnotVectorValue(uint32 i)const
    {
        VSMAC_ASSERT(m_pKnotVector);
        VSMAC_ASSERT(i < m_uiKnotNum);
        return m_pKnotVector[i];
    }

    inline void SetBasisValue(int32 i, int32 j, float Value)
    {
        m_pBasis[i * m_uiKnotNum + j] = Value;
    }

    inline float GetBasisValue(int32 i, int32 j)const
    {
        return m_pBasis[i * m_uiKnotNum + j];
    }

    inline void SetBasisFDValue(int32 i, int32 j, float Value)
    {
        m_pBasisFD[i * m_uiKnotNum + j] = Value;
    }

    inline float GetBasisFDValue(int32 i, int32 j)const
    {
        return m_pBasisFD[i * m_uiKnotNum + j];
    }

    inline void SetBasisSDValue(int32 i, int32 j, float Value)
    {
        m_pBasisSD[i * m_uiKnotNum + j] = Value;
    }

    inline float GetBasisSDValue(int32 i, int32 j)const
    {
        return m_pBasisSD[i * m_uiKnotNum + j];
    }

    inline void SetBasisTDValue(int32 i, int32 j, float Value)
    {
        m_pBasisTD[i * m_uiKnotNum + j] = Value;
    }

    inline float GetBasisTDValue(int32 i, int32 j)const
    {
        return m_pBasisTD[i * m_uiKnotNum + j];
    }

    inline uint32 GetDegree()const
    {
        return m_uiDegree;
    }

    inline uint32 GetKnotNum()const
    {
        return m_uiKnotNum;
    }


	//复制
	bool Copy(const BSplineCurve & B_SplineCurve3);
	//设置控制点
	bool Set(const Vector3 * pControlPoint,uint32 uiControlPointNum);
	//把控制点变换成闭合开放式控制点
	bool ChangeToOpenClosePoints();
	//把控制点变换成周期闭合控制点
	bool ChangeToPeriodicClosePoints(uint32 uiDegree);
	//设置节向量
	bool SetKnotVector(const float * pKnotVector,uint32 uiDegree,uint32 uiControlPointNum);
	//建立开发等距节向量
	bool CreateOpenUniformKnotVector(uint32 uiDegree,uint32 uiControlPointNum);
	//建立周期节向量
	bool CreatePeriodicKnotVector(uint32 uiDegree,uint32 uiControlPointNum);
	//求导数
	virtual Vector3	GetFirstDerivative(float t);
	virtual Vector3	GetSecondDerivative(float t);
	virtual Vector3	GetThirdDerivative(float t);
	//virtual float		GetLength(float t1 ,float t2);
	//得到上面的点
	virtual Vector3	GetPoint(float t);

protected:
	int32 GetKey(float t)const;
	bool GetFuncValue(float t,int32 &i,uint32 uiOrder);

protected:
	float		* m_pKnotVector;
	uint32 m_uiDegree;
	uint32 m_uiKnotNum;
	float		*m_pBasis;
	float		*m_pBasisFD;
	float		*m_pBasisSD;
	float		*m_pBasisTD;
	uint32 m_uiBasisNum;
};

}