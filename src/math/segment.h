#pragma once


#include "math/ray.h"
#include "math/matrix3X3w.h"


namespace zq{


class MATH_API Segment:public Ray
{
public:
	Segment();
	~Segment();
	Segment(const Vector3 &Orig,const Vector3 &End);
	Segment(const Vector3 &Orig,const Vector3 &Dir,float fLen);


    inline const Vector3& GetEnd()const
    {
        return m_End;
    }

    inline float GetLen()const
    {
        return m_fLen;
    }

    inline void Set(const Vector3 &Orig, const Vector3 &End)
    {
        Vector3 Dir = End - Orig;
        m_fLen = Dir.GetLength();
        Line::Set(Orig, Dir);
    }

    inline void Set(const Vector3 &Orig, const Vector3 &Dir, float fLen)
    {
        Line::Set(Orig, Dir);
        m_fLen = fLen;
        m_End = Orig + (m_Dir * m_fLen);
    }

    inline Vector3 GetParameterPoint(float fSegmentParameter)const
    {
        if (fSegmentParameter > m_fLen)
            fSegmentParameter = m_fLen;

        return Ray::GetParameterPoint(fSegmentParameter);
    }


	bool GetParameter(const Vector3 &Point,float &fSegmentParameter )const;
	//用矩阵变换射线
	void Transform(const Segment & Segment,const Matrix3X3W &Mat); 

	//测试线段与三角形位置关系 bCull为是否为背面剪裁,是否考虑朝向,t返回相交长度
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Triangle & Triangle, bool bCull,float &fSegmentParameter,
					float fTriangleParameter[3])const;
	//测试线段与平面位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Plane &Plane, bool bCull,float &fSegmentParameter)const;
	//测试线段与矩形位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Rectangle &Rectangle,bool bCull,float &fSegmentParameter,
					float fRectangleParameter[2])const;



	//测试线段与OBB位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_IN
	int32 RelationWith(const OBB &OBB, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试线段与AABB位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_IN
	int32 RelationWith(const AABB &AABB, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试线段与球位置关系
	//MATH_NOINTERSECT VSNTERSECT MATH_IN
	int32 RelationWith(const Sphere &sphere, uint32 &Quantity,float &tNear,float &tFar)const;
	//测试线段与多边形位置关系
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Polygon &Polygon,float &fSegmentParameter,
					bool bCull,int32 &iIndexTriangle,float fTriangleParameter[3])const;

	/*************************************距离************************************************/
	//点和线段距离
	float SquaredDistance(const Vector3 &Point,float &fSegmentParameter)const;
	//直线和线段距离
	float SquaredDistance(const Line &Line,float &fSegmentParameter,float &fLineParameter)const;
	//射线和线段距离
	float SquaredDistance(const Ray &Ray,float &fSegmentParameter,float &fRayParameter)const;
	//线段和线段距离
	float SquaredDistance(const Segment & Segment,float &fSegment1Parameter,float &fSegment2Parameter)const;
	//线段和三角形距离
	float SquaredDistance(const Triangle& Triangle,float &fSegmentParameter,float fTriangleParameter[3])const;
	//线段和矩形距离
	float SquaredDistance(const Rectangle& Rectangle,float &fSegmentParameter,float fRectangleParameter[2])const;
	//线段和OBB距离
	float SquaredDistance(const OBB & OBB,float &fSegmentParameter,float fOBBParameter[3])const;

	//线段和球的距离
	float Distance(const Sphere &Sphere,float &fSegmentParameter,Vector3 & SpherePoint)const;

	//线段和平面距离
	float Distance(const Plane & Plane,Vector3 &SegmentPoint,Vector3 &PlanePoint)const;

	//线段和AABB距离
	float SquaredDistance(const AABB &AABB,float &fSegmentParameter,float fAABBParameter[3])const;

	//线段和多边形距离
	float SquaredDistance(const Polygon &Polygon,float &fSegmentParameter,
							int& IndexTriangle,
							float fTriangleParameter[3])const;

private:
    Vector3	m_End;
    float		m_fLen;
};

}