#pragma once


#include "math/matrix3X3w.h"
#include "graphic/core/graphic.h"
#include "graphic/core/transform.h"


namespace zq{


	class GRAPHIC_API VSAnimAtom
	{
	public:
		VSAnimAtom(const Vector3& fScale,const Vector3& Pos,const Quat& Rotator)
			:m_fScale(fScale),m_Pos(Pos),m_Rotator(Rotator)
		{

		}
		VSAnimAtom()
		{
			Identity();
		}
		~VSAnimAtom()
		{
			
		}
		Vector3	m_fScale;
		Vector3	m_Pos;
		Quat		m_Rotator;
		void GetMatrix(Matrix3X3W & OutMatrix)const;
		void Interpolation(const VSAnimAtom & Atom1 , const VSAnimAtom Atom2,float t);
		void Identity();
		void FromTransform(const VSTransform &T);
		void FromMatrix(const Matrix3X3W & m);
		void ParitialBlend(const VSAnimAtom &Atom, float f);
		void FastParitialBlend(const VSAnimAtom &Atom, float f);
		

		void    operator *= (float f);
		void    operator += (const VSAnimAtom &Atom);
		void    operator -= (const VSAnimAtom &Atom);
		
		static VSAnimAtom ms_Identity;
		static VSAnimAtom ms_FastBlendZero;
		static VSTransform VSEngineFrom3DMax(const VSAnimAtom & AtomIn3DMax);
		VSTransform GetTransfrom()const;
	};
	GRAPHIC_API VSAnimAtom operator +(const VSAnimAtom & Atom1, const VSAnimAtom Atom2);
	GRAPHIC_API VSAnimAtom operator -(const VSAnimAtom & Atom1, const VSAnimAtom Atom2);
	GRAPHIC_API VSAnimAtom operator *  (const VSAnimAtom & Atom,float f);
	GRAPHIC_API VSAnimAtom FastParitialMul(const VSAnimAtom & Atom, float f);
}
