#include "graphic/pass/indirectrenderpass.h"
#include "graphic/render/scenemanager/spatial.h"
#include "graphic/node/model/bonenode.h"
#include "graphic/material/shaderstringfactory.h"
#include "graphic/render/constvalue.h"
#include "graphic/node/geometry.h"
#include "graphic/node/model/bonenode.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/material/material.h"
#include "graphic/material/sortlight.h"
#include "graphic/node/nodecomponent/light/skylight.h"
#include "graphic/core/profiler.h"
using namespace zq;
IMPLEMENT_RTTI(VSIndirectRenderPass,VSPass)
BEGIN_ADD_PROPERTY(VSIndirectRenderPass,VSPass)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSIndirectRenderPass)
IMPLEMENT_INITIAL_END
DECLEAR_TIME_PROFILENODE(IndirectRenderPassDraw, )
VSIndirectRenderPass::VSIndirectRenderPass()
{



}
VSIndirectRenderPass::~VSIndirectRenderPass()
{

}
bool VSIndirectRenderPass::Draw(VSRenderer * pRenderer)
{
	ADD_TIME_PROFILE(IndirectRenderPassDraw)


	VSMaterial * pMaterial = MSPara.pMaterialInstance->GetMaterial();
	
	VSColorRGBA SkyLightUpColor = VSColorRGBA(0.0f,0.0f,0.0f,0.0f);
	VSColorRGBA SkyLightDownColor = VSColorRGBA(0.0f,0.0f,0.0f,0.0f);

	for (uint32 i = 0 ; i < m_Light.GetNum() ; i++)
	{
		if (m_Light[i]->GetLightType() == VSLight::LT_SKY)
		{
			SkyLightUpColor += ((VSSkyLight *)m_Light[i])->m_UpColor;
			SkyLightDownColor +=((VSSkyLight *)m_Light[i])->m_DownColor;
		}
	}

	MSPara.LightArray = m_Light;
	MSPara.SkyLightUpColor = SkyLightUpColor;
	MSPara.SkyLightDownColor = SkyLightDownColor;

	if (pMaterial && pMaterial->GetShaderMainFunction(MSPara.uiPassId)->HasTessellation())
	{
		MSPara.bUseTessellation = true;
	}
	else
	{
		MSPara.bUseTessellation = false;
	}
	m_VShaderkey.Clear();
	m_PShaderkey.Clear();
	m_GShaderkey.Clear();
	m_HShaderkey.Clear();
	m_DShaderkey.Clear();
	
	if (!GetVShader(VSResourceManager::GetIndirectVertexShaderMap(), pMaterial->GetResourceName()))
	{
		m_Light.Clear();
		return 0;
	}
	if (!GetHShader(VSResourceManager::GetInnerHullShaderMap(), VSUsedName::ms_cHullShader))
	{
		m_Light.Clear();
		return 0;
	}
	if (!GetDShader(VSResourceManager::GetIndirectDomainShaderMap(), pMaterial->GetResourceName()))
	{
		m_Light.Clear();
		return 0;
	}
	if (!GetPShader(VSResourceManager::GetIndirectPixelShaderMap(),pMaterial->GetResourceName()))
	{
		m_Light.Clear();
		return 0;
	}


	pRenderer->SetMaterialVShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurVShader[GetPassType()]);
	if (MSPara.bUseTessellation)
	{
		pRenderer->SetMaterialHShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurHShader[GetPassType()]);
		pRenderer->SetMaterialDShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]);
	}
	pRenderer->SetMaterialPShaderConstant(MSPara, MSPara.pMaterialInstance->m_pCurPShader[GetPassType()]);

	pMaterial->SetGlobleValue(MSPara,
		MSPara.pMaterialInstance->m_pCurVShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurPShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurGShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurHShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]);

	if(!pRenderer->DrawMesh(MSPara.pGeometry,&pMaterial->GetRenderState(MSPara.uiPassId),
		MSPara.pMaterialInstance->m_pCurVShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurPShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurGShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurHShader[GetPassType()],
		MSPara.pMaterialInstance->m_pCurDShader[GetPassType()]))
	{
		return false;
	}
	


	m_Light.Clear();
	return 1;
}

