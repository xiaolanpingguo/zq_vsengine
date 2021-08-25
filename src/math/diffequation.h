#pragma once


#include "math/function1.h"


namespace zq{


class MATH_API DiffEquation
{
public:
	DiffEquation();
	~DiffEquation();
	//����ԭ����
	inline void SetFunction1(Function1 * Func){if(!Func) return ; m_pFunc = Func;}
	inline Function1 *GetFunc(){return m_pFunc;}
    //���ò���
	inline void SetStep(float Step){if(ABS(Step) < EPSILON_E4) return; m_Step = Step;}
	inline float GetStep(){return m_Step;}
	//���ÿ�ʼ�Ա���
	inline void SetBeginT(float BeginT){m_BeginT = BeginT;}
	inline float GetBeginT(){return m_BeginT;}
	//���ÿ�ʼֵ
	inline void SetBeginX(float BeginX){m_BeginX = BeginX;}
	inline float GetBeginX(){return m_BeginX;}
	//����
	void Update();
protected:
	Function1 * m_pFunc;
	float m_Step;
	float m_BeginT;
	float m_BeginX;

};


}
