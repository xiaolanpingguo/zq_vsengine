#include "math/curveset.h"
#include "core/memorymanager.h"
#include "math/matrix3X3w.h"


namespace zq{


CurveSet::CurveSet()
{
	m_uiNum = 0;
	m_pHead = NULL;
	m_pNext = NULL;
}

CurveSet::~CurveSet()
{
	Clear();	

}

void CurveSet::Clear()
{
	m_pNext = m_pHead;
	while(m_pNext)
	{
		ELEMENT * Temp = m_pNext;
		m_pNext = m_pNext->next;
		SAFE_DELETE(Temp);
	}
	m_uiNum = 0;
}
/*----------------------------------------------------------------*/
bool CurveSet::Set(Curve * pCurve)
{
	if(!pCurve)
		return 0;

	ELEMENT *Temp = NULL;
	Temp = MEM_NEW ELEMENT;
	VSMAC_ASSERT(Temp);

	Temp->Ptr = pCurve;
	Temp->next = NULL;

	if(!m_pHead)
	{
		m_pHead = Temp;
		m_pNext = m_pHead;

	
	}
	else
	{
		m_pNext->next = Temp;
		m_pNext = m_pNext->next;
	
	}
	m_uiNum++;
	return 1;
}
/*----------------------------------------------------------------*/
uint32 CurveSet::GetIndex(float &t)
{
	uint32 i = 0;
	while(t > (float)1.0)
	{
		t = t - (float)1.0;
		i++;
	}
	return i;
}
/*----------------------------------------------------------------*/
bool CurveSet::GetElement(uint32 Index,Curve * &Element)
{
	VSMAC_ASSERT(Index < m_uiNum);

	m_pNext = m_pHead;
	uint32 temp = 0;
	while (temp != Index)
	{
		m_pNext = m_pNext->next;
		temp++;
	}
	
	Element = m_pNext->Ptr;
	return 1;
}
/*----------------------------------------------------------------*/
bool CurveSet::GetElement(float& t,Curve * &Element)
{
	VSMAC_ASSERT(t <= m_uiNum * 1.0);

	m_pNext = m_pHead;

	while (t > (float)1.0)
	{
		m_pNext = m_pNext->next;
		t = t - (float)1.0;
	}

	Element = m_pNext->Ptr;
	return 1;
}
/*----------------------------------------------------------------*/
bool CurveSet::GetFixedUpBNT(float t ,Vector3 & B,Vector3 & N,Vector3 &T)
{
	Curve *Element = NULL;
	if(!GetElement(t,Element))
		return 0;
	VSMAC_ASSERT(Element);		
	Element->GetFixedUpBNT(t,B,N,T);
	return 1;
}
/*----------------------------------------------------------------*/
bool CurveSet::GetFrenetFrameBNT(float t,Vector3 & B,Vector3 & N,Vector3 &T)
{
	Curve *Element = NULL;
	if(!GetElement(t,Element))
		return 0;
	VSMAC_ASSERT(Element);

	Element->GetFrenetFrameBNT(t,B,N,T);
	return 1;

}
/*----------------------------------------------------------------*/
Vector3	CurveSet::GetFirstDerivative(float t)
{
	Curve *Element = NULL;
	if(!GetElement(t,Element))
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	VSMAC_ASSERT(Element);
	return Element->GetFirstDerivative(t);
}
/*----------------------------------------------------------------*/
Vector3	CurveSet::GetSecondDerivative(float t)
{
	Curve *Element = NULL;

	if(!GetElement(t,Element))
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	VSMAC_ASSERT(Element);
	return Element->GetSecondDerivative(t);
}
/*----------------------------------------------------------------*/
Vector3	CurveSet::GetThirdDerivative(float t)
{
	Curve *Element = NULL;
	if(!GetElement(t,Element))
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	VSMAC_ASSERT(Element);
	return Element->GetThirdDerivative(t);
}
/*----------------------------------------------------------------*/
float		CurveSet::GetLength(float t1 ,float t2,uint32 uiIterations)
{

	VSMAC_ASSERT(t1 <= t2);

	VSMAC_ASSERT(t2 <= m_uiNum * 1.0);


	uint32 index1 = GetIndex(t1);
	uint32 index2 = GetIndex(t2);

	t1 = t1 - index1 * (float)1.0;
	t2 = t2 - index2 * (float)1.0;
	if(index1 == index2)
	{
		Curve *Element = NULL;

		GetElement(index1,Element);
	
		return Element->GetLength(t1,t2,uiIterations);
		
		
	}
	else
	{
		Curve *Element = NULL;
		float Len = 0.0;
		GetElement(index1,Element);

		Len +=Element->GetLength(t1,1.0,uiIterations);

		index1++;
		while(index1 != index2)
		{
			
			GetElement(index1,Element);
		
			Len +=Element->GetTotalLength(uiIterations);
			
			index1++;
		}
		GetElement(index1,Element);
	
		Len +=Element->GetLength(0.0,t2,uiIterations);
	
		return Len;
	}


	
	
	
}
/*----------------------------------------------------------------*/
Vector3	CurveSet::GetPoint(float t)
{
	Curve *Element = NULL;
	uint32 Type = 0;
	if(!GetElement(t,Element))
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);

	VSMAC_ASSERT(Element);
	return Element->GetPoint(t);

}
/*----------------------------------------------------------------*/
float CurveSet::GetTotalLength(uint32 uiIterations)
{
	float Len = 0.0;
	m_pNext = m_pHead;
	while(m_pNext)
	{
		

		Len += m_pNext->Ptr->GetTotalLength(uiIterations);

		
		m_pNext = m_pNext->next;
		
	}

	return Len;
}

}