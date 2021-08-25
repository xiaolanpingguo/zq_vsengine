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
	Vector3	    m_N;       //ƽ�淨����
	float		m_fD;       // ��ԭ��ľ���(ax+by+cz+d=0)
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
	/*************************************����************************************************/
	//�㵽ƽ�����
	float Distance(const Vector3 &Point,Vector3 &PlanePoint)const;
	//ƽ�����ľ���
	float Distance(const Sphere &Sphere,Vector3 & SpherePoint)const;
	//ֱ�ߺ�ƽ�����
	float Distance(const Line &Line,Vector3 &PlanePoint,Vector3 &LinePoint)const;
	//���ߺ�ƽ�����
	float Distance(const Ray & Ray,Vector3 &PlanePoint,Vector3 &RayPoint)const;
	//�߶κ�ƽ�����
	float Distance(const Segment & Segment,Vector3 &PlanePoint,Vector3 &SegmentPoint)const;

	//������뺯����û�п����ཻ������¼���ģ���Ϊ�ཻ������¾���Ϊ0������RelationWith�ж�λ�ù�ϵ�������������������
	//ƽ���ƽ�����
	float Distance(const Plane &Plane,Vector3 &Plane1Point,Vector3 &Plane2Point)const;
	//ƽ��������ξ���
	float Distance(const Triangle &Triangle,Vector3 &PlanePoint,Vector3 &TrianglePoint)const;
	//���κ�ƽ�����
	float Distance(const Rectangle &Rectangle,Vector3 &PlanePoint,Vector3 &RectanglePoint)const;
	//OBB��ƽ�����
	float Distance(const OBB& OBB,Vector3 &PlanePoint,Vector3 &OBBPoint)const;
	//AABB��ƽ�����
	float Distance(const AABB &AABB,Vector3 &PlanePoint,Vector3 &AABBPoint)const;
	//ƽ��Ͷ���ξ���
	float Distance(const Polygon &Polygon,Vector3 &PlanePoint,int& IndexTriangle,
					Vector3 &TrianglePoint)const;
	/********************************RelationWith******************************************/
	//���ƽ���λ�ù�ϵ(MATH_FRONT MATH_BACK VSPLANAR)
	int32 RelationWith(const Vector3 &Point)const;
	//����ֱ����ƽ��λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Line &Line, bool bCull,float &fLineParameter)const;
	//����������ƽ��λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Ray &Ray, bool bCull,float &fRayParameter)const;
	//�����߶���ƽ��λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT MATH_VSON MATH_BACK MATH_FRONT
	int32 RelationWith(const Segment &Segment, bool bCull,float &fSegmentParameter)const;
	//����ƽ���OBBλ�ù�ϵ
	//MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const OBB &OBB)const;
	//����ƽ���AABBλ�ù�ϵ
	//MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const AABB &AABB)const;
	//ƽ�������λ�ù�ϵ
	//MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Sphere &Sphere)const;
	//ƽ���������λ�ù�ϵ
	//MATH_VSON MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Triangle &Triangle)const;
	int32 RelationWith(const Triangle &Triangle ,Segment & Segment)const;
	//������Ϊ����ƽ���ƽ��λ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT 
	int32 RelationWith(const Plane &Plane)const;
	int32 RelationWith(const Plane &Plane,Line &Line)const;
	//ƽ��;���λ�ù�ϵ
	//MATH_VSON MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Rectangle & Rectangle)const;		
	int32 RelationWith(const Rectangle &Rectangle,Segment &Segment)const;

	//ƽ��Ͷ����λ�ù�ϵ
	//MATH_VSON MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Polygon &Polygon)const;
	int32 RelationWith(const Polygon &Polygon,Segment & Segment)const;

	//ƽ���Բ��λ�ù�ϵ
	int32 RelationWith(const Cylinder &Cylinder3)const;

};


}