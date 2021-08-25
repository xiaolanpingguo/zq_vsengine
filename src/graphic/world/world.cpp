#include "graphic/world/world.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/world/actor/staticactor.h"
#include "graphic/world/actor/skeletonactor.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/render/scenemanager/scenemanager.h"
#include "graphic/world/actor/lightactor.h"
#include "graphic/core/profiler.h"
using namespace zq;
IMPLEMENT_RTTI(VSWorld,VSObject)
BEGIN_ADD_PROPERTY(VSWorld,VSObject)
REGISTER_PROPERTY(m_SceneArray, SceneArray, VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSWorld)
IMPLEMENT_INITIAL_END
VSWorld * VSWorld::ms_pWorld = NULL;
DECLEAR_TIME_PROFILENODE(VSWorldUpdate, Update)
VSWorld::VSWorld()
{
	VSMAC_ASSERT(ms_pWorld == NULL);
	ms_pWorld = this;
	VSSceneMap * pSceneMap = MEM_NEW VSSceneMap("Main");
	VSResourceManager::AddGCObject(pSceneMap);
	m_SceneArray.AddElement(pSceneMap);
	VSResourceManager::AddRootObject(this);
	IncreRef();//default reference = 1 ,避免智能指针指向后释放。
}
VSWorld::~VSWorld()
{
	VSResourceManager::DeleteAllMapResource();
	for (uint32 i = 0; i < m_SceneArray.GetNum();i++)
	{
		if (m_SceneArray[i])
		{
			m_SceneArray[i]->OnDestroy();
		}
	}
	m_SceneArray.Clear();
	m_ActorArray.Clear();
	VSResourceManager::GCObject();
	VSResourceManager::RunAllGCTask();
}
void VSWorld::DestroyScene(const VSUsedName & Name)
{
	//first scene  must be not delete
	for (uint32 i = 1; i < m_SceneArray.GetNum(); i++)
	{
		if (m_SceneArray[i]->m_Name == Name)
		{
			for (uint32 j = 0; j < m_ActorArray.GetNum();)
			{
				if (m_ActorArray[j]->m_pSceneMap == m_SceneArray[i])
				{
					DestroyActor(m_ActorArray[j]);
				}
				else
				{
					j++;
				}
			}
			m_SceneArray[i]->OnDestroy();
			m_SceneArray[i]->SetFlag(OF_PendingKill);
			m_SceneArray.Erase(i);
			return;
		}
	}

}

VSSceneMap* VSWorld::CreateScene(const char * pName)
{
	for (uint32 i = 0 ; i < m_SceneArray.GetNum() ;i++)
	{
		if(m_SceneArray[i]->m_Name == pName)
			return NULL;

	}
	VSSceneMap * pSceneMap = MEM_NEW VSSceneMap(pName);
	VSResourceManager::AddGCObject(pSceneMap);
	m_SceneArray.AddElement(pSceneMap);
	return pSceneMap;
}
VSSceneMap * VSWorld::GetScene(const VSUsedName & Name)
{
	for (uint32 i = 0 ; i < m_SceneArray.GetNum() ;i++)
	{
		if(m_SceneArray[i]->m_Name == Name)
			return m_SceneArray[i];

	}
	return NULL;
}
void VSWorld::ProcessInput(uint32 uiInputType, uint32 uiEvent, uint32 uiKey, int32 x, int32 y, int32 z)
{

	for (uint32 i = 0; i < m_ActorArray.GetNum(); i++)
	{
		if (!m_ActorArray[i]->IsHasFlag(VSObject::OF_PendingKill))
		{
			m_ActorArray[i]->ProcessInput(uiInputType,uiEvent,uiKey,x,y,z);
		}
	}
}
void VSWorld::Update(double dAppTime)
{
	ADD_TIME_PROFILE(VSWorldUpdate)
	static double LastTime = dAppTime;
	double DetTime = dAppTime - LastTime;
	LastTime = dAppTime;

	for (uint32 i = 0 ; i < m_ActorArray.GetNum() ; i++)
	{
		if (!m_ActorArray[i]->IsHasFlag(VSObject::OF_PendingKill))
		{
			m_ActorArray[i]->Update(dAppTime);
		}
	}
	uint32 i = 0 ;
	while(i < m_MessageArray.GetNum())
	{
		if (m_MessageArray[i].DispatchTime <= 0.0)
		{
			VSActor *pActor = GetActor(m_MessageArray[i].Receiver);
			if (pActor)
			{
				pActor->HandleMessage(m_MessageArray[i]);
			}
			m_MessageArray.Erase(i);
		}
		else
		{
			m_MessageArray[i].DispatchTime -= DetTime;
			i++;
		}
	}

}
VSActor *VSWorld::GetActor(VSUsedName Name)
{
	for (uint32 i = 0 ; i < m_ActorArray.GetNum() ; i++)
	{
		if (m_ActorArray[i]->m_ActorName == Name)
		{

			return m_ActorArray[i];
		}
	}	
	return NULL;
}

void VSWorld::DeleteActor(VSActor * pActor)
{

	for (uint32 i = 0 ; i < m_ActorArray.GetNum() ; i++)
	{
		if (m_ActorArray[i] == pActor)
		{
			m_ActorArray.Erase(i);
			return ;
		}
	}
}
void VSWorld::AddActor(VSActor * pActor)
{
	if (pActor)
	{
		m_ActorArray.AddElement(pActor);
	}
	
}
bool VSWorld::LoadMap(const char * MapPath, bool IsAsyn)
{
	if (!MapPath)
	{
		return false;
	}
	VSResourceProxyBase * pResource = VSResourceManager::LoadASYNResource<VSSceneMap>(MapPath, IsAsyn);
	if (!pResource)
	{
		return false;
	}
	else
	{
		pResource->AddLoadEventObject(this);
		
	}
	return true;
}
bool VSWorld::UnLoadMap(const char * MapPath)
{
	if (!MapPath)
	{
		return false;
	}
	VSResourceManager::DeleteMapResource(MapPath);

	return true;
}
VSActor * VSWorld::CreateActor(const char *  ActorPath, const Vector3 & Pos, const Matrix3X3 & Rotate, const Vector3 &Scale, VSSceneMap * pSceneMap)
{
	if (!ActorPath)
	{
		return NULL;
	}
	VSActor * pActor = NULL;
	VSResourceProxyBase * pResource = VSResourceManager::LoadResource(ActorPath,false);
	if (pResource)
	{
		uint32 ResourceType = pResource->GetResourceType();
		if (ResourceType == VSResource::RT_STATIC_MODEL)
		{
			pActor = CreateActor<VSStaticActor>(Pos, Rotate, Scale,pSceneMap);
			((VSStaticMeshComponent *)pActor->GetActorNode())->SetStaticMeshResource((VSStaticMeshNodeR *)pResource);
		}
		else if (ResourceType == VSResource::RT_SKELECTON_MODEL)
		{
			pActor = CreateActor<VSSkeletonActor>(Pos, Rotate, Scale, pSceneMap);
			((VSSkeletonMeshComponent *)pActor->GetActorNode())->SetSkeletonMeshResource((VSSkeletonMeshNodeR *)pResource);
		}
		else if (ResourceType == VSResource::RT_ACTOR)
		{
			if (!pResource->IsLoaded())
			{
				uint32 i = 0;
				for (; i < m_SceneArray.GetNum(); i++)
				{
					if (m_SceneArray[i]->m_Name == pSceneMap->m_Name)
						break;
				}
				pResource->AddLoadEventObject(this,(void *)GUID32ToSizeType(i));
				pActor = NULL;
			}
			else
			{
				pActor = (VSActor *)VSObject::CloneCreateObject(((VSActorR *)pResource)->GetResource());
				if (pSceneMap)
				{
					pSceneMap->AddActor(pActor);
				}
				else
				{
					m_SceneArray[0]->AddActor(pActor);
				}
			}
		}
	}
	return pActor;
}
void VSWorld::AddSceneMap(VSSceneMap * pSceneMap)
{
	VSResourceManager::AddGCObject(pSceneMap);
	for (uint32 i = 0; i < pSceneMap->m_pActorArray.GetNum(); i++)
	{
		AddActor(pSceneMap->m_pActorArray[i]);
	}
	m_SceneArray.AddElement(pSceneMap);
}
void VSWorld::LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data)
{
	if (pResourceProxy->GetResourceType() == VSResource::RT_ACTOR)
	{
		VSActor * pActor = (VSActor *)VSObject::CloneCreateObject(((VSActorR *)pResourceProxy)->GetResource());
		uint32 i = SizeTypeToGUID32((uint64)Data);
		m_SceneArray[i]->AddActor(pActor);
	}
	else if (pResourceProxy->GetResourceType() == VSResource::RT_MAP)
	{
		VSSceneMap * pSceneMap = ((VSResourceProxy<VSSceneMap> *)pResourceProxy)->GetResource();
		AddSceneMap(pSceneMap);
	}

}
void VSWorld::DestroyActor(VSActor * pActor)
{
	pActor->OnDestory();
	pActor->SetFlag(OF_PendingKill);
	DeleteActor(pActor);

}

void VSWorld::AddMessage(const VSMessage & Message)
{
	m_MessageArray.AddElement(Message);
}
void VSWorld::AttachCaptureViewFamilyToCamera(VSCameraActor * pCameraActor,
									 uint32 uiCaptureViewFamilyType,const VSString & ViewFamilyName,
									 uint32 uiWidth,uint32 uiHeight,
									 VSArray<VSString> & SceneMapName,
									 const char * RenderMethodRTTIName,
									 VSPostEffectSetR*  pPostEffectSet)
{
	VSCamera * pCamera = (VSCamera *)pCameraActor->GetActorNode();
	if (pCamera)
	{
		VSViewFamliyInfo * pInfo = MEM_NEW VSViewFamliyInfo();
		
		VSViewFamily * pViewFamily = NULL;
		if (uiCaptureViewFamilyType == VSCaptureViewFamily::VT_CAPTURE_2D_CAPTURE_NORMAL)
		{
			pViewFamily = MEM_NEW VS2DCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera,pPostEffectSet,RenderMethodRTTIName);

		}
		else if (uiCaptureViewFamilyType == VSCaptureViewFamily::VT_CAPTURE_CUB_CAPTURE_NORMAL)
		{
			pViewFamily = MEM_NEW VSCubCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera,pPostEffectSet,RenderMethodRTTIName);
		}

		pInfo->m_SceneMapName = SceneMapName;
		pInfo->m_uiCaptureViewFamilyType = uiCaptureViewFamilyType;
		pInfo->m_uiHeight = uiHeight;
		pInfo->m_uiWidth = uiWidth;
		pInfo->m_pPostEffect = pPostEffectSet;
		pInfo->m_ViewFamilyName = ViewFamilyName;
		pInfo->m_pViewFamily = pViewFamily;
		pCameraActor->m_ViewFamliyInfoArray.AddElement(pInfo);
		pInfo->GetScene();
	}
}
void VSWorld::AttachWindowViewFamilyToCamera(VSCameraActor * pCameraActor,
									uint32 uiWindowViewFamilyType,const VSString & ViewFamilyName,
									VSArray<VSString> & SceneMapName,
									const char * RenderMethodRTTIName,
									int32 iWindowID,				
									VSPostEffectSetR* pPostEffectSet)
{
	VSCamera * pCamera = (VSCamera *)pCameraActor->GetActorNode();
	if (pCamera)
	{
		VSViewFamliyInfo * pInfo = MEM_NEW VSViewFamliyInfo();

		VSViewFamily * pViewFamily = NULL;
		if (uiWindowViewFamilyType == VSWindowViewFamily::VT_WINDOW_NORMAL)
		{
			pViewFamily= MEM_NEW VSWindowViewFamily(ViewFamilyName,pCamera,pPostEffectSet,RenderMethodRTTIName,iWindowID);
			
			
		}
		pInfo->m_SceneMapName = SceneMapName;
		pInfo->m_uiCaptureViewFamilyType = uiWindowViewFamilyType;
		pInfo->m_pPostEffect = pPostEffectSet;
		pInfo->m_ViewFamilyName = ViewFamilyName;
		pInfo->m_pViewFamily = pViewFamily;
		pCameraActor->m_ViewFamliyInfoArray.AddElement(pInfo);
		pInfo->GetScene();
	}
}
void VSWorld::UpdateLightActorScene(VSLightActor * pActor)
{
	VSLight * pLightNode = pActor->GetTypeNode();
	VSLocalLight * pLocalLight = DynamicCast<VSLocalLight>(pLightNode);
	if (pLocalLight)
	{
		pLocalLight->ClearAllShadowScene();
		for (uint32 i = 0 ; i < m_SceneArray.GetNum() ; i++)
		{
			pLocalLight->AddShadowScene(m_SceneArray[i]->m_pScene);
		}
	}
	
}