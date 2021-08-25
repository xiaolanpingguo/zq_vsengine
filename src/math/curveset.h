#pragma once


#include "math/curve.h"
#include "math/segment.h"


namespace zq{


class MATH_API CurveSet : public MemObject
{
public:
	CurveSet();
	~CurveSet();
	void Clear();
	bool Set(Curve * pCurve);
	bool	GetFrenetFrameBNT(float t,Vector3 & B,Vector3 & N,Vector3 &T);
	bool	GetFixedUpBNT(float t ,Vector3 & B,Vector3 & N,Vector3& T);
	Vector3	GetFirstDerivative(float t);
	Vector3	GetSecondDerivative(float t);
	Vector3	GetThirdDerivative(float t);
	float		GetLength(float t1 ,float t2,uint32 uiIterations);
	Vector3	GetPoint(float t);
	float		GetTotalLength(uint32 uiIterations);
	float GetTime (float fLength, int32 iIterations = 32,
					float fTolerance = EPSILON_E4);
	uint32 GetIndex(float &t);
	bool GetElement(uint32 Index,Curve * &Element );
	bool GetElement(float &t,Curve * &Element);

	inline uint32 GetCurveNum()const{return m_uiNum;}
protected:
	struct ELEMENT
	{
		Curve * Ptr;
		ELEMENT * next;
	};
	ELEMENT * m_pHead;
	ELEMENT * m_pNext;
	uint32 m_uiNum;
};


}
