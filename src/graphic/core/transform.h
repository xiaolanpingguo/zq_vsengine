#pragma once


#include "math/vector3.h"
#include "math/matrix3X3w.h"
#include "math/othermath.h"
#include "graphic/core/graphic.h"


namespace zq{


class VSStream;
class GRAPHIC_API VSTransform
{
public:
	
	VSTransform();
	~VSTransform();

    enum //Transform type
    {
        TT_POS,
        TT_DIR,
        TT_MAX
    };

    enum //Transform Flag
    {
        TF_SCALE = 1,
        TF_ROTATE = 2,
        TF_TRANSFORM = 4,
        TF_ALL = TF_SCALE | TF_TRANSFORM | TF_ROTATE
    };

    inline const Matrix3X3W& GetCombine()
    {
        Combine();
        return m_mCombine;
    }

    inline const Matrix3X3W& GetCombineInverse()
    {
        Combine();
        if (!m_bIsHaveInverse)
        {
            m_bIsHaveInverse = 1;
            m_mCombineInverse.InverseOf(m_mCombine);
        }

        return m_mCombineInverse;
    }

    inline const Vector3& GetScale()const
    {
        return m_fScale;
    }

    inline const Vector3& GetTranslate()const
    {
        return m_vTranslate;
    }

    inline const Matrix3X3& GetRotate()const
    {
        return m_mRotate;
    }

    inline void SetScale(const Vector3& fScale)
    {
        m_fScale = fScale;
        m_bIsCombine = 0;
        m_bIsHaveInverse = 0;
    }

    inline void SetTranslate(const Vector3& Translate)
    {
        m_vTranslate = Translate;
        m_bIsCombine = 0;
        m_bIsHaveInverse = 0;
    }

    inline void SetRotate(const Matrix3X3 & Rotate)
    {
        m_mRotate = Rotate;
        m_bIsCombine = 0;
        m_bIsHaveInverse = 0;
    }

    void GetDir(Vector3 &Dir, Vector3 &Up, Vector3 & Right)const
    {
        Right.x = m_mRotate._00;
        Right.y = m_mRotate._01;
        Right.z = m_mRotate._02;

        Up.x = m_mRotate._10;
        Up.y = m_mRotate._11;
        Up.z = m_mRotate._12;

        Dir.x = m_mRotate._20;
        Dir.y = m_mRotate._21;
        Dir.z = m_mRotate._22;
    }
	
    void ApplyForward(const Vector3 & In, Vector3 & Out, uint32 uiTransformType);
	void Product(const VSTransform & t1,const VSTransform &t2,uint32 TransformFlag = TF_ALL);
	void Inverse(VSTransform & Out)const;
	void ApplyInverse(const Vector3 & In, Vector3 & Out,uint32 uiTransformType);	
	void SetMatrix(const Matrix3X3W & VSMat);
	void Interpolation(const VSTransform &t1 , const VSTransform &t2,float t);
	void Indetity();
	const static VSTransform ms_Indetity;

private:
    Matrix3X3W m_mCombine;
    Matrix3X3W m_mCombineInverse;
    void Combine();
    Vector3 m_fScale;
    Matrix3X3 m_mRotate;
    Vector3 m_vTranslate;
    bool m_bIsCombine;
    bool m_bIsHaveInverse;
};

};
