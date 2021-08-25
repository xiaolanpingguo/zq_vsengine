#include "graphic/core/streamingmanager.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/asyn/updatethread.h"
#include "graphic/render/scenemanager/scenemanager.h"
#include "core/timer.h"
#include "graphic/core/profiler.h"


namespace zq{


VSStreamingManager* VSStreamingManager::ms_pStreamingManager = NULL;
DECLEAR_TIME_PROFILENODE(VSStreamingManagerUpdate, Update)
DECLEAR_TIME_PROFILENODE(GetStreamInfo, VSStreamingManagerUpdate)
VSStreamingManager::VSStreamingManager() : m_StreamObjectList(false)
{
	ms_pStreamingManager = this;
}

VSStreamingManager::~VSStreamingManager()
{
	ReleaseAllStream();
}

void VSStreamingManager::ReleaseAllStream()
{
	for (VSList<StreamElementType>::VSListIterator t = m_StreamObjectList.Begin(); t != m_StreamObjectList.End(); ++t)
	{
		if (!t->bFree)
		{
			StreamElementType& StreamElement = *t;
			for (auto &Temp : StreamElement.pResourceProxy)
			{
				Temp->ReleaseStreamObject();
			}
			StreamElement.pResourceProxy.Clear();
			t->bFree = true;
		}
	}
}

void VSStreamingManager::GetStreamInfo(const AABB & WorldAABB ,StreamInformation_TYPE & StreamInformation)
{
	if (StreamInformation.ProjectInfo.z > EPSILON_E3)
	{
		float DistSqr = (WorldAABB.GetCenter() - StreamInformation.CameraPos).GetSqrLength();
		float RadiusSqr = (WorldAABB.GetCenter() - WorldAABB.GetMaxPoint()).GetSqrLength();

		float MaxProjectXY = Max(StreamInformation.ProjectInfo.x, StreamInformation.ProjectInfo.y);
		float MaxProjectXYSqr = MaxProjectXY * MaxProjectXY * RadiusSqr;
		StreamInformation.m_fStreamInfo =  Clamp((MaxProjectXYSqr / Max(1.0f, DistSqr)) * StreamInformation.ProjectInfo.w, 1.0f, 0.0f);
	}
	else
	{
		StreamInformation.m_fStreamInfo = 1.0f;
	}
}

void VSStreamingManager::Update(double dAppTime)
{
	ADD_TIME_PROFILE(VSStreamingManagerUpdate)
	GetStreamInfo();
	if (VSResourceManager::ms_bUpdateThread)
	{
		VSUpdateThreadSys::ms_pUpdateThreadSys->Begin();
		VSUpdateThreadSys::ms_pUpdateThreadSys->ExChange();
	}
	DispathStream();
}

void VSStreamingManager::GetStreamInfo()
{
	ADD_TIME_PROFILE(GetStreamInfo)
	for (uint32 i = 0; i < VSSceneManager::ms_pSceneManager->GetViewFamilyNum(); i++)
	{
		VSViewFamily* pViewFamily = VSSceneManager::ms_pSceneManager->GetViewFamily(i);
		for (uint32 j = 0; j < pViewFamily->GetSceneNum(); j++)
		{
			VSScene* pScene = pViewFamily->GetScene(j);
			Vector3 CameraPos;
			Vector3W ProjectInfo;
			if (pViewFamily->GetStreamCameraInfo(CameraPos, ProjectInfo))
			{
				pScene->GetStreamResourceWithPos(CameraPos, ProjectInfo);
			}
		}
	}
}

void VSStreamingManager::DispathStream()
{
	for (VSList<StreamElementType>::VSListIterator t = m_StreamObjectList.Begin(); t != m_StreamObjectList.End(); ++t)
	{
		if (!t->bFree)
		{
			StreamElementType &StreamElement = *t;
			for(uint32 i = 0 ; i < StreamElement.pResourceProxy.GetNum();)
			{
				auto Temp = StreamElement.pResourceProxy[i];
				if (Temp->m_StreamState == STREAM_STATE::SS_READY_STREAM)
				{
					Temp->SetStreamJob();
					i++;
				}
				else if (Temp->m_StreamState == STREAM_STATE::SS_STREAMING)
				{
					i++;
				}
				else if (Temp->m_StreamState == STREAM_STATE::SS_END_STREAM)
				{
					Temp->StreamEnd();
					StreamElement.pResourceProxy.Erase(i);
				}
			}
			if (!StreamElement.pResourceProxy.GetNum())
			{
				t->bFree = true;
			}
		}
	}
}

StreamElementType& VSStreamingManager::GetFreeSlot()
{
	for (VSList<StreamElementType>::VSListIterator t = m_StreamObjectList.Begin(); t != m_StreamObjectList.End(); ++t)
	{
		if (t->bFree)
		{
			t->bFree = false;
			return *t;
		}
	}
	m_StreamObjectList.AddElement(StreamElementType());
	(*m_StreamObjectList.Tail()).bFree = false;
	return *m_StreamObjectList.Tail();
}

}
