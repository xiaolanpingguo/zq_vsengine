#pragma once


#include "math/vector3.h"
#include "math/plane.h"
#include "math/rectangle.h"


namespace zq{


class MATH_API AABB
{
private:
	static const Vector3	m_A[3];			// 3个轴				
	Vector3	m_Center;					// 中心点
	float		m_fA[3];					// 3个半轴的长度
	Vector3	m_Max;						//最大点
	Vector3	m_Min;						//最小点
public:
		
	AABB();
	~AABB();
    //通过最大点和最小点构造AABB
    AABB(const Vector3 & Max, const Vector3 & Min);
    //通过中心点和3个轴的半长度构造AABB
    AABB(const Vector3 & Center, float fA0, float fA1, float fA2);
    AABB(const Vector3 & Center, float fA[3]);

    inline void Set(const Vector3 & Max, const Vector3 & Min)
    {
        m_Max = Max;
        m_Min = Min;
        m_Center = (Max + Min) / 2.0f;
        Vector3 Temp = (Max - Min) / 2.0f;
        for (int32 i = 0; i < 3; i++)
            m_fA[i] = Temp.m[i];

    }

    inline void Set(const Vector3 & Center, float fA0, float fA1, float fA2)
    {
        m_fA[0] = ABS(fA0);
        m_fA[1] = ABS(fA1);
        m_fA[2] = ABS(fA2);

        m_Center = Center;

        m_Max.Set(m_Center.x + m_fA[0], m_Center.y + m_fA[1], m_Center.z + m_fA[2]);
        m_Min.Set(m_Center.x - m_fA[0], m_Center.y - m_fA[1], m_Center.z - m_fA[2]);
    }

    inline void Set(const Vector3 & Center, float fA[3])
    {
        m_fA[0] = ABS(fA[0]);
        m_fA[1] = ABS(fA[1]);
        m_fA[2] = ABS(fA[2]);

        m_Center = Center;

        m_Max.Set(m_Center.x + m_fA[0], m_Center.y + m_fA[1], m_Center.z + m_fA[2]);
        m_Min.Set(m_Center.x - m_fA[0], m_Center.y - m_fA[1], m_Center.z - m_fA[2]);

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

    inline Vector3 GetParameterPoint(float fAABBParameter[3])const
    {
        return Vector3(m_Center.x + fAABBParameter[0], m_Center.y + fAABBParameter[1], m_Center.z + fAABBParameter[2]);
    }

    inline Vector3 GetParameterPoint(float fAABBParameter0, float fAABBParameter1, float fAABBParameter2)const
    {
        return Vector3(m_Center.x + fAABBParameter0, m_Center.y + fAABBParameter1, m_Center.z + fAABBParameter2);
    }

    inline const Vector3& GetMaxPoint()const
    {
        return m_Max;
    }

    inline const Vector3& GetMinPoint()const
    {
        return m_Min;
    }

	//通过点集合构造AABB
	void CreateAABB(const Vector3 * const pPointArray, uint32 uiPointNum);
	/*********************************** inline *************************************/
	//得到AABB6个平面
	void GetPlane(Plane pPlanes[6])const;
	//得到AABB8个点
	void GetPoint(Vector3 Point[8])const;
	//得到AABB6个矩形
	void GetRectangle(Rectangle Rectangle[6])const;
	//给定AABB内一点返回AABB的参数
	bool GetParameter(const Vector3 &Point,float fAABBParameter[3])const;
	//用矩阵变换AABB
	void Transform(const AABB &aabb, const Matrix3X3W &m,bool bHasProject = false);
	//合并2个AABB
	AABB MergeAABB(const AABB &aabb)const;
	void GetQuadAABB(AABB aabb[4])const;
	void GetOctAABB(AABB aabb[8])const;
	AABB GetMin(const AABB& aabb) const;

	bool GetIntersect(AABB& aabb, AABB& OutAABB)const;
	/*************************************距离************************************************/
	//点和AABB距离
	float SquaredDistance(const Vector3 & Point,float fAABBParameter[3])const;
	//直线和AABB距离
	float SquaredDistance(const Line & Line, float fAABBParameter[3],float &fLineParameter)const;
	//射线和AABB距离
	float SquaredDistance(const Ray &Ray,float fAABBParameter[3],float &fRayParameter)const;
	//线段和AABB距离
	float SquaredDistance(const Segment & Segment,float fAABBParameter[3],float &fSegmentParameter)const;

	//三角形和AABB距离
	float SquaredDistance(const Triangle & Triangle , float AABBParameter[3], float TriangleParameter[3])const;

	//矩形和AABB距离
	float SquaredDistance(const Rectangle& Rectangle , float AABBParameter[3], float RectangleParameter[2])const;
	//AABB和AABB距离
	float SquaredDistance(const AABB &AABB , float AABB1Parameter[3], float AABB2Parameter[3])const;
	//AABB和球的距离
	float Distance(const Sphere & Sphere,float fAABBParameter[3],Vector3 & SpherePoint)const;

	//AABB和平面距离
	float Distance(const Plane &Plane,Vector3 &AABBPoint,Vector3 &PlanePoint)const;

	//多边形和AABB距离
	float SquaredDistance(const Polygon &Polygon,float AABBParameter[3],int& IndexTriangle, float TriangleParameter[3])const;
	/********************************RelationWith******************************************/
	//点和AABB位置关系
	//MATH_IN MATH_OUT MATH_VSON
	int32 RelationWith(const Vector3 &Point)const;
	//测试直线与AABB位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Line &Line, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试射线与AABB位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Ray &Ray, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试线段与AABB位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_IN
	int32 RelationWith(const Segment &Segment, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试平面和AABB位置关系
	//MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Plane &Plane)const;
	//三角形和AABB位置关系
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Triangle &Triangle)const;
	//矩形和AABB位置关系
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Rectangle &Rectangle)const;

	//AABB和AABB位置关系
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const AABB &aabb)const;

	//AABB和圆位置关系
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const Sphere &Sphere)const;
};

};
