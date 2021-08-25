#pragma once


#include "math/vector3.h"


namespace zq{


class MATH_API Sphere
{
public:
	Vector3 m_Center;		//����
	float m_fRadius;		//�뾶
	Sphere();
	~Sphere();
	Sphere(const Vector3 & Center, float fRadius);

    inline void Set(const Vector3 & Center, float fRadius)
    {
        m_Center = Center;
        m_fRadius = fRadius;
    }


	//��������������Χ��
	void CreateSphere(const Vector3 *pPointArray,uint32 uiPointNum);
	//��ϰ�Χ��
	Sphere MergeSpheres(const Sphere &Sphere)const;
	//�任����
	void Transform(const Sphere & Sphere,const Matrix3X3W &Mat); 
	AABB GetAABB()const;

	//�����ľ���
	float Distance(const Vector3 & Point,Vector3 & SpherePoint)const;
	//�ߺ���ľ���
	float Distance(const Line & Line,Vector3 & SpherePoint,float &fLineParameter)const;
	//���ߺ���ľ���
	float Distance(const Ray & Ray,Vector3 & SpherePoint,float &fRayParameter)const;
	//�߶κ���ľ���
	float Distance(const Segment & Segment,Vector3 & SpherePoint,float &fSegmentParameter)const;
	//OBB����ľ���
	float Distance(const OBB &OBB,Vector3 & SpherePoint,float fOBBParameter[3])const;
	//ƽ�����ľ���
	float Distance(const Plane & Plane,Vector3 & SpherePoint)const;
	//���κ���ľ���
	float Distance(const Rectangle & Rectangle,Vector3 & SpherePoint,float fRectangleParameter[2])const;
	//�����κ���ľ���
	float Distance(const Triangle Triangle,Vector3 & SpherePoint,float fTriangleParameter[3])const;
	//AABB����ľ���
	float Distance(const AABB &AABB,Vector3 & SpherePoint,float fAABBParameter[3])const;

	//����κ���ľ���
	float Distance(const Polygon &Polygon,Vector3 & SpherePoint,int& IndexTriangle,
					float fTriangleParameter[3])const;
	/********************************RelationWith******************************************/
	//�����λ�ù�ϵ
	//MATH_IN MATH_OUT MATH_VSON	
	int32 RelationWith(const Vector3 &Point)const;
	//ֱ������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Line &Line, uint32 &Quantity,float &tNear,float &tFar)const;
	//��������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Ray &Ray, uint32 &Quantity,float &tNear,float &tFar)const;
	//�߶�����λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT MATH_IN
	int32 RelationWith(const Segment &Segment, uint32 &Quantity,float &tNear,float &tFar)const;
	//ƽ�������λ�ù�ϵ
	//MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Plane &Plane)const;
	//�����κ�Բλ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Triangle Triangle)const;
	//���κ�Բλ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Rectangle &Rectangle)const;

	//OBB��Բλ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const OBB &OBB)const;
	//԰��Բλ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const Sphere &Sphere)const;
};


}