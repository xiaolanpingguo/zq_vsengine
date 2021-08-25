#include "math/vector2.h"


namespace zq{


Vector2::Vector2(void) 
{
	x=0;y=0;
}
/*----------------------------------------------------------------*/
Vector2::Vector2(float _x, float _y)
{
	x=_x; y=_y; 
}
/*----------------------------------------------------------------*/
void Vector2::operator += (const Vector2 &v)
{
	x += v.x;   y += v.y; 
}
/*----------------------------------------------------------------*/


Vector2 Vector2::operator + (const Vector2 &v) const 
{
	return Vector2(x+v.x, y+v.y);
}
/*----------------------------------------------------------------*/
void Vector2::operator -= (const Vector2 &v) 
{
	x -= v.x;   y -= v.y; 

}
/*----------------------------------------------------------------*/

Vector2 Vector2::operator - (const Vector2 &v) const 
{
	return Vector2(x-v.x, y-v.y);
}
/*----------------------------------------------------------------*/

void Vector2::operator *= (float f) 
{
	x *= f;   y *= f;  
}
/*----------------------------------------------------------------*/

void Vector2::operator /= (float f) 
{
	x /= f;   y /= f; 
}
/*----------------------------------------------------------------*/

Vector2 Vector2::operator * (float f) const 
{
	return Vector2(x*f, y*f);
}
/*----------------------------------------------------------------*/

Vector2 Vector2::operator / (float f) const 
{
	return Vector2(x/f, y/f);
}
/*----------------------------------------------------------------*/

void Vector2::operator += (float f) 
{
	x += f;   y += f; 
}
/*----------------------------------------------------------------*/

void Vector2::operator -= (float f) 
{
	x -= f;   y -= f; 
}
/*----------------------------------------------------------------*/

Vector2 Vector2::operator + (float f) const 
{
	return Vector2(x+f, y+f);
}
/*----------------------------------------------------------------*/

Vector2 Vector2::operator - (float f) const 
{
	return Vector2(x-f, y-f);
}
/*----------------------------------------------------------------*/

float Vector2::operator * (const Vector2 &v) const 
{
	return (v.x*x + v.y*y);
}
/*----------------------------------------------------------------*/
bool Vector2::operator ==(const Vector2 &v)const
{
	for (uint32 i = 0 ; i < 2 ; i++)
	{
		if (ABS(m[i] - v.m[i]) > EPSILON_E4)
		{
			return false;
		}
	}
	return true;
// 	return (ABS(x-v.x) < EPSILON_E4 && 
// 		ABS(y-v.y) < EPSILON_E4);
}

}