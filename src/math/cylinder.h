#pragma once


#include "math/vector3.h"


namespace zq{


class MATH_API Cylinder
{
public:
    Cylinder(Vector3 Center, Vector3 Dir, float fRadius, float fHalfHeight);
    ~Cylinder();
    Vector3	m_Center;
    Vector3	m_Dir;
    float		m_fRadius;
    float		m_fHalfHeight;
};

}
