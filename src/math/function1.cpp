#include "math/function1.h"
#include "core/memorymanager.h"
using namespace zq;
/*----------------------------------------------------------------*/
Function1::Function1()
{
	m_pNext = NULL;
	m_pHead = NULL;
	m_pRoot = NULL;
	m_uiRootNum = 0;
}
/*----------------------------------------------------------------*/
Function1::~Function1()
{
	if(m_pHead)
	{
	
		m_pNext = m_pHead;
		while(m_pNext)
		{
			INTERVAL *Temp = m_pNext;
			m_pNext = m_pNext->next;
			SAFE_DELETE(Temp);
		
		}
		m_pNext = NULL;
		m_pHead = NULL;	
	}
	SAFE_DELETEA(m_pRoot);
}
/*----------------------------------------------------------------*/
bool Function1::ISHaveRoot(float t1 , float t2,uint32 uiIterator)
{
	if(m_pHead)
	{

		m_pNext = m_pHead;
		while(m_pNext)
		{
			INTERVAL *Temp = m_pNext;
			m_pNext = m_pNext->next;
			SAFE_DELETE(Temp);

		}
		m_pNext = NULL;
		m_pHead = NULL;	
	}
	m_uiRootNum = 0;

	float Temp = ABS(t2 - t1) / uiIterator;
	float k1 = t1;
	float k2;
	float Value1 ,Value2;
	Value1 = Value2 = GetValue(k1);
	for(uint32 i = 0 ; i < uiIterator ; i++)
	{
		Value1 = Value2;
		k2 = k1 + Temp;
		Value2 = GetValue(k2);
		if(Value1 * Value2 < 0)
		{
			m_uiRootNum++;
			if(m_pHead == NULL)
			{
				m_pHead = MEM_NEW INTERVAL;
				if(!m_pHead)
					return 0;
				m_pNext = m_pHead;
				m_pNext->t1 = k1;
				m_pNext->t2 = k2;
				m_pNext->value1 = Value1;
				m_pNext->value2 = Value2;
				m_pNext->next = NULL;
				
			}
			else
			{
				m_pNext->next = MEM_NEW INTERVAL;
				if(!m_pNext->next)
					return 0;
				m_pNext = m_pNext->next;
				m_pNext->t1 = k1;
				m_pNext->t2 = k2;
				m_pNext->value1 = Value1;
				m_pNext->value2 = Value2;
				m_pNext->next = NULL;
				
			}
		}
		k1 = k2;
	
	}
	if(m_pHead)
	{
		SAFE_DELETEA(m_pRoot);
		m_pRoot = MEM_NEW float[m_uiRootNum];
		if(!m_pRoot)
			return 0;
		return 1;
	}
	else
		return 0;
}
/*----------------------------------------------------------------*/
void Function1::Root(float t1 ,float t2,float Value1,float Value2,uint32 uiLevel)
{
	
	float tm = (t2 + t1) / (float)2.0;

	float Valuem = GetValue(tm);
	if(uiLevel == 0)
	{
		m_pRoot[m_uiIndex] = tm;
		m_uiIndex++;
		return ;
	}
	if(Value1 * Valuem < 0.0)
	{
		Root(t1 ,tm,Value1,Valuem,uiLevel - 1);
	}
	else
	{
		Root(tm ,t2,Valuem,Value2,uiLevel - 1);
	}

}
/*----------------------------------------------------------------*/
bool Function1::ComputeRoot(float t1 , float t2,uint32 uiLevel)
{
	if(!m_pHead)
		return 0;
	m_uiIndex = 0;

	m_pNext = m_pHead;
	while(m_pNext)
	{
		Root(m_pNext->t1 ,m_pNext->t2,m_pNext->value1,m_pNext->value2,uiLevel);
		m_pNext = m_pNext->next;
		

	}
	return 1;

}
/*----------------------------------------------------------------*/
void Function1::GetExtremumValue(float t1 , float t2,float & Min,float &Max,uint32 uiLevel)
{
	float value1 = GetValue(t1);
	float value2 = GetValue(t2);
	ExtremumValue(t1 ,t2,value1,value2,Min,Max,uiLevel);

}
/*----------------------------------------------------------------*/
void Function1::ExtremumValue(float t1 , float t2,float Value1,float Value2,
								float & Min,float &Max,uint32 uiLevel)
{

	float tm = (t1 + t2) / (float)2.0;
	float valuem = GetValue(tm);
	
	
	if(uiLevel == 0)
	{
		float min,max;
		min = Value1;
		max = Value1;
		if(min > valuem)
		{
			min = valuem;
		}
		if(min > Value2)
		{
			min = Value2;
		}
		if(max < valuem)
		{
			max = valuem;
		}
		if(max < Value2)
		{
			max = Value2;
		}
		Min = min;
		Max = max;
	}
	else
	{
		float min1,max1;
		float min2,max2;
		ExtremumValue(t1,tm,Value1,valuem,min1,max1,uiLevel - 1);
		ExtremumValue(tm,t2,valuem,Value2,min2,max2,uiLevel - 1);

		if(min1 < min2)
		{
			Min = min1;
		}
		else
		{
			Min = min2;
		}
		if(max1 > max2)
		{
		
			Max = max1;		
		}
		else
		{
		
			Max = max2;
		}
	}

}
/*----------------------------------------------------------------*/
float Function1::Integration(float t1 , float t2,uint32 uiIterator)
{
	float tTemp = (t2 - t1) / uiIterator;
	float result = 0.0;
	float t1Temp = t1;
	float t2Temp = t1 + t1Temp;
	float t1TempValue = GetValue(t1Temp);
	float t2TempValue = t1TempValue;
	for(uint32 i = 0 ; i < uiIterator ; i++)
	{
		t2TempValue = GetValue(t2Temp);
		result += (float)0.5 * tTemp * (t1TempValue + t2TempValue);
		t1Temp = t2Temp;
		t2Temp += tTemp;
		t1TempValue = t2TempValue;
	
	}
	return result;
}
/*----------------------------------------------------------------*/