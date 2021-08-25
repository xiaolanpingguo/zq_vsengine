#pragma once


#include "math/vector3.h"
#include "math/triangle.h"
#include "math/plane.h"
#include "math/matrix3X3w.h"


/*****************************************************************************************


                               p1  _______________________________ P0
                                  /|                            /|
                                /  |                          /  |
							  /    |m_A[1]  |        m_A[2] /    |
						  p5/______|________|______/______/ p4   |
							|      |        |    /        |      |
							|  p2  |_ _  _ _|  / _ _ _ _ _|_ _ __| p3
							|     /         |/____________|      /
							|	 /	             m_A[0]    |    /      
							|  /                          |  /
						p6	|/____________________________|/ p7



Plane0 right Plane1 left Plane2 front Plane3 back Plane4 up Plane5 down
****************************************************************************************/
namespace zq{


class Matrix3X3;
class Line;
class Ray;
class Rectangle;
class Segment;
class MATH_API OBB
{
private:
	Vector3	m_A[3];		// 3个轴
	Vector3	m_Center;	// 中心点
	float		m_fA[3];	// 3个半轴的长度
public:	
		
		
	OBB();
	~OBB();
	OBB(const Vector3 A[3],float fA[3],const Vector3 & Center);
	OBB(const Vector3 &A0,const Vector3 &A1,const Vector3 &A2, float fA0,float fA1,float fA2, const Vector3 & Center);

    inline void Set(const Vector3 A[3], float fA[3], const Vector3 & Center)
    {
        m_A[0] = A[0];
        m_A[1] = A[1];
        m_A[2] = A[2];
        m_A[0].Normalize();
        m_A[1].Normalize();
        m_A[2].Normalize();
        m_fA[0] = ABS(fA[0]);
        m_fA[1] = ABS(fA[1]);
        m_fA[2] = ABS(fA[2]);
        m_Center = Center;

    }

    inline void Set(const Vector3 &A0, const Vector3 &A1, const Vector3 &A2, float fA0, float fA1, float fA2, const Vector3 & Center)
    {

        m_A[0] = A0;
        m_A[1] = A1;
        m_A[2] = A2;

        m_A[0].Normalize();
        m_A[1].Normalize();
        m_A[2].Normalize();

        m_fA[0] = ABS(fA0);
        m_fA[1] = ABS(fA1);
        m_fA[2] = ABS(fA2);
        m_Center = Center;
    }

    inline void GetA(Vector3 A[3])const
    {
        for (int32 i = 0; i < 3; i++)
        {
            A[i] = m_A[i];
        }
    }

    inline void GetfA(float fA[3])const
    {
        for (int32 i = 0; i < 3; i++)
        {
            fA[i] = m_fA[i];
        }
    }

    inline const Vector3& GetCenter()const
    {
        return m_Center;
    }

    inline Vector3 GetParameterPoint(float fOBBParameter[3])const
    {
        return (m_Center + m_A[0] * fOBBParameter[0] + m_A[1] * fOBBParameter[1] + m_A[2] * fOBBParameter[2]);
    }

    inline Vector3 GetParameterPoint(float fOBBParameter0, float fOBBParameter1, float fOBBParameter2)const
    {
        return (m_Center + m_A[0] * fOBBParameter0 + m_A[1] * fOBBParameter1 + m_A[2] * fOBBParameter2);
    }

	//通过点集合构造AABB
	void CreateOBB(const Vector3 * const pPointArray,uint32 uiPointNum);
	//合并AABB
	OBB MergeOBB(const OBB &OBB)const;
	//取得6个平面
	void GetPlane(Plane pPlanes[6])const;
	//取得8个点
	void GetPoint(Vector3 Point[8])const;
	//取得6个矩形
	void GetRectangle(Rectangle Rectangle[6])const;
	//获得变换到OBB下变换矩阵，物体必须和OBB在同一个坐标系下
	void GetTransform(Matrix3X3W &m)const;
	//得到点参数
	bool GetParameter(const Vector3 &Point,float fOBBParameter[3])const;
	//用给定的OBB和变换矩阵来构造OBB
	void Transform(const OBB &OBB, 
		const Matrix3X3W &Mat);
	AABB GetAABB()const;
		 
	//点和OBB距离
	float SquaredDistance(const Vector3 & Point,float fOBBParameter[3])const;
	//直线和OBB距离
	float SquaredDistance(const Line & Line, float fOBBParameter[3],float &fLineParameter)const;
	//射线和OBB距离
	float SquaredDistance(const Ray &Ray,float fOBBParameter[3],float &fRayParameter)const;
	//线段和OBB距离
	float SquaredDistance(const Segment & Segment,float fOBBParameter[3],float &fSegmentParameter)const;

	//三角形和OBB距离
	float SquaredDistance(const Triangle & Triangle , float OBBParameter[3],
							float TriangleParameter[3])const;

	//矩形和OBB距离
	float SquaredDistance(const Rectangle& Rectangle , float OBBParameter[3],
							float RectangleParameter[2])const;
	//OBB和OBB距离
	float SquaredDistance(const OBB &OBB , float OBB1Parameter[3],
							float OBB2Parameter[3])const;

	//OBB和球的距离
	float Distance(const Sphere & Sphere,float fOBBParameter[3],Vector3 & SpherePoint)const;

	//OBB和平面距离
	float Distance(const Plane &Plane,Vector3 &OBBPoint,Vector3 &PlanePoint)const;

	//多边形和OBB距离
	float SquaredDistance(const Polygon &Polygon, 
						float OBBParameter[3],int& IndexTriangle,
						float TriangleParameter[3])const;
	/********************************RelationWith******************************************/
	//点和OBB位置关系
	//MATH_IN MATH_OUT MATH_VSON
	int32 RelationWith(const Vector3 &Point)const;
	//测试直线与OBB位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Line &Line, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试射线与OBB位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Ray &Ray, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试线段与OBB位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_IN
	int32 RelationWith(const Segment &Segment, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试平面和OBB位置关系
	//MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Plane &Plane)const;
	//测试三角形与OBB位置关系
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Triangle & Triangle)const;    

	//矩形和OBB位置关系
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Rectangle &Rectangle)const;

	//OBB和OBB位置关系
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const OBB &OBB)const;

	//OBB和圆位置关系
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const Sphere &Sphere)const;
};


}