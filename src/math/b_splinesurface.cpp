#include "math/b_splinesurface.h"
#include "core/memorymanager.h"


namespace zq{


BSplineSurface::BSplineSurface()
{
	m_pUKnotVector = NULL;
	m_uiUDegree = 0;
	m_uiUKnotNum = 0;
	m_pUBasis = NULL;
	m_pUBasisFD = NULL;
	m_uiUBasisNum = 0;


	m_pVKnotVector = NULL;
	m_uiVDegree = 0;
	m_uiVKnotNum = 0;
	m_pVBasis = NULL;
	m_pVBasisFD = NULL;
	m_uiVBasisNum = 0;
	
}
/*----------------------------------------------------------------*/
BSplineSurface::~BSplineSurface()
{
	SAFE_DELETEA(m_pUKnotVector);
	m_uiUDegree = 0;
	m_uiUKnotNum = 0;
	SAFE_DELETEA(m_pUBasis);
	SAFE_DELETEA(m_pUBasisFD);
	m_uiUBasisNum = 0;


	SAFE_DELETEA(m_pVKnotVector);
	m_uiVDegree = 0;
	m_uiVKnotNum = 0;
	SAFE_DELETEA(m_pVBasis);
	SAFE_DELETEA(m_pVBasisFD);
	m_uiVBasisNum = 0;

}
/*----------------------------------------------------------------*/
bool BSplineSurface::Set(const Vector3 * pControlPoint,uint32 uiRow,uint32 uiColumn)
{
	VSMAC_ASSERT(pControlPoint && uiRow && uiColumn);

	SAFE_DELETEA(m_pControlPoint);
	m_uiControlPointNum = uiRow * uiColumn;
	m_uiRow = uiRow;
	m_uiColumn = uiColumn;
	m_pControlPoint = MEM_NEW Vector3[m_uiControlPointNum];
	VSMAC_ASSERT(m_pControlPoint);

	VSMemcpy(m_pControlPoint,pControlPoint,m_uiControlPointNum * sizeof(Vector3));
	return 1;

}
/*----------------------------------------------------------------*/
bool BSplineSurface::SetUKnotVector(const float * pUKnotVector,uint32 uiUDegree,uint32 uiRow)
{
	VSMAC_ASSERT(pUKnotVector);
	for(uint32 i = 0 ; i < uiUDegree + uiRow; i++)
	{
		if(pUKnotVector[i] > pUKnotVector[i+1])
			return 0;
	}
	m_uiUDegree = uiUDegree;
	m_uiRow = uiRow;
	SAFE_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = uiRow + 1 + uiUDegree;
	m_pUKnotVector = MEM_NEW float[m_uiUKnotNum];
	VSMAC_ASSERT(m_pUKnotVector)

	VSMemcpy(m_pUKnotVector,pUKnotVector,sizeof(float) * m_uiUKnotNum);

	SAFE_DELETEA(m_pUBasis);
	m_uiUBasisNum = (uiUDegree + 1) * m_uiUKnotNum;
	m_pUBasis = MEM_NEW float[m_uiUBasisNum]; 
	VSMAC_ASSERT(m_pUBasis);


	SAFE_DELETEA(m_pUBasisFD);
	m_pUBasisFD = MEM_NEW float[m_uiUBasisNum]; 
	VSMAC_ASSERT(m_pUBasisFD);



	return 1;

}
/*----------------------------------------------------------------*/
bool BSplineSurface::SetVKnotVector(const float * pVKnotVector,uint32 uiVDegree,uint32 uiColumn)
{
	VSMAC_ASSERT(pVKnotVector);

	for(uint32 i = 0 ; i < uiVDegree + uiColumn; i++)
	{
		if(pVKnotVector[i] > pVKnotVector[i+1])
			return 0;
	}

	m_uiVDegree = uiVDegree;
	m_uiColumn= uiColumn;
	SAFE_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = uiColumn + 1 + uiVDegree;
	m_pVKnotVector = MEM_NEW float[m_uiVKnotNum];
	VSMemcpy(m_pVKnotVector,pVKnotVector,sizeof(float) * (m_uiVKnotNum));

	SAFE_DELETEA(m_pVBasis);
	m_uiVBasisNum = (uiVDegree + 1) * m_uiVKnotNum;
	m_pVBasis = MEM_NEW float[m_uiVBasisNum]; 
	VSMAC_ASSERT(m_pVBasis)


	SAFE_DELETEA(m_pVBasisFD);
	m_pVBasisFD = MEM_NEW float[m_uiVBasisNum]; 
	VSMAC_ASSERT(m_pVBasisFD)



	return 1;
}
/*----------------------------------------------------------------*/
bool BSplineSurface::CreateOpenUniformUKnotVector(uint32 uiDegree,uint32 uiRow)
{
	m_uiUDegree = uiDegree;
	m_uiRow = uiRow;
	SAFE_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = uiRow + 1 + uiDegree;

	m_pUKnotVector = MEM_NEW float[m_uiUKnotNum];
	VSMAC_ASSERT(m_pUKnotVector)

	for(uint32 i = 0; i < m_uiUKnotNum ; i++)
	{
		if(i >= 0 && i <= m_uiUDegree)
		{
			m_pUKnotVector[i] = 0;

		}
		else if( i >= m_uiUDegree + 1 && i <= m_uiRow)
		{
			m_pUKnotVector[i] = ((float)1.0 * (i - m_uiUDegree)) / (m_uiRow + 1 - m_uiUDegree);

		}
		else
		{
			m_pUKnotVector[i] = 1;
		}
	}

	SAFE_DELETEA(m_pUBasis);
	m_uiUBasisNum = (uiDegree + 1) * m_uiUKnotNum;
	m_pUBasis = MEM_NEW float[m_uiUBasisNum]; 
	VSMAC_ASSERT(m_pUBasis);


	SAFE_DELETEA(m_pUBasisFD);
	m_pUBasisFD = MEM_NEW float[m_uiUBasisNum]; 
	VSMAC_ASSERT(m_pUBasisFD);


	return 1;
}
/*----------------------------------------------------------------*/
bool BSplineSurface::CreatePeriodicUKnotVector(uint32 uiDegree,uint32 uiRow)
{
	m_uiUDegree = uiDegree;
	m_uiRow = uiRow;
	SAFE_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = uiRow + 1 + uiDegree;
	m_pUKnotVector = MEM_NEW float[m_uiUKnotNum];
	VSMAC_ASSERT(m_pUKnotVector);

	for(uint32 i = 0 ; i < m_uiUKnotNum ; i++)
	{

		m_pUKnotVector[i] = ((float)1.0 * (i - m_uiUDegree)) / (m_uiRow + 1 - m_uiUDegree);

	}

	SAFE_DELETEA(m_pUBasis);
	m_uiUBasisNum = (uiDegree + 1) * m_uiUKnotNum;
	m_pUBasis = MEM_NEW float[m_uiUBasisNum]; 
	VSMAC_ASSERT(m_pUBasis);


	SAFE_DELETEA(m_pUBasisFD);
	m_pUBasisFD = MEM_NEW float[m_uiUBasisNum]; 
	VSMAC_ASSERT(m_pUBasisFD);

	return 1;
}
/*----------------------------------------------------------------*/
bool BSplineSurface::CreateOpenUniformVKnotVector(uint32 uiDegree,uint32 uiColumn)
{
	m_uiVDegree = uiDegree;
	m_uiColumn = uiColumn;
	SAFE_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = uiColumn + 1 + uiDegree;

	m_pVKnotVector = MEM_NEW float[m_uiVKnotNum];
	VSMAC_ASSERT(m_pVKnotVector);

	for(uint32 i  = 0 ; i < m_uiVKnotNum ; i++)
	{
		if(i >= 0 && i <= m_uiVDegree)
		{
			m_pVKnotVector[i] = 0;

		}
		else if( i >= m_uiVDegree + 1 && i <= m_uiColumn)
		{
			m_pVKnotVector[i] = ((float)1.0 * (i - m_uiVDegree)) / (m_uiColumn + 1 - m_uiVDegree);

		}
		else
		{
			m_pVKnotVector[i] = 1;
		}
	}

	SAFE_DELETEA(m_pVBasis);
	m_uiVBasisNum = (uiDegree + 1) * m_uiVKnotNum;
	m_pVBasis = MEM_NEW float[m_uiVBasisNum]; 

	VSMAC_ASSERT(m_pVBasis);


	SAFE_DELETEA(m_pVBasisFD);
	m_pVBasisFD = MEM_NEW float[m_uiVBasisNum]; 
	VSMAC_ASSERT(m_pVBasisFD);

	return 1;
}
/*----------------------------------------------------------------*/
bool BSplineSurface::CreatePeriodicVKnotVector(uint32 uiDegree,uint32 uiColumn)
{
	m_uiVDegree = uiDegree;
	m_uiColumn = uiColumn;
	SAFE_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = uiColumn + 1 + uiDegree;
	m_pVKnotVector = MEM_NEW float[m_uiVKnotNum];
	VSMAC_ASSERT(m_pVKnotVector);

	for(uint32 i = 0; i < m_uiVKnotNum ; i++)
	{

		m_pVKnotVector[i] = ((float)1.0 * (i - m_uiVDegree)) / (m_uiColumn + 1 - m_uiVDegree);

	}

	SAFE_DELETEA(m_pVBasis);
	m_uiVBasisNum = (uiDegree + 1) * m_uiVKnotNum;
	m_pVBasis = MEM_NEW float[m_uiVBasisNum]; 
	VSMAC_ASSERT(m_pVBasis);


	SAFE_DELETEA(m_pVBasisFD);
	m_pVBasisFD = MEM_NEW float[m_uiVBasisNum]; 
	VSMAC_ASSERT(m_pVBasisFD);


	return 1;

}
/*----------------------------------------------------------------*/
int32 BSplineSurface::GetUKey(float t)const
{
	VSMAC_ASSERT(t <= 1.0f && t >= 0.0f);
	uint32 i;
	for(i = 0 ; i < m_uiUKnotNum ; i++)
	{
		if(t < m_pUKnotVector[i])
			break;

	}
	return (i - 1);
}
/*----------------------------------------------------------------*/
int32 BSplineSurface::GetVKey(float t)const
{
	VSMAC_ASSERT(t <= 1.0f && t >= 0.0f);
	uint32 i;
	for(i = 0 ; i < m_uiVKnotNum ; i++)
	{
		if(t < m_pVKnotVector[i])
			break;

	}
	return (i - 1);
}
/*----------------------------------------------------------------*/
bool BSplineSurface::GetUFuncValue(float t,int32 &i,uint32 uiOrder)
{
	VSMAC_ASSERT(m_pUBasis);
	i = GetUKey(t);
	memset(m_pUBasis,0,sizeof(float) * m_uiUBasisNum);
	memset(m_pUBasisFD,0,sizeof(float) * m_uiUBasisNum);

	SetUBasisValue(0,i,1.0f);



	float fN0 = t - m_pUKnotVector[i];
	float fN1 = m_pUKnotVector[i + 1] - t;
	float fInvD0, fInvD1;
	uint32 j;
	for (j = 1; j <= m_uiUDegree; j++)
	{
		fInvD0 = ((float)1.0) / (m_pUKnotVector[i + j] - m_pUKnotVector[i]);
		fInvD1 = ((float)1.0) / (m_pUKnotVector[i + 1] - m_pUKnotVector[i - j + 1]);

		SetUBasisValue(j,i,fN0 * GetUBasisValue(j - 1,i) * fInvD0);
		SetUBasisValue(j,i - j,fN1 * GetUBasisValue(j - 1,i - j + 1) * fInvD1);

		if (uiOrder >= 1) 
		{
			SetUBasisFDValue(j,i,(fN0 * GetUBasisFDValue(j - 1,i) + GetUBasisFDValue(j - 1,i)) * fInvD0);
			SetUBasisFDValue(j,i - j, (fN1*GetUBasisFDValue(j - 1,i - j + 1)-GetUBasisFDValue(j - 1,i - j + 1)) * fInvD1);

			
		}
	}

	for (j = 2; j <= m_uiUDegree; j++)
	{
		for (int32 k = i - j + 1; k < i; k++)
		{
			fN0 = t - m_pUKnotVector[k];
			fN1 = m_pUKnotVector[k + j + 1]-t;
			fInvD0 = ((float)1.0) / (m_pUKnotVector[k + j]  - m_pUKnotVector[k]);
			fInvD1 = ((float)1.0) / (m_pUKnotVector[k + j + 1] - m_pUKnotVector[k +1 ]);

			SetUBasisValue(j,k ,fN0 * GetUBasisValue(j - 1,k) * fInvD0 + fN1 * GetUBasisValue(j - 1,k + 1) * fInvD1);

			if (uiOrder >= 1)
			{
				SetUBasisFDValue(j,k,(fN0 * GetUBasisFDValue(j-1,k) + GetUBasisFDValue(j-1,k) ) *
					fInvD0 + (fN1 * GetUBasisFDValue(j-1,k+1) - GetUBasisFDValue(j-1,k+1)) *
					fInvD1);

				
			}
		}
	}

	return 1;
}
/*----------------------------------------------------------------*/
bool BSplineSurface::GetVFuncValue(float t,int32 &i,uint32 uiOrder)
{
	VSMAC_ASSERT(m_pVBasis);
		return 0;

	i = GetVKey(t);

	memset(m_pVBasis,0,sizeof(float) * m_uiVBasisNum);
	memset(m_pVBasisFD,0,sizeof(float) * m_uiVBasisNum);

	SetVBasisValue(0,i,1.0f);



	float fN0 = t - m_pVKnotVector[i];
	float fN1 = m_pVKnotVector[i + 1] - t;
	float fInvD0, fInvD1;
	uint32 j;
	for (j = 1; j <= m_uiVDegree; j++)
	{
		fInvD0 = ((float)1.0) / (m_pVKnotVector[i + j] - m_pVKnotVector[i]);
		fInvD1 = ((float)1.0) / (m_pVKnotVector[i + 1] - m_pVKnotVector[i - j + 1]);

		SetVBasisValue(j,i,fN0 * GetVBasisValue(j - 1,i) * fInvD0);
		SetVBasisValue(j,i - j,fN1 * GetVBasisValue(j - 1,i - j + 1) * fInvD1);

		if (uiOrder >= 1) 
		{
			SetVBasisFDValue(j,i,(fN0 * GetVBasisFDValue(j - 1,i) + GetVBasisFDValue(j - 1,i)) * fInvD0);
			SetVBasisFDValue(j,i - j, (fN1*GetVBasisFDValue(j - 1,i - j + 1)-GetVBasisFDValue(j - 1,i - j + 1)) * fInvD1);


		}
	}

	for (j = 2; j <= m_uiVDegree; j++)
	{
		for (int32 k = i - j + 1; k < i; k++)
		{
			fN0 = t - m_pVKnotVector[k];
			fN1 = m_pVKnotVector[k + j + 1]-t;
			fInvD0 = ((float)1.0) / (m_pVKnotVector[k + j]  - m_pVKnotVector[k]);
			fInvD1 = ((float)1.0) / (m_pVKnotVector[k + j + 1] - m_pVKnotVector[k +1 ]);

			SetVBasisValue(j,k ,fN0 * GetVBasisValue(j - 1,k) * fInvD0 + fN1 * GetVBasisValue(j - 1,k + 1) * fInvD1);

			if (uiOrder >= 1)
			{
				SetVBasisFDValue(j,k,(fN0 * GetVBasisFDValue(j-1,k) + GetVBasisFDValue(j-1,k) ) *
					fInvD0 + (fN1 * GetVBasisFDValue(j-1,k+1) - GetVBasisFDValue(j-1,k+1)) *
					fInvD1);


			}
		}
	}

	return 1;
}
/*----------------------------------------------------------------*/
Vector3 BSplineSurface::GetPoint(float U,float V)
{
	int32 i0,i1;
	if(GetUFuncValue(U,i0,0) && GetUFuncValue(V,i1,0))
	{
		Vector3 Point(0,0,0);
		for(int32 j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
		for(int32 j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
		{
			Point += GetControlPoint(j0,j1) * GetUBasisValue(m_uiUDegree,j0) * GetUBasisValue(m_uiVDegree,j1);

		}
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
}
/*----------------------------------------------------------------*/
Vector3 BSplineSurface::GetUPartialDerivative(float U,float V)
{
	int32 i0,i1;
	if(GetUFuncValue(U,i0,1) && GetUFuncValue(V,i1,0))
	{
		Vector3 Point(0,0,0);
		for(int32 j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
			for(int32 j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
			{
				Point += GetControlPoint(j0,j1) * GetUBasisFDValue(m_uiUDegree,j0) * GetUBasisValue(m_uiVDegree,j1);

			}
			return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
}
/*----------------------------------------------------------------*/
Vector3 BSplineSurface::GetVPartialDerivative(float U,float V)
{
	int32 i0,i1;
	if(GetUFuncValue(U,i0,0) && GetUFuncValue(V,i1,1))
	{
		Vector3 Point(0,0,0);
		for(int32 j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
			for(int32 j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
			{
				Point += GetControlPoint(j0,j1) * GetUBasisValue(m_uiUDegree,j0) * GetUBasisFDValue(m_uiVDegree,j1);

			}
			return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
}
/*----------------------------------------------------------------*/
bool BSplineSurface::Copy(const BSplineSurface & BSplineSurface)
{
	if(!ControlSurface::Copy(BSplineSurface))
		return 0;

	if(!BSplineSurface.m_pUBasis || !BSplineSurface.m_pUKnotVector || !BSplineSurface.m_pUBasisFD ||
		!BSplineSurface.m_uiUDegree || !BSplineSurface.m_uiUBasisNum || !BSplineSurface.m_uiUKnotNum)
		return 0;
	if(!BSplineSurface.m_pVBasis || !BSplineSurface.m_pVKnotVector || !BSplineSurface.m_pVBasisFD ||
		!BSplineSurface.m_uiVDegree || !BSplineSurface.m_uiVBasisNum || !BSplineSurface.m_uiVKnotNum)
		return 0;

	m_uiUDegree = BSplineSurface.m_uiUDegree;
	SAFE_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = BSplineSurface.m_uiUKnotNum;
	m_pUKnotVector = MEM_NEW float[m_uiUKnotNum];
	VSMAC_ASSERT(m_pUKnotVector);

	VSMemcpy(m_pUKnotVector,BSplineSurface.m_pUKnotVector,sizeof(float) * (m_uiUKnotNum));

	SAFE_DELETEA(m_pUBasis);
	m_uiUBasisNum = BSplineSurface.m_uiUBasisNum;
	m_pUBasis = MEM_NEW float[m_uiUBasisNum]; 
	VSMAC_ASSERT(m_pUBasis);


	SAFE_DELETEA(m_pUBasisFD);
	m_pUBasisFD = MEM_NEW float[m_uiUBasisNum]; 
	VSMAC_ASSERT(m_pUBasisFD);



	m_uiVDegree = BSplineSurface.m_uiVDegree;
	SAFE_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = BSplineSurface.m_uiVKnotNum;
	m_pVKnotVector = MEM_NEW float[m_uiVKnotNum];
	VSMAC_ASSERT(m_pVKnotVector);

	VSMemcpy(m_pVKnotVector,BSplineSurface.m_pVKnotVector,sizeof(float) * (m_uiVKnotNum));

	SAFE_DELETEA(m_pVBasis);
	m_uiVBasisNum = BSplineSurface.m_uiVBasisNum;
	m_pVBasis = MEM_NEW float[m_uiVBasisNum]; 
	VSMAC_ASSERT(m_pVBasis);


	SAFE_DELETEA(m_pVBasisFD);
	m_pVBasisFD = MEM_NEW float[m_uiVBasisNum]; 
	VSMAC_ASSERT(m_pVBasisFD);

	return 1;
}

}