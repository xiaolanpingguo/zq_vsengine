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

	//设置曲线
	bool SetSubCurve(ControlCurve * pCurve);
	//指定曲线的某个控制点变化，重新求所有划分后的点
	bool ControlPointChange(uint32 i);
	//划分曲线
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
    //指点递归层次和误差 划分曲线t1,t2之间
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
    //给定划分点个数和迭代次数，划分曲线按长度l1,l2之间
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
    //给定点的个数划分曲线 t1,t2之间
    virtual bool Subdivision();
    void SetPointNum(uint32 uiPointNum);
    virtual uint32 GetSubdivisionMethod() { return SM_UNIFORM; }
};



}