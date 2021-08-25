#include "math/b_splinecurve.h"
#include "core/memorymanager.h"


namespace zq{


BSplineCurve::BSplineCurve()
{
	m_pControlPoint = NULL;
	m_pKnotVector = NULL;
	m_uiControlPointNum = 0;
	m_uiDegree = 0;
	m_uiKnotNum = 0;
	m_pBasis = 0;
	m_pBasisFD = 0;
	m_pBasisSD = 0;
	m_pBasisTD = 0;
	m_uiBasisNum = 0;
	m_CurveType = B_SPLINE;
}
/*----------------------------------------------------------------*/
BSplineCurve::~BSplineCurve()
{
	SAFE_DELETEA(m_pKnotVector);
	SAFE_DELETEA(m_pControlPoint);
	SAFE_DELETEA(m_pBasis);
	SAFE_DELETEA(m_pBasisFD);
	SAFE_DELETEA(m_pBasisSD);
	SAFE_DELETEA(m_pBasisTD);
}
/*----------------------------------------------------------------*/
bool BSplineCurve::Set(const Vector3 * pControlPoint,uint32 uiControlPointNum)
{

	VSMAC_ASSERT(pControlPoint && uiControlPointNum);
	SAFE_DELETEA(m_pControlPoint);

	m_pControlPoint = MEM_NEW Vector3[uiControlPointNum];
	VSMAC_ASSERT(m_pControlPoint);
	m_uiControlPointNum = uiControlPointNum;
	VSMemcpy(m_pControlPoint,pControlPoint,uiControlPointNum * sizeof(Vector3));
	return 1;
	
}
/*----------------------------------------------------------------*/
bool BSplineCurve::ChangeToOpenClosePoints()
{
	VSMAC_ASSERT(m_pControlPoint && m_uiControlPointNum);

	m_uiControlPointNum = m_uiControlPointNum + 1;
	Vector3 * m_pControlPointTemp = NULL;
	m_pControlPointTemp = MEM_NEW Vector3[m_uiControlPointNum];
	VSMAC_ASSERT(m_pControlPointTemp);
	VSMemcpy(m_pControlPointTemp,m_pControlPoint,(m_uiControlPointNum - 1) * sizeof(Vector3));
	m_pControlPointTemp[m_uiControlPointNum - 1] = m_pControlPointTemp[0];
	SAFE_DELETEA(m_pControlPoint);
	m_pControlPoint = m_pControlPointTemp  ;
	return 1;
}
/*----------------------------------------------------------------*/
bool BSplineCurve::ChangeToPeriodicClosePoints(uint32 uiDegree)
{
	VSMAC_ASSERT(uiDegree && m_pControlPoint && m_uiControlPointNum);
	m_uiDegree = uiDegree;
	uint32 k = m_uiControlPointNum ;
	m_uiControlPointNum = m_uiControlPointNum + m_uiDegree;
	Vector3 * m_pControlPointTemp = NULL;
	m_pControlPointTemp	= MEM_NEW Vector3[m_uiControlPointNum];
	VSMAC_ASSERT(m_pControlPointTemp);
	VSMemcpy(m_pControlPointTemp,m_pControlPoint,k * sizeof(Vector3));
	for(uint32 i = 0 ; i < m_uiDegree ; i++)
	{
		m_pControlPointTemp[k + i] = m_pControlPointTemp[i];
	}
	SAFE_DELETEA(m_pControlPoint);
	m_pControlPoint = m_pControlPointTemp  ;
	return 1;

}
/*----------------------------------------------------------------*/
bool BSplineCurve::SetKnotVector(const float * pKnotVector,uint32 uiDegree,uint32 uiControlPointNum)
{
	VSMAC_ASSERT(pKnotVector);
	for(uint32 i = 0 ; i < uiDegree + uiControlPointNum; i++)
	{
		if(pKnotVector[i] > pKnotVector[i+1])
			return 0;
	}
	m_uiDegree = uiDegree;
	m_uiControlPointNum = uiControlPointNum;
	SAFE_DELETEA(m_pKnotVector);
	m_uiKnotNum = uiControlPointNum + 1 + uiDegree;
	m_pKnotVector = MEM_NEW float[m_uiKnotNum];
	VSMAC_ASSERT(m_pKnotVector);
	VSMemcpy(m_pKnotVector,pKnotVector,sizeof(float) * (m_uiKnotNum));

	SAFE_DELETEA(m_pBasis);
	m_uiBasisNum = (uiDegree + 1) * m_uiKnotNum;
	m_pBasis = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasis);


	SAFE_DELETEA(m_pBasisFD);
	m_pBasisFD = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasisFD);


	SAFE_DELETEA(m_pBasisSD);
	m_pBasisSD = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasisSD);



	SAFE_DELETEA(m_pBasisTD);
	m_pBasisTD = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasisTD);


	return 1;

}
/*----------------------------------------------------------------*/
bool BSplineCurve::CreateOpenUniformKnotVector(uint32 uiDegree,uint32 uiControlPointNum)
{
	m_uiDegree = uiDegree;
	m_uiControlPointNum = uiControlPointNum;
	SAFE_DELETEA(m_pKnotVector);
	m_uiKnotNum = uiControlPointNum + 1 + uiDegree;

	m_pKnotVector = MEM_NEW float[m_uiKnotNum];
	VSMAC_ASSERT(m_pKnotVector);
	for(uint32 i  = 0; i < m_uiKnotNum ; i++)
	{
		if(i >= 0 && i <= m_uiDegree)
		{
			m_pKnotVector[i] = 0;

		}
		else if( i >= m_uiDegree + 1 && i <= m_uiControlPointNum)
		{
			m_pKnotVector[i] =((float)1.0 * (i - m_uiDegree)) / (m_uiControlPointNum + 1 - m_uiDegree);
		
		}
		else
		{
			m_pKnotVector[i] = 1;
		}
	}

	SAFE_DELETEA(m_pBasis);
	m_uiBasisNum = (uiDegree + 1) * m_uiKnotNum;
	m_pBasis = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasis);



	SAFE_DELETEA(m_pBasisFD);
	m_pBasisFD = MEM_NEW float[m_uiBasisNum];
	VSMAC_ASSERT(m_pBasisFD);



	SAFE_DELETEA(m_pBasisSD);
	m_pBasisSD = MEM_NEW float[m_uiBasisNum];
	VSMAC_ASSERT(m_pBasisSD);



	SAFE_DELETEA(m_pBasisTD);
	m_pBasisTD = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasisTD);

	return 1;
}
/*----------------------------------------------------------------*/
bool BSplineCurve::CreatePeriodicKnotVector(uint32 uiDegree,uint32 uiControlPointNum)
{
	m_uiDegree = uiDegree;
	m_uiControlPointNum = uiControlPointNum;
	SAFE_DELETEA(m_pKnotVector);
	m_uiKnotNum = uiControlPointNum + 1 + uiDegree;
	m_pKnotVector = MEM_NEW float[m_uiKnotNum];
	VSMAC_ASSERT(m_pKnotVector);

	for(uint32 i  = 0; i < m_uiKnotNum ; i++)
	{
		
		m_pKnotVector[i] =((float)1.0 * (i - m_uiDegree)) / (m_uiControlPointNum + 1 - m_uiDegree);
		
	}

	SAFE_DELETEA(m_pBasis);
	m_uiBasisNum = (uiDegree + 1) * m_uiKnotNum;
	m_pBasis = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasis);


	SAFE_DELETEA(m_pBasisFD);
	m_pBasisFD = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasisFD);

	
	SAFE_DELETEA(m_pBasisSD);
	m_pBasisSD = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasisSD);


	SAFE_DELETEA(m_pBasisTD);
	m_pBasisTD = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasisTD);

	return 1;
}
/*----------------------------------------------------------------*/
int32 BSplineCurve::GetKey(float t)const
{

	VSMAC_ASSERT(t <= 1.0f && t >= 0.0f);

	uint32 i;
	for(i = 0 ; i < m_uiKnotNum ; i++)
	{
		if(t < m_pKnotVector[i])
			break;
	
	}
	return (i - 1);

}
/*----------------------------------------------------------------*/
bool BSplineCurve::GetFuncValue(float t,int32 &i,uint32 uiOrder)
{
	VSMAC_ASSERT(m_pBasis);

	i = GetKey(t);

	memset(m_pBasis,0,sizeof(float) * m_uiBasisNum);
	memset(m_pBasisFD,0,sizeof(float) * m_uiBasisNum);
	memset(m_pBasisSD,0,sizeof(float) * m_uiBasisNum);
	memset(m_pBasisTD,0,sizeof(float) * m_uiBasisNum);
	SetBasisValue(0,i,1.0f);

	
	
	float fN0 = t - m_pKnotVector[i];
	float fN1 = m_pKnotVector[i + 1] - t;
	float fInvD0, fInvD1;
	uint32 j;
	for (j = 1; j <= m_uiDegree; j++)
	{
		fInvD0 = ((float)1.0) / (m_pKnotVector[i + j] - m_pKnotVector[i]);
		fInvD1 = ((float)1.0) / (m_pKnotVector[i + 1] - m_pKnotVector[i - j + 1]);

		SetBasisValue(j,i,fN0 * GetBasisValue(j - 1,i) * fInvD0);
		SetBasisValue(j,i - j,fN1 * GetBasisValue(j - 1,i - j + 1) * fInvD1);

		if (uiOrder >= 1) 
		{
			SetBasisFDValue(j,i,(fN0 * GetBasisFDValue(j - 1,i) + GetBasisFDValue(j - 1,i)) * fInvD0);
			SetBasisFDValue(j,i - j, (fN1*GetBasisFDValue(j - 1,i - j + 1)-GetBasisFDValue(j - 1,i - j + 1)) * fInvD1);

			if (uiOrder >= 2)
			{
				SetBasisSDValue(j,i,(fN0 * GetBasisSDValue(j - 1,i) + ((float)2.0) * GetBasisSDValue(j - 1,i)) * fInvD0);
				SetBasisSDValue(j,i - j ,(fN1 * GetBasisSDValue(j - 1,i - j + 1) - ((float)2.0) * GetBasisSDValue(j - 1,i - j + 1)) * fInvD1);

				if (uiOrder >= 3)
				{
					SetBasisTDValue(j,i,(fN0 * GetBasisTDValue(j - 1,i) + ((float)3.0) * GetBasisTDValue(j - 1 , i)) * fInvD0);
					SetBasisTDValue(j,i - j,(fN1*GetBasisTDValue(j-1,i-j+1) -((float)3.0)*GetBasisTDValue(j-1,i-j+1))*fInvD1);
				}
			}
		}
	}

	for (j = 2; j <= m_uiDegree; j++)
	{
		for (int32 k = i - j + 1; k < i; k++)
		{
			fN0 = t - m_pKnotVector[k];
			fN1 = m_pKnotVector[k + j + 1]-t;
			fInvD0 = ((float)1.0) / (m_pKnotVector[k + j]  - m_pKnotVector[k]);
			fInvD1 = ((float)1.0) / (m_pKnotVector[k + j + 1] - m_pKnotVector[k +1 ]);

			SetBasisValue(j,k ,fN0 * GetBasisValue(j - 1,k) * fInvD0 + fN1 * GetBasisValue(j - 1,k + 1) * fInvD1);

			if (uiOrder >= 1)
			{
				SetBasisFDValue(j,k,(fN0 * GetBasisFDValue(j-1,k) + GetBasisFDValue(j-1,k) ) *
					fInvD0 + (fN1 * GetBasisFDValue(j-1,k+1) - GetBasisFDValue(j-1,k+1)) *
					fInvD1);

				if (uiOrder >= 2)
				{
					SetBasisSDValue(j,k,(fN0*GetBasisSDValue(j - 1,k) +
						((float)2.0)*GetBasisSDValue(j - 1,k))*fInvD0 +
						(fN1*GetBasisSDValue(j - 1,k + 1)- ((float)2.0)*
						GetBasisSDValue(j - 1,k + 1))*fInvD1);

					if (uiOrder >= 3)
					{
						SetBasisTDValue(j,k, (fN0*GetBasisTDValue(j-1,k) +
							((float)3.0)*GetBasisTDValue(j-1,k))*fInvD0 +
							(fN1*GetBasisTDValue(j-1,k+1) - ((float)3.0)*
							GetBasisTDValue(j-1,k+1))*fInvD1);
					}
				}
			}
		}
	}
	
	return 1;
}
/*----------------------------------------------------------------*/
Vector3 BSplineCurve::GetPoint(float t)
{
	int32 i;
	if(GetFuncValue(t,i,0))
	{
		Vector3 Point(0,0,0);
		for(int32 j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisValue(m_uiDegree,j);
		
		}
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}

}
/*----------------------------------------------------------------*/
Vector3 BSplineCurve::GetFirstDerivative(float t)
{
	int32 i;
	if(GetFuncValue(t,i,1))
	{
		Vector3 Point(0,0,0);
		for(int32 j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisFDValue(m_uiDegree,j);

		}
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}

}
/*----------------------------------------------------------------*/
Vector3 BSplineCurve::GetSecondDerivative(float t)
{
	int32 i;
	if(GetFuncValue(t,i,2))
	{
		Vector3 Point(0,0,0);
		for(int32 j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisSDValue(m_uiDegree,j);

		}
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
}
/*----------------------------------------------------------------*/
Vector3 BSplineCurve::GetThirdDerivative(float t)
{
	int32 i;
	if(GetFuncValue(t,i,3))
	{
		Vector3 Point(0,0,0);
		for(int32 j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisTDValue(m_uiDegree,j);

		}
		return Point;
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}

}
/*----------------------------------------------------------------*/
bool BSplineCurve::Copy(const BSplineCurve & B_SplineCurve3)
{
	if(!ControlCurve::Copy(B_SplineCurve3))
		return 0;
	VSMAC_ASSERT(B_SplineCurve3.m_pBasis && B_SplineCurve3.m_pBasisFD
		&& B_SplineCurve3.m_pBasisSD && B_SplineCurve3.m_pBasisTD &&
		B_SplineCurve3.m_pKnotVector && B_SplineCurve3.m_uiBasisNum
		&& B_SplineCurve3.m_uiDegree && B_SplineCurve3.m_uiKnotNum);

	m_uiDegree = B_SplineCurve3.m_uiDegree;
	m_uiControlPointNum = B_SplineCurve3.m_uiControlPointNum;
	m_uiKnotNum = B_SplineCurve3.m_uiKnotNum;

	SAFE_DELETEA(m_pKnotVector);
	SAFE_DELETEA(m_pControlPoint);
	SAFE_DELETEA(m_pBasis);
	SAFE_DELETEA(m_pBasisFD);
	SAFE_DELETEA(m_pBasisSD);
	SAFE_DELETEA(m_pBasisTD);
	
	m_pKnotVector = MEM_NEW float[m_uiKnotNum];
	VSMAC_ASSERT(m_pKnotVector);

	VSMemcpy(m_pKnotVector,B_SplineCurve3.m_pKnotVector,sizeof(float) * (m_uiKnotNum));

	SAFE_DELETEA(m_pBasis);
	m_uiBasisNum = B_SplineCurve3.m_uiBasisNum;
	m_pBasis = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasis);


	SAFE_DELETEA(m_pBasisFD);
	m_pBasisFD = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasisFD);

	SAFE_DELETEA(m_pBasisSD);
	m_pBasisSD = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasisSD);

	SAFE_DELETEA(m_pBasisTD);
	m_pBasisTD = MEM_NEW float[m_uiBasisNum]; 
	VSMAC_ASSERT(m_pBasisTD);

	return 1;
}

}