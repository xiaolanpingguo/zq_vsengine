#include "math/nurbscurve.h"
#include "core/memorymanager.h"


namespace zq {


NURBSCurve::NURBSCurve()
{
	m_pWeight = NULL;
	m_CurveType = NURBS;
}
/*----------------------------------------------------------------*/
NURBSCurve::~NURBSCurve()
{
	SAFE_DELETEA(m_pWeight);
}
/*----------------------------------------------------------------*/
bool NURBSCurve::Set(const float * pWeight,uint32 uiControlPointNum)
{
	if(!pWeight || !uiControlPointNum)
		return 0;
	m_pWeight = MEM_NEW float[uiControlPointNum];
	VSMAC_ASSERT(m_pWeight);

	VSMemcpy(m_pWeight,pWeight,sizeof(float) * uiControlPointNum);
	return 1;

}
/*----------------------------------------------------------------*/
bool NURBSCurve::Get(uint32 Order,float t ,Vector3 *D0,Vector3 *D1,Vector3 *D2,Vector3 *D3)
{
	int32 i;
	if(GetFuncValue(t,i,Order))
	{
		Vector3 Point(0,0,0);
		float totalW = 0.0f;
		for(int32 j = i - m_uiDegree; j <= i ; j++)
		{
			float tmp =  GetBasisValue(m_uiDegree,j) * m_pWeight[j];

			Point += m_pControlPoint[j] * tmp;
			totalW +=tmp;

		}
		float K0 = 1.0f / totalW;
		Vector3 Point0 = Point * K0 ;
		if(D0)
			*D0 = Point0;
		if(Order == 0)
			return 1;



		Vector3 kXDer1(0,0,0);
		float fWDer1 = 0.0f;
		for(int32 j = i - m_uiDegree; j <= i ; j++)
		{
			float fTmp = GetBasisFDValue(m_uiDegree,i) * m_pWeight[i];
			kXDer1 += m_pControlPoint[i] * fTmp;
			fWDer1 += fTmp;


		}
		float K1 = fWDer1;
		Vector3 Point1 = (kXDer1 - Point0 * fWDer1) * K0 ;
		if(D1)
			*D1 = Point1;
		if(Order == 1)
			return 1;

		Vector3 kXDer2(0,0,0);
		float fWDer2 = 0.0f;
		for(int32 j = i - m_uiDegree; j <= i ; j++)
		{
			float fTmp = GetBasisSDValue(m_uiDegree,i) * m_pWeight[i];
			kXDer2 += m_pControlPoint[i] * fTmp;
			fWDer2 += fTmp;

		}
		float K2 = fWDer2;
		Vector3 Point2 = (kXDer2 - Point1 * (2.0f) * K1  - Point0  * fWDer2) * K0;
		if(D2)
			*D2 = Point2;
		if(Order == 2)
			return 1;

		Vector3 kXDer3(0,0,0);
		float fWDer3 = 0.0f;
		for(int32 j = i - m_uiDegree; j <= i ; j++)
		{
			float fTmp = GetBasisTDValue(m_uiDegree,i) * m_pWeight[i];
			kXDer3 += m_pControlPoint[i] * fTmp;
			fWDer3 += fTmp;

		}
		Vector3 Point3 = (kXDer3 -  Point2 * ( 3.0 )* K1  - Point1 * ( 3.0 ) * K2   -  Point0 * fWDer3) * K0;
		if(D3)
			*D3 = Point3;
		if(Order ==  3)
			return 1;

		//not control all path
		return 1;
	}
	else
		return 0;

}
/*----------------------------------------------------------------*/
Vector3	NURBSCurve::GetPoint(float t)
{


	Vector3 Point;
	if(Get(0,t,&Point,NULL,NULL,NULL))
	{
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}

}
/*----------------------------------------------------------------*/
Vector3	NURBSCurve::GetFirstDerivative(float t)
{

	Vector3 Point;
	if(Get(1,t,NULL,&Point,NULL,NULL))
	{
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}

}
/*----------------------------------------------------------------*/
Vector3	NURBSCurve::GetSecondDerivative(float t)
{

	Vector3 Point;
	if(Get(2,t,NULL,NULL,&Point,NULL))
	{
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}

}
/*----------------------------------------------------------------*/
Vector3	NURBSCurve::GetThirdDerivative(float t)
{

	Vector3 Point;
	if(Get(3,t,NULL,NULL,NULL,&Point))
	{
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}

}
/*----------------------------------------------------------------*/
bool NURBSCurve::Copy(const NURBSCurve &NURBSCurve3)
{
	if(!BSplineCurve::Copy(NURBSCurve3))
		return 0;
	if(!NURBSCurve3.m_pWeight)
		return 0;
	m_pWeight = MEM_NEW float[m_uiControlPointNum];
	VSMAC_ASSERT(m_pWeight);

	VSMemcpy(m_pWeight,NURBSCurve3.m_pWeight,sizeof(float) * m_uiControlPointNum);
	return 1;
}

}