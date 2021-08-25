#include "math/beziersurface.h"
#include "core/memorymanager.h"


namespace zq{


BezierSurface::BezierSurface()
{
	m_UC = NULL;
	m_VC = NULL;
	m_UPDValue = NULL;
	m_VPDValue = NULL;

}
/*----------------------------------------------------------------*/
BezierSurface::~BezierSurface()
{
	SAFE_DELETEA(m_UC);
	SAFE_DELETEA(m_VC);
	SAFE_DELETEA(m_UPDValue);
	SAFE_DELETEA(m_VPDValue);
}
/*----------------------------------------------------------------*/
bool BezierSurface::Set(const Vector3 * pControlPoint,uint32 uiRow,uint32 uiColumn)
{
	VSMAC_ASSERT(pControlPoint && uiRow && uiColumn);

	m_uiColumn = uiColumn;
	m_uiRow = uiRow;
	m_uiControlPointNum = m_uiColumn * m_uiRow;
	
	SAFE_DELETEA(m_UC);
	SAFE_DELETEA(m_VC);
	SAFE_DELETEA(m_UPDValue);
	SAFE_DELETEA(m_VPDValue);
	SAFE_DELETEA(m_pControlPoint);

	m_pControlPoint = MEM_NEW Vector3[m_uiControlPointNum];
	VSMAC_ASSERT(m_pControlPoint);

	VSMemcpy(m_pControlPoint,pControlPoint,sizeof(Vector3) * m_uiControlPointNum);

	m_UC = MEM_NEW float [uiRow * uiRow];
	VSMAC_ASSERT(m_UC);
	memset(m_UC,0,sizeof(float) * uiRow * uiRow);
	SetUC(0,0,1.0f);
	SetUC(1,0,1.0f);
	SetUC(1,1,1.0f);
	for (uint32 i = 2; i <  uiRow ; i++)
	{
		SetUC(i,0,1.0f);
		SetUC(i,i,1.0f);

		for (uint32 j = 1; j < i; j++)
		{

			SetUC(i,j,GetUC(i - 1,j - 1) + GetUC(i - 1,j));
		}
	}

	m_VC = MEM_NEW float [m_uiColumn * m_uiColumn];
	VSMAC_ASSERT(m_VC);
	memset(m_VC,0,sizeof(float) * m_uiColumn * m_uiColumn);
	SetVC(0,0,1.0f);
	SetVC(1,0,1.0f);
	SetVC(1,1,1.0f);
	for (uint32 i = 2; i <  m_uiColumn; i++)
	{
		SetVC(i,0,1.0f);
		SetVC(i,i,1.0f);

		for (uint32 j = 1; j < i; j++)
		{

			SetVC(i,j,GetVC(i - 1,j - 1) + GetVC(i - 1,j));
		}
	}

	m_UPDValue = MEM_NEW Vector3[(m_uiRow - 1) * m_uiColumn];
	VSMAC_ASSERT(m_UPDValue);
	for (uint32 i = 0; i < m_uiRow - 1; i++)
	{
		for(uint32 j = 0 ; j < m_uiColumn ; j++)
		{
			SetUPD(i,j,GetControlPoint(i + 1,j) - GetControlPoint(i,j));
		}
	}

	m_VPDValue = MEM_NEW Vector3[m_uiRow * (m_uiColumn- 1)];
	VSMAC_ASSERT(m_VPDValue);

	for (uint32 i = 0; i < m_uiRow; i++)
	{
		for(uint32 j = 0 ; j < m_uiColumn - 1 ; j++)
		{
			SetVPD(i,j,GetControlPoint(i,j+1) - GetControlPoint(i,j));
		}
	}
	return 1;
}
/*----------------------------------------------------------------*/
Vector3 BezierSurface::GetPoint(float U,float V)
{
	VSMAC_ASSERT(m_UPDValue);

	float fFU = (float)1.0 - U;

	float fU =(float)1.0;
	


	uint32 uiDegreeU = m_uiRow - 1;
	uint32 uiDegreeV = m_uiColumn - 1;
	Vector3 kResult(0,0,0);

	for(uint32 i = 0 ; i <= uiDegreeU ; i++)
	{
		float fFV = (float)1.0 - V;

		float fV =(float)1.0; 
		float PU = float(uiDegreeU - i);
		for(uint32 j = 0; j <= uiDegreeV; j++)
		{
			float PV = float(uiDegreeV - j);
			Vector3 fCoeff = GetControlPoint(i,j) * GetUC(uiDegreeU,i) * fU * POW(fFU,PU) * GetVC(uiDegreeV,j) * fV * POW(fFV,PV);
			kResult = kResult + fCoeff;
			fV *= V;
			fU *= U;

		}

	}

	return kResult;

}
/*----------------------------------------------------------------*/
Vector3 BezierSurface::GetUPartialDerivative(float U,float V)
{
	VSMAC_ASSERT(m_UPDValue);

	float fFU = (float)1.0 - U;
	float fU = (float)1.0;
	
	

	uint32 uiDegreeU = m_uiRow - 2;
	uint32 uiDegreeV = m_uiColumn - 1;

	Vector3 kResult(0,0,0);
	
	for(uint32 i = 0 ; i <= uiDegreeU ; i++)
	{
		float PU = float(uiDegreeU - i);
		float fFV = (float)1.0 - V;
		float fV = (float)1.0; 
		for(uint32 j = 0; j <= uiDegreeV; j++)
		{
			float PV = float(uiDegreeV - j);
			Vector3 fCoeff = GetUPD(i,j) * GetUC(uiDegreeU,i) * fU * POW(fFU,PU) * GetVC(uiDegreeV,j) * fV * POW(fFV,PV);
			kResult = kResult + fCoeff;
			fV *= V;
			fU *= U;
		}

	}
		
	kResult *= (float)uiDegreeU;	
	return kResult;

}
/*----------------------------------------------------------------*/
Vector3 BezierSurface::GetVPartialDerivative(float U,float V)
{
	VSMAC_ASSERT(m_UPDValue);

	float fFU = (float)1.0 - U;
	float fU = (float)1.0;
	


	uint32 uiDegreeU = m_uiRow - 1;
	uint32 uiDegreeV = m_uiColumn - 2;
	Vector3 kResult(0,0,0);

	for(uint32 i = 0 ; i <= uiDegreeU ; i++)
	{
		float PU = float(uiDegreeU - i);
		float fFV = (float)1.0 - V;
		float fV = (float)1.0; 
		for(uint32 j = 0; j <= uiDegreeV; j++)
		{
			float PV = float(uiDegreeV - j);
			Vector3 fCoeff = GetVPD(i,j) * GetUC(uiDegreeU,i) * fU * POW(fFU,PU) * GetVC(uiDegreeV,j) * fV * POW(fFV,PV);
			kResult = kResult + fCoeff;
			fV *= V;
			fU *= U;
		}

	}

	kResult *= (float)uiDegreeV;	
	return kResult;
}
/*----------------------------------------------------------------*/
bool BezierSurface::Copy(const BezierSurface & BezierSurface3)
{
	if(!ControlSurface::Copy(BezierSurface3))
		return 0;
	VSMAC_ASSERT(BezierSurface3.m_VC && BezierSurface3.m_UC && BezierSurface3.m_UPDValue && BezierSurface3.m_VPDValue);

	SAFE_DELETEA(m_UC);
	SAFE_DELETEA(m_VC);
	SAFE_DELETEA(m_UPDValue);
	SAFE_DELETEA(m_VPDValue);
	
	m_UC = MEM_NEW float [m_uiRow * m_uiRow];
	VSMAC_ASSERT(m_UC);

	VSMemcpy(m_UC,BezierSurface3.m_UC,sizeof(float) * m_uiRow * m_uiRow);

	m_VC = MEM_NEW float [m_uiColumn * m_uiColumn];
	VSMAC_ASSERT(m_VC);

	VSMemcpy(m_VC,BezierSurface3.m_VC,sizeof(float) * m_uiColumn * m_uiColumn);

	m_UPDValue = MEM_NEW Vector3[(m_uiRow - 1) * m_uiColumn];
	VSMAC_ASSERT(m_UPDValue);

	VSMemcpy(m_UPDValue,BezierSurface3.m_UPDValue,sizeof(Vector3) * (m_uiRow - 1) * m_uiColumn);

	m_VPDValue = MEM_NEW Vector3[m_uiRow * (m_uiColumn- 1)];
	VSMAC_ASSERT(m_VPDValue);

	VSMemcpy(m_VPDValue,BezierSurface3.m_VPDValue,sizeof(Vector3) * m_uiRow * (m_uiColumn- 1));
	return 1;
}

}