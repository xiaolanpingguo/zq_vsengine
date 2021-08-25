#include "math/controlsurface.h"
#include "core/memorymanager.h"
#include "math/surfacelengthfunction1.h"


namespace zq{


ControlSurface::ControlSurface()
{
	m_pControlPoint = NULL;
	m_uiControlPointNum = 0;
	m_uiRow = 0;
	m_uiColumn = 0;

}
/*----------------------------------------------------------------*/
ControlSurface::~ControlSurface()
{
	SAFE_DELETEA(m_pControlPoint);
	m_uiControlPointNum = 0;
	m_uiRow = 0;
	m_uiColumn = 0;

}

/*----------------------------------------------------------------*/
bool ControlSurface::Copy(const ControlSurface& ControlSurface)
{
	VSMAC_ASSERT(ControlSurface.m_pControlPoint && ControlSurface.m_uiColumn &&
		ControlSurface.m_uiControlPointNum && ControlSurface.m_uiRow);

	SAFE_DELETEA(m_pControlPoint);
	
	m_uiControlPointNum = ControlSurface.m_uiControlPointNum;
	m_uiRow = ControlSurface.m_uiRow;
	m_uiColumn = ControlSurface.m_uiColumn;
	m_pControlPoint = MEM_NEW Vector3[m_uiControlPointNum];
	VSMAC_ASSERT(m_pControlPoint);

	VSMemcpy(m_pControlPoint,ControlSurface.m_pControlPoint,sizeof(Vector3) * m_uiControlPointNum); 
	return 1;
}
float ControlSurface::GetULength(float V,float U1, float U2,uint32 uiIterations)
{
	SurfaceLengthFunction1 LengthF(SurfaceLengthFunction1::LT_U_LEN,V);
	LengthF.SetSurface(this);
	return LengthF.Integration(U1,U2,uiIterations);
}
float ControlSurface::GetVLength(float U,float V1, float V2,uint32 uiIterations)
{
	SurfaceLengthFunction1 LengthF(SurfaceLengthFunction1::LT_V_LEN,U);
	LengthF.SetSurface(this);
	return LengthF.Integration(V1,V2,uiIterations);
}
/*----------------------------------------------------------------*/
float ControlSurface::GetUTime (float V,float fLength, uint32 uiIterations,float fTolerance)
{

	if (fLength <= 0.0f)
	{
		return 0.0f;
	}

	float TotalLength = GetULength(V,0.0f,1.0f,uiIterations);

	if (fLength >= TotalLength )
	{
		return 1.0f;
	}

	float fTime = fLength / TotalLength;

	for (uint32 i = 0; i < uiIterations; i++)
	{
		float fDifference = GetULength(V,0.0f,fTime,uiIterations) - fLength;
		if (ABS(fDifference) < fTolerance)
		{
			return fTime;
		}

		fTime -= fDifference/(GetUPartialDerivative(fTime,V).GetLength());
	}

	return fTime;

}
/*----------------------------------------------------------------*/
float ControlSurface::GetVTime (float U,float fLength, uint32 uiIterations,float fTolerance)
{

	if (fLength <= 0.0f)
	{
		return 0.0f;
	}
	float TotalLength = GetVLength(U,0.0f,1.0f,uiIterations);
	if (fLength >= TotalLength )
	{
		return 1.0f;
	}

	float fTime = fLength / TotalLength;

	for (uint32 i = 0; i < uiIterations; i++)
	{
		float fDifference = GetVLength(U,0.0f,fTime,uiIterations) - fLength;
		if (ABS(fDifference) < fTolerance)
		{
			return fTime;
		}

		fTime -= fDifference/(GetVPartialDerivative(fTime,U).GetLength());
	}

	return fTime;
}

}