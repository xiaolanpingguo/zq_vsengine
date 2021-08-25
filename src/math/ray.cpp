#include "math/ray.h"
#include "math/segment.h"
#include "math/aabb.h"
#include "math/polygon.h"


namespace zq {


Ray::Ray()
{
}

Ray::Ray(const Vector3 & Orig, const Vector3 &Dir)
{
    Set(Orig, Dir);
}

Ray::~Ray()
{
}

bool Ray::GetParameter(const Vector3 &Point, float &fRayParameter)const
{
    if (!Line::GetParameter(Point, fRayParameter))
        return 0;

    if (fRayParameter < 0)
        return 0;

    return 1;
}

void Ray::Transform(const Ray &Ray, const Matrix3X3W &Mat)
{
    Line::Transform(Ray, Mat);
}

float Ray::SquaredDistance(const Vector3 &Point, float &fLineParameter)const
{
    float fSqDis = Line::SquaredDistance(Point, fLineParameter);
    if (fLineParameter >= 0)
        return fSqDis;
    else
    {
        fLineParameter = 0;
        Vector3 Diff;
        Diff = Point - m_Orig;
        return Diff.GetSqrLength();
    }
}

float Ray::SquaredDistance(const Line &Line, float &fRayParameter, float &fLineParameter)const
{
    return Line.SquaredDistance(*this, fLineParameter, fRayParameter);
}

float Ray::SquaredDistance(const Ray &Ray, float &fRay1Parameter, float &fRay2Parameter)const
{
    const Line Line = Ray;
    float sqrDist = SquaredDistance(Line, fRay1Parameter, fRay2Parameter);
    if (fRay2Parameter < 0)
    {
        fRay2Parameter = 0;
        sqrDist = SquaredDistance(Ray.m_Orig, fRay1Parameter);
    }

    return sqrDist;
}

float Ray::SquaredDistance(const Segment & Segment, float &fRayParameter, float &fSegmentParameter)const
{
    const Ray Ray = Segment;
    float sqrDist = SquaredDistance(Ray, fRayParameter, fSegmentParameter);
    float fLen = Segment.GetLen();
    if (fSegmentParameter > fLen)
    {
        fSegmentParameter = fLen;
        Vector3 End = Segment.GetEnd();
        sqrDist = SquaredDistance(End, fRayParameter);
    }

    return sqrDist;
}

float Ray::SquaredDistance(const Triangle& Triangle, float &fRayParameter, float fTriangleParameter[3])const
{
    float sqrDist = Line::SquaredDistance(Triangle, fRayParameter, fTriangleParameter);
    if (fRayParameter < 0)
    {
        fRayParameter = 0;
        sqrDist = m_Orig.SquaredDistance(Triangle, fTriangleParameter);
    }

    return sqrDist;
}

float Ray::SquaredDistance(const Rectangle& Rectangle, float &fRayParameter, float fRectangleParameter[2])const
{
    float sqrDist = Line::SquaredDistance(Rectangle, fRayParameter, fRectangleParameter);
    if (fRayParameter < 0)
    {
        fRayParameter = 0;
        sqrDist = m_Orig.SquaredDistance(Rectangle, fRectangleParameter);
    }

    return sqrDist;
}

float Ray::SquaredDistance(const OBB & OBB, float &fRayParameter, float fOBBParameter[3])const
{
    float sqrDist = Line::SquaredDistance(OBB, fRayParameter, fOBBParameter);
    if (fRayParameter < 0)
    {
        fRayParameter = 0;
        sqrDist = OBB.SquaredDistance(m_Orig, fOBBParameter);
    }

    return sqrDist;
}

float Ray::Distance(const Sphere &Sphere, float &fRayParameter, Vector3 & SpherePoint)const
{
    return Sphere.Distance(*this, SpherePoint, fRayParameter);
}

float Ray::Distance(const Plane & Plane, Vector3 &RayPoint, Vector3 &PlanePoint)const
{
    return Plane.Distance(*this, PlanePoint, RayPoint);
}

float Ray::SquaredDistance(const AABB &AABB, float &fRayParameter, float fAABBParameter[3])const
{
    float sqrDist = Line::SquaredDistance(AABB, fRayParameter, fAABBParameter);
    if (fRayParameter < 0)
    {
        fRayParameter = 0;
        sqrDist = AABB.SquaredDistance(m_Orig, fAABBParameter);

    }

    return sqrDist;
}

float Ray::SquaredDistance(const Polygon &Polygon, float &fRayParameter, int& IndexTriangle, float fTriangleParameter[3])const
{
    return Polygon.SquaredDistance(*this, IndexTriangle, fTriangleParameter, fRayParameter);
}

int32 Ray::RelationWith(const Triangle & Triangle, bool bCull, float &fRayParameter, float fTriangleParameter[3])const
{
    if (Line::RelationWith(Triangle, bCull, fRayParameter, fTriangleParameter) == MATH_NOINTERSECT)
        return MATH_NOINTERSECT;
    if (fRayParameter < 0.0f)
        return MATH_NOINTERSECT;

    return MATH_INTERSECT;
}

int32 Ray::RelationWith(const Plane &Plane, bool bCull, float &fRayParameter)const
{
    int32 iFlag = Line::RelationWith(Plane, bCull, fRayParameter);
    if (iFlag != MATH_INTERSECT)
        return iFlag;
    if (fRayParameter < 0.0f)
    {
        iFlag = m_Orig.RelationWith(Plane);
        return iFlag;
    }

    return MATH_INTERSECT;
}

int32 Ray::RelationWith(const Rectangle &Rectangle, bool bCull, float &fRayParameter,
    float fRectangleParameter[2])const
{
    if (Line::RelationWith(Rectangle, bCull, fRayParameter, fRectangleParameter) == MATH_NOINTERSECT)
        return MATH_NOINTERSECT;
    if (fRayParameter < 0.0f)
        return MATH_NOINTERSECT;

    return MATH_INTERSECT;
}

int32 Ray::RelationWith(const OBB &OBB, uint32 &Quantity, float &tNear, float &tFar)const
{
    if (Line::RelationWith(OBB, Quantity, tNear, tFar) == MATH_NOINTERSECT)
        return MATH_NOINTERSECT;
    if (tFar < 0.0f)
    {
        Quantity = 0;
        return MATH_NOINTERSECT;
    }
    if (tNear < 0.0f)
    {
        Quantity = 1;
        tNear = tFar;
    }

    return MATH_INTERSECT;
}

int32 Ray::RelationWith(const Sphere &sphere, uint32 &Quantity, float &tNear, float &tFar)const
{
    if (Line::RelationWith(sphere, Quantity, tNear, tFar) == MATH_NOINTERSECT)
        return MATH_NOINTERSECT;
    if (tFar < 0.0f)
    {
        Quantity = 0;
        return MATH_NOINTERSECT;
    }
    if (tNear < 0.0f)
    {
        Quantity = 1;
        tNear = tFar;
    }

    return MATH_INTERSECT;
}

int32 Ray::RelationWith(const AABB &AABB, uint32 &Quantity, float &tNear, float &tFar)const
{
    if (Line::RelationWith(AABB, Quantity, tNear, tFar) == MATH_NOINTERSECT)
        return MATH_NOINTERSECT;
    if (tFar < 0.0f)
    {
        Quantity = 0;
        return MATH_NOINTERSECT;
    }
    if (tNear < 0.0f)
    {
        Quantity = 1;
        tNear = tFar;
    }

    return MATH_INTERSECT;
}

int32 Ray::RelationWith(const Polygon &Polygon, float &fRayParameter, bool bCull, int32 &iIndexTriangle, float fTriangleParameter[3])const
{
    return Polygon.RelationWith(*this, bCull, iIndexTriangle, fTriangleParameter, fRayParameter);
}


}