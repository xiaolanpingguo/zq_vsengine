#pragma once
#include "graphic/controller/animtree/oneparamsmoothanimblend.h"
#include "graphic/controller/animtree/animsequencefunc.h"
namespace zq
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) 是动画0 [1,2) 是动画1 ，以此类推
	0		1		2


	*/
	class VSStream;
	class GRAPHIC_API VSOneParamSmoothAnimBlendSequence : public VSOneParamSmoothAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~VSOneParamSmoothAnimBlendSequence();

		VSOneParamSmoothAnimBlendSequence(const VSUsedName & ShowName, VSAnimTree * pAnimTree);

	public:

		VSOneParamSmoothAnimBlendSequence();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		void CreateSlot(uint32 uiWidth);
		void SetAnim(uint32 i,const VSUsedName &AnimName);

		void SetAnimFrequency(uint32 Index,double Frequency);
		void SetAnimPhase(uint32 Index,double Phase);
		virtual bool SetObject(VSObject * pObject);
		virtual bool IsLeafNode(){ return true; }
		virtual void SetOnlyUpdateRootMotion(bool bOnlyUpdateRootMotion)
		{
			for (uint32 i = 0; i < m_AnimSequenceFuncArray.GetNum(); i++)
			{
				if (m_AnimSequenceFuncArray[i])
				{
					m_AnimSequenceFuncArray[i]->SetOnlyUpdateRootMotion(bOnlyUpdateRootMotion);
				}
			}
			
		}
		virtual bool IsSupportSimpleInstance()
		{
			for (uint32 i = 0; i < m_AnimSequenceFuncArray.GetNum(); i++)
			{
				if (m_AnimSequenceFuncArray[i])
				{
					if (!m_AnimSequenceFuncArray[i]->IsSupportSimpleInstance())
					{
						return false;
					}
				}
			}

			return true;
		}
	protected:
		VSArray<VSAnimSequenceFuncPtr> m_AnimSequenceFuncArray;
	};
	DECLARE_Ptr(VSOneParamSmoothAnimBlendSequence);
	VSTYPE_MARCO(VSOneParamSmoothAnimBlendSequence);
}
