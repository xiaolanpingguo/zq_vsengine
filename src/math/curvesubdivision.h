#pragma once


#include "math/controlcurve.h"


namespace zq{


class MATH_API CurveSubdivision : public MemObject
{
public:
	enum	//Subdivision Method
	{
		SM_ERROR,
		SM_LENGTH,
		SM_UNIFORM,
		SM_MAX
	};
	CurveSubdivision();
	virtual ~CurveSubdivision() = 0;

    inline Vector3* GetPoints()const
    {
        return m_pPoint;
    }

    inline uint32 GetPointNum()const
    {
        return m_uiPointNum;
    }

    inline ControlCurve* GetCurve()const
    {
        return m_pCurve;
    }

    inline float* GetT()const
    {
        return m_pT;
    }

	//��������
	bool SetSubCurve(ControlCurve * pCurve);
	//ָ�����ߵ�ĳ�����Ƶ�仯�����������л��ֺ�ĵ�
	bool ControlPointChange(uint32 i);
	//��������
	virtual bool Subdivision() = 0;

	virtual uint32 GetSubdivisionMethod() = 0;
protected:
	Vector3 *m_pPoint;
	uint32 m_uiPointNum;
	ControlCurve *m_pCurve;
	float			*m_pT;
	float m_ft1;
	float m_ft2;
};




// --------------CurveSubdivisionByError---------------
class MATH_API CurveSubdivisionByError :public CurveSubdivision
{
public:
    CurveSubdivisionByError(float t1 = 0.0f, float t2 = 1.0f, uint32 Level = 5, float error = EPSILON_E4);
    ~CurveSubdivisionByError();
    //ָ��ݹ��κ���� ��������t1,t2֮��
    virtual bool Subdivision();


    virtual uint32 GetSubdivisionMethod() { return SM_ERROR; }
protected:
    struct  POINTLIST
    {
        Vector3 Point;
        POINTLIST * next;
        float	  t;
    };
    POINTLIST * m_pPointHead;
    POINTLIST * m_pNext;
    bool SubdivisionBisect(uint32 Level, float error, float t1, float t2,
        const Vector3 &Point1, const Vector3 &Point2);
    void Clear();
    uint32 m_uiLevel;
    float m_fError;
};




// --------------CurveSubdivisionByLength---------------
class MATH_API CurveSubdivisionByLength :public CurveSubdivision
{
public:
    CurveSubdivisionByLength(uint32 PointNum, float t1, float t2, uint32 uiIterations = 32);
    virtual ~CurveSubdivisionByLength();
    //�������ֵ�����͵����������������߰�����l1,l2֮��
    virtual bool Subdivision();
    void SetPointNum(uint32 uiPointNum);
    virtual uint32 GetSubdivisionMethod() { return SM_LENGTH; }
protected:
    uint32 m_uiIterations;
};





// --------------CurveSubdivisionByUniform---------------
class MATH_API CurveSubdivisionByUniform :public CurveSubdivision
{
public:
    CurveSubdivisionByUniform(uint32 PointNum, float t1 = 0.0f, float t2 = 1.0f);
    virtual ~CurveSubdivisionByUniform();
    //������ĸ����������� t1,t2֮��
    virtual bool Subdivision();
    void SetPointNum(uint32 uiPointNum);
    virtual uint32 GetSubdivisionMethod() { return SM_UNIFORM; }
};



}