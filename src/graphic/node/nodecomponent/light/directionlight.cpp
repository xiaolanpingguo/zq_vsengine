#include "graphic/node/nodecomponent/light/directionlight.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/render/scenemanager/scenerender.h"
#include "graphic/core/stream/stream.h"
#include "math/ray.h"


namespace zq{


IMPLEMENT_RTTI(VSDirectionLight,VSLocalLight)
BEGIN_ADD_PROPERTY(VSDirectionLight,VSLocalLight)
REGISTER_ENUM_PROPERTY(m_uiShadowType, ShadowType, DirectionLightShadowType,VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunWidth, LightFunWidth, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunHeight, LightFunHeight, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunNear, LightFunNear, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLightFunFar, LightFunFar, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_WorldRenderBV, WorldRenderBV, VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_fOSMDistance, OSMDistance, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
BEGIN_ADD_ENUM
ADD_ENUM(DirectionLightShadowType, ST_VOLUME)
ADD_ENUM(DirectionLightShadowType, ST_OSM)
ADD_ENUM(DirectionLightShadowType, ST_CSM)
ADD_ENUM(DirectionLightShadowType, ST_PROJECT)
END_ADD_ENUM
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSDirectionLight)
IMPLEMENT_INITIAL_END
#define CSM_LEVLE 3
VSDirectionLight::VSDirectionLight()
{
	m_uiRTWidth = 2048;
	SetShadowType(ST_OSM);
	m_ZBias = 0.005f;
	m_fLightFunWidth = 1000.0f;
	m_fLightFunHeight = 1000.0f; 
	m_fLightFunNear = 1.0f; 
	m_fLightFunFar = 8000.0f;
	m_fOSMDistance = 5000.0f;
}
VSDirectionLight::~VSDirectionLight()
{

}
void VSDirectionLight::ResetShadow()
{
	m_pShadowTexture.Clear();
	m_pPEVolumeSMSceneRender = NULL;
	m_pShadowMapSceneRender = NULL;
	m_pProjectShadowSceneRender = NULL;
	m_pVolumeShadowSceneRender = NULL;
	m_pCSMRTArray.Clear();
	m_VolumeShadowCuller.ClearAll();
	m_ShadowCuller.ClearAll();
}
void VSDirectionLight::SetShadowType(uint32 uiShadowType)
{
	ResetShadow();
	if (uiShadowType == ST_VOLUME)
	{
		
		m_pShadowTexture.AddElement(MEM_NEW VSTexAllState());
		m_pShadowTexture[0]->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLine());
		m_pPEVolumeSMSceneRender = MEM_NEW VSPEVolumeShadowMapSceneRender();
		m_pPEVolumeSMSceneRender->SetParam(VSRenderer::CF_COLOR,VSColorRGBA(1.0f,1.0f,1.0f,1.0f),1.0f,0);
		m_pVolumeShadowSceneRender = MEM_NEW VSVolumeShadowSceneRender();
		m_pVolumeShadowSceneRender->m_pLocalLight = this;
		m_pVolumeShadowSceneRender->SetParam(VSRenderer::CF_STENCIL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 15);

	}
	else if (uiShadowType == ST_OSM || uiShadowType == ST_PROJECT)
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
	else if (uiShadowType == ST_CSM)
	{
		for (uint32 i = 0 ; i < CSM_LEVLE ; i++)
		{
			m_pShadowTexture.AddElement(MEM_NEW VSTexAllState());
			m_pShadowTexture[i]->SetSamplerState((VSSamplerState*)VSSamplerState::GetTwoLineBorderOne());
		}
		m_pCSMRTArray.SetBufferNum(CSM_LEVLE);
		m_pShadowMapSceneRender = MEM_NEW VSShadowMapSceneRender(VSShadowMapSceneRender::SMT_SHADOWMAP);
		m_pShadowMapSceneRender->m_pLocalLight = this;
		m_pShadowMapSceneRender->SetParam(VSRenderer::CF_USE_ALL, VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
	}
	m_uiShadowType = uiShadowType;

}

bool VSDirectionLight::PostClone(VSObject * pObjectSrc)
{
	VSLight::PostClone(pObjectSrc);
	SetShadowType(m_uiShadowType);
	return true;
}

bool VSDirectionLight::PostLoad(VSStream* pStream)
{
	VSLight::PostLoad(pStream);
	SetShadowType(m_uiShadowType);
	return true;
}

void VSDirectionLight::DisableDependenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	if (m_uiShadowType == ST_VOLUME || m_uiShadowType == ST_OSM || m_uiShadowType == ST_PROJECT)
	{
		if (m_pVolumeShadowRenderTarget)
		{
			VSResourceManager::DisableOutputResource(m_pVolumeShadowRenderTarget);
		}
	}
	else if (m_uiShadowType == ST_CSM)
	{
		for (uint32 i = 0 ; i < CSM_LEVLE ; i++)
		{
			if (m_pCSMRTArray[i])
			{
				VSResourceManager::DisableOutputResource(m_pCSMRTArray[i]);
			}
		}
	}
	for (uint32 i = 0 ; i < m_pShadowTexture.GetNum() ; i++)
	{
		m_pShadowTexture[i]->SetTexture(NULL);
	}
}

void VSDirectionLight::DrawVolumeShadow(VSCuller & CurCuller,double dAppTime)
{
	if (m_uiShadowType != ST_VOLUME)
	{
		return;
	}
	m_VolumeShadowCuller.GetSceneContent(*CurCuller.GetCamera(), m_pScene,this, dAppTime);	

	m_pVolumeShadowSceneRender->Draw(m_VolumeShadowCuller, dAppTime);
	
	m_pVolumeShadowRenderTarget = VSResourceManager::GetRenderTarget(VSRenderer::ms_pRenderer->GetCurRTWidth(), VSRenderer::ms_pRenderer->GetCurRTHeight(), VSRenderer::SFT_A8R8G8B8, 0);

	m_pPEVolumeSMSceneRender->ClearRTAndDepth();
	m_pPEVolumeSMSceneRender->AddRenderTarget(m_pVolumeShadowRenderTarget);
	m_pPEVolumeSMSceneRender->Draw(CurCuller,dAppTime);

	m_pShadowTexture[0]->SetTexture((VSTexture *)m_pVolumeShadowRenderTarget->GetCreateBy());

}

AABB VSDirectionLight::GetMaxAABB(VSArray<AABB> &AABBArray)
{
	AABB Temp;
	for (uint32 i = 0 ;i < AABBArray.GetNum() ; i++)
	{	
		if (i == 0)
		{
			Temp = AABBArray[i];
		}
		else
		{
			Temp = Temp.MergeAABB(AABBArray[i]);
		}
	}
	return Temp;
}

void VSDirectionLight::DrawOSM(VSCuller & CurCuller,double dAppTime)
{
#define DRAW_DEPTH \
	{\
		m_pOSMShadowRenderTarget = VSResourceManager::GetRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R16F, 0);\
		VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_D24S8, 0);\
		m_pShadowMapSceneRender->ClearRTAndDepth();\
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, VSCuller::RG_NORMAL);\
		m_pShadowMapSceneRender->AddRenderTarget(m_pOSMShadowRenderTarget);\
		m_pShadowMapSceneRender->Draw(m_ShadowCuller, dAppTime);\
		m_pShadowTexture[0]->SetTexture((VSTexture *)m_pOSMShadowRenderTarget->GetCreateBy());\
		VSResourceManager::DisableOutputResource(pDepthStencil);\
	}
	VSCamera * pCamera = CurCuller.GetCamera();
	float fFar = pCamera->GetZFar();
	float fTempOSMDistance = m_fOSMDistance;
	if (fTempOSMDistance > fFar)
	{
		fTempOSMDistance = fFar;
	}
	pCamera->SetPerspectiveFov(pCamera->GetFov(), pCamera->GetAspect(), pCamera->GetZNear(), fTempOSMDistance);
	m_ShadowCuller.GetSceneContent(*pCamera, m_pScene, this, dAppTime);

	AABB CasterAABB = pCamera->GetFrustumAABB();

	Vector3 Center = CasterAABB.GetCenter();

	Vector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	Ray ray(Center,Dir * (-1.0f));

	uint32 Q;
	float tN,tF;
	if(CasterAABB.RelationWith(ray,Q,tN,tF) != MATH_INTERSECT)
	{
		DRAW_DEPTH;
		return ;
	}

	Vector3 LigthPT = Center - Dir * tN * 2.0f;

	VSCamera LightCamera;
	LightCamera.CreateFromLookAt(LigthPT,Center);

	Matrix3X3W LightView = LightCamera.GetViewMatrix();

	AABB NewCasterAABB;
	NewCasterAABB.Transform(CasterAABB,LightView);

	float NewNear = NewCasterAABB.GetMinPoint().z;
	float NewFar = NewCasterAABB.GetMaxPoint().z;
	LightCamera.SetOrthogonal(NewCasterAABB.GetMaxPoint().x - NewCasterAABB.GetMinPoint().x,
		NewCasterAABB.GetMaxPoint().y - NewCasterAABB.GetMinPoint().y,
		NewNear,NewFar);

	m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();


	DRAW_DEPTH;

	pCamera->SetPerspectiveFov(pCamera->GetFov(), pCamera->GetAspect(), pCamera->GetZNear(), fFar);
}
void VSDirectionLight::DrawDependenceShadowMap(VSCuller & CurCuller, double dAppTime)
{
	if (m_bEnable && m_bIsCastShadow)
	{	
		if (m_uiShadowType == ST_OSM)
		{
			DrawOSM(CurCuller,dAppTime);	
		}
		else if (m_uiShadowType == ST_CSM)
		{
			DrawCSM(CurCuller,dAppTime);
		}

	}
}
void VSDirectionLight::DrawCSM(VSCuller & CurCuller, double dAppTime)
{
	if (HaveLightFun())
	{
		return;
	}
#define DRAW_CSM_DEPTH \
	{\
		m_pCSMRTArray[i] = VSResourceManager::GetRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R16F, 0);\
		VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_D24S8, 0);\
		m_pShadowMapSceneRender->ClearRTAndDepth();\
		m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, VSCuller::RG_NORMAL);\
		m_pShadowMapSceneRender->AddRenderTarget(m_pCSMRTArray[i]);\
		m_pShadowMapSceneRender->Draw(m_ShadowCuller, dAppTime);\
		m_pShadowTexture[i]->SetTexture((VSTexture *)m_pCSMRTArray[i]->GetCreateBy());\
		VSResourceManager::DisableOutputResource(pDepthStencil);\
	}

	VSCamera * pCamera = CurCuller.GetCamera();
	float fNear = pCamera->GetZNear();
	float fFar = pCamera->GetZFar();

	Vector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	float Range[CSM_LEVLE + 1] = {fNear , 2000.0f , 7000.0f , fFar};
	for (uint32 i = 0; i < CSM_LEVLE; i++)
	{
		pCamera->SetPerspectiveFov(pCamera->GetFov(), pCamera->GetAspect(), Range[i], Range[i + 1]);
		m_ShadowCuller.GetSceneContent(*pCamera, m_pScene, this, dAppTime);

		VSArray<AABB> CasterAABBArray;
		GetCullerAABBArray(m_ShadowCuller, CasterAABBArray);
		if (CasterAABBArray.GetNum() == 0)
		{
			DRAW_CSM_DEPTH;
			continue;
		}
		AABB CasterAABB = GetMaxAABB(CasterAABBArray);
		AABB ReceiverAABB = m_ShadowCuller.GetCamera()->GetFrustumAABB();

		Matrix3X3W LightRot;
		LightRot.CreateFromLookDir(Vector3::ms_Zero,Dir);

		AABB NewCasterAABB, NewReceiverAABB;
		NewCasterAABB.Transform(CasterAABB, LightRot);
		NewReceiverAABB.Transform(ReceiverAABB, LightRot);

		AABB MinAABB = NewReceiverAABB.GetMin(NewCasterAABB);
		Vector3 MinP(MinAABB.GetMinPoint().x, MinAABB.GetMinPoint().y, NewCasterAABB.GetMinPoint().z);
		Vector3 MaxP(MinAABB.GetMaxPoint().x, MinAABB.GetMaxPoint().y, NewCasterAABB.GetMaxPoint().z);
		MinAABB.Set(MaxP,MinP);
		Vector3 Center = MinAABB.GetCenter();

		Ray ray(Center, Vector3::ms_Front * (-1.0f));

		uint32 Q;
		float tN, tF;
		if (MinAABB.RelationWith(ray, Q, tN, tF) != MATH_INTERSECT)
		{
			DRAW_CSM_DEPTH;
			continue;
		}

		Vector3 LigthPT = Center - Vector3::ms_Front * tN * 10.0f;

		Vector3 NewLightPT = LigthPT * LightRot.GetInverse();

		VSCamera LightCamera;
		LightCamera.CreateFromLookDir(NewLightPT, Dir);

		MinAABB.Set(MinAABB.GetMaxPoint() - LigthPT,MinAABB.GetMinPoint() - LigthPT);

		
		LightCamera.SetOrthogonal(MinAABB.GetMaxPoint().x - MinAABB.GetMinPoint().x,
			MinAABB.GetMaxPoint().y - MinAABB.GetMinPoint().y,
			MinAABB.GetMinPoint().z, MinAABB.GetMaxPoint().z);

		m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();

		m_CSMLightShadowMatrix[i] = m_LightShadowMatrix;

		DRAW_CSM_DEPTH;	
	}

	pCamera->SetPerspectiveFov(pCamera->GetFov(), pCamera->GetAspect(), fNear, fFar);
}

void VSDirectionLight::GetLightRange()
{
	
	Vector3 Point3 = GetWorldTranslate();
	Vector3 Dir, Up, Right;
	GetWorldDir(Dir, Up, Right);
	Vector3 Middle = Point3 + Dir * (m_fLightFunFar + m_fLightFunNear) * 0.5;
	OBB OBB(Dir, Up, Right, (m_fLightFunFar - m_fLightFunNear) * 0.5f, m_fLightFunHeight * 0.5f, m_fLightFunWidth * 0.5f, Middle);
	m_WorldRenderBV = OBB.GetAABB();
	if (HaveLightFun())
	{
		VSCamera LightCamera;
		LightCamera.CreateFromLookDir(Point3, Dir);
		LightCamera.SetOrthogonal(m_fLightFunWidth, m_fLightFunHeight, m_fLightFunNear, m_fLightFunFar);
		LightCamera.UpdateAll(0);
		m_WVP = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();
	}
}

bool VSDirectionLight::IsRelative(VSGeometry * pGeometry)
{
	if (!VSLight::IsRelative(pGeometry))
	{
		return false;
	}
	if (HaveLightFun())
	{
		AABB GeometryAABB = pGeometry->GetWorldAABB();
		if (GeometryAABB.RelationWith(m_WorldRenderBV) == MATH_NOINTERSECT)
		{
			return false;
		}
	}	
	return true;
}

void VSDirectionLight::DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture)
{
	//not for dynamic instance , much instance have huge aabb box , shadow effect will be low.
	if (m_bEnable && m_bIsCastShadow && m_uiShadowType == ST_PROJECT)
	{
		Vector3 Dir,Up,Right;
		GetWorldDir(Dir,Up,Right);
		m_ShadowCuller.GetSceneContent(*CurCuller.GetCamera(), m_pScene, this, dAppTime,false);

		VSArray<VSDirShadowMapCuller> Temp;
		VSMeshNode * pCurMeshNode = NULL;
		for (uint32 t = 0; t <= VSCuller::VST_MAX; t++)
		{
			for (uint32 j = 0; j < m_ShadowCuller.GetVisibleNum(t); j++)
			{
				VSRenderContext& RenderContext = m_ShadowCuller.GetVisibleSpatial(j, t);
				if (pCurMeshNode != RenderContext.m_pMeshNode || RenderContext.IsInstanceData())
				{
					Temp.AddElement(VSDirShadowMapCuller());
					Temp[Temp.GetNum() - 1].PushCameraPlane(*m_ShadowCuller.GetCamera());
					pCurMeshNode = RenderContext.m_pMeshNode;
				}
				Temp[Temp.GetNum() - 1].InsertObject(RenderContext, t);
			}
		}

		m_pProjectShadowSceneRender->m_pNormalDepthTexture = pNormalDepthTexture;
		for (uint32 i = 0; i < Temp.GetNum(); i++)
		{
			VSArray<AABB> CasterAABBArray;
			GetCullerAABBArray(Temp[i], CasterAABBArray);
	
			AABB CasterAABB = GetMaxAABB(CasterAABBArray);
			Vector3 Center = CasterAABB.GetCenter();

			Ray ray(Center, Dir * (-1.0f));

			uint32 Q;
			float tN, tF;
			if (CasterAABB.RelationWith(ray, Q, tN, tF) != MATH_INTERSECT)
			{
				continue;
			}

			Vector3 LigthPT = Center - Dir * tN * 2.0f;

			VSCamera LightCamera;
			LightCamera.CreateFromLookAt(LigthPT, Center);

			LightCamera.UpdateAll(0);
			Matrix3X3W LightView = LightCamera.GetViewMatrix();

			AABB NewCasterAABB;
			NewCasterAABB.Transform(CasterAABB, LightView);
			LightCamera.SetOrthogonal(NewCasterAABB.GetMaxPoint().x - NewCasterAABB.GetMinPoint().x,
				NewCasterAABB.GetMaxPoint().y - NewCasterAABB.GetMinPoint().y,
				NewCasterAABB.GetMinPoint().z, NewCasterAABB.GetMaxPoint().z + 10000.0f);

			m_LightShadowMatrix = LightCamera.GetViewMatrix() * LightCamera.GetProjMatrix();


			m_pProjectShadowRenderTarget = VSResourceManager::GetRenderTarget(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_R16F, 0);
			VSDepthStencil * pDepthStencil = VSResourceManager::GetDepthStencil(m_uiRTWidth, m_uiRTWidth, VSRenderer::SFT_D24S8, 0);

			m_pShadowMapSceneRender->ClearRTAndDepth();
			m_pShadowMapSceneRender->SetDepthStencil(pDepthStencil, VSCuller::RG_NORMAL);
			m_pShadowMapSceneRender->AddRenderTarget(m_pProjectShadowRenderTarget);
			m_pShadowMapSceneRender->Draw(Temp[i], dAppTime);
			m_pShadowTexture[0]->SetTexture((VSTexture *)m_pProjectShadowRenderTarget->GetCreateBy());
			VSResourceManager::DisableOutputResource(pDepthStencil);

			m_pProjectShadowSceneRender->Draw(Temp[i], dAppTime);
			DisableDependenceShadowMap(Temp[i], dAppTime);
		}	
	}
}

bool VSDirectionLight::SetLightMaterial(VSMaterialR * pMaterial)
{
	if (VSLocalLight::SetLightMaterial(pMaterial))
	{
		return true;
	}

	return false;
}

bool VSDirectionLight::Cullby(VSCuller & Culler)
{
	if (HaveLightFun())
	{
		uint32 uiVSF = Culler.IsVisible(m_WorldRenderBV, true);
		if (uiVSF == VSCuller::VSF_ALL || uiVSF == VSCuller::VSF_PARTIAL)
		{
			m_bEnable = true;
			Culler.InsertLight(this);
		}
	}
	else
	{
		m_bEnable = true;
		Culler.InsertLight(this);
	}
	return true;
}

}