#include "math/matrix3X3w.h"
#include "math/maths.h"


namespace zq{


const Matrix3X3W Matrix3X3W::ms_Materix3X3WIdentity;
const Matrix3X3W Matrix3X3W::ms_3DMax_To_Engine = Matrix3X3W(1.0f, 0.0f, 0.0f, 0.0f,
																	0.0f, 0.0f, 1.0f, 0.0f,
																	0.0f, 1.0f, 0.0f, 0.0f,
																	0.0f, 0.0f, 0.0f, 1.0f);
/*----------------------------------------------------------------*/
Matrix3X3W::Matrix3X3W(void)
{
	Identity();

}
/*----------------------------------------------------------------*/
Matrix3X3W::Matrix3X3W(float m00,float m01,float m02,float m03,
							 float m10,float m11,float m12,float m13,
							 float m20,float m21,float m22,float m23,
							 float m30,float m31,float m32,float m33)
{

	_00 = m00; _01 = m01; _02 = m02; _03 = m03;
	_10 = m10; _11 = m11; _12 = m12; _13 = m13;
	_20 = m20; _21 = m21; _22 = m22; _23 = m23;
	_30 = m30; _31 = m31; _32 = m32; _33 = m33;
}
/*----------------------------------------------------------------*/
void Matrix3X3W::CreateTranslate(float dx, float dy, float dz) 
{
	Identity();
	_30 = dx;
	_31 = dy;
	_32 = dz;
}
/*----------------------------------------------------------------*/
void Matrix3X3W::CreateTranslate(const Vector3 & V)
{
	Identity();
	_30 = V.x;
	_31 = V.y;
	_32 = V.z;

}
/*----------------------------------------------------------------*/

void Matrix3X3W::CreateFromBillboard(const Vector3 &vcPos, const Matrix3X3 &CameraRotMatrix, bool bAxisY) 
{
	Matrix3X3 Mat;
	Mat.InverseOf(CameraRotMatrix);
	CreateFrom3X3(Mat);
	_30 = vcPos.x;
	_31 = vcPos.y;
	_32 = vcPos.z;
	_33 = 1.0f;
	/*
	���ֻ���а�Y����ת����ôֻ��Ҫ�����������A��Y������(0,1,0)
	A�� * A = E
		x1,y1,z1
	A=( x2,y2,z2	)
		x3,y3,z3

			a1,a2,a3
	A��=(	b1,b2,b3	)
			c1,c2,c3

	��ΪAΪ�������󣬴��п���Ҳ����������Ϊ��λ������������������Ҳ���ǵ��Ϊ0

	����A��ҲΪ�������󣬴��п���

	��ʵ����������д��п�������ν��ֻ����Ϊ�˺����������������ϣ����ⷽ��

	Ϊ��ֻ�ù����ֻ��Y����ת�����Ǽ����������Y������ʼ��Ϊ(0,1,0),Ҳ��˵�������

	Y�������ϣ���������������Ҳֻ����Y����ת�ˡ�

	�����ǲ��������������������ֻ��Ҫ�޸������Ϳ��ԡ�

	����

		x1,0,z1
	A=( x2,1,z2	)
		x3,0,z3

	�������������ΪX��������Z�������ǰ����ϸ��������������,
	����Ϊ����Y����(0,1,0),Ȼ�����ӵ�͹۲������E����������������������R
	Ȼ������������R��E����Ϸ���������

	�޸ĵ�A����������û���ڼ������Ϸ������Բ��ܱ�֤AΪ��������

			a1,a2,a3
	A��=(	0,1,0	)
			c1,c2,c3

	*/
	if(bAxisY)
	{
		_10 = 0.0f;
		_11 = 1.0f;
		_12 = 0.0f;

	}
} // Billboard
/*----------------------------------------------------------------*/
bool Matrix3X3W::CreateFromLookDir(const Vector3 &vcPos,					
									const Vector3 &vcDir,					
									const Vector3 &vcWorldUp)
{
	Vector3 vcTemp, vcUp;
	Vector3 Dir = vcDir;
	Dir.Normalize();

	//���㳯������Ϸ�����,��ΪDirΪ��λ����,����fDotΪvcWorld��
	//Dir�ϵ�ͶӰ
	float fDot = vcWorldUp.Dot(Dir);
	//����ͶӰ����,�����Dir��ͬ������
	vcTemp = Dir * fDot;
	//����������ƽ��4���η������up
	/*
			  /|
worldup-->	 / |<---up
			/__|__ 
	vcTemp
	*/				
	vcUp = vcWorldUp - vcTemp;

	//�������
	float fL = vcUp.GetLength();

	// ����Ϊ0,˵�� vcDir��worldUp����ƽ��,���Ǽ���Ϊ0.
	if (fL < EPSILON_E4) 
	{
		Vector3 vcY;
		//����ѡ����������,����up����
		vcY.Set(0.0f, 0.0f, 1.0f);

		vcTemp = Dir * Dir.z;
		vcUp = vcY - vcTemp;


		fL = vcUp.GetLength();
		// ����Ϊ0,˵�� vcDir��worldUp����ƽ��,���Ǽ���Ϊ0.
		if (fL < EPSILON_E4) 
		{
			
			//����ѡ����������,����up����
			vcY.Set(1.0f, 0.0f, 0.0f);

			vcTemp = Dir * Dir.x;
			vcUp = vcY - vcTemp;

			fL = vcUp.GetLength();

			
				if(fL < EPSILON_E4) return 0;
			
		}
	}
	//��λ��
	vcUp /= fL;


	Vector3 vcRight;
	vcRight.Cross(vcUp, Dir);

	CreateInWorldObject(vcRight, vcUp, Dir, vcPos);
	return 1;

}//LookDir
/*----------------------------------------------------------------*/
bool Matrix3X3W::CreateFromLookAt(const Vector3 &vcPos, const Vector3 &vcLookAt,
									const Vector3 &vcWorldUp) 
{
	Vector3 vcDir;
	//���㳯��
	vcDir = vcLookAt - vcPos;
	return CreateFromLookDir(vcPos,vcDir,vcWorldUp);
	
} // LookAt
/*----------------------------------------------------------------*/
void Matrix3X3W::CreateInWorldObject(const Vector3 &U,const Vector3 &V,const Vector3 & N,const Vector3 &Point)
{
	_00 = U.x;
	_10 = U.y;
	_20 = U.z;

	_01 = V.x;
	_11 = V.y;
	_21 = V.z;

	_02 = N.x;
	_12 = N.y;
	_22 = N.z;

	_03=0.0f; _13=0.0f; _23=0.0f; _33=1.0f;

	_30 = -(U.Dot(Point));
	_31 = -(V.Dot(Point));
	_32 = -(N.Dot(Point));

}
/*----------------------------------------------------------------*/
Matrix3X3W Matrix3X3W::operator +(float f)const
{

	Matrix3X3W Temp;
#ifdef VS_SSE
	{
		VectorRegister _v1 = VectorLoad(&m[0]);
		VectorRegister _v2 = VectorLoad(&m[4]);
		VectorRegister _v3 = VectorLoad(&m[8]);
		VectorRegister _v4 = VectorLoad(&m[12]);
		VectorRegister _f = VectorSetFloat1(f);

		VectorRegister _r1 = VectorAdd(_v1,_f);
		VectorRegister _r2 = VectorAdd(_v2,_f);
		VectorRegister _r3 = VectorAdd(_v3,_f);
		VectorRegister _r4 = VectorAdd(_v4,_f);

		VectorStore(_r1, &Temp.m[0]);
		VectorStore(_r2, &Temp.m[4]);
		VectorStore(_r3, &Temp.m[8]);
		VectorStore(_r4, &Temp.m[12]);
	}
#else
	{
		
		Temp.m[0] = m[0] + f;
		Temp.m[1] = m[1] + f;
		Temp.m[2] = m[2] + f;
		Temp.m[3] = m[3] + f;
		Temp.m[4] = m[4] + f;
		Temp.m[5] = m[5] + f;
		Temp.m[6] = m[6] + f;
		Temp.m[7] = m[7] + f;
		Temp.m[8] = m[8] + f;
		Temp.m[9] = m[9] + f;
		Temp.m[10] = m[10] + f;
		Temp.m[11] = m[11] + f;
		Temp.m[12] = m[12] + f;
		Temp.m[13] = m[13] + f;
		Temp.m[14] = m[14] + f;
		Temp.m[15] = m[15] + f;
		
	}
#endif
	return Temp;

}
/*----------------------------------------------------------------*/
Matrix3X3W Matrix3X3W::operator -(float f)const
{
	Matrix3X3W Temp;
#ifdef VS_SSE
	{
		VectorRegister _v1 = VectorLoad(&m[0]);
		VectorRegister _v2 = VectorLoad(&m[4]);
		VectorRegister _v3 = VectorLoad(&m[8]);
		VectorRegister _v4 = VectorLoad(&m[12]);
		VectorRegister _f = VectorSetFloat1(f);

		VectorRegister _r1 = VectorSubtract(_v1, _f);
		VectorRegister _r2 = VectorSubtract(_v2, _f);
		VectorRegister _r3 = VectorSubtract(_v3, _f);
		VectorRegister _r4 = VectorSubtract(_v4, _f);

		VectorStore(_r1, &Temp.m[0]);
		VectorStore(_r2, &Temp.m[4]);
		VectorStore(_r3, &Temp.m[8]);
		VectorStore(_r4, &Temp.m[12]);
	}
#else
	{
		Temp.m[0] = m[0] - f;
		Temp.m[1] = m[1] - f;
		Temp.m[2] = m[2] - f;
		Temp.m[3] = m[3] - f;
		Temp.m[4] = m[4] - f;
		Temp.m[5] = m[5] - f;
		Temp.m[6] = m[6] - f;
		Temp.m[7] = m[7] - f;
		Temp.m[8] = m[8] - f;
		Temp.m[9] = m[9] - f;
		Temp.m[10] = m[10] - f;
		Temp.m[11] = m[11] - f;
		Temp.m[12] = m[12] - f;
		Temp.m[13] = m[13] - f;
		Temp.m[14] = m[14] - f;
		Temp.m[15] = m[15] - f;
	}
#endif
	return Temp;

}
/*----------------------------------------------------------------*/
Matrix3X3W Matrix3X3W::operator *(float f)const
{
	Matrix3X3W Temp;
	#ifdef VS_SSE
	{
		VectorRegister _v1 = VectorLoad(&m[0]);
		VectorRegister _v2 = VectorLoad(&m[4]);
		VectorRegister _v3 = VectorLoad(&m[8]);
		VectorRegister _v4 = VectorLoad(&m[12]);
		VectorRegister _f = VectorSetFloat1(f);

		VectorRegister _r1 = VectorMultiply(_v1, _f);
		VectorRegister _r2 = VectorMultiply(_v2, _f);
		VectorRegister _r3 = VectorMultiply(_v3, _f);
		VectorRegister _r4 = VectorMultiply(_v4, _f);

		VectorStore(_r1, &Temp.m[0]);
		VectorStore(_r2, &Temp.m[4]);
		VectorStore(_r3, &Temp.m[8]);
		VectorStore(_r4, &Temp.m[12]);
	}
	#else
	{
		Temp.m[0] = m[0] * f;
		Temp.m[1] = m[1] * f;
		Temp.m[2] = m[2] * f;
		Temp.m[3] = m[3] * f;
		Temp.m[4] = m[4] * f;
		Temp.m[5] = m[5] * f;
		Temp.m[6] = m[6] * f;
		Temp.m[7] = m[7] * f;
		Temp.m[8] = m[8] * f;
		Temp.m[9] = m[9] * f;
		Temp.m[10] = m[10] * f;
		Temp.m[11] = m[11] * f;
		Temp.m[12] = m[12] * f;
		Temp.m[13] = m[13] * f;
		Temp.m[14] = m[14] * f;
		Temp.m[15] = m[15] * f;
	}
	#endif
	return Temp;
}
/*----------------------------------------------------------------*/
Matrix3X3W Matrix3X3W::operator +(const Matrix3X3W &Matirx)const
{

	Matrix3X3W Temp;
	#ifdef VS_SSE
	{
		VectorRegister _v1 = VectorLoad(&m[0]);
		VectorRegister _v2 = VectorLoad(&m[4]);
		VectorRegister _v3 = VectorLoad(&m[8]);
		VectorRegister _v4 = VectorLoad(&m[12]);

		VectorRegister _fv1 = VectorLoad(&Matirx.m[0]);
		VectorRegister _fv2 = VectorLoad(&Matirx.m[4]);
		VectorRegister _fv3 = VectorLoad(&Matirx.m[8]);
		VectorRegister _fv4 = VectorLoad(&Matirx.m[12]);

		VectorRegister _r1 = VectorAdd(_v1, _fv1);
		VectorRegister _r2 = VectorAdd(_v2, _fv2);
		VectorRegister _r3 = VectorAdd(_v3, _fv3);
		VectorRegister _r4 = VectorAdd(_v4, _fv4);

		VectorStore(_r1, &Temp.m[0]);
		VectorStore(_r2, &Temp.m[4]);
		VectorStore(_r3, &Temp.m[8]);
		VectorStore(_r4, &Temp.m[12]);
	}
	#else
	{
		Temp.m[0] = m[0] + Matirx.m[0];
		Temp.m[1] = m[1] + Matirx.m[1];
		Temp.m[2] = m[2] + Matirx.m[2];
		Temp.m[3] = m[3] + Matirx.m[3];
		Temp.m[4] = m[4] + Matirx.m[4];
		Temp.m[5] = m[5] + Matirx.m[5];
		Temp.m[6] = m[6] + Matirx.m[6];
		Temp.m[7] = m[7] + Matirx.m[7];
		Temp.m[8] = m[8] + Matirx.m[8];
		Temp.m[9] = m[9] + Matirx.m[9];
		Temp.m[10] = m[10] + Matirx.m[10];
		Temp.m[11] = m[11] + Matirx.m[11];
		Temp.m[12] = m[12] + Matirx.m[12];
		Temp.m[13] = m[13] + Matirx.m[13];
		Temp.m[14] = m[14] + Matirx.m[14];
		Temp.m[15] = m[15] + Matirx.m[15];
	}
	#endif
	return Temp;	

}
/*----------------------------------------------------------------*/
Matrix3X3W Matrix3X3W::operator -(const Matrix3X3W &Matirx)const
{
	Matrix3X3W Temp;
	#ifdef VS_SSE
	{
		VectorRegister _v1 = VectorLoad(&m[0]);
		VectorRegister _v2 = VectorLoad(&m[4]);
		VectorRegister _v3 = VectorLoad(&m[8]);
		VectorRegister _v4 = VectorLoad(&m[12]);

		VectorRegister _fv1 = VectorLoad(&Matirx.m[0]);
		VectorRegister _fv2 = VectorLoad(&Matirx.m[4]);
		VectorRegister _fv3 = VectorLoad(&Matirx.m[8]);
		VectorRegister _fv4 = VectorLoad(&Matirx.m[12]);

		VectorRegister _r1 = VectorSubtract(_v1, _fv1);
		VectorRegister _r2 = VectorSubtract(_v2, _fv2);
		VectorRegister _r3 = VectorSubtract(_v3, _fv3);
		VectorRegister _r4 = VectorSubtract(_v4, _fv4);

		VectorStore(_r1, &Temp.m[0]);
		VectorStore(_r2, &Temp.m[4]);
		VectorStore(_r3, &Temp.m[8]);
		VectorStore(_r4, &Temp.m[12]);
	}
	#else
	{
		Temp.m[0] = m[0] - Matirx.m[0];
		Temp.m[1] = m[1] - Matirx.m[1];
		Temp.m[2] = m[2] - Matirx.m[2];
		Temp.m[3] = m[3] - Matirx.m[3];
		Temp.m[4] = m[4] - Matirx.m[4];
		Temp.m[5] = m[5] - Matirx.m[5];
		Temp.m[6] = m[6] - Matirx.m[6];
		Temp.m[7] = m[7] - Matirx.m[7];
		Temp.m[8] = m[8] - Matirx.m[8];
		Temp.m[9] = m[9] - Matirx.m[9];
		Temp.m[10] = m[10] - Matirx.m[10];
		Temp.m[11] = m[11] - Matirx.m[11];
		Temp.m[12] = m[12] - Matirx.m[12];
		Temp.m[13] = m[13] - Matirx.m[13];
		Temp.m[14] = m[14] - Matirx.m[14];
		Temp.m[15] = m[15] - Matirx.m[15];
	}
	#endif
	return Temp;	

}


/*----------------------------------------------------------------*/
void Matrix3X3W::operator +=(float f)
{
#ifdef VS_SSE
	{
		VectorRegister _v1 = VectorLoad(&m[0]);
		VectorRegister _v2 = VectorLoad(&m[4]);
		VectorRegister _v3 = VectorLoad(&m[8]);
		VectorRegister _v4 = VectorLoad(&m[12]);
		VectorRegister _f = VectorSetFloat1(f);

		VectorRegister _r1 = VectorAdd(_v1, _f);
		VectorRegister _r2 = VectorAdd(_v2, _f);
		VectorRegister _r3 = VectorAdd(_v3, _f);
		VectorRegister _r4 = VectorAdd(_v4, _f);

		VectorStore(_r1, &m[0]);
		VectorStore(_r2, &m[4]);
		VectorStore(_r3, &m[8]);
		VectorStore(_r4, &m[12]);
	}
#else
	{
		m[0] = m[0] + f;
		m[1] = m[1] + f;
		m[2] = m[2] + f;
		m[3] = m[3] + f;
		m[4] = m[4] + f;
		m[5] = m[5] + f;
		m[6] = m[6] + f;
		m[7] = m[7] + f;
		m[8] = m[8] + f;
		m[9] = m[9] + f;
		m[10] = m[10] + f;
		m[11] = m[11] + f;
		m[12] = m[12] + f;
		m[13] = m[13] + f;
		m[14] = m[14] + f;
		m[15] = m[15] + f;
	}
#endif


}
/*----------------------------------------------------------------*/
void Matrix3X3W::operator -=(float f)
{
#ifdef VS_SSE
	{
		VectorRegister _v1 = VectorLoad(&m[0]);
		VectorRegister _v2 = VectorLoad(&m[4]);
		VectorRegister _v3 = VectorLoad(&m[8]);
		VectorRegister _v4 = VectorLoad(&m[12]);
		VectorRegister _f = VectorSetFloat1(f);

		VectorRegister _r1 = VectorSubtract(_v1, _f);
		VectorRegister _r2 = VectorSubtract(_v2, _f);
		VectorRegister _r3 = VectorSubtract(_v3, _f);
		VectorRegister _r4 = VectorSubtract(_v4, _f);

		VectorStore(_r1, &m[0]);
		VectorStore(_r2, &m[4]);
		VectorStore(_r3, &m[8]);
		VectorStore(_r4, &m[12]);
	}
#else
	{
		m[0] = m[0] - f;
		m[1] = m[1] - f;
		m[2] = m[2] - f;
		m[3] = m[3] - f;
		m[4] = m[4] - f;
		m[5] = m[5] - f;
		m[6] = m[6] - f;
		m[7] = m[7] - f;
		m[8] = m[8] - f;
		m[9] = m[9] - f;
		m[10] = m[10] - f;
		m[11] = m[11] - f;
		m[12] = m[12] - f;
		m[13] = m[13] - f;
		m[14] = m[14] - f;
		m[15] = m[15] - f;
	}
#endif

}
/*----------------------------------------------------------------*/
void Matrix3X3W::operator *=(float f)
{
	#ifdef VS_SSE
	{
		VectorRegister _v1 = VectorLoad(&m[0]);
		VectorRegister _v2 = VectorLoad(&m[4]);
		VectorRegister _v3 = VectorLoad(&m[8]);
		VectorRegister _v4 = VectorLoad(&m[12]);
		VectorRegister _f = VectorSetFloat1(f);

		VectorRegister _r1 = VectorMultiply(_v1, _f);
		VectorRegister _r2 = VectorMultiply(_v2, _f);
		VectorRegister _r3 = VectorMultiply(_v3, _f);
		VectorRegister _r4 = VectorMultiply(_v4, _f);

		VectorStore(_r1, &m[0]);
		VectorStore(_r2, &m[4]);
		VectorStore(_r3, &m[8]);
		VectorStore(_r4, &m[12]);
 	}
	#else
	{
		m[0] = m[0] * f;
		m[1] = m[1] * f;
		m[2] = m[2] * f;
		m[3] = m[3] * f;
		m[4] = m[4] * f;
		m[5] = m[5] * f;
		m[6] = m[6] * f;
		m[7] = m[7] * f;
		m[8] = m[8] * f;
		m[9] = m[9] * f;
		m[10] = m[10] * f;
		m[11] = m[11] * f;
		m[12] = m[12] * f;
		m[13] = m[13] * f;
		m[14] = m[14] * f;
		m[15] = m[15] * f;
	}
	#endif
}
/*----------------------------------------------------------------*/
void Matrix3X3W::operator +=(const Matrix3X3W &Matirx)
{
	#ifdef VS_SSE
	{
		VectorRegister _v1 = VectorLoad(&m[0]);
		VectorRegister _v2 = VectorLoad(&m[4]);
		VectorRegister _v3 = VectorLoad(&m[8]);
		VectorRegister _v4 = VectorLoad(&m[12]);

		VectorRegister _fv1 = VectorLoad(&Matirx.m[0]);
		VectorRegister _fv2 = VectorLoad(&Matirx.m[4]);
		VectorRegister _fv3 = VectorLoad(&Matirx.m[8]);
		VectorRegister _fv4 = VectorLoad(&Matirx.m[12]);

		VectorRegister _r1 = VectorAdd(_v1, _fv1);
		VectorRegister _r2 = VectorAdd(_v2, _fv2);
		VectorRegister _r3 = VectorAdd(_v3, _fv3);
		VectorRegister _r4 = VectorAdd(_v4, _fv4);

		VectorStore(_r1, &m[0]);
		VectorStore(_r2, &m[4]);
		VectorStore(_r3, &m[8]);
		VectorStore(_r4, &m[12]);
	}
	#else
	{
		m[0] = m[0] + Matirx.m[0];
		m[1] = m[1] + Matirx.m[1];
		m[2] = m[2] + Matirx.m[2];
		m[3] = m[3] + Matirx.m[3];
		m[4] = m[4] + Matirx.m[4];
		m[5] = m[5] + Matirx.m[5];
		m[6] = m[6] + Matirx.m[6];
		m[7] = m[7] + Matirx.m[7];
		m[8] = m[8] + Matirx.m[8];
		m[9] = m[9] + Matirx.m[9];
		m[10] = m[10] + Matirx.m[10];
		m[11] = m[11] + Matirx.m[11];
		m[12] = m[12] + Matirx.m[12];
		m[13] = m[13] + Matirx.m[13];
		m[14] = m[14] + Matirx.m[14];
		m[15] = m[15] + Matirx.m[15];
	}
	#endif

}
/*----------------------------------------------------------------*/
void Matrix3X3W::operator -=(const Matrix3X3W &Matirx)
{

	Matrix3X3W Temp;
	#ifdef VS_SSE
	{
		VectorRegister _v1 = VectorLoad(&m[0]);
		VectorRegister _v2 = VectorLoad(&m[4]);
		VectorRegister _v3 = VectorLoad(&m[8]);
		VectorRegister _v4 = VectorLoad(&m[12]);

		VectorRegister _fv1 = VectorLoad(&Matirx.m[0]);
		VectorRegister _fv2 = VectorLoad(&Matirx.m[4]);
		VectorRegister _fv3 = VectorLoad(&Matirx.m[8]);
		VectorRegister _fv4 = VectorLoad(&Matirx.m[12]);

		VectorRegister _r1 = VectorSubtract(_v1, _fv1);
		VectorRegister _r2 = VectorSubtract(_v2, _fv2);
		VectorRegister _r3 = VectorSubtract(_v3, _fv3);
		VectorRegister _r4 = VectorSubtract(_v4, _fv4);

		VectorStore(_r1, &m[0]);
		VectorStore(_r2, &m[4]);
		VectorStore(_r3, &m[8]);
		VectorStore(_r4, &m[12]);
	}
	#else
	{
		m[0] = m[0] - Matirx.m[0];
		m[1] = m[1] - Matirx.m[1];
		m[2] = m[2] - Matirx.m[2];
		m[3] = m[3] - Matirx.m[3];
		m[4] = m[4] - Matirx.m[4];
		m[5] = m[5] - Matirx.m[5];
		m[6] = m[6] - Matirx.m[6];
		m[7] = m[7] - Matirx.m[7];
		m[8] = m[8] - Matirx.m[8];
		m[9] = m[9] - Matirx.m[9];
		m[10] = m[10] - Matirx.m[10];
		m[11] = m[11] - Matirx.m[11];
		m[12] = m[12] - Matirx.m[12];
		m[13] = m[13] - Matirx.m[13];
		m[14] = m[14] - Matirx.m[14];
		m[15] = m[15] - Matirx.m[15];
	}
	#endif
}
/*----------------------------------------------------------------*/
void Matrix3X3W::CreateFrom3X3(const Matrix3X3 & Mat)
{
	Identity();
	for(int32 i = 0 ; i < 3 ; i++)
	{
		for(int32 j = 0 ; j < 3 ; j++)
		{
			M[i][j] = Mat.M[i][j];
		}
	
	}
	
}
/*----------------------------------------------------------------*/
void Matrix3X3W::Add3X3(const Matrix3X3 & Mat)
{
	for(int32 i = 0 ; i < 3 ; i++)
	{
		for(int32 j = 0 ; j < 3 ; j++)
		{
			M[i][j] = Mat.M[i][j];
		}

	}
}
/*----------------------------------------------------------------*/
void Matrix3X3W::AddTranslate(const Vector3 & V)
{
	_30 = V.x;
	_31 = V.y;
	_32 = V.z;
}
/*----------------------------------------------------------------*/
void Matrix3X3W::AddTranslate(float dx, float dy, float dz)
{
	_30 = dx;
	_31 = dy;
	_32 = dz;
}
/*----------------------------------------------------------------*/
bool Matrix3X3W::CreatePerspective(float fFov , float fAspect, float fZN ,float fZF)
{
	
	if(ABS(fZF - fZN) < 0.01f)
		return 0;

	float sinFOV2 = SIN(fFov/2);

	if(ABS(sinFOV2) < EPSILON_E5)
		return 0;

	float cosFOV2 = COS(fFov/2);

	float w =  1.0f  *(cosFOV2 / sinFOV2);
	float h =   fAspect * (cosFOV2 / sinFOV2);
	float Q = fZF / (fZF - fZN);
	Identity();
	_00 = w;
	_11 = h;
	_22 = Q;
	_23 = 1.0f;
	_32 = -Q * fZN;
	_33 = 0; 
	return 1;

}
/*----------------------------------------------------------------*/
bool Matrix3X3W::CreateOrthogonal(float fW ,float fH,	float fZN ,float fZF)
{
	if(ABS(fZF - fZN) < 0.01f)
		return 0;
	Identity();
	_00 = 2.0f / fW;
	_11 = 2.0f / fH;
	_22 = 1.0f / (fZF - fZN);
	_32 = -fZN / (fZF - fZN);
	return 1;
}
/*----------------------------------------------------------------*/
bool Matrix3X3W::CreateViewPort(float fX,float fY,float fWidth,float fHeight,float fMinz,float fMaxz)
{
	if(fMinz > fMaxz)
		return 0;
	Identity();
	_00 = fWidth * 0.5f ;
	_11 = - fHeight * 0.5f ;
	_22 = fMaxz - fMinz;
	_30 = fX + _00;
	_31 = fY - _11;
	_32 = fMinz;

	return 1;
	 
}
/*----------------------------------------------------------------*/
void Matrix3X3W::GetRowVector(Vector3W Row[4])const
{
	for(int32 i = 0 ; i < 4 ; i++)
	{
		for(int32 j = 0 ; j < 4 ; j++)
			Row[i].m[j] = M[i][j];

	}

}
/*----------------------------------------------------------------*/
void Matrix3X3W::GetColumnVector(Vector3W Column[4])const
{
	for(int32 i = 0 ; i < 4 ; i++)
	{
		for(int32 j = 0 ; j < 4 ; j++)
			Column[i].m[j] = M[j][i];

	}

}
/*----------------------------------------------------------------*/
void Matrix3X3W::GetRowVector(Vector3W &Row0,Vector3W &Row1,Vector3W &Row2,Vector3W &Row3)const
{
	Row0.x = M[0][0] ; Row0.y = M[0][1] ; Row0.z = M[0][2] ; Row0.w = M[0][3] ;
	Row1.x = M[1][0] ; Row1.y = M[1][1] ; Row1.z = M[1][2] ; Row1.w = M[1][3] ;
	Row2.x = M[2][0] ; Row2.y = M[2][1] ; Row2.z = M[2][2] ; Row2.w = M[2][3] ;
	Row3.x = M[3][0] ; Row3.y = M[3][1] ; Row3.z = M[3][2] ; Row3.w = M[3][3] ;
}
/*----------------------------------------------------------------*/
void Matrix3X3W::GetColumnVector(Vector3W &Column0,Vector3W &Column1,Vector3W &Column2,Vector3W &Column3)const
{
	Column0.x = M[0][0] ; Column0.y = M[1][0] ; Column0.z = M[2][0] ; Column0.w = M[3][0] ;
	Column1.x = M[0][1] ; Column1.y = M[1][1] ; Column1.z = M[2][1] ; Column1.w = M[3][1] ;
	Column2.x = M[0][2] ; Column2.y = M[1][2] ; Column2.z = M[2][2] ; Column2.w = M[3][2] ;
	Column3.x = M[0][3] ; Column3.y = M[1][3] ; Column3.z = M[2][3] ; Column3.w = M[3][3] ; 

}
/*----------------------------------------------------------------*/
bool Matrix3X3W::operator ==(const Matrix3X3W &v)const
{
	for (uint32 i = 0 ; i < 4 ; i++)
	{
		for (uint32 j = 0 ; j < 4 ; j++)
		{
			if (ABS(M[i][j] - v.M[i][j]) > EPSILON_E4)
			{
				return false;
			}
		}
		
	}
	return true;
}
/*void Matrix3X3W::CreatePointLightPlaneShadow(const Vector3 & LightPoint,const Plane &Plane)
{
	Vector3 PlaneN = Plane.GetN();
	float fD = Plane.GetfD();
	float temp = PlaneN * LightPoint + fD;

	_00 = temp - PlaneN.x * LightPoint.x;
	_01 = -PlaneN.x * LightPoint.y;
	_02 = -PlaneN.x * LightPoint.z;
	_03 = -PlaneN.x;

	_10 = -PlaneN.y * LightPoint.x;
	_11 = temp - PlaneN.y * LightPoint.y;
	_12 = -PlaneN.y * LightPoint.z;
	_13 = -PlaneN.y;

	_20 = -PlaneN.z * LightPoint.x;
	_21 = -PlaneN.z * LightPoint.y;
	_22 = temp - PlaneN.z * LightPoint.z;
	_23 = -PlaneN.z;

	_30 = -fD * LightPoint.x ;
	_31 = -fD * LightPoint.y ;
	_32 = -fD * LightPoint.z ;
	_33 = PlaneN * LightPoint;
}
/*----------------------------------------------------------------*/
/*void Matrix3X3W::CreateInfiniteLightPlaneShadow(const Vector3 & LightDir,const Plane &Plane)
{

	Vector3 PlaneN = Plane.GetN();
	float fD = Plane.GetfD();
	float temp = PlaneN * LightDir;

	_00 = temp - PlaneN.x * LightDir.x;
	_01 = -PlaneN.x * LightDir.y;
	_02 = -PlaneN.x * LightDir.z;
	_03 = -PlaneN.x;

	_10 = -PlaneN.y * LightDir.x;
	_11 = temp - PlaneN.y * LightDir.y;
	_12 = -PlaneN.y * LightDir.z;
	_13 = -PlaneN.y;

	_20 = -PlaneN.z * LightDir.x;
	_21 = -PlaneN.z * LightDir.y;
	_22 = temp - PlaneN.z * LightDir.z;
	_23 = -PlaneN.z;

	_30 = -fD * LightDir.x ;
	_31 = -fD * LightDir.y ;
	_32 = -fD * LightDir.z ;
	_33 = PlaneN * LightDir;
}*/

}
