#pragma once


#include "math/controlcurve.h"
/*
	B_Spline���ߣ����ڿ��Ƶ���������һ�֣��ɿ��Ƶ㣬�����������ߵĶȣ�����ʽ���,����t��0-1֮��

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


	//����
	bool Copy(const BSplineCurve & B_SplineCurve3);
	//���ÿ��Ƶ�
	bool Set(const Vector3 * pControlPoint,uint32 uiControlPointNum);
	//�ѿ��Ƶ�任�ɱպϿ���ʽ���Ƶ�
	bool ChangeToOpenClosePoints();
	//�ѿ��Ƶ�任�����ڱպϿ��Ƶ�
	bool ChangeToPeriodicClosePoints(uint32 uiDegree);
	//���ý�����
	bool SetKnotVector(const float * pKnotVector,uint32 uiDegree,uint32 uiControlPointNum);
	//���������Ⱦ������
	bool CreateOpenUniformKnotVector(uint32 uiDegree,uint32 uiControlPointNum);
	//�������ڽ�����
	bool CreatePeriodicKnotVector(uint32 uiDegree,uint32 uiControlPointNum);
	//����
	virtual Vector3	GetFirstDerivative(float t);
	virtual Vector3	GetSecondDerivative(float t);
	virtual Vector3	GetThirdDerivative(float t);
	//virtual float		GetLength(float t1 ,float t2);
	//�õ�����ĵ�
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