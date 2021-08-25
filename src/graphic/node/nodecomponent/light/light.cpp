#include "graphic/node/nodecomponent/light/light.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/render/scenemanager/scenerender.h"
#include "graphic/core/stream/stream.h"
#include "graphic/material/material.h"
#include "graphic/render/outputresource/rendertarget.h"
using namespace zq;
IMPLEMENT_RTTI_NoCreateFun(VSLight, VSNodeComponent)
BEGIN_ADD_PROPERTY(VSLight, VSNodeComponent)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSLight)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
VSLight::VSLight()
{

	m_bEnable = true;

	
}
VSLight::~VSLight()
{

	
}
bool VSLight::IsRelative(VSGeometry * pGeometry)
{
	if(pGeometry->GetMeshNode()->m_bLighted)
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}
bool VSLight::Cullby(VSCuller & Culler)
{
	m_bEnable = true;
	Culler.InsertLight(this);
	return true;
}
void VSLight::UpdateLightState(double dAppTime)
{
	VSNodeComponent::UpdateLightState(dAppTime);
	m_pAllLight.AddElement(this);
}
void VSLight::UpdateAll(double dAppTime)
{
	VSNodeComponent::UpdateAll(dAppTime);
	m_bEnable = false;
}
void VSLight::UpdateTransform(double dAppTime)
{
	if (m_bIsChanged)
	{
		GetLightRange();
	}
	VSNodeComponent::UpdateTransform(dAppTime);
}
void VSLight::BuildSceneInfo(VSScene * pScene)
{

}
IMPLEMENT_RTTI_NoCreateFun(VSLocalLight,VSLight)
BEGIN_ADD_PROPERTY(VSLocalLight,VSLight)
REGISTER_PROPERTY(m_Diffuse, Diffuse, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Specular, Specular, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bIsCastShadow, IsCastShadow, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_uiRTWidth, RTWidth, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_ZBias, ZBias, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WVP, WVP, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiLightMaterialRTWidth, LightMaterialRTWidth, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_ProjectShadowColor, ProjectShadowColor, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_LightFunScale, LightFunScale, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_LightFunOffset, LightFunOffset, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pLightMaterial, LightMaterial, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_ProjectShadowFallOff, ProjectShadowFallOff, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSLocalLight)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
VSLocalLight::VSLocalLight()
{

	m_bEnable = true;
	m_Diffuse = VSColorRGBA(1.0f,1.0f,1.0f,1.0f);
	m_Specular = VSColorRGBA(1.0f,1.0f,1.0f,1.0f);
	m_bIsCastShadow = true;
	m_pShadowTexture.AddElement(MEM_NEW VSTexAllState());
	m_ZBias = 0.0f;
	m_pLightFunDiffuseTexture = NULL;
	m_uiLightMaterialRTWidth = 1024;
	m_uiRTWidth = 1024;
	m_pLMSceneRender = NULL;
	m_LightFunScale = Vector2(0.5f, -0.5f);
	m_LightFunOffset = Vector2(0.5f, 0.5f);
	m_ProjectShadowColor = VSColorRGBA(0.2f,0.2f,0.2f,1.0f);
	m_ProjectShadowFallOff = 1.0f;
	m_bShadowMapDrawEnd = false;
	m_WVP.Identity();
}
VSLocalLight::~VSLocalLight()
{
	m_pShadowTexture.Clear();
	m_pLightFunDiffuseTexture = NULL;
	m_pLMSceneRender = NULL;
}
void VSLocalLight::SetCastShadow(bool bIsCastShadow)
{
	m_bIsCastShadow = bIsCastShadow;
}
void VSLocalLight::DrawNoDependenceShadowMap(VSCuller & CurCuller,double dAppTime)
{

}
void VSLocalLight::DisableNoDependenceShadowMap(double dAppTime)
{
	m_bShadowMapDrawEnd = false;
}
void VSLocalLight::DrawDependenceShadowMap(VSCuller & CurCuller, double dAppTime)
{

}
void VSLocalLight::DisableDependenceShadowMap(VSCuller & CurCuller, double dAppTime)
{

}
void VSLocalLight::DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture)
{

}
void VSLocalLight::DrawVolumeShadow(VSCuller & CurCuller, double dAppTime)
{

}
void VSLocalLight::SetLightMaterialRtWidth(uint32 uiRtWidth)
{
	if (uiRtWidth == m_uiLightMaterialRTWidth)
	{
		return ;
	}

	m_uiLightMaterialRTWidth = uiRtWidth;

	if (m_pLightMaterial)
	{
		m_pLightFunDiffuseTexture->SetTexture(MEM_NEW VS2DTexture(m_uiLightMaterialRTWidth,m_uiLightMaterialRTWidth,VSRenderer::SFT_A8R8G8B8, 1, true));
		m_pLightFunDiffuseRenderTarget = VSResourceManager::CreateRenderTarget(StaticCast<VS2DTexture>(m_pLightFunDiffuseTexture->GetTexture()));
	}
}
bool VSLocalLight::SetLightMaterial(VSMaterialR * pMaterial)
{
	if (pMaterial)
	{
		m_pLightMaterial = MEM_NEW VSMaterialInstance(pMaterial);
		m_pLightFunDiffuseTexture = MEM_NEW VSTexAllState();
		m_pLightFunDiffuseTexture->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLineBorderZero());
		m_pLightFunDiffuseTexture->SetTexture(MEM_NEW VS2DTexture(m_uiLightMaterialRTWidth,m_uiLightMaterialRTWidth,VSRenderer::SFT_A8R8G8B8, 1, true));
		m_pLightFunDiffuseRenderTarget = VSResourceManager::CreateRenderTarget(StaticCast<VS2DTexture>(m_pLightFunDiffuseTexture->GetTexture()));

		m_pLMSceneRender = MEM_NEW VSLightMaterialSceneRender();
		m_pLMSceneRender->m_pMaterialInstacne = m_pLightMaterial;
		m_pLMSceneRender->SetParam(VSRenderer::CF_COLOR,VSColorRGBA(0.0f,0.0f,0.0f,0.0f),1.0f,0);

	}
	else
	{
		m_pLightFunDiffuseTexture = NULL;
		m_pLightMaterial = NULL;
		m_pLMSceneRender = NULL;
		m_pLightFunSpecularRenderTarget = NULL;
		m_pLightFunDiffuseRenderTarget = NULL;
	}
	m_bIsChanged = true;
	return true;
}
void VSLocalLight::UpdateAll(double dAppTime)
{
	VSLight::UpdateAll(dAppTime);
}
void VSLocalLight::DrawLightMaterial(double dAppTime)
{
	if (m_pLightMaterial && m_bEnable)
	{
		VSMaterial * pMaterial = m_pLightMaterial->GetMaterial();
		VSLightShaderFunction * pLightShaderFunction = pMaterial->GetLightShaderFunction();
		if (pLightShaderFunction)
		{
			VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiLightMaterialRTWidth,m_uiLightMaterialRTWidth,VSRenderer::SFT_D24S8,0);
			m_pLMSceneRender->ClearRTAndDepth();
			m_pLMSceneRender->AddRenderTarget(m_pLightFunDiffuseRenderTarget);
			m_pLMSceneRender->SetDepthStencil(pDepthStencil);
			m_pLMSceneRender->m_pLight = this;
			VSCuller Temp;
			m_pLMSceneRender->Draw(Temp,dAppTime);
			VSResourceManager::DisableOutputResource(pDepthStencil);
		}
	}
	
}
void VSLocalLight::GetCullerAABBArray(VSCuller & Culler,VSArray<AABB> & AABBArray)
{

	for (uint32 i = 0 ; i < VSCuller::VST_MAX ;i++)
	{

		Culler.GetAllVisibleAABB(i, VSCuller::RG_NORMAL, AABBArray);

	}
}
IMPLEMENT_RTTI_NoCreateFun(VSIndirectLight,VSLight)
BEGIN_ADD_PROPERTY(VSIndirectLight,VSLight)
REGISTER_PROPERTY(m_Range, Range, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WorldRenderBV, WorldRenderBV, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSIndirectLight)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
VSIndirectLight::VSIndirectLight()
{
	m_bInheritScale = false;
	m_bInheritRotate = false;

	m_Range = Vector3(999999.0f, 999999.0f, 999999.0f);
}
VSIndirectLight::~VSIndirectLight()
{

}
void VSIndirectLight::SetLocalRotate(const Matrix3X3 & Rotate)
{

}
bool VSIndirectLight::IsRelative(VSGeometry * pGeometry)
{
	if (!VSLight::IsRelative(pGeometry))
	{
		return false;
	}
	AABB GeometryAABB = pGeometry->GetWorldAABB();
	if (GeometryAABB.RelationWith(m_WorldRenderBV) == MATH_NOINTERSECT)
	{
		return false;
	}
	return true;
}
void VSIndirectLight::SetLocalTransform(const VSTransform & LocalTransform)
{
	Vector3 Tranlation = LocalTransform.GetTranslate();
	SetLocalTranslate(Tranlation);
}
void VSIndirectLight::SetLocalMat(const Matrix3X3W VSMat)
{
	Vector3 Tranlation = VSMat.GetTranslation();
	SetLocalTranslate(Tranlation);
}
void VSIndirectLight::SetLocalScale(const Vector3 & fScale)
{

}
void VSIndirectLight::GetLightRange()
{
	Vector3 Pos = GetWorldTranslate();
	m_WorldRenderBV = AABB(Pos, m_Range.x, m_Range.y, m_Range.z);
}
bool VSIndirectLight::Cullby(VSCuller & Culler)
{
	uint32 uiVSF = Culler.IsVisible(m_WorldRenderBV, true);
	if (uiVSF == VSCuller::VSF_ALL || uiVSF == VSCuller::VSF_PARTIAL)
	{
		m_bEnable = true;
		Culler.InsertLight(this);

	}	

	return true;
}
void VSLocalLight::BuildSceneInfo(VSScene * pScene)
{
	VSLight::BuildSceneInfo(pScene);
}

