#include "math/curvelengthfunction1.h"


namespace zq{


CurveLengthFunction1::CurveLengthFunction1()
{
	m_pCurve = NULL;
}
/*----------------------------------------------------------------*/
CurveLengthFunction1::~CurveLengthFunction1()
{

	m_pCurve = NULL;
}
/*----------------------------------------------------------------*/
float CurveLengthFunction1::GetValue(float fParameter)
{
	VSMAC_ASSERT(m_pCurve);
	Vector3 p = m_pCurve->GetFirstDerivative(fParameter);
	return SQRT(p.x * p.x + p.y * p.y + p.z * p.z);

}
/*----------------------------------------------------------------*/
float CurveLengthFunction1::GetFirstDerivative(float fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
float CurveLengthFunction1::GetSecondDerivative(float fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
float CurveLengthFunction1::GetThirdDerivative(float fParameter)
{
	return 0.0;
}

}