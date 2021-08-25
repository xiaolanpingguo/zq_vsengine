#include "graphic/controller/animtree/twoparamanimblendsequence.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSTwoParamAnimBlendSequence, VSTwoParamAnimBlend)
BEGIN_ADD_PROPERTY(VSTwoParamAnimBlendSequence, VSTwoParamAnimBlend)
REGISTER_PROPERTY(m_AnimSequenceFuncArray, AnimSequenceFuncArray, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSTwoParamAnimBlendSequence)
IMPLEMENT_INITIAL_END
VSTwoParamAnimBlendSequence::~VSTwoParamAnimBlendSequence()
{

}
VSTwoParamAnimBlendSequence::VSTwoParamAnimBlendSequence()
{

	
}
VSTwoParamAnimBlendSequence::VSTwoParamAnimBlendSequence(const VSUsedName & ShowName, VSAnimTree * pAnimTree)
:VSTwoParamAnimBlend(ShowName, pAnimTree)
{
	//no need input
	m_pInput.Clear();
	
	
}
void VSTwoParamAnimBlendSequence::SetAnimFrequency(uint32 i, uint32 j, double Frequency)
{
	VSMAC_ASSERT(j < m_uiWidth || i < m_uiHeight);

	m_AnimSequenceFuncArray[i * m_uiWidth + j]->m_dFrequency = Frequency;
}
void VSTwoParamAnimBlendSequence::SetAnimPhase(uint32 i, uint32 j, double Phase)
{
	VSMAC_ASSERT(j < m_uiWidth || i < m_uiHeight);
	m_AnimSequenceFuncArray[i * m_uiWidth + j]->m_dPhase = Phase;
}
bool VSTwoParamAnimBlendSequence::ComputeOutBoneMatrix(double dAppTime)
{
	if (m_AnimSequenceFuncArray.GetNum() == 0)
	{
		return false;
	}

	uint32 Index1[2] = { 0 ,0};
	uint32 Index2[2] = { 0, 0 };
	float fWeight[2] = {0.0f,0.0f};
	if (m_uiWidth > 1)
	{
		float fInternal = m_fParamMax[0] - m_fParamMin[0];
		float fInternalSeg = fInternal / (m_uiWidth - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		float fTemp = (m_fParam[0] - m_fParamMin[0]) / fInternalSeg;
		Index1[0] = (unsigned int)fTemp;
		Index2[0] = Index1[0] + 1;
		fWeight[0] = fTemp - (float)Index1[0];
	}
	if (m_uiHeight > 1)
	{
		float fInternal = m_fParamMax[1] - m_fParamMin[1];
		float fInternalSeg = fInternal / (m_uiHeight - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		float fTemp = (m_fParam[1] - m_fParamMin[1]) / fInternalSeg;
		Index1[1] = (unsigned int)fTemp;
		Index2[1] = Index1[1] + 1;
		fWeight[1] = fTemp - (float)Index1[1];
	}

	VSAnimSequenceFunc *pAnimBaseFunction1 = GetAnimSequenceFunction(Index1[1], Index1[0]);
	VSAnimSequenceFunc *pAnimBaseFunction2 = GetAnimSequenceFunction(Index1[1], Index2[0]);
	VSAnimSequenceFunc *pAnimBaseFunction3 = GetAnimSequenceFunction(Index2[1], Index1[0]);
	VSAnimSequenceFunc *pAnimBaseFunction4 = GetAnimSequenceFunction(Index2[1], Index2[0]);

	float AnimTime1 = pAnimBaseFunction1->GetAnimTime();
	float AnimTime2 = pAnimBaseFunction2->GetAnimTime();
	float BlendTime1 = AnimTime1 + (AnimTime2 - AnimTime1) * fWeight[0];

	float AnimTime3 = pAnimBaseFunction3->GetAnimTime();
	float AnimTime4 = pAnimBaseFunction4->GetAnimTime();
	float BlendTime2 = AnimTime3 + (AnimTime4 - AnimTime3) * fWeight[0];

	float BlendTime = BlendTime1 + (BlendTime2 - BlendTime1) * fWeight[1];

	float AnimTimeScale1 = AnimTime1 / BlendTime;
	float AnimTimeScale2 = AnimTime2 / BlendTime;
	float AnimTimeScale3 = AnimTime3 / BlendTime;
	float AnimTimeScale4 = AnimTime4 / BlendTime;

	pAnimBaseFunction1->m_fInnerTimeScale = AnimTimeScale1;
	pAnimBaseFunction2->m_fInnerTimeScale = AnimTimeScale2;
	pAnimBaseFunction3->m_fInnerTimeScale = AnimTimeScale3;
	pAnimBaseFunction4->m_fInnerTimeScale = AnimTimeScale4;

	pAnimBaseFunction1->ClearFlag();
	pAnimBaseFunction2->ClearFlag();
	pAnimBaseFunction3->ClearFlag();
	pAnimBaseFunction4->ClearFlag();

	pAnimBaseFunction1->Update(dAppTime);
	pAnimBaseFunction2->Update(dAppTime);
	pAnimBaseFunction3->Update(dAppTime);
	pAnimBaseFunction4->Update(dAppTime);



	pAnimBaseFunction1->m_fInnerTimeScale = 1.0f;
	pAnimBaseFunction2->m_fInnerTimeScale = 1.0f;
	pAnimBaseFunction3->m_fInnerTimeScale = 1.0f;
	pAnimBaseFunction4->m_fInnerTimeScale = 1.0f;

	LineBlendTwoAll(this, pAnimBaseFunction1,pAnimBaseFunction2,
		pAnimBaseFunction3, pAnimBaseFunction4, fWeight[0], fWeight[1]);
	return 1;
}
void VSTwoParamAnimBlendSequence::CreateSlot(uint32 uiWidth, uint32 uiHeight)
{
	m_AnimSequenceFuncArray.Clear();
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	for (uint32 i = 0; i < uiWidth;i++)
	{
		for (uint32 j = 0; j < uiHeight; j++)
		{
			VSAnimSequenceFunc * pAnimSequenceFun = MEM_NEW VSAnimSequenceFunc();
			m_AnimSequenceFuncArray.AddElement(pAnimSequenceFun);
		}
	}
}
void VSTwoParamAnimBlendSequence::SetAnim(uint32 i , uint32 j, const VSUsedName &AnimName)
{
	VSMAC_ASSERT(j < m_uiWidth || i < m_uiHeight);
	m_AnimSequenceFuncArray[i * m_uiWidth + j]->SetAnim(AnimName);
	m_bStart = false;
	for (uint32 i = 0; i < m_AnimSequenceFuncArray.GetNum(); i++)
	{
		if (m_AnimSequenceFuncArray[i])
		{
			m_AnimSequenceFuncArray[i]->m_bStart = false;
		}
	}
}

VSAnimSequenceFunc * VSTwoParamAnimBlendSequence::GetAnimSequenceFunction(uint32 i, uint32 j)
{
	return m_AnimSequenceFuncArray[i * m_uiWidth + j];
}
bool VSTwoParamAnimBlendSequence::SetObject(VSObject * pObject)
{
	if (VSTwoParamAnimBlend::SetObject(pObject))
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