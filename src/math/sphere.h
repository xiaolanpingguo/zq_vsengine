#pragma once


#include "math/vector3.h"


namespace zq{


class MATH_API Sphere
{
public:
	Vector3 m_Center;		//中心
	float m_fRadius;		//半径
	Sphere();
	~Sphere();
	Sphere(const Vector3 & Center, float fRadius);

    inline void Set(const Vector3 & Center, float fRadius)
    {
        m_Center = Center;
        m_fRadius = fRadius;
    }


	//根绝点来建立包围球
	void CreateSphere(const Vector3 *pPointArray,uint32 uiPointNum);
	//结合包围球
	Sphere MergeSpheres(const Sphere &Sphere)const;
	//变换球体
	void Transform(const Sphere & Sphere,const Matrix3X3W &Mat); 
	AABB GetAABB()const;

	//点和球的距离
	float Distance(const Vector3 & Point,Vector3 & SpherePoint)const;
	//线和球的距离
	float Distance(const Line & Line,Vector3 & SpherePoint,float &fLineParameter)const;
	//射线和球的距离
	float Distance(const Ray & Ray,Vector3 & SpherePoint,float &fRayParameter)const;
	//线段和球的距离
	float Distance(const Segment & Segment,Vector3 & SpherePoint,float &fSegmentParameter)const;
	//OBB和球的距离
	float Distance(const OBB &OBB,Vector3 & SpherePoint,float fOBBParameter[3])const;
	//平面和球的距离
	float Distance(const Plane & Plane,Vector3 & SpherePoint)const;
	//矩形和球的距离
	float Distance(const Rectangle & Rectangle,Vector3 & SpherePoint,float fRectangleParameter[2])const;
	//三角形和球的距离
	float Distance(const Triangle Triangle,Vector3 & SpherePoint,float fTriangleParameter[3])const;
	//AABB和球的距离
	float Distance(const AABB &AABB,Vector3 & SpherePoint,float fAABBParameter[3])const;

	//多边形和球的距离
	float Distance(const Polygon &Polygon,Vector3 & SpherePoint,int& IndexTriangle,
					float fTriangleParameter[3])const;
	/********************************RelationWith******************************************/
	//点和球位置关系
	//MATH_IN MATH_OUT MATH_VSON	
	int32 RelationWith(const Vector3 &Point)const;
	//直线与球位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Line &Line, uint32 &Quantity,float &tNear,float &tFar)const;
	//射线与球位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Ray &Ray, uint32 &Quantity,float &tNear,float &tFar)const;
	//线段与球位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_IN
	int32 RelationWith(const Segment &Segment, uint32 &Quantity,float &tNear,float &tFar)const;
	//平面与球的位置关系
	//MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Plane &Plane)const;
	//三角形和圆位置关系
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Triangle Triangle)const;
	//矩形和圆位置关系
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Rectangle &Rectangle)const;

	//OBB和圆位置关系
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const OBB &OBB)const;
	//园和圆位置关系
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const Sphere &Sphere)const;
};


}