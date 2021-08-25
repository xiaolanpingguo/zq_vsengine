#pragma once
#include "graphic/controller/animtree/oneparamanimblend.h"
namespace zq
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) �Ƕ���0 [1,2) �Ƕ���1 ���Դ�����
	0		1		2


	*/
	class VSStream;
	class GRAPHIC_API VSOneParamSmoothAnimBlend : public VSOneParamAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~VSOneParamSmoothAnimBlend();

		VSOneParamSmoothAnimBlend(const VSUsedName & ShowName, VSAnimTree * pAnimTree);

	public:

		VSOneParamSmoothAnimBlend();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
	};
	DECLARE_Ptr(VSOneParamSmoothAnimBlend);
	VSTYPE_MARCO(VSOneParamSmoothAnimBlend);
}
