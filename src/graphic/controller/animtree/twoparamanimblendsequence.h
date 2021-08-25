#pragma once
#include "graphic/controller/animtree/twoparamanimblend.h"
#include "graphic/controller/animtree/animsequencefunc.h"
namespace zq
{
	/*

		ParaMin[0]			width					ParaMax[0]

		j0				j1				j3				j4
	i0	-------------------------------------------------	ParaMin[1]
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
	i1	-------------------------------------------------	 height
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
	i2	-------------------------------------------------	ParaMax[1]


	*/
	class VSStream;
	class GRAPHIC_API VSTwoParamAnimBlendSequence : public VSTwoParamAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~VSTwoParamAnimBlendSequence();

		VSTwoParamAnimBlendSequence(const VSUsedName & ShowName, VSAnimTree * pAnimTree);

	public:

		VSTwoParamAnimBlendSequence();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		void CreateSlot(uint32 uiWidth, uint32 uiHeight);
		void SetAnim(uint32 i,uint32 j ,const VSUsedName &AnimName);


		void SetAnimFrequency(uint32 i, uint32 j, double Frequency);
		void SetAnimPhase(uint32 i, uint32 j, double Phase);
		virtual bool IsLeafNode(){ return true; }
		virtual bool SetObject(VSObject * pObject);
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
		
		
		VSAnimSequenceFunc * GetAnimSequenceFunction(uint32 i, uint32 j);
	};
	DECLARE_Ptr(VSTwoParamAnimBlendSequence);
	VSTYPE_MARCO(VSTwoParamAnimBlendSequence);
}
