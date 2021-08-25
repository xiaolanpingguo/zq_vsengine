#pragma once


#include "math/maths.h"
#include "math/vector3.h"
#include "math/quat.h"


namespace zq{


class MATH_API Matrix3X3 
{
public:
	union
	{
		float m[9];
		struct
		{
			float _00, _01, _02;
			float _10, _11, _12;
			float _20, _21, _22;
		};
		float M[3][3];
	};

	Matrix3X3(void);
	Matrix3X3(float m00,float m01,float m02,
				float m10,float m11,float m12,
				float m20,float m21,float m22);


    inline void Identity(void)
    {
        memset(m, 0, sizeof(Matrix3X3));
        _00 = _11 = _22 = 1.0f;
    }

    inline void TransposeOf(const Matrix3X3 &Matrix)
    {
        _00 = Matrix._00;
        _10 = Matrix._01;
        _20 = Matrix._02;

        _01 = Matrix._10;
        _11 = Matrix._11;
        _21 = Matrix._12;

        _02 = Matrix._20;
        _12 = Matrix._21;
        _22 = Matrix._22;
    }

    inline float Det()const
    {
        return (M[0][0] * (M[1][1] * M[2][2] - M[2][1] * M[1][2]) -
            M[0][1] * (M[1][0] * M[2][2] - M[2][0] * M[1][2]) +
            M[0][2] * (M[1][0] * M[2][1] - M[2][0] * M[1][1]));

    }
    inline void InverseOf(const Matrix3X3 &Matirx)
    {
        float det = Matirx.Det();

        if (ABS(det) < EPSILON_E4)
            return;

        float det_inv = 1.0f / det;

        M[0][0] = det_inv * (Matirx.M[1][1] * Matirx.M[2][2] - Matirx.M[2][1] * Matirx.M[1][2]);
        M[1][0] = -det_inv * (Matirx.M[1][0] * Matirx.M[2][2] - Matirx.M[2][0] * Matirx.M[1][2]);
        M[2][0] = det_inv * (Matirx.M[1][0] * Matirx.M[2][1] - Matirx.M[2][0] * Matirx.M[1][1]);

        M[0][1] = -det_inv * (Matirx.M[0][1] * Matirx.M[2][2] - Matirx.M[2][1] * Matirx.M[0][2]);
        M[1][1] = det_inv * (Matirx.M[0][0] * Matirx.M[2][2] - Matirx.M[2][0] * Matirx.M[0][2]);
        M[2][1] = -det_inv * (Matirx.M[0][0] * Matirx.M[2][1] - Matirx.M[2][0] * Matirx.M[0][1]);

        M[0][2] = det_inv * (Matirx.M[0][1] * Matirx.M[1][2] - Matirx.M[1][1] * Matirx.M[0][2]);
        M[1][2] = -det_inv * (Matirx.M[0][0] * Matirx.M[1][2] - Matirx.M[1][0] * Matirx.M[0][2]);
        M[2][2] = det_inv * (Matirx.M[0][0] * Matirx.M[1][1] - Matirx.M[1][0] * Matirx.M[0][1]);
    }

    inline Matrix3X3 operator* (const Matrix3X3 &Matirx) const
    {
        Matrix3X3 mResult;
        mResult.M[0][0] = 0;
        mResult.M[1][1] = 0;
        mResult.M[2][2] = 0;
#ifdef VS_SSE
        {
            VectorRegister A0 = MakeVectorRegister(M[0][0], M[0][1], M[0][2], 0.0f);
            VectorRegister A1 = MakeVectorRegister(M[1][0], M[1][1], M[1][2], 0.0f);
            VectorRegister A2 = MakeVectorRegister(M[2][0], M[2][1], M[2][2], 0.0f);

            VectorRegister B0 = MakeVectorRegister(Matirx.M[0][0], Matirx.M[0][1], Matirx.M[0][2], 0.0f);
            VectorRegister B1 = MakeVectorRegister(Matirx.M[1][0], Matirx.M[1][1], Matirx.M[1][2], 0.0f);
            VectorRegister B2 = MakeVectorRegister(Matirx.M[2][0], Matirx.M[2][1], Matirx.M[2][2], 0.0f);

            // First row of result (Matrix1[0] * Matrix2).
            VectorRegister Temp = VectorMultiply(VectorReplicate(A0, 0), B0);
            Temp = VectorMultiplyAdd(VectorReplicate(A0, 1), B1, Temp);
            VectorRegister R0 = VectorMultiplyAdd(VectorReplicate(A0, 2), B2, Temp);


            // Second row of result (Matrix1[1] * Matrix2).
            Temp = VectorMultiply(VectorReplicate(A1, 0), B0);
            Temp = VectorMultiplyAdd(VectorReplicate(A1, 1), B1, Temp);
            VectorRegister R1 = VectorMultiplyAdd(VectorReplicate(A1, 2), B2, Temp);

            // Third row of result (Matrix1[2] * Matrix2).
            Temp = VectorMultiply(VectorReplicate(A2, 0), B0);
            Temp = VectorMultiplyAdd(VectorReplicate(A2, 1), B1, Temp);
            VectorRegister R2 = VectorMultiplyAdd(VectorReplicate(A2, 2), B2, Temp);

            VectorStoreFloat3(R0, &mResult.m[0]);
            VectorStoreFloat3(R1, &mResult.m[3]);
            VectorStoreFloat3(R2, &mResult.m[6]);
        }
#else
        {
            for (unsigned char i = 0; i < 3; i++)
                for (unsigned char j = 0; j < 3; j++)
                    for (uint32 k = 0; k < 3; k++)
                        mResult.M[i][j] += M[i][k] * Matirx.M[k][j];
        }
#endif
        return mResult;
    }

    inline Vector3 operator * (const Vector3 &vc) const
    {
        return vc * (*this);
    }

    inline void CreateFromTwoVector(const Vector3 & v1, const Vector3 & v2)
    {
        for (int32 i = 0; i < 3; i++)
        {
            for (int32 j = 0; j < 3; j++)
            {
                M[i][j] = v1.m[i] * v2.m[j];
            }
        }
    }

    inline void SetZero()
    {
        memset(m, 0, sizeof(Matrix3X3));
    }
	

	void CreateFromDirection(Vector3 & Direction , const Vector3 &Up = Vector3(0,1,0));
	void CreateRotX(float a);								// 绕x旋转
	void CreateRotY(float a);								// 绕y旋转
	void CreateRotZ(float a);								// 绕z旋转
	void CreateEuler(float Roll,float Pitch, float Yaw);		// 绕z x and y构建，欧拉角  -pi <= y z <= pi  -pi/2 < x < pi/2
	void CreateAxisAngle(const Vector3 &vAxis, float a);	//绕vAxis旋转a弧度	
	//创建旋转矩阵
	void CreateRot(const Vector3 &U,const Vector3 &V,const Vector3 & N);
	//创建缩放矩阵
	void CreateScale(float fX,float fY,float fZ);

	void CreateScale(const Vector3 & Axis,float fScale);
	//得到eluer角度
	void GetEuler(float &Yaw,float &Pitch,float &Roll)const;
	//得到旋转轴和旋转角
	void GetAxisAngle(Vector3 & Axis,float & fAngle)const;
	//得到四元数
	Quat GetQuat()const;
	//线性插值
	void LineInterpolation(float t,const Matrix3X3 & M1, const Matrix3X3 &M2);
	//球形插值
	void Slerp(float t,const Matrix3X3 & M1, const Matrix3X3 &M2);
	Matrix3X3 operator *(float f)const;
	Matrix3X3 operator +(float f)const;
	Matrix3X3 operator -(float f)const;
	Matrix3X3 operator +(const Matrix3X3 &Matirx)const;
	Matrix3X3 operator -(const Matrix3X3 &Matirx)const;

	void operator *= (float f);
	void operator += (float f);
	void operator -= (float f);
	void operator += (const Matrix3X3 &Matirx);
	void operator -= (const Matrix3X3 &Matirx);

	bool operator ==(const Matrix3X3 &v)const;
	//按行获得向量
	void GetRowVector(Vector3 Row[3])const;
	//按行列得向量
	void GetColumnVector(Vector3 Column[3])const;

	void GetRowVector(Vector3 &Row0,Vector3 &Row1,Vector3 &Row2)const;
	void GetColumnVector(Vector3 &Column0,Vector3 &Column1,Vector3 &Column2)const;
	//按UVN获得
	void GetUVN(Vector3 UVN[3])const;
	void GetUVN(Vector3 & U,Vector3 &V,Vector3 &N)const;
	//求特征值，特征向量
	void GetEigenSystem(float EigenValue[3],Vector3 Eigen[3])const;

	void GetScale(Vector3 & Scale)const;
	void GetScaleAndRotator(Vector3 & Scale);
	const static Matrix3X3 ms_CameraViewRight;
	const static Matrix3X3 ms_CameraViewLeft;
	const static Matrix3X3 ms_CameraViewUp;
	const static Matrix3X3 ms_CameraViewDown;
	const static Matrix3X3 ms_CameraViewFront;
	const static Matrix3X3 ms_CameraViewBack;
	const static Matrix3X3 ms_Identity;
	const static Matrix3X3 ms_3DMax_To_Engine;
};

}