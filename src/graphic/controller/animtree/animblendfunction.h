#pragma once


#include "graphic/controller/animtree/animfunction.h"


namespace zq{


class VSStream;
class GRAPHIC_API VSAnimBlendFunction : public VSAnimFunction
{
	DECLARE_RTTI;
	DECLARE_INITIAL_NO_CLASS_FACTORY

public:
		
	virtual ~VSAnimBlendFunction() = 0;
	virtual bool Update(double dAppTime);

	VSAnimBlendFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree);

		
	static void LineBlendTwoAll(VSAnimFunction * pOut,
		VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, float fWeight);

		

	static void AdditiveBlend(VSAnimFunction * pOut, VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2);
protected:
	VSAnimBlendFunction();
	virtual bool ComputePara(double dAppTime) = 0;
	virtual bool ComputeOutBoneMatrix(double dAppTime) = 0;

private:
	static void LineBlendTwo(VSAnimFunction * pOut,
		VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, float fWeight);

	static void LineBlendTwoRoot(VSAnimFunction * pOut,
		VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, float fWeight);

};
DECLARE_Ptr(VSAnimBlendFunction);
VSTYPE_MARCO(VSAnimBlendFunction);

}
