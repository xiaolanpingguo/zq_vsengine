#include "graphic/controller/animtree/oneparamsmoothanimblend.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSOneParamSmoothAnimBlend, VSOneParamAnimBlend)
BEGIN_ADD_PROPERTY(VSOneParamSmoothAnimBlend, VSOneParamAnimBlend)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOneParamSmoothAnimBlend)
IMPLEMENT_INITIAL_END
VSOneParamSmoothAnimBlend::~VSOneParamSmoothAnimBlend()
{

}
VSOneParamSmoothAnimBlend::VSOneParamSmoothAnimBlend()
{

}
VSOneParamSmoothAnimBlend::VSOneParamSmoothAnimBlend(const VSUsedName & ShowName, VSAnimTree * pAnimTree)
:VSOneParamAnimBlend(ShowName, pAnimTree)
{


}

bool VSOneParamSmoothAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{
	float fInternal = m_fParamMax - m_fParamMin;
	float fInternalSeg = fInternal / (m_pInput.GetNum() - 1);
	if (fInternalSeg < EPSILON_E4)
	{
		return 0;
	}
	uint32 uiIndex1 = 0;
	uint32 uiIndex2 = 0;

	float fTemp = (m_fParam - m_fParamMin) / fInternalSeg;
	uiIndex1 = (unsigned int)fTemp;

	if (uiIndex1 >= m_pInput.GetNum() - 1)
	{
		uiIndex2 = uiIndex1;
	}
	else
	{
		uiIndex2 = uiIndex1 + 1;
	}


	float fWeight = fTemp - (float)uiIndex1;


	VSInputNode* pInputNode1 = GetInputNode(uiIndex1);
	VSInputNode* pInputNode2 = GetInputNode(uiIndex2);
	if (pInputNode1->GetOutputLink() && pInputNode2->GetOutputLink())
	{
		VSAnimFunction *pAnimBaseFunction1 = (VSAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
		VSAnimFunction *pAnimBaseFunction2 = (VSAnimFunction *)pInputNode2->GetOutputLink()->GetOwner();

		LineBlendTwoAll(this, pAnimBaseFunction1, pAnimBaseFunction2, fWeight);

	}
	else if (pInputNode1->GetOutputLink())
	{
		VSAnimFunction *pAnimBaseFunction1 = (VSAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
		LineBlendTwoAll(this, pAnimBaseFunction1, NULL, 0.0f);

	}
	else if (pInputNode2->GetOutputLink())
	{
		VSAnimFunction *pAnimBaseFunction2 = (VSAnimFunction *)pInputNode2->GetOutputLink()->GetOwner();

		LineBlendTwoAll(this, NULL, pAnimBaseFunction2, 0.0f);

	}
	return 1;
}
