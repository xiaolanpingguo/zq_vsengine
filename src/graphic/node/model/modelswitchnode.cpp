#include "graphic/node/model/modelswitchnode.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSModelSwitchNode,VSSwitchNode)
BEGIN_ADD_PROPERTY(VSModelSwitchNode,VSSwitchNode)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSModelSwitchNode)
IMPLEMENT_INITIAL_END
VSModelSwitchNode::VSModelSwitchNode()
{

}
VSModelSwitchNode::~VSModelSwitchNode()
{

}
void VSModelSwitchNode::SetUseMaterialInstance(uint32 SubMeshID, uint32 UsedID)
{
	for (uint32 i = 0; i < m_pChild.GetNum(); i++)
	{
		VSGeometryNode* pGeometryNode = DynamicCast<VSGeometryNode>(m_pChild[i]);
		pGeometryNode->GetNormalGeometry(SubMeshID)->SetUseMaterialInstance(UsedID);
	}
}
void VSModelSwitchNode::GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const
{
	VSGeometryNode* pGeometryNode = DynamicCast<VSGeometryNode>(m_pChild[m_uiActiveNode]);
	if (pGeometryNode)
	{
		pGeometryNode->GetStreamResource(pResourceProxy, StreamInformation);
	}
}
VSGeometryNode * VSModelSwitchNode::GetGeometryNode(uint32 uiLodLevel)
{
	if (uiLodLevel >= m_pChild.GetNum())
	{
		return NULL;
	}
	else
	{
		VSGeometryNode * pGeometryNode = DynamicCast<VSGeometryNode>(m_pChild[uiLodLevel]);
		return pGeometryNode;
	}
}
void VSModelSwitchNode::UpdateView(VSCuller & Culler, double dAppTime)
{
	VSSwitchNode::UpdateView(Culler, dAppTime);
	VSCamera * pCamera = Culler.GetCamera();
	VSMAC_ASSERT(pCamera);
	m_uiActiveNode = 0;
	if (Culler.GetCullerType() == VSCuller::CUT_MAIN)
	{
		float fScreenSize = pCamera->GetProjectScreenSize(GetWorldAABB()) * VSConfig::ms_LODScreenScale;
		for (uint32 i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			VSGeometryNode *pGeometryNode = DynamicCast<VSGeometryNode>(m_pChild[i]);
			if (pGeometryNode && fScreenSize > pGeometryNode->m_fLODScreenSize)
			{
				m_uiActiveNode = i;
				break;
			}
		}
		/*float ZFar = pCamera->GetZFar();
		Vector3 DistVector = pCamera->GetWorldTranslate() - GetWorldTranslate();
		float Dist = DistVector.GetLength();
		float LastTemp = ZFar * 0.5f;
		float CurTemp = LastTemp;

		m_uiActiveNode = 0;
		while (true)
		{
			if (Dist > CurTemp)
			{		
				if (m_uiActiveNode + 1 >= m_pChild.GetNum() || m_pChild[m_uiActiveNode + 1] == NULL)
				{
					break;
				}
				LastTemp = LastTemp * 0.5f;
				CurTemp = CurTemp + LastTemp;
				m_uiActiveNode++;
			}
			else
			{
				break;
			}
		}*/
	}


}