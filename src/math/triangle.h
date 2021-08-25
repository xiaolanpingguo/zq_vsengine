#pragma once


#include "math/vector3.h"
#include "math/plane.h"


/***************************************************************************************

				    fTriangleParameter[1]
		   m_V[0]	_________________________m_V[1]
				   \                       /
				    \                     /
				     \                   /
				      \                 /  
				       \               /
					    \             /
fTriangleParameter[2]  \           /  fTriangleParameter[0]
					      \         /
					       \       /
						     \    /
						      \  /
						       \/m_V[2]
***************************************************************************************/
namespace zq{


class OBB;
class MATH_API Triangle : public Plane
{
private:	
	Vector3 m_V[3];
public:
		
	Triangle();
	~Triangle();
	Triangle(const Vector3 & P0,const Vector3 &P1,const Vector3 &P2);
	Triangle(const Vector3 Point[3]);


    inline void Set(const Vector3 & P0, const Vector3 &P1, const Vector3 &P2)
    {
        m_V[0] = P0;
        m_V[2] = P2;
        m_V[1] = P1;
        Plane::Set(m_V);
    }

    inline void Set(const Vector3 Point[3])
    {
        m_V[0] = Point[0];
        m_V[2] = Point[2];
        m_V[1] = Point[1];
        Plane::Set(m_V);
    }

    inline void GetPoint(Vector3  Point[3])const
    {
        Point[0] = m_V[0];
        Point[1] = m_V[1];
        Point[2] = m_V[2];
    }

    inline Vector3 GetParameterPoint(float fTriangleParameter[3])const
    {
        return (m_V[0] * fTriangleParameter[0] + m_V[1] * fTriangleParameter[1] + m_V[2] * fTriangleParameter[2]);
    }

    inline Vector3 GetParameterPoint(float fTriangleParameter0, float fTriangleParameter1, float fTriangleParameter2)const
    {

        return (m_V[0] * fTriangleParameter0 + m_V[1] * fTriangleParameter1 + m_V[2] * fTriangleParameter2);
    }


	bool GetParameter(const Vector3 &Point,float fTriangleParameter[3])const;
	void Transform(const Triangle & Triangle,const Matrix3X3W &Mat); 

	//��������ξ���
	float SquaredDistance(const Vector3 & Point,float fTriangleParameter[3])const;
	//�����κ������ξ���
	float SquaredDistance(const Triangle &Triangle,
		float fTriangle1Parameter[3],
		float fTriangle2Parameter[3])const;
	//�����κ;��ξ���
	float SquaredDistance(const Rectangle &Rectangle,
						float fTriangleParameter[3],
						float fRectangleParameter[2])const;
		
	//ֱ�ߺ������ξ���
	float SquaredDistance(const Line &Line,float fTriangleParameter[3],float &fLineParameter)const;
	//���ߺ������ξ���
	float SquaredDistance(const Ray & Ray,float fTriangleParameter[3],float &fRayParameter)const;
	//�߶κ������ξ���
	float SquaredDistance(const Segment& Segment,float fTriangleParameter[3],float &fSegmentParameter)const;
	//�����κ�OBB����
	float SquaredDistance(const OBB &OBB,float TriangleParameter[3], float OBBParameter[3])const;
	//�����κ���ľ���
	float Distance(const Sphere &Sphere,float fTriangleParameter[3],Vector3 & SpherePoint)const;

	//ƽ��������ξ���
	float Distance(const Plane &Plane,Vector3 &TrianglePoint,Vector3 &PlanePoint)const;
	//�����κ�AABB����
	float SquaredDistance(const AABB &AABB ,float TriangleParameter[3]
							, float AABBParameter[3])const;
	//�����κͶ���ξ���
	float SquaredDistance(const Polygon& Polygon,
							float fTriangle1Parameter[3],
							int& IndexTriangle,
							float fTriangle2Parameter[3])const;
	/********************************RelationWith******************************************/
	//����ֱ����������λ�ù�ϵ bCullΪ�Ƿ�Ϊ�������,�Ƿ��ǳ���,t�����ཻ����
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Line &Line, bool bCull,float fTriangleParameter[3],
					float &fLineParameter)const;
	//����������������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Ray &Ray, bool bCull,float fTriangleParameter[3],
					float &fRayParameter)const;
	//�����߶���������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Segment &Segment, bool bCull,float fTriangleParameter[3],
					float &fSegmentParameter)const;
	//ƽ���������λ�ù�ϵ
	//MATH_VSON MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Plane & Plane)const;
	int32 RelationWith(const Plane & Plane,Segment & Segment)const;
	//�����κ�������λ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const Triangle & Triangle)const;
	int32 RelationWith(const Triangle & Triangle,Segment & Segment)const;
	//�����κ;���λ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const Rectangle & Rectangle)const;
	int32 RelationWith(const Rectangle & Rectangle,Segment & Segment)const;
	//�����κ�AABBλ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const AABB &AABB)const;

	//�����κ�OBBλ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const OBB &OBB)const;

	//�����κ�Բλ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Sphere &Sphere)const;
};


}