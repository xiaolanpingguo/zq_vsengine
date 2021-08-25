#pragma once


#include "math/maths.h"
#include "math/plane.h"
#include "math/obb.h"
#include "math/sphere.h"
#include "math/triangle.h"
#include "math/rectangle.h"


namespace zq{


class Ray;
class Segment;
class MATH_API Line
{
protected:
	Vector3	m_Orig;  // 源点
	Vector3	m_Dir;   // 方向
public:
	Line();
	Line(const Vector3 & Orig,const Vector3 &Dir);
	~Line();

    void Set(const Vector3 & Orig, const Vector3 &Dir)
    {
        m_Orig = Orig;
        m_Dir = Dir;
        m_Dir.Normalize();
    }

    inline const Vector3& GetOrig()const
    {
        return m_Orig;
    }

    inline const Vector3& GetDir()const
    {
        return m_Dir;
    }

    inline Vector3 GetParameterPoint(float fLineParameter)const
    {
        return  (m_Orig + m_Dir * fLineParameter);
    }


	void Transform(const Line &Line,const Matrix3X3W &Mat);
	bool GetParameter(const Vector3 &Point,float &fLineParameter )const;
		
		
	/********************************RelationWith******************************************/
	//测试直线与三角形位置关系 bCull为是否为背面剪裁,是否考虑朝向,t返回相交长度
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Triangle & Triangle, bool bCull,float &fLineParameter,
					float fTriangleParameter[3])const;
	//测试直线与平面位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Plane &Plane, bool bCull,float &fLineParameter)const;
	//测试直线与矩形位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Rectangle &Rectangle,bool bCull,float &fLineParameter,
					float fRectangleParameter[2])const;
	//测试直线与球位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Sphere &sphere, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试直线与OBB位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const OBB &OBB, uint32 &Quantity,float &tNear,float &tFar)const;
		

	//测试直线与AABB位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const AABB &AABB, uint32 &Quantity,float &tNear,float &tFar)const;

	//测试直线与多边形位置关系 bCull为是否为背面剪裁,是否考虑朝向,t返回相交长度
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Polygon &Polygon,float &fLineParameter, 
					bool bCull,int32 &iIndexTriangle,float fTriangleParameter[3])const;
	/*************************************距离************************************************/

	//点到直线距离
	float SquaredDistance(const Vector3 &Point,float &fLineParameter)const;
	//直线和直线的距离
	float SquaredDistance(const Line &Line,float &fLine1Parameter,float &fLine2Parameter)const;
	//直线和射线距离
	float SquaredDistance(const Ray &Ray,float &fLineParameter,float &fRayParameter)const;
	//直线和线段距离
	float SquaredDistance(const Segment & Segment,float &fLineParameter,float &fSegmentParameter)const;
	//直线和三角形距离
	float SquaredDistance(const Triangle& Triangle,float &fLineParameter,float fTriangleParameter[3])const;
	//直线和矩形距离
	float SquaredDistance(const Rectangle& Rectangle,float &fLineParameter,float fRectangleParameter[2])const;
	//直线和OBB距离
	float SquaredDistance(const OBB & OBB,float &fLineParameter,float fOBBParameter[3])const;
	//直线和球的距离
	float Distance(const Sphere &Sphere,float &fLineParameter,Vector3 & SpherePoint)const;
	//直线和平面距离
	float Distance(const Plane &Plane,Vector3 &LinePoint,Vector3 &PlanePoint)const;
	//直线和AABB距离
	float SquaredDistance(const AABB &AABB,float &fLineParameter, float fAABBParameter[3])const;

	//直线和多边形距离
	float SquaredDistance(const Polygon & Polygon,float &fLineParameter, int& IndexTriangle, float fTriangleParameter[3])const;
};

}