#include "math/rect2.h"


namespace zq{
	
	
VSRect2::VSRect2(void)
{

}
VSRect2::VSRect2(const Vector2& Min, const Vector2 &Max)
{
	m_Min = Min;
	m_Max = Max;
}

}