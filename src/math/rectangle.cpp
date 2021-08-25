#include "math/rectangle.h"
#include "math/segment.h"
#include "math/aabb.h"
#include "math/polygon.h"


namespace zq{


Rectangle::Rectangle()
{

}

Rectangle::~Rectangle()
{

}

Rectangle::Rectangle(const Vector3 & Center,const Vector3 &A0,const Vector3 & A1,float fA0, float fA1)
{
	Set(Center,A0,A1,fA0, fA1);
}

Rectangle::Rectangle(const Vector3 & Center,const Vector3 A[2],const float fA[2])
{
	Set(Center,A,fA);
}

bool Rectangle::GetParameter(const Vector3 &Point,float fRectangleParameter[2])const
{
	Vector3 A2;
	A2.Cross(m_A[0],m_A[1]);
	Matrix3X3W m;
	m.CreateInWorldObject(m_A[0],m_A[1],A2,m_Center);
	Vector3 Temp;
	Temp = Point * m;
	fRectangleParameter[0] = Temp.x;
	fRectangleParameter[1] = Temp.y;
	if(ABS(Temp.z) > EPSILON_E4 || ABS(Temp.x) > m_fA[0] || ABS(Temp.y) > m_fA[1])
		return 0;
	return 1;
}

void Rectangle::Transform(const Rectangle &Rectangle, const Matrix3X3W &Mat)
{
	m_A[0] = Mat.Apply3X3(Rectangle.m_A[0]);
	m_A[1] = Mat.Apply3X3(Rectangle.m_A[1]);
	m_A[0].Normalize();
	m_A[1].Normalize();
	m_Center = Rectangle.m_Center * Mat;
	m_fA[0] = Rectangle.m_fA[0];
	m_fA[1] = Rectangle.m_fA[1];

	Vector3 N;
	N.Cross(m_A[0],m_A[1]);
	Plane(N,m_Center);
}

float Rectangle::SquaredDistance(const Rectangle &Rectangle, float fRectangle1Parameter[2], float fRectangle2Parameter[2]) const
{
    float t, temp[2];
    Segment Edge;
    float fSqrDist = MATH_MAX_FLOAT;
    float fSqrDistTmp;
    Vector3 RectanglePoint[4];
    Rectangle.GetPoint(RectanglePoint);
    Edge.Set(RectanglePoint[1], RectanglePoint[0]);
    fSqrDistTmp = Edge.SquaredDistance(*this, t, temp);
    float fS0, fT0, fS1, fT1;
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS1 = t - Rectangle.m_fA[0];
        fT1 = Rectangle.m_fA[1];
        fS0 = temp[0];
        fT0 = temp[1];
    }

    Edge.Set(RectanglePoint[3], RectanglePoint[0]);
    fSqrDistTmp = Edge.SquaredDistance(*this, t, temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS1 = Rectangle.m_fA[0];
        fT1 = t - Rectangle.m_fA[1];
        fS0 = temp[0];
        fT0 = temp[1];
    }

    Edge.Set(RectanglePoint[2], RectanglePoint[3]);
    fSqrDistTmp = Edge.SquaredDistance(*this, t, temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS1 = t - Rectangle.m_fA[0];
        fT1 = -Rectangle.m_fA[1];
        fS0 = temp[0];
        fT0 = temp[1];
    }

    Edge.Set(RectanglePoint[2], RectanglePoint[1]);
    fSqrDistTmp = Edge.SquaredDistance(*this, t, temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS1 = -Rectangle.m_fA[0];
        fT1 = t - Rectangle.m_fA[1];
        fS0 = temp[0];
        fT0 = temp[1];
    }

    GetPoint(RectanglePoint);
    Edge.Set(RectanglePoint[1], RectanglePoint[0]);
    fSqrDistTmp = Edge.SquaredDistance(Rectangle, t, temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS0 = t - Rectangle.m_fA[0];
        fT0 = Rectangle.m_fA[1];
        fS1 = temp[0];
        fT1 = temp[1];
    }

    Edge.Set(RectanglePoint[3], RectanglePoint[0]);
    fSqrDistTmp = Edge.SquaredDistance(Rectangle, t, temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS0 = Rectangle.m_fA[0];
        fT0 = t - Rectangle.m_fA[1];
        fS1 = temp[0];
        fT1 = temp[1];
    }

    Edge.Set(RectanglePoint[2], RectanglePoint[3]);
    fSqrDistTmp = Edge.SquaredDistance(Rectangle, t, temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS0 = t - Rectangle.m_fA[0];
        fT0 = -Rectangle.m_fA[1];
        fS1 = temp[0];
        fT1 = temp[1];
    }

    Edge.Set(RectanglePoint[2], RectanglePoint[1]);
    fSqrDistTmp = Edge.SquaredDistance(Rectangle, t, temp);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        fS0 = -Rectangle.m_fA[0];
        fT0 = t - Rectangle.m_fA[1];
        fS1 = temp[0];
        fT1 = temp[1];
    }
    fRectangle1Parameter[0] = fS0;
    fRectangle1Parameter[1] = fT0;
    fRectangle2Parameter[0] = fS1;
    fRectangle2Parameter[1] = fT1;
    return fSqrDist;
}

float Rectangle::SquaredDistance(const Line& Line, float fRectangleParameter[2], float &fLineParameter)const
{
    return Line.SquaredDistance(*this, fLineParameter, fRectangleParameter);
}

float Rectangle::SquaredDistance(const OBB &OBB, float RectangleParameter[2], float OBBParameter[3])const
{
    return OBB.SquaredDistance(*this, OBBParameter, RectangleParameter);
}

float Rectangle::SquaredDistance(const Ray &Ray, float fRectangleParameter[2], float &fRayParameter)const
{
    return Ray.SquaredDistance(*this, fRayParameter, fRectangleParameter);
}

float Rectangle::SquaredDistance(const Segment & Segment, float fRectangleParameter[2], float &fSegmentParameter)const
{
    return Segment.SquaredDistance(*this, fSegmentParameter, fRectangleParameter);
}

float Rectangle::SquaredDistance(const Triangle &Triangle, float fRectangleParameter[2], float fTriangleParameter[3])const
{
    return Triangle.SquaredDistance(*this, fTriangleParameter, fRectangleParameter);
}

float Rectangle::SquaredDistance(const Vector3 &Point, float fRectangleParameter[2])const
{
    return Point.SquaredDistance(*this, fRectangleParameter);
}

float Rectangle::Distance(const Sphere &Sphere, float fRectangleParameter[2], Vector3 & SpherePoint)const
{
    return Sphere.Distance(*this, SpherePoint, fRectangleParameter);
}

float Rectangle::Distance(const Plane &Plane, Vector3 &RectanglePoint, Vector3 &PlanePoint)const
{
    return Plane.Distance(*this, PlanePoint, RectanglePoint);
}

float Rectangle::SquaredDistance(const AABB &AABB, float RectangleParameter[2], float AABBParameter[3])const
{
    return 	AABB.SquaredDistance(*this, RectangleParameter, AABBParameter);
}

float Rectangle::SquaredDistance(const Polygon &Polygon, float fRectangleParameter[2], int& IndexTriangle, float fTriangleParameter[3])const
{
    return Polygon.SquaredDistance(*this, IndexTriangle, fTriangleParameter, fRectangleParameter);
}

int32 Rectangle::RelationWith(const Line &Line, bool bCull, float fRectangleParameter[2], float &fLineParameter)const
{
    return Line.RelationWith(*this, 1, fLineParameter, fRectangleParameter);
}

int32 Rectangle::RelationWith(const Ray &Ray, bool bCull, float fRectangleParameter[2], float &fRayParameter)const
{
    return Ray.RelationWith(*this, 1, fRayParameter, fRectangleParameter);
}

int32 Rectangle::RelationWith(const Segment &Segment, bool bCull, float fRectangleParameter[2], float &fSegmentParameter)const
{
    return Segment.RelationWith(*this, 1, fSegmentParameter, fRectangleParameter);
}

int32 Rectangle::RelationWith(const Plane &Plane)const
{
    int32 iFrontNum = 0;
    int32 iBackNum = 0;
    Vector3 Point[4];
    GetPoint(Point);
    for (int32 i = 0; i < 4; i++)
    {
        int32 iFlag = Point[i].RelationWith(Plane);
        if (iFlag == MATH_FRONT)
        {
            iFrontNum++;
        }
        else if (iFlag == MATH_BACK)
        {
            iBackNum++;
        }


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

int32 Rectangle::RelationWith(const Triangle &Triangle)const
{
    return Triangle.RelationWith(*this);
}

int32 Rectangle::RelationWith(const Rectangle &Rectangle)const
{
    Vector3 Point[4];
    GetPoint(Point);

    Segment Segment;
    for (int32 i = 0; i < 4; i++)
    {
        int32 j = i + 1;
        if (j == 4)
            j = 0;
        Segment.Set(Point[i], Point[j]);
        float fSegmentParameter;
        float RectangleParameter[2];
        if (Segment.RelationWith(Rectangle, 0, fSegmentParameter, RectangleParameter) == MATH_INTERSECT)
            return MATH_INTERSECT;

    }
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

int32 Rectangle::RelationWith(const AABB &AABB)const
{
    Plane Plane = GetPlane();
    if (Plane.RelationWith(AABB) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    int32 InNum = 0, OutNum = 0;
    Vector3 Point[4];
    GetPoint(Point);
    for (int32 i = 0; i < 4; i++)
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
    /*Plane Plane = GetPlane();
    if(Plane.RelationWith(AABB) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    Vector3 Point[4];
    GetPoint(Point);
    Segment Segment;
    int32 InNum = 0;
    for(int32 i = 0 ; i < 4 ; i++)
    {
        int32 j = i + 1;
        if(j == 4)
            j = 0;
        Segment.Set(Point[i],Point[j]);
        float fNear,fFar;
        uint32 uiQuantity;
        int32 iFlag = Segment.RelationWith(AABB,uiQuantity,fNear,fFar);
        if( iFlag == MATH_INTERSECT)
            return MATH_INTERSECT;
        else if(iFlag == MATH_IN)
            InNum ++;


    }
    if(InNum == 4)
        return MATH_IN;
    return MATH_NOINTERSECT;*/
}

int32 Rectangle::RelationWith(const OBB &OBB)const
{
    Plane Plane = GetPlane();
    if (Plane.RelationWith(OBB) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    int32 InNum = 0, OutNum = 0;
    Vector3 Point[4];
    GetPoint(Point);
    for (int32 i = 0; i < 4; i++)
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
    Vector3 Point[4];
    GetPoint(Point);
    Segment Segment;
    int32 InNum = 0;
    for(int32 i = 0 ; i < 4 ; i++)
    {
        int32 j = i + 1;
        if(j == 4)
            j = 0;
        Segment.Set(Point[i],Point[j]);
        float fNear,fFar;
        uint32 uiQuantity;
        int32 iFlag = Segment.RelationWith(OBB,uiQuantity,fNear,fFar);
        if( iFlag == MATH_INTERSECT)
            return MATH_INTERSECT;
        else if(iFlag == MATH_IN)
            InNum ++;


    }
    if(InNum == 4)
        return MATH_IN;
    return MATH_NOINTERSECT;*/
}

int32 Rectangle::RelationWith(const Sphere & Sphere)const
{
    Plane Plane = GetPlane();
    if (Plane.RelationWith(Sphere) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    int32 InNum = 0, OutNum = 0;
    Vector3 Point[4];
    GetPoint(Point);
    for (int32 i = 0; i < 4; i++)
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
    Vector3 Point[4];
    GetPoint(Point);
    Segment Segment;
    int32 InNum = 0;
    for(int32 i = 0 ; i < 4 ; i++)
    {
        int32 j = i + 1;
        if(j == 4)
            j = 0;
        Segment.Set(Point[i],Point[j]);
        float fNear,fFar;
        uint32 uiQuantity;
        int32 iFlag = Segment.RelationWith(Sphere,uiQuantity,fNear,fFar);
        if( iFlag == MATH_INTERSECT)
            return MATH_INTERSECT;
        else if(iFlag == MATH_IN)
            InNum ++;


    }
    if(InNum == 4)
        return MATH_IN;
    return MATH_NOINTERSECT;*/

}

int32 Rectangle::RelationWith(const Plane& plane, Segment& segment)const
{
    Segment segmentTemp;
    int32 InNum = 0;
    int32 OnNum = 0, BackNum = 0, FrontNum = 0;
    Vector3 Orig, End;
    Vector3 Point[4];
    GetPoint(Point);
    for (int32 i = 0; i < 4; i++)
    {
        int32 j = i + 1;
        if (j == 4)
            j = 0;
        segmentTemp.Set(Point[i], Point[j]);
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
    if (OnNum == 4)
        return MATH_VSON;
    if (BackNum == 4)
        return MATH_BACK;
    if (FrontNum == 4)
        return MATH_FRONT;

    //not control all path
    return MATH_INTERSECT;
}

int32 Rectangle::RelationWith(const Triangle& triangle, Segment& segment)const
{
    return triangle.RelationWith(*this, segment);
}

int32 Rectangle::RelationWith(const Rectangle& rectangle, Segment& segment)const
{
    Segment segmentTemp;
    int32 InNum = 0;
    Vector3 Orig, End;
    Vector3 Point[4];
    GetPoint(Point);
    for (int32 i = 0; i < 4; i++)
    {
        int32 j = i + 1;
        if (j == 4)
            j = 0;
        segmentTemp.Set(Point[i], Point[j]);
        float fSegmentParameter;
        float fRectangleParameter[2];
        int32 iFlag = segmentTemp.RelationWith(rectangle, 0, fSegmentParameter, fRectangleParameter);
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


    rectangle.GetPoint(Point);
    for (int32 i = 0; i < 4; i++)
    {
        int32 j = i + 1;
        if (j == 4)
            j = 0;
        segmentTemp.Set(Point[i], Point[j]);
        float fSegmentParameter;
        float fRectangleParameter[2];
        int32 iFlag = segmentTemp.RelationWith(*this, 0, fSegmentParameter, fRectangleParameter);
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
    if (!InNum)
        return MATH_NOINTERSECT;

    //not control all path
    return MATH_INTERSECT;
}



}
