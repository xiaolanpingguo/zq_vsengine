#pragma once


#include "math/maths.h"


namespace zq{


class MATH_API Vector2
{
public:
	union
	{
		float m[2];
		struct  
		{
			float x, y;
		};
	};
	Vector2(void);
	Vector2(float _x, float _y);

	
    inline void Set(float _x, float _y)
    {
        x = _x; y = _y;
    }

    inline void Normalize(void)
    {
        float f = x * x + y * y;
        if (f > EPSILON_E4)
        {

#ifdef VS_SSE
            f = InvSqrt(f);
#else
            f = (float)1.0f / SQRT(f);
#endif
            x *= f; y *= f;
        }
        else
        {
            Set(0.0f, 0.0f);
        }
    }

		
	void operator += (const Vector2 &v);          
	void operator -= (const Vector2 &v);          
	void operator *= (float f);                     
	void operator /= (float f);                     
	void operator += (float f);                     
	void operator -= (float f);                     
	float operator* (const Vector2 &v)const;
	bool operator ==(const Vector2 &v)const;
	Vector2 operator * (float f)const;            
	Vector2 operator / (float f)const;            
	Vector2 operator + (float f)const;            
	Vector2 operator - (float f)const;            
	Vector2 operator + (const Vector2 &v)const; 
	Vector2 operator - (const Vector2 &v)const; 
};


inline bool VS2DIntersect(Vector2 & A1,Vector2 & B1,Vector2 & A2 ,Vector2 & B2, Vector2 & Out)
{
	float denominator = (B1.y * (A2.x - B2.x) + A1.y * (B2.x - A2.x) + (A1.x - B1.x) * (A2.y - B2.y));

	if(ABS(denominator) < 0.0001)
		return false;

	Out = Vector2((-B1.x * A2.y * B2.x + A1.y * B1.x * (B2.x - A2.x) + B1.x * A2.x * B2.y + A1.x * (B1.y * A2.x - B2.y * A2.x - B1.y * B2.x + A2.y * B2.x)) / denominator,
		( B1.y * (-A2.y * B2.x + A1.x * (A2.y - B2.y) + A2.x * B2.y) + A1.y * (A2.y * B2.x - A2.x * B2.y + B1.x * (B2.y - A2.y))) / denominator);

	return true;
}


}