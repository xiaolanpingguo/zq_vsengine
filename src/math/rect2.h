#pragma once
#include "math/maths.h"
#include "math/vector2.h"
namespace zq
{
	class MATH_API VSRect2
	{
	public:
		Vector2 m_Min;
		Vector2 m_Max;
		VSRect2(void);
		VSRect2(const Vector2& Min, const Vector2 &Max);

	};


}