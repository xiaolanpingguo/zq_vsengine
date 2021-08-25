#include "math/generalizedcylindersurface.h"


namespace zq{


GeneralizedCylinderSurface::GeneralizedCylinderSurface()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}
/*----------------------------------------------------------------*/
GeneralizedCylinderSurface::~GeneralizedCylinderSurface()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}
/*----------------------------------------------------------------*/
bool GeneralizedCylinderSurface::SetCurve(Curve * pCurve1,Curve *pCurve2)
{
	if(!pCurve1 || !pCurve2)
		return 0;
	m_pCurve1 = pCurve1;
	m_pCurve2 = pCurve2;
	return 1;
}
/*----------------------------------------------------------------*/
Vector3 GeneralizedCylinderSurface::GetPoint(float U,float V)
{
	return m_pCurve1->GetPoint(U) * (1 - V) + m_pCurve2->GetPoint(U) * V;
}
/*----------------------------------------------------------------*/
Vector3 GeneralizedCylinderSurface::GetUPartialDerivative(float U,float V)
{
	Vector3 Temp = m_pCurve1->GetFirstDerivative(U) * (1 - V) + m_pCurve2->GetFirstDerivative(U) * V;
	//Temp.Normalize();
	return Temp;
}
/*----------------------------------------------------------------*/
Vector3 GeneralizedCylinderSurface::GetVPartialDerivative(float U,float V)
{
	Vector3 Temp = m_pCurve2->GetPoint(U) - m_pCurve1->GetPoint(U);
	//Temp.Normalize();
	return Temp;
}

}