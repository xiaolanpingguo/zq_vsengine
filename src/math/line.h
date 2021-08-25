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
	Vector3	m_Orig;  // Դ��
	Vector3	m_Dir;   // ����
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
	//����ֱ����������λ�ù�ϵ bCullΪ�Ƿ�Ϊ�������,�Ƿ��ǳ���,t�����ཻ����
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Triangle & Triangle, bool bCull,float &fLineParameter,
					float fTriangleParameter[3])const;
	//����ֱ����ƽ��λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Plane &Plane, bool bCull,float &fLineParameter)const;
	//����ֱ�������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Rectangle &Rectangle,bool bCull,float &fLineParameter,
					float fRectangleParameter[2])const;
	//����ֱ������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Sphere &sphere, uint32 &Quantity,float &tNear,float &tFar)const;
	//����ֱ����OBBλ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const OBB &OBB, uint32 &Quantity,float &tNear,float &tFar)const;
		

	//����ֱ����AABBλ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const AABB &AABB, uint32 &Quantity,float &tNear,float &tFar)const;

	//����ֱ��������λ�ù�ϵ bCullΪ�Ƿ�Ϊ�������,�Ƿ��ǳ���,t�����ཻ����
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Polygon &Polygon,float &fLineParameter, 
					bool bCull,int32 &iIndexTriangle,float fTriangleParameter[3])const;
	/*************************************����************************************************/

	//�㵽ֱ�߾���
	float SquaredDistance(const Vector3 &Point,float &fLineParameter)const;
	//ֱ�ߺ�ֱ�ߵľ���
	float SquaredDistance(const Line &Line,float &fLine1Parameter,float &fLine2Parameter)const;
	//ֱ�ߺ����߾���
	float SquaredDistance(const Ray &Ray,float &fLineParameter,float &fRayParameter)const;
	//ֱ�ߺ��߶ξ���
	float SquaredDistance(const Segment & Segment,float &fLineParameter,float &fSegmentParameter)const;
	//ֱ�ߺ������ξ���
	float SquaredDistance(const Triangle& Triangle,float &fLineParameter,float fTriangleParameter[3])const;
	//ֱ�ߺ;��ξ���
	float SquaredDistance(const Rectangle& Rectangle,float &fLineParameter,float fRectangleParameter[2])const;
	//ֱ�ߺ�OBB����
	float SquaredDistance(const OBB & OBB,float &fLineParameter,float fOBBParameter[3])const;
	//ֱ�ߺ���ľ���
	float Distance(const Sphere &Sphere,float &fLineParameter,Vector3 & SpherePoint)const;
	//ֱ�ߺ�ƽ�����
	float Distance(const Plane &Plane,Vector3 &LinePoint,Vector3 &PlanePoint)const;
	//ֱ�ߺ�AABB����
	float SquaredDistance(const AABB &AABB,float &fLineParameter, float fAABBParameter[3])const;

	//ֱ�ߺͶ���ξ���
	float SquaredDistance(const Polygon & Polygon,float &fLineParameter, int& IndexTriangle, float fTriangleParameter[3])const;
};

}