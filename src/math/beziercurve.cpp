#include "math/beziercurve.h"
#include "core/memorymanager.h"


namespace zq{


BezierCurve3::BezierCurve3()
{
	m_pControlPoint = NULL;
	m_uiControlPointNum = 0;
	m_C = NULL;
	m_FDValue = NULL;
	m_SDValue = NULL;
	m_TDValue = NULL;
	m_CurveType = BEZIER;
}
/*----------------------------------------------------------------*/
BezierCurve3::~BezierCurve3()
{
	SAFE_DELETEA(m_pControlPoint);
	m_uiControlPointNum = 0;
	SAFE_DELETEA(m_C);
	SAFE_DELETEA(m_FDValue);
	SAFE_DELETEA(m_SDValue);
	SAFE_DELETEA(m_TDValue);

}
/*----------------------------------------------------------------*/
bool BezierCurve3::Set(const Vector3 * pControlPoint,uint32 uiControlPointNum)
{
	VSMAC_ASSERT(pControlPoint && uiControlPointNum);

	SAFE_DELETEA(m_pControlPoint);
	m_uiControlPointNum = uiControlPointNum;
	SAFE_DELETEA(m_C);
	SAFE_DELETEA(m_FDValue);
	SAFE_DELETEA(m_SDValue);
	SAFE_DELETEA(m_TDValue);

	m_pControlPoint = MEM_NEW Vector3[uiControlPointNum];
	VSMAC_ASSERT(m_pControlPoint);

	VSMemcpy(m_pControlPoint,pControlPoint,sizeof(Vector3) * m_uiControlPointNum);


	m_C = MEM_NEW float [uiControlPointNum * uiControlPointNum];
	VSMAC_ASSERT(m_C);

	memset(m_C,0,sizeof(float) * uiControlPointNum * uiControlPointNum);
	SetC(0,0,1.0f);
	SetC(1,0,1.0f);
	SetC(1,1,1.0f);
	for (uint32 i = 2; i <  uiControlPointNum ; i++)
	{
		SetC(i,0,1.0f);
		SetC(i,i,1.0f);
		
		for (uint32 j = 1; j < i; j++)
		{

			SetC(i,j,GetC(i - 1,j - 1) + GetC(i - 1,j));
		}
	}
	
	m_FDValue = MEM_NEW Vector3[m_uiControlPointNum-1];
	VSMAC_ASSERT(m_FDValue);
	for (uint32 i = 0; i < m_uiControlPointNum - 1; i++)
	{
		m_FDValue[i] = m_pControlPoint[i + 1] - m_pControlPoint[i];
	}

	
	m_SDValue = MEM_NEW Vector3[m_uiControlPointNum - 2];
	VSMAC_ASSERT(m_SDValue);
	for (uint32 i = 0; i < m_uiControlPointNum - 2; i++)
	{
		m_SDValue[i] = m_FDValue[i + 1] - m_FDValue[i];
	}

	
	if (m_uiControlPointNum >= 4)
	{
		m_TDValue = MEM_NEW Vector3[m_uiControlPointNum - 3];
		if(!m_TDValue)
			return 0;
		for (uint32 i = 0; i < m_uiControlPointNum - 3; i++)
		{
			m_TDValue[i] = m_SDValue[i + 1] - m_SDValue[i];
		}
	}
	return 1;
}
/*----------------------------------------------------------------*/
Vector3	BezierCurve3::GetFirstDerivative(float t)
{
	VSMAC_ASSERT(m_FDValue);

	float fOmTime = (float)1.0 - t;
	float fPowTime = (float)1.0;
	
	uint32 uiDegree = m_uiControlPointNum - 2;

	Vector3 kResult(0,0,0);
	for (uint32 i = 0; i <= uiDegree; i++)
	{
		float fCoeff = GetC(uiDegree,i) * fPowTime;
		float P =  float(uiDegree - i);
		kResult = kResult + m_FDValue[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}
	kResult *= float(m_uiControlPointNum - 1);
	return kResult;

}
/*----------------------------------------------------------------*/
Vector3	BezierCurve3::GetSecondDerivative(float t)
{
	VSMAC_ASSERT(m_SDValue);

	float fOmTime = (float)1.0 - t;
	float fPowTime = (float)1.0;

	uint32 uiDegree = m_uiControlPointNum - 3;

	Vector3 kResult(0,0,0);
	for (uint32 i = 0; i <= uiDegree; i++)
	{
		float fCoeff = GetC(uiDegree,i) * fPowTime;
		float P =  float(uiDegree - i);
		kResult = kResult + m_SDValue[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}
	kResult *= float((m_uiControlPointNum - 1) * (m_uiControlPointNum - 2));
	return kResult;

}
/*----------------------------------------------------------------*/
Vector3	BezierCurve3::GetThirdDerivative(float t)
{
	VSMAC_ASSERT(m_TDValue);

	float fOmTime = (float)1.0 - t;
	float fPowTime = (float)1.0;
	
	uint32 uiDegree = m_uiControlPointNum - 4;

	Vector3 kResult(0,0,0);
	for (uint32 i = 0; i <= uiDegree; i++)
	{
		float fCoeff = GetC(uiDegree,i) * fPowTime;
		float P =  float(uiDegree - i);
		kResult = kResult + m_TDValue[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}
	kResult *= float((m_uiControlPointNum - 3) * (m_uiControlPointNum - 1) * (m_uiControlPointNum - 2));
	return kResult;

}
/*----------------------------------------------------------------*/
Vector3	BezierCurve3::GetPoint(float t)
{
	float fOmTime = (float)1.0 - t;
	float fPowTime = (float)1.0;
	uint32 uiDegree = m_uiControlPointNum - 1;

	Vector3 kResult(0.0f,0.0f,0.0f);

	for (uint32 i = 0; i <= uiDegree; i++)
	{
		float fCoeff = GetC(uiDegree,i) * fPowTime;
		float P =  float(uiDegree - i);
		kResult = kResult + m_pControlPoint[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}


	return kResult;

}
/*----------------------------------------------------------------*/
bool BezierCurve3::Copy(const BezierCurve3 & BezierCurve3)
{
	
	if(!ControlCurve::Copy(BezierCurve3))
		return 0;
	VSMAC_ASSERT(BezierCurve3.m_C && BezierCurve3.m_FDValue && BezierCurve3.m_SDValue && BezierCurve3.m_TDValue);

	SAFE_DELETEA(m_C);
	SAFE_DELETEA(m_FDValue);
	SAFE_DELETEA(m_SDValue);
	SAFE_DELETEA(m_TDValue);
	m_C = MEM_NEW float [m_uiControlPointNum * m_uiControlPointNum];
	VSMAC_ASSERT(m_C);

	VSMemcpy(m_C,BezierCurve3.m_C,sizeof(float) * m_uiControlPointNum * m_uiControlPointNum);

	m_FDValue = MEM_NEW Vector3[m_uiControlPointNum-1];
	VSMAC_ASSERT(m_FDValue);

	VSMemcpy(m_FDValue,BezierCurve3.m_FDValue,sizeof(Vector3) * (m_uiControlPointNum - 1));

	m_SDValue = MEM_NEW Vector3[m_uiControlPointNum - 2];
	VSMAC_ASSERT(m_SDValue);

	VSMemcpy(m_SDValue,BezierCurve3.m_SDValue,sizeof(Vector3) * (m_uiControlPointNum - 2));
	if (m_uiControlPointNum >= 4)
	{
		m_TDValue = MEM_NEW Vector3[m_uiControlPointNum - 3];
		VSMAC_ASSERT(m_TDValue);
		VSMemcpy(m_TDValue,BezierCurve3.m_TDValue,sizeof(Vector3) * (m_uiControlPointNum - 1));
	}
	return 1;
}

}
