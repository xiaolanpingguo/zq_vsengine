#pragma once


#include "math/function1.h"
#include "math/surface.h"


namespace zq{


//the function is sqrt(dy/dt * dy/dt + dx/dt * dx/dt);
class MATH_API SurfaceLengthFunction1:public Function1
{
public:
	enum // length type
	{
		LT_U_LEN,
		LT_V_LEN
	};
	SurfaceLengthFunction1(uint32 uiLengthType,float fTypePara);
	~SurfaceLengthFunction1();
	//得到值
	virtual float GetValue(float fParameter);
	//求导数 to do 
	virtual float GetFirstDerivative(float fParameter);
	virtual float GetSecondDerivative(float fParameter);
	virtual float GetThirdDerivative(float fParameter);
	//设置曲线
	inline void SetSurface(Surface *pSurface){if(!pSurface) return ; m_pSurface = pSurface;}
	inline Surface *GetSurface(){return m_pSurface;}
protected:
	Surface *m_pSurface;
	float m_fTypePara;
	uint32 m_uiLengthType;
};


}