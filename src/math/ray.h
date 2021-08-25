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
	//测试射线与三角形是否位置关系 bCull为是否为背面剪裁,是否考虑朝向,t返回相交长度
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Triangle & Triangle, bool bCull,float &fRayParameter,
					float fTriangleParameter[3])const;
	//测试射线与平面位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Plane &Plane, bool bCull,float &fRayParameter)const;
	//测试射线与矩形位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Rectangle &Rectangle,bool bCull,float &fRayParameter,
					float fRectangleParameter[2])const;

	//测试射线与OBB位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const OBB &OBB, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试直线与AABB位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const AABB &AABB, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试射线与球位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Sphere &sphere, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试射线与多边形位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Polygon &Polygon,float &fRayParameter ,
					bool bCull,int32 &iIndexTriangle,float fTriangleParameter[3])const;
	/*************************************距离************************************************/
	//点和射线距离
	float SquaredDistance(const Vector3 &Point,float &fLineParameter)const;
	//直线和射线距离
	float SquaredDistance(const Line &Line,float &fRayParameter,float &fLineParameter)const;
	//射线和射线距离
	float SquaredDistance(const Ray &Ray,float &fRay1Parameter,float &fRay2Parameter)const;
	//射线和线段距离
	float SquaredDistance(const Segment & Segment,float &fRayParameter,float &fSegmentParameter)const;
	//射线和三角形距离
	float SquaredDistance(const Triangle& Triangle,float &fRayParameter,float fTriangleParameter[3])const;
	//射线和矩形距离
	float SquaredDistance(const Rectangle& Rectangle,float &fRayParameter,float fRectangleParameter[2])const;
	//射线和OBB距离
	float SquaredDistance(const OBB & OBB,float &fRayParameter,float fOBBParameter[3])const;
	//射线和球的距离
	float Distance(const Sphere &Sphere,float &fRayParameter,Vector3 & SpherePoint)const;
	//射线和平面距离
	float Distance(const Plane & Plane,Vector3 &RayPoint,Vector3 &PlanePoint)const;
	//射线和AABB距离
	float SquaredDistance(const AABB &AABB,float &fRayParameter, float fAABBParameter[3])const;
	//射线和多边形距离
	float SquaredDistance(const Polygon &Polygon,float &fRayParameter,
							int& IndexTriangle,
							float fTriangleParameter[3])const;
};

}