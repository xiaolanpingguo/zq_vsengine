#include "graphic/controller/animtree/animatom.h"
#include "math/othermath.h"
#include "graphic/core/transform.h"


using namespace zq;
VSAnimAtom VSAnimAtom::ms_Identity;
VSAnimAtom VSAnimAtom::ms_FastBlendZero(Vector3(0.0f,0.0f,0.0f), Vector3(0.0f, 0.0f, 0.0f),Quat(0.0f, 0.0f, 0.0f,0.0f));
void VSAnimAtom::Identity()
{
	m_fScale.Set(1.0f,1.0f,1.0f);
	m_Pos.Set(0.0f,0.0f,0.0f);
	m_Rotator.Set(0.0f,0.0f,0.0f,1.0f);

}
void VSAnimAtom::GetMatrix(Matrix3X3W & OutMatrix)const
{
	Matrix3X3 mRotate;
	m_Rotator.GetMatrix(mRotate);

	Matrix3X3 Mat;
	Mat = Matrix3X3(	mRotate._00 * m_fScale.x,mRotate._01 * m_fScale.x,mRotate._02 * m_fScale.x,
						mRotate._10 * m_fScale.y,mRotate._11 * m_fScale.y,mRotate._12 * m_fScale.y,
						mRotate._20 * m_fScale.z,mRotate._21 * m_fScale.z,mRotate._22 * m_fScale.z);
	OutMatrix.AddTranslate(m_Pos);
	OutMatrix.Add3X3(Mat);
}
void VSAnimAtom::FromTransform(const VSTransform& T)
{
	m_fScale = T.GetScale();
	m_Pos = T.GetTranslate();
	m_Rotator =T.GetRotate().GetQuat();
}
void VSAnimAtom::FromMatrix(const Matrix3X3W & m)
{
	VSTransform T;
	T.SetMatrix(m);
	FromTransform(T);
}
VSTransform VSAnimAtom::GetTransfrom()const
{
	VSTransform Temp;
	Matrix3X3 Mat;
	m_Rotator.GetMatrix(Mat);
	Temp.SetRotate(Mat);
	Temp.SetScale(m_fScale);
	Temp.SetTranslate(m_Pos);
	return Temp;
}
VSTransform VSAnimAtom::VSEngineFrom3DMax(const VSAnimAtom & AtomIn3DMax)
{
	VSTransform t = AtomIn3DMax.GetTransfrom();
	Matrix3X3W Combine = t.GetCombine();
	Matrix3X3W Trans3DMaxToVSEngine(	1.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 1.0f, 0.0f,
										0.0f, 1.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f);
	Matrix3X3W VSEngineMat = Trans3DMaxToVSEngine * Combine * Trans3DMaxToVSEngine;
	VSTransform Result;
	Result.SetMatrix(VSEngineMat);
	return Result;
}
void VSAnimAtom::Interpolation(const VSAnimAtom & Atom1 , const VSAnimAtom Atom2,float t)
{
#ifdef DEBUG
	if (t < 0.0f)
	{
		t = 0.0f;
		VSMAC_ASSERT(0);
	}
	if (t > 1.0f)
	{
		t = 1.0f;
		VSMAC_ASSERT(0);
	}
#endif // DEBUG

	m_fScale = LineInterpolation(Atom1.m_fScale,Atom2.m_fScale,t);
	m_Pos = LineInterpolation(Atom1.m_Pos,Atom2.m_Pos,t);
	m_Rotator.Slerp(t,Atom1.m_Rotator,Atom2.m_Rotator);
}
void VSAnimAtom::ParitialBlend(const VSAnimAtom &Atom, float f)
{
	*this = Atom * f;
}
void VSAnimAtom::FastParitialBlend(const VSAnimAtom &Atom, float f)
{
	if (f > 0.0f)
	{	
		m_fScale += Atom.m_fScale * f;
		m_Pos += Atom.m_Pos * f;

		m_Rotator += Atom.m_Rotator * f;
	}		
}
void    VSAnimAtom::operator *= (float f)
{
	*this = *this * f;

}
void VSAnimAtom::operator += (const VSAnimAtom &Atom)
{
	*this = *this + Atom;
}
void VSAnimAtom::operator -= (const VSAnimAtom &Atom)
{
	m_Pos = m_Pos - Atom.m_Pos;
	m_fScale = m_fScale / Atom.m_fScale;
	m_Rotator = m_Rotator * Atom.m_Rotator.GetConjugate();
}
namespace zq
{
	VSAnimAtom FastParitialMul(const VSAnimAtom & Atom, float f)
	{
		if (f > 0.0f)
		{
			VSAnimAtom Temp;
			Temp.m_fScale = Atom.m_fScale * f;
			Temp.m_Pos = Atom.m_Pos * f;
			Temp.m_Rotator = Atom.m_Rotator * f;
			return Temp;
		}
		else
		{
			return VSAnimAtom::ms_FastBlendZero;
		}
	}
	VSAnimAtom operator +(const VSAnimAtom & Atom1, const VSAnimAtom Atom2)
	{
		VSAnimAtom Temp;
		Temp.m_Pos = Atom1.m_Pos + Atom2.m_Pos;
		Temp.m_fScale = Atom1.m_fScale * Atom2.m_fScale;
		Temp.m_Rotator = Atom1.m_Rotator * Atom2.m_Rotator;
		return Temp;
	}
	VSAnimAtom operator -(const VSAnimAtom & Atom1, const VSAnimAtom Atom2)
	{
		VSAnimAtom Temp;
		Temp.m_Pos = Atom1.m_Pos - Atom2.m_Pos;
		Temp.m_fScale = Atom1.m_fScale / Atom2.m_fScale;
		Temp.m_Rotator = Atom1.m_Rotator * Atom2.m_Rotator.GetConjugate();
		return Temp;

	}
	VSAnimAtom operator *  (const VSAnimAtom & Atom, float f)
	{
		if (f > 0.0f)
		{
			VSAnimAtom Temp;
			Temp.m_fScale = Atom.m_fScale * f;
			Temp.m_Pos = Atom.m_Pos * f;
			Temp.m_Rotator = Atom.m_Rotator.Pow(f);
			return Temp;
		}
		else
		{
			return VSAnimAtom::ms_Identity;
		}
	}
}