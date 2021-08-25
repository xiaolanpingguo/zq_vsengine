#include "math/triangle.h"
#include "math/segment.h"
#include "math/aabb.h"
#include "math/polygon.h"


namespace zq{


Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

Triangle::Triangle(const Vector3 & P0,const Vector3 &P1,const Vector3 &P2)
{
	Set(P0,P1,P2);	
}

Triangle::Triangle(const Vector3 Point[3])
{
	Set(Point);
}

bool Triangle::GetParameter(const Vector3 &Point,float fTriangleParameter[3])const
{
	Vector3 v01, v02,v0p;
	Vector3 vp1, vp2;
	v01 = m_V[1] - m_V[0];
	v02 = m_V[2] - m_V[0];
	v0p = Point - m_V[0];
	vp1 = m_V[1] - Point;
	vp2 = m_V[2] - Point;

	Vector3 S[4];

	S[0].Cross(v01,v02);
	S[1].Cross(v01,v0p);
	S[2].Cross(v0p,v02);
	S[3].Cross(vp1,vp2);

	float fS[4];
	for(int32 i = 0 ;i < 4 ; i++)
	{
		fS[i] = S[i].GetLength();
	}
	for(int32 i = 0 ; i < 3 ; i++)
	{
		fTriangleParameter[i] = fS[i+1] / fS[0];
	}
	float fSum = 0.0f;
	for(int32 i = 0 ; i < 3 ; i++)
	{
		fSum += fTriangleParameter[i];
		if(fTriangleParameter[i] < 0.0f || fTriangleParameter[i] > 1.0f)
			return 0;

	}
	if(fSum > 1.0f)
		return 0;
	return 1;
	/*Vector3 V[3];
	V[0] = Point - m_V[0];
	V[1] = m_V[1] - m_V[0];
	V[2] = m_V[2] - m_V[0];
	float s1,t1,s2,t2,k1,k2;
	bool flag1 = 0;
	bool flag2 = 0;
	if(ABS(V[1].x) > EPSILON_E4 && ABS(V[2].x) > EPSILON_E4)
	{
		s1 = V[1].x;
		t1 = V[2].x;
		k1 = V[0].x;
		flag1 = 1;
	}

	if(ABS(V[1].y) > EPSILON_E4 && ABS(V[2].y) > EPSILON_E4)
	{
		if(flag1 ==0)
		{
			s1 = V[1].y;
			t1 = V[2].y;
			k1 = V[0].y;
			flag1 = 1;
		}
		else
		{
			if(ABS((V[1].x * V[2].y) - (V[1].y * V[2].x))> EPSILON_E4)
			{
				flag2 = 1;
				s2 = V[1].y;
				t2 = V[2].y;
				k2 = V[0].y;
			}
		}

	}

	if(ABS(V[1].z) > EPSILON_E4 && ABS(V[2].z) > EPSILON_E4)
	{
		if(flag1 ==0)
		{
			return 0;
		}
		else
		{
			if(flag2 == 0)
			{
				s2 = V[1].z;
				t2 = V[2].z;
				k2 = V[0].z;
				flag2 = 1;
			}
		}

	}

	fTriangleParameter[1] = (t1 * k2 - t2 * k1) / (t1 * s2 - t2 * s1);
	fTriangleParameter[2] = (s1 * k2 - s2 * k1) / (s1 * t2 - s2 * t1);
	fTriangleParameter[0] = 1.0f - fTriangleParameter[1] - fTriangleParameter[2];

	if(fTriangleParameter[0] > 1.0f || fTriangleParameter[1] > 1.0f || fTriangleParameter[2] > 1.0f)
		return 0;
	return 1;
	*/
}

void Triangle::Transform(const Triangle & Triangle,const Matrix3X3W &Mat)
{
	m_V[0] = Triangle.m_V[0] * Mat;
	m_V[1] = Triangle.m_V[1] * Mat;
	m_V[2] = Triangle.m_V[2] * Mat;
	Set(m_V);
}

float Triangle::SquaredDistance(const Line &Line, float fTriangleParameter[3], float &fLineParameter)const
{
    return Line.SquaredDistance(*this, fLineParameter, fTriangleParameter);
}

float Triangle::SquaredDistance(const OBB &OBB, float TriangleParameter[3], float OBBParameter[3])const
{
    return OBB.SquaredDistance(*this, OBBParameter, TriangleParameter);
}

float Triangle::SquaredDistance(const Ray & Ray, float fTriangleParameter[3], float &fRayParameter)const
{
    return Ray.SquaredDistance(*this, fRayParameter, fTriangleParameter);
}

float Triangle::SquaredDistance(const Segment& Segment, float fTriangleParameter[3], float &fSegmentParameter)const
{
    return Segment.SquaredDistance(*this, fSegmentParameter, fTriangleParameter);
}

float Triangle::SquaredDistance(const Vector3 & Point, float fTriangleParameter[3])const
{
    return Point.SquaredDistance(*this, fTriangleParameter);
}

float Triangle::SquaredDistance(const Triangle &Triangle, float fTriangle1Parameter[3], float fTriangle2Parameter[3])const
{
    float fSqrDist = MATH_MAX_FLOAT;
    float fSqrDistTmp;
    Segment Edge;
    float Tempt0, Temp[3];
    float fR0, fS0, fT0, fR1, fS1, fT1;
    Edge.Set(m_V[0], m_V[1]);
    fSqrDistTmp = Edge.SquaredDistance(Triangle, Tempt0, Temp);

    if (fSqrDistTmp < fSqrDist)
    {

        fS0 = Tempt0 / Edge.GetLen();
        fR0 = 1.0f - fS0;
        fT0 = 0.0f;
        fR1 = Temp[0];
        fS1 = Temp[1];
        fT1 = Temp[2];
        fSqrDist = fSqrDistTmp;
    }

    Edge.Set(m_V[0], m_V[2]);
    fSqrDistTmp = Edge.SquaredDistance(Triangle, Tempt0, Temp);
    if (fSqrDistTmp < fSqrDist)
    {


        fS0 = 0.0f;
        fT0 = Tempt0 / Edge.GetLen();
        fR0 = 1.0f - fT0;

        fR1 = Temp[0];
        fS1 = Temp[1];
        fT1 = Temp[2];

        fSqrDist = fSqrDistTmp;
    }

    Edge.Set(m_V[1], m_V[2]);
    fSqrDistTmp = Edge.SquaredDistance(Triangle, Tempt0, Temp);

    if (fSqrDistTmp < fSqrDist)
    {
        fS0 = Tempt0 / Edge.GetLen();
        fT0 = 1.0f - fS0;
        fR0 = 0.0f;

        fR1 = Temp[0];
        fS1 = Temp[1];
        fT1 = Temp[2];

        fSqrDist = fSqrDistTmp;
    }

    Edge.Set(Triangle.m_V[0], Triangle.m_V[1]);
    fSqrDistTmp = Edge.SquaredDistance(*this, Tempt0, Temp);

    if (fSqrDistTmp < fSqrDist)
    {

        fS1 = Tempt0 / Edge.GetLen();
        fR1 = 1.0f - fS0;
        fT1 = 0.0f;
        fR0 = Temp[0];
        fS0 = Temp[1];
        fT0 = Temp[2];
        fSqrDist = fSqrDistTmp;
    }

    Edge.Set(Triangle.m_V[0], Triangle.m_V[2]);
    fSqrDistTmp = Edge.SquaredDistance(*this, Tempt0, Temp);

    if (fSqrDistTmp < fSqrDist)
    {


        fS1 = 0.0f;
        fT1 = Tempt0 / Edge.GetLen();
        fR1 = 1.0f - fT0;

        fR0 = Temp[0];
        fS0 = Temp[1];
        fT0 = Temp[2];

        fSqrDist = fSqrDistTmp;
    }

    Edge.Set(Triangle.m_V[1], Triangle.m_V[2]);
    fSqrDistTmp = Edge.SquaredDistance(*this, Tempt0, Temp);

    if (fSqrDistTmp < fSqrDist)
    {

        fS1 = Tempt0 / Edge.GetLen();
        fT1 = 1.0f - fS0;
        fR1 = 0.0f;

        fR0 = Temp[0];
        fS0 = Temp[1];
        fT0 = Temp[2];

        fSqrDist = fSqrDistTmp;
    }

    fTriangle1Parameter[0] = fR0;
    fTriangle1Parameter[1] = fS0;
    fTriangle1Parameter[2] = fT0;

    fTriangle2Parameter[0] = fR1;
    fTriangle2Parameter[1] = fS1;
    fTriangle2Parameter[2] = fT1;
    return fSqrDist;
}

float Triangle::SquaredDistance(const Rectangle &Rectangle, float fTriangleParameter[3], float fRectangleParameter[2])const
{
    Vector3 RectanglePoint[4];
    Rectangle.GetPoint(RectanglePoint);
    Segment Edge;
    //Vector3 A[2];
    //Rectangle.GetA(A);
    float fA[2];
    Rectangle.GetfA(fA);
    float t, Temp[3];
    float fSqrDist = MATH_MAX_FLOAT;
    float fSqrDistTmp;
    float fR0, fS0, fT0, fT1, fS1;
    Edge.Set(RectanglePoint[1], RectanglePoint[0]);
    fSqrDistTmp = Edge.SquaredDistance(*this, t, Temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;

        fS1 = t - fA[0];
        fT1 = fA[1];
        fR0 = Temp[0];
        fS0 = Temp[1];
        fT0 = Temp[2];

    }
    Edge.Set(RectanglePoint[3], RectanglePoint[0]);
    fSqrDistTmp = Edge.SquaredDistance(*this, t, Temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS1 = fA[0];
        fT1 = t - fA[1];
        fR0 = Temp[0];
        fS0 = Temp[1];
        fT0 = Temp[2];
    }

    Edge.Set(RectanglePoint[2], RectanglePoint[3]);
    fSqrDistTmp = Edge.SquaredDistance(*this, t, Temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS1 = t - fA[0];
        fT1 = -fA[1];
        fR0 = Temp[0];
        fS0 = Temp[1];
        fT0 = Temp[2];
    }
    Edge.Set(RectanglePoint[2], RectanglePoint[1]);
    fSqrDistTmp = Edge.SquaredDistance(*this, t, Temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS1 = -fA[0];
        fT1 = t - fA[1];
        fR0 = Temp[0];
        fS0 = Temp[1];
        fT0 = Temp[2];
    }

    Edge.Set(m_V[0], m_V[1]);
    fSqrDistTmp = Edge.SquaredDistance(Rectangle, t, Temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fS0 = t / Edge.GetLen();
        fR0 = 1.0f - fS0;
        fT0 = 0.0f;
        fS1 = Temp[0];
        fT1 = Temp[1];
        fSqrDist = fSqrDistTmp;
    }

    Edge.Set(m_V[0], m_V[2]);
    fSqrDistTmp = Edge.SquaredDistance(Rectangle, t, Temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fS0 = 0.0f;
        fT0 = t / Edge.GetLen();
        fR0 = 1.0f - fT0;

        fS1 = Temp[0];
        fT1 = Temp[1];
        fSqrDist = fSqrDistTmp;
    }

    Edge.Set(m_V[1], m_V[2]);
    fSqrDistTmp = Edge.SquaredDistance(Rectangle, t, Temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fS0 = t / Edge.GetLen();
        fT0 = 1.0f - fS0;
        fR0 = 0.0f;

        fS1 = Temp[0];
        fT1 = Temp[1];
        fSqrDist = fSqrDistTmp;
    }

    fTriangleParameter[0] = fR0;
    fTriangleParameter[1] = fS0;
    fTriangleParameter[2] = fT0;
    fRectangleParameter[0] = fS1;
    fRectangleParameter[2] = fT1;
    return fSqrDist;
}

float Triangle::Distance(const Sphere &Sphere, float fTriangleParameter[3], Vector3 & SpherePoint)const
{
    return Sphere.Distance(*this, SpherePoint, fTriangleParameter);
}

float Triangle::Distance(const Plane &Plane, Vector3 &TrianglePoint, Vector3 &PlanePoint)const
{
    return Plane.Distance(*this, PlanePoint, TrianglePoint);
}

float Triangle::SquaredDistance(const AABB &AABB, float TriangleParameter[3], float AABBParameter[3])const
{
    return AABB.SquaredDistance(*this, AABBParameter, TriangleParameter);
}

float Triangle::SquaredDistance(const Polygon& Polygon, float fTriangle1Parameter[3], int& IndexTriangle, float fTriangle2Parameter[3])const
{
    return Polygon.SquaredDistance(*this, IndexTriangle, fTriangle2Parameter, fTriangle1Parameter);
}

int32 Triangle::RelationWith(const Line &Line, bool bCull, float fTriangleParameter[3], float &fLineParameter)const
{
    return Line.RelationWith(*this, bCull, fLineParameter, fTriangleParameter);
}

int32 Triangle::RelationWith(const Ray & Ray, bool bCull, float fTriangleParameter[3], float &fRayParameter)const
{
    return Ray.RelationWith(*this, bCull, fRayParameter, fTriangleParameter);
}

int32 Triangle::RelationWith(const Segment & Segment, bool bCull, float fTriangleParameter[3], float &fSegmentParameter)const
{
    return Segment.RelationWith(*this, bCull, fSegmentParameter, fTriangleParameter);
}

int32 Triangle::RelationWith(const Plane & Plane)const
{
    int32 iFrontNum = 0;
    int32 iBackNum = 0;

    for (int32 i = 0; i < 3; i++)
    {
        int32 iFlag = m_V[i].RelationWith(Plane);
        if (iFlag == MATH_FRONT)
        {
            iFrontNum++;
        }
        else if (iFlag == MATH_BACK)
        {
            iBackNum++;
        }
        if (iFrontNum > 0 && iBackNum > 0)
            return MATH_INTERSECT;


    }
    if (iFrontNum == 0 && iBackNum == 0)
    {
        return MATH_VSON;
    }
    else if (iFrontNum == 0)
        return MATH_BACK;
    else if (iBackNum == 0)
        return MATH_FRONT;
    else
        return MATH_INTERSECT;
}

int32 Triangle::RelationWith(const Triangle & Triangle)const
{
    Segment Segment;
    for (int32 i = 0; i < 3; i++)
    {
        int32 j = i + 1;
        if (j == 3)
            j = 0;
        Segment.Set(m_V[i], m_V[j]);
        float fSegmentParameter;
        float TriangleParameter[3];
        if (Segment.RelationWith(Triangle, 0, fSegmentParameter, TriangleParameter) == MATH_INTERSECT)
            return MATH_INTERSECT;

    }
    for (int32 i = 0; i < 3; i++)
    {
        int32 j = i + 1;
        if (j == 3)
            j = 0;
        Segment.Set(Triangle.m_V[i], Triangle.m_V[j]);
        float fSegmentParameter;
        float TriangleParameter[3];
        if (Segment.RelationWith(*this, 0, fSegmentParameter, TriangleParameter) == MATH_INTERSECT)
            return MATH_INTERSECT;
    }

    return MATH_NOINTERSECT;
}

int32 Triangle::RelationWith(const Rectangle & Rectangle)const
{
    Segment Segment;
    for (int32 i = 0; i < 3; i++)
    {
        int32 j = i + 1;
        if (j == 3)
            j = 0;
        Segment.Set(m_V[i], m_V[j]);
        float fSegmentParameter;
        float RectangleParameter[2];
        if (Segment.RelationWith(Rectangle, 0, fSegmentParameter, RectangleParameter) == MATH_INTERSECT)
            return MATH_INTERSECT;
    }

    Vector3 Point[4];
    Rectangle.GetPoint(Point);
    for (int32 i = 0; i < 4; i++)
    {
        int32 j = i + 1;
        if (j == 4)
            j = 0;
        Segment.Set(Point[i], Point[j]);
        float fSegmentParameter;
        float RectangleParameter[2];
        if (Segment.RelationWith(*this, 0, fSegmentParameter, RectangleParameter) == MATH_INTERSECT)
            return MATH_INTERSECT;
    }

    return MATH_NOINTERSECT;
}

int32 Triangle::RelationWith(const AABB &AABB)const
{
    Plane Plane = GetPlane();
    if (Plane.RelationWith(AABB) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    int32 InNum = 0, OutNum = 0;
    Vector3 Point[3];
    GetPoint(Point);
    for (int32 i = 0; i < 3; i++)
    {
        int32 iFlag = Point[i].RelationWith(AABB);
        if (iFlag == MATH_IN || iFlag == MATH_VSON)
        {
            InNum++;

        }
        else if (iFlag == MATH_OUT)
        {

            OutNum++;
        }
        if (InNum > 0 && OutNum > 0)
            return MATH_INTERSECT;
    }
    if (InNum == 0)
        return MATH_OUT;
    if (OutNum == 0)
        return MATH_IN;
    else
        return MATH_INTERSECT;
    /*Segment Segment;
    int32 InNum = 0;
    for(int32 i = 0 ; i < 3 ; i++)
    {
        int32 j = i + 1;
        if(j == 3)
            j = 0;
        Segment.Set(m_V[i],m_V[j]);
        float fNear,fFar;
        uint32 uiQuantity;
        int32 iFlag = Segment.RelationWith(AABB,uiQuantity,fNear,fFar);
        if( iFlag == MATH_INTERSECT)
            return MATH_INTERSECT;
        else if(iFlag == MATH_IN)
            InNum ++;
    }

    if(InNum == 3)
        return MATH_IN;

    return MATH_NOINTERSECT;*/
}

int32 Triangle::RelationWith(const OBB &OBB)const
{
    Plane Plane = GetPlane();
    if (Plane.RelationWith(OBB) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    int32 InNum = 0, OutNum = 0;
    Vector3 Point[3];
    GetPoint(Point);
    for (int32 i = 0; i < 3; i++)
    {
        int32 iFlag = Point[i].RelationWith(OBB);
        if (iFlag == MATH_IN || iFlag == MATH_VSON)
        {
            InNum++;

        }
        else if (iFlag == MATH_OUT)
        {

            OutNum++;
        }
        if (InNum > 0 && OutNum > 0)
            return MATH_INTERSECT;
    }
    if (InNum == 0)
        return MATH_OUT;
    if (OutNum == 0)
        return MATH_IN;
    else
        return MATH_INTERSECT;
    /*Plane Plane = GetPlane();
    if(Plane.RelationWith(OBB) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    Segment Segment;
    int32 InNum = 0;
    for(int32 i = 0 ; i < 3 ; i++)
    {
        int32 j = i + 1;
        if(j == 3)
            j = 0;
        Segment.Set(m_V[i],m_V[j]);
        float fNear,fFar;
        uint32 uiQuantity;
        int32 iFlag = Segment.RelationWith(OBB,uiQuantity,fNear,fFar);
        if( iFlag == MATH_INTERSECT)
            return MATH_INTERSECT;
        else if(iFlag == MATH_IN)
            InNum ++;


    }
    if(InNum == 3)
        return MATH_IN;
    return MATH_NOINTERSECT;*/
}

int32 Triangle::RelationWith(const Sphere &Sphere)const
{

    Plane Plane = GetPlane();
    if (Plane.RelationWith(Sphere) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    int32 InNum = 0, OutNum = 0;
    Vector3 Point[3];
    GetPoint(Point);
    for (int32 i = 0; i < 3; i++)
    {
        int32 iFlag = Point[i].RelationWith(Sphere);
        if (iFlag == MATH_IN || iFlag == MATH_VSON)
        {
            InNum++;

        }
        else if (iFlag == MATH_OUT)
        {

            OutNum++;
        }
        if (InNum > 0 && OutNum > 0)
            return MATH_INTERSECT;
    }
    if (InNum == 0)
        return MATH_OUT;
    if (OutNum == 0)
        return MATH_IN;
    else
        return MATH_INTERSECT;
    /*Plane Plane = GetPlane();
    if(Plane.RelationWith(Sphere) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    Segment Segment;
    int32 InNum = 0;
    for(int32 i = 0 ; i < 3 ; i++)
    {
        int32 j = i + 1;
        if(j == 3)
            j = 0;
        Segment.Set(m_V[i],m_V[j]);
        float fNear,fFar;
        uint32 uiQuantity;
        int32 iFlag = Segment.RelationWith(Sphere,uiQuantity,fNear,fFar);
        if( iFlag == MATH_INTERSECT)
            return MATH_INTERSECT;
        else if(iFlag == MATH_IN)
            InNum ++;


    }
    if(InNum == 3)
        return MATH_IN;
    return MATH_NOINTERSECT;*/

}

int32 Triangle::RelationWith(const Plane& plane, Segment& segment)const
{
    Segment segmentTemp;
    int32 InNum = 0;
    int32 OnNum = 0, BackNum = 0, FrontNum = 0;
    Vector3 Orig, End;
    for (int32 i = 0; i < 3; i++)
    {
        int32 j = i + 1;
        if (j == 3)
            j = 0;
        segmentTemp.Set(m_V[i], m_V[j]);
        float t;
        int32 iFlag = segmentTemp.RelationWith(plane, 0, t);
        if (iFlag == MATH_INTERSECT)
        {
            InNum++;
            if (InNum == 1)
            {
                Orig = segmentTemp.GetParameterPoint(t);

            }
            else if (InNum == 2)
            {
                End = segmentTemp.GetParameterPoint(t);
                segment.Set(Orig, End);
                return MATH_INTERSECT;
            }
        }
        else if (iFlag == MATH_VSON)
        {
            OnNum++;
        }
        else if (iFlag == MATH_BACK)
        {
            BackNum++;
        }
        else if (iFlag == MATH_FRONT)
        {
            FrontNum++;
        }

    }
    if (OnNum == 3)
        return MATH_VSON;
    if (BackNum == 3)
        return MATH_BACK;
    if (FrontNum == 3)
        return MATH_FRONT;

    //not control all path
    return MATH_INTERSECT;
}

int32 Triangle::RelationWith(const Triangle& triangle, Segment& segment)const
{
    Segment segmentTemp;
    int32 InNum = 0;
    Vector3 Orig, End;
    for (int32 i = 0; i < 3; i++)
    {
        int32 j = i + 1;
        if (j == 3)
            j = 0;
        segmentTemp.Set(m_V[i], m_V[j]);
        float fTriangleParameter[3];
        float fSegmentParameter;
        int32 iFlag = segmentTemp.RelationWith(triangle, 0, fSegmentParameter, fTriangleParameter);
        if (iFlag == MATH_INTERSECT)
        {
            InNum++;
            if (InNum == 1)
            {
                Orig = segmentTemp.GetParameterPoint(fSegmentParameter);

            }
            else if (InNum == 2)
            {
                End = segmentTemp.GetParameterPoint(fSegmentParameter);
                segment.Set(Orig, End);
                return MATH_INTERSECT;
            }
        }
    }

    for (int32 i = 0; i < 3; i++)
    {
        int32 j = i + 1;
        if (j == 3)
            j = 0;
        segmentTemp.Set(triangle.m_V[i], triangle.m_V[j]);
        float fTriangleParameter[3];
        float fSegmentParameter;
        int32 iFlag = segmentTemp.RelationWith(*this, 0, fSegmentParameter, fTriangleParameter);
        if (iFlag == MATH_INTERSECT)
        {
            InNum++;
            if (InNum == 1)
            {
                Orig = segmentTemp.GetParameterPoint(fSegmentParameter);

            }
            else if (InNum == 2)
            {
                End = segmentTemp.GetParameterPoint(fSegmentParameter);
                segment.Set(Orig, End);
                return MATH_INTERSECT;
            }
        }
    }

    return MATH_NOINTERSECT;
}

int32 Triangle::RelationWith(const Rectangle& rectangle, Segment& segment)const
{
    Segment segmentTemp;
    int32 InNum = 0;
    Vector3 Orig, End;
    for (int32 i = 0; i < 3; i++)
    {
        int32 j = i + 1;
        if (j == 3)
            j = 0;
        segmentTemp.Set(m_V[i], m_V[j]);
        float fTriangleParameter[2];
        float fSegmentParameter;
        int32 iFlag = segmentTemp.RelationWith(rectangle, 0, fSegmentParameter, fTriangleParameter);
        if (iFlag == MATH_INTERSECT)
        {
            InNum++;
            if (InNum == 1)
            {
                Orig = segmentTemp.GetParameterPoint(fSegmentParameter);

            }
            else if (InNum == 2)
            {
                End = segmentTemp.GetParameterPoint(fSegmentParameter);
                segment.Set(Orig, End);
                return MATH_INTERSECT;
            }
        }


    }

    Vector3 Point[4];
    rectangle.GetPoint(Point);
    for (int32 i = 0; i < 4; i++)
    {
        int32 j = i + 1;
        if (j == 4)
            j = 0;
        segmentTemp.Set(Point[i], Point[j]);
        float fTriangleParameter[3];
        float fSegmentParameter;
        int32 iFlag = segmentTemp.RelationWith(*this, 0, fSegmentParameter, fTriangleParameter);
        if (iFlag == MATH_INTERSECT)
        {
            InNum++;
            if (InNum == 1)
            {
                Orig = segmentTemp.GetParameterPoint(fSegmentParameter);

            }
            else if (InNum == 2)
            {
                End = segmentTemp.GetParameterPoint(fSegmentParameter);
                segment.Set(Orig, End);
                return MATH_INTERSECT;
            }
        }
    }

    return MATH_NOINTERSECT;
}

}
