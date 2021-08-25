#pragma once


#include "math/maths.h"


namespace zq{


class Matrix3X3;
class Quat;
class Ray;
class Line;
class Plane;
class Ray;
class Segment;
class Triangle;
class Rectangle;
class OBB;
class Sphere;
class AABB;
class Matrix3X3W;
class Polygon;
class  MATH_API Vector3 
{
public:
	union
	{
		float m[3];
		struct  
		{
			float x, y, z;
		};
	};

	Vector3();
	Vector3(float _x, float _y, float _z);


    inline void Set(float _x, float _y, float _z)
    {
        x = _x; y = _y; z = _z;
    }

    inline float GetLength(void)const
    {
        return SQRT(x*x + y * y + z * z);
    }

    inline float Vector3::GetSqrLength(void) const
    {
        return (x*x + y * y + z * z);
    }

    inline void Negate(void)
    {
        x = -x;  y = -y;  z = -z;
    }

    inline void Normalize(void)
    {

        float f = x * x + y * y + z * z;
        if (f > EPSILON_E4)
        {
#ifdef VS_SSE
            f = InvSqrt(f);
#else
            f = (float)1.0f / SQRT(f);
#endif
            x *= f; y *= f; z *= f;
        }
        else
        {
            *this = Vector3::ms_Zero;
        }
    }

    inline void Cross(const Vector3 &v1, const Vector3 &v2)
    {
        x = v1.y * v2.z - v1.z * v2.y;
        y = v1.z * v2.x - v1.x * v2.z;
        z = v1.x * v2.y - v1.y * v2.x;
    }


    inline float AngleWith(Vector3 &v)
    {
        return (float)acos(((*this).Dot(v)) / (this->GetLength()*v.GetLength()));
    }

    inline void  Create(const Vector3 &v1, const Vector3 &v2)
    {
        x = v2.x - v1.x;
        y = v2.y - v1.y;
        z = v2.z - v1.z;
    }


	/*            N     _
				\	  /|\   /|
		Dir	  \	   |   /  Reflect		 
		        _\|  |  /
	--------------------------
	*/
	Vector3 ReflectDir(const Vector3 &N)const;

	void operator += (const Vector3 &v);
	void operator -= (const Vector3 &v);
	void operator *= (float f);
	void operator /= (float f);
	void operator += (float f);
	void operator -= (float f);	
	bool operator ==(const Vector3 &v)const;
	Vector3 operator * (float f)const;            
	Vector3 operator / (float f)const;            
	Vector3 operator + (float f)const;            
	Vector3 operator - (float f)const;            
	Quat   operator * (const Quat   &q)const; 
	Vector3 operator * (const Matrix3X3 &m)const; 
	Vector3 operator * (const Matrix3X3W &m)const;
	Vector3 operator + (const Vector3 &v)const; 
	Vector3 operator - (const Vector3 &v)const; 
	Vector3 operator / (const Vector3 &v)const;
	Vector3 operator * (const Vector3 &v)const;
	void operator /= (const Vector3 &v);
	void operator *= (const Vector3 &v);

    float Dot(const Vector3 &v)const;
	bool IsParallel(const Vector3 & Vector)const;

	//点和点距离
	float SquaredDistance(const Vector3 &Point)const;
	//点到直线距离
	float SquaredDistance(const Line & Line,float &fLineParameter)const;
	//点和射线距离
	float SquaredDistance(const Ray & Ray,float &fRayParameter)const;
	//点和线段距离
	float SquaredDistance(const Segment & Segment,float &fSegmentParameter)const;
	//点和三角形距离
	float SquaredDistance(const Triangle &Triangle,float fTriangleParameter[3])const;
	//点和矩形距离
	float SquaredDistance(const Rectangle &Rectangle,float fRectangleParameter[2])const;
	//点和OBB距离
	float SquaredDistance(const OBB &OBB,float fOBBParameter[3])const;
	//点和球的距离
	float Distance(const Sphere& Sphere,Vector3 & SpherePoint)const;
	//点和平面距离
	float Distance(const Plane & Plane,Vector3 &PlanePoint)const;
	//点和AABB距离
	float SquaredDistance(const AABB &AABB,float fAABBParameter[3])const;
	//点和多边形距离
	float SquaredDistance(const Polygon & Polygon , int& IndexTriangle,float fTriangleParameter[3])const;
	/********************************RelationWith******************************************/
	//点和球位置关系
	//MATH_IN MATH_OUT MATH_VSON	
	int32 RelationWith(const Sphere & Sphere)const;
	//点和平面位置关系
	//MATH_FRONT MATH_BACK MATH_VSON
	int32 RelationWith(const Plane &Plane)const;
	//点和OBB位置关系
	//MATH_IN MATH_OUT MATH_VSON
	int32 RelationWith(const OBB &OBB)const; 
	//点和AABB位置关系
	//MATH_IN MATH_OUT MATH_VSON
	int32 RelationWith(const AABB &AABB)const; 
	const static Vector3 ms_Up;
	const static Vector3 ms_Right;
	const static Vector3 ms_Front;
	const static Vector3 ms_Zero;
	const static Vector3 ms_One;
};

}