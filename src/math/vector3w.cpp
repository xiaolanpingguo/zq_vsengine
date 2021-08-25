#include "math/vector3w.h"
#include "math/matrix3X3w.h"


namespace zq{


HalfVector3W::HalfVector3W()
{
	x = 0, y = 0, z = 0, w = 0;
}

HalfVector3W::HalfVector3W(unsigned short _x, unsigned short _y, unsigned short _z, unsigned short _w)
{
	x = _x; y = _y; z = _z; w = _w;
}

HalfVector3W HalfVector3W::operator + (const HalfVector3W &v)const
{
	Vector3W v1 = HalfToFloat(*this);
	Vector3W v2 = HalfToFloat(v);
	return HalfVector3W(FloatToHalf(v1 + v2));
}

HalfVector3W HalfVector3W::operator * (float f)const
{
	Vector3W v1 = HalfToFloat(*this) * f;
	return HalfVector3W(FloatToHalf(v1));
}

Vector3W::Vector3W(void) 
{
	x=0, y=0, z=0, w=1.0f; 
}

Vector3W::Vector3W(float _x, float _y, float _z,float _w)
{
	x=_x; y=_y; z=_z; w=_w; 
}

Vector3W::Vector3W(unsigned char R,unsigned char G,unsigned char B,unsigned char A)
{
	r = (R * (float)1.0) / 255;
	g = (G * (float)1.0) / 255;
	b = (B * (float)1.0) / 255;
	a = (A * (float)1.0) / 255;
}

Vector3W::Vector3W(const Vector3 & V)
{
	Set(V);
}

void Vector3W::CreateFromARGB(uint32 ARGB)
{
	unsigned char a,r,g,b;
	ColorGetARGB(ARGB,a,r,g,b);
	*this = Vector3W(r, g, b, a);
}

void Vector3W::CreateFromBGRA(uint32 BGRA)
{
	unsigned char a,r,g,b;
	ColorGetBGRA(BGRA,a,r,g,b);
	*this = Vector3W(r,g,b,a);
}

void Vector3W::CreateFromRGBA(uint32 RGBA)
{
	unsigned char a,r,g,b;
	ColorGetRGBA(RGBA,a,r,g,b);
	*this = Vector3W(r,g,b,a);
}

void Vector3W::CreateFromABGR(uint32 ABGR)
{
	unsigned char a,r,g,b;
	ColorGetABGR(ABGR,a,r,g,b);
	*this = Vector3W(r,g,b,a);
}

Vector3W Vector3W::operator * (const Matrix3X3W &m) const 
{
	Vector3W vcResult;

#ifdef VS_SSE
	{
		VectorRegister _v = VectorLoad(this->m);
		VectorRegister _m0 = VectorLoad(&m.m[0]);
		VectorRegister _m1 = VectorLoad(&m.m[4]);
		VectorRegister _m2 = VectorLoad(&m.m[8]);
		VectorRegister _m3 = VectorLoad(&m.m[12]);
		// Splat x,y,z and w
		VectorRegister VTempX = VectorReplicate(_v, 0);
		VectorRegister VTempY = VectorReplicate(_v, 1);
		VectorRegister VTempZ = VectorReplicate(_v, 2);
		VectorRegister VTempW = VectorReplicate(_v, 3);
		// Mul by the matrix
		VTempX = VectorMultiply(VTempX, _m0);
		VTempY = VectorMultiply(VTempY, _m1);
		VTempZ = VectorMultiply(VTempZ, _m2);
		VTempW = VectorMultiply(VTempW, _m3);
		// Add them all together
		VTempX = VectorAdd(VTempX, VTempY);
		VTempZ = VectorAdd(VTempZ, VTempW);
		VTempX = VectorAdd(VTempX, VTempZ);

		VectorStore(VTempX, &vcResult);
	}
#else
	{
		vcResult.x = x*m._00 + y*m._10 + z*m._20 + w * m._30;
		vcResult.y = x*m._01 + y*m._11 + z*m._21 + w * m._31;
		vcResult.z = x*m._02 + y*m._12 + z*m._22 + w * m._32;
		vcResult.w = x*m._03 + y*m._13 + z*m._23 + w * m._33;

	}
#endif

	return vcResult;
}

void Vector3W::operator += (const Vector3W &v)
{
	x += v.x;   y += v.y;   z += v.z; w += v.w;
}

void Vector3W::operator -= (const Vector3W &v)
{
	x -= v.x;   y -= v.y;   z -= v.z; w -= v.w;
}

void Vector3W::operator *= (const Vector3W &v)
{
	x *= v.x;   y *= v.y;   z *= v.z; w *= v.w;
}

void Vector3W::operator /= (const Vector3W &v)
{
	x /= v.x;   y /= v.y;   z /= v.z; w /= v.w;
}

void Vector3W::operator *= (float f)
{
	x *= f;   y *= f;   z *= f; w *= f;
}

void Vector3W::operator /= (float f)
{
	x /= f;   y /= f;   z /= f; w /= f;
}

void Vector3W::operator += (float f)
{
	x += f;   y += f;   z += f; w += f;
}

void Vector3W::operator -= (float f)
{
	x -= f;   y -= f;   z -= f; w -= f;
}

Vector3W Vector3W::operator * (float f)const
{
	return Vector3W(x*f, y*f, z*f,w*f);	
}

Vector3W Vector3W::operator / (float f)const
{
	return Vector3W(x/f, y/f, z/f,w/f);
}

Vector3W Vector3W::operator + (float f)const
{
	return Vector3W(x+f, y+f, z+f,w+f);
}

Vector3W Vector3W::operator - (float f)const
{
	return Vector3W(x-f, y-f, z-f,w-f);
}

Vector3W Vector3W::operator + (const Vector3W &v)const
{
	return Vector3W(x+v.x, y+v.y, z+v.z,w+v.w);
}

Vector3W Vector3W::operator - (const Vector3W &v)const
{
	return Vector3W(x-v.x, y-v.y, z-v.z,w-v.w);
}

Vector3W Vector3W::operator * (const Vector3W &v)const
{
	return Vector3W(x*v.x, y*v.y, z*v.z,w*v.w);
}

Vector3W Vector3W::operator / (const Vector3W &v)const
{
	return Vector3W(x-v.x, y-v.y, z-v.z,w-v.w);
}

bool Vector3W::operator ==(const Vector3W &v)const
{
// 	for (uint32 i = 0 ; i < 4 ; i++)
// 	{
// 		if (ABS(m[i] - v.m[i]) > EPSILON_E4)
// 		{
// 			return false;
// 		}
// 	}
// 	return true;
	return (ABS(x-v.x) < EPSILON_E4 && 
		ABS(y-v.y) < EPSILON_E4 &&
		ABS(z-v.z) < EPSILON_E4 &&
		ABS(w-v.w) < EPSILON_E4 );

}

uint32 Vector3W::GetDWARGB()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);
	
	return (unsigned int)(((((A)&0xff)<<24)|(((R)&0xff)<<16)|(((G)&0xff)<<8)|((B)&0xff)));
}

uint32 Vector3W::GetDWRGBA()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);
	return (unsigned int)(((((R)&0xff)<<24)|(((G)&0xff)<<16)|(((B)&0xff)<<8)|((A)&0xff)));
}

uint32 Vector3W::GetDWBGRA()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);
	return ((unsigned int)((((B)&0xff)<<24)|(((G)&0xff)<<16)|(((R)&0xff)<<8)|((A)&0xff)));
}

uint32 Vector3W::GetDWABGR()const
{
	unsigned char R,G,B,A;
	GetUCColor(A,R,G,B);
	return ((unsigned int)((((A)&0xff)<<24)|(((B)&0xff)<<16)|(((G)&0xff)<<8)|((R)&0xff)));
}

void Vector3W::GetUCColor(unsigned char &A,unsigned char &R,unsigned char &G,unsigned char &B)const
{
	A = (unsigned char)(a * 255);
	R = (unsigned char)(r * 255);
	G = (unsigned char)(g * 255);
	B = (unsigned char)(b * 255);
	
}

}