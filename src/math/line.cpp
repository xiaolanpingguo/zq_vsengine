#include "math/line.h"

#include "math/ray.h"
#include "math/segment.h"
#include "math/aabb.h"
#include "math/polygon.h"


namespace zq{


Line::Line()
{
}

Line::Line(const Vector3 & Orig,const Vector3 &Dir)
{
	Set(Orig,Dir);
}

Line::~Line()
{
}

bool Line::GetParameter(const Vector3 &Point,float &fLineParameter )const
{
	Vector3 Temp = Point - m_Orig;
	if(Temp.IsParallel(m_Dir))
	{
		fLineParameter = Temp.GetLength();
		return 1;
	}
    else
    {
        return 0;
    }
}

void Line::Transform(const Line &Line,const Matrix3X3W &Mat)
{
	m_Dir = Mat.Apply3X3(Line.m_Dir);
	m_Dir.Normalize();
	m_Orig = Line.m_Orig * Mat;
}

float Line::SquaredDistance(const Vector3 &Point, float &fLineParameter)const
{
    Vector3 Diff = Point - m_Orig;

    fLineParameter = m_Dir.Dot(Diff);

    Diff -= m_Dir * fLineParameter;

    return Diff.GetSqrLength();
}

float Line::SquaredDistance(const Line &Line, float &fLine1Parameter, float &fLine2Parameter)const
{
    float SquareDis;
    Vector3 Diff = m_Orig - Line.m_Orig;
    float a01 = -(m_Dir.Dot(Line.m_Dir));
    float b0 = m_Dir.Dot(Diff);
    float c = Diff.GetSqrLength();
    float det = 1 - a01 * a01;

    if (ABS(det) >= EPSILON_E4)
    {
        float b1 = -(Line.m_Dir.Dot(Diff));
        float invDet = 1 / det;
        fLine1Parameter = (a01 * b1 - b0) * invDet;
        fLine2Parameter = (a01 * b0 - b1) * invDet;
        SquareDis = fLine1Parameter * (fLine1Parameter + a01 * fLine2Parameter + 2 * b0) + fLine2Parameter * (a01 * fLine1Parameter + fLine2Parameter + 2 * b1) + c;

    }
    else
    {
        fLine1Parameter = -b0;
        fLine2Parameter = 0;

        SquareDis = b0 * fLine1Parameter + c;

    }
    return SquareDis;
}

float Line::SquaredDistance(const Ray &Ray, float &fLineParameter, float &fRayParameter)const
{
    const Line Line = Ray;
    float sqrDist = SquaredDistance(Line, fLineParameter, fRayParameter);

    if (fRayParameter < 0)
    {
        fRayParameter = 0;
        sqrDist = SquaredDistance(Ray.m_Orig, fLineParameter);

    }

    return sqrDist;
}

float Line::SquaredDistance(const Segment & Segment, float &fLineParameter, float &fSegmentParameter)const
{
    const Ray Ray = Segment;
    float sqrDist = SquaredDistance(Ray, fLineParameter, fSegmentParameter);
    float fLen = Segment.GetLen();

    if (fSegmentParameter > fLen)
    {
        Vector3 End = Segment.GetEnd();
        fSegmentParameter = fLen;
        sqrDist = SquaredDistance(End, fLineParameter);

    }

    return sqrDist;
}

float Line::SquaredDistance(const Triangle &Triangle, float &fLineParameter, float fTriangleParameter[3])const
{
    Vector3 TrianglePoint[3];
    Triangle.GetPoint(TrianglePoint);

    /*Vector3 Diff = TrianglePoint[0] - m_Orig;
    Vector3 Edge0 = TrianglePoint[1] - TrianglePoint[0];
    Vector3 Edge1 = TrianglePoint[2] - TrianglePoint[0];
    float fA00 = m_Dir * m_Dir;
    float fA01 = m_Dir * Edge0;
    float fA02 = m_Dir * Edge1;
    float fA11 = Edge0.GetSqrLength();
    float fA12 = Edge0 * Edge1;
    float fA22 = Edge1 * Edge1;
    float fB0  = Diff * m_Dir;
    float fB1  = Diff * Edge0;
    float fB2  = Diff * Edge1;
    Vector3 Normal;
    Normal.Cross(Edge0,Edge1);
    float fDot = Normal * m_Dir;
    if (ABS(fDot) >= EPSILON_E4)
    {

        float fCof00 = fA11*fA22-fA12*fA12;
        float fCof01 = fA02*fA12-fA01*fA22;
        float fCof02 = fA01*fA12-fA02*fA11;
        float fCof11 = fA00*fA22-fA02*fA02;
        float fCof12 = fA02*fA01-fA00*fA12;
        float fCof22 = fA00*fA11-fA01*fA01;
        float fInvDet = (1.0f)/(fA00*fCof00+fA01*fCof01+fA02*fCof02);
        float fRhs0 = -fB0*fInvDet;
        float fRhs1 = -fB1*fInvDet;
        float fRhs2 = -fB2*fInvDet;


        fLineParameter = fCof00*fRhs0+fCof01*fRhs1+fCof02*fRhs2;


        fTriangleParameter[1] = fCof01*fRhs0+fCof11*fRhs1+fCof12*fRhs2;
        fTriangleParameter[2] = fCof02*fRhs0+fCof12*fRhs1+fCof22*fRhs2;
        fTriangleParameter[0] = 1.0f - fTriangleParameter[1] - fTriangleParameter[2];

        if (fTriangleParameter[0] >= 0.0f
            &&  fTriangleParameter[1]>= 0.0f
            &&  fTriangleParameter[2] >= 0.0f)
        {

            return 0.0f;
        }
    }*/


    float fSqrDist = MATH_MAX_FLOAT;
    float fSqrDistTmp;
    Segment Edge;
    float Tempt0, Tempt1;
    Edge.Set(TrianglePoint[0], TrianglePoint[1]);
    fSqrDistTmp = SquaredDistance(Edge, Tempt0, Tempt1);

    if (fSqrDistTmp < fSqrDist)
    {
        fLineParameter = Tempt0;

        fTriangleParameter[1] = Tempt1 / Edge.GetLen();
        fTriangleParameter[0] = 1.0f - fTriangleParameter[1];
        fTriangleParameter[2] = 0.0f;

        fSqrDist = fSqrDistTmp;
    }

    Edge.Set(TrianglePoint[0], TrianglePoint[2]);
    fSqrDistTmp = SquaredDistance(Edge, Tempt0, Tempt1);

    if (fSqrDistTmp < fSqrDist)
    {
        fLineParameter = Tempt0;

        fTriangleParameter[1] = 0.0f;
        fTriangleParameter[2] = Tempt1 / Edge.GetLen();
        fTriangleParameter[0] = 1.0f - fTriangleParameter[2];

        fSqrDist = fSqrDistTmp;
    }

    Edge.Set(TrianglePoint[1], TrianglePoint[2]);
    fSqrDistTmp = SquaredDistance(Edge, Tempt0, Tempt1);

    if (fSqrDistTmp < fSqrDist)
    {
        fLineParameter = Tempt0;

        fTriangleParameter[1] = Tempt1 / Edge.GetLen();
        fTriangleParameter[2] = 1.0f - fTriangleParameter[1];
        fTriangleParameter[0] = 0.0f;

        fSqrDist = fSqrDistTmp;
    }

    return fSqrDist;
}

float Line::SquaredDistance(const Rectangle& Rectangle, float &fLineParameter, float fRectangleParameter[2])const
{
    float fSqrDist = MATH_MAX_FLOAT;
    float fSqrDistTmp;
    Vector3 r[4];

    Vector3 A[2];
    Rectangle.GetA(A);
    float fA[2];
    Rectangle.GetfA(fA);

    Vector3 Temp0 = Rectangle.GetCenter() + A[0] * fA[0];
    Vector3 Temp1 = Rectangle.GetCenter() + A[0] * (-fA[0]);
    r[0] = Temp0 + A[1] * fA[1];
    r[1] = Temp0 + A[1] * (-fA[1]);
    r[2] = Temp1 + A[1] * (-fA[1]);
    r[3] = Temp1 + A[1] * fA[1];

    Segment Edge;
    Edge.Set(r[0], r[1]);

    float fT0, fT1;
    fSqrDistTmp = SquaredDistance(Edge, fT0, fT1);

    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fLineParameter = fT0;
        fRectangleParameter[0] = fA[0];
        fRectangleParameter[1] = fT1 - fA[1];
    }
    Edge.Set(r[2], r[1]);

    fSqrDistTmp = SquaredDistance(Edge, fT0, fT1);

    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fLineParameter = fT0;
        fRectangleParameter[0] = fA[1];
        fRectangleParameter[1] = fT1 - fA[0];
    }
    Edge.Set(r[3], r[2]);

    fSqrDistTmp = SquaredDistance(Edge, fT0, fT1);

    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fLineParameter = fT0;
        fRectangleParameter[0] = -fA[0];
        fRectangleParameter[1] = fT1 - fA[1];
    }
    Edge.Set(r[3], r[0]);
    fSqrDistTmp = SquaredDistance(Edge, fT0, fT1);

    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fLineParameter = fT0;
        fRectangleParameter[0] = -fA[1];
        fRectangleParameter[1] = fT1 - fA[0];
    }

    return fSqrDist;
}

float Line::SquaredDistance(const OBB &OBB, float &fLineParameter, float fOBBParameter[3])const
{
    return OBB.SquaredDistance(*this, fOBBParameter, fLineParameter);
}

float Line::Distance(const Sphere &Sphere, float &fLineParameter, Vector3 & SpherePoint)const
{
    return Sphere.Distance(*this, SpherePoint, fLineParameter);
}

float Line::Distance(const Plane &Plane, Vector3 &LinePoint, Vector3 &PlanePoint)const
{
    return Plane.Distance(*this, PlanePoint, LinePoint);
}

float Line::SquaredDistance(const AABB &AABB, float &fLineParameter, float fAABBParameter[3])const
{
    return AABB.SquaredDistance(*this, fAABBParameter, fLineParameter);
}

float Line::SquaredDistance(const Polygon & Polygon, float &fLineParameter, int& IndexTriangle, float fTriangleParameter[3])const
{
    return Polygon.SquaredDistance(*this, IndexTriangle, fTriangleParameter, fLineParameter);
}

int32 Line::RelationWith(const Triangle & Triangle, bool bCull, float &fLineParameter, float fTriangleParameter[3])const
{
    Vector3 pvec, tvec, qvec;
    Vector3 TrianglePoint[3];
    Triangle.GetPoint(TrianglePoint);
    Vector3 edge1 = TrianglePoint[1] - TrianglePoint[0];
    Vector3 edge2 = TrianglePoint[2] - TrianglePoint[0];

    pvec.Cross(m_Dir, edge2);

    float det = edge1.Dot(pvec);
    if ((bCull) && (det < EPSILON_E4))
        return MATH_NOINTERSECT;
    else if ((det < EPSILON_E4) && (det > -EPSILON_E4))
        return MATH_NOINTERSECT;

    float f_det = 1.0f / det;
    tvec = m_Orig - TrianglePoint[0];
    float u = (tvec.Dot(pvec)) * f_det;
    if (u < 0.0f || u > 1)
        return MATH_NOINTERSECT;

    qvec.Cross(tvec, edge1);
    float v = (m_Dir.Dot(qvec)) * f_det;
    if (v < 0.0f || u + v > 1)
        return MATH_NOINTERSECT;

    float f = (edge2.Dot(qvec)) * f_det;

    fTriangleParameter[1] = u;
    fTriangleParameter[2] = v;
    fTriangleParameter[0] = 1.0f - u - v;
    fLineParameter = f;

    return MATH_INTERSECT;
}

int32 Line::RelationWith(const Plane &Plane, bool bCull, float &fLineParameter)const
{
    float Vd = Plane.GetN().Dot(m_Dir);
    if (ABS(Vd) < EPSILON_E4)
    {
        return m_Orig.RelationWith(Plane);
    }

    if (bCull && (Vd > 0.0f))
        return MATH_NOINTERSECT;

    float Vo = -((Plane.GetN().Dot(m_Orig)) + Plane.GetfD());

    float _t = Vo / Vd;

    fLineParameter = _t;

    return MATH_INTERSECT;
}

int32 Line::RelationWith(const Rectangle &Rectangle, bool bCull, float &fLineParameter, float fRectangleParameter[2])const
{
    Plane Plane = Rectangle.GetPlane();
    if (bCull)
    {
        if (m_Orig.RelationWith(Plane) == MATH_BACK)
            return MATH_NOINTERSECT;
    }
    if (RelationWith(Plane, bCull, fLineParameter) == MATH_INTERSECT)
    {
        Vector3 Point = GetParameterPoint(fLineParameter);
        if (Rectangle.GetParameter(Point, fRectangleParameter))
        {
            return MATH_INTERSECT;
        }

    }

    return MATH_NOINTERSECT;
}

int32 Line::RelationWith(const OBB &OBB, uint32 &Quantity, float &tNear, float &tFar)const
{
    float e, f, t1, t2, temp;
    float	tmin = -MATH_MAX_FLOAT,
        tmax = MATH_MAX_FLOAT;
    Vector3 A[3];
    float fA[3];
    Vector3 vcP = OBB.GetCenter() - m_Orig;
    OBB.GetA(A);
    OBB.GetfA(fA);
    for (uint32 i = 0; i < 3; i++)
    {
        e = A[i].Dot(vcP);
        f = A[i].Dot(m_Dir);
        if (ABS(f) > EPSILON_E4)
        {

            t1 = (e + fA[i]) / f;
            t2 = (e - fA[i]) / f;

            if (t1 > t2) { temp = t1; t1 = t2; t2 = temp; }
            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;
            if (tmin > tmax) return MATH_NOINTERSECT;
            if (tmax < 0.0f) return MATH_NOINTERSECT;
        }
        else if (((-e - fA[i]) > 0.0f) || ((-e + fA[i]) < 0.0f))
            return MATH_NOINTERSECT;

    }

    return MATH_INTERSECT;
}

int32 Line::RelationWith(const Sphere &sphere, uint32 &Quantity, float &tNear, float &tFar)const
{
    Vector3 Delta = m_Orig - sphere.m_Center;
    float a0 = Delta.Dot(Delta) - sphere.m_fRadius * sphere.m_fRadius;
    float a1 = Delta.Dot(m_Dir);
    float Disc = a1 * a1 - a0;
    if (Disc < 0)
    {
        Quantity = 0;
    }
    else if (Disc >= EPSILON_E4)
    {
        float root = SQRT(Disc);

        tFar = -a1 + root;
        tNear = -a1 - root;
        Quantity = 2;

    }
    else
    {
        tNear = -a1;
        tFar = -a1;
        Quantity = 1;

    }

    if (Quantity == 0)
        return MATH_NOINTERSECT;
    else
        return MATH_INTERSECT;
}

int32 Line::RelationWith(const AABB &AABB, uint32 &Quantity, float &tNear, float &tFar)const
{
    float t0, t1, tmp;
    tNear = -MATH_MAX_FLOAT;
    tFar = MATH_MAX_FLOAT;
    Vector3 MaxT;
    Vector3 Min = AABB.GetMinPoint();
    Vector3 Max = AABB.GetMaxPoint();

    for (int32 i = 0; i < 3; i++)
    {
        if (ABS(m_Dir.m[i]) < EPSILON_E4)
        {
            if ((m_Orig.m[i] < Min.m[i]) ||
                (m_Orig.m[i] > Max.m[i]))
                return MATH_NOINTERSECT;
        }
        t0 = (Min.m[i] - m_Orig.m[i]) / m_Dir.m[i];
        t1 = (Max.m[i] - m_Orig.m[i]) / m_Dir.m[i];
        if (t0 > t1) { tmp = t0; t0 = t1; t1 = tmp; }
        if (t0 > tNear) tNear = t0;
        if (t1 < tFar)  tFar = t1;
        if (tNear > tFar) return MATH_NOINTERSECT;
        if (tFar < 0) return MATH_NOINTERSECT;
    }

    return MATH_INTERSECT;
}

int32 Line::RelationWith(const Polygon &Polygon, float &fLineParameter, bool bCull, int32 &iIndexTriangle, float fTriangleParameter[3])const
{
    return Polygon.RelationWith(*this, bCull, iIndexTriangle, fTriangleParameter, fLineParameter);
}

}