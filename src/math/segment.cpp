#include "math/segment.h"
#include "math/aabb.h"
#include "math/polygon.h"


namespace zq{


Segment::Segment()
{
}

Segment::~Segment()
{
}

Segment::Segment(const Vector3 &Orig,const Vector3 &End)
{
	Set(Orig,End);
}

Segment::Segment(const Vector3 &Orig,const Vector3 &Dir,float fLen)
{
	Set(Orig,Dir,fLen);
}

void Segment::Transform(const Segment & Segment,const Matrix3X3W &Mat)
{
	m_Orig = Mat * Segment.m_Orig;
	m_End =  Mat * Segment.m_End;
	m_Dir  = Mat.Apply3X3(Segment.m_Dir);
	m_Dir.Normalize();
	m_fLen = Segment.m_fLen;
}

bool Segment::GetParameter(const Vector3 &Point,float &fSegmentParameter )const
{
	if(!Ray::GetParameter(Point,fSegmentParameter))
		return 0;
	if(fSegmentParameter > m_fLen)
		return 0;
	return 1;
}

float Segment::SquaredDistance(const Vector3 &Point, float &fSegmentParameter)const
{
    float SqDis = Ray::SquaredDistance(Point, fSegmentParameter);
    if (fSegmentParameter > m_fLen)
    {
        fSegmentParameter = m_fLen;
        Vector3 Diff;
        Diff = Point - m_End;
        return Diff.GetSqrLength();
    }
    else
    {
        return SqDis;
    }
}

float Segment::SquaredDistance(const Line &Line, float &fSegmentParameter, float &fLineParameter)const
{
    return Line.SquaredDistance(*this, fLineParameter, fSegmentParameter);
}

float Segment::SquaredDistance(const Ray &Ray, float &fSegmentParameter, float &fRayParameter)const
{
    return Ray.SquaredDistance(*this, fRayParameter, fSegmentParameter);
}

float Segment::SquaredDistance(const Segment & Segment, float &fSegment1Parameter, float &fSegment2Parameter)const
{
    const Ray & Ray = Segment;
    float sqrDist = SquaredDistance(Ray, fSegment1Parameter, fSegment2Parameter);
    float fLen = Segment.GetLen();
    if (fSegment2Parameter > fLen)
    {
        fSegment2Parameter = fLen;
        Vector3 End = Segment.GetEnd();
        sqrDist = SquaredDistance(End, fSegment1Parameter);
    }

    return sqrDist;
}

float Segment::SquaredDistance(const Triangle& Triangle, float &fSegmentParameter, float fTriangleParameter[3])const
{
    float sqrDist = Ray::SquaredDistance(Triangle, fSegmentParameter, fTriangleParameter);

    if (fSegmentParameter > m_fLen)
    {
        fSegmentParameter = m_fLen;
        sqrDist = m_End.SquaredDistance(Triangle, fTriangleParameter);

    }

    return sqrDist;
}

float Segment::SquaredDistance(const Rectangle& Rectangle, float &fSegmentParameter, float fRectangleParameter[2])const
{
    float sqrDist = Ray::SquaredDistance(Rectangle, fSegmentParameter, fRectangleParameter);

    if (fSegmentParameter > m_fLen)
    {
        fSegmentParameter = m_fLen;
        sqrDist = m_End.SquaredDistance(Rectangle, fRectangleParameter);

    }

    return sqrDist;
}

float Segment::SquaredDistance(const OBB & OBB, float &fSegmentParameter, float fOBBParameter[3])const
{
    float sqrDist = Ray::SquaredDistance(OBB, fSegmentParameter, fOBBParameter);
    if (fSegmentParameter > m_fLen)
    {
        fSegmentParameter = m_fLen;
        sqrDist = OBB.SquaredDistance(m_End, fOBBParameter);

    }

    return sqrDist;
}

float Segment::Distance(const Sphere &Sphere, float &fSegmentParameter, Vector3 & SpherePoint)const
{
    return Sphere.Distance(*this, SpherePoint, fSegmentParameter);
}

float Segment::Distance(const Plane & Plane, Vector3 &SegmentPoint, Vector3 &PlanePoint)const
{
    return Plane.Distance(*this, PlanePoint, SegmentPoint);
}

float Segment::SquaredDistance(const AABB &AABB, float &fSegmentParameter, float fAABBParameter[3])const
{
    float sqrDist = Ray::SquaredDistance(AABB, fSegmentParameter, fAABBParameter);
    if (fSegmentParameter > m_fLen)
    {
        fSegmentParameter = m_fLen;
        sqrDist = AABB.SquaredDistance(m_End, fAABBParameter);
    }

    return sqrDist;
}

float Segment::SquaredDistance(const Polygon &Polygon, float &fSegmentParameter, int& IndexTriangle, float fTriangleParameter[3])const
{
    return Polygon.SquaredDistance(*this, IndexTriangle, fTriangleParameter, fSegmentParameter);
}

int32 Segment::RelationWith(const Triangle & Triangle, bool bCull, float &fSegmentParameter, float fTriangleParameter[3])const
{
    if (Ray::RelationWith(Triangle, bCull, fSegmentParameter, fTriangleParameter) == MATH_NOINTERSECT)
        return MATH_NOINTERSECT;
    if (fSegmentParameter > m_fLen)
        return MATH_NOINTERSECT;

    return MATH_INTERSECT;
}

int32 Segment::RelationWith(const Plane &Plane, bool bCull, float &fSegmentParameter)const
{
    int32 iFlag = Ray::RelationWith(Plane, bCull, fSegmentParameter);
    if (iFlag != MATH_INTERSECT)
        return iFlag;
    if (fSegmentParameter > m_fLen)
    {
        return m_Orig.RelationWith(Plane);
    }

    return MATH_INTERSECT;
}

int32 Segment::RelationWith(const Rectangle &Rectangle, bool bCull, float &fSegmentParameter, float fRectangleParameter[2])const
{
    if (Ray::RelationWith(Rectangle, bCull, fSegmentParameter, fRectangleParameter) == MATH_NOINTERSECT)
        return MATH_NOINTERSECT;
    if (fSegmentParameter > m_fLen)
        return MATH_NOINTERSECT;
    return MATH_INTERSECT;
}

int32 Segment::RelationWith(const OBB &OBB, uint32 &Quantity, float &tNear, float &tFar)const
{
    if (Ray::RelationWith(OBB, Quantity, tNear, tFar) == MATH_NOINTERSECT)
        return MATH_NOINTERSECT;

    if (tNear > m_fLen)
    {
        Quantity = 0;
        if (m_Orig.RelationWith(OBB) != MATH_OUT)
            return MATH_IN;
        return MATH_NOINTERSECT;
    }

    if (tFar > m_fLen)
    {
        Quantity = 1;
        tFar = tNear;
    }

    return MATH_INTERSECT;
}

int32 Segment::RelationWith(const Sphere &sphere, uint32 &Quantity, float &tNear, float &tFar)const
{
    if (Ray::RelationWith(sphere, Quantity, tNear, tFar) == MATH_NOINTERSECT)
        return MATH_NOINTERSECT;
    if (tNear > m_fLen)
    {
        Quantity = 0;
        if (m_Orig.RelationWith(sphere) != MATH_OUT)
            return MATH_IN;
        return MATH_NOINTERSECT;
    }

    if (tFar > m_fLen)
    {
        Quantity = 1;
        tFar = tNear;
    }

    return MATH_INTERSECT;
}

int32 Segment::RelationWith(const AABB &AABB, uint32 &Quantity, float &tNear, float &tFar)const
{
    if (Ray::RelationWith(AABB, Quantity, tNear, tFar) == MATH_NOINTERSECT)
        return MATH_NOINTERSECT;
    if (tNear > m_fLen)
    {
        Quantity = 0;
        if (m_Orig.RelationWith(AABB) != MATH_OUT)
            return MATH_IN;
        return MATH_NOINTERSECT;
    }

    if (tFar > m_fLen)
    {
        Quantity = 1;
        tFar = tNear;
    }

    return MATH_INTERSECT;
}

int32 Segment::RelationWith(const Polygon &Polygon, float &fSegmentParameter, bool bCull, int32 &iIndexTriangle, float fTriangleParameter[3])const
{
    return Polygon.RelationWith(*this, bCull, iIndexTriangle, fTriangleParameter, fSegmentParameter);
}

}