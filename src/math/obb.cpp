#include "math/obb.h"
#include "math/maths.h"
#include "math/matrix3X3.h"
#include "math/line.h"
#include "math/aabb.h"
#include "math/ray.h"
#include "math/rectangle.h"
#include "math/segment.h"
#include "math/polygon.h"


namespace zq{


OBB::OBB()
{
}

OBB::~OBB()
{
}

OBB::OBB(const Vector3 A[3],float fA[3],const Vector3 & Center)
{
	Set(A,fA,Center);
}

OBB::OBB(const Vector3 &A0,const Vector3 &A1,const Vector3 &A2,
			float fA0,float fA1,float fA2,
			 const Vector3 & Center)
{
	Set(A0,A1,A2,fA0,fA1,fA2,Center);
}

void OBB::Transform(const OBB &OBB, const Matrix3X3W &Mat)
{
	m_Center = OBB.m_Center * Mat;
	m_A[0]     = Mat.Apply3X3(OBB.m_A[0]);
	m_A[1]     = Mat.Apply3X3(OBB.m_A[1]);
	m_A[2]     = Mat.Apply3X3(OBB.m_A[2]);

	m_fA[0] = OBB.m_fA[0] * m_A[0].GetLength();
	m_fA[1] = OBB.m_fA[1] * m_A[1].GetLength();
	m_fA[2] = OBB.m_fA[2] * m_A[2].GetLength();

	m_A[0].Normalize();
	m_A[1].Normalize();
	m_A[2].Normalize();
}

void OBB::GetTransform(Matrix3X3W &m)const
{
	m.CreateInWorldObject(m_A[0],m_A[1],m_A[2],m_Center);
}

void OBB::GetRectangle(Rectangle Rectangle[6])const
{
	Vector3 Center[6];
	Center[0] = m_Center + m_A[0] * m_fA[0];
	Center[1] = m_Center + m_A[0] * (-m_fA[0]);
	Center[2] = m_Center + m_A[1] * m_fA[1];
	Center[3] = m_Center + m_A[1] * (-m_fA[1]);
	Center[4] = m_Center + m_A[2] * m_fA[2];
	Center[5] = m_Center + m_A[2] * (-m_fA[2]);
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
	/**/
}

void OBB::GetPlane(Plane pPlanes[6])const
{
	Vector3 Max,Min;
	Max = m_Center + m_A[0] * m_fA[0] + m_A[1] * m_fA[1] + m_A[2] * m_fA[2];
	Min = m_Center + m_A[0] * (-m_fA[0]) + m_A[1] * (-m_fA[1]) + m_A[2] * (-m_fA[2]);

	Vector3 vcN;

	// 右面
	vcN = m_A[0];
	pPlanes[0].Set(vcN, Max);

	// 左面
	vcN = m_A[0] * (-1.0f);
	pPlanes[1].Set(vcN, Min);

	// 前面
	vcN = m_A[2] * (-1.0f);
	pPlanes[2].Set(vcN, Min);

	// 后面
	vcN = m_A[2];
	pPlanes[3].Set(vcN, Max);

	// 上面
	vcN = m_A[1];
	pPlanes[4].Set(vcN, Max);

	// 下面
	vcN = m_A[1] * (-1.0f);
	pPlanes[5].Set(vcN, Min);
}
/*----------------------------------------------------------------*/
void OBB::GetPoint(Vector3 Point[8])const
{
	Vector3 Temp[6];
	Temp[0] =  m_A[0] * m_fA[0];
	Temp[1] =  m_A[0] * (-m_fA[0]);

	Temp[2] =  m_A[1] * m_fA[1];
	Temp[3] =  m_A[1] * (-m_fA[1]);

	Temp[4] =  m_A[2] * m_fA[2];
	Temp[5] =  m_A[2] * (-m_fA[2]);

	Vector3 Temp1[6];
	Temp1[0] = m_Center + Temp[0];
	Temp1[1] = m_Center + Temp[1];

	Temp1[2] = Temp1[0] + Temp[2];
	Temp1[3] = Temp1[1] + Temp[2];
	Temp1[4] = Temp1[1] + Temp[3];
	Temp1[5] = Temp1[0] + Temp[3];

	Point[0] = Temp1[2] + Temp[4];
	Point[1] = Temp1[3] + Temp[4];
	Point[2] = Temp1[4] + Temp[4];
	Point[3] = Temp1[5] + Temp[4];

	Point[4] = Temp1[2] + Temp[5];
	Point[5] = Temp1[3] + Temp[5];
	Point[6] = Temp1[4] + Temp[5];
	Point[7] = Temp1[5] + Temp[5];
}

bool OBB::GetParameter(const Vector3 &Point,float fOBBParameter[3])const
{
	Matrix3X3W m;
	GetTransform(m);
	Vector3 Temp = Point * m;
	fOBBParameter[0] = Temp.x;
	fOBBParameter[1] = Temp.y;
	fOBBParameter[2] = Temp.z;
	if(ABS(Temp.x) > m_fA[0] || ABS(Temp.y) > m_fA[1] || ABS(Temp.z) > m_fA[2])
		return 0;
	else
		return 1;	
}

void OBB::CreateOBB(const Vector3 * const pPointArray,uint32 uiPointNum)
{
	if(!pPointArray || !uiPointNum)
		return;

	Vector3 Sum = pPointArray[0];
	for(uint32 i = 1 ; i < uiPointNum ; i++)
	{
		Sum += pPointArray[i];
	}
	m_Center = Sum / (uiPointNum * 1.0f);
	Matrix3X3 MatSum,MatTemp;
	MatSum.SetZero();
	
	for(uint32 i = 0 ; i < uiPointNum ; i++)
	{
		Vector3 Diff = pPointArray[i] - m_Center;
		MatTemp.CreateFromTwoVector(Diff,Diff);
		MatSum +=MatTemp;
	}

	float InvNun = 1.0f / uiPointNum ;
	MatSum *= InvNun;
	float EigenValue[3];
	MatSum.GetEigenSystem(EigenValue,m_A);

	Vector3 Min, Max;
	Vector3 Diff = pPointArray[0] - m_Center;
	for(int32 j = 0 ; j < 3 ; j++)
	{
		Min.m[j] = m_A[j].Dot(Diff);
		Max.m[j] = Min.m[j];
	}
	for(uint32 i = 1 ; i < uiPointNum ; i++)
	{
		Diff = pPointArray[i] - m_Center;
		for(int32 j = 0 ; j < 3 ; j++)
		{
			float temp = m_A[j].Dot(Diff);
			if(temp < Min.m[j])
			{
				Min.m[j] = temp;
			}
			else if(temp > Max.m[j])
			{
				Max.m[j] = temp;
			}
		}
	}

	for(int32 j = 0 ; j < 3 ; j++)
	{
		m_Center += m_A[j] * (0.5f * ( Min.m[j] + Max.m[j]));
		m_fA[j] = 0.5f * (Max.m[j] - Min.m[j]);
	}
} 

OBB OBB::MergeOBB(const OBB& obb)const
{
	OBB Temp;
	Temp.m_Center = (obb.m_Center + m_Center) * 0.5f;
	Matrix3X3 m1,m2;
	m1.CreateRot(m_A[0],m_A[1],m_A[2]);
	m2.CreateRot(obb.m_A[0], obb.m_A[1], obb.m_A[2]);

	Quat q,q1,q2;
	q1 = m1.GetQuat();
	q2 = m2.GetQuat();
	if(q1.Dot(q2) < 0)
	{
		q2 *= -1.0f;
	}
	q = q1 + q2;
	q.Normalize();

	Matrix3X3 Mat;

	q.GetMatrix(Mat);
	Mat.GetUVN(Temp.m_A);

	//m_Center = (OBB1.m_Center + OBB2.m_Center) / 2.0f;

	Vector3 Point[8];

	GetPoint(Point);
	Vector3 Diff;
	Diff = Point[0] - Temp.m_Center;
	Vector3 Min = Vector3(Diff.Dot(Temp.m_A[0]), Diff.Dot(Temp.m_A[1]), Diff.Dot(Temp.m_A[2]));//= Point[0];
	Vector3 Max = Min;
	
	for(int32 i = 1 ; i < 8 ; i++)
	{
		Diff = Point[i] - Temp.m_Center;
		for(int32 j = 0 ; j < 3 ; j++)
		{
			float fDot = Diff.Dot(Temp.m_A[j]);
			if (fDot > Max.m[j])
			{
				Max.m[j] = fDot;
			}
			else if (fDot < Min.m[j])
			{
				Min.m[j] = fDot;
			}
		}
	}

    obb.GetPoint(Point);
	for(int32 i = 1 ; i < 8 ; i++)
	{
		Diff = Point[i] - Temp.m_Center;
		for(int32 j = 0 ; j < 3 ; j++)
		{
			float fDot = Diff.Dot(Temp.m_A[j]);
			if (fDot > Max.m[j])
			{
				Max.m[j] = fDot;
			}
			else if (fDot < Min.m[j])
			{
				Min.m[j] = fDot;
			}
		}
	}
	for(int32 i = 0 ; i < 3 ; i++)
	{
		Temp.m_Center += Temp.m_A[i] * ((Min.m[i] + Max.m[i]) * 0.5f);
		Temp.m_fA[i] = 0.5f * (Max.m[i] - Min.m[i]);
	}

	return Temp;
}

AABB OBB::GetAABB()const
{
	Vector3 Point[8];
	GetPoint(Point);
	AABB Temp;
	Temp.CreateAABB(Point,8);
	return Temp;
}

float OBB::SquaredDistance(const Vector3 & Point, float fOBBParameter[3])const
{
    Vector3 Diff = Point - m_Center;


    float fSqrDistance = 0.0f, fDelta;

    fOBBParameter[0] = Diff.Dot(m_A[0]);
    if (fOBBParameter[0] < -m_fA[0])
    {
        fDelta = fOBBParameter[0] + m_fA[0];
        fSqrDistance += fDelta * fDelta;
        fOBBParameter[0] = -m_fA[0];
    }
    else if (fOBBParameter[0] > m_fA[0])
    {
        fDelta = fOBBParameter[0] - m_fA[0];
        fSqrDistance += fDelta * fDelta;
        fOBBParameter[0] = m_fA[0];
    }

    fOBBParameter[1] = Diff.Dot(m_A[1]);
    if (fOBBParameter[1] < -m_fA[1])
    {
        fDelta = fOBBParameter[1] + m_fA[1];
        fSqrDistance += fDelta * fDelta;
        fOBBParameter[1] = -m_fA[1];
    }
    else if (fOBBParameter[1] > m_fA[1])
    {
        fDelta = fOBBParameter[1] - m_fA[1];
        fSqrDistance += fDelta * fDelta;
        fOBBParameter[1] = m_fA[1];
    }

    fOBBParameter[2] = Diff.Dot(m_A[2]);
    if (fOBBParameter[2] < -m_fA[2])
    {
        fDelta = fOBBParameter[2] + m_fA[2];
        fSqrDistance += fDelta * fDelta;
        fOBBParameter[2] = -m_fA[2];
    }
    else if (fOBBParameter[2] > m_fA[2])
    {
        fDelta = fOBBParameter[2] - m_fA[2];
        fSqrDistance += fDelta * fDelta;
        fOBBParameter[2] = m_fA[2];
    }
    return fSqrDistance;
}

float OBB::SquaredDistance(const Line & Line, float fOBBParameter[3], float &fLineParameter)const
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
                fOBBParameter[j] = fAABB[j];
            }

        }

    }

    return fSqrDist;
}

float OBB::SquaredDistance(const Triangle & Triangle, float OBBParameter[3], float TriangleParameter[3])const
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
        OBBParameter[0] = m_fA[0];
        OBBParameter[1] = RectP[0];
        OBBParameter[2] = RectP[1];
    }

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[1], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        OBBParameter[0] = -m_fA[0];
        OBBParameter[1] = RectP[0];
        OBBParameter[2] = -RectP[1];
    }

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[2], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        OBBParameter[0] = RectP[1];
        OBBParameter[1] = m_fA[1];
        OBBParameter[2] = RectP[0];
    }

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[3], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        OBBParameter[0] = -RectP[1];
        OBBParameter[1] = -m_fA[1];
        OBBParameter[2] = RectP[0];
    }

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[4], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        OBBParameter[0] = -RectP[1];
        OBBParameter[1] = RectP[0];
        OBBParameter[2] = m_fA[2];
    }

    fSqrDistTmp = Triangle.SquaredDistance(Rectangle[5], TriP, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        TriangleParameter[0] = TriP[0];
        TriangleParameter[1] = TriP[1];
        TriangleParameter[2] = TriP[2];
        OBBParameter[0] = RectP[1];
        OBBParameter[1] = RectP[0];
        OBBParameter[2] = -m_fA[2];
    }

    return fSqrDist;
}

float OBB::SquaredDistance(const Rectangle& rectangle, float OBBParameter[3], float RectangleParameter[2])const
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

        OBBParameter[0] = m_fA[0];
        OBBParameter[1] = RectP[0];
        OBBParameter[2] = RectP[1];
    }

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[1], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        OBBParameter[0] = -m_fA[0];
        OBBParameter[1] = RectP[0];
        OBBParameter[2] = -RectP[1];
    }

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[2], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        OBBParameter[0] = RectP[1];
        OBBParameter[1] = m_fA[1];
        OBBParameter[2] = RectP[0];
    }

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[3], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        OBBParameter[0] = -RectP[1];
        OBBParameter[1] = -m_fA[1];
        OBBParameter[2] = RectP[0];
    }

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[4], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        OBBParameter[0] = -RectP[1];
        OBBParameter[1] = RectP[0];
        OBBParameter[2] = m_fA[2];
    }

    fSqrDistTmp = rectangle.SquaredDistance(RectangleTemp[5], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        RectangleParameter[0] = RectP1[0];
        RectangleParameter[1] = RectP1[1];

        OBBParameter[0] = RectP[1];
        OBBParameter[1] = RectP[0];
        OBBParameter[2] = -m_fA[2];
    }

    return fSqrDist;
}

float OBB::SquaredDistance(const OBB &OBB, float OBB1Parameter[3], float OBB2Parameter[3])const
{

    Rectangle RectangleTemp[6];
    GetRectangle(RectangleTemp);
    float fSqrDist = MATH_MAX_FLOAT;
    float fSqrDistTmp;
    float RectP[2], RectP1[3];

    fSqrDistTmp = OBB.SquaredDistance(RectangleTemp[0], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        OBB1Parameter[0] = m_fA[0];
        OBB1Parameter[1] = RectP[0];
        OBB1Parameter[2] = RectP[1];

        OBB2Parameter[0] = RectP1[0];
        OBB2Parameter[1] = RectP1[0];
        OBB2Parameter[2] = RectP1[1];
    }

    fSqrDistTmp = OBB.SquaredDistance(RectangleTemp[1], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        OBB1Parameter[0] = -m_fA[0];
        OBB1Parameter[1] = RectP[0];
        OBB1Parameter[2] = -RectP[1];

        OBB2Parameter[0] = RectP1[0];
        OBB2Parameter[1] = RectP1[0];
        OBB2Parameter[2] = RectP1[1];
    }

    fSqrDistTmp = OBB.SquaredDistance(RectangleTemp[2], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        OBB1Parameter[0] = RectP[1];
        OBB1Parameter[1] = m_fA[1];
        OBB1Parameter[2] = RectP[0];

        OBB2Parameter[0] = RectP1[0];
        OBB2Parameter[1] = RectP1[0];
        OBB2Parameter[2] = RectP1[1];
    }

    fSqrDistTmp = OBB.SquaredDistance(RectangleTemp[3], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        OBB1Parameter[0] = -RectP[1];
        OBB1Parameter[1] = -m_fA[1];
        OBB1Parameter[2] = RectP[0];

        OBB2Parameter[0] = RectP1[0];
        OBB2Parameter[1] = RectP1[0];
        OBB2Parameter[2] = RectP1[1];
    }

    fSqrDistTmp = OBB.SquaredDistance(RectangleTemp[4], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        OBB1Parameter[0] = -RectP[1];
        OBB1Parameter[1] = RectP[0];
        OBB1Parameter[2] = m_fA[2];

        OBB2Parameter[0] = RectP1[0];
        OBB2Parameter[1] = RectP1[0];
        OBB2Parameter[2] = RectP1[1];
    }

    fSqrDistTmp = OBB.SquaredDistance(RectangleTemp[5], RectP1, RectP);
    if (fSqrDistTmp < fSqrDist)
    {
        fSqrDist = fSqrDistTmp;
        OBB1Parameter[0] = RectP[1];
        OBB1Parameter[1] = RectP[0];
        OBB1Parameter[2] = -m_fA[2];

        OBB2Parameter[0] = RectP1[0];
        OBB2Parameter[1] = RectP1[0];
        OBB2Parameter[2] = RectP1[1];
    }

    return fSqrDist;
}

float OBB::SquaredDistance(const Ray &Ray, float fOBBParameter[3], float &fRayParameter)const
{
    return Ray.SquaredDistance(*this, fRayParameter, fOBBParameter);
}

float OBB::SquaredDistance(const Segment & Segment, float fOBBParameter[3], float &fSegmentParameter)const
{
    return Segment.SquaredDistance(*this, fSegmentParameter, fOBBParameter);
}

float OBB::Distance(const Sphere & Sphere, float fOBBParameter[3], Vector3 & SpherePoint)const
{
    return Sphere.Distance(*this, SpherePoint, fOBBParameter);
}

float OBB::Distance(const Plane &Plane, Vector3 &OBBPoint, Vector3 &PlanePoint)const
{
    return Plane.Distance(*this, PlanePoint, OBBPoint);
}

float OBB::SquaredDistance(const Polygon &Polygon, float OBBParameter[3], int& IndexTriangle, float TriangleParameter[3])const
{
    return Polygon.SquaredDistance(*this, IndexTriangle, TriangleParameter, OBBParameter);
}

int32 OBB::RelationWith(const Vector3 &Point)const
{
    return Point.RelationWith(*this);
}

int32 OBB::RelationWith(const Line &Line, uint32 &Quantity, float &tNear, float &tFar)const
{
    return Line.RelationWith(*this, Quantity, tNear, tFar);
}

int32 OBB::RelationWith(const Ray &Ray, uint32 &Quantity, float &tNear, float &tFar)const
{
    return Ray.RelationWith(*this, Quantity, tNear, tFar);
}

int32 OBB::RelationWith(const Segment &Segment, uint32 &Quantity, float &tNear, float &tFar)const
{
    return Segment.RelationWith(*this, Quantity, tNear, tFar);
}

int32 OBB::RelationWith(const Plane &Plane)const
{
    Vector3 N = Plane.GetN();
    float fD = Plane.GetfD();
    float fRadius = ABS(m_fA[0] * (N.Dot(m_A[0])))
        + ABS(m_fA[1] * (N.Dot(m_A[1])))
        + ABS(m_fA[2] * (N.Dot(m_A[2])));


    float fTest = N.Dot(m_Center) + fD;


    if (fTest > fRadius)
        return MATH_FRONT;
    else if (fTest < -fRadius)
        return MATH_BACK;
    else
        return MATH_INTERSECT;

}

int32 OBB::RelationWith(const Triangle & Triangle)const
{
    return Triangle.RelationWith(*this);

}

int32 OBB::RelationWith(const Rectangle &Rectangle)const
{
    return Rectangle.RelationWith(*this);
}

int32 OBB::RelationWith(const OBB &OBB)const
{
    float T[3];
    Vector3 vcD = OBB.m_Center - m_Center;
    float matM[3][3];
    float ra, rb, t;

    matM[0][0] = m_A[0].Dot(OBB.m_A[0]);
    matM[0][1] = m_A[0].Dot(OBB.m_A[1]);
    matM[0][2] = m_A[0].Dot(OBB.m_A[2]);
    ra = m_fA[0];
    rb = OBB.m_fA[0] * ABS(matM[0][0]) +
        OBB.m_fA[1] * ABS(matM[0][1]) +
        OBB.m_fA[2] * ABS(matM[0][2]);

    T[0] = vcD.Dot(m_A[0]);
    t = ABS(T[0]);
    if (t > (ra + rb))
        return MATH_NOINTERSECT;

    matM[1][0] = m_A[1].Dot(OBB.m_A[0]);
    matM[1][1] = m_A[1].Dot(OBB.m_A[1]);
    matM[1][2] = m_A[1].Dot(OBB.m_A[2]);
    ra = m_fA[1];
    rb = OBB.m_fA[0] * ABS(matM[1][0]) +
        OBB.m_fA[1] * ABS(matM[1][1]) +
        OBB.m_fA[2] * ABS(matM[1][2]);
    T[1] = vcD.Dot(m_A[1]);
    t = ABS(T[1]);
    if (t > (ra + rb))
        return MATH_NOINTERSECT;

    matM[2][0] = m_A[2].Dot(OBB.m_A[0]);
    matM[2][1] = m_A[2].Dot(OBB.m_A[1]);
    matM[2][2] = m_A[2].Dot(OBB.m_A[2]);
    ra = m_fA[2];
    rb = OBB.m_fA[0] * ABS(matM[2][0]) +
        OBB.m_fA[1] * ABS(matM[2][1]) +
        OBB.m_fA[2] * ABS(matM[2][2]);
    T[2] = vcD.Dot(m_A[2]);
    t = ABS(T[2]);
    if (t > (ra + rb))
        return MATH_NOINTERSECT;

    ra = m_fA[0] * ABS(matM[0][0]) +
        m_fA[1] * ABS(matM[1][0]) +
        m_fA[2] * ABS(matM[2][0]);
    rb = OBB.m_fA[0];
    t = ABS(T[0] * matM[0][0] + T[1] * matM[1][0] + T[2] * matM[2][0]);
    if (t > (ra + rb))
        return MATH_NOINTERSECT;


    ra = m_fA[0] * ABS(matM[0][1]) +
        m_fA[1] * ABS(matM[1][1]) +
        m_fA[2] * ABS(matM[2][1]);
    rb = OBB.m_fA[1];
    t = ABS(T[0] * matM[0][1] + T[1] * matM[1][1] + T[2] * matM[2][1]);
    if (t > (ra + rb))
        return MATH_NOINTERSECT;

    ra = m_fA[0] * ABS(matM[0][2]) +
        m_fA[1] * ABS(matM[1][2]) +
        m_fA[2] * ABS(matM[2][2]);
    rb = OBB.m_fA[2];
    t = ABS(T[0] * matM[0][2] + T[1] * matM[1][2] + T[2] * matM[2][2]);
    if (t > (ra + rb))
        return MATH_NOINTERSECT;


    ra = m_fA[1] * ABS(matM[2][0]) + m_fA[2] * ABS(matM[1][0]);
    rb = OBB.m_fA[1] * ABS(matM[0][2]) + OBB.m_fA[2] * ABS(matM[0][1]);
    t = ABS(T[2] * matM[1][0] - T[1] * matM[2][0]);
    if (t > ra + rb)
        return MATH_NOINTERSECT;

    ra = m_fA[1] * ABS(matM[2][1]) + m_fA[2] * ABS(matM[1][1]);
    rb = OBB.m_fA[0] * ABS(matM[0][2]) + OBB.m_fA[2] * ABS(matM[0][0]);
    t = ABS(T[2] * matM[1][1] - T[1] * matM[2][1]);
    if (t > ra + rb)
        return MATH_NOINTERSECT;


    ra = m_fA[1] * ABS(matM[2][2]) + m_fA[2] * ABS(matM[1][2]);
    rb = OBB.m_fA[0] * ABS(matM[0][1]) + OBB.m_fA[1] * ABS(matM[0][0]);
    t = ABS(T[2] * matM[1][2] - T[1] * matM[2][2]);
    if (t > ra + rb)
        return MATH_NOINTERSECT;

    ra = m_fA[0] * ABS(matM[2][0]) + m_fA[2] * ABS(matM[0][0]);
    rb = OBB.m_fA[1] * ABS(matM[1][2]) + OBB.m_fA[2] * ABS(matM[1][1]);
    t = ABS(T[0] * matM[2][0] - T[2] * matM[0][0]);
    if (t > ra + rb)
        return MATH_NOINTERSECT;


    ra = m_fA[0] * ABS(matM[2][1]) + m_fA[2] * ABS(matM[0][1]);
    rb = OBB.m_fA[0] * ABS(matM[1][2]) + OBB.m_fA[2] * ABS(matM[1][0]);
    t = ABS(T[0] * matM[2][1] - T[2] * matM[0][1]);
    if (t > ra + rb)
        return MATH_NOINTERSECT;

    ra = m_fA[0] * ABS(matM[2][2]) + m_fA[2] * ABS(matM[0][2]);
    rb = OBB.m_fA[0] * ABS(matM[1][1]) + OBB.m_fA[1] * ABS(matM[1][0]);
    t = ABS(T[0] * matM[2][2] - T[2] * matM[0][2]);
    if (t > ra + rb)
        return MATH_NOINTERSECT;

    ra = m_fA[0] * ABS(matM[1][0]) + m_fA[1] * ABS(matM[0][0]);
    rb = OBB.m_fA[1] * ABS(matM[2][2]) + OBB.m_fA[2] * ABS(matM[2][1]);
    t = ABS(T[1] * matM[0][0] - T[0] * matM[1][0]);
    if (t > ra + rb)
        return MATH_NOINTERSECT;


    ra = m_fA[0] * ABS(matM[1][1]) + m_fA[1] * ABS(matM[0][1]);
    rb = OBB.m_fA[0] * ABS(matM[2][2]) + OBB.m_fA[2] * ABS(matM[2][0]);
    t = ABS(T[1] * matM[0][1] - T[0] * matM[1][1]);
    if (t > ra + rb)
        return MATH_NOINTERSECT;


    ra = m_fA[0] * ABS(matM[1][2]) + m_fA[1] * ABS(matM[0][2]);
    rb = OBB.m_fA[0] * ABS(matM[2][1]) + OBB.m_fA[1] * ABS(matM[2][0]);
    t = ABS(T[1] * matM[0][2] - T[0] * matM[1][2]);
    if (t > ra + rb)
        return MATH_NOINTERSECT;

    return MATH_INTERSECT;
}

int32 OBB::RelationWith(const Sphere &Sphere)const
{
    float fOBBParameter[3];
    float Dist = Sphere.m_Center.SquaredDistance(*this, fOBBParameter);
    Dist = SQRT(Dist);
    if (Dist - Sphere.m_fRadius < 0.0f)
        return MATH_INTERSECT;

    return MATH_NOINTERSECT;
}

}