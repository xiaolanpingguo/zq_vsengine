#include "math/cylinder.h"


namespace zq {


Cylinder::Cylinder(Vector3 Center,Vector3 Dir,float fRadius, float fHalfHeight)
{
	m_Center = Center;
	m_Dir = Dir;
	m_fRadius = fRadius;
	m_fHalfHeight = fHalfHeight;
}

Cylinder::~Cylinder()
{

}

}