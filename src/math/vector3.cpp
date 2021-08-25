#include "math/vector3.h"
#include "math/matrix3X3.h"
#include "math/matrix3X3w.h"
#include "math/quat.h"
#include "math/line.h"
#include "math/ray.h"
#include "math/segment.h"
#include "math/triangle.h"
#include "math/rectangle.h"
#include "math/obb.h"
#include "math/sphere.h"
#include "math/aabb.h"
#include "math/polygon.h"


namespace zq{


const Vector3 Vector3::ms_Up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::ms_Right = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::ms_Front = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::ms_Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::ms_One = Vector3(1.0f, 1.0f, 1.0f);

Vector3::Vector3(void) 
{
	x=0;y=0; z=0; 
}

Vector3::Vector3(float _x, float _y, float _z)
{
	x=_x; y=_y; z=_z;
}

void Vector3::operator += (const Vector3 &v)
{
	x += v.x;   y += v.y;   z += v.z;
}

Vector3 Vector3::operator + (const Vector3 &v) const 
{
	return Vector3(x+v.x, y+v.y, z+v.z);
}

void Vector3::operator -= (const Vector3 &v)
{
	x -= v.x;   y -= v.y;   z -= v.z;
}

Vector3 Vector3::operator - (const Vector3 &v) const 
{
	return Vector3(x-v.x, y-v.y, z-v.z);
}

Vector3 Vector3::operator / (const Vector3 &v)const
{
	return Vector3(x / v.x, y / v.y, z / v.z);
}

void Vector3::operator /= (const Vector3 &v)
{
	x /= v.x; y /= v.y; z /= v.z;
}

Vector3 Vector3::operator* (const Vector3 &v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

void Vector3::operator *= (const Vector3 &v)
{
	x *= v.x; y *= v.y, z *= v.z;
}

void Vector3::operator *= (float f)
{
	x *= f;   y *= f;   z *= f;
}

void Vector3::operator /= (float f)
{
	x /= f;   y /= f;   z /= f;
}

Vector3 Vector3::operator * (float f) const 
{
	return Vector3(x*f, y*f, z*f);
}

Vector3 Vector3::operator / (float f) const 
{
	return Vector3(x/f, y/f, z/f);
}

void Vector3::operator += (float f)
{
	x += f;   y += f;   z += f;
}

void Vector3::operator -= (float f)
{
	x -= f;   y -= f;   z -= f;
}

Vector3 Vector3::operator + (float f) const 
{
	return Vector3(x+f, y+f, z+f);
}

Vector3 Vector3::operator - (float f) const 
{
	return Vector3(x-f, y-f, z-f);
}

float Vector3::Dot(const Vector3 &v)const
{
	return (v.x*x + v.y*y + v.z*z);
}

bool Vector3::operator ==(const Vector3 &v)const
{
	for (uint32 i = 0 ; i < 3 ; i++)
	{
		if (ABS(m[i] - v.m[i]) > EPSILON_E4)
		{
			return false;
		}
	}
	return true;
// 	return (ABS(x-v.x) < EPSILON_E4 && 
// 			ABS(y-v.y) < EPSILON_E4 &&
// 			ABS(z-v.z) < EPSILON_E4 );
}

Quat Vector3::operator* (const Quat &q) const 
{
	return Quat(  q.w*x + q.z*y - q.y*z,
				q.w*y + q.x*z - q.z*x,
				q.w*z + q.y*x - q.x*y,
				-(q.x*x + q.y*y + q.z*z) );
}

Vector3 Vector3::operator* (const Matrix3X3 &m) const 
{
	Vector3 vcResult;
	
	#ifdef VS_SSE
	{
		VectorRegister _v = MakeVectorRegister(x, y, z, 0.0f);
		VectorRegister _m0 = MakeVectorRegister(m._00, m._01, m._02, 0.0f);
		VectorRegister _m1 = MakeVectorRegister(m._10, m._11, m._12, 0.0f);
		VectorRegister _m2 = MakeVectorRegister(m._20, m._21, m._22, 0.0f);

		// Splat x,y,z and w
		VectorRegister VTempX = VectorReplicate(_v, 0);
		VectorRegister VTempY = VectorReplicate(_v, 1);
		VectorRegister VTempZ = VectorReplicate(_v, 2);

		// Mul by the matrix
		VTempX = VectorMultiply(VTempX, _m0);
		VTempY = VectorMultiply(VTempY, _m1);
		VTempZ = VectorMultiply(VTempZ, _m2);
		// Add them all together
		VTempX = VectorAdd(VTempX, VTempY);

		VTempX = VectorAdd(VTempX, VTempZ);
		VectorStoreFloat3(VTempX, &vcResult);
	}
	#else
	{
		vcResult.x = x*m._00 + y*m._10 + z*m._20;
		vcResult.y = x*m._01 + y*m._11 + z*m._21;
		vcResult.z = x*m._02 + y*m._12 + z*m._22;
	}
	#endif

	return vcResult;
}

bool Vector3::IsParallel(const Vector3 & Vector)const
{
	float t0,t1;
	bool temp = 0;
	t0 = x * Vector.y;
	t1 = y * Vector.x;
	
	if( ABS(t0 - t1) > EPSILON_E4)
		temp = 1;

	t0 = y * Vector.z;
	t1 = z * Vector.y;

	if( ABS(t0 - t1) > EPSILON_E4 && temp)
		return 1;
	else
		return 0;
}

Vector3 Vector3::operator * (const Matrix3X3W &m)const
{
	Vector3 vcResult;

	#ifdef VS_SSE
	{
		VectorRegister _v = MakeVectorRegister(x, y, z, 1.0f);
		VectorRegister _m0 = VectorLoad(&m.m[0]);
		VectorRegister _m1 = VectorLoad(&m.m[4]);
		VectorRegister _m2 = VectorLoad(&m.m[8]);
		VectorRegister _m3 = VectorLoad(&m.m[12]);
		// Splat x,y,z and w
		VectorRegister VTempX = VectorReplicate(_v, 0);
		VectorRegister VTempY = VectorReplicate(_v, 1);
		VectorRegister VTempZ = VectorReplicate(_v, 2);
		VectorRegister VTempW = VectorReplicate(_v, 3);
		// Mul by the matrix
		VTempX = VectorMultiply(VTempX, _m0);
		VTempY = VectorMultiply(VTempY, _m1);
		VTempZ = VectorMultiply(VTempZ, _m2);
		VTempW = VectorMultiply(VTempW, _m3);
		// Add them all together
		VTempX = VectorAdd(VTempX, VTempY);
		VTempZ = VectorAdd(VTempZ, VTempW);
		VTempX = VectorAdd(VTempX, VTempZ);

		
		VectorRegister _div_w = VectorSetFloat1(VectorGetComponent(VTempX,3));
		VectorRegister _l = VectorDivide(VTempX,_div_w);
		VectorStoreFloat3(_l, &vcResult);
	}
	#else
	{
		vcResult.x = x*m._00 + y*m._10 + z*m._20 + m._30;
		vcResult.y = x*m._01 + y*m._11 + z*m._21 + m._31;
		vcResult.z = x*m._02 + y*m._12 + z*m._22 + m._32;
		float w = x*m._03 + y*m._13 + z*m._23 + m._33;

		vcResult.x = vcResult.x / w;
		vcResult.y = vcResult.y / w;
		vcResult.z = vcResult.z / w;
	}
	#endif
	return vcResult;
}

Vector3 Vector3::ReflectDir(const Vector3 &N)const
{
	return N * Dot(N) * 2 + *this;
}

float Vector3::SquaredDistance(const Vector3 &Point)const
{
    return (x * Point.x + y * Point.y + z * Point.z);
}

float Vector3::SquaredDistance(const Line & Line, float &fLineParameter)const
{
    return Line.SquaredDistance(*this, fLineParameter);
}

float Vector3::SquaredDistance(const Ray & Ray, float &fRayParameter)const
{
    return Ray.SquaredDistance(*this, fRayParameter);
}

float Vector3::SquaredDistance(const Segment & Segment, float &fSegmentParameter)const
{
    return  Segment.SquaredDistance(*this, fSegmentParameter);
}

float Vector3::SquaredDistance(const Triangle &Triangle, float fTriangleParameter[3])const
{
    Vector3 TrianglePoint[3];
    Triangle.GetPoint(TrianglePoint);

    Vector3 Diff = TrianglePoint[0] - *this;
    Vector3 Edge0 = TrianglePoint[1] - TrianglePoint[0];
    Vector3 Edge1 = TrianglePoint[2] - TrianglePoint[0];

    float fA00 = Edge0.GetSqrLength();
    float fA01 = Edge0.Dot(Edge1);
    float fA11 = Edge1.GetSqrLength();
    float fB0 = Diff.Dot(Edge0);
    float fB1 = Diff.Dot(Edge1);
    float fC = Diff.GetSqrLength();
    float fDet = ABS(fA00*fA11 - fA01 * fA01);
    float fS = fA01 * fB1 - fA11 * fB0;
    float fT = fA01 * fB0 - fA00 * fB1;
    float fSqrDistance;

    if (fS + fT <= fDet)
    {
        if (fS < 0.0f)
        {
            if (fT < 0.0f)  // region 4
            {
                if (fB0 < 0.0f)
                {
                    fT = 0.0f;
                    if (-fB0 >= fA00)
                    {
                        fS = 1.0f;
                        fSqrDistance = fA00 + (2.0f) * fB0 + fC;
                    }
                    else
                    {
                        fS = -fB0 / fA00;
                        fSqrDistance = fB0 * fS + fC;
                    }
                }
                else
                {
                    fS = 0.0f;
                    if (fB1 >= 0.0f)
                    {
                        fT = 0.0f;
                        fSqrDistance = fC;
                    }
                    else if (-fB1 >= fA11)
                    {
                        fT = 1.0f;
                        fSqrDistance = fA11 + 2.0f * fB1 + fC;
                    }
                    else
                    {
                        fT = -fB1 / fA11;
                        fSqrDistance = fB1 * fT + fC;
                    }
                }
            }
            else  // region 3
            {
                fS = 0.0f;
                if (fB1 >= 0.0f)
                {
                    fT = 0.0f;
                    fSqrDistance = fC;
                }
                else if (-fB1 >= fA11)
                {
                    fT = 1.0f;
                    fSqrDistance = fA11 + 2.0f * fB1 + fC;
                }
                else
                {
                    fT = -fB1 / fA11;
                    fSqrDistance = fB1 * fT + fC;
                }
            }
        }
        else if (fT < 0.0f)  // region 5
        {
            fT = 0.0f;
            if (fB0 >= 0.0f)
            {
                fS = 0.0f;
                fSqrDistance = fC;
            }
            else if (-fB0 >= fA00)
            {
                fS = 1.0f;
                fSqrDistance = fA00 + 2.0f * fB0 + fC;
            }
            else
            {
                fS = -fB0 / fA00;
                fSqrDistance = fB0 * fS + fC;
            }
        }
        else  // region 0
        {

            float fInvDet = 1.0f / fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDistance = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
                fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
        }
    }
    else
    {
        float fTmp0, fTmp1, fNumer, fDenom;

        if (fS < 0.0f)  // region 2
        {
            fTmp0 = fA01 + fB0;
            fTmp1 = fA11 + fB1;
            if (fTmp1 > fTmp0)
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00 - 2.0f * fA01 + fA11;
                if (fNumer >= fDenom)
                {
                    fS = 1.0f;
                    fT = 0.0f;
                    fSqrDistance = fA00 + 2.0f * fB0 + fC;
                }
                else
                {
                    fS = fNumer / fDenom;
                    fT = 1.0f - fS;
                    fSqrDistance = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
                        fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
                }
            }
            else
            {
                fS = 0.0f;
                if (fTmp1 <= 0.0f)
                {
                    fT = 1.0f;
                    fSqrDistance = fA11 + 2.0f * fB1 + fC;
                }
                else if (fB1 >= 0.0f)
                {
                    fT = 0.0f;
                    fSqrDistance = fC;
                }
                else
                {
                    fT = -fB1 / fA11;
                    fSqrDistance = fB1 * fT + fC;
                }
            }
        }
        else if (fT < 0.0f)  // region 6
        {
            fTmp0 = fA01 + fB1;
            fTmp1 = fA00 + fB0;
            if (fTmp1 > fTmp0)
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00 - 2.0f * fA01 + fA11;
                if (fNumer >= fDenom)
                {
                    fT = 1.0f;
                    fS = 0.0f;
                    fSqrDistance = fA11 + 2.0f * fB1 + fC;
                }
                else
                {
                    fT = fNumer / fDenom;
                    fS = 1.0f - fT;
                    fSqrDistance = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
                        fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
                }
            }
            else
            {
                fT = 0.0f;
                if (fTmp1 <= 0.0f)
                {
                    fS = 1.0f;
                    fSqrDistance = fA00 + 2.0f * fB0 + fC;
                }
                else if (fB0 >= 0.0f)
                {
                    fS = 0.0f;
                    fSqrDistance = fC;
                }
                else
                {
                    fS = -fB0 / fA00;
                    fSqrDistance = fB0 * fS + fC;
                }
            }
        }
        else  // region 1
        {
            fNumer = fA11 + fB1 - fA01 - fB0;
            if (fNumer <= 0.0f)
            {
                fS = 0.0f;
                fT = 1.0f;
                fSqrDistance = fA11 + 2.0f * fB1 + fC;
            }
            else
            {
                fDenom = fA00 - 2.0f * fA01 + fA11;
                if (fNumer >= fDenom)
                {
                    fS = 1.0f;
                    fT = 0.0f;
                    fSqrDistance = fA00 + 2.0f * fB0 + fC;
                }
                else
                {
                    fS = fNumer / fDenom;
                    fT = 1.0f - fS;
                    fSqrDistance = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
                        fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
                }
            }
        }
    }

    if (fSqrDistance < 0.0f)
    {
        fSqrDistance = 0.0f;
    }
    fTriangleParameter[1] = fS;
    fTriangleParameter[2] = fT;
    fTriangleParameter[0] = 1 - fTriangleParameter[1] - fTriangleParameter[2];

    return fSqrDistance;
}

float Vector3::SquaredDistance(const Rectangle &Rectangle, float fRectangleParameter[2])const
{
    Vector3 Diff = Rectangle.GetCenter() - *this;

    Vector3 A[2];
    Rectangle.GetA(A);
    float fA[2];
    Rectangle.GetfA(fA);

    float fB0 = Diff.Dot(A[0]);
    float fB1 = Diff.Dot(A[1]);
    float fS = -fB0;
    float fT = -fB1;
    float fSqrDistance = Diff.GetSqrLength();

    if (fS < -fA[0])
    {
        fS = -fA[0];
    }
    else if (fS > fA[0])
    {
        fS = fA[0];
    }
    fSqrDistance += fS * (fS + 2.0f * fB0);

    if (fT < -fA[1])
    {
        fT = -fA[1];
    }
    else if (fT > fA[1])
    {
        fT = fA[1];
    }
    fSqrDistance += fT * (fT + 2.0f * fB1);

    if (fSqrDistance < 0.0f)
    {
        fSqrDistance = 0.0f;
    }
    fRectangleParameter[0] = fS;
    fRectangleParameter[1] = fT;

    return fSqrDistance;
}

float Vector3::SquaredDistance(const OBB &OBB, float fOBBParameter[3])const
{
    return OBB.SquaredDistance(*this, fOBBParameter);
}

float Vector3::Distance(const Sphere& Sphere, Vector3 & SpherePoint)const
{
    return Sphere.Distance(*this, SpherePoint);
}

float Vector3::Distance(const Plane & Plane, Vector3 &PlanePoint)const
{
    return Plane.Distance(*this, PlanePoint);
}

float Vector3::SquaredDistance(const AABB &AABB, float fAABBParameter[3])const
{
    return AABB.SquaredDistance(*this, fAABBParameter);
}

float Vector3::SquaredDistance(const Polygon & Polygon, int& IndexTriangle, float fTriangleParameter[3])const
{
    return Polygon.SquaredDistance(*this, IndexTriangle, fTriangleParameter);
}

int32 Vector3::RelationWith(const Sphere & Sphere)const
{
    float sqrDist = SquaredDistance(Sphere.m_Center);
    float diff = sqrDist - Sphere.m_fRadius * Sphere.m_fRadius;
    if (diff > EPSILON_E4)
        return MATH_OUT;
    else if (diff < -EPSILON_E4)
        return MATH_IN;
    else
        return MATH_VSON;
}

int32 Vector3::RelationWith(const Plane &Plane)const
{
    return Plane.RelationWith(*this);
}

int32 Vector3::RelationWith(const OBB &OBB)const
{
    Matrix3X3W m;
    OBB.GetTransform(m);
    Vector3 PointTemp = *this * m;
    float fA[3];
    OBB.GetfA(fA);
    float fDiffX = ABS(PointTemp.x) - fA[0];
    float fDiffY = ABS(PointTemp.y) - fA[1];
    float fDiffZ = ABS(PointTemp.z) - fA[2];

    if (fDiffX > EPSILON_E4 || fDiffY > EPSILON_E4 || fDiffZ > EPSILON_E4)
        return MATH_OUT;

    if (fDiffX < -EPSILON_E4 || fDiffY < -EPSILON_E4 || fDiffZ < -EPSILON_E4)
        return MATH_IN;

    return MATH_VSON;
}

int32 Vector3::RelationWith(const AABB &AABB)const
{
    return AABB.RelationWith(*this);
}

}
