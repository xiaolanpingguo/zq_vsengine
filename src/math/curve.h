#pragma once


#include "math/vector3.h"
#include "math/matrix3X3.h"


namespace zq{


class MATH_API Curve : public MemObject
{
public:
	enum //CURVE TYPE
	{
		CT_SEGMENT,
		CT_CONTROL,
		CT_MAX
	};
	Curve();
	virtual ~Curve() = 0;

	virtual uint32 GetCurveType() = 0;
	//获得FrenetFrame3个方向
	virtual void GetFrenetFrameBNT(float t,Vector3 & B,Vector3 & N,Vector3 &T) = 0;
	//获得FixedUp3个方向
	void	GetFixedUpBNT(float t ,Vector3 & B,Vector3 & N,Vector3& T);
	//求导数
	virtual Vector3	GetFirstDerivative(float t)= 0 ;
	virtual Vector3	GetSecondDerivative(float t)= 0;
	virtual Vector3	GetThirdDerivative(float t)= 0;
	//求长度
	virtual float		GetLength(float t1 ,float t2,uint32 uiIterations)= 0;
	//求点
	virtual Vector3	GetPoint(float t) = 0;
	//求总长
	virtual float		GetTotalLength(uint32 uiIterations) = 0;
	//根据长度得到时间
	virtual float GetTime (float fLength, uint32 iIterations = 32,
			float fTolerance = EPSILON_E4) = 0;
	//求曲率
	virtual float GetCurvature(float fTime) = 0;
	//求转矩
	virtual float GetTorsion(float fTime) = 0;
};

}