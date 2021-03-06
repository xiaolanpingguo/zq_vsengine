#include "graphic/render/renderstate/rasterizerstate.h"
#include "graphic/material/shader/shader.h"
#include "graphic/material/material.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSRasterizerState,VSBind)
VSPointer<VSRasterizerState> VSRasterizerState::ms_Default;
VSPointer<VSRasterizerState> VSRasterizerState::ScissorEnable;
BEGIN_ADD_PROPERTY(VSRasterizerState,VSBind)
REGISTER_PROPERTY(m_RasterizerDesc,RasterizerDesc,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSRasterizerState)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
bool VSRasterizerState::InitialDefaultState()
{
	VSRasterizerDesc RasterizerDesc;
	ms_Default = VSResourceManager::CreateRasterizerState(RasterizerDesc);
	if(!ms_Default)
		return 0;

	RasterizerDesc.m_bScissorTestEnable = true;
	ScissorEnable = VSResourceManager::CreateRasterizerState(RasterizerDesc);
	if (!ScissorEnable)
		return 0;

	return 1;
}
bool VSRasterizerState::TerminalDefaultState()
{
	ms_Default = NULL;
	ScissorEnable = NULL;
	return 1;
}
bool VSRasterizerState::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadRasterizerState(this,pID))
		return 0;
	return 1;
}
VSRasterizerState::VSRasterizerState()
{
	
}
VSRasterizerState::~VSRasterizerState()
{
	ReleaseResource();
}

IMPLEMENT_RTTI(VSRasterizerDesc,VSObject)
BEGIN_ADD_PROPERTY(VSRasterizerDesc,VSObject)
REGISTER_PROPERTY(m_bWireEnable,WireEnable,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCullType,CullType,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bClipPlaneEnable,ClipPlaneEnable,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bScissorTestEnable,ScissorTestEnable,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_fDepthBias,DepthBias,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSRasterizerDesc)
IMPLEMENT_INITIAL_END
