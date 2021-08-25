#pragma once


#include "math/surface.h"
#include "math/segmentcurve.h"


namespace zq{


class MATH_API CylinderSurface:public Surface
{
public:

	CylinderSurface();
	~CylinderSurface();
	bool SetCurve(Curve * pCurve1,SegmentCurve *pCurve2);
	virtual Vector3 GetPoint(float U,float V);
	virtual Vector3 GetUPartialDerivative(float U,float V);
	virtual Vector3 GetVPartialDerivative(float U,float V);
private:
	Curve *m_pCurve1;
	SegmentCurve *m_pCurve2;
};


}
