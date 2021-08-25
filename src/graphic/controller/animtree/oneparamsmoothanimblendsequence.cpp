#include "graphic/controller/animtree/oneparamsmoothanimblendsequence.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSOneParamSmoothAnimBlendSequence, VSOneParamSmoothAnimBlend)
BEGIN_ADD_PROPERTY(VSOneParamSmoothAnimBlendSequence, VSOneParamSmoothAnimBlend)
REGISTER_PROPERTY(m_AnimSequenceFuncArray, AnimSequenceFuncArray, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOneParamSmoothAnimBlendSequence)
IMPLEMENT_INITIAL_END
VSOneParamSmoothAnimBlendSequence::~VSOneParamSmoothAnimBlendSequence()
{

}
VSOneParamSmoothAnimBlendSequence::VSOneParamSmoothAnimBlendSequence()
{

}
VSOneParamSmoothAnimBlendSequence::VSOneParamSmoothAnimBlendSequence(const VSUsedName & ShowName, VSAnimTree * pAnimTree)
:VSOneParamSmoothAnimBlend(ShowName, pAnimTree)
{
	//no need input
	for (uint32 i = 0; i < m_pInput.GetNum(); i++)
	{
		SAFE_DELETE(m_pInput[i]);
	}
	m_pInput.Clear();

}
void VSOneParamSmoothAnimBlendSequence::SetAnimFrequency(uint32 Index, double Frequency)
{
	if (Index >= m_AnimSequenceFuncArray.GetNum())
	{
		return ;
	}
	m_AnimSequenceFuncArray[Index]->m_dFrequency = Frequency;
}
void VSOneParamSmoothAnimBlendSequence::SetAnimPhase(uint32 Index, double Phase)
{
	if (Index >= m_AnimSequenceFuncArray.GetNum())
	{
		return;
	}
	m_AnimSequenceFuncArray[Index]->m_dPhase = Phase;
}
bool VSOneParamSmoothAnimBlendSequence::ComputeOutBoneMatrix(double dAppTime)
{
	if (m_AnimSequenceFuncArray.GetNum() == 0)
	{
		return false;
	}
	uint32 Index1 = 0;
	uint32 Index2 = 0;
	float fBlendWeight = 0.0f;
	if (m_AnimSequenceFuncArray.GetNum() > 1)
	{
		float fInternal = m_fParamMax - m_fParamMin;
		float fInternalSeg = fInternal / (m_AnimSequenceFuncArray.GetNum() - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		float fTemp = (m_fParam - m_fParamMin) / fInternalSeg;
		Index1 = (unsigned int)fTemp;
		Index2 = Index1 + 1;
		fBlendWeight = fTemp - (float)Index1;


		if (Index2 >= m_AnimSequenceFuncArray.GetNum())
		{
			Index2 = m_AnimSequenceFuncArray.GetNum() - 1;
		}

	}

	float AnimTime1 = m_AnimSequenceFuncArray[Index1]->GetAnimTime();
	float AnimTime2 = m_AnimSequenceFuncArray[Index2]->GetAnimTime();
	float BlendTime = AnimTime1 + (AnimTime2 - AnimTime1) * fBlendWeight;
	float AnimTimeScale1 = AnimTime1 / BlendTime;
	float AnimTimeScale2 = AnimTime2 / BlendTime;
	m_AnimSequenceFuncArray[Index1]->m_fInnerTimeScale = AnimTimeScale1;
	m_AnimSequenceFuncArray[Index2]->m_fInnerTimeScale = AnimTimeScale2;

	m_AnimSequenceFuncArray[Index1]->ClearFlag();
	m_AnimSequenceFuncArray[Index2]->ClearFlag();
	m_AnimSequenceFuncArray[Index1]->Update(dAppTime);
	m_AnimSequenceFuncArray[Index2]->Update(dAppTime);

	m_AnimSequenceFuncArray[Index1]->m_fInnerTimeScale = 1.0f;
	m_AnimSequenceFuncArray[Index2]->m_fInnerTimeScale = 1.0f;

	VSAnimSequenceFunc* pAnimBaseFunction1 = m_AnimSequenceFuncArray[Index1];
	VSAnimSequenceFunc* pAnimBaseFunction2 = m_AnimSequenceFuncArray[Index2];

	LineBlendTwoAll(this, pAnimBaseFunction1, pAnimBaseFunction2, fBlendWeight);	
	return 1;
}
void VSOneParamSmoothAnimBlendSequence::CreateSlot(uint32 uiWidth)
{
	m_AnimSequenceFuncArray.Clear();
	for (uint32 i = 0; i < uiWidth; i++)
	{
		VSAnimSequenceFunc * pAnimSequenceFun = pAnimSequenceFun = MEM_NEW VSAnimSequenceFunc();
		m_AnimSequenceFuncArray.AddElement(pAnimSequenceFun);
	}
}
void VSOneParamSmoothAnimBlendSequence::SetAnim(uint32 i, const VSUsedName &AnimName)
{
	if (i >= m_AnimSequenceFuncArray.GetNum())
	{
		return;
	}
	m_AnimSequenceFuncArray[i]->SetAnim(AnimName);
	m_bStart = false;
	for (uint32 i = 0; i < m_AnimSequenceFuncArray.GetNum(); i++)
	{
		if (m_AnimSequenceFuncArray[i])
		{
			m_AnimSequenceFuncArray[i]->m_bStart = false;
		}
	}
}
bool VSOneParamSmoothAnimBlendSequence::SetObject(VSObject * pObject)
{
	if (VSOneParamSmoothAnimBlend::SetObject(pObject))
	{
		for (uint32 i = 0; i < m_AnimSequenceFuncArray.GetNum(); i++)
		{
			if (m_AnimSequenceFuncArray[i])
			{
				m_AnimSequenceFuncArray[i]->SetObject(pObject);
			}
		}
		return true;
	}

	return false;
}