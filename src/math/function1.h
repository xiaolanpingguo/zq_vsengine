#pragma once


#include "math/maths.h"


namespace zq{


class MATH_API Function1 : public MemObject
{
public:
	Function1();
	virtual ~Function1() = 0;
	//��ֵ
	virtual float GetValue(float fParameter) = 0 ;
	//����
	virtual float GetFirstDerivative(float fParameter) = 0;
	virtual float GetSecondDerivative(float fParameter) = 0;
	virtual float GetThirdDerivative(float fParameter) = 0;
	//�����
	float Integration(float t1 , float t2,uint32 uiIterator);
	//��ֵ
	void GetExtremumValue(float t1 , float t2,float & Min,float &Max,uint32 uiLevel);
	//�Ƿ��и�
	bool ISHaveRoot(float t1 , float t2,uint32 uiIterator);
	//�����
	bool ComputeRoot(float t1 , float t2,uint32 uiLevel);

protected:
	struct INTERVAL
	{
		float t1;
		float t2;
		float value1;
		float value2;
		INTERVAL * next;
	};

	INTERVAL  *m_pHead;
	INTERVAL  *m_pNext;
	float	*m_pRoot;
	uint32 m_uiRootNum;
	uint32 m_uiIndex;
	void Root(float t1 ,float t2,float Value1,float Value2,uint32 uiLevel);
	void ExtremumValue(float t1 , float t2,float Value1,float Value2,float & Min,float &Max,uint32 uiLevel);
};


}