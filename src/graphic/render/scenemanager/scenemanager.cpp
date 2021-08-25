#include "graphic/render/scenemanager/scenemanager.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/asyn/updatethread.h"
#include "core/timer.h"
#include "graphic/core/profiler.h"
using namespace zq;
VSSceneManager * VSSceneManager::ms_pSceneManager = NULL;
DECLEAR_TIME_PROFILENODE(VSSceneManagerUpdate,Update)
DECLEAR_TIME_PROFILENODE(VSSceneManagerDraw,Update)
DECLEAR_TIME_PROFILENODE(ViewFamilyUpdate,VSSceneManagerDraw);
DECLEAR_TIME_PROFILENODE(ViewFamilyDraw, VSSceneManagerDraw);
VSSceneManager::VSSceneManager()
{
	
	ms_pSceneManager = this;
}
VSSceneManager::~VSSceneManager()
{
	
}

void VSSceneManager::Update(double dAppTime)
{
	ADD_TIME_PROFILE(VSSceneManagerUpdate)
	for (uint32 i = 0 ;i < m_pScene.GetNum() ; i++)
	{
		m_pScene[i]->Update(dAppTime);
	}
	if (VSResourceManager::ms_bUpdateThread)
	{
		VSUpdateThreadSys::ms_pUpdateThreadSys->Begin();
/*		double Time = Timer::ms_pTimer->GetGamePlayTime();*/
		VSUpdateThreadSys::ms_pUpdateThreadSys->ExChange();
// 		Time = Timer::ms_pTimer->GetGamePlayTime() - Time;
// 		VSOutputDebugString("Time %f\n",Time);
	}
	
	for (uint32 i = 0 ;i < m_pScene.GetNum() ; i++)
	{
		m_pScene[i]->CollectUpdateInfo();
	}
}

void VSSceneManager::Draw(double dAppTime)
{
	ADD_TIME_PROFILE(VSSceneManagerDraw)
	for (uint32 i = 0 ; i < m_pViewFamily.GetNum() ;i++)
	{
		if (m_pViewFamily[i] && m_pViewFamily[i]->m_bEnable)
		{
			{
				ADD_TIME_PROFILE(ViewFamilyUpdate)
				m_pViewFamily[i]->Update(dAppTime);
			}

			{
				ADD_TIME_PROFILE(ViewFamilyDraw)
				m_pViewFamily[i]->Draw(dAppTime);
			}
			
		}
		
	}
	for (uint32 i = 0 ;i < m_pScene.GetNum() ; i++)
	{
		for (uint32 j = 0 ; j < m_pScene[i]->GetAllLightNum() ;j++)
		{
			VSLocalLight * pLight = DynamicCast<VSLocalLight>(m_pScene[i]->GetAllLight(j));
			if (pLight)
			{
				pLight->DrawLightMaterial(dAppTime);
				pLight->DisableNoDependenceShadowMap(dAppTime);
			}
			
		}

	}
}
