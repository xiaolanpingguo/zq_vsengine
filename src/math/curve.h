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
	//���FrenetFrame3������
	virtual void GetFrenetFrameBNT(float t,Vector3 & B,Vector3 & N,Vector3 &T) = 0;
	//���FixedUp3������
	void	GetFixedUpBNT(float t ,Vector3 & B,Vector3 & N,Vector3& T);
	//����
	virtual Vector3	GetFirstDerivative(float t)= 0 ;
	virtual Vector3	GetSecondDerivative(float t)= 0;
	virtual Vector3	GetThirdDerivative(float t)= 0;
	//�󳤶�
	virtual float		GetLength(float t1 ,float t2,uint32 uiIterations)= 0;
	//���
	virtual Vector3	GetPoint(float t) = 0;
	//���ܳ�
	virtual float		GetTotalLength(uint32 uiIterations) = 0;
	//���ݳ��ȵõ�ʱ��
	virtual float GetTime (float fLength, uint32 iIterations = 32,
			float fTolerance = EPSILON_E4) = 0;
	//������
	virtual float GetCurvature(float fTime) = 0;
	//��ת��
	virtual float GetTorsion(float fTime) = 0;
};

}