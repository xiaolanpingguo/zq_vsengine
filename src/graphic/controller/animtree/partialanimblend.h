#pragma once
#include "graphic/controller/animtree/animblendfunction.h"
namespace zq
{
	/*
	CrossFading IMMEDIATE |______|______|  [0,1) 是动画0 [1,2) 是动画1 ，以此类推
	0		1		2


	*/
	class VSStream;
	class GRAPHIC_API VSPartialAnimBlend : public VSAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		

	public:
		virtual ~VSPartialAnimBlend();

		VSPartialAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree);

		virtual void AddInputNode();

		virtual void DeleteInputNode();

		virtual void SetBoneWeight(uint32 i, const VSUsedName & BoneName,float Weight);

		virtual bool ComputePara(double dAppTime);
		void AddControllBone(const VSUsedName & BoneName);
	protected:
#if (FAST_BLEND_PARIAL_ANIMITION == 0)
		VSArray<Matrix3X3W> m_FastTempMatrix;
		Matrix3X3W m_FastRootMatrix;
#endif
		VSArray<VSArray<float>> m_Weight;
		VSMap<VSUsedName, VSArray<float>> m_BoneWeight;
		void ComputeWeight();
		virtual bool IsSupportSimpleInstance() { return false; }
	public:

		VSPartialAnimBlend();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		virtual bool SetObject(VSObject * pObject);

	};
	DECLARE_Ptr(VSPartialAnimBlend);
	VSTYPE_MARCO(VSPartialAnimBlend);
}
