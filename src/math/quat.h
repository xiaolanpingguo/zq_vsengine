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
	//ͨ����ת�����ת�ǹ�����Ԫ��
	void CreateAxisAngle(const Vector3& Axis,float fAngle);
	//��ŷ���ǹ�����Ԫ��
	void  CreateEuler(float fRoll, float fPitch, float fYaw);
	//��λ��
	void  Normalize();
	//����
	Quat  GetConjugate()const;
	//�õ�ŷ����
	void  GetEulers(float &fRoll, float &fPitch, float &fYaw)const;
	//����Ԫ���õ��任����
	void  GetMatrix(Matrix3X3 &Matrix)const;
	//�õ�����
	float GetLength(void)const;
	//ȡ����
	Quat GetInverse()const;
	//ȡ����ת�����ת��
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
	//����
	float Dot(const Quat& q)const;
	//����
	Quat operator ~(void) const;

	//��q2��q1��ת�����Ԫ��
	void Rotate(const Quat &q1, const Quat &q2);

	//��תһ������
	Vector3 Rotate(const Vector3 &v)const;
	//��ֵ
	void Slerp(float t,const Quat & q1,const Quat & q2);
	//������2ά���Ͳ�ֵ
	void TriangleSlerp(float t1,float t2, const Quat & q1,const Quat & q2,const Quat & q3);
	//��Ԫ��������ֵ
	void Slerp(float t,const Quat & q1,const Quat & q2,const Quat & s1,const Quat & s2);
	void SlerpSValueOf(const Quat & q1,const Quat & q2,const Quat & q3);
	//����
	Quat Pow(float exp)const;
	//����eΪ�׵Ķ���
	Quat Ln()const;
	//����eΪ��ָ��
	Quat Exp()const;
};

}