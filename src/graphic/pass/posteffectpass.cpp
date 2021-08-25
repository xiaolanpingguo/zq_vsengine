#include "graphic/pass/posteffectpass.h"
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
IMPLEMENT_RTTI(VSPostEffectPass, VSPass)
BEGIN_ADD_PROPERTY(VSPostEffectPass, VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPostEffectPass)
ADD_PRIORITY(VSBlendState)
ADD_PRIORITY(VSDepthStencilState)
ADD_PRIORITY(VSRasterizerState)
ADD_INITIAL_FUNCTION(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(PostEffectRenderPassDraw, )
bool VSPostEffectPass::InitialDefaultState()
{

	return 1;
}
bool VSPostEffectPass::TerminalDefaultState()
{
	return 1;
}
VSPostEffectPass::VSPostEffectPass()
{


	MSPara.VSShaderPath = _T("PostEffectVShader.txt");
	MSPara.VMainFunName = _T("VSMain");

}
VSPostEffectPass::~VSPostEffectPass()
{


}


bool VSPostEffectPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(PostEffectRenderPassDraw)


	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();


	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	m_GShaderkey.Clear();
	m_HShaderkey.Clear();
	m_DShaderkey.Clear();
	if (!GetVShader(VSResourceManager::GetInnerVertexShaderMap(), VSUsedName::ms_cPostEffectShader))
	{
		return 0;
	}
	if (!GetPShader(VSResourceManager::GetMaterialPixelShaderMap(), pMaterial->GetResourceName()))
	{
		return 0;
	}

	pRenderer->SetMaterialVShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurVShader[GetPassType()]);
	pRenderer->SetMaterialPShaderConstant(MSPara,MSPara.pMaterialInstance->m_pCurPShader[GetPassType()]);
	SetCustomConstant(MSPara, MSPara.pMaterialInstance->m_pCurVShader[GetPassType()], MSPara.pMaterialInstance->m_pCurPShader[GetPassType()]);
	if (!pRenderer->DrawMesh(MSPara.pGeometry, &m_RenderState,
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
void VSPostEffectPass::SetCustomConstant(MaterialShaderPara &MSPara, VSVShader * pVShader, VSPShader * pPShader)
{
	//VS
	float Inv_Width = 1.0f / m_uiRTWidth;
	float Inv_Height = 1.0f / m_uiRTHeight;
	pVShader->SetParam(VSUsedName::ms_cPostInv_Width, &Inv_Width);
	pVShader->SetParam(VSUsedName::ms_cPostInv_Height, &Inv_Height);
	pPShader->SetParam(VSShaderStringFactory::ms_PSColorBufferResource, m_PColorBuffer);
}

