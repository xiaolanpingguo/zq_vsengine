#pragma once


#include "math/vector3.h"
#include "math/matrix3X3.h"


namespace zq{


class Line;
class Plane;
class Sphere;
class Cylinder;
class MATH_API Plane 
{
protected:
	Vector3	    m_N;       //平面法向量
	float		m_fD;       // 到原点的距离(ax+by+cz+d=0)
public:
	Plane();
	Plane(const Vector3 &N, const Vector3 &P);
	Plane(const Vector3 &P0,  const Vector3 &P1, const Vector3 &P2);
	Plane(const Vector3 Point[3]);
	Plane(const Vector3 &N , float fD);


    inline const Vector3& GetN()const
    {
        return m_N;
    }

    inline Vector3 GetPoint()const
    {
        if (ABS(m_N.z) < EPSILON_E4)
        {
            return Vector3(0.0f, 0.0f, 0.0f);
        }

        return Vector3(0.0f, 0.0f, -m_fD / m_N.z);
    }

    inline float GetfD()const
    {
        return m_fD;
    }

    inline void Set(const Vector3 &N, const Vector3 &P)
    {
        m_N = N;
        m_N.Normalize();
        m_fD = -(m_N.Dot(P));
    }

    inline void Set(const Vector3 &P0, const Vector3 &P1, const Vector3 &P2)
    {
        Vector3 vcEdge1 = P1 - P0;
        Vector3 vcEdge2 = P2 - P0;

        m_N.Cross(vcEdge1, vcEdge2);
        m_N.Normalize();
        m_fD = -(m_N.Dot(P0));
    }

    inline void Set(const Vector3 Point[3])
    {
        Set(Point[0], Point[1], Point[2]);
    }

    inline Plane GetPlane()const
    {
        return *this;
    }

    inline void Set(const Vector3 &N, float fD)
    {
        float Len = N.GetLength();
        m_N = N / Len;

        m_fD = fD / Len;
    }


	void Transform(const Plane &Plane,const Matrix3X3W &Mat);
	Vector3 ReflectDir(const Vector3 & Dir)const;
	void  GetReverse(Plane &OutPlane)const;
	/*************************************距离************************************************/
	//点到平面距离
	float Distance(const Vector3 &Point,Vector3 &PlanePoint)const;
	//平面和球的距离
	float Distance(const Sphere &Sphere,Vector3 & SpherePoint)const;
	//直线和平面距离
	float Distance(const Line &Line,Vector3 &PlanePoint,Vector3 &LinePoint)const;
	//射线和平面距离
	float Distance(const Ray & Ray,Vector3 &PlanePoint,Vector3 &RayPoint)const;
	//线段和平面距离
	float Distance(const Segment & Segment,Vector3 &PlanePoint,Vector3 &SegmentPoint)const;

	//下面距离函数再没有考虑相交的情况下计算的，因为相交的情况下举例为0，先用RelationWith判断位置关系，再用下面来计算距离
	//平面和平面距离
	float Distance(const Plane &Plane,Vector3 &Plane1Point,Vector3 &Plane2Point)const;
	//平面和三角形距离
	float Distance(const Triangle &Triangle,Vector3 &PlanePoint,Vector3 &TrianglePoint)const;
	//矩形和平面距离
	float Distance(const Rectangle &Rectangle,Vector3 &PlanePoint,Vector3 &RectanglePoint)const;
	//OBB和平面距离
	float Distance(const OBB& OBB,Vector3 &PlanePoint,Vector3 &OBBPoint)const;
	//AABB和平面距离
	float Distance(const AABB &AABB,Vector3 &PlanePoint,Vector3 &AABBPoint)const;
	//平面和多边形距离
	float Distance(const Polygon &Polygon,Vector3 &PlanePoint,int& IndexTriangle,
					Vector3 &TrianglePoint)const;
	/********************************RelationWith******************************************/
	//点和平面的位置关系(MATH_FRONT MATH_BACK VSPLANAR)
	int32 RelationWith(const Vector3 &Point)const;
	//测试直线与平面位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Line &Line, bool bCull,float &fLineParameter)const;
	//测试射线与平面位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Ray &Ray, bool bCull,float &fRayParameter)const;
	//测试线段与平面位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Segment &Segment, bool bCull,float &fSegmentParameter)const;
	//测试平面和OBB位置关系
	//MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const OBB &OBB)const;
	//测试平面和AABB位置关系
	//MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const AABB &AABB)const;
	//平面与球的位置关系
	//MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Sphere &Sphere)const;
	//平面和三角形位置关系
	//MATH_VSON MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Triangle &Triangle)const;
	int32 RelationWith(const Triangle &Triangle ,Segment & Segment)const;
	//测试做为参数平面和平面位置关系
	//MATH_NOINTERSECT MATH_INTERSECT 
	int32 RelationWith(const Plane &Plane)const;
	int32 RelationWith(const Plane &Plane,Line &Line)const;
	//平面和矩形位置关系
	//MATH_VSON MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Rectangle & Rectangle)const;		
	int32 RelationWith(const Rectangle &Rectangle,Segment &Segment)const;

	//平面和多边形位置关系
	//MATH_VSON MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Polygon &Polygon)const;
	int32 RelationWith(const Polygon &Polygon,Segment & Segment)const;

	//平面和圆柱位置关系
	int32 RelationWith(const Cylinder &Cylinder3)const;

};


}