#pragma once


#include "math/function1.h"
#include "math/curve.h"


namespace zq{


//the function is sqrt(dy/dt * dy/dt + dx/dt * dx/dt);
class MATH_API CurveLengthFunction1:public Function1
{
public:
	CurveLengthFunction1();
	~CurveLengthFunction1();
	//得到值
	virtual float GetValue(float fParameter);
	//求导数 to do 
	virtual float GetFirstDerivative(float fParameter);
	virtual float GetSecondDerivative(float fParameter);
	virtual float GetThirdDerivative(float fParameter);
	//设置曲线
	inline void SetCurve(Curve *pCurve){if(!pCurve) return ; m_pCurve = pCurve;}
	inline Curve *GetCurve(){return m_pCurve;}
protected:
	Curve *m_pCurve;
};


}
