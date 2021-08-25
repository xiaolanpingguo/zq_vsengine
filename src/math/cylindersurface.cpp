#include "math/cylindersurface.h"


namespace zq{


CylinderSurface::CylinderSurface()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}

CylinderSurface::~CylinderSurface()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}

bool CylinderSurface::SetCurve(Curve * pCurve1,SegmentCurve *pCurve2)
{
	if(!pCurve1 || !pCurve2)
		return 0;
	m_pCurve1 = pCurve1;
	m_pCurve2 = pCurve2;
	return 1;

}

Vector3 CylinderSurface::GetPoint(float U,float V)
{
	return m_pCurve1->GetPoint(U) + m_pCurve2->GetSegment().GetDir() * (V *  m_pCurve2->GetSegment().GetLen());

}

Vector3 CylinderSurface::GetUPartialDerivative(float U,float V)
{

	return m_pCurve1->GetFirstDerivative(U);
}

Vector3 CylinderSurface::GetVPartialDerivative(float U,float V)
{

	return m_pCurve2->GetSegment().GetDir();
}

}