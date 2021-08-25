#pragma once


#include "math/vector3.h"
#include "math/matrix3X3.h"
#include "math/quat.h"
#include "math/vector3w.h"


namespace zq{


bool MATH_API IsUniformScale(float fScale);
bool MATH_API IsZeroTranslate(const Vector3 & Translate);
bool MATH_API IsIdentityRotate(const Matrix3X3 & Rotate);
bool MATH_API IsIdentityRotate(const Quat & Rotate);

// 	template< typename T>
// 	T LineInterpolation (T t1, T t2, float t)
// 	{
// 		return t1 + (t2 - t1) * t;
// 	}
// 	Quat LineInterpolation (Quat t1, Quat t2, float t)
// 	{
// 		float fCos = t2.Dot(t1);
// 		if(fCos < 0.0f)
// 		{
// 			t1 *= -1.0f;
// 		}
// 		return t1 + (t2 - t1) * t;
// 	}
float MATH_API LineInterpolation (float t1, float t2, float t);
Vector3 MATH_API LineInterpolation (const Vector3& t1, const Vector3& t2, float t);
Quat MATH_API LineInterpolation(const Quat& t1, const Quat& t2, float t);
Vector3W MATH_API LineInterpolation(const Vector3W& t1, const Vector3W& t2, float t);

}
