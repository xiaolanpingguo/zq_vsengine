#include "math/polygon.h"
#include "math/aabb.h"
#include "math/plane.h"
#include "math/segment.h"
#include "math/triangle.h"


namespace zq{


Polygon::Polygon()
{
	m_PointNum = 0;		//点个数
	m_IndexNum = 0;		//索引个数
	m_Flag = 0;			// 标志位
	m_pPoint = NULL;		// 点列表
	m_pIndex = NULL;		// 索引列表
}

Polygon::~Polygon()
{
	SAFE_DELETEA(m_pPoint);
	SAFE_DELETEA(m_pIndex);
}

Polygon::Polygon(const Vector3 *pPoint,uint32 PointNum)
{
	Set(pPoint,PointNum);
}

bool Polygon::CopyFrom(const Polygon & Polygon)
{
	if(!Polygon.m_pPoint || !Polygon.m_PointNum || !Polygon.m_pIndex || !Polygon.m_IndexNum)
		return 0;
	m_PointNum = Polygon.m_PointNum;	
	m_IndexNum = Polygon.m_IndexNum;		
	m_Flag = Polygon.m_Flag;
	SAFE_DELETEA(m_pPoint);
	SAFE_DELETEA(m_pIndex);
	m_pPoint = MEM_NEW Vector3[m_PointNum];
	VSMAC_ASSERT(m_pPoint);

	m_pIndex = MEM_NEW MATH_USHORT_INDEX[m_IndexNum];
	VSMAC_ASSERT(m_pIndex);
	VSMemcpy(m_pPoint,Polygon.m_pPoint,sizeof(Vector3) * m_PointNum);
	VSMemcpy(m_pIndex,Polygon.m_pIndex,sizeof(MATH_USHORT_INDEX) * m_IndexNum);

	return 1;
}

void Polygon::SwapFaces()
{
	MATH_USHORT_INDEX *pIndis = new MATH_USHORT_INDEX[m_IndexNum];

	for (uint32 i = 0; i < m_IndexNum ; i++)
		pIndis[m_IndexNum - i - 1] = m_pIndex[i];

	m_Flag = !m_Flag;

	Plane::Set(m_N * (-1.0f),m_fD);

	SAFE_DELETEA(m_pIndex);
	m_pIndex = pIndis;
}

void Polygon::Clip(const Plane &Plane, Polygon &pFront, Polygon &pBack)const
{
    int			nNumFront = 0,     // 前面多边形点的个数
        nNumBack = 0,      // 后面多边形点的个数
        nLoop = 0,
        nCurrent = 0;

    Vector3 *pvcFront = MEM_NEW Vector3[m_PointNum * 3];
    Vector3 *pvcBack = MEM_NEW Vector3[m_PointNum * 3];

    for (uint32 i = 0; i < m_PointNum; i++)
    {
        int32 j = i + 1;
        if (j == m_PointNum)
        {
            j = 0;
        }
        int32 iFlagi = Plane.RelationWith(m_pPoint[i]);
        int32 iFlagj = Plane.RelationWith(m_pPoint[j]);
        if (iFlagi == MATH_VSON)
        {
            pvcFront[nNumFront] = m_pPoint[i];
            nNumFront++;
            pvcBack[nNumBack] = m_pPoint[i];
            nNumBack++;
        }
        else
        {
            Segment Segment;
            Segment.Set(m_pPoint[i], m_pPoint[j]);
            float t;
            if (Segment.RelationWith(Plane, 0, t) == MATH_INTERSECT && iFlagj != MATH_VSON)
            {
                Vector3 Hit = Segment.GetParameterPoint(t);
                pvcFront[nNumFront] = Hit;
                nNumFront++;
                pvcBack[nNumBack] = Hit;
                nNumBack++;
            }

            if (iFlagi == MATH_FRONT)
            {

                pvcFront[nNumFront++] = m_pPoint[i];
            }
            else if (iFlagi == MATH_BACK)
            {

                pvcBack[nNumBack++] = m_pPoint[i];
            }
        }
    }

    pFront.Set(pvcFront, nNumFront);
    if (pFront.m_N.Dot(m_N) < 0.0f)
        pFront.SwapFaces();
    pBack.Set(pvcBack, nNumBack);
    if (pBack.m_N.Dot(m_N) < 0.0f)
        pBack.SwapFaces();

    SAFE_DELETEA(pvcFront);
    SAFE_DELETEA(pvcBack);
}

void Polygon::Clip(const AABB &ABBB)
{
    Plane Plane[6];
    ABBB.GetPlane(Plane);
    Polygon Front, Back;
    for (int32 i = 0; i < 6; i++)
    {
        if (RelationWith(Plane[i]) == MATH_INTERSECT)
        {
            Clip(Plane[i], Front, Back);
            CopyFrom(Back);
        }
    }
}

float Polygon::SquaredDistance(const Vector3 & Point, int& IndexTriangle, float fTriangleParameter[3]) const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        float fParaTemp[3];
        float dist = TriangleTemp.SquaredDistance(Point, fParaTemp);
        if (distTemp < dist)
        {
            distTemp = dist;
            for (int32 j = 0; j < 3; j++)
                fTriangleParameter[j] = fParaTemp[j];
            IndexTriangle = i;
        }
    }

    return distTemp;
}

float Polygon::SquaredDistance(const Triangle& triangle, int& IndexTriangle, float fTriangle1Parameter[3], float fTriangle2Parameter[3])const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle triangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        triangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        float fPara1Temp[3];
        float fPara2Temp[3];
        float dist = triangleTemp.SquaredDistance(triangle, fPara1Temp, fPara2Temp);
        if (distTemp < dist)
        {
            distTemp = dist;
            for (int32 j = 0; j < 3; j++)
            {
                fTriangle1Parameter[j] = fPara1Temp[j];
                fTriangle2Parameter[j] = fPara2Temp[j];
            }

            IndexTriangle = i;
        }
    }

    return distTemp;
}

float Polygon::SquaredDistance(const Rectangle &Rectangle, int& IndexTriangle, float fTriangleParameter[3], float fRectangleParameter[2])const
{

    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        float fPara1Temp[3];
        float fPara2Temp[2];
        float dist = TriangleTemp.SquaredDistance(Rectangle, fPara1Temp, fPara2Temp);
        if (distTemp < dist)
        {
            distTemp = dist;
            for (int32 j = 0; j < 3; j++)
            {
                fTriangleParameter[j] = fPara1Temp[j];

            }
            for (int32 k = 0; k < 2; k++)
            {
                fRectangleParameter[k] = fPara2Temp[k];
            }
            IndexTriangle = i;
        }
    }

    return distTemp;
}

float Polygon::SquaredDistance(const Line &Line, int& IndexTriangle, float fTriangleParameter[3], float& fLineParameter) const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        float fPara1Temp[3];
        float fPara2;
        float dist = TriangleTemp.SquaredDistance(Line, fPara1Temp, fPara2);
        if (distTemp < dist)
        {
            distTemp = dist;
            for (int32 j = 0; j < 3; j++)
            {
                fTriangleParameter[j] = fPara1Temp[j];
            }
            fLineParameter = fPara2;
            IndexTriangle = i;
        }
    }

    return distTemp;
}

float Polygon::SquaredDistance(const Ray & Ray, int& IndexTriangle, float fTriangleParameter[3], float& fRayParameter) const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        float fPara1Temp[3];
        float fPara2;
        float dist = TriangleTemp.SquaredDistance(Ray, fPara1Temp, fPara2);
        if (distTemp < dist)
        {
            distTemp = dist;
            for (int32 j = 0; j < 3; j++)
            {
                fTriangleParameter[j] = fPara1Temp[j];
            }

            fRayParameter = fPara2;
            IndexTriangle = i;
        }
    }

    return distTemp;
}

float Polygon::SquaredDistance(const Segment& Segment, int& IndexTriangle, float fTriangleParameter[3], float& fSegmentParameter)const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        float fPara1Temp[3];
        float fPara2;
        float dist = TriangleTemp.SquaredDistance(Segment, fPara1Temp, fPara2);
        if (distTemp < dist)
        {
            distTemp = dist;
            for (int32 j = 0; j < 3; j++)
            {
                fTriangleParameter[j] = fPara1Temp[j];
            }

            fSegmentParameter = fPara2;
            IndexTriangle = i;
        }
    }

    return distTemp;
}

float Polygon::SquaredDistance(const OBB & OBB, int& IndexTriangle, float TriangleParameter[3], float OBBParameter[3]) const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        float fPara1Temp[3];
        float fPara2Temp[3];
        float dist = TriangleTemp.SquaredDistance(OBB, fPara1Temp, fPara2Temp);
        if (distTemp < dist)
        {
            distTemp = dist;
            for (int32 j = 0; j < 3; j++)
            {
                TriangleParameter[j] = fPara1Temp[j];
                OBBParameter[j] = fPara2Temp[j];
            }
            IndexTriangle = i;
        }
    }

    return distTemp;
}

float Polygon::Distance(const Sphere &Sphere, int& IndexTriangle, float fTriangleParameter[3], Vector3 & SpherePoint) const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        float fPara1Temp[3];
        Vector3 Point;
        float dist = TriangleTemp.Distance(Sphere, fPara1Temp, Point);
        if (distTemp < dist)
        {
            distTemp = dist;
            for (int32 j = 0; j < 3; j++)
            {
                fTriangleParameter[j] = fPara1Temp[j];

            }
            SpherePoint = Point;
            IndexTriangle = i;
        }
    }

    return distTemp;
}

float Polygon::Distance(const Plane &Plane, int& IndexTriangle, Vector3 &TrianglePoint, Vector3 &PlanePoint) const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        Vector3 Point1, Point2;
        float dist = TriangleTemp.Distance(Plane, Point1, Point2);
        if (distTemp < dist)
        {
            distTemp = dist;

            TrianglePoint = Point1;
            PlanePoint = Point2;
            IndexTriangle = i;
        }
    }

    return distTemp;
}

float Polygon::SquaredDistance(const AABB &AABB, int& IndexTriangle, float TriangleParameter[3], float AABBParameter[3]) const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        float fPara1Temp[3];
        float fPara2Temp[3];
        float dist = TriangleTemp.SquaredDistance(AABB, fPara1Temp, fPara2Temp);
        if (distTemp < dist)
        {
            distTemp = dist;
            for (int32 j = 0; j < 3; j++)
            {
                TriangleParameter[j] = fPara1Temp[j];
                AABBParameter[j] = fPara2Temp[j];
            }
            IndexTriangle = i;
        }
    }

    return distTemp;
}

float Polygon::SquaredDistance(const Polygon & Polygon, int& Index1Triangle,
    float Triangle1Parameter[3], int& Index2Triangle,
    float Triangle2Parameter[3])const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        float fPara1Temp[3];
        float fPara2Temp[3];
        int32 k;
        float dist = TriangleTemp.SquaredDistance(Polygon, fPara1Temp, k, fPara2Temp);
        if (distTemp < dist)
        {
            distTemp = dist;
            for (int32 j = 0; j < 3; j++)
            {
                Triangle1Parameter[j] = fPara1Temp[j];
                Triangle2Parameter[j] = fPara2Temp[j];
            }
            Index1Triangle = i;
            Index2Triangle = k;
        }
    }

    return distTemp;
}

int32 Polygon::RelationWith(const Line &Line, bool bCull, int32 &iIndexTriangle, float fTriangleParameter[3], float &fLineParameter)const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        int32 iFlag = TriangleTemp.RelationWith(Line, bCull, fTriangleParameter, fLineParameter);
        if (iFlag == MATH_INTERSECT)
            return MATH_INTERSECT;
    }
    return MATH_NOINTERSECT;
}

int32 Polygon::RelationWith(const Ray &Ray, bool bCull, int32 &iIndexTriangle, float fTriangleParameter[3], float &fRayParameter)const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        int32 iFlag = TriangleTemp.RelationWith(Ray, bCull, fTriangleParameter, fRayParameter);
        iIndexTriangle = i;
        if (iFlag == MATH_INTERSECT)
            return MATH_INTERSECT;

    }
    return MATH_NOINTERSECT;
}

int32 Polygon::RelationWith(const Segment &Segment, bool bCull, int32 &iIndexTriangle, float fTriangleParameter[3], float &fSegmentParameter)const
{
    int32 iTriangleNum = m_IndexNum / 3;
    Triangle TriangleTemp;
    float distTemp = MATH_MAX_FLOAT;

    for (int32 i = 0; i < iTriangleNum; i++)
    {
        TriangleTemp.Set(m_pPoint[m_pIndex[i]], m_pPoint[m_pIndex[i + 1]], m_pPoint[m_pIndex[i + 2]]);
        int32 iFlag = TriangleTemp.RelationWith(Segment, bCull, fTriangleParameter, fSegmentParameter);
        iIndexTriangle = i;
        if (iFlag == MATH_INTERSECT)
            return MATH_INTERSECT;
    }

    return MATH_NOINTERSECT;
}

int32 Polygon::RelationWith(const Plane & Plane)const
{
    Segment Segment;
    int32 iBackNum = 0, iFrontNum = 0;
    for (uint32 i = 0; i < m_PointNum; i++)
    {
        int32 iFlag = m_pPoint[i].RelationWith(Plane);
        if (iFlag == MATH_BACK)
            iBackNum++;
        else if (iFlag == MATH_FRONT)
            iFrontNum++;

        if (iBackNum > 0 && iFrontNum > 0)
            return MATH_INTERSECT;
    }
    if (iBackNum == 0 && iFrontNum == 0)
        return MATH_VSON;
    else if (iBackNum == 0)
        return MATH_FRONT;
    else if (iFrontNum == 0)
        return MATH_BACK;
    else
        return MATH_INTERSECT;
}

int32 Polygon::RelationWith(const Plane& plane, Segment& segment)const
{
    Segment segmentTemp;
    int32 InNum = 0;
    int32 OnNum = 0, BackNum = 0, FrontNum = 0;
    Vector3 Orig, End;
    for (uint32 i = 0; i < m_PointNum; i++)
    {
        int32 j = i + 1;
        if (j == m_PointNum)
        {
            j = 0;
        }

        segmentTemp.Set(m_pPoint[i], m_pPoint[j]);
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

int32 Polygon::RelationWith(const Triangle & Triangle)const
{
    Segment Segment;
    for (uint32 i = 0; i < m_PointNum; i++)
    {
        int32 j = i + 1;
        if (j == m_PointNum)
            j = 0;
        Segment.Set(m_pPoint[i], m_pPoint[j]);
        float fSegmentParameter;
        float TriangleParameter[3];
        if (Segment.RelationWith(Triangle, 0, fSegmentParameter, TriangleParameter) == MATH_INTERSECT)
            return MATH_INTERSECT;

    }

    Vector3 Point[3];
    Triangle.GetPoint(Point);
    for (int32 i = 0; i < 3; i++)
    {
        int32 j = i + 1;
        if (j == 3)
            j = 0;
        Segment.Set(Point[i], Point[j]);
        float fSegmentParameter;
        float TriangleParameter[3];
        int32 Index;
        if (Segment.RelationWith(*this, fSegmentParameter, 0, Index, TriangleParameter) == MATH_INTERSECT)
            return MATH_INTERSECT;
    }

    return MATH_NOINTERSECT;
}

int32 Polygon::RelationWith(const Triangle& triangle, Segment& segment)const
{
    Segment segmentTemp;
    int32 InNum = 0;
    Vector3 Orig, End;
    for (uint32 i = 0; i < m_PointNum; i++)
    {
        int32 j = i + 1;
        if (j == m_PointNum)
            j = 0;
        segment.Set(m_pPoint[i], m_pPoint[j]);
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

    Vector3 Point[3];
    triangle.GetPoint(Point);
    for (int32 i = 0; i < 3; i++)
    {
        int32 j = i + 1;
        if (j == 3)
            j = 0;
        segmentTemp.Set(Point[i], Point[j]);
        float fSegmentParameter;
        float fTriangleParameter[3];
        int32 Index;
        int32 iFlag = segmentTemp.RelationWith(*this, fSegmentParameter, 0, Index, fTriangleParameter);
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

int32 Polygon::RelationWith(const Rectangle & Rectangle)const
{
    Segment Segment;
    for (uint32 i = 0; i < m_PointNum; i++)
    {
        int32 j = i + 1;
        if (j == m_PointNum)
            j = 0;
        Segment.Set(m_pPoint[i], m_pPoint[j]);
        float fSegmentParameter;
        float TriangleParameter[2];
        if (Segment.RelationWith(Rectangle, 0, fSegmentParameter, TriangleParameter) == MATH_INTERSECT)
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
        float TriangleParameter[3];
        int32 Index;
        if (Segment.RelationWith(*this, fSegmentParameter, 0, Index, TriangleParameter) == MATH_INTERSECT)
            return MATH_INTERSECT;

    }

    return MATH_NOINTERSECT;
}

int32 Polygon::RelationWith(const Rectangle& rectangle, Segment& segment)const
{
    Segment segmentTemp;
    int32 InNum = 0;
    Vector3 Orig, End;
    for (uint32 i = 0; i < m_PointNum; i++)
    {
        int32 j = i + 1;
        if (j == m_PointNum)
            j = 0;
        segment.Set(m_pPoint[i], m_pPoint[j]);
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
        {
            j = 0;
        }

        segmentTemp.Set(Point[i], Point[j]);
        float fSegmentParameter;
        float fTriangleParameter[3];
        int32 Index;
        int32 iFlag = segmentTemp.RelationWith(*this, fSegmentParameter, 0, Index, fTriangleParameter);
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

int32 Polygon::RelationWith(const AABB &AABB)const
{
    Plane Plane = GetPlane();
    if (Plane.RelationWith(AABB) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    int32 InNum = 0, OutNum = 0;

    for (uint32 i = 0; i < m_PointNum; i++)
    {
        int32 iFlag = m_pPoint[i].RelationWith(AABB);
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

}

int32 Polygon::RelationWith(const OBB &OBB)const
{
    Plane Plane = GetPlane();
    if (Plane.RelationWith(OBB) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    int32 InNum = 0, OutNum = 0;

    for (uint32 i = 0; i < m_PointNum; i++)
    {
        int32 iFlag = m_pPoint[i].RelationWith(OBB);
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
}

int32 Polygon::RelationWith(const Sphere &Sphere)const
{
    Plane Plane = GetPlane();
    if (Plane.RelationWith(Sphere) != MATH_INTERSECT)
        return MATH_NOINTERSECT;
    int32 InNum = 0, OutNum = 0;

    for (uint32 i = 0; i < m_PointNum; i++)
    {
        int32 iFlag = m_pPoint[i].RelationWith(Sphere);
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
}

}