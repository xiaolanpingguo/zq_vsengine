#pragma once


namespace zq{


class Matrix3X3;
class Vector3;
class MATH_API Quat 
{
public:
	union
	{
		float m[4];
		struct  
		{
			float x, y, z, w;
		};
	};

	Quat(void);
	Quat(float _x, float _y, float _z, float _w);
	void Set(float _x, float _y, float _z, float _w);
	//通过旋转轴和旋转角构造四元数
	void CreateAxisAngle(const Vector3& Axis,float fAngle);
	//由欧拉角构造四元数
	void  CreateEuler(float fRoll, float fPitch, float fYaw);
	//单位化
	void  Normalize();
	//求共轭
	Quat  GetConjugate()const;
	//得到欧拉角
	void  GetEulers(float &fRoll, float &fPitch, float &fYaw)const;
	//从四元数得到变换矩阵
	void  GetMatrix(Matrix3X3 &Matrix)const;
	//得到长度
	float GetLength(void)const;
	//取得逆
	Quat GetInverse()const;
	//取得旋转轴和旋转角
	void GetAxisAngle(Vector3 & Axis , float & fAngle)const;
	void    operator /= (float f);
	Quat operator /  (float f)const;

	void    operator *= (float f);
	Quat operator *  (float f)const;

	Quat operator *  (const Vector3 &v) const;

	Quat operator *  (const Quat &q) const;
	void    operator *= (const Quat &q);

	void    operator += (const Quat &q);
	Quat operator +  (const Quat &q) const;

	void    operator -= (const Quat &q);
	Quat operator -  (const Quat &q) const;

	bool operator ==(const Quat &q)const;
	//求点积
	float Dot(const Quat& q)const;
	//求共轭
	Quat operator ~(void) const;

	//求q2绕q1旋转后的四元数
	void Rotate(const Quat &q1, const Quat &q2);

	//旋转一个向量
	Vector3 Rotate(const Vector3 &v)const;
	//插值
	void Slerp(float t,const Quat & q1,const Quat & q2);
	//三角形2维球型插值
	void TriangleSlerp(float t1,float t2, const Quat & q1,const Quat & q2,const Quat & q3);
	//四元数样条插值
	void Slerp(float t,const Quat & q1,const Quat & q2,const Quat & s1,const Quat & s2);
	void SlerpSValueOf(const Quat & q1,const Quat & q2,const Quat & q3);
	//求幂
	Quat Pow(float exp)const;
	//求以e为底的对数
	Quat Ln()const;
	//求以e为底指数
	Quat Exp()const;
};

}