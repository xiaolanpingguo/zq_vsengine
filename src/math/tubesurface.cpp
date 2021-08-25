#include "math/tubesurface.h"


namespace zq{


TubeSurface::TubeSurface()
{
	m_pCurve = NULL;
}

TubeSurface::~TubeSurface()
{
	m_pCurve = NULL;
}

bool TubeSurface::SetCurve(Curve * pCurve,float fN,float fB)
{
	if(!pCurve)
		return 0;
	m_pCurve = pCurve;
	m_fN = fN;
	m_fB = fB;

	return 1;
}

Vector3 TubeSurface::GetPoint(float U,float V)
{
	Vector3 T,B,N;
	m_pCurve->GetFrenetFrameBNT(U,B,N,T);
	B.Normalize();
	N.Normalize();
	return m_pCurve->GetPoint(U) + N * COS(MATH_2PI * V) * m_fN  + B * SIN(MATH_2PI * V) * m_fB;
}

Vector3 TubeSurface::GetUPartialDerivative(float U,float V)
{
	return m_pCurve->GetFirstDerivative(U);
}

Vector3 TubeSurface::GetVPartialDerivative(float U,float V)
{
	Vector3 T,B,N;
	m_pCurve->GetFrenetFrameBNT(U,B,N,T);
	B.Normalize();
	N.Normalize();
	return B * COS(MATH_2PI * V) * m_fB - N * SIN(MATH_2PI * V) * m_fN ;
}

}