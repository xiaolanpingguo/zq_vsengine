#include "math/controlcurve.h"
#include "math/matrix3X3w.h"
#include "core/memorymanager.h"
#include "math/curvelengthfunction1.h"


namespace zq{


ControlCurve::ControlCurve()
{
	m_pControlPoint = NULL;
	m_uiControlPointNum = 0;

}
/*----------------------------------------------------------------*/
ControlCurve::~ControlCurve()
{
	SAFE_DELETEA(m_pControlPoint);
	m_uiControlPointNum = 0;

}
/*----------------------------------------------------------------*/
void	ControlCurve::GetFrenetFrameBNT(float t,Vector3 & B,Vector3 & N,Vector3 &T)
{
	Vector3 FD = GetFirstDerivative(t);
	FD.Normalize();
	Vector3 SD = GetSecondDerivative(t);
	SD.Normalize();
	float FDDot = FD.Dot(FD);
	float SDDotFD = FD.Dot(SD);
	N = SD - FD * SDDotFD;
	N.Normalize();
	T = FD;
	B.Cross(N,T);

}
/*----------------------------------------------------------------*/
float ControlCurve::GetTotalLength(uint32 uiIterations)
{
	return GetLength(0.0f,1.0f,uiIterations);

}
/*----------------------------------------------------------------*/
float ControlCurve::GetTime (float fLength, uint32 uiIterations,float fTolerance)
{

	if (fLength <= 0.0f)
	{
		return 0.0f;
	}
	float TotalLength = GetTotalLength(uiIterations);

	if (fLength >= TotalLength )
	{
		return 1.0f;
	}

	float fTime = fLength / TotalLength;

	for (uint32 i = 0; i < uiIterations; i++)
	{
		float fDifference = GetLength(0.0f,fTime,uiIterations) - fLength;
		if (ABS(fDifference) < fTolerance)
		{
			return fTime;
		}

		fTime -= fDifference/(GetFirstDerivative(fTime).GetLength());
	}

	return fTime;

}
/*----------------------------------------------------------------*/
float ControlCurve::GetCurvature(float fTime)
{
	Vector3 FD = GetFirstDerivative(fTime);
	float fSpeedSqr = FD.GetSqrLength();

	if (fSpeedSqr >= EPSILON_E4)
	{
		Vector3 SD = GetSecondDerivative(fTime);
		Vector3 Cross;
		Cross.Cross(FD,SD);
		float fNumer = Cross.GetLength();
		float fDenom = POW(fSpeedSqr,(float)1.5);

		return fNumer / fDenom;
	}
	else
	{

		return 0.0f;
	}

}
/*----------------------------------------------------------------*/
float ControlCurve::GetTorsion(float fTime)
{
	Vector3 FD = GetFirstDerivative(fTime);
	Vector3 SD = GetSecondDerivative(fTime);
	Vector3 Cross;
	Cross.Cross(FD,SD);
	float fDenom = Cross.GetSqrLength();

	if (fDenom >= EPSILON_E4)
	{
		Vector3 TD= GetThirdDerivative(fTime);
		float fNumer = Cross.Dot(TD);
		return fNumer / fDenom;
	}
	else
	{
		return 0.0f;
	}

}
/*----------------------------------------------------------------*/
bool ControlCurve::Copy(const ControlCurve& ControlCurve3)
{
	VSMAC_ASSERT(ControlCurve3.m_pControlPoint && ControlCurve3.m_uiControlPointNum);
		return 0;

	SAFE_DELETEA(m_pControlPoint);
	m_uiControlPointNum = ControlCurve3.m_uiControlPointNum;
	m_pControlPoint = MEM_NEW Vector3[m_uiControlPointNum];
	VSMAC_ASSERT(m_pControlPoint);

	VSMemcpy(m_pControlPoint,ControlCurve3.m_pControlPoint,sizeof(Vector3) * m_uiControlPointNum);
	return 1;
}
/*----------------------------------------------------------------*/
float	ControlCurve::GetLength(float t1 ,float t2,uint32 uiIterations)
{
	CurveLengthFunction1 LengthF;
	LengthF.SetCurve(this);
	return LengthF.Integration(t1,t2,uiIterations);
}

}