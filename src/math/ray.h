#pragma once


#include "math/line.h"


namespace zq{


class  MATH_API Ray :public Line
{
public:

	Ray();
	Ray(const Vector3 & Orig,const Vector3 &Dir);
	~Ray();

    inline Vector3 GetParameterPoint(float fRayParameter)const
    {
        if (fRayParameter < 0)
            fRayParameter = 0;
        return Line::GetParameterPoint(fRayParameter);
    }

	bool GetParameter(const Vector3 &Point,float &fRayParameter )const;
	void Transform(const Ray &Ray,const Matrix3X3W &Mat);

	/********************************RelationWith******************************************/
	//�����������������Ƿ�λ�ù�ϵ bCullΪ�Ƿ�Ϊ�������,�Ƿ��ǳ���,t�����ཻ����
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Triangle & Triangle, bool bCull,float &fRayParameter,
					float fTriangleParameter[3])const;
	//����������ƽ��λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Plane &Plane, bool bCull,float &fRayParameter)const;
	//�������������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Rectangle &Rectangle,bool bCull,float &fRayParameter,
					float fRectangleParameter[2])const;

	//����������OBBλ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const OBB &OBB, uint32 &Quantity,float &tNear,float &tFar)const;
	//����ֱ����AABBλ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const AABB &AABB, uint32 &Quantity,float &tNear,float &tFar)const;
	//������������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Sphere &sphere, uint32 &Quantity,float &tNear,float &tFar)const;
	//��������������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Polygon &Polygon,float &fRayParameter ,
					bool bCull,int32 &iIndexTriangle,float fTriangleParameter[3])const;
	/*************************************����************************************************/
	//������߾���
	float SquaredDistance(const Vector3 &Point,float &fLineParameter)const;
	//ֱ�ߺ����߾���
	float SquaredDistance(const Line &Line,float &fRayParameter,float &fLineParameter)const;
	//���ߺ����߾���
	float SquaredDistance(const Ray &Ray,float &fRay1Parameter,float &fRay2Parameter)const;
	//���ߺ��߶ξ���
	float SquaredDistance(const Segment & Segment,float &fRayParameter,float &fSegmentParameter)const;
	//���ߺ������ξ���
	float SquaredDistance(const Triangle& Triangle,float &fRayParameter,float fTriangleParameter[3])const;
	//���ߺ;��ξ���
	float SquaredDistance(const Rectangle& Rectangle,float &fRayParameter,float fRectangleParameter[2])const;
	//���ߺ�OBB����
	float SquaredDistance(const OBB & OBB,float &fRayParameter,float fOBBParameter[3])const;
	//���ߺ���ľ���
	float Distance(const Sphere &Sphere,float &fRayParameter,Vector3 & SpherePoint)const;
	//���ߺ�ƽ�����
	float Distance(const Plane & Plane,Vector3 &RayPoint,Vector3 &PlanePoint)const;
	//���ߺ�AABB����
	float SquaredDistance(const AABB &AABB,float &fRayParameter, float fAABBParameter[3])const;
	//���ߺͶ���ξ���
	float SquaredDistance(const Polygon &Polygon,float &fRayParameter,
							int& IndexTriangle,
							float fTriangleParameter[3])const;
};

}