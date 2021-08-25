#include "math/nurbssurface.h"
#include "core/memorymanager.h"


namespace zq{


NURBSSurface::NURBSSurface()
{
	m_pWeight = NULL;
}

NURBSSurface::~NURBSSurface()
{
	SAFE_DELETEA(m_pWeight);
}

bool NURBSSurface::Set(const float * pWeight,uint32 uiRow,uint32 uiColumn)
{
	if(!pWeight || !uiRow || !uiColumn)
		return 0;
	uint32 uiControlPointNum = uiRow * uiColumn;
	m_pWeight = MEM_NEW float[uiControlPointNum];
	VSMAC_ASSERT(m_pWeight);

	VSMemcpy(m_pWeight,pWeight,sizeof(float) * uiControlPointNum);
	return 1;
}

bool NURBSSurface::Get(uint32 Order,float U,float V ,Vector3 *D0,Vector3 *DU,Vector3 *DV)
{
	int32 i0,i1;
	if(GetUFuncValue(U,i0,Order) && GetVFuncValue(V,i1,Order))
	{
		Vector3 Point(0,0,0);
		float totalW = 0.0f;
		for(int32 j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
		for(int32 j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
		{
			float tmp =  GetUBasisValue(m_uiUDegree,j0) * GetVBasisValue(m_uiVDegree,j1) *GetWeight(j0,j1);

			Point += GetControlPoint(j0,j1) * tmp;
			totalW +=tmp;

		}
		float K0 = 1.0f / totalW;
		Vector3 Point0 = Point * K0 ;
		if(D0)
			*D0 = Point0;
		if(Order == 0)
			return 1;



		Vector3 kXDer1(0,0,0);
		Vector3 kXDer2(0,0,0);
		float fWDer1 = 0.0f;
		float fWDer2 = 0.0f;
		for(int32 j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
		for(int32 j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
		{
			float fTmp1 = GetUBasisFDValue(m_uiUDegree,j0) * GetVBasisValue(m_uiVDegree,j1) *GetWeight(j0,j1);
			float fTmp2 = GetUBasisValue(m_uiUDegree,j0) * GetVBasisFDValue(m_uiVDegree,j1) *GetWeight(j0,j1); 
			kXDer1 += GetControlPoint(j0,j1) * fTmp1;
			kXDer2 += GetControlPoint(j0,j1) * fTmp2;
			fWDer1 += fTmp1;
			fWDer2 += fTmp2;

		}

		Vector3 Point1 = (kXDer1 - Point0 * fWDer1) * K0 ;
		Vector3 Point2 = (kXDer2 - Point0 * fWDer2) * K0 ;
		if(DU)
			*DU = Point1;
		if(DV)
			*DV = Point2;
		if(Order == 1)
			return 1;
		// not control all path
		return 1;
	}
	else
		return 0;

}
/*----------------------------------------------------------------*/
Vector3 NURBSSurface::GetPoint(float U,float V)
{
	Vector3 Point;
	if(Get(0,U,V,&Point,NULL,NULL))
	{
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}

}
Vector3 NURBSSurface::GetUPartialDerivative(float U,float V)
{
	Vector3 Point;
	if(Get(1,U,V,NULL,&Point,NULL))
	{
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
}
/*----------------------------------------------------------------*/
Vector3 NURBSSurface::GetVPartialDerivative(float U,float V)
{
	Vector3 Point;
	if(Get(1,U,V,NULL,NULL,&Point))
	{
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}


}
/*----------------------------------------------------------------*/
bool NURBSSurface::Copy(const NURBSSurface & NURBSSurface3)
{
	if(!BSplineSurface::Copy(NURBSSurface3))
		return 0;
	if(!NURBSSurface3.m_pWeight)
		return 0;

	m_pWeight = MEM_NEW float[m_uiControlPointNum];
	VSMAC_ASSERT(m_pWeight);

	VSMemcpy(m_pWeight,NURBSSurface3.m_pWeight,sizeof(float) * m_uiControlPointNum);
	return 1;
}

}