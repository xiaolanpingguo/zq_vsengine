#pragma once


#include "math/controlsurface.h"
#include "math/vector2.h"


namespace zq{


class MATH_API SurfaceSubdivision
{
public:
	enum  //Subdivision Method
	{
		SM_UNIFORM,
		SM_LENGTH,
		SM_MAX
	};
	SurfaceSubdivision();
	virtual ~SurfaceSubdivision() = 0;
	virtual bool Subdivision() = 0;
	virtual uint32 GetSubdivisionMethod() = 0;


    inline bool SetSurface(Surface * pSurface)
    {
        if (!pSurface)
            return 0;
        m_pSurface = pSurface;
        return 1;
    }

    inline Vector3* GetPoints() const
    {
        return m_pPoint;
    }

    inline uint32 GetRow() const
    {
        return m_uiRow;
    }

    inline uint32 GetColumn() const
    {

        return m_uiColumn;
    }

    inline Vector2* GetUV() const
    {
        return m_pUV;
    }

protected:
	Surface * m_pSurface;
	Vector3 *m_pPoint;
	uint32 m_uiRow;
	uint32 m_uiColumn;
	float	m_fU1,m_fU2;
	float	m_fV1,m_fV2;
	Vector2 * m_pUV;
};





//----------------------------SurfaceSubdivisionByLength-----------------
class MATH_API SurfaceSubdivisionByLength : public SurfaceSubdivision
{
public:
    SurfaceSubdivisionByLength(uint32 Row, uint32 Column, float U1 = 0.0f, float U2 = 1.0f, float V1 = 0.0f, float V2 = 1.0f, uint32 uiIterations = 32);
    virtual ~SurfaceSubdivisionByLength();
    virtual bool Subdivision();
    virtual uint32 GetSubdivisionMethod() { return SM_LENGTH; }
protected:
    uint32 m_uiIterations;
};




//----------------------------SurfaceSubdivisionByLength-----------------
class MATH_API SurfaceSubdivisionByUniform : public SurfaceSubdivision
{
public:
    SurfaceSubdivisionByUniform(uint32 uiRow, uint32 uiColumn, float U1 = 0.0f, float U2 = 1.0f, float V1 = 0.0f, float V2 = 1.0f);
    virtual ~SurfaceSubdivisionByUniform();
    virtual bool Subdivision();
    virtual uint32 GetSubdivisionMethod() { return SM_UNIFORM; }
};

}