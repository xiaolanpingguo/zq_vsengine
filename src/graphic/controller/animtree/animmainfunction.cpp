#include "graphic/controller/animtree/animmainfunction.h"
#include "graphic/node/model/bonenode.h"
#include "graphic/controller/animtree/animfunction.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSAnimMainFunction,VSAnimBaseFunction)
BEGIN_ADD_PROPERTY(VSAnimMainFunction,VSAnimBaseFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSAnimMainFunction)
IMPLEMENT_INITIAL_END
VSAnimMainFunction::VSAnimMainFunction(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSAnimBaseFunction(ShowName,pAnimTree)
{
	VSString InputName = _T("Anim");
	VSInputNode * pInputNode = NULL;

	pInputNode = MEM_NEW VSInputNode(VSPutNode::AVT_ANIM,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

	InputName = _T("IK");
	pInputNode = NULL;

	pInputNode = MEM_NEW VSInputNode(VSPutNode::AVT_IK,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
}
VSAnimMainFunction::VSAnimMainFunction()
{

}
VSAnimMainFunction::~VSAnimMainFunction()
{
	
}
bool VSAnimMainFunction::Update(double dAppTime)
{
	if (!VSAnimBaseFunction::Update(dAppTime))
		return false;

	const VSSkeletonMeshNode * pMesh = GetSkeletonMeshNode();
	VSMAC_ASSERT(pMesh);

	VSSkeleton * pSkeleton = pMesh->GetSkeleton();
	VSMAC_ASSERT(pSkeleton);

	if(m_pInput[0]->GetOutputLink())
	{
		VSAnimFunction *pAnimFunction = (VSAnimFunction *)m_pInput[0]->GetOutputLink()->GetOwner();
		if(pAnimFunction)
		{
			pAnimFunction->Update(dAppTime);
			if (!m_bOnlyUpdateRootMotion)
			{
				for (uint32 i = 0; i < pSkeleton->GetBoneNum(); i++)
				{
					VSBoneNode * pBone = pSkeleton->GetBoneNode(i);
					if (pBone)
					{
						Matrix3X3W Mat;
						pAnimFunction->m_BoneOutput[i].GetMatrix(Mat);
						pBone->SetLocalMat(Mat);
					}
				}
			}
			m_bHaveRootMotion = pAnimFunction->m_bHaveRootMotion;
			m_AnimDataInstance = pAnimFunction->m_AnimDataInstance;
		}
	}
	
	if(m_pInput[1]->GetOutputLink())
	{
		VSAnimFunction *pAnimFunction = (VSAnimFunction *)m_pInput[1]->GetOutputLink()->GetOwner();
		if(pAnimFunction)
		{
			pAnimFunction->Update(dAppTime);
			
		}
	}
	return true;

}
bool VSAnimMainFunction::IsSupportSimpleInstance()
{
	if (m_pInput[0]->GetOutputLink())
	{
		VSAnimFunction *pAnimFunction = (VSAnimFunction *)m_pInput[0]->GetOutputLink()->GetOwner();
		if (pAnimFunction)
		{
			return pAnimFunction->IsSupportSimpleInstance();
		}
	}

	return false;
}
const VSAnimAtom &VSAnimMainFunction::GetRootDelta()
{
	if (m_pInput[0]->GetOutputLink())
	{
		VSAnimFunction *pAnimFunction = (VSAnimFunction *)m_pInput[0]->GetOutputLink()->GetOwner();
		if (pAnimFunction && m_bHaveRootMotion)
		{
			return pAnimFunction->m_RootAtom;
			
		}
		else
		{
			VSAnimAtom::ms_Identity;
		}
	}
	return VSAnimAtom::ms_Identity;
}
// void VSAnimMainFunction::CreateNode(const VSString & NodeName,uint32 uiNodeType)
// {
// 	if (uiNodeType > VSPutNode::AVT_ANIM && uiNodeType < VSPutNode::AVT_MAX)
// 	{
// 		VSString InputName = NodeName;
// 		VSInputNode * pInputNode = NULL;
// 
// 		pInputNode = MEM_NEW VSInputNode(uiNodeType,InputName,this);
// 		VSMAC_ASSERT(pInputNode);
// 		m_pInput.AddElement(pInputNode);
// 	}
// 
// }