#include "graphic/pass/lightfunpass.h"
#include "graphic/render/scenemanager/spatial.h"
#include "graphic/node/model/bonenode.h"
#include "graphic/material/shaderstringfactory.h"
#include "graphic/render/constvalue.h"
#include "graphic/node/geometry.h"
#include "graphic/node/model/bonenode.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/material/material.h"
#include "graphic/core/profiler.h"
using namespace zq;
IMPLEMENT_RTTI(VSLightFunPass,VSPass)
BEGIN_ADD_PROPERTY(VSLightFunPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSLightFunPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(LightFunPRenderPassDraw, )
bool VSLightFunPass::InitialDefaultState()
{

	return 1;
}
bool VSLightFunPass::TerminalDefaultState()
{
	return 1;
}
VSLightFunPass::VSLightFunPass()
{



}
VSLightFunPass::~VSLightFunPass()
{


}


bool VSLightFunPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(LightFunPRenderPassDraw)


	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();

	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	m_GShaderkey.Clear();
	m_HShaderkey.Clear();
	m_DShaderkey.Clear();
	if (!GetVShader(VSResourceManager::GetInnerVertexShaderMap(), VSUsedName::ms_cLightFunShader))
	{
		return 0;
	}
	if (!GetPShader(VSResourceManager::GetMaterialPixelShaderMap(),pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurPShader[GetPassType()]);
	pMaterial->SetGlobleValue(MSPara,
		MSPara.pMaterialInstance->m_pCurVShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurPShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurGShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurHShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]);
	if(!pRenderer->DrawMesh(MSPara.pGeometry,&m_RenderState,
		MSPara.pMaterialInstance->m_pCurVShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurPShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurGShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurHShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]))
	{
		return false;
	}
	return 1;
}

