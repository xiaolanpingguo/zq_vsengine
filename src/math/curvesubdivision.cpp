#include "math/curvesubdivision.h"
#include "core/memorymanager.h"
#include "math/b_splinecurve.h"

#include "math/segment.h"


namespace zq{


CurveSubdivision::CurveSubdivision()
{
	m_pPoint = NULL;
	m_uiPointNum = 0;
	m_pCurve = NULL;
	m_pT = NULL;
}

CurveSubdivision::~CurveSubdivision()
{
	SAFE_DELETEA(m_pPoint);
	SAFE_DELETEA(m_pT);
	m_uiPointNum = 0;
}

bool CurveSubdivision::SetSubCurve(ControlCurve * pCurve)
{
	VSMAC_ASSERT(pCurve);
	m_pCurve = pCurve;
	return 1;
}

bool CurveSubdivision::ControlPointChange(uint32 i)
{
	VSMAC_ASSERT(m_pCurve && m_pPoint && m_pT && m_uiPointNum);
	VSMAC_ASSERT(i < m_pCurve->GetControlPointNum());

	if(m_pCurve->GetType() == ControlCurve::BEZIER)
	{
		for(uint32 i = 0; i < m_uiPointNum ; i++)
			m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
		return 1;
	}
	else if(m_pCurve->GetType() == ControlCurve::B_SPLINE || m_pCurve->GetType() == ControlCurve::NURBS)
	{
		BSplineCurve *pCurve =(BSplineCurve *)m_pCurve;
		float t1 =pCurve->GetKnotVectorValue(i);
		float t2 =pCurve->GetKnotVectorValue(pCurve->GetDegree() + i); 
		for(uint32 i = 0; i < m_uiPointNum ; i++)
			if(m_pT[i] >= t1 && m_pT[i] <= t2)
				m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
		return 1;
	}

	return 0;
}





// -------------CurveSubdivisionByError------------------
CurveSubdivisionByError::CurveSubdivisionByError(float t1, float t2, uint32 Level, float error)
{
    m_pPointHead = NULL;
    m_uiPointNum = 0;
    m_uiLevel = Level;
    m_fError = error;
    m_ft1 = t1;
    m_ft2 = t2;
}

CurveSubdivisionByError::~CurveSubdivisionByError()
{
    m_pNext = m_pPointHead;
    while (m_pNext)
    {
        POINTLIST * Temp = m_pNext;
        m_pNext = m_pNext->next;
        SAFE_DELETE(Temp);
    }
}

void CurveSubdivisionByError::Clear()
{
    m_pNext = m_pPointHead;
    while (m_pNext)
    {
        POINTLIST * Temp = m_pNext;
        m_pNext = m_pNext->next;
        SAFE_DELETE(Temp);
    }

    m_pPointHead = NULL;
    m_pNext = NULL;
}

bool CurveSubdivisionByError::Subdivision()
{
    if (!m_uiLevel || !m_pCurve)
        return 0;
    if (m_ft2 < m_ft1)
        return 0;
    Clear();

    m_uiPointNum = 0;
    Vector3 Point1 = m_pCurve->GetPoint(m_ft1);
    Vector3 Point2 = m_pCurve->GetPoint(m_ft2);

    m_pPointHead = MEM_NEW POINTLIST;
    VSMAC_ASSERT(m_pPointHead);

    m_pPointHead->Point = Point1;
    m_pPointHead->next = NULL;
    m_uiPointNum++;
    m_pNext = m_pPointHead;

    if (!SubdivisionBisect(m_uiLevel, m_fError, m_ft1, m_ft2, Point1, Point2))
    {

        Clear();
        return 0;
    }

    if (!m_uiPointNum)
        return 0;
    SAFE_DELETEA(m_pPoint);
    m_pPoint = MEM_NEW Vector3[m_uiPointNum];
    VSMAC_ASSERT(m_pPoint);


    SAFE_DELETEA(m_pT);
    m_pT = MEM_NEW float[m_uiPointNum];
    VSMAC_ASSERT(m_pT);

    uint32 i = 0;
    m_pNext = m_pPointHead;
    while (m_pNext)
    {
        m_pPoint[i] = m_pNext->Point;
        m_pT[i] = m_pNext->t;
        POINTLIST * Temp = m_pNext;
        m_pNext = m_pNext->next;
        i++;
        //SAFE_DELETE(Temp);
    }

    Clear();
    return 1;
}

bool CurveSubdivisionByError::SubdivisionBisect(uint32 Level, float error, float t1, float t2, const Vector3 &Point1, const Vector3 &Point2)
{
    float tm = (t1 + t2) / (float)2.0;
    if (Level > 0)
    {
        Vector3 Point = m_pCurve->GetPoint(tm);
        Segment s;
        s.Set(Point1, Point2);
        float Length = s.GetLen();
        float t;
        float Dis = s.SquaredDistance(Point, t);
        float TempError = Dis / Length;
        if (TempError > error)
        {
            if (!SubdivisionBisect(Level - 1, error, t1, tm, Point1, Point))
                return 0;
            if (!SubdivisionBisect(Level - 1, error, tm, t2, Point, Point2))
                return 0;
            return 1;
        }
    }

    POINTLIST * Temp = NULL;
    Temp = MEM_NEW POINTLIST;
    VSMAC_ASSERT(Temp);
    Temp->Point = Point2;
    Temp->t = tm;
    Temp->next = NULL;
    m_pNext->next = Temp;
    m_pNext = Temp;
    m_uiPointNum++;
    return 1;
}





// -------------CurveSubdivisionByLength------------------
CurveSubdivisionByLength::CurveSubdivisionByLength(uint32 PointNum, float t1, float t2, uint32 uiIterations)
{
    m_uiPointNum = PointNum;
    m_ft1 = t1;
    m_ft2 = t2;
    m_uiIterations = uiIterations;
}

CurveSubdivisionByLength::~CurveSubdivisionByLength()
{
}

bool CurveSubdivisionByLength::Subdivision()
{
    if (!m_uiPointNum || !m_pCurve)
        return 0;
    if (m_ft1 > m_ft2)
        return 0;
    SAFE_DELETEA(m_pPoint);
    m_pPoint = MEM_NEW Vector3[m_uiPointNum];
    VSMAC_ASSERT(m_pPoint);

    SAFE_DELETEA(m_pT);
    m_pT = MEM_NEW float[m_uiPointNum];
    VSMAC_ASSERT(m_pT);

    float det = m_pCurve->GetLength(m_ft1, m_ft2, m_uiIterations) / float(m_uiPointNum - 1);
    float temp = m_ft1;
    for (uint32 i = 0; i < m_uiPointNum; i++)
    {
        m_pT[i] = m_pCurve->GetTime(temp, m_uiIterations);
        m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
        temp += det;

    }

    return 1;
}

void CurveSubdivisionByLength::SetPointNum(uint32 uiPointNum)
{
    VSMAC_ASSERT(uiPointNum);
    m_uiPointNum = uiPointNum;
}





// -------------CurveSubdivisionByUniform------------------
CurveSubdivisionByUniform::CurveSubdivisionByUniform(uint32 PointNum, float t1, float t2)
{
    m_uiPointNum = PointNum;
    m_ft1 = t1;
    m_ft2 = t2;
}

CurveSubdivisionByUniform::~CurveSubdivisionByUniform()
{
}

bool CurveSubdivisionByUniform::Subdivision()
{
    if (!m_uiPointNum || !m_pCurve)
        return 0;
    if (m_ft1 > m_ft2)
        return 0;
    SAFE_DELETEA(m_pPoint);
    m_pPoint = MEM_NEW Vector3[m_uiPointNum];
    VSMAC_ASSERT(m_pPoint);

    SAFE_DELETEA(m_pT);
    m_pT = MEM_NEW float[m_uiPointNum];
    VSMAC_ASSERT(m_pT);

    float det = (m_ft2 - m_ft1) / float(m_uiPointNum - 1);
    float temp = m_ft1;
    for (uint32 i = 0; i < m_uiPointNum; i++)
    {
        m_pT[i] = temp;
        m_pPoint[i] = m_pCurve->GetPoint(temp);
        temp += det;

    }

    return 1;
}

void CurveSubdivisionByUniform::SetPointNum(uint32 uiPointNum)
{
    VSMAC_ASSERT(uiPointNum);
    m_uiPointNum = uiPointNum;
}

}