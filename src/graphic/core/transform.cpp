#include "graphic/core/transform.h"
#include "math/matrix3X3.h"
#include "graphic/core/stream/stream.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
const VSTransform VSTransform::ms_Indetity;
VSTransform::VSTransform()
{
	m_mRotate.Identity();
	m_fScale = Vector3(1.0f,1.0f,1.0f);
	m_vTranslate.Set(0.0f,0.0f,0.0f);
	m_mCombine.Identity();
	m_mCombineInverse.Identity();
	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;
}
void VSTransform::Indetity()
{
	m_mRotate.Identity();
	m_fScale = Vector3(1.0f,1.0f,1.0f);
	m_vTranslate.Set(0.0f,0.0f,0.0f);
	m_mCombine.Identity();
	m_mCombineInverse.Identity();
	m_bIsCombine = 1;
	m_bIsHaveInverse = 1;
}
VSTransform::~VSTransform()
{



}
void VSTransform::Combine()
{
	if(!m_bIsCombine)
	{
		Matrix3X3 Mat;
		Mat = Matrix3X3(	m_mRotate._00 * m_fScale.x,m_mRotate._01 * m_fScale.x,m_mRotate._02 * m_fScale.x,
							m_mRotate._10 * m_fScale.y,m_mRotate._11 * m_fScale.y,m_mRotate._12 * m_fScale.y,
							m_mRotate._20 * m_fScale.z,m_mRotate._21 * m_fScale.z,m_mRotate._22 * m_fScale.z);
		m_mCombine.Add3X3(Mat);
		m_mCombine.AddTranslate(m_vTranslate);
		m_bIsCombine = 1;
	}

}

void VSTransform::ApplyForward(const Vector3 & In, Vector3 & Out,uint32 uiTransformType)
{
	
	if(uiTransformType == TT_POS)
	{
		Combine();
		Out = In * m_mCombine;
	}
	else if (uiTransformType == TT_DIR)
	{
		Combine();
		Matrix3X3 Mat;
		m_mCombine.Get3X3(Mat);
		Out = In * Mat;
	}


}
void VSTransform::Product(const VSTransform & t1,const VSTransform &t2,uint32 TransformFlag)
{
	
	Matrix3X3 Mat1;
	Mat1 = Matrix3X3(	t1.m_mRotate._00 * t1.m_fScale.x,t1.m_mRotate._01 * t1.m_fScale.x,t1.m_mRotate._02 * t1.m_fScale.x,
						t1.m_mRotate._10 * t1.m_fScale.y,t1.m_mRotate._11 * t1.m_fScale.y,t1.m_mRotate._12 * t1.m_fScale.y,
						t1.m_mRotate._20 * t1.m_fScale.z,t1.m_mRotate._21 * t1.m_fScale.z,t1.m_mRotate._22 * t1.m_fScale.z);

	Matrix3X3 TempRotate = t2.m_mRotate;
	Vector3 TempTranslate = t2.m_vTranslate;
	Vector3 TempScale = t2.m_fScale;
	if (!(TransformFlag & TF_SCALE))
	{
		TempScale = Vector3(1.0f,1.0f,1.0f);
	}

	if (!(TransformFlag & TF_ROTATE))
	{
		TempRotate.Identity();	
	}
	if (!(TransformFlag & TF_TRANSFORM))
	{
		TempTranslate = Vector3(0.0f,0.0f,0.0f);
	}

	Matrix3X3 Mat2;
	Mat2 = Matrix3X3(	TempRotate._00 * TempScale.x,TempRotate._01 * TempScale.x,TempRotate._02 * TempScale.x,
						TempRotate._10 * TempScale.y,TempRotate._11 * TempScale.y,TempRotate._12 * TempScale.y,
						TempRotate._20 * TempScale.z,TempRotate._21 * TempScale.z,TempRotate._22 * TempScale.z);

	m_mRotate = Mat1 * Mat2;
	m_mRotate.GetScaleAndRotator(m_fScale);
	m_vTranslate = t1.m_vTranslate * Mat2 + TempTranslate;

	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;
}

void VSTransform::Inverse(VSTransform & Out)const
{
	
	Matrix3X3 Mat;
	Mat = Matrix3X3(	m_mRotate._00 * m_fScale.x,m_mRotate._01 * m_fScale.x,m_mRotate._02 * m_fScale.x,
						m_mRotate._10 * m_fScale.y,m_mRotate._11 * m_fScale.y,m_mRotate._12 * m_fScale.y,
						m_mRotate._20 * m_fScale.z,m_mRotate._21 * m_fScale.z,m_mRotate._22 * m_fScale.z);

	Out.m_mRotate.InverseOf(Mat);
	Out.m_vTranslate = m_vTranslate * Out.m_mRotate * (-1.0f);
	Out.m_mRotate.GetScaleAndRotator(Out.m_fScale);
	Out.m_bIsCombine = 0;
	Out.m_bIsHaveInverse = 0;
}
void VSTransform::ApplyInverse(const Vector3 & In, Vector3 & Out,uint32 uiTransformType)
{
	
	if(uiTransformType == TT_POS)
	{
		const Matrix3X3W &mat = GetCombineInverse();
		Out = In * mat;

	}
	else if (uiTransformType == TT_DIR)
	{
		const Matrix3X3W &mat = GetCombineInverse();
		Matrix3X3 Mat;
		mat.Get3X3(Mat);
		Out = In * Mat;
	}
}
void VSTransform::SetMatrix(const Matrix3X3W & VSMat)
{
	VSMat.Get3X3(m_mRotate);
	m_vTranslate = VSMat.GetTranslation();
	m_mRotate.GetScaleAndRotator(m_fScale);

	m_bIsCombine = 1;
	m_mCombine = VSMat;
	m_bIsHaveInverse = 0;
}
void VSTransform::Interpolation(const VSTransform &t1 , const VSTransform & t2,float t)
{

	m_mRotate.Slerp(t,t1.m_mRotate,t2.m_mRotate);	


	m_fScale = LineInterpolation(t1.m_fScale,t2.m_fScale,t);


	m_vTranslate = LineInterpolation(t1.m_vTranslate , t2.m_vTranslate , t);

	
	m_bIsCombine = 0;
	m_bIsHaveInverse = 0;
}
