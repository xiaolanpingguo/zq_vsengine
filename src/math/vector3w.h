#pragma once


#include "math/maths.h"
#include "math/vector3.h"


namespace zq{


class Matrix3X3W;
class MATH_API HalfVector3W
{
public:
	union
	{
		unsigned short m[4];
		struct
		{
			unsigned short x, y, z, w;
		};
		struct
		{
			unsigned short r, g, b, a;
		};
	};
	HalfVector3W operator * (float f)const;
	HalfVector3W operator + (const HalfVector3W &v)const;
	HalfVector3W();
	HalfVector3W(unsigned short _x, unsigned short _y, unsigned short _z, unsigned short _w);
};



class MATH_API Vector3W 
{
public:
	union
	{
		float m[4];
		struct  
		{
			float x, y, z, w;
		};
		struct  
		{
			float r,g,b,a;
		};
	};


	Vector3W(void);
	Vector3W(float _x, float _y, float _z,float _w = 1.0f);
	Vector3W(const Vector3 & V);
	Vector3W(unsigned char R,unsigned char G,unsigned char B,unsigned char A = 255);


    inline void Set(float _x, float _y, float _z, float _w)
    {
        x = _x; y = _y; z = _z; w = _w;
    }

    Vector3 GetV3()const
    {
        return Vector3(x, y, z);
    }

    inline void GetV3(Vector3 & V)const
    {
        V.x = x;
        V.y = y;
        V.z = z;
    }

    inline void Set(const Vector3 & V)
    {
        x = V.x;
        y = V.y;
        z = V.z;
        w = 1.0f;
    }


	Vector3W operator * (const Matrix3X3W &m)const; 		
	void operator += (const Vector3W &v);          
	void operator -= (const Vector3W &v);          
	void operator *= (const Vector3W &v);          
	void operator /= (const Vector3W &v); 
	void operator *= (float f);                     
	void operator /= (float f);                     
	void operator += (float f);                     
	void operator -= (float f);                     
	Vector3W operator * (float f)const;            
	Vector3W operator / (float f)const;            
	Vector3W operator + (float f)const;            
	Vector3W operator - (float f)const;            
	Vector3W operator + (const Vector3W &v)const; 
	Vector3W operator - (const Vector3W &v)const;
	Vector3W operator * (const Vector3W &v)const;
	Vector3W operator / (const Vector3W &v)const;
	bool operator ==(const Vector3W &v)const;

    uint32 GetDWARGB()const;
    uint32 GetDWRGBA()const;

    uint32 GetDWBGRA()const;
    uint32 GetDWABGR()const;
	void GetUCColor(unsigned char &R,unsigned char &G,unsigned char &B,unsigned char &A)const;

	void CreateFromARGB(uint32 ARGB);
	void CreateFromBGRA(uint32 BGRA);
	void CreateFromRGBA(uint32 RGBA);
	void CreateFromABGR(uint32 ABGR);
};  // class


typedef class Vector3W VSColorRGBA;


inline uint32 ColorABGR(unsigned char a, unsigned char r, unsigned char g,unsigned char b)
{
	return ((unsigned int)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)));
}

inline void ColorGetARGB(uint32 ARGB,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b)
{
	a = (ARGB>>24) & 0xff;
	r = (ARGB>>16) & 0xff;
	g = (ARGB>>8) & 0xff;
	b = (ARGB) & 0xff;
}

inline void ColorGetBGRA(uint32 BGRA,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b)
{
	b = (BGRA>>24) & 0xff;
	g = (BGRA>>16) & 0xff;
	r = (BGRA>>8) & 0xff;
	a = (BGRA) & 0xff;
}

inline void ColorGetRGBA(uint32 RGBA,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b)
{
	r = (RGBA>>24) & 0xff;
	g = (RGBA>>16) & 0xff;
	b = (RGBA>>8) & 0xff;
	a = (RGBA) & 0xff;
}

inline void ColorGetABGR(uint32 ABGR,unsigned char &a, unsigned char &r, unsigned char &g,unsigned char &b)
{
	a = (ABGR>>24) & 0xff;
	b = (ABGR>>16) & 0xff;
	g = (ABGR>>8) & 0xff;
	r = (ABGR) & 0xff;
}

inline HalfVector3W FloatToHalf(const Vector3W& v)
{
	return HalfVector3W(FloatToHalf(v.x), FloatToHalf(v.y), FloatToHalf(v.z), FloatToHalf(v.w));
}

inline Vector3W HalfToFloat(const HalfVector3W& v)
{
	return Vector3W(HalfToFloat(v.x), HalfToFloat(v.y), HalfToFloat(v.z), HalfToFloat(v.w));
}

}