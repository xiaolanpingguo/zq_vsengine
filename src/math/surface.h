#pragma once


#include "math/vector3.h"


namespace zq{


class MATH_API Surface : public MemObject
{
public:
	Surface();
	virtual ~Surface() = 0;


    inline Vector3 GetN(float U, float V)
    {
        Vector3 N;
        N.Cross(GetUPartialDerivative(U, V), GetVPartialDerivative(U, V));
        return N;
    }


	//求点
	virtual Vector3 GetPoint(float U,float V) = 0;
	//求偏导
	virtual Vector3 GetUPartialDerivative(float U,float V) = 0;
	virtual Vector3 GetVPartialDerivative(float U,float V) = 0;
};


}