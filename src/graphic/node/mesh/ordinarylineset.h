#pragma once


#include "graphic/node/mesh/lineset.h"
#include "math/curve.h"
#include "math/curvesubdivision.h"
#include "math/line.h"
#include "math/ray.h"
#include "math/segment.h"


namespace zq{


class GRAPHIC_API VSOrdinaryLineSet : public VSLineSet
{
public:
	DECLARE_RTTI;
	DECLARE_INITIAL

public:
	VSOrdinaryLineSet(ControlCurve * pControlCurve,CurveSubdivision * pSubidvision);
	VSOrdinaryLineSet(const Line & Line);
	VSOrdinaryLineSet(const Ray & Ray);
	VSOrdinaryLineSet(const Segment & Segment);
	virtual ~VSOrdinaryLineSet();
	
protected:
	VSOrdinaryLineSet();

};
DECLARE_Ptr(VSOrdinaryLineSet);
VSTYPE_MARCO(VSOrdinaryLineSet);

};
