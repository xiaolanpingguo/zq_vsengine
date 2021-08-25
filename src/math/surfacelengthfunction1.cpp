#include "math/surfacelengthfunction1.h"


namespace zq{


SurfaceLengthFunction1::SurfaceLengthFunction1(uint32 uiLengthType,float fTypePara)
{
	m_pSurface = NULL;
	m_fTypePara = fTypePara;
	m_uiLengthType = uiLengthType;
}
/*----------------------------------------------------------------*/
SurfaceLengthFunction1::~SurfaceLengthFunction1()
{

	m_pSurface = NULL;
}
/*----------------------------------------------------------------*/
float SurfaceLengthFunction1::GetValue(float fParameter)
{
	if(!m_pSurface)
		return MATH_MAX_FLOAT;
	if (m_uiLengthType == LT_U_LEN)
	{
		Vector3 p = m_pSurface->GetUPartialDerivative(fParameter,m_fTypePara);
		return SQRT(p.x * p.x + p.y * p.y + p.z * p.z);
	}
	else
	{
		Vector3 p = m_pSurface->GetVPartialDerivative(m_fTypePara,fParameter);
		return SQRT(p.x * p.x + p.y * p.y + p.z * p.z);
	}

}
/*----------------------------------------------------------------*/
float SurfaceLengthFunction1::GetFirstDerivative(float fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
float SurfaceLengthFunction1::GetSecondDerivative(float fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
float SurfaceLengthFunction1::GetThirdDerivative(float fParameter)
{
	return 0.0;
}

}