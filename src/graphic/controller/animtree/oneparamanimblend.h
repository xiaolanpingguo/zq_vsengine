#pragma once
#include "graphic/controller/animtree/animblendfunction.h"
namespace zq
{
	/*
		CrossFading IMMEDIATE|______|______|  [0,1) 是动画0 [1,2) 是动画1 ，以此类推
							 0		1		2


	*/
	class VSStream;
	class GRAPHIC_API VSOneParamAnimBlend : public VSAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		

	public:
		virtual ~VSOneParamAnimBlend();

		VSOneParamAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree);

		virtual void AddInputNode();
		virtual void DeleteInputNode();
	public:
		float m_fParam;
		float m_fParamMax;
		float m_fParamMin;
		
		VSOneParamAnimBlend();
		virtual bool ComputePara(double dAppTime);
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		virtual void SetPara(void * pPara)
		{
			m_fParam = *((float *)pPara);
		}
	};
	DECLARE_Ptr(VSOneParamAnimBlend);
	VSTYPE_MARCO(VSOneParamAnimBlend);
}

