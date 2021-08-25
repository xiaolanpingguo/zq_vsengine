#pragma once


#include "math/curve.h"
#include "math/surface.h"


namespace zq{


class MATH_API RevolutionSurface :public Surface
{
public:
	enum{XAXIS,YAXIS,ZAXIS};
	enum{XYPLANE,XZPLANE,YZPLANE};
	RevolutionSurface();
	~RevolutionSurface();
	//设置曲线，旋转轴，所在平面
	bool SetCurve(Curve * pCurve,uint32 Axis,uint32 uiPlane);	
	virtual Vector3 GetPoint(float U,float V);
	virtual Vector3 GetUPartialDerivative(float U,float V);
	virtual Vector3 GetVPartialDerivative(float U,float V);
private:
	Curve *m_pCurve;
	uint32 m_uiAxis;
	uint32 m_uiPlane;
};

}