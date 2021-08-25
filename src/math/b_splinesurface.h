#pragma once


#include "math/controlsurface.h"


namespace zq{


class MATH_API BSplineSurface : public ControlSurface
{
public:
	BSplineSurface();
	virtual ~BSplineSurface();

    inline bool SetVKnotVector(uint32 i, float Value)
    {
        VSMAC_ASSERT(m_pVKnotVector);
        VSMAC_ASSERT(i < m_uiVKnotNum);
        m_pVKnotVector[i] = Value;
        return 1;
    }

    inline const float* GetpVKnotVector()const
    {

        return m_pVKnotVector;
    }

    inline float GetVKnotVectorValue(uint32 i)const
    {
        VSMAC_ASSERT(m_pVKnotVector);

        VSMAC_ASSERT(i < m_uiVKnotNum);
        return m_pVKnotVector[i];
    }

    inline void SetVBasisValue(int32 i, int32 j, float Value)
    {
        m_pVBasis[i * m_uiVKnotNum + j] = Value;
    }

    inline float GetVBasisValue(int32 i, int32 j)const
    {
        return m_pVBasis[i * m_uiVKnotNum + j];

    }

    inline void SetVBasisFDValue(int32 i, int32 j, float Value)
    {
        m_pVBasisFD[i * m_uiUKnotNum + j] = Value;
    }

    inline float GetVBasisFDValue(int32 i, int32 j)const
    {
        return m_pVBasisFD[i * m_uiVKnotNum + j];
    }

    inline uint32 GetVDegree()const
    {
        return m_uiVDegree;
    }

    inline uint32 GetVKnotNum()const
    {
        return m_uiVKnotNum;
    }

    inline bool SetUKnotVector(uint32 i, float Value)
    {
        VSMAC_ASSERT(m_pUKnotVector);
        VSMAC_ASSERT(i < m_uiUKnotNum);
        m_pUKnotVector[i] = Value;
        return 1;
    }

    inline const float* GetpUKnotVector()const
    {
        return m_pUKnotVector;
    }

    inline float GetUKnotVectorValue(uint32 i)const
    {
        VSMAC_ASSERT(m_pUKnotVector);
        VSMAC_ASSERT(i < m_uiUKnotNum);
        return m_pUKnotVector[i];
    }

    inline void SetUBasisValue(int32 i, int32 j, float Value)
    {
        m_pUBasis[i * m_uiUKnotNum + j] = Value;
    }

    inline float GetUBasisValue(int32 i, int32 j)const
    {
        return m_pUBasis[i * m_uiUKnotNum + j];
    }

    inline void SetUBasisFDValue(int32 i, int32 j, float Value)
    {
        m_pUBasisFD[i * m_uiUKnotNum + j] = Value;
    }

    inline float GetUBasisFDValue(int32 i, int32 j)const
    {
        return m_pUBasisFD[i * m_uiUKnotNum + j];
    }

    inline uint32 GetUDegree()const
    {
        return m_uiUDegree;
    }

    inline uint32 GetUKnotNum()const
    {
        return m_uiUKnotNum;
    }

	bool Copy(const BSplineSurface & BSplineSurface);
	bool Set(const Vector3 * pControlPoint,uint32 uiRow,uint32 uiColumn);

	bool SetUKnotVector(const float * pUKnotVector,uint32 uiUDegree,uint32 uiRow);
	bool SetVKnotVector(const float * pVKnotVector,uint32 uiVDegree,uint32 uiColumn);

	bool CreateOpenUniformUKnotVector(uint32 uiDegree,uint32 uiRow);
	bool CreatePeriodicUKnotVector(uint32 uiDegree,uint32 uiRow);

	bool CreateOpenUniformVKnotVector(uint32 uiDegree,uint32 uiColumn);
	bool CreatePeriodicVKnotVector(uint32 uiDegree,uint32 uiColumn);
	int32 GetUKey(float t)const;
	int32 GetVKey(float t)const;

	virtual Vector3 GetPoint(float U,float V) = 0;
	virtual Vector3 GetUPartialDerivative(float U,float V) = 0;
	virtual Vector3 GetVPartialDerivative(float U,float V) = 0;

protected:
	bool GetUFuncValue(float t,int32 &i,uint32 uiOrder);
	bool GetVFuncValue(float t,int32 &i,uint32 uiOrder);

	float		* m_pUKnotVector;
	uint32 m_uiUDegree;
	uint32 m_uiUKnotNum;
	float		*m_pUBasis;
	float		*m_pUBasisFD;
	uint32 m_uiUBasisNum;


	float		* m_pVKnotVector;
	uint32 m_uiVDegree;
	uint32 m_uiVKnotNum;
	float		*m_pVBasis;
	float		*m_pVBasisFD;
	uint32 m_uiVBasisNum;
};
}
