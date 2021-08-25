#pragma once


#include "core/system.h"
#include "core/memorymanager.h"

#include <math.h>  
#include <emmintrin.h>

#ifndef VS_SSE
//#define VS_SSE
#endif


#ifdef USE_DLL

#ifdef MATH_EXPORTS
	#define  MATH_API __declspec(dllexport) 
#else
	#define  MATH_API __declspec(dllimport)
#endif
#else
#define  MATH_API
#endif



namespace zq{


#define IABS(x)		abs(x)
#define LABS(x)		labs(x)
#define LLABS(x)	llabs(x)
#define SIN(x)		sinf(x)
#define COS(x)		cosf(x)
#define TAN(x)		tanf(x)
#define ASIN(x)		asinf(x)
#define ACOS(x)		acosf(x)
#define ATAN(x)		atanf(x)
#define ATAN2(y,x)	atan2f(y,x)
#define SQRT(x)		sqrtf(x)
#define POW(x,y)	powf(x,y)
#define CEIL(x)		ceilf(x)
#define EXP(x)		expf(x)
#define EXP2(x)		POW(2.0f,x)
#define FMOD(x,y)	fmodf(x,y)
#define	FLOOR(x)	floorf(x)
#define	LOG(x)		logf(x)
#define	LOG2(x)		LOG(x) * 1.4426950f
#define FABS(x)		fabsf(x)

#define EPSILON_E3 (float)(1E-3) 
#define EPSILON_E4 (float)(1E-4) 
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)
#define EPSILON_E8 (float)(1E-8)

#define MATH_FRONT			0
#define MATH_BACK			1
#define MATH_VSON			2

#define MATH_INTERSECT		3
#define MATH_OUT			4
#define MATH_IN			    5

#define MATH_NOINTERSECT	6

#define MATH_MAX_FLOAT		3.402823466e+38F
#define MATH_MIN_FLOAT		1.175494351e-38F
#define MATH_MAX_UINT32	    0xffffffff
#define MATH_FLOAT_1		0.99999f		
#define MATH_PI			    3.1415926535897932f
#define MATH_PI2			1.57079632679f
#define MATH_2PI			6.2831853071795864f
#define MATH_INV_PI		    0.31830988618f
#define MATH_FASTASIN_VSPI2	1.5707963050f



#define MATH_USHORT_INDEX	uint16
#define MATH_UINT_INDEX     uint32

inline float ABS(float t)
{
	return FABS(t);
}

inline int32 ABS(int32 t)
{
	return IABS(t);
}

inline long ABS(long t)
{
	return LABS(t);
}

inline long long ABS(long long t)
{
	return LLABS(t);
}

inline int32 VSstoi(const char* p)
{
	return atoi(p);
}

inline float VSstof(const char* p)
{
	return atof(p);
}

inline float FastAtan2(float Y, float X)
{
	//return atan2f(Y,X);
	// atan2f occasionally returns NaN with perfectly valid input (possibly due to a compiler or library bug).
	// We are replacing it with a minimax approximation with a max relative error of 7.15255737e-007 compared to the C library function.
	// On PC this has been measured to be 2x faster than the std C version.

	const float absX = FABS(X);
	const float absY = FABS(Y);
	const bool yAbsBigger = (absY > absX);
	float t0 = yAbsBigger ? absY : absX; // Max(absY, absX)
	float t1 = yAbsBigger ? absX : absY; // Min(absX, absY)

	if (t0 == 0.f)
		return 0.f;

	float t3 = t1 / t0;
	float t4 = t3 * t3;

	static const float c[7] = {
		+7.2128853633444123e-03f,
		-3.5059680836411644e-02f,
		+8.1675882859940430e-02f,
		-1.3374657325451267e-01f,
		+1.9856563505717162e-01f,
		-3.3324998579202170e-01f,
		+1.0f
	};

	t0 = c[0];
	t0 = t0 * t4 + c[1];
	t0 = t0 * t4 + c[2];
	t0 = t0 * t4 + c[3];
	t0 = t0 * t4 + c[4];
	t0 = t0 * t4 + c[5];
	t0 = t0 * t4 + c[6];
	t3 = t0 * t3;

	t3 = yAbsBigger ? (0.5f * MATH_PI) - t3 : t3;
	t3 = (X < 0.0f) ? MATH_PI - t3 : t3;
	t3 = (Y < 0.0f) ? -t3 : t3;

	return t3;
}

inline float Fmod(float X, float Y)
{
	if (FABS(Y) <= EPSILON_E8)
	{
		return 0.f;
	}
	const float Div = (X / Y);
	// All floats where abs(f) >= 2^23 (8388608) are whole numbers so do not need truncation, and avoid overflow in TruncToFloat as they get even larger.
	const float Quotient = FABS(Div) < 8388608.f ? int(Div) : Div;
	float IntPortion = Y * Quotient;

	// Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
	// For example Fmod(55.8, 9.3) would result in a very small negative value!
	if (FABS(IntPortion) > FABS(X))
	{
		IntPortion = X;
	}

	const float Result = X - IntPortion;
	return Result;
}

/** Return true if value is NaN (not a number). */
inline bool IsNaN(float A)
{
	return ((*(unsigned int*)&A) & 0x7FFFFFFF) > 0x7F800000;
}

/** Return true if value is finite (not NaN and not Infinity). */
inline bool IsFinite(float A)
{
	return ((*(unsigned int*)&A) & 0x7F800000) != 0x7F800000;
}

inline bool IsNegativeFloat(const float& A)
{
	return ((*(unsigned int*)&A) >= (unsigned int)0x80000000); // Detects sign bit.
}

inline void SinCos(float* ScalarSin, float* ScalarCos, float  Value)
{
	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float quotient = (MATH_INV_PI*0.5f)*Value;
	if (Value >= 0.0f)
	{
		quotient = (float)((int)(quotient + 0.5f));
	}
	else
	{
		quotient = (float)((int)(quotient - 0.5f));
	}
	float y = Value - (2.0f*MATH_PI)*quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
	float sign;
	if (y > MATH_PI2)
	{
		y = MATH_PI - y;
		sign = -1.0f;
	}
	else if (y < -MATH_PI2)
	{
		y = -MATH_PI - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	float y2 = y * y;

	// 11-degree minimax approximation
	*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	// 10-degree minimax approximation
	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*ScalarCos = sign * p;
}


// Note:  We use MATH_FASTASIN_VSPI2 instead of HALF_PI inside of FastASin(), since it was the value that accompanied the minimax coefficients below.
// It is important to use exactly the same value in all places inside this function to ensure that FastASin(0.0f) == 0.0f.
// For comparison:
//		MATH_PI2				== 1.57079632679f == 0x3fC90FDB
//		VSHALF_PI3			== 1.5707963050f  == 0x3fC90FDA

/**
* Computes the ASin of a scalar value.
*
* @param Value  input angle
* @return ASin of Value
*/
inline float FastAsin(float Value)
{
	// Clamp input to [-1,1].
	bool nonnegative = (Value >= 0.0f);
	float x = ABS(Value);
	float omx = 1.0f - x;
	if (omx < 0.0f)
	{
		omx = 0.0f;
	}
	float root = SQRT(omx);
	// 7-degree minimax approximation
	float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + MATH_FASTASIN_VSPI2;
	result *= root;  // acos(|x|)
	// acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
	return (nonnegative ? MATH_FASTASIN_VSPI2 - result : result - MATH_FASTASIN_VSPI2);
}



class MATH_API VSMathInstance
{
public:
	VSMathInstance();
	~VSMathInstance();
	float GetFastSin(uint32 i);
	float GetFastCos(uint32 i);
	uint32 CRC32Compute(const void *pData, uint32 uiDataSize);
	static VSMathInstance & GetMathInstance()
	{
		static VSMathInstance g_MathInitial;
		return g_MathInitial;
	}
protected:
	void VSInitCRCTable();
	float FastSin[361];
	float FastCos[361];
	uint32 CRCTable[256];
};

inline uint32 CalcAlign(uint32 n, uint32 align)
{
	return ((n + align - 1) & (~(align - 1)));
}

inline float RadianToAngle(float Radian)
{
	return 57.29578f * Radian;
	//return ( Radian * 180.0f ) / MATH_PI ;
}

inline float AngleToRadian(float Angle)
{
	//return ( Angle * MATH_PI ) /  180.0f;
	return 0.01745329f * Angle;
}

inline bool IsTwoPower(uint32 uiN)
{
	return !(uiN & (uiN - 1));
}

inline unsigned short FloatToHalf(float Value)
{
	uint32 Result;

	uint32 IValue = ((uint32 *)(&Value))[0];
	uint32 Sign = (IValue & 0x80000000U) >> 16U;
	IValue = IValue & 0x7FFFFFFFU;      // Hack off the sign

	if (IValue > 0x47FFEFFFU)
	{
		// The number is too large to be represented as a half.  Saturate to infinity.
		Result = 0x7FFFU;
	}
	else
	{
		if (IValue < 0x38800000U)
		{
			// The number is too small to be represented as a normalized half.
			// Convert it to a denormalized value.
			uint32 Shift = 113U - (IValue >> 23U);
			IValue = (0x800000U | (IValue & 0x7FFFFFU)) >> Shift;
		}
		else
		{
			// Rebias the exponent to represent the value as a normalized half.
			IValue += 0xC8000000U;
		}

		Result = ((IValue + 0x0FFFU + ((IValue >> 13U) & 1U)) >> 13U)&0x7FFFU; 
	}

	return (unsigned short)(Result|Sign);
}

inline float HalfToFloat(unsigned short Value)
{
	uint32 Mantissa;
	uint32 Exponent;
	uint32 Result;

	Mantissa = (unsigned int)(Value & 0x03FF);

	if ((Value & 0x7C00) != 0)  // The value is normalized
	{
		Exponent = (unsigned int)((Value >> 10) & 0x1F);
	}
	else if (Mantissa != 0)     // The value is denormalized
	{
		// Normalize the value in the resulting float
		Exponent = 1;

		do
		{
			Exponent--;
			Mantissa <<= 1;
		} while ((Mantissa & 0x0400) == 0);

		Mantissa &= 0x03FF;
	}
	else                        // The value is zero
	{
		Exponent = (unsigned int)-112;
	}

	Result = ((Value & 0x8000) << 16) | // Sign
		((Exponent + 112) << 23) | // Exponent
		(Mantissa << 13);          // Mantissa

	return *(float*)&Result;
}

//f must [0,1]
inline uint32 CompressUnitFloat(float f, uint32 Bit = 16)
{
	uint32 nIntervals = 1 << Bit;
	float scaled = f * (nIntervals - 1);
	uint32 rounded = (unsigned int)(scaled + 0.5f);
	if (rounded > nIntervals - 1)
	{
		rounded = nIntervals - 1;
	}
	return rounded;
	
}

inline uint32 CompressFloat(float f, float Max , float Min ,uint32 Bit = 16)
{
	float Unitf = (f - Min)/(Max - Min);
	return CompressUnitFloat(Unitf,Bit);
}

inline float DecompressUnitFloat(uint32 quantized,uint32 Bit = 16)
{
	uint32 nIntervals = 1 << Bit;
	float IntervalSize = 1.0f / (nIntervals - 1);
	
	return quantized * IntervalSize;
}

inline float DecompressFloat(uint32 quantized,float Max , float Min ,uint32 Bit = 16)
{
	float Unitf = DecompressUnitFloat(quantized,Bit);
	return (Min + Unitf * (Max - Min));
}

inline  float ClampAxis(float Angle)
{
	// returns Angle in the range (-360,360)
	Angle = FMOD(Angle, 360.f);

	if (Angle < 0.f)
	{
		// shift to [0,360) range
		Angle += 360.f;
	}

	return Angle;
}

inline float NormalizeAxis(float Angle)
{
	// returns Angle in the range [0,360)
	Angle = ClampAxis(Angle);

	if (Angle > 180.f)
	{
		// shift to (-180,180]
		Angle -= 360.f;
	}

	return Angle;
}

inline int32 FastLog2(int32 x)
{
	float fx;
	uint32 ix, exp;
	fx = (float)x;
	ix = *(unsigned int*)&fx;
	exp = (ix >> 23) & 0xFF;
	return exp - 127;
}

//-360.....360 can use
inline float ClampAngle(float AngleDegrees, float MinAngleDegrees, float MaxAngleDegrees)
{
	float MaxDelta = ClampAxis(MaxAngleDegrees - MinAngleDegrees) * 0.5f;			// 0..180
	float RangeCenter = ClampAxis(MinAngleDegrees + MaxDelta);						// 0..360
	float DeltaFromCenter = NormalizeAxis(AngleDegrees - RangeCenter);				// -180..180

	// maybe clamp to nearest edge
	if (DeltaFromCenter > MaxDelta)
	{
		return NormalizeAxis(RangeCenter + MaxDelta);
	}
	else if (DeltaFromCenter < -MaxDelta)
	{
		return NormalizeAxis(RangeCenter - MaxDelta);
	}

	// already in range, just return it
	return NormalizeAxis(AngleDegrees);
}

inline int32 VSRand() { return rand(); }
inline void VSRandInit(int32 Seed) { srand(Seed); }
inline float VSFRand() { return VSRand() / (float)RAND_MAX; }

inline uint32 HashCombine(uint32 A, uint32 C)
{
	uint32 B = 0x9e3779b9;
	A += B;

	A -= B; A -= C; A ^= (C >> 13);
	B -= C; B -= A; B ^= (A << 8);
	C -= A; C -= B; C ^= (B >> 13);
	A -= B; A -= C; A ^= (C >> 12);
	B -= C; B -= A; B ^= (A << 16);
	C -= A; C -= B; C ^= (B >> 5);
	A -= B; A -= C; A ^= (C >> 3);
	B -= C; B -= A; B ^= (A << 10);
	C -= A; C -= B; C ^= (B >> 15);

	return C;
}

inline uint32 PointerHash(const void* Key, uint32 C = 0)
{
	uint64 PtrInt = 0;
#if PLATFORM_WIN
	#if _WIN64
		PtrInt = reinterpret_cast<uint64>(Key) >> 4;
	
	#else
		PtrInt = unsigned int(Key);
	#endif
#else
	VSMAC_ASSERT(0);
#endif
	return HashCombine((unsigned int)PtrInt, C);
}

inline uint32 GetTypeHash(const unsigned char A)
{
	return A;
}

inline uint32 GetTypeHash(const char A)
{
	return A;
}

inline uint32 GetTypeHash(const unsigned short A)
{
	return A;
}

inline uint32 GetTypeHash(const short A)
{
	return A;
}

inline uint32 GetTypeHash(const int32 A)
{
	return A;
}

inline uint32 GetTypeHash(const uint32 A)
{
	return A;
}

inline uint32 GetTypeHash(const unsigned long long A)
{
	return (unsigned int)A + ((unsigned int)(A >> 32) * 23);
}

inline uint32 GetTypeHash(const long long A)
{
	return (unsigned int)A + ((unsigned int)(A >> 32) * 23);
}

inline uint32 GetTypeHash(float A)
{
	return *(uint32 *)&A;
}

inline uint32 GetTypeHash(double A)
{
	return GetTypeHash(*(const unsigned long long *)&A);
}

inline uint32 GetTypeHash(const void * A)
{
	return PointerHash(A);
}

inline uint32 GetTypeHash(void * A)
{
	return PointerHash(A);
}

bool MATH_API VSInitMath();

inline uint32 GetTypeHash(const char* A)
{
	return VSMathInstance::GetMathInstance().CRC32Compute(A,VSStrLen(A));
}

inline uint32 Rounding(float fNum)
{
	return unsigned int(fNum + 0.5f);
}

inline uint32 FloorLog2(uint32 Value)
{
	uint32 pos = 0;
	if (Value >= 1 << 16) { Value >>= 16; pos += 16; }
	if (Value >= 1 << 8) { Value >>= 8; pos += 8; }
	if (Value >= 1 << 4) { Value >>= 4; pos += 4; }
	if (Value >= 1 << 2) { Value >>= 2; pos += 2; }
	if (Value >= 1 << 1) { pos += 1; }
	return (Value == 0) ? 0 : pos;
}

inline uint32 CountLeadingZeros(uint32 Value)
{
	if (Value == 0) return 32;
	return 31 - FloorLog2(Value);
}

inline uint32 CeilLogTwo(uint32 Arg)
{
	uint32 Bitmask = ((unsigned int)(CountLeadingZeros(Arg) << 26)) >> 31;
	return (32 - CountLeadingZeros(Arg - 1)) & (~Bitmask);
}









typedef __m128	VectorRegister;
typedef __m128i VectorRegisterInt;
/**
* @param A0	Selects which element (0-3) from 'A' into 1st slot in the result
* @param A1	Selects which element (0-3) from 'A' into 2nd slot in the result
* @param B2	Selects which element (0-3) from 'B' into 3rd slot in the result
* @param B3	Selects which element (0-3) from 'B' into 4th slot in the result
*/
#define SHUFFLEMASK(A0,A1,B2,B3) ( (A0) | ((A1)<<2) | ((B2)<<4) | ((B3)<<6) )

/**
* Returns a bitwise equivalent vector based on 4 DWORDs.
*
* @param X		1st uint32 component
* @param Y		2nd uint32 component
* @param Z		3rd uint32 component
* @param W		4th uint32 component
* @return		Bitwise equivalent vector with 4 floats
*/
inline VectorRegister MakeVectorRegister(uint32 X, uint32 Y, uint32 Z, uint32 W)
{
    union { VectorRegister v; VectorRegisterInt i; } Tmp;
    Tmp.i = _mm_setr_epi32(X, Y, Z, W);
    return Tmp.v;
}

/**
* Returns a vector based on 4 FLOATs.
*
* @param X		1st float component
* @param Y		2nd float component
* @param Z		3rd float component
* @param W		4th float component
* @return		Vector of the 4 FLOATs
*/

inline VectorRegister MakeVectorRegister(float X, float Y, float Z, float W)
{
    return _mm_setr_ps(X, Y, Z, W);
}

/**
* Returns a vector based on 4 int.
*
* @param X		1st int32 component
* @param Y		2nd int32 component
* @param Z		3rd int32 component
* @param W		4th int32 component
* @return		Vector of the 4 int
*/
inline VectorRegisterInt MakeVectorRegisterInt(int32 X, int32 Y, int32 Z, int32 W)
{
    return _mm_setr_epi32(X, Y, Z, W);
}

/*=============================================================================
*	Intrinsics:
*============================================================================*/
/**
* Returns a vector with all zeros.
*
* @return		VectorRegister(0.0f, 0.0f, 0.0f, 0.0f)
*/
inline VectorRegister VectorZero(void)
{
    return _mm_setzero_ps();
}
/**
* Returns an component from a vector.
*
* @param Vec				Vector register
* @param ComponentIndex	Which component to get, X=0, Y=1, Z=2, W=3
* @return					The component as a float
*/
inline float VectorGetComponent(VectorRegister Vec, uint32 ComponentIndex)
{
    return (((float*)&(Vec))[ComponentIndex]);
}
/**
* Returns X + Y + Z.
*
* @param Vec				Vector register
* @return					The component as a float
*/
inline float VectorGetXYZSum(VectorRegister Vec)
{
    return (((float*)&(Vec))[0]) + (((float*)&(Vec))[1]) + (((float*)&(Vec))[2]);
}
/**
* Loads 4 FLOATs from unaligned memory.
*
* @param Ptr	Unaligned memory pointer to the 4 FLOATs
* @return		VectorRegister(Ptr[0], Ptr[1], Ptr[2], Ptr[3])
*/
inline VectorRegister VectorLoad(const void* Ptr)
{
    return _mm_loadu_ps((float*)(Ptr));
}
/**
* Loads 3 FLOATs from unaligned memory and leaves W undefined.
*
* @param Ptr	Unaligned memory pointer to the 3 FLOATs
* @return		VectorRegister(Ptr[0], Ptr[1], Ptr[2], undefined)
*/
#define VectorLoadFloat3( Ptr )			MakeVectorRegister( ((const float*)(Ptr))[0], ((const float*)(Ptr))[1], ((const float*)(Ptr))[2], 0.0f )

/**
* Loads 3 FLOATs from unaligned memory and sets W=0.
*
* @param Ptr	Unaligned memory pointer to the 3 FLOATs
* @return		VectorRegister(Ptr[0], Ptr[1], Ptr[2], 0.0f)
*/
#define VectorLoadFloat3_W0( Ptr )		MakeVectorRegister( ((const float*)(Ptr))[0], ((const float*)(Ptr))[1], ((const float*)(Ptr))[2], 0.0f )

/**
* Loads 3 FLOATs from unaligned memory and sets W=1.
*
* @param Ptr	Unaligned memory pointer to the 3 FLOATs
* @return		VectorRegister(Ptr[0], Ptr[1], Ptr[2], 1.0f)
*/
#define VectorLoadFloat3_W1( Ptr )		MakeVectorRegister( ((const float*)(Ptr))[0], ((const float*)(Ptr))[1], ((const float*)(Ptr))[2], 1.0f )

/**
* Loads 4 FLOATs from aligned memory.
*
* @param Ptr	Aligned memory pointer to the 4 FLOATs
* @return		VectorRegister(Ptr[0], Ptr[1], Ptr[2], Ptr[3])
*/
#define VectorLoadAligned( Ptr )		_mm_load_ps( (const float*)(Ptr) )

/**
* Loads 1 float from unaligned memory and replicates it to all 4 elements.
*
* @param Ptr	Unaligned memory pointer to the float
* @return		VectorRegister(Ptr[0], Ptr[0], Ptr[0], Ptr[0])
*/
#define VectorLoadFloat1( Ptr )			_mm_load1_ps( (const float*)(Ptr) )

/**
* Loads 2 floats from unaligned memory into X and Y and duplicates them in Z and W.
*
* @param Ptr	Unaligned memory pointer to the floats
* @return		VectorRegister(Ptr[0], Ptr[1], Ptr[0], Ptr[1])
*/
#define VectorLoadFloat2( Ptr )			_mm_castpd_ps(_mm_load1_pd((const double*)(Ptr)))

/**
* Creates a vector out of three FLOATs and leaves W undefined.
*
* @param X		1st float component
* @param Y		2nd float component
* @param Z		3rd float component
* @return		VectorRegister(X, Y, Z, undefined)
*/
#define VectorSetFloat3( X, Y, Z )		MakeVectorRegister( X, Y, Z, 0.0f )

/**
* Propagates passed in float to all registers
*
* @param F		Float to set
* @return		VectorRegister(F,F,F,F)
*/
#define VectorSetFloat1( F )					_mm_set1_ps( F )
/**
* Stores a vector to memory (aligned or unaligned).
*
* @param Vec	Vector to store
* @param Ptr	Memory pointer
*/
#define VectorStore( Vec, Ptr )					_mm_storeu_ps((float*)(Ptr), Vec)
/**
* Performs non-temporal store of a vector to aligned memory without polluting the caches
*
* @param Vec	Vector to store
* @param Ptr	Aligned memory pointer
*/
#define VectorStoreAlignedStreamed( Vec, Ptr )	_mm_stream_ps( (float*)(Ptr), Vec )
/**
* Stores a vector to aligned memory.
*
* @param Vec	Vector to store
* @param Ptr	Aligned memory pointer
*/
#define VectorStoreAligned( Vec, Ptr )			_mm_store_ps( (float*)(Ptr), Vec )
/**
* Stores the XYZ components of a vector to unaligned memory.
*
* @param Vec	Vector to store XYZ
* @param Ptr	Unaligned memory pointer
*/
inline void VectorStoreFloat3(const VectorRegister& Vec, void* Ptr)
{
    union { VectorRegister v; float f[4]; } Tmp;
    Tmp.v = Vec;
    float* FloatPtr = (float*)(Ptr);
    FloatPtr[0] = Tmp.f[0];
    FloatPtr[1] = Tmp.f[1];
    FloatPtr[2] = Tmp.f[2];
}
/**
* Stores the X component of a vector to unaligned memory.
*
* @param Vec	Vector to store X
* @param Ptr	Unaligned memory pointer
*/
#define VectorStoreFloat1( Vec, Ptr )			_mm_store_ss((float*)(Ptr), Vec)
/**
* Returns the absolute value (component-wise).
*
* @param Vec			Source vector
* @return				VectorRegister( abs(Vec.x), abs(Vec.y), abs(Vec.z), abs(Vec.w) )
*/
#define VectorAbs( Vec )						_mm_and_ps(Vec, VectorRegisterConstants::SignMask)
/**
* Returns the negated value (component-wise).
*
* @param Vec			Source vector
* @return				VectorRegister( -Vec.x, -Vec.y, -Vec.z, -Vec.w )
*/
#define VectorNegate( Vec )						_mm_sub_ps(_mm_setzero_ps(),Vec)
/**
* Replicates one element into all four elements and returns the new vector.
*
* @param Vec			Source vector
* @param ElementIndex	Index (0-3) of the element to replicate
* @return				VectorRegister( Vec[ElementIndex], Vec[ElementIndex], Vec[ElementIndex], Vec[ElementIndex] )
*/
#define VectorReplicate( Vec, ElementIndex )	_mm_shuffle_ps( Vec, Vec, SHUFFLEMASK(ElementIndex,ElementIndex,ElementIndex,ElementIndex) )
/**
* Adds two vectors (component-wise) and returns the result.
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( Vec1.x+Vec2.x, Vec1.y+Vec2.y, Vec1.z+Vec2.z, Vec1.w+Vec2.w )
*/
#define VectorAdd(Vec1, Vec2)				 _mm_add_ps(Vec1, Vec2)
/**
* Subtracts a vector from another (component-wise) and returns the result.
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( Vec1.x-Vec2.x, Vec1.y-Vec2.y, Vec1.z-Vec2.z, Vec1.w-Vec2.w )
*/
#define VectorSubtract(Vec1, Vec2)			_mm_sub_ps(Vec1, Vec2)
/**
* Multiplies two vectors (component-wise) and returns the result.
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( Vec1.x*Vec2.x, Vec1.y*Vec2.y, Vec1.z*Vec2.z, Vec1.w*Vec2.w )
*/
#define VectorMultiply(Vec1, Vec2)			_mm_mul_ps(Vec1, Vec2)
/**
* Divides two vectors (component-wise) and returns the result.
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( Vec1.x/Vec2.x, Vec1.y/Vec2.y, Vec1.z/Vec2.z, Vec1.w/Vec2.w )
*/
#define VectorDivide(Vec1, Vec2)			_mm_div_ps(Vec1, Vec2)
/**
* Multiplies two vectors (component-wise), adds in the third vector and returns the result.
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @param Vec3	3rd vector
* @return		VectorRegister( Vec1.x*Vec2.x + Vec3.x, Vec1.y*Vec2.y + Vec3.y, Vec1.z*Vec2.z + Vec3.z, Vec1.w*Vec2.w + Vec3.w )
*/
#define VectorMultiplyAdd( Vec1, Vec2, Vec3 )	_mm_add_ps( _mm_mul_ps(Vec1, Vec2), Vec3 )
/**
* Creates a four-part mask based on component-wise == compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( Vec1.x == Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareEQ( Vec1, Vec2 )			_mm_cmpeq_ps( Vec1, Vec2 )

/**
* Creates a four-part mask based on component-wise != compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( Vec1.x != Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareNE( Vec1, Vec2 )			_mm_cmpneq_ps( Vec1, Vec2 )

/**
* Creates a four-part mask based on component-wise > compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( Vec1.x > Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareGT( Vec1, Vec2 )			_mm_cmpgt_ps( Vec1, Vec2 )

/**
* Creates a four-part mask based on component-wise >= compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( Vec1.x >= Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareGE( Vec1, Vec2 )			_mm_cmpge_ps( Vec1, Vec2 )

/**
* Creates a four-part mask based on component-wise < compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( Vec1.x < Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareLT( Vec1, Vec2 )			_mm_cmplt_ps( Vec1, Vec2 )

/**
* Creates a four-part mask based on component-wise <= compares of the input vectors
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( Vec1.x <= Vec2.x ? 0xFFFFFFFF : 0, same for yzw )
*/
#define VectorCompareLE( Vec1, Vec2 )			_mm_cmple_ps( Vec1, Vec2 )

/**
* Does a bitwise vector selection based on a mask (e.g., created from VectorCompareXX)
*
* @param Mask  Mask (when 1: use the corresponding bit from Vec1 otherwise from Vec2)
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( for each bit i: Mask[i] ? Vec1[i] : Vec2[i] )
*
*/
#define VectorSelect(Mask, Vec1,Vec2) _mm_xor_ps(Vec2, _mm_and_ps(Mask, _mm_xor_ps(Vec1, Vec2)))

/**
* Combines two vectors using bitwise OR (treating each vector as a 128 bit field)
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( for each bit i: Vec1[i] | Vec2[i] )
*/
#define VectorBitwiseOr(Vec1, Vec2)	_mm_or_ps(Vec1, Vec2)

/**
* Combines two vectors using bitwise AND (treating each vector as a 128 bit field)
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( for each bit i: Vec1[i] & Vec2[i] )
*/
#define VectorBitwiseAnd(Vec1, Vec2) _mm_and_ps(Vec1, Vec2)

/**
* Combines two vectors using bitwise XOR (treating each vector as a 128 bit field)
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( for each bit i: Vec1[i] ^ Vec2[i] )
*/
#define VectorBitwiseXor(Vec1, Vec2) _mm_xor_ps(Vec1, Vec2)

/**
* Returns an estimate of 1/SQRT(c) for each component of the vector
*
* @param Vector		Vector
* @return			VectorRegister(1/SQRT(t), 1/SQRT(t), 1/SQRT(t), 1/SQRT(t))
*/
#define VectorReciprocalSqrt(Vec)		_mm_rsqrt_ps( Vec )

/**
* Computes an estimate of the reciprocal of a vector (component-wise) and returns the result.
*
* @param Vec	1st vector
* @return		VectorRegister( (Estimate) 1.0f / Vec.x, (Estimate) 1.0f / Vec.y, (Estimate) 1.0f / Vec.z, (Estimate) 1.0f / Vec.w )
*/
#define VectorReciprocal(Vec)			_mm_rcp_ps(Vec)
/**
* Loads XYZ and sets W=0
*
* @param Vector	VectorRegister
* @return		VectorRegister(X, Y, Z, 0.0f)
*/
#define VectorSet_W0( Vec )		_mm_and_ps( Vec, VectorRegisterConstants::XYZMask )
/**
* Returns the minimum values of two vectors (component-wise).
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( min(Vec1.x,Vec2.x), min(Vec1.y,Vec2.y), min(Vec1.z,Vec2.z), min(Vec1.w,Vec2.w) )
*/
#define VectorMin( Vec1, Vec2 )			_mm_min_ps( Vec1, Vec2 )

/**
* Returns the maximum values of two vectors (component-wise).
*
* @param Vec1	1st vector
* @param Vec2	2nd vector
* @return		VectorRegister( max(Vec1.x,Vec2.x), max(Vec1.y,Vec2.y), max(Vec1.z,Vec2.z), max(Vec1.w,Vec2.w) )
*/
#define VectorMax( Vec1, Vec2 )			_mm_max_ps( Vec1, Vec2 )

/**
* Swizzles the 4 components of a vector and returns the result.
*
* @param Vec		Source vector
* @param X			Index for which component to use for X (literal 0-3)
* @param Y			Index for which component to use for Y (literal 0-3)
* @param Z			Index for which component to use for Z (literal 0-3)
* @param W			Index for which component to use for W (literal 0-3)
* @return			The swizzled vector
*/
#define VectorSwizzle( Vec, X, Y, Z, W )	_mm_shuffle_ps( Vec, Vec, SHUFFLEMASK(X,Y,Z,W) )

/**
* Creates a vector through selecting two components from each vector via a shuffle mask.
*
* @param Vec1		Source vector1
* @param Vec2		Source vector2
* @param X			Index for which component of Vector1 to use for X (literal 0-3)
* @param Y			Index for which component of Vector1 to use for Y (literal 0-3)
* @param Z			Index for which component of Vector2 to use for Z (literal 0-3)
* @param W			Index for which component of Vector2 to use for W (literal 0-3)
* @return			The swizzled vector
*/
#define VectorShuffle( Vec1, Vec2, X, Y, Z, W )	_mm_shuffle_ps( Vec1, Vec2, SHUFFLEMASK(X,Y,Z,W) )
/**
* Creates a vector by combining two high components from each vector
*
* @param Vec1		Source vector1
* @param Vec2		Source vector2
* @return			The combined vector
*/
#define VectorCombineHigh(Vec1,Vec2) VectorShuffle(Vec1, Vec2, 2, 3, 2, 3)


/**
* Creates a vector by combining two low components from each vector
*
* @param Vec1		Source vector1
* @param Vec2		Source vector2
* @return			The combined vector
*/
#define VectorCombineLow(Vec1,Vec2) VectorShuffle(Vec1, Vec2, 0, 1, 0, 1)

/**
* These functions return a vector mask to indicate which components pass the comparison.
* Each component is 0xffffffff if it passes, 0x00000000 if it fails.
*
* @param Vec1			1st source vector
* @param Vec2			2nd source vector
* @return				Vector with a mask for each component.
*/
#define VectorMask_LT( Vec1, Vec2 )			_mm_cmplt_ps(Vec1, Vec2)
#define VectorMask_LE( Vec1, Vec2 )			_mm_cmple_ps(Vec1, Vec2)
#define VectorMask_GT( Vec1, Vec2 )			_mm_cmpgt_ps(Vec1, Vec2)
#define VectorMask_GE( Vec1, Vec2 )			_mm_cmpge_ps(Vec1, Vec2)
#define VectorMask_EQ( Vec1, Vec2 )			_mm_cmpeq_ps(Vec1, Vec2)
#define VectorMask_NE( Vec1, Vec2 )			_mm_cmpneq_ps(Vec1, Vec2)
/**
* Returns non-zero if any element in Vec1 is greater than the corresponding element in Vec2, otherwise 0.
*
* @param Vec1			1st source vector
* @param Vec2			2nd source vector
* @return				Non-zero integer if (Vec1.x > Vec2.x) || (Vec1.y > Vec2.y) || (Vec1.z > Vec2.z) || (Vec1.w > Vec2.w)
*/
#define VectorAnyGreaterThan( Vec1, Vec2 )		_mm_movemask_ps( _mm_cmpgt_ps(Vec1, Vec2) )
/**
* Returns an integer bit-mask (0x00 - 0x0f) based on the sign-bit for each component in a vector.
*
* @param VecMask		Vector
* @return				Bit 0 = sign(VecMask.x), Bit 1 = sign(VecMask.y), Bit 2 = sign(VecMask.z), Bit 3 = sign(VecMask.w)
*/
#define VectorMaskBits( VecMask )			_mm_movemask_ps( VecMask )

inline float InvSqrt(float F)
{
    // Performs two passes of Newton-Raphson iteration on the hardware estimate
    //    v^-0.5 = x
    // => x^2 = v^-1
    // => 1/(x^2) = v
    // => F(x) = x^-2 - v
    //    F'(x) = -2x^-3

    //    x1 = x0 - F(x0)/F'(x0)
    // => x1 = x0 + 0.5 * (x0^-2 - Vec) * x0^3
    // => x1 = x0 + 0.5 * (x0 - Vec * x0^3)
    // => x1 = x0 + x0 * (0.5 - 0.5 * Vec * x0^2)
    //
    // This final form has one more operation than the legacy factorization (X1 = 0.5*X0*(3-(Y*X0)*X0)
    // but retains better accuracy (namely InvSqrt(1) = 1 exactly).

    const __m128 fOneHalf = _mm_set_ss(0.5f);
    __m128 Y0, X0, X1, X2, FOver2;
    float temp;

    Y0 = _mm_set_ss(F);
    X0 = _mm_rsqrt_ss(Y0);	// 1/sqrt estimate (12 bits)
    FOver2 = _mm_mul_ss(Y0, fOneHalf);

    // 1st Newton-Raphson iteration
    X1 = _mm_mul_ss(X0, X0);
    X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
    X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

    // 2nd Newton-Raphson iteration
    X2 = _mm_mul_ss(X1, X1);
    X2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X2));
    X2 = _mm_add_ss(X1, _mm_mul_ss(X1, X2));

    _mm_store_ss(&temp, X2);
    return temp;
}



namespace VectorRegisterConstants {


    static const VectorRegister FloatOne = MakeVectorRegister(1.0f, 1.0f, 1.0f, 1.0f);
    static const VectorRegister FloatZero = MakeVectorRegister(0.0f, 0.0f, 0.0f, 0.0f);
    static const VectorRegister FloatMinusOne = MakeVectorRegister(-1.0f, -1.0f, -1.0f, -1.0f);
    static const VectorRegister Float0001 = MakeVectorRegister(0.0f, 0.0f, 0.0f, 1.0f);
    static const VectorRegister SmallLengthThreshold = MakeVectorRegister(1.e-8f, 1.e-8f, 1.e-8f, 1.e-8f);
    static const VectorRegister FloatOneHundredth = MakeVectorRegister(0.01f, 0.01f, 0.01f, 0.01f);
    static const VectorRegister Float111_Minus1 = MakeVectorRegister(1.f, 1.f, 1.f, -1.f);
    static const VectorRegister FloatMinus1_111 = MakeVectorRegister(-1.f, 1.f, 1.f, 1.f);
    static const VectorRegister FloatOneHalf = MakeVectorRegister(0.5f, 0.5f, 0.5f, 0.5f);
    static const VectorRegister FloatMinusOneHalf = MakeVectorRegister(-0.5f, -0.5f, -0.5f, -0.5f);
    static const VectorRegister KindaSmallNumber = MakeVectorRegister(EPSILON_E8, EPSILON_E8, EPSILON_E8, EPSILON_E8);
    static const VectorRegister SmallNumber = MakeVectorRegister(EPSILON_E4, EPSILON_E4, EPSILON_E4, EPSILON_E4);
    static const VectorRegister ThreshQuatNormalized = MakeVectorRegister(0.01f, 0.01f, 0.01f, 0.01f);
    static const VectorRegister BigNumber = MakeVectorRegister(MATH_MAX_FLOAT, MATH_MAX_FLOAT, MATH_MAX_FLOAT, MATH_MAX_FLOAT);

    static const VectorRegisterInt IntOne = MakeVectorRegisterInt(1, 1, 1, 1);
    static const VectorRegisterInt IntZero = MakeVectorRegisterInt(0, 0, 0, 0);
    static const VectorRegisterInt IntMinusOne = MakeVectorRegisterInt(-1, -1, -1, -1);

    /** This is to speed up Quaternion Inverse. Static variable to keep sign of inverse **/
    static const VectorRegister QINV_SIGN_MASK = MakeVectorRegister(-1.f, -1.f, -1.f, 1.f);

    static const VectorRegister QMULTI_SIGN_MASK0 = MakeVectorRegister(1.f, -1.f, 1.f, -1.f);
    static const VectorRegister QMULTI_SIGN_MASK1 = MakeVectorRegister(1.f, 1.f, -1.f, -1.f);
    static const VectorRegister QMULTI_SIGN_MASK2 = MakeVectorRegister(-1.f, 1.f, 1.f, -1.f);

    static const VectorRegister DEG_TO_RAD = MakeVectorRegister(MATH_PI / (180.f), MATH_PI / (180.f), MATH_PI / (180.f), MATH_PI / (180.f));
    static const VectorRegister DEG_TO_RAD_HALF = MakeVectorRegister((MATH_PI / 180.f)*0.5f, (MATH_PI / 180.f)*0.5f, (MATH_PI / 180.f)*0.5f, (MATH_PI / 180.f)*0.5f);
    static const VectorRegister RAD_TO_DEG = MakeVectorRegister((180.f) / MATH_PI, (180.f) / MATH_PI, (180.f) / MATH_PI, (180.f) / MATH_PI);

    /** Bitmask to AND out the XYZ components in a vector */
    static const VectorRegister XYZMask = MakeVectorRegister((unsigned int)0xffffffff, (unsigned int)0xffffffff, (unsigned int)0xffffffff, (unsigned int)0x00000000);

    /** Bitmask to AND out the sign bit of each components in a vector */
#define SIGN_BIT ((1 << 31))
    static const VectorRegister SignBit = MakeVectorRegister((unsigned int)SIGN_BIT, (unsigned int)SIGN_BIT, (unsigned int)SIGN_BIT, (unsigned int)SIGN_BIT);
    static const VectorRegister SignMask = MakeVectorRegister((unsigned int)(~SIGN_BIT), (unsigned int)(~SIGN_BIT), (unsigned int)(~SIGN_BIT), (unsigned int)(~SIGN_BIT));
    static const VectorRegisterInt IntSignBit = MakeVectorRegisterInt(SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT);
    static const VectorRegisterInt IntSignMask = MakeVectorRegisterInt((~SIGN_BIT), (~SIGN_BIT), (~SIGN_BIT), (~SIGN_BIT));
#undef SIGN_BIT
    static const VectorRegister AllMask = MakeVectorRegister(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
    static const VectorRegisterInt IntAllMask = MakeVectorRegisterInt(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);

    /** Vector full of positive infinity */
    static const VectorRegister FloatInfinity = MakeVectorRegister((unsigned int)0x7F800000, (unsigned int)0x7F800000, (unsigned int)0x7F800000, (unsigned int)0x7F800000);


    static const VectorRegister Pi = MakeVectorRegister(MATH_PI, MATH_PI, MATH_PI, MATH_PI);
    static const VectorRegister TwoPi = MakeVectorRegister(2.0f*MATH_PI, 2.0f*MATH_PI, 2.0f*MATH_PI, 2.0f*MATH_PI);
    static const VectorRegister PiByTwo = MakeVectorRegister(0.5f*MATH_PI, 0.5f*MATH_PI, 0.5f*MATH_PI, 0.5f*MATH_PI);
    static const VectorRegister PiByFour = MakeVectorRegister(0.25f*MATH_PI, 0.25f*MATH_PI, 0.25f*MATH_PI, 0.25f*MATH_PI);
    static const VectorRegister OneOverPi = MakeVectorRegister(1.0f / MATH_PI, 1.0f / MATH_PI, 1.0f / MATH_PI, 1.0f / MATH_PI);
    static const VectorRegister OneOverTwoPi = MakeVectorRegister(1.0f / (2.0f*MATH_PI), 1.0f / (2.0f*MATH_PI), 1.0f / (2.0f*MATH_PI), 1.0f / (2.0f*MATH_PI));

    static const VectorRegister Float255 = MakeVectorRegister(255.0f, 255.0f, 255.0f, 255.0f);
    static const VectorRegister Float127 = MakeVectorRegister(127.0f, 127.0f, 127.0f, 127.0f);
    static const VectorRegister FloatNeg127 = MakeVectorRegister(-127.0f, -127.0f, -127.0f, -127.0f);
    static const VectorRegister Float360 = MakeVectorRegister(360.f, 360.f, 360.f, 360.f);
    static const VectorRegister Float180 = MakeVectorRegister(180.f, 180.f, 180.f, 180.f);

    // All float numbers greater than or equal to this have no fractional value.
    static const VectorRegister FloatNonFractional = MakeVectorRegister(8388608.f, 8388608.f, 8388608.f, 8388608.f);

    static const VectorRegister FloatTwo = MakeVectorRegister(2.0f, 2.0f, 2.0f, 2.0f);
    static const uint32 AlmostTwoBits = 0x3fffffff;
    static const VectorRegister FloatAlmostTwo = MakeVectorRegister(*(float*)&AlmostTwoBits, *(float*)&AlmostTwoBits, *(float*)&AlmostTwoBits, *(float*)&AlmostTwoBits);
}



}
