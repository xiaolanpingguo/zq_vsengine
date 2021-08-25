#pragma once


#include "math/curve.h"
#include "math/surface.h"


namespace zq{


class MATH_API GeneralizedCylinderSurface :public Surface
{
public:

	GeneralizedCylinderSurface();
	~GeneralizedCylinderSurface();
	bool SetCurve(Curve * pCurve1,Curve *pCurve2);
	virtual Vector3 GetPoint(float U,float V);
	virtual Vector3 GetUPartialDerivative(float U,float V);
	virtual Vector3 GetVPartialDerivative(float U,float V);
private:
	Curve *m_pCurve1;
	Curve *m_pCurve2;
	
};


}