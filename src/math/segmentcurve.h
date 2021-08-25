#pragma once


#include "math/curve.h"
#include "math/segment.h"


namespace zq{


class MATH_API SegmentCurve : public Curve
{
public:
    SegmentCurve();
    ~SegmentCurve();


    inline virtual void GetFrenetFrameBNT(float t, Vector3& B, Vector3& N, Vector3& T)
    {
        Curve::GetFixedUpBNT(t, B, N, T);
    }

    inline virtual Vector3 GetFirstDerivative(float t)
    {

        return m_Segment.GetDir() * m_Segment.GetLen();
    }

    inline virtual Vector3 GetSecondDerivative(float t)
    {
        return Vector3(0, 0, 0);
    }

    inline virtual Vector3 GetThirdDerivative(float t)
    {
        return Vector3(0, 0, 0);
    }

    inline virtual float GetLength(float t1, float t2, uint32 uiIterations)
    {
        return m_Segment.GetLen() * (t2 - t1);
    }

    inline virtual Vector3 GetPoint(float t)
    {
        return m_Segment.GetParameterPoint(t * m_Segment.GetLen());
    }

    inline virtual float GetTotalLength(uint32 uiIterations)
    {
        return m_Segment.GetLen();
    }

    inline virtual float GetTime(float fLength, uint32 uiIterations, float fTolerance)
    {
        return fLength / m_Segment.GetLen();
    }

    inline virtual float GetCurvature(float fTime)
    {
        return 0;
    }

    inline virtual float GetTorsion(float fTime)
    {
        return 0;
    }


    void Set(const Segment& Segment);
    Segment & GetSegment();
    virtual uint32 GetCurveType() { return CT_SEGMENT; }

private:
    Segment m_Segment;
};

}