#pragma once


#include "math/curve.h"
#include "math/surface.h"


namespace zq{


class MATH_API TubeSurface :public Surface
{
public:
	TubeSurface();
	~TubeSurface();
	//设置曲线，按照U,V方向的长度
	bool SetCurve(Curve * pCurve,float fN,float fB);	
	virtual Vector3 GetPoint(float U,float V);
	virtual Vector3 GetUPartialDerivative(float U,float V);
	virtual Vector3 GetVPartialDerivative(float U,float V);
private:
	Curve *m_pCurve;
	float m_fN;
	float m_fB;

};


}