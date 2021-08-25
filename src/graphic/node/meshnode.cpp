#include "graphic/node/meshnode.h"
#include "graphic/node/switchnode.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/render/debugdraw.h"
#include "graphic/render/scenemanager/viewfamily.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSMeshNode,VSNode)
BEGIN_ADD_PROPERTY(VSMeshNode,VSNode)
REGISTER_PROPERTY(m_uiRenderGroup, m_RenderGroup, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bReceiveShadow, ReceiveShadow, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bCastShadow, CastShadow, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bDrawInstance, DrawInstance, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSMeshNode)
IMPLEMENT_INITIAL_END
VSMeshNode::VSMeshNode()
{
	m_bIsDrawBoundVolume = false;
	m_uiRenderGroup = VSCuller::RG_NORMAL;
	m_bIsVisibleUpdate = true;
	m_bReceiveShadow = true;
	m_bCastShadow = true;
	m_bLighted = true;
	m_bDrawInstance = false;
}
VSMeshNode::~VSMeshNode()
{

}
void VSMeshNode::UpdateView(VSCuller & Culler,double dAppTime)
{
	
	VSNode::UpdateView(Culler,dAppTime);
	VSCamera * pCamera = Culler.GetCamera();
	VSMAC_ASSERT(pCamera);

	if (Culler.GetCullerType() == VSCuller::CUT_MAIN)
	{
		if (m_bIsDrawBoundVolume)
		{
			for (uint32 i = 0 ; i < pCamera->GetViewFamilyNum() ;i++)
			{
				VSViewFamily * pViewFamily = pCamera->GetViewFamily(i);
				if (pViewFamily)
				{
					VSSceneRenderMethod * pRM = pViewFamily->m_pSceneRenderMethod;
					VSDebugDraw * pDebugDraw = pRM->GetDebugDraw(m_uiRenderGroup);
					if (pDebugDraw)
					{
						pDebugDraw->AddDebugLineAABB(m_WorldBV,VSColorRGBA(1.0f,0.0f,0.0f,1.0f).GetDWABGR(),false);
					}
				}
			}		
		}
	}
}
void VSMeshNode::ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{
	if (!Culler.CullConditionNode(this))
	{
		VSNode::ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);
	}
}