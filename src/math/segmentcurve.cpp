#include "math/segmentcurve.h"


namespace zq{


SegmentCurve::SegmentCurve()
{
}

SegmentCurve::~SegmentCurve()
{	
}

void SegmentCurve::Set(const Segment &Segment)
{
	m_Segment = Segment;
}

Segment & SegmentCurve::GetSegment()
{
	return m_Segment;
}

}