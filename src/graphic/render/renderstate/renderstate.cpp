#include "graphic/render/renderstate/renderstate.h"
#include "graphic/material/shader/shader.h"
#include "graphic/material/material.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSRenderState,VSObject)
BEGIN_ADD_PROPERTY(VSRenderState,VSObject)
REGISTER_PROPERTY(m_DepthStencilDesc,DepthStencilDesc,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RasterizerDesc,RasterizerDesc,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_BlendDesc,BlendDesc,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RectArray,RectArray,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_Plane,Plane,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSRenderState)
IMPLEMENT_INITIAL_END
bool VSRenderState::PostLoad(VSStream* pStream)
{
	m_pDepthStencilState = VSResourceManager::CreateDepthStencilState(m_DepthStencilDesc);
	m_pBlendState = VSResourceManager::CreateBlendState(m_BlendDesc);
	m_pRasterizerState = VSResourceManager::CreateRasterizerState(m_RasterizerDesc);
	return true;
}
bool VSRenderState::PostClone(VSObject * pObjectSrc)
{
	m_pDepthStencilState = VSResourceManager::CreateDepthStencilState(m_DepthStencilDesc);
	m_pBlendState = VSResourceManager::CreateBlendState(m_BlendDesc);
	m_pRasterizerState = VSResourceManager::CreateRasterizerState(m_RasterizerDesc);
	return true;
}
void VSRenderState::Inherit(const VSRenderState * pRenderState,uint32 uiInheritFlag)
{
	if (!uiInheritFlag)
	{
		return ;
	}
	VSMAC_ASSERT(pRenderState);
	bool bReCreateDepthStencil = false;
	bool bReCreateRasterizer = false;
	bool bReCreateBlend = false;
	if (uiInheritFlag & IF_WIRE_ENABLE)
	{
		if (m_pRasterizerState->GetRasterizerDesc().m_bWireEnable != 
			pRenderState->m_pRasterizerState->GetRasterizerDesc().m_bWireEnable)
		{
			bReCreateRasterizer = true;
			m_RasterizerDesc.m_bWireEnable = pRenderState->m_pRasterizerState->GetRasterizerDesc().m_bWireEnable;
		}
	}
	if (bReCreateRasterizer)
	{
		m_pRasterizerState = VSResourceManager::CreateRasterizerState(m_RasterizerDesc);
	}
	if (bReCreateDepthStencil)
	{
		m_pDepthStencilState = VSResourceManager::CreateDepthStencilState(m_DepthStencilDesc);
	}
	if (bReCreateBlend)
	{
		m_pBlendState = VSResourceManager::CreateBlendState(m_BlendDesc);
	}
	return;
}
void VSRenderState::SwapCull()
{
	uint32 uiChangeType[3] = {VSRasterizerDesc::CT_NONE,VSRasterizerDesc::CT_CCW,VSRasterizerDesc::CT_CW};
	m_RasterizerDesc.m_uiCullType = uiChangeType[m_RasterizerDesc.m_uiCullType];
	m_pRasterizerState = VSResourceManager::CreateRasterizerState(m_RasterizerDesc);
}