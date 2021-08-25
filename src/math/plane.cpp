#include "math/plane.h"
#include "math/maths.h"
#include "math/line.h"
#include "math/triangle.h"
#include "math/segment.h"
#include "math/aabb.h"
#include "math/polygon.h"
#include "math/cylinder.h"


namespace zq{


Plane::Plane()
{
	m_fD = 0.0f;
}

Plane::Plane(const Vector3 &N , float fD)
{
	Set(N,fD);
}

Plane::Plane(const Vector3 &N, const Vector3 &P)
{
	Set(N,P);
}

Plane::Plane(const Vector3 &P0,  const Vector3 &P1, const Vector3 &P2)
{
	Set(P0,P1,P2); 
}

Plane::Plane(const Vector3 Point[3])
{
	Set(Point[0],Point[1],Point[2]);
}

void Plane::Transform(const Plane &Plane,const Matrix3X3W &Mat)
{
	Vector3W Temp(m_N.x,m_N.y,m_N.z,m_fD);

	Matrix3X3W Inv;
	Inv.InverseOf(Mat);
	Matrix3X3W Transpose;
	Transpose.TransposeOf(Inv);
	Vector3W Resoult = Temp * Transpose;
	Set(Vector3(Resoult.x, Resoult.y, Resoult.z), Resoult.w);
}

Vector3 Plane::ReflectDir(const Vector3 & Dir)const
{
	Vector3 TempN = m_N * (-1.0f);
	return Dir - m_N * Dir.Dot(m_N) * 2.0f;
}

void  Plane::GetReverse(Plane &OutPlane)const
{
	OutPlane.Set(m_N * (-1.0f), -m_fD);
}

float Plane::Distance(const Vector3 &Point, Vector3 &PlanePoint) const
{

    float Dist = ABS((m_N.Dot(Point)) - m_fD);

    Line Line(Point, m_N * (-1));
    PlanePoint = Line.GetParameterPoint(Dist);

    return Dist;
}

float Plane::Distance(const Sphere &Sphere, Vector3 & SpherePoint) const
{
    return Sphere.Distance(*this, SpherePoint);
}

float Plane::Distance(const Line &Line, Vector3 &PlanePoint, Vector3 &LinePoint)const
{
    float fDot = Line.GetDir().Dot(m_N);
    if (ABS(fDot) < EPSILON_E4)
    {
        LinePoint = Line.GetOrig();
        return LinePoint.Distance(*this, PlanePoint);
    }
    else
        return 0;
}

float Plane::Distance(const Ray & Ray, Vector3 &PlanePoint, Vector3 &RayPoint)const
{
    float fDot = Ray.GetDir().Dot(m_N);
    RayPoint = Ray.GetOrig();

    float f = RayPoint.Dot(m_N) + m_fD;
    if (fDot * f > 0)
        return RayPoint.Distance(*this, PlanePoint);
    else
        return 0;
}

float Plane::Distance(const Segment & Segment, Vector3 &PlanePoint, Vector3 &SegmentPoint)const
{

    Vector3 SegmentOrig = Segment.GetOrig();
    Vector3 SegmentEnd = Segment.GetEnd();
    float f0 = SegmentOrig.Dot(m_N) + m_fD;
    float f1 = SegmentEnd.Dot(m_N) + m_fD;

    if (f0 * f1 > 0)
    {
        Vector3 PPoint;
        float dist = SegmentOrig.Distance(*this, PPoint);
        SegmentPoint = SegmentOrig;
        PlanePoint = PPoint;
        float distTemp = SegmentEnd.Distance(*this, PPoint);
        if (dist > distTemp)
        {
            dist = distTemp;
            SegmentPoint = SegmentEnd;
            PlanePoint = PPoint;
        }
        return dist;
    }
    else
        return 0;

}

float Plane::Distance(const Plane &Plane, Vector3 &Plane1Point, Vector3 &Plane2Point)const
{
    if (m_N.IsParallel(Plane.m_N))
    {
        Plane1Point = GetPoint();
        return Plane1Point.Distance(Plane, Plane2Point);
    }
    else
    {
        return 0;
    }
}

float Plane::Distance(const Triangle &Triangle, Vector3 &PlanePoint, Vector3 &TrianglePoint)const
{
    /*int32 i[3];
    Vector3 Point[3];
    Triangle.GetPoint(Point);
    i[0] = RelationWith(Point[0]);
    i[1] = RelationWith(Point[1]);
    i[2] = RelationWith(Point[2]);

    if((i[0] == i[1]) && (i[1] == i[2]))
    {
        float dist[3],distTemp;
        Vector3 PlanePointTemp[3];
        dist[0] = Point[0].Distance(*this,PlanePointTemp[0]);
        dist[1] = Point[1].Distance(*this,PlanePointTemp[1]);
        dist[2] = Point[2].Distance(*this,PlanePointTemp[2]);
        distTemp = dist[0];
        PlanePoint = PlanePointTemp[0];
        TrianglePoint = Point[0];
        if(distTemp > dist[1])
        {
            distTemp = dist[1];
            PlanePoint = PlanePointTemp[1];
            TrianglePoint = Point[1];
        }
        if(distTemp > dist[2])
        {
            distTemp = dist[2];
            PlanePoint = PlanePointTemp[2];
            TrianglePoint = Point[2];
        }
        return distTemp;
    }
    return 0;*/
    Vector3 Point[3];
    Triangle.GetPoint(Point);

    float dist[3], distTemp;
    Vector3 PlanePointTemp[3];
    dist[0] = Point[0].Distance(*this, PlanePointTemp[0]);
    dist[1] = Point[1].Distance(*this, PlanePointTemp[1]);
    dist[2] = Point[2].Distance(*this, PlanePointTemp[2]);
    distTemp = dist[0];
    PlanePoint = PlanePointTemp[0];
    TrianglePoint = Point[0];
    if (distTemp > dist[1])
    {
        distTemp = dist[1];
        PlanePoint = PlanePointTemp[1];
        TrianglePoint = Point[1];
    }
    if (distTemp > dist[2])
    {
        distTemp = dist[2];
        PlanePoint = PlanePointTemp[2];
        TrianglePoint = Point[2];
    }

    return distTemp;
}

float Plane::Distance(const Rectangle &Rectangle, Vector3 &PlanePoint, Vector3 &RectanglePoint)const
{
    /*int32 f[4];
    Vector3 Point[4];
    Rectangle.GetPoint(Point);
    for(int32 i = 0 ; i < 4 ; i++)
    {
        f[i] = RelationWith(Point[i]);
    }

    if((f[0] == f[1]) && (f[1] == f[2]) && (f[2] == f[3]))
    {
        float dist[4],distTemp;
        Vector3 PlanePointTemp[4];
        for(int32 i = 0 ; i < 4 ; i++)
            dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

        distTemp = dist[0];
        PlanePoint = PlanePointTemp[0];
        RectanglePoint = Point[0];
        for(int32 i = 1 ; i < 4 ; i++)
        {
            if(distTemp > dist[i])
            {
                distTemp = dist[i];
                PlanePoint = PlanePointTemp[i];
                RectanglePoint = Point[i];
            }
        }
        return distTemp;
    }
    return 0;*/

    Vector3 Point[4];
    Rectangle.GetPoint(Point);

    float dist[4], distTemp;
    Vector3 PlanePointTemp[4];
    for (int32 i = 0; i < 4; i++)
        dist[i] = Point[i].Distance(*this, PlanePointTemp[i]);

    distTemp = dist[0];
    PlanePoint = PlanePointTemp[0];
    RectanglePoint = Point[0];
    for (int32 i = 1; i < 4; i++)
    {
        if (distTemp > dist[i])
        {
            distTemp = dist[i];
            PlanePoint = PlanePointTemp[i];
            RectanglePoint = Point[i];
        }
    }

    return distTemp;
}

float Plane::Distance(const OBB& OBB, Vector3 &PlanePoint, Vector3 &OBBPoint)const
{
    /*int32 f[8];
    Vector3 Point[8];
    OBB.GetPoint(Point);
    for(int32 i = 0 ; i < 8 ; i++)
    {
        f[i] = RelationWith(Point[i]);
    }

    if((f[0] == f[1]) && (f[1] == f[2]) && (f[2] == f[3]) && (f[3] == f[4])
            && (f[4] == f[6]) && (f[5] == f[6]) && (f[6] == f[7]))
    {
        float dist[4],distTemp;
        Vector3 PlanePointTemp[8];
        for(int32 i = 0 ; i < 8 ; i++)
            dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

        distTemp = dist[0];
        PlanePoint = PlanePointTemp[0];
        OBBPoint = Point[0];
        for(int32 i = 1 ; i < 8 ; i++)
        {
            if(distTemp > dist[i])
            {
                distTemp = dist[i];
                PlanePoint = PlanePointTemp[i];
                OBBPoint = Point[i];
            }
        }
        return distTemp;
    }
    return 0;*/

    Vector3 Point[8];
    OBB.GetPoint(Point);

    float dist[4], distTemp;
    Vector3 PlanePointTemp[8];
    for (int32 i = 0; i < 8; i++)
        dist[i] = Point[i].Distance(*this, PlanePointTemp[i]);

    distTemp = dist[0];
    PlanePoint = PlanePointTemp[0];
    OBBPoint = Point[0];
    for (int32 i = 1; i < 8; i++)
    {
        if (distTemp > dist[i])
        {
            distTemp = dist[i];
            PlanePoint = PlanePointTemp[i];
            OBBPoint = Point[i];
        }
    }

    return distTemp;
}

float Plane::Distance(const AABB &AABB, Vector3 &PlanePoint, Vector3 &AABBPoint)const
{
    /*int32 f[8];
    Vector3 Point[8];
    AABB.GetPoint(Point);
    for(int32 i = 0 ; i < 8 ; i++)
    {
        f[i] = RelationWith(Point[i]);
    }

    if((f[0] == f[1]) && (f[1] == f[2]) && (f[2] == f[3]) && (f[3] == f[4])
        && (f[4] == f[6]) && (f[5] == f[6]) && (f[6] == f[7]))
    {
        float dist[4],distTemp;
        Vector3 PlanePointTemp[8];
        for(int32 i = 0 ; i < 8 ; i++)
            dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

        distTemp = dist[0];
        PlanePoint = PlanePointTemp[0];
        AABBPoint = Point[0];
        for(int32 i = 1 ; i < 8 ; i++)
        {
            if(distTemp > dist[i])
            {
                distTemp = dist[i];
                PlanePoint = PlanePointTemp[i];
                AABBPoint = Point[i];
            }
        }
        return distTemp;
    }
    return 0;*/

    Vector3 Point[8];
    AABB.GetPoint(Point);

    float dist[4], distTemp;
    Vector3 PlanePointTemp[8];
    for (int32 i = 0; i < 8; i++)
        dist[i] = Point[i].Distance(*this, PlanePointTemp[i]);

    distTemp = dist[0];
    PlanePoint = PlanePointTemp[0];
    AABBPoint = Point[0];
    for (int32 i = 1; i < 8; i++)
    {
        if (distTemp > dist[i])
        {
            distTemp = dist[i];
            PlanePoint = PlanePointTemp[i];
            AABBPoint = Point[i];
        }
    }

    return distTemp;
}

float Plane::Distance(const Polygon &Polygon, Vector3 &PlanePoint, int& IndexTriangle, Vector3 &TrianglePoint) const
{
    return Polygon.Distance(*this, IndexTriangle, TrianglePoint, PlanePoint);
}

int32 Plane::RelationWith(const Vector3 &Point)const
{
    float f = (Point.Dot(m_N)) + m_fD;

    if (f > EPSILON_E4) return MATH_FRONT;
    if (f < -EPSILON_E4) return MATH_BACK;
    return MATH_VSON;
}

int32 Plane::RelationWith(const Line &Line, bool bCull, float &fLineParameter)const
{
    return Line.RelationWith(*this, bCull, fLineParameter);
}

int32 Plane::RelationWith(const Ray &Ray, bool bCull, float &fRayParameter)const
{
    return Ray.RelationWith(*this, bCull, fRayParameter);
}

int32 Plane::RelationWith(const Segment &Segment, bool bCull, float &fSegmentParameter)const
{
    return Segment.RelationWith(*this, bCull, fSegmentParameter);
}

int32 Plane::RelationWith(const OBB &OBB)const
{
    return OBB.RelationWith(*this);

}

int32 Plane::RelationWith(const AABB &AABB)const
{
    return AABB.RelationWith(*this);
}

int32 Plane::RelationWith(const Sphere &Sphere)const
{
    return Sphere.RelationWith(*this);
}

int32 Plane::RelationWith(const Triangle &Triangle)const
{
    return Triangle.RelationWith(*this);
}

int32 Plane::RelationWith(const Plane &Plane)const
{
    Vector3 vcCross;
    float     fSqrLength;

    vcCross.Cross(m_N, Plane.m_N);
    fSqrLength = vcCross.GetSqrLength();

    if (fSqrLength < EPSILON_E4)
    {
        //return Plane.m_Point.RelationWith(*this);
        return MATH_NOINTERSECT;
    }

    return MATH_INTERSECT;
    /*float fN00 = m_N.GetSqrLength();
    float fN01 = m_N * Plane.m_N;
    float fN11 = Plane.m_N.GetSqrLength();
    float fDet = fN00*fN11 - fN01*fN01;

    if (ABS(fDet) < EPSILON_E4)
        return Plane.m_Point.RelationWith(*this);

    float fInvDet = 1.0f/fDet;
    float fC0 = (fN11 * m_fD - fN01 * Plane.m_fD) * fInvDet;
    float fC1 = (fN00 * Plane.m_fD - fN01 * m_fD) * fInvDet;
    Line.Set(vcCross,m_N * fC0 + Plane.m_N * fC1);
    return MATH_INTERSECT;*/
}

int32 Plane::RelationWith(const Rectangle & Rectangle)const
{
    return Rectangle.RelationWith(*this);
}

int32 Plane::RelationWith(const Plane &Plane, Line &Line)const
{
    Vector3 vcCross;
    float     fSqrLength;

    vcCross.Cross(m_N, Plane.m_N);
    fSqrLength = vcCross.GetSqrLength();

    if (fSqrLength < EPSILON_E4)
    {
        return MATH_NOINTERSECT;
        //return Plane.m_Point.RelationWith(*this);
    }
    float fN00 = m_N.GetSqrLength();
    float fN01 = m_N.Dot(Plane.m_N);
    float fN11 = Plane.m_N.GetSqrLength();
    float fDet = fN00 * fN11 - fN01 * fN01;

    if (ABS(fDet) < EPSILON_E4)
        return MATH_NOINTERSECT;

    float fInvDet = 1.0f / fDet;
    float fC0 = (fN11 * m_fD - fN01 * Plane.m_fD) * fInvDet;
    float fC1 = (fN00 * Plane.m_fD - fN01 * m_fD) * fInvDet;
    Line.Set(m_N * fC0 + Plane.m_N * fC1, vcCross);
    return MATH_INTERSECT;/**/
}

int32 Plane::RelationWith(const Triangle &Triangle, Segment & Segment)const
{
    return Triangle.RelationWith(*this, Segment);
}

int32 Plane::RelationWith(const Rectangle &Rectangle, Segment &Segment)const
{
    return Rectangle.RelationWith(*this, Segment);
}

int32 Plane::RelationWith(const Polygon &Polygon)const
{
    return Polygon.RelationWith(*this);
}

int32 Plane::RelationWith(const Polygon &Polygon, Segment & Segment)const
{
    return Polygon.RelationWith(*this, Segment);
}

int32 Plane::RelationWith(const Cylinder &Cylinder3)const
{
    return MATH_INTERSECT;
}

}

