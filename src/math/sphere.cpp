#include "math/sphere.h"
#include "math/aabb.h"
#include "math/maths.h"
#include "math/segment.h"
#include "math/polygon.h"


namespace zq{


Sphere::Sphere()
{
}

Sphere::~Sphere()
{
	m_fRadius = 0.0f;
}

Sphere::Sphere(const Vector3 & Center, float fRadius)
{
	Set(Center,fRadius);
}

void Sphere::CreateSphere(const Vector3 *pPointArray,uint32 uiPointNum)
{
	if(!pPointArray || !uiPointNum)
		return ;
	Vector3 Sum;
	for(uint32 i = 0 ; i < uiPointNum ; i++)
		Sum += pPointArray[i];
	m_Center = Sum / (uiPointNum * 1.0f);
	float radiusSqr = 0.0f;
	for(uint32 i = 0 ; i < uiPointNum ; i++)
	{
		Vector3 diff = pPointArray[i] - m_Center;
		float Temp = diff.GetSqrLength();
		if(Temp > radiusSqr)
			radiusSqr = Temp;
	
	}

	m_fRadius = SQRT(radiusSqr);
}

Sphere Sphere::MergeSpheres(const Sphere& sphere)const
{
	Sphere temp;
	Vector3 CenterDiff = m_Center - sphere.m_Center;
	float fRadiusDiff = m_fRadius - sphere.m_fRadius;
	float fRadiusDiffSqr = fRadiusDiff * fRadiusDiff;
	float Lsqr = CenterDiff.GetSqrLength();
	if(fRadiusDiffSqr >= Lsqr)
	{
		if(fRadiusDiff >= 0.0f)
		{
            temp.m_Center = m_Center;
            temp.m_fRadius = m_fRadius;
		}
		else
		{
            temp.m_Center = sphere.m_Center;
            temp.m_fRadius = sphere.m_fRadius;
		}
	}
	else
	{
		float L = SQRT(Lsqr);
		float t = (L + m_fRadius - sphere.m_fRadius)/(2.0f * L);
        temp.m_Center = sphere.m_Center + CenterDiff * t;
        temp.m_fRadius = (L + m_fRadius + sphere.m_fRadius)/2.0f;
	}

	return temp;
}

void Sphere::Transform(const Sphere & Sphere,const Matrix3X3W &Mat)
{
	m_Center = Sphere.m_Center * Mat;
	/*Matrix3X3 M;
	Mat.Get3X3(M);
	Vector3 V[3];
	float E[3];
	M.GetEigenSystem(E,V);
	float Max = E[0];
	for(uint32 i = 1 ; i < 3 ; i++)
	{
	if(Max < E[i])
	Max = E[i];
	}
	m_fRadius = Sphere.m_fRadius * Max;*/
	Vector3 V0(1.0f,0.0f,0.0f),V1(0.0f,1.0f,0.0f),V2(0.0f,0.0f,1.0f);
	Vector3 Temp0 = Sphere.m_Center +  V0 * Sphere.m_fRadius;
	Vector3 Temp1 = Sphere.m_Center +  V1 * Sphere.m_fRadius;
	Vector3 Temp2 = Sphere.m_Center +  V2 * Sphere.m_fRadius;

	Temp0 = Temp0 * Mat;
	Temp1 = Temp1 * Mat;
	Temp2 = Temp2 * Mat;

	float fRadiusTemp;

	m_fRadius = (Temp0 - m_Center).GetLength();

	fRadiusTemp = (Temp1 - m_Center).GetLength();

	if(m_fRadius < fRadiusTemp)
		m_fRadius = fRadiusTemp;

	fRadiusTemp = (Temp2 - m_Center).GetLength();

	if(m_fRadius < fRadiusTemp)
		m_fRadius = fRadiusTemp;
}

AABB Sphere::GetAABB()const
{
	Vector3 Max = m_Center + Vector3(m_fRadius,m_fRadius,m_fRadius);
	Vector3 Min = m_Center + Vector3(-m_fRadius,-m_fRadius,-m_fRadius);
	return AABB(Max,Min);
}

float Sphere::Distance(const Vector3 & Point, Vector3 & SpherePoint)const
{
    float sqrDist = Point.SquaredDistance(m_Center);

    sqrDist = SQRT(sqrDist);
    sqrDist = sqrDist - m_fRadius;

    Line Line(m_Center, Point);

    SpherePoint = Line.GetParameterPoint(m_fRadius);
    return sqrDist;
}

float Sphere::Distance(const Line& line, Vector3& SpherePoint, float& fLineParameter)const
{
    float sqrDist = line.SquaredDistance(m_Center, fLineParameter);
    sqrDist = SQRT(sqrDist);
    sqrDist = sqrDist - m_fRadius;

    Line LineTemp(m_Center, line.GetParameterPoint(fLineParameter));

    SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
    return sqrDist;
}

float Sphere::Distance(const Ray & Ray, Vector3 & SpherePoint, float &fRayParameter)const
{
    float sqrDist = Ray.SquaredDistance(m_Center, fRayParameter);

    sqrDist = SQRT(sqrDist);
    sqrDist = sqrDist - m_fRadius;

    Line LineTemp(m_Center, Ray.GetParameterPoint(fRayParameter));

    SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
    return sqrDist;
}

float Sphere::Distance(const Segment & Segment, Vector3 & SpherePoint, float &fSegmentParameter)const
{
    float sqrDist = Segment.SquaredDistance(m_Center, fSegmentParameter);

    sqrDist = SQRT(sqrDist);
    sqrDist = sqrDist - m_fRadius;

    Line LineTemp(m_Center, Segment.GetParameterPoint(fSegmentParameter));

    SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
    return sqrDist;
}

float Sphere::Distance(const OBB &OBB, Vector3 & SpherePoint, float fOBBParameter[3])const
{
    float sqrDist = OBB.SquaredDistance(m_Center, fOBBParameter);

    sqrDist = SQRT(sqrDist);
    sqrDist = sqrDist - m_fRadius;

    Line LineTemp(m_Center, OBB.GetParameterPoint(fOBBParameter));

    SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
    return sqrDist;
}

float Sphere::Distance(const Plane & Plane, Vector3 & SpherePoint)const
{
    Vector3 PlanePoint;
    float sqrDist = Plane.Distance(m_Center, PlanePoint);

    sqrDist = sqrDist - m_fRadius;

    Line LineTemp(m_Center, PlanePoint);

    SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
    return sqrDist;
}

float Sphere::Distance(const Rectangle & Rectangle, Vector3 & SpherePoint, float fRectangleParameter[2])const
{
    float sqrDist = Rectangle.SquaredDistance(m_Center, fRectangleParameter);

    sqrDist = SQRT(sqrDist);
    sqrDist = sqrDist - m_fRadius;

    Line LineTemp(m_Center, Rectangle.GetParameterPoint(fRectangleParameter));

    SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
    return sqrDist;
}

float Sphere::Distance(const Triangle Triangle, Vector3 & SpherePoint, float fTriangleParameter[3]) const
{
    float sqrDist = Triangle.SquaredDistance(m_Center, fTriangleParameter);

    sqrDist = SQRT(sqrDist);
    sqrDist = sqrDist - m_fRadius;

    Line LineTemp(m_Center, Triangle.GetParameterPoint(fTriangleParameter));

    SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
    return sqrDist;
}

float Sphere::Distance(const AABB &AABB, Vector3 & SpherePoint, float fAABBParameter[3])const
{
    float sqrDist = AABB.SquaredDistance(m_Center, fAABBParameter);

    sqrDist = SQRT(sqrDist);
    sqrDist = sqrDist - m_fRadius;

    Line LineTemp(m_Center, AABB.GetParameterPoint(fAABBParameter));

    SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
    return sqrDist;
}

float Sphere::Distance(const Polygon &Polygon, Vector3 & SpherePoint, int& IndexTriangle, float fTriangleParameter[3])const
{
    return Polygon.Distance(*this, IndexTriangle, fTriangleParameter, SpherePoint);
}

int32 Sphere::RelationWith(const Vector3 &Point)const
{
    return Point.RelationWith(*this);
}

int32 Sphere::RelationWith(const Line &Line, uint32 &Quantity, float &tNear, float &tFar)const
{
    return Line.RelationWith(*this, Quantity, tNear, tFar);
}

int32 Sphere::RelationWith(const Ray &Ray, uint32 &Quantity, float &tNear, float &tFar)const
{
    return Ray.RelationWith(*this, Quantity, tNear, tFar);
}

int32 Sphere::RelationWith(const Segment &Segment, uint32 &Quantity, float &tNear, float &tFar)const
{
    return Segment.RelationWith(*this, Quantity, tNear, tFar);
}

int32 Sphere::RelationWith(const Plane &Plane)const
{
    Vector3 N = Plane.GetN();
    float fD = Plane.GetfD();

    float test = m_Center.Dot(N) + fD;
    if (test > m_fRadius)
        return MATH_FRONT;
    else if (test < -m_fRadius)
        return MATH_BACK;
    else
        return MATH_INTERSECT;
}

int32 Sphere::RelationWith(const Triangle Triangle) const
{
    return Triangle.RelationWith(*this);
}

int32 Sphere::RelationWith(const Rectangle &Rectangle) const
{
    return Rectangle.RelationWith(*this);
}

int32 Sphere::RelationWith(const OBB &OBB) const
{
    return OBB.RelationWith(*this);
}

int32 Sphere::RelationWith(const Sphere &Sphere) const
{
    float Sum = m_fRadius + Sphere.m_fRadius;
    Sum *= Sum;
    Vector3 Sub = m_Center - Sphere.m_Center;

    if (Sub.GetSqrLength() > Sum)
        return MATH_NOINTERSECT;

    return MATH_INTERSECT;
}

}