#include "graphic/controller/animtree/partialanimblend.h"
#include "graphic/node/model/skeletonmeshnode.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/node/model/bonenode.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSPartialAnimBlend,VSAnimBlendFunction)
BEGIN_ADD_PROPERTY(VSPartialAnimBlend,VSAnimBlendFunction)
REGISTER_PROPERTY(m_BoneWeight, BoneWeight, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPartialAnimBlend)
IMPLEMENT_INITIAL_END
#define  FAST_BLEND_PARIAL_ANIMITION 2
VSPartialAnimBlend::~VSPartialAnimBlend()
{
	
	m_BoneWeight.Clear();
}
VSPartialAnimBlend::VSPartialAnimBlend()
{
	m_BoneWeight.Clear();
}
VSPartialAnimBlend::VSPartialAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree)
:VSAnimBlendFunction(ShowName,pAnimTree)
{
	
	VSString InputName0 = _T("Child0");
	VSInputNode * pInputNode = NULL;
	pInputNode = MEM_NEW VSInputNode(VSPutNode::AVT_ANIM,InputName0,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);



	VSString InputName1 = _T("Child1");
	pInputNode = NULL;
	pInputNode = MEM_NEW VSInputNode(VSPutNode::AVT_ANIM,InputName1,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


}
void VSPartialAnimBlend::AddInputNode()
{
	VSString InputName = _T("Child");
	VSString ID = IntToString(m_pInput.GetNum()); 
	InputName += ID;
	VSInputNode * pInputNode = NULL;
	pInputNode = MEM_NEW VSInputNode(VSPutNode::AVT_ANIM,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	for (uint32 i = 0; i < m_BoneWeight.GetNum(); i++)
	{
		m_BoneWeight[i].Value.AddElement(0.0f);
	}

}
void VSPartialAnimBlend::DeleteInputNode()
{
	if (m_pInput.GetNum() > 2)
	{
		VSInputNode * pInputNode = m_pInput[m_pInput.GetNum() - 1];

		m_pInput.Erase(m_pInput.GetNum() - 1);
		if (!pInputNode)
		{
			return ;
		}
		SAFE_DELETE(pInputNode);

		for (uint32 i = 0; i < m_BoneWeight.GetNum(); i++)
		{
			m_BoneWeight[i].Value.Erase(m_BoneWeight[i].Value.GetNum() - 1);
		}
	}
}
void VSPartialAnimBlend::AddControllBone(const VSUsedName & BoneName)
{
	VSArray<float> Temp;
	Temp.SetBufferNum(m_pInput.GetNum());
	for (uint32 i = 0; i < Temp.GetNum(); i++)
	{
		if (i == 0)
		{
			Temp[i] = 1.0f;
		}
		else
		{
			Temp[i] = 0.0f;
		}
	}
	m_BoneWeight.AddElement(BoneName, Temp);

}
void VSPartialAnimBlend::SetBoneWeight(uint32 i, const VSUsedName & BoneName, float Weight)
{
	uint32 uiIndex = m_BoneWeight.Find(BoneName);
	if (uiIndex != m_BoneWeight.GetNum())
	{
		if (i >= m_BoneWeight[uiIndex].Value.GetNum())
		{
			return;
		}
		m_BoneWeight[uiIndex].Value[i] = Weight;
	}
	
}
bool VSPartialAnimBlend::SetObject(VSObject * pObject)
{
	if (VSAnimFunction::SetObject(pObject))
	{
		ComputeWeight();
#if FAST_BLEND_PARIAL_ANIMITION > 1
		m_FastTempMatrix.SetBufferNum(m_BoneOutput.GetNum());
#endif
		return true;
	}

	return false;
}
bool VSPartialAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{

	if (m_Weight.GetNum() == 0)
	{
		VSMAC_ASSERT(0);
	}
	
	for ( uint32 i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		VSInputNode* pInputNode = GetInputNode(i);
		VSAnimFunction *pAnimBaseFunction = (VSAnimFunction *)pInputNode->GetOutputLink()->GetOwner();
		if (!pAnimBaseFunction)
		{
			return false;
		}
		m_bHaveRootMotion |= (pAnimBaseFunction->m_bHaveRootMotion && m_Weight[i][0] > 0.0f);
#if FAST_BLEND_PARIAL_ANIMITION == 0
		if (i == 0)
		{
			m_RootAtom = pAnimBaseFunction->m_RootAtom * m_Weight[i][0];
		}
		else
		{
			m_RootAtom += pAnimBaseFunction->m_RootAtom * m_Weight[i][0];
		}		
#elif FAST_BLEND_PARIAL_ANIMITION == 1
		if (i == 0)
		{
			m_RootAtom = FastParitialMul(pAnimBaseFunction->m_RootAtom , m_Weight[i][0]);
		}
		else
		{
			m_RootAtom.FastParitialBlend(pAnimBaseFunction->m_RootAtom , m_Weight[i][0]);
		}
#else
		if (i == 0)
		{
			pAnimBaseFunction->m_RootAtom.GetMatrix(m_FastRootMatrix);
			m_FastRootMatrix *= m_Weight[i][0];
		}
		else
		{
			Matrix3X3W Temp;
			pAnimBaseFunction->m_RootAtom.GetMatrix(Temp);
			m_FastRootMatrix += Temp * m_Weight[i][0];
	}
#endif
		if (m_bOnlyUpdateRootMotion)
		{
			continue;
		}
		for (uint32 j = 0; j < m_BoneOutput.GetNum(); j++)
		{

#if FAST_BLEND_PARIAL_ANIMITION == 0
			if (i == 0)
			{
				m_BoneOutput[j] = pAnimBaseFunction->m_BoneOutput[j] * m_Weight[i][j];
			}
			else
			{
				m_BoneOutput[j] += pAnimBaseFunction->m_BoneOutput[j] * m_Weight[i][j];
			}
#elif FAST_BLEND_PARIAL_ANIMITION == 1
			if (i == 0)
			{
				m_BoneOutput[j] = FastParitialMul(pAnimBaseFunction->m_BoneOutput[j] , m_Weight[i][j]);
			}
			else
			{
				m_BoneOutput[j].FastParitialBlend(pAnimBaseFunction->m_BoneOutput[j] , m_Weight[i][j]);
			}
#else
			if (i == 0)
			{
				pAnimBaseFunction->m_BoneOutput[j].GetMatrix(m_FastTempMatrix[j]);
				m_FastTempMatrix[j] *= m_Weight[i][j];


			}
			else
			{
				Matrix3X3W Temp;
				pAnimBaseFunction->m_BoneOutput[j].GetMatrix(Temp);
				m_FastTempMatrix[j] += Temp * m_Weight[i][j];

			}
#endif
		}
	}
#if FAST_BLEND_PARIAL_ANIMITION > 1
	if (!m_bOnlyUpdateRootMotion)
	{
		for (uint32 j = 0; j < m_BoneOutput.GetNum(); j++)
		{
			m_BoneOutput[j].FromMatrix(m_FastTempMatrix[j]);
		}
	}	
	if (m_bOnlyUpdateRootMotion && m_bHaveRootMotion)
	{
		m_RootAtom.FromMatrix(m_FastRootMatrix);
	}
#endif
	return 1;
}

void VSPartialAnimBlend::ComputeWeight()
{

	const VSSkeletonMeshNode * pMesh = GetSkeletonMeshNode();
	VSMAC_ASSERT(pMesh);


	VSSkeleton * pSkeleton = pMesh->GetSkeleton();
	VSMAC_ASSERT(pSkeleton);

	m_Weight.SetBufferNum(m_pInput.GetNum());
	for (uint32 i = 0; i < m_Weight.GetNum(); i++)
	{
		m_Weight[i].SetBufferNum(pSkeleton->GetBoneNum());
		if (i == 0)
		{
			for (uint32 j = 0; j < pSkeleton->GetBoneNum(); j++)
			{
				m_Weight[i][j] = 1.0f;
			}
		}
		else
		{
			for (uint32 j = 0; j < pSkeleton->GetBoneNum(); j++)
			{
				m_Weight[i][j] = 0.0f;
			}
		}
	}


	for (uint32 i = 0; i < m_BoneWeight.GetNum(); i++)
	{
		float Sum = 0.0f;
		for (uint32 j = 0; j < m_BoneWeight[i].Value.GetNum(); j++)
		{
			Sum += m_BoneWeight[i].Value[j];
		}

		for (uint32 j = 0; j < m_BoneWeight[i].Value.GetNum(); j++)
		{
			m_BoneWeight[i].Value[j] /= Sum;
		}
	}


	for (uint32 i = 0; i < m_BoneWeight.GetNum(); i++)
	{
			
		VSBoneNode * pBone = pSkeleton->GetBoneNode(m_BoneWeight[i].Key);

		VSArray<VSUsedName> AllBoneName;

		VSArray<VSBoneNode *> AllBoneArray;
		pBone->GetAllBoneArray(AllBoneArray);

		for (uint32 m = 0; m < AllBoneArray.GetNum(); m++)
		{
			AllBoneName.AddElement(AllBoneArray[m]->m_cName);
		}

		VSArray<unsigned int> BoneIndex;
		BoneIndex.SetBufferNum(AllBoneName.GetNum());
		for (uint32 s = 0; s < AllBoneName.GetNum(); s++)
		{
			for (uint32 j = 0; j < pSkeleton->GetBoneNum(); j++)
			{
				VSBoneNode * pBone = pSkeleton->GetBoneNode(j);

				if (pBone->m_cName == AllBoneName[s])
				{
					BoneIndex[s] = j;
				}
			}
		}	

		for (uint32 s = 0; s < m_Weight.GetNum(); s++)
		{
			for (uint32 j = 0; j < BoneIndex.GetNum(); j++)
			{

				m_Weight[s][BoneIndex[j]] = m_BoneWeight[i].Value[s];
			}
		}
	}		
}
bool VSPartialAnimBlend::ComputePara(double dAppTime)
{
	return true;
}