#include "graphic/node/nodecomponent/light/spotlight.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/render/scenemanager/scenerender.h"
#include "graphic/core/stream/stream.h"
#include "math/obb.h"
using namespace zq;
IMPLEMENT_RTTI(VSSpotLight,VSLocalLight)
BEGIN_ADD_PROPERTY(VSSpotLight,VSLocalLight)
REGISTER_PROPERTY(m_Range, m_Range, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Falloff, Falloff, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Theta, Theta, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_Phi, Phi, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WorldRenderBV, WorldRenderBV, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_ENUM_PROPERTY(m_uiShadowType, ShadowType, SpotLightShadowType, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
BEGIN_ADD_ENUM
ADD_ENUM(SpotLightShadowType, ST_NORMAL)
ADD_ENUM(SpotLightShadowType, ST_PROJECT)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSpotLight)
IMPLEMENT_INITIAL_END
VSSpotLight::VSSpotLight()
{
	m_Range = 1500.0f;

	m_Falloff		= 1.0;
	m_Theta			= MATH_PI2 / 2.0;
	m_Phi			= MATH_PI2;
	m_uiRTWidth = 1024;
	m_ZBias = 0.005f;
	SetShadowType(ST_NORMAL);
	m_ProjectShadowColor = VSColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
}
VSSpotLight::~VSSpotLight()
{
	m_pShadowTexture.Clear();

	m_pShadowMapSceneRender = NULL;

}
void VSSpotLight::GetLightRange()
{	
	Vector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	Vector3 Point3 = GetWorldTranslate();
	float R = TAN(m_Phi * 0.5f) * m_Range;
	OBB Obb(Dir, Up, Right, m_Range * 0.5f, R, R, Point3 + Dir * m_Range * 0.5f);
	m_WorldRenderBV = Obb.GetAABB();
	if (HaveLightFun())
	{
		VSCamera LightCamera;
		LightCamera.CreateFromLookDir(Point3, Dir);
		LightCamera.UpdateAll(0);

		LightCamera.SetPerspectiveFov(m_Phi, 1.0f, 1.0f, m_Range);
		m_WVP = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();
	}	
}
bool VSSpotLight::PostClone(VSObject * pObjectSrc)
{
	VSLight::PostClone(pObjectSrc);
	SetShadowType(m_uiShadowType);
	return true;
}
bool VSSpotLight::PostLoad(VSStream* pStream)
{
	VSLight::PostLoad(pStream);
	SetShadowType(m_uiShadowType);
	return true;
}
bool VSSpotLight::Cullby(VSCuller & Culler)
{
	uint32 uiVSF = Culler.IsVisible(m_WorldRenderBV,true);
	if (uiVSF == VSCuller::VSF_ALL || uiVSF == VSCuller::VSF_PARTIAL)
	{
		m_bEnable = true;
		Culler.InsertLight(this);
	}
	return true;
}
bool VSSpotLight::IsRelative(VSGeometry * pGeometry)
{
	if (!VSLocalLight::IsRelative(pGeometry))
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
void VSSpotLight::ResetShadow()
{
	m_pShadowTexture.Clear();
	m_pShadowMapSceneRender = NULL;
	m_pProjectShadowSceneRender = NULL;
	m_ShadowCuller.ClearAll();
}
void VSSpotLight::SetShadowType(uint32 uiShadowType)
{
	ResetShadow();
	if (uiShadowType == ST_NORMAL || uiShadowType == ST_PROJECT)
	{
		m_pShadowTexture.AddElement(MEM_NEW VSTexAllState());
		m_pShadowTexture[0]->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLineBorderOne());
		m_pShadowMapSceneRender = MEM_NEW VSShadowMapSceneRender(VSShadowMapSceneRender::SMT_SHADOWMAP);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(VSRenderer::CF_USE_ALL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
		if (uiShadowType == ST_PROJECT)
		{
			m_pProjectShadowSceneRender = MEM_NEW VSProjectShadowSceneRender();
			m_pProjectShadowSceneRender->m_pLocalLight = this;
			m_pProjectShadowSceneRender->SetParam(VSRenderer::CF_STENCIL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 15);
		}
	}
	m_bShadowMapDrawEnd = false;
	m_uiShadowType = uiShadowType;
}
void VSSpotLight::DisableDependenceShadowMap(VSCuller & CurCuller,double dAppTime)
{
	if (m_uiShadowType == ST_PROJECT)
	{
		if (m_pShadowRenderTarget)
		{
			VSResourceManager::DisableOutputResource(m_pShadowRenderTarget);
		}
		for (uint32 i = 0; i < m_pShadowTexture.GetNum(); i++)
		{
			m_pShadowTexture[i]->SetTexture(NULL);
		}
	}
	
}
void VSSpotLight::DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_uiShadowType == ST_PROJECT)
		{
			Vector3 WorldPos = GetWorldTranslate();

			Vector3 Dir,Up,Right;
			GetWorldDir(Dir,Up,Right);


			Matrix3X3 Rot = GetWorldRotate();
			VSCamera TempCamera;
			TempCamera.CreateFromLookDir(GetWorldTranslate(), Dir);
			TempCamera.SetPerspectiveFov(m_Phi, 1.0f, 1.0f, m_Range);

			m_ShadowCuller.GetSceneContent(TempCamera, m_pScene, this, dAppTime);
			

			m_LightShadowMatrix = TempCamera.GetViewMatrix() * TempCamera.GetProjMatrix();

			m_pShadowRenderTarget = VSResourceManager::GetRenderTarget(m_uiRTWidth,m_uiRTWidth,VSRenderer::SFT_R32F,0);
			VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth,m_uiRTWidth,VSRenderer::SFT_D24S8,0);
			m_pShadowMapSceneRender->ClearRTAndDepth();
			m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil,VSCuller::RG_NORMAL);
			m_pShadowMapSceneRender->AddRenderTarget(m_pShadowRenderTarget);

			m_pShadowMapSceneRender->Draw(m_ShadowCuller,dAppTime);

			m_pShadowTexture[0]->SetTexture((VSTexture *)m_pShadowRenderTarget->GetCreateBy());

			VSResourceManager::DisableOutputResource(pDepthStencil);

			m_pProjectShadowSceneRender->m_pNormalDepthTexture = pNormalDepthTexture;
			m_pProjectShadowSceneRender->m_fLightRange = m_Range;
			m_pProjectShadowSceneRender->m_LightWorldDirection = Dir;
			m_pProjectShadowSceneRender->m_Falloff = m_Falloff;
			m_pProjectShadowSceneRender->m_Theta = m_Theta;
			m_pProjectShadowSceneRender->m_Phi = m_Phi;
			m_pProjectShadowSceneRender->Draw(CurCuller, dAppTime);
			DisableDependenceShadowMap(m_ShadowCuller, dAppTime);
		}
	}
}
void VSSpotLight::DrawDependenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	
}
void VSSpotLight::DrawNormalShadowMap(VSCuller & CurCuller,double dAppTime)
{
	if (CurCuller.HasLight(this) == false)
	{
		return;
	}
	Vector3 Dir,Up,Right;
	GetWorldDir(Dir,Up,Right);

	m_ShadowCuller.ClearAll();

	VSCamera LightCamera;
	LightCamera.CreateFromLookDir(GetWorldTranslate(),Dir);
	LightCamera.SetPerspectiveFov(m_Phi,1.0f,1.0f,m_Range);
	m_ShadowCuller.GetSceneContent(LightCamera, m_pScene, this, dAppTime);
	
	m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();

	m_pShadowRenderTarget = VSResourceManager::GetRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R32F, 0);
	VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_D24S8, 0);
	m_pShadowMapSceneRender->ClearRTAndDepth();
	m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, VSCuller::RG_NORMAL);
	m_pShadowMapSceneRender->AddRenderTarget(m_pShadowRenderTarget);

	m_pShadowMapSceneRender->Draw(m_ShadowCuller, dAppTime);

	m_pShadowTexture[0]->SetTexture((VSTexture *)m_pShadowRenderTarget->GetCreateBy());

	VSResourceManager::DisableOutputResource(pDepthStencil);
}
void VSSpotLight::DrawNoDependenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	if (m_bEnable && m_bIsCastShadow)
	{
		if (m_bShadowMapDrawEnd == false)
		{
			m_bShadowMapDrawEnd = true;
		}
		else
		{
			return;
		}
		if (m_uiShadowType == ST_NORMAL)
		{
			
			DrawNormalShadowMap(CurCuller, dAppTime);
		}
	}
}
void VSSpotLight::DisableNoDependenceShadowMap(double dAppTime)
{
	VSLocalLight::DisableNoDependenceShadowMap(dAppTime);
	if (m_uiShadowType == ST_NORMAL)
	{
		if (m_pShadowRenderTarget)
		{
			VSResourceManager::DisableOutputResource(m_pShadowRenderTarget);
		}
		for (uint32 i = 0; i < m_pShadowTexture.GetNum(); i++)
		{
			m_pShadowTexture[i]->SetTexture(NULL);
		}
	}
	
}