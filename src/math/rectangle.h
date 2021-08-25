#pragma once


#include "math/vector3.h"
#include "math/plane.h"
#include "math/matrix3X3w.h"

/********************************************************************************************


__________________
|        |        |  
|    A[0]|        |      
|		  |        |      
|        |_A[1]___|
|                 |
|                 |
|_________________|


左手定则 法向量 为 cross (A[0],A[1]),规定可见 ，方向朝向屏幕外， 
********************************************************************************************/
namespace zq{


class Triangle;
class MATH_API Rectangle:public Plane
{
public:	
	Rectangle();
	~Rectangle();
	Rectangle(const Vector3 & Center,const Vector3 &A0,const Vector3 & A1,float fA0, float fA1);
	Rectangle(const Vector3 & Center,const Vector3 A[2],const float fA[2]);


    inline void Set(const Vector3 & Center, const Vector3 &A0, const Vector3 & A1, float fA0, float fA1)
    {
        m_Center = Center;
        m_A[0] = A0;
        m_A[1] = A1;
        m_A[0].Normalize();
        m_A[1].Normalize();
        m_fA[0] = ABS(fA0);
        m_fA[1] = ABS(fA1);
        Vector3 N;
        N.Cross(A0, A1);
        Plane::Set(N, Center);
    }

    inline void Set(const Vector3 & Center, const Vector3 A[2], const float fA[2])
    {
        Set(Center, A[0], A[1], fA[0], fA[1]);
    }

    inline void GetA(Vector3 A[2])const
    {
        A[0] = m_A[0];
        A[1] = m_A[1];
    }

    inline void GetfA(float fA[2])const
    {
        fA[0] = m_fA[0];
        fA[1] = m_fA[1];
    }

    inline const Vector3& GetCenter()const
    {
        return m_Center;
    }

    inline Vector3 GetParameterPoint(float fRectangleParameter[2])const
    {
        return (m_Center + m_A[0] * fRectangleParameter[0] + m_A[1] * fRectangleParameter[1]);
    }

    inline Vector3 GetParameterPoint(float fRectangleParameter0, float fRectangleParameter1)const
    {
        return (m_Center + m_A[0] * fRectangleParameter0 + m_A[1] * fRectangleParameter1);
    }

    void GetPoint(Vector3 Point[4]) const
    {
        Vector3 temp0 = m_Center + m_A[0] * m_fA[0];
        Vector3 temp1 = m_Center + m_A[0] * (-m_fA[0]);
        Point[0] = temp0 + m_A[1] * m_fA[1];
        Point[1] = temp1 + m_A[1] * m_fA[1];
        Point[2] = temp1 + m_A[1] * (-m_fA[1]);
        Point[3] = temp0 + m_A[1] * (-m_fA[1]);
    }


	bool GetParameter(const Vector3 &Point,float fRectangleParameter[2])const;
	void Transform(const Rectangle &Rectangle, 
				const Matrix3X3W &Mat);

	//点和矩形距离
	float SquaredDistance(const Vector3 &Point,float fRectangleParameter[2])const;

	//矩形和矩形距离
	float SquaredDistance(const Rectangle &Rectangle,
							float fRectangle1Parameter[2],float fRectangle2Parameter[2])const;

	//三角形和矩形距离
	float SquaredDistance(const Triangle &Triangle,float fRectangleParameter[2],float fTriangleParameter[3])const;

	//直线和矩形距离
	float SquaredDistance(const Line& Line,float fRectangleParameter[2],float &fLineParameter)const;
	//射线和矩形距离
	float SquaredDistance(const Ray &Ray,float fRectangleParameter[2],float &fRayParameter)const;
	//线段和矩形距离
	float SquaredDistance(const Segment & Segment,float fRectangleParameter[2],float &fSegmentParameter)const;
	//矩形和OBB距离
	float SquaredDistance(const OBB &OBB,float RectangleParameter[2], float OBBParameter[3])const;
	//矩形和球的距离
	float Distance(const Sphere &Sphere,float fRectangleParameter[2],Vector3 & SpherePoint)const;
	//矩形和平面距离
	float Distance(const Plane &Plane,Vector3 &RectanglePoint,Vector3 &PlanePoint)const;
	//矩形和AABB距离
	float SquaredDistance(const AABB &AABB ,float RectangleParameter[2]
							, float AABBParameter[3])const;

	//多边形和矩形距离
	float SquaredDistance(const Polygon &Polygon,
								float fRectangleParameter[2],
								int& IndexTriangle,
								float fTriangleParameter[3])const;
	/********************************RelationWith******************************************/
	//测试直线与矩形位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Line &Line,bool bCull,float fRectangleParameter[2],float &fLineParameter)const;
	//测试射线与矩形位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Ray &Ray,bool bCull,float fRectangleParameter[2],float &fRayParameter)const;
	//测试线段与矩形位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Segment &Segment,bool bCull,float fRectangleParameter[2],float &fSegmentParameter)const;
	//平面和矩形位置关系
	//MATH_VSON MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Plane &Plane)const;
	int32 RelationWith(const Plane &Plane,Segment &Segment)const;
	//三角形和矩形位置关系
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const Triangle &Triangle)const;
	int32 RelationWith(const Triangle &Triangle,Segment & Segment)const;
	//矩形和矩形位置关系
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const Rectangle &Rectangle)const;
	int32 RelationWith(const Rectangle &Rectangle,Segment & Segment)const;
	//矩形和AABB位置关系
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const AABB &AABB)const;

	//矩形和OBB位置关系
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const OBB &OBB)const;

	//矩形和圆位置关系
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Sphere & Sphere)const;

private:
    Vector3 m_A[2];
    float m_fA[2];
    Vector3 m_Center;
};

}
