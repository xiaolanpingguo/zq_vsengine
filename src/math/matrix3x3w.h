#pragma once


#include "math/maths.h"
#include "math/vector3.h"
#include "math/vector3w.h"
#include "math/matrix3X3.h"


namespace zq{


class MATH_API Matrix3X3W 
{
public:
	union
	{
		float m[16];
		struct
		{
			float _00, _01, _02, _03;
			float _10, _11, _12, _13;
			float _20, _21, _22, _23;
			float _30, _31, _32, _33;
		};
		float M[4][4];
	};

	Matrix3X3W(void);
	Matrix3X3W(float m00,float m01,float m02,float m03,
				float m10,float m11,float m12,float m13,
				float m20,float m21,float m22,float m23,
				float m30,float m31,float m32,float m33);

    inline void Identity(void)
    {
        memset(m, 0, sizeof(Matrix3X3W));
        _00 = _11 = _22 = _33 = 1.0f;
    }

    inline Vector3 GetTranslation(void)const
    {
        return Vector3(_30, _31, _32);
    }

    inline Matrix3X3W operator* (const Matrix3X3W &Matirx) const
    {
        Matrix3X3W mResult;
#ifdef VS_SSE

        VectorRegister A0 = VectorLoad(&m[0]);
        VectorRegister A1 = VectorLoad(&m[4]);
        VectorRegister A2 = VectorLoad(&m[8]);
        VectorRegister A3 = VectorLoad(&m[12]);

        VectorRegister B0 = VectorLoad(&Matirx.m[0]);
        VectorRegister B1 = VectorLoad(&Matirx.m[4]);
        VectorRegister B2 = VectorLoad(&Matirx.m[8]);
        VectorRegister B3 = VectorLoad(&Matirx.m[12]);

        // First row of result (Matrix1[0] * Matrix2).
        VectorRegister Temp = VectorMultiply(VectorReplicate(A0, 0), B0);
        Temp = VectorMultiplyAdd(VectorReplicate(A0, 1), B1, Temp);
        Temp = VectorMultiplyAdd(VectorReplicate(A0, 2), B2, Temp);
        VectorRegister R0 = VectorMultiplyAdd(VectorReplicate(A0, 3), B3, Temp);

        // Second row of result (Matrix1[1] * Matrix2).
        Temp = VectorMultiply(VectorReplicate(A1, 0), B0);
        Temp = VectorMultiplyAdd(VectorReplicate(A1, 1), B1, Temp);
        Temp = VectorMultiplyAdd(VectorReplicate(A1, 2), B2, Temp);
        VectorRegister R1 = VectorMultiplyAdd(VectorReplicate(A1, 3), B3, Temp);

        // Third row of result (Matrix1[2] * Matrix2).
        Temp = VectorMultiply(VectorReplicate(A2, 0), B0);
        Temp = VectorMultiplyAdd(VectorReplicate(A2, 1), B1, Temp);
        Temp = VectorMultiplyAdd(VectorReplicate(A2, 2), B2, Temp);
        VectorRegister R2 = VectorMultiplyAdd(VectorReplicate(A2, 3), B3, Temp);

        // Fourth row of result (Matrix1[3] * Matrix2).
        Temp = VectorMultiply(VectorReplicate(A3, 0), B0);
        Temp = VectorMultiplyAdd(VectorReplicate(A3, 1), B1, Temp);
        Temp = VectorMultiplyAdd(VectorReplicate(A3, 2), B2, Temp);
        VectorRegister R3 = VectorMultiplyAdd(VectorReplicate(A3, 3), B3, Temp);
        VectorStore(R0, &mResult.m[0]);
        VectorStore(R1, &mResult.m[4]);
        VectorStore(R2, &mResult.m[8]);
        VectorStore(R3, &mResult.m[12]);
#else
        mResult.M[0][0] = 0;
        mResult.M[1][1] = 0;
        mResult.M[2][2] = 0;
        mResult.M[3][3] = 0;
        for (unsigned char i = 0; i < 4; i++)
            for (unsigned char j = 0; j < 4; j++)
                for (uint32 k = 0; k < 4; k++)
                    mResult.M[i][j] += M[i][k] * Matirx.M[k][j];
#endif		

        return mResult;
    }

    inline Vector3W Matrix3X3W::operator* (const Vector3W &vc)const
    {
        Vector3W vcResult;

        vcResult.x = vc.x * _00 + vc.y * _10 + vc.z * _20 + _30;
        vcResult.y = vc.x * _01 + vc.y * _11 + vc.z * _21 + _31;
        vcResult.z = vc.x * _02 + vc.y * _12 + vc.z * _22 + _32;
        vcResult.w = vc.x * _03 + vc.y * _13 + vc.z * _23 + _33;

        vcResult.x = vcResult.x / vcResult.w;
        vcResult.y = vcResult.y / vcResult.w;
        vcResult.z = vcResult.z / vcResult.w;
        vcResult.w = 1.0f;

        return vcResult;
    }

    inline Vector3 operator* (const Vector3 &vc) const
    {
        Vector3 vcResult;

        vcResult.x = vc.x * _00 + vc.y * _10 + vc.z * _20 + _30;
        vcResult.y = vc.x * _01 + vc.y * _11 + vc.z * _21 + _31;
        vcResult.z = vc.x * _02 + vc.y * _12 + vc.z * _22 + _32;

        float w = vc.x * _03 + vc.y * _13 + vc.z * _23 + _33;

        vcResult.x = vcResult.x / w;
        vcResult.y = vcResult.y / w;
        vcResult.z = vcResult.z / w;

        return vcResult;
    }

    inline Vector3 Apply3X3(const Vector3 &v)const
    {
        float x, y, z;
        x = v.x * _00 + v.y * _10 + v.z * _20;
        y = v.x * _01 + v.y * _11 + v.z * _21;
        z = v.x * _02 + v.y * _12 + v.z * _22;

        return Vector3(x, y, z);
    }

    inline Vector3 Matrix3X3W::ApplyTranlaste(const Vector3 &Point)const
    {
        return Vector3(Point.x + _30, Point.y + _31, Point.z + _32);
    }

    inline void TransposeOf(const Matrix3X3W &Matrix)
    {
        _00 = Matrix._00;
        _10 = Matrix._01;
        _20 = Matrix._02;
        _30 = Matrix._03;

        _01 = Matrix._10;
        _11 = Matrix._11;
        _21 = Matrix._12;
        _31 = Matrix._13;

        _02 = Matrix._20;
        _12 = Matrix._21;
        _22 = Matrix._22;
        _32 = Matrix._23;

        _03 = Matrix._30;
        _13 = Matrix._31;
        _23 = Matrix._32;
        _33 = Matrix._33;
    }

    Matrix3X3W GetTranspose()const
    {
        Matrix3X3W Temp;
        Temp.TransposeOf(*this);
        return Temp;
    }

    Matrix3X3W GetInverse()const
    {
        Matrix3X3W Temp;
        Temp.InverseOf(*this);
        return Temp;
    }

    inline void InverseOf(const Matrix3X3W &Matirx)
    {
        Matrix3X3W mTrans;
        float    fTemp[12], fDet;

        mTrans.TransposeOf(Matirx);

        fTemp[0] = mTrans._22 * mTrans._33;
        fTemp[1] = mTrans._23 * mTrans._32;
        fTemp[2] = mTrans._21 * mTrans._33;
        fTemp[3] = mTrans._23 * mTrans._31;
        fTemp[4] = mTrans._21 * mTrans._32;
        fTemp[5] = mTrans._22 * mTrans._31;
        fTemp[6] = mTrans._20 * mTrans._33;
        fTemp[7] = mTrans._23 * mTrans._30;
        fTemp[8] = mTrans._20 * mTrans._32;
        fTemp[9] = mTrans._22 * mTrans._30;
        fTemp[10] = mTrans._20 * mTrans._31;
        fTemp[11] = mTrans._21 * mTrans._30;


        _00 = fTemp[0] * mTrans._11 + fTemp[3] * mTrans._12 + fTemp[4] * mTrans._13;
        _00 -= fTemp[1] * mTrans._11 + fTemp[2] * mTrans._12 + fTemp[5] * mTrans._13;
        _01 = fTemp[1] * mTrans._10 + fTemp[6] * mTrans._12 + fTemp[9] * mTrans._13;
        _01 -= fTemp[0] * mTrans._10 + fTemp[7] * mTrans._12 + fTemp[8] * mTrans._13;
        _02 = fTemp[2] * mTrans._10 + fTemp[7] * mTrans._11 + fTemp[10] * mTrans._13;
        _02 -= fTemp[3] * mTrans._10 + fTemp[6] * mTrans._11 + fTemp[11] * mTrans._13;
        _03 = fTemp[5] * mTrans._10 + fTemp[8] * mTrans._11 + fTemp[11] * mTrans._12;
        _03 -= fTemp[4] * mTrans._10 + fTemp[9] * mTrans._11 + fTemp[10] * mTrans._12;
        _10 = fTemp[1] * mTrans._01 + fTemp[2] * mTrans._02 + fTemp[5] * mTrans._03;
        _10 -= fTemp[0] * mTrans._01 + fTemp[3] * mTrans._02 + fTemp[4] * mTrans._03;
        _11 = fTemp[0] * mTrans._00 + fTemp[7] * mTrans._02 + fTemp[8] * mTrans._03;
        _11 -= fTemp[1] * mTrans._00 + fTemp[6] * mTrans._02 + fTemp[9] * mTrans._03;
        _12 = fTemp[3] * mTrans._00 + fTemp[6] * mTrans._01 + fTemp[11] * mTrans._03;
        _12 -= fTemp[2] * mTrans._00 + fTemp[7] * mTrans._01 + fTemp[10] * mTrans._03;
        _13 = fTemp[4] * mTrans._00 + fTemp[9] * mTrans._01 + fTemp[10] * mTrans._02;
        _13 -= fTemp[5] * mTrans._00 + fTemp[8] * mTrans._01 + fTemp[11] * mTrans._02;


        fTemp[0] = mTrans._02 * mTrans._13;
        fTemp[1] = mTrans._03 * mTrans._12;
        fTemp[2] = mTrans._01 * mTrans._13;
        fTemp[3] = mTrans._03 * mTrans._11;
        fTemp[4] = mTrans._01 * mTrans._12;
        fTemp[5] = mTrans._02 * mTrans._11;
        fTemp[6] = mTrans._00 * mTrans._13;
        fTemp[7] = mTrans._03 * mTrans._10;
        fTemp[8] = mTrans._00 * mTrans._12;
        fTemp[9] = mTrans._02 * mTrans._10;
        fTemp[10] = mTrans._00 * mTrans._11;
        fTemp[11] = mTrans._01 * mTrans._10;


        _20 = fTemp[0] * mTrans._31 + fTemp[3] * mTrans._32 + fTemp[4] * mTrans._33;
        _20 -= fTemp[1] * mTrans._31 + fTemp[2] * mTrans._32 + fTemp[5] * mTrans._33;
        _21 = fTemp[1] * mTrans._30 + fTemp[6] * mTrans._32 + fTemp[9] * mTrans._33;
        _21 -= fTemp[0] * mTrans._30 + fTemp[7] * mTrans._32 + fTemp[8] * mTrans._33;
        _22 = fTemp[2] * mTrans._30 + fTemp[7] * mTrans._31 + fTemp[10] * mTrans._33;
        _22 -= fTemp[3] * mTrans._30 + fTemp[6] * mTrans._31 + fTemp[11] * mTrans._33;
        _23 = fTemp[5] * mTrans._30 + fTemp[8] * mTrans._31 + fTemp[11] * mTrans._32;
        _23 -= fTemp[4] * mTrans._30 + fTemp[9] * mTrans._31 + fTemp[10] * mTrans._32;
        _30 = fTemp[2] * mTrans._22 + fTemp[5] * mTrans._23 + fTemp[1] * mTrans._21;
        _30 -= fTemp[4] * mTrans._23 + fTemp[0] * mTrans._21 + fTemp[3] * mTrans._22;
        _31 = fTemp[8] * mTrans._23 + fTemp[0] * mTrans._20 + fTemp[7] * mTrans._22;
        _31 -= fTemp[6] * mTrans._22 + fTemp[9] * mTrans._23 + fTemp[1] * mTrans._20;
        _32 = fTemp[6] * mTrans._21 + fTemp[11] * mTrans._23 + fTemp[3] * mTrans._20;
        _32 -= fTemp[10] * mTrans._23 + fTemp[2] * mTrans._20 + fTemp[7] * mTrans._21;
        _33 = fTemp[10] * mTrans._22 + fTemp[4] * mTrans._20 + fTemp[9] * mTrans._21;
        _33 -= fTemp[8] * mTrans._21 + fTemp[11] * mTrans._22 + fTemp[5] * mTrans._20;

        fDet = mTrans._00*_00 + mTrans._01*_01 + mTrans._02*_02 + mTrans._03*_03;

        fDet = 1 / fDet;

        _00 *= fDet;
        _01 *= fDet;
        _02 *= fDet;
        _03 *= fDet;

        _10 *= fDet;
        _11 *= fDet;
        _12 *= fDet;
        _13 *= fDet;

        _20 *= fDet;
        _21 *= fDet;
        _22 *= fDet;
        _23 *= fDet;

        _30 *= fDet;
        _31 *= fDet;
        _32 *= fDet;
        _33 *= fDet;
    }

    inline void Get3X3(Matrix3X3 & Mat)const
    {
        for (int32 i = 0; i < 3; i++)
            for (int32 j = 0; j < 3; j++)
            {
                Mat.M[i][j] = M[i][j];
            }
    }

    inline void SetZero()
    {
        memset(M, 0, sizeof(Matrix3X3W));
    }


		
	//用3*3矩阵创建
	void CreateFrom3X3(const Matrix3X3 & Mat);
	//平移矩阵
	void CreateTranslate(float dx, float dy, float dz);
	void CreateTranslate(const Vector3 & V);
	//建立变换矩阵
	void CreateInWorldObject(const Vector3 &U,const Vector3 &V,const Vector3 & N,const Vector3 &Point);
		
	//建立广告牌变换矩阵
	void CreateFromBillboard(const Vector3 &vcPos,					//广告牌位置
							const Matrix3X3 &CameraRotMatrix,		//相机或其他矩阵
							bool bAxisY);						//是否只绕Y轴旋转
	//够建相机矩阵(根据视见方向)
	bool CreateFromLookDir(const Vector3 &vcPos,					//相机位置
				const Vector3 &vcDir,								//观察方向
				const Vector3 &vcWorldUp = Vector3(0,1,0));
	//够建相机矩阵(根据目标位置)
	bool CreateFromLookAt(const Vector3 &vcPos,									//相机位置
							const Vector3 &vcLookAt,							//观察位置
							const Vector3 &vcWorldUp = Vector3(0,1,0));		//上方向
	//建立透视投影矩阵
	bool CreatePerspective(float fFov ,				//X方向张角
							float fAspect,				//宽高比
							float fZN ,				//近剪裁面
							float fZF);				//远剪裁面
	//建立正交投影矩阵
	bool CreateOrthogonal(float fW ,				//宽
						float fH,					//高
						float fZN ,				//近剪裁面
						float fZF)	;				//远剪裁面
	//建立视口矩阵
	bool CreateViewPort(float fX,float fY,float fWidth,float fHeight,float fMinz,float fMaxz);
	//void CreatePointLightPlaneShadow(const Vector3 & LightPoint,const Plane &Plane);
	//void CreateInfiniteLightPlaneShadow(const Vector3 & LightDir,const Plane &Plane);
	Matrix3X3W operator *(float f)const;
	Matrix3X3W operator +(float f)const;
	Matrix3X3W operator -(float f)const;
	Matrix3X3W operator +(const Matrix3X3W &Matirx)const;
	Matrix3X3W operator -(const Matrix3X3W &Matirx)const;

	void operator *= (float f);
	void operator += (float f);
	void operator -= (float f);
	void operator += (const Matrix3X3W &Matirx);
	void operator -= (const Matrix3X3W &Matirx);

	bool operator ==(const Matrix3X3W &v)const;

	//加入把3*3矩阵 旋转或者缩放
	void Add3X3(const Matrix3X3 & Mat);
	//添加平移
	void AddTranslate(const Vector3 & V);
	void AddTranslate(float dx, float dy, float dz);

	//按行获得向量
	void GetRowVector(Vector3W Row[4])const;
	//按行列得向量
	void GetColumnVector(Vector3W Column[4])const;

	void GetRowVector(Vector3W &Row0,Vector3W &Row1,Vector3W &Row2,Vector3W &Row3)const;
	void GetColumnVector(Vector3W &Column0,Vector3W &Column1,Vector3W &Column2,Vector3W &Column3)const;
	const static Matrix3X3W ms_Materix3X3WIdentity;
	const static Matrix3X3W ms_3DMax_To_Engine;
};

}