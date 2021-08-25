#pragma once


#include "graphic/node/mesh/triangleset.h"
#include "math/aabb.h"
#include "math/obb.h"
#include "math/surface.h"
#include "math/surfacesubdivision.h"
#include "math/polygon.h"
#include "math/sphere.h"


namespace zq{


class GRAPHIC_API VSOrdinaryTriangleSet : public VSTriangleSet
{
public:
	//PRIORITY
		
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	VSOrdinaryTriangleSet(const AABB& aabb);
	VSOrdinaryTriangleSet(const OBB& obb);
	VSOrdinaryTriangleSet(const Sphere& sphere);
	VSOrdinaryTriangleSet(Surface* pSurface ,SurfaceSubdivision * pSubidvision);
	VSOrdinaryTriangleSet(const Plane& plane);
	VSOrdinaryTriangleSet(const Triangle& triangle);
	VSOrdinaryTriangleSet(const Rectangle& rectangle);
	VSOrdinaryTriangleSet(const Polygon& polyon);
	virtual ~VSOrdinaryTriangleSet();

protected:
	VSOrdinaryTriangleSet();

};


DECLARE_Ptr(VSOrdinaryTriangleSet);
VSTYPE_MARCO(VSOrdinaryTriangleSet);


};
