#include "graphic/world/scenemap.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/render/scenemanager/scenemanager.h"


namespace zq{


IMPLEMENT_RTTI(VSSceneMap,VSObject)
BEGIN_ADD_PROPERTY(VSSceneMap,VSObject)
REGISTER_PROPERTY(m_Name, Name, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pScene, Scene, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_NO_USE_GC)
REGISTER_PROPERTY(m_pActorArray, ActorArray, VSProperty::F_SAVE_LOAD_CLONE  | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSSceneMap)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
IMPLEMENT_RESOURCE(VSSceneMap)
VSSceneMap::VSSceneMap(const char * pName)
{
	m_Name = pName;
	m_pScene = MEM_NEW VSScene();
	VSSceneManager::ms_pSceneManager->AddScene(m_pScene);	
}
VSSceneMap::~VSSceneMap()
{	
	m_pScene = NULL;
}
VSSceneMap::VSSceneMap()
{
	
}
void VSSceneMap::OnDestroy()
{
	VSSceneManager::ms_pSceneManager->DeleteScene(m_pScene);
	m_pScene = NULL;
}
bool VSSceneMap::InitialDefaultState()
{
	ms_Default = NULL;// MEM_NEW VSSceneMap();
	return true;
}
bool VSSceneMap::TerminalDefaultState()
{
	ms_Default = NULL;
	return true;
}
void VSSceneMap::AddActor(VSActor * pActor)
{

	if (pActor)
	{
		for (uint32 i = 0 ; i < m_pActorArray.GetNum() ; i++)
		{
			if (pActor == m_pActorArray[i])
			{
				pActor->m_pSceneMap = this;
				return ;
			}
		}
		if (pActor->m_pSceneMap)
		{
			pActor->m_pSceneMap->DeleteActor(pActor);
		}
		
		m_pActorArray.AddElement(pActor);

		m_pScene->AddObject(pActor->GetActorNode());

		pActor->AddToSceneMap(this);
	}
}
VSActor *VSSceneMap::GetActor(VSUsedName Name)
{
	for (uint32 i = 0 ; i < m_pActorArray.GetNum() ; i++)
	{
		if (m_pActorArray[i]->m_ActorName == Name)
		{
			
			return m_pActorArray[i];
		}
	}	
	return NULL;
}

void VSSceneMap::DeleteActor(VSActor * pActor)
{

	for (uint32 i = 0 ; i < m_pActorArray.GetNum() ; i++)
	{
		if (m_pActorArray[i] == pActor)
		{
			if (m_pActorArray[i]->GetOwner())
			{
				m_pActorArray[i]->GetOwner()->DeleteChildActor(m_pActorArray[i]);
			}
			else
			{
				m_pScene->DeleteObject(m_pActorArray[i]->GetActorNode());
			}
			m_pActorArray[i]->AddToSceneMap(NULL);
			m_pActorArray.Erase(i);
			return ;
		}
	}
}
bool VSSceneMap::PostLoad(VSStream* pStream)
{
	VSSceneManager::ms_pSceneManager->AddScene(m_pScene);
	return  true;
}

}