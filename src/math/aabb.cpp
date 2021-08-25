#include "math/aabb.h"

#include "math/maths.h"
#include "math/matrix3X3.h"
#include "math/line.h"
#include "math/ray.h"
#include "math/rectangle.h"
#include "math/segment.h"
#include "math/polygon.h"
#include "stl/array.h"
#include <algorithm>


namespace zq{


const Vector3 AABB::m_A[3] = {Vector3(1.0f,0.0f,0.0f),Vector3(0.0f,1.0f,0.0f),Vector3(0.0f,0.0f,1.0f)};

AABB::AABB()
{
	m_fA[0] = m_fA[1] = m_fA[2] = 0.0f;
}

AABB::~AABB()
{
	   
}

AABB::AABB(const Vector3 & Max , const Vector3 & Min)
{
	Set(Max,Min);
}

AABB::AABB(const Vector3 & Center,float fA0,float fA1,float fA2)
{
	Set(Center,fA0,fA1,fA2);
}

AABB::AABB(const Vector3 & Center,float fA[3])
{
	Set(Center,fA);
}

void AABB::GetPlane(Plane pPlanes[6])const
{
	Vector3 vcN;

	// 右面
	vcN = m_A[0];
	pPlanes[0].Set(vcN, m_Max);

	// 左面
	vcN = m_A[0] * (-1.0f);
	pPlanes[1].Set(vcN, m_Min);

	// 前面
	vcN = m_A[2] * (-1.0f);
	pPlanes[2].Set(vcN, m_Min);

	// 后面
	vcN = m_A[2];
	pPlanes[3].Set(vcN, m_Max);

	// 上面
	vcN = m_A[1];
	pPlanes[4].Set(vcN, m_Max);

	// 下面
	vcN = m_A[1] * (-1.0f);
	pPlanes[5].Set(vcN, m_Min);
}

void AABB::GetPoint(Vector3 Point[8])const
{
	Point[0].Set(m_Center.x + m_fA[0], m_Center.y + m_fA[1] ,m_Center.z + m_fA[2]);
	Point[1].Set(m_Center.x - m_fA[0], m_Center.y + m_fA[1] ,m_Center.z + m_fA[2]);
	Point[2].Set(m_Center.x - m_fA[0], m_Center.y - m_fA[1] ,m_Center.z + m_fA[2]);
	Point[3].Set(m_Center.x + m_fA[0], m_Center.y - m_fA[1] ,m_Center.z + m_fA[2]);
	Point[4].Set(m_Center.x + m_fA[0], m_Center.y + m_fA[1] ,m_Center.z - m_fA[2]);
	Point[5].Set(m_Center.x - m_fA[0], m_Center.y + m_fA[1] ,m_Center.z - m_fA[2]);
	Point[6].Set(m_Center.x - m_fA[0], m_Center.y - m_fA[1] ,m_Center.z - m_fA[2]);
	Point[7].Set(m_Center.x + m_fA[0], m_Center.y - m_fA[1] ,m_Center.z - m_fA[2]);
}

void AABB::GetRectangle(Rectangle Rectangle[6])const
{
	Vector3 Center[6];
	Center[0].Set(m_Center.x + m_fA[0],m_Center.y,m_Center.z);
	Center[1].Set(m_Center.x - m_fA[0],m_Center.y,m_Center.z);
	Center[2].Set(m_Center.x,m_Center.y + m_fA[1],m_Center.z);
	Center[3].Set(m_Center.x,m_Center.y - m_fA[1],m_Center.z);
	Center[4].Set(m_Center.x,m_Center.y,m_Center.z + m_fA[2]);
	Center[5].Set(m_Center.x,m_Center.y,m_Center.z - m_fA[2]);
	//right
	Rectangle[0].Set(Center[0],m_A[1],m_A[2],m_fA[1],m_fA[2]);
	//left
	Rectangle[1].Set(Center[1],m_A[1],m_A[2] * (-1),m_fA[1],m_fA[2]);

	//up
	Rectangle[2].Set(Center[4],m_A[2],m_A[0],m_fA[2],m_fA[1]);
	//down
	Rectangle[3].Set(Center[5],m_A[2],m_A[0] * (-1),m_fA[2],m_fA[0]);

	//front
	Rectangle[4].Set(Center[2],m_A[1],m_A[0] * (-1),m_fA[1],m_fA[0]);
	//back    
	Rectangle[5].Set(Center[3],m_A[1],m_A[0],m_fA[1],m_fA[0]);


}

bool AABB::GetParameter(const Vector3 &Point,float fAABBParameter[3])const
{
	for(int32 i = 0 ; i < 3 ; i++)
	{
		fAABBParameter[i] = Point.m[i] - m_Center.m[i];
	
	}
	for(int32 i = 0 ; i < 3 ; i++)
		if(ABS(fAABBParameter[i]) > m_fA[i])
			return 0;
	
	return 1;
}

void AABB::Transform(const AABB &aabb, const Matrix3X3W &m,bool bHasProject)
{
	if (bHasProject == false)
	{
		m_Max = m_Min = m.GetTranslation();
		if(m._00 > 0.0f)
		{
			m_Min.x += m._00 * aabb.m_Min.x;
			m_Max.x += m._00 * aabb.m_Max.x;

		}
		else
		{
			m_Min.x += m._00 * aabb.m_Max.x;
			m_Max.x += m._00 * aabb.m_Min.x;
		}

		if(m._01 > 0.0f)
		{
			m_Min.y += m._01 * aabb.m_Min.x;
			m_Max.y += m._01 * aabb.m_Max.x;

		}
		else
		{
			m_Min.y += m._01 * aabb.m_Max.x;
			m_Max.y += m._01 * aabb.m_Min.x;
		}

		if(m._02 > 0.0f)
		{
			m_Min.z += m._02 * aabb.m_Min.x;
			m_Max.z += m._02 * aabb.m_Max.x;

		}
		else
		{
			m_Min.z += m._02 * aabb.m_Max.x;
			m_Max.z += m._02 * aabb.m_Min.x;
		}

		if(m._10 > 0.0f)
		{
			m_Min.x += m._10 * aabb.m_Min.y;
			m_Max.x += m._10 * aabb.m_Max.y;

		}
		else
		{
			m_Min.x += m._10 * aabb.m_Max.y;
			m_Max.x += m._10 * aabb.m_Min.y;
		}

		if(m._11 > 0.0f)
		{
			m_Min.y += m._11 * aabb.m_Min.y;
			m_Max.y += m._11 * aabb.m_Max.y;

		}
		else
		{
			m_Min.y += m._11 * aabb.m_Max.y;
			m_Max.y += m._11 * aabb.m_Min.y;
		}

		if(m._12 > 0.0f)
		{
			m_Min.z += m._12 * aabb.m_Min.y;
			m_Max.z += m._12 * aabb.m_Max.y;

		}
		else
		{
			m_Min.z += m._12 * aabb.m_Max.y;
			m_Max.z += m._12 * aabb.m_Min.y;
		}

		if(m._20 > 0.0f)
		{
			m_Min.x += m._20 * aabb.m_Min.z;
			m_Max.x += m._20 * aabb.m_Max.z;

		}
		else
		{
			m_Min.x += m._20 * aabb.m_Max.z;
			m_Max.x += m._20 * aabb.m_Min.z;
		}

		if(m._21 > 0.0f)
		{
			m_Min.y += m._21 * aabb.m_Min.z;
			m_Max.y += m._21 * aabb.m_Max.z;

		}
		else
		{
			m_Min.y += m._21 * aabb.m_Max.z;
			m_Max.y += m._21 * aabb.m_Min.z;
		}

		if(m._22 > 0.0f)
		{
			m_Min.z += m._22 * aabb.m_Min.z;
			m_Max.z += m._22 * aabb.m_Max.z;

		}
		else
		{
			m_Min.z += m._22 * aabb.m_Max.z;
			m_Max.z += m._22 * aabb.m_Min.z;
		}
	}
	else
	{	
		Vector3 Point[8];
        aabb.GetPoint(Point);

		m_Max = Vector3(-MATH_MAX_FLOAT ,-MATH_MAX_FLOAT ,-MATH_MAX_FLOAT);
		m_Min = Vector3(MATH_MAX_FLOAT ,MATH_MAX_FLOAT ,MATH_MAX_FLOAT);
		for (uint32 i = 0 ; i < 8 ; i++)
		{
			Vector3 New = Point[i] * m;
			
			m_Min.x = Min(m_Min.x, New.x);
			m_Min.y = Min(m_Min.y, New.y);
			m_Min.z = Min(m_Min.z, New.z);
			m_Max.x = Max(m_Max.x, New.x);
			m_Max.y = Max(m_Max.y, New.y);
			m_Max.z = Max(m_Max.z, New.z);
		}
	}

	Set(m_Max,m_Min);
}

AABB AABB::MergeAABB(const AABB &aabb)const
{
	AABB Temp;
	Temp.m_Max = m_Max;
	Temp.m_Min = m_Min;
	if(Temp.m_Min.x > aabb.m_Min.x)
	{
		Temp.m_Min.x = aabb.m_Min.x;
	}
	if(Temp.m_Min.y > aabb.m_Min.y)
	{
		Temp.m_Min.y = aabb.m_Min.y;
	}
	if(Temp.m_Min.z > aabb.m_Min.z)
	{
		Temp.m_Min.z = aabb.m_Min.z;
	}

	if(Temp.m_Max.x < aabb.m_Max.x)
	{
		Temp.m_Max.x = aabb.m_Max.x;
	}
	if(Temp.m_Max.y < aabb.m_Max.y)
	{
		Temp.m_Max.y = aabb.m_Max.y;
	}
	if(Temp.m_Max.z < aabb.m_Max.z)
	{
		Temp.m_Max.z = aabb.m_Max.z;
	}

	Temp.Set(Temp.m_Max,Temp.m_Min);
	return Temp;
}

void AABB::CreateAABB(const Vector3* const pPointArray,uint32 uiPointNum)
{
	if(!pPointArray || !uiPointNum)
		return;
	m_Min = pPointArray[0]; 
	m_Max = pPointArray[0];
	for (uint32 i = 1; i < uiPointNum; i++)
	{
		if (pPointArray[i].x < m_Min.x)
		{
			m_Min.x = pPointArray[i].x;
		}
		else if (pPointArray[i].x > m_Max.x)
		{
			m_Max.x = pPointArray[i].x;
		}

		if (pPointArray[i].y < m_Min.y)
		{
			m_Min.y = pPointArray[i].y;
		}
		else if (pPointArray[i].y > m_Max.y)
		{
			m_Max.y = pPointArray[i].y;
		}

		if (pPointArray[i].z < m_Min.z)
		{
			m_Min.z = pPointArray[i].z;
		}
		else if (pPointArray[i].z > m_Max.z)
		{
			m_Max.z = pPointArray[i].z;
		}
	}
	m_Center = (m_Min + m_Max)/2.0f;

	Vector3 Temp = (m_Max - m_Min)/2.0f;

	m_fA[0] = Temp.x;
	m_fA[1] = Temp.y;
	m_fA[2] = Temp.z;
} 

void AABB::GetQuadAABB(AABB aabb[4])const
{
	Vector3 Point[8];
	GetPoint(Point);
	Vector3 Mid[4];
	Mid[0] = (m_Center + Point[0]) * 0.5;
	Mid[1] = (m_Center + Point[1]) * 0.5;
	Mid[2] = (m_Center + Point[4]) * 0.5;
	Mid[3] = (m_Center + Point[5]) * 0.5;
	float fA[3] = {m_fA[0] * 0.5f , m_fA[1], m_fA[2] * 0.5f};
	for (uint32 i = 0 ; i < 4 ; i++)
	{
		Mid[i].y = m_Center.y;
        aabb[i].Set(Mid[i],fA);
	}	
}

AABB AABB::GetMin(const AABB& aabb) const
{
	AABB Temp;
	Temp.m_Max = m_Max;
	Temp.m_Min = m_Min;
	if (Temp.m_Min.x < aabb.m_Min.x)
	{
		Temp.m_Min.x = aabb.m_Min.x;
	}
	if (Temp.m_Min.y < aabb.m_Min.y)
	{
		Temp.m_Min.y = aabb.m_Min.y;
	}
	if (Temp.m_Min.z < aabb.m_Min.z)
	{
		Temp.m_Min.z = aabb.m_Min.z;
	}

	if (Temp.m_Max.x > aabb.m_Max.x)
	{
		Temp.m_Max.x = aabb.m_Max.x;
	}
	if (Temp.m_Max.y > aabb.m_Max.y)
	{
		Temp.m_Max.y = aabb.m_Max.y;
	}
	if (Temp.m_Max.z > aabb.m_Max.z)
	{
		Temp.m_Max.z = aabb.m_Max.z;
	}

	Temp.Set(Temp.m_Max, Temp.m_Min);
	return Temp;
}

void AABB::GetOctAABB(AABB aabb[8])const
{
	Vector3 Point[8];
	GetPoint(Point);
	Vector3 Mid[8];

	float fA[3] = {m_fA[0] * 0.5f , m_fA[1] * 0.5f , m_fA[2] * 0.5f};
	for (uint32 i = 0 ; i < 8 ; i++)
	{
		Mid[i] = (m_Center + Point[i]) * 0.5;
        aabb[i].Set(Mid[i],fA);
	}
}

float AABB::SquaredDistance(const Vector3 & Point, float fAABBParameter[3])const
{
    Vector3 Diff = Point - m_Center;


    float fSqrDistance = 0.0, fDelta;

    fAABBParameter[0] = Diff.x;
    if (fAABBParameter[0] < -m_fA[0])
    {
        fDelta = fAABBParameter[0] + m_fA[0];
        fSqrDistance += fDelta * fDelta;
        fAABBParameter[0] = -m_fA[0];
    }
    else if (fAABBParameter[0] > m_fA[0])
    {
        fDelta = fAABBParameter[0] - m_fA[0];
        fSqrDistance += fDelta * fDelta;
        fAABBParameter[0] = m_fA[0];
    }

    fAABBParameter[1] = Diff.y;
    if (fAABBParameter[1] < -m_fA[1])
    {
        fDelta = fAABBParameter[1] + m_fA[1];
        fSqrDistance += fDelta * fDelta;
        fAABBParameter[1] = -m_fA[1];
    }
    else if (fAABBParameter[1] > m_fA[1])
    {
        fDelta = fAABBParameter[1] - m_fA[1];
        fSqrDistance += fDelta * fDelta;
        fAABBParameter[1] = m_fA[1];
    }

    fAABBParameter[2] = Diff.z;
    if (fAABBParameter[2] < -m_fA[2])
    {
        fDelta = fAABBParameter[2] + m_fA[2];
        fSqrDistance += fDelta * fDelta;
        fAABBParameter[2] = -m_fA[2];
    }
    else if (fAABBParameter[2] > m_fA[2])
    {
        fDelta = fAABBParameter[2] - m_fA[2];
        fSqrDistance += fDelta * fDelta;
        fAABBParameter[2] = m_fA[2];
    }
    return fSqrDistance;
}

float AABB::SquaredDistance(const Line & Line, float fAABBParameter[3], float &fLineParameter)const
{
    Rectangle Rectangle[6];
    GetRectangle(Rectangle);
    float fSqrDist = MATH_MAX_FLOAT;
    float fSqrDistTmp;
    float t, fAABB[3];
    for (int32 i = 0; i < 6; i++)
    {
        fSqrDistTmp = Line.SquaredDistance(*this, t, fAABB);
        if (fSqrDist < fSqrDistTmp)
        {
            fSqrDist = fSqrDistTmp;
            fLineParameter = t;
            for (int32 j = 0; j < 3; j++)
            {
                fAABBParameter[j] = fAABB[j];
            }

        }

    }
    return fSqrDist;
    /*Vector3 Orig = Line.GetOrig() - m_Center;
    Vector3 Dir = Line.GetDir();

    bool bReflect[3];
    int32 i;
    for (i = 0; i < 3; i++)
    {
        if (Dir.m[i] < 0.0f)
        {
            Orig.m[i] = -Orig.m[i];
            Dir.m[i] = -Dir.m[i];
            bReflect[i] = true;
        }
        else
        {
            bReflect[i] = false;
        }
    }

    float fSqrDistance = 0.0f;
    fLineParameter = 0.0f;

    if (Dir.x > 0.0f)
    {
        if (Dir.y > 0.0f)
        {
            if (Dir.z > 0.0f)  // (+,+,+)
            {
                CaseNoZeros(Orig,Dir,fSqrDistance,fLineParameter);
            }
            else  // (+,+,0)
            {
                Case0(0,1,2,Orig,Dir,fSqrDistance,fLineParameter);
            }
        }
        else
        {
            if (Dir.z > 0.0f)  // (+,0,+)
            {
                Case0(0,2,1,Orig,Dir,fSqrDistance,fLineParameter);
            }
            else  // (+,0,0)
            {
                Case00(0,1,2,Orig,Dir,fSqrDistance,fLineParameter);
            }
        }
    }
    else
    {
        if (Dir.y > 0.0f)
        {
            if (Dir.z > 0.0f)  // (0,+,+)
            {
                Case0(1,2,0,Orig,Dir,fSqrDistance,fLineParameter);
            }
            else  // (0,+,0)
            {
                Case00(1,0,2,Orig,Dir,fSqrDistance,fLineParameter);
            }
        }
        else
        {
            if (Dir.z > 0.0f)  // (0,0,+)
            {
                Case00(2,0,1,Orig,Dir,fSqrDistance,fLineParameter);
            }
            else  // (0,0,0)
            {
                Case000(Orig,fSqrDistance);
            }
        }
    }


    for (i = 0; i < 3; i++)
    {

        if (bReflect[i])
        {
            Orig.m[i] = -Orig.m[i];
        }


    }
    fAABBParameter[0] = Orig.x;
    fAABBParameter[1] = Orig.y;
    fAABBParameter[2] = Orig.z;
    return fSqrDistance;*/
}

float AABB::SquaredDistance(const Ray &Ray, float fAABBParameter[3], float &fRayParameter)const
{
    return Ray.SquaredDistance(*this, fRayParameter, fAABBParameter);
}

float AABB::SquaredDistance(const Segment & Segment, float fAABBParameter[3], float &fSegmentParameter)const
{
    return Segment.SquaredDistance(*this, fSegmentParameter, fAABBParameter);
}

float AABB::SquaredDistance(const Triangle & Triangle, float AABBParameter[3], float TriangleParameter[3])const
{
    Rectangle Rectangle[6];
    GetRectangle(Rectangle);
    float fSqrDist = MATH_MAX_FLOAT;
    float fSqrDistTmp;
    float RectP[2], TriP[3];

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[0], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        AABBParameter[0] = m_fA[0];
        AABBParameter[1] = RectP[0];
        AABBParameter[2] = RectP[1];
    }

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[1], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        AABBParameter[0] = -m_fA[0];
        AABBParameter[1] = RectP[0];
        AABBParameter[2] = -RectP[1];
    }

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[2], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        AABBParameter[0] = RectP[1];
        AABBParameter[1] = m_fA[1];
        AABBParameter[2] = RectP[0];
    }

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[3], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        AABBParameter[0] = -RectP[1];
        AABBParameter[1] = -m_fA[1];
        AABBParameter[2] = RectP[0];
    }

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[4], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        AABBParameter[0] = -RectP[1];
        AABBParameter[1] = RectP[0];
        AABBParameter[2] = m_fA[2];
    }

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[5], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        AABBParameter[0] = RectP[1];
        AABBParameter[1] = RectP[0];
        AABBParameter[2] = -m_fA[2];
    }
    return fSqrDist;
}

float AABB::SquaredDistance(const Rectangle& rectangle, float AABBParameter[3], float RectangleParameter[2])const
{

    Rectangle RectangleTemp[6];
    GetRectangle(RectangleTemp);
    float fSqrDist = MATH_MAX_FLOAT;
    float fSqrDistTmp;
    float RectP[2], RectP1[2];

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[0], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        AABBParameter[0] = m_fA[0];
        AABBParameter[1] = RectP[0];
        AABBParameter[2] = RectP[1];
    }

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[1], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        AABBParameter[0] = -m_fA[0];
        AABBParameter[1] = RectP[0];
        AABBParameter[2] = -RectP[1];
    }

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[2], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        AABBParameter[0] = RectP[1];
        AABBParameter[1] = m_fA[1];
        AABBParameter[2] = RectP[0];
    }

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[3], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        AABBParameter[0] = -RectP[1];
        AABBParameter[1] = -m_fA[1];
        AABBParameter[2] = RectP[0];
    }

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[4], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        AABBParameter[0] = -RectP[1];
        AABBParameter[1] = RectP[0];
        AABBParameter[2] = m_fA[2];
    }

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[5], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        AABBParameter[0] = RectP[1];
        AABBParameter[1] = RectP[0];
        AABBParameter[2] = -m_fA[2];
    }

    return fSqrDist;
}

float AABB::SquaredDistance(const AABB &AABB, float AABB1Parameter[3], float AABB2Parameter[3])const
{
    float fSqrDist = MATH_MAX_FLOAT;
    float fSqrDistTmp;
    Vector3 Point[8];
    GetPoint(Point);
    float Temp[3];
    for (int32 i = 0; i < 8; i++)
    {
        fSqrDistTmp = AABB.SquaredDistance(Point[i], Temp);
        if (fSqrDistTmp < fSqrDist)
        {
            GetParameter(Point[i], AABB1Parameter);
            for (int32 j = 0; j < 3; j++)
            {
                AABB2Parameter[j] = Temp[j];
            }
        }

    }

    return fSqrDist;
}

float AABB::Distance(const Plane &Plane, Vector3 &AABBPoint, Vector3 &PlanePoint)const
{

    return Plane.Distance(*this, PlanePoint, AABBPoint);
}

float AABB::Distance(const Sphere & Sphere, float fAABBParameter[3], Vector3 & SpherePoint)const
{
    return Sphere.Distance(*this, SpherePoint, fAABBParameter);
}

float AABB::SquaredDistance(const Polygon& polygon, float AABBParameter[3], int& IndexTriangle, float TriangleParameter[3])const
{
    return polygon.SquaredDistance(*this, IndexTriangle, TriangleParameter, AABBParameter);
}

int32 AABB::RelationWith(const Vector3 &Point)const
{
    Vector3 PointTemp = Point - m_Center;
    float fDiffX = ABS(PointTemp.x) - m_fA[0];
    float fDiffY = ABS(PointTemp.y) - m_fA[1];
    float fDiffZ = ABS(PointTemp.z) - m_fA[2];

    if (fDiffX > EPSILON_E4 || fDiffY > EPSILON_E4 || fDiffZ > EPSILON_E4)
        return MATH_OUT;

    if (fDiffX < -EPSILON_E4 || fDiffY < -EPSILON_E4 || fDiffZ < -EPSILON_E4)
        return MATH_IN;

    return MATH_VSON;
}

int32 AABB::RelationWith(const Line &Line, uint32 &Quantity, float &tNear, float &tFar)const
{
    return Line.RelationWith(*this, Quantity, tNear, tFar);
}

int32 AABB::RelationWith(const Ray &Ray, uint32 &Quantity, float &tNear, float &tFar)const
{
    return Ray.RelationWith(*this, Quantity, tNear, tFar);
}

int32 AABB::RelationWith(const Segment &Segment, uint32 &Quantity, float &tNear, float &tFar)const
{
    return Segment.RelationWith(*this, Quantity, tNear, tFar);
}

int32 AABB::RelationWith(const Plane &Plane)const
{
    Vector3 N = Plane.GetN();
    float fD = Plane.GetfD();
    Vector3 MinTemp, MaxTemp;
    // x 
    if (N.x >= 0.0f)
    {
        MinTemp.x = m_Min.x;
        MaxTemp.x = m_Max.x;
    }
    else
    {
        MinTemp.x = m_Max.x;
        MaxTemp.x = m_Min.x;
    }
    // y 
    if (N.y >= 0.0f)
    {
        MinTemp.y = m_Min.y;
        MaxTemp.y = m_Max.y;
    }
    else
    {
        MinTemp.y = m_Max.y;
        MaxTemp.y = m_Min.y;
    }
    // z 
    if (N.z >= 0.0f)
    {
        MinTemp.z = m_Min.z;
        MaxTemp.z = m_Max.z;
    }
    else
    {
        MinTemp.z = m_Max.z;
        MaxTemp.z = m_Min.z;
    }

    if ((N.Dot(MinTemp) + fD) > 0.0f)
        return MATH_FRONT;
    else if ((N.Dot(MaxTemp) + fD) < 0.0f)
        return MATH_BACK;
    else
        return MATH_INTERSECT;
}

int32 AABB::RelationWith(const Triangle &Triangle)const
{
    return Triangle.RelationWith(*this);
}

int32 AABB::RelationWith(const Rectangle &Rectangle)const
{
    return Rectangle.RelationWith(*this);
}

int32 AABB::RelationWith(const AABB &aabb)const
{
    if ((m_Min.x > aabb.m_Max.x) || (aabb.m_Min.x > m_Max.x))
        return MATH_NOINTERSECT;
    if ((m_Min.y > aabb.m_Max.y) || (aabb.m_Min.y > m_Max.y))
        return MATH_NOINTERSECT;
    if ((m_Min.z > aabb.m_Max.z) || (aabb.m_Min.z > m_Max.z))
        return MATH_NOINTERSECT;
    return MATH_INTERSECT;
}

int32 AABB::RelationWith(const Sphere &Sphere)const
{
    float fAABBParameter[3];
    float Dist = Sphere.m_Center.SquaredDistance(*this, fAABBParameter);
    Dist = SQRT(Dist);
    if (Dist - Sphere.m_fRadius < 0.0f)
        return MATH_INTERSECT;

    return MATH_NOINTERSECT;
}

bool AABB::GetIntersect(AABB& aabb, AABB& OutAABB)const
{
    if (RelationWith(aabb) == MATH_INTERSECT)
    {
        //VSArrayOrder<float> XArray;
        //XArray.push_back(GetMinPoint().x);
        //XArray.push_back(GetMaxPoint().x);
        //XArray.push_back(aabb.GetMinPoint().x);
        //XArray.push_back(aabb.GetMaxPoint().x);

        //VSArrayOrder<float> YArray;
        //YArray.push_back(GetMinPoint().y);
        //YArray.push_back(GetMaxPoint().y);
        //YArray.push_back(aabb.GetMinPoint().y);
        //YArray.push_back(aabb.GetMaxPoint().y);

        //VSArrayOrder<float> ZArray;
        //ZArray.push_back(GetMinPoint().z);
        //ZArray.push_back(GetMaxPoint().z);
        //ZArray.push_back(aabb.GetMinPoint().z);
        //ZArray.push_back(aabb.GetMaxPoint().z);

        Array<float, 4> XArray;
        XArray[0] = GetMinPoint().x;
        XArray[1] = GetMaxPoint().x;
        XArray[2] = aabb.GetMinPoint().x;
        XArray[3] = aabb.GetMaxPoint().x;
        std::sort(XArray.begin(), XArray.end());

        Array<float, 4> YArray;
        YArray[0] = GetMinPoint().y;
        YArray[1] = GetMaxPoint().y;
        YArray[2] = aabb.GetMinPoint().y;
        YArray[3] = aabb.GetMaxPoint().y;
        std::sort(YArray.begin(), YArray.end());

        Array<float, 4> ZArray;
        ZArray[0] = GetMinPoint().z;
        ZArray[1] = GetMaxPoint().z;
        ZArray[2] = aabb.GetMinPoint().z;
        ZArray[3] = aabb.GetMaxPoint().z;
        std::sort(ZArray.begin(), ZArray.end());

        OutAABB.Set(Vector3(XArray[2], YArray[2], ZArray[2]), Vector3(XArray[1], YArray[1], ZArray[1]));
        return true;
    }

    return false;
}

}