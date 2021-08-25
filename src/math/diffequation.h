#pragma once


#include "math/function1.h"


namespace zq{


class MATH_API DiffEquation
{
public:
	DiffEquation();
	~DiffEquation();
	//设置原函数
	inline void SetFunction1(Function1 * Func){if(!Func) return ; m_pFunc = Func;}
	inline Function1 *GetFunc(){return m_pFunc;}
    //设置步长
	inline void SetStep(float Step){if(ABS(Step) < EPSILON_E4) return; m_Step = Step;}
	inline float GetStep(){return m_Step;}
	//设置开始自变量
	inline void SetBeginT(float BeginT){m_BeginT = BeginT;}
	inline float GetBeginT(){return m_BeginT;}
	//设置开始值
	inline void SetBeginX(float BeginX){m_BeginX = BeginX;}
	inline float GetBeginX(){return m_BeginX;}
	//更新
	void Update();
protected:
	Function1 * m_pFunc;
	float m_Step;
	float m_BeginT;
	float m_BeginX;

};


}
