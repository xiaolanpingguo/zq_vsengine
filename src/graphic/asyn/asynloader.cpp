#include "graphic/asyn/asynloader.h"
#include "graphic/core/graphicinclude.h"
#include "core/image.h"
#include "graphic/render/texture/texture2d.h"
#include "graphic/core/nvcompression.h"
#include "core/timer.h"
#include "graphic/core/profiler.h"
using namespace zq;						
DECLEAR_TIME_PROFILENODE(VSASYNLoadManagerUpdate, ApplicationUpdate)
VSAsynJob::VSAsynJob()
{
	m_uiControllState = CS_NONE;
	m_uiJobState = JS_PENDING;
}
VSAsynJob::~VSAsynJob()
{

}
VSFileJob::VSFileJob()
{
	m_pBuffer = NULL;
}
VSFileJob::~VSFileJob()
{
	SAFE_DELETEA(m_pBuffer);
}
void VSFileJob::AsynThreadProcess()
{
	File File;
	if(!File.Open(m_FileName.GetBuffer(),File::OM_RB))
	{
		m_uiJobState = JS_FAIL;
		return ;
	}

	m_uiSize = File.GetFileSize();
	m_pBuffer = MEM_NEW unsigned char[m_uiSize];
	if(!m_pBuffer)
	{
		m_uiJobState = JS_FAIL;
		return ;
	}
	if(!File.Read(m_pBuffer,m_uiSize,1))
	{
		m_uiJobState = JS_FAIL;
		return ;
	}
	
	return ;
	
}
bool VSResourceJob::ms_bEnableBreakLoadResouce = true;
VSResourceJob::VSResourceJob() : m_LoadStream(VSStream::ASYN_LOAD_FLAG)
{
	
}
VSResourceJob::~VSResourceJob()
{

}
void VSResourceJob::AsynThreadProcess()
{
	if (!ms_bEnableBreakLoadResouce)
	{
		if (m_uiJobState == JS_PENDING)
		{
			VSFileJob::AsynThreadProcess();
			if (m_uiJobState != JS_FAIL)
			{
				m_uiJobState = JS_MAIN_THREAD;
			}
		}	
		return;
	}
	if (m_uiJobState == JS_WaitLoadFromDisk_AnyThread)
	{
		VSFileJob::AsynThreadProcess();
		if (m_uiJobState == JS_FAIL)
		{
			return;
		}
		if (!m_BreakLoadStream.LoadFromBuffer(m_pBuffer, m_uiSize))
		{
			m_uiJobState = JS_FAIL;
			return;
		}
		m_uiJobState = JS_CreateLoadObject_MainThread;
	}
	else if(m_uiJobState == JS_LoadAndLinkObject_AnyThread)
	{
		m_BreakLoadStream.LoadAndLinkOjbect();
		m_uiJobState = JS_GetResource_MainThread;
	}
}
void VSResourceJob::MainThreadProcess()
{
	if (!ms_bEnableBreakLoadResouce)
	{
		if (m_uiJobState == JS_MAIN_THREAD)
		{	
			if (!m_LoadStream.LoadFromBuffer(m_pBuffer, m_uiSize))
			{
				m_uiJobState = JS_FAIL;
			}
		}
		return;
	}
	if (m_uiJobState == JS_CreateLoadObject_MainThread)
	{
		m_BreakLoadStream.CreateAndRegisterObject();
		m_uiJobState = JS_LoadAndLinkObject_AnyThread;
		VSASYNLoadManager::ms_pASYNLoadManager->AddHaveJob(this);
	}
}
void VSResourceJob::Failed()
{
	m_BreakLoadStream.GetObjectFailed();
}
void VSAsynJobThread::Stop()
{	
	Thread::Stop();
}
VSAsynJobThread::VSAsynJobThread()
{
	m_pResourceQueue.Clear();
}
VSAsynJobThread::~VSAsynJobThread()
{
	m_pResourceQueue.Clear();
}
bool VSAsynJobThread::AddJob(VSAsynJob* m_pJob)
{	
	bool bAddJobed = m_pResourceQueue.TryEnqueue(m_pJob);
	if (bAddJobed)
	{
		Start();
	}
	return bAddJobed;
}
void VSAsynJobThread::Run()
{
	while(!IsStopTrigger())
	{
		while(!m_pResourceQueue.IsEmpty())
		{
			VSAsynJob* pJob = NULL;
			m_pResourceQueue.Dequeue(pJob);
			pJob->AsynThreadProcess();			
		}
	}

}
VSASYNLoadManager *VSASYNLoadManager::ms_pASYNLoadManager = NULL;
VSASYNLoadManager::VSASYNLoadManager()
{
	
	m_JobArray.Clear();
	VSMAC_ASSERT(!ms_pASYNLoadManager);
	ms_pASYNLoadManager = this;
}
void VSASYNLoadManager::FlushAllJob()
{
	while (m_JobArray.GetNum() > 0) // Must Load All Resource  
	{
		PendingResource(10.0f);
	}
	for (uint32 i = 0; i < TASK_THREAD_NUM; i++)
	{
		m_AsynJobThread[i].Suspend();
	}
}
VSASYNLoadManager::~VSASYNLoadManager()
{
	while (m_JobArray.GetNum() > 0) // Must Load All Resource  
	{
		PendingResource(10.0f);
	}	
	for (uint32 i = 0; i < TASK_THREAD_NUM; i++)
	{
		m_AsynJobThread[i].Start();
		m_AsynJobThread[i].Stop();
	}
}
void VSASYNLoadManager::PendingResource(float fTime)
{
	uint32 i = 0;
	float t1 = (float)Timer::ms_pTimer->GetGamePlayTime();
	while (i < m_JobArray.GetNum())
	{
		if (m_JobArray[i]->m_uiControllState == VSAsynJob::CS_DELETE)
		{
			m_JobArray[i]->Failed();
			SAFE_DELETE(m_JobArray[i]);
			m_JobArray.Erase(i);
			continue;
		}
		else if (m_JobArray[i]->m_uiControllState == VSAsynJob::CS_PENDING_HAVE_JOB)
		{
			AddHaveJob(m_JobArray[i]);
			i++;
			continue;
		}
		else if (m_JobArray[i]->m_uiControllState == VSAsynJob::CS_PENDING_JOB)
		{
			AddJob(m_JobArray[i]);
			i++;
			continue;
		}
		uint32 uiJobState = m_JobArray[i]->m_uiJobState;
		if (uiJobState == VSResourceJob::JS_FAIL)
		{
			m_JobArray[i]->Failed();
			SAFE_DELETE(m_JobArray[i]);
			m_JobArray.Erase(i);
			continue;
		}
		else if (uiJobState >= VSResourceJob::JS_PENDING && uiJobState < VSResourceJob::JS_MAIN_THREAD)
		{
			i++;
		}
		else if (uiJobState >= VSResourceJob::JS_MAIN_THREAD && uiJobState < VSResourceJob::JS_SUCCESS)
		{
			m_JobArray[i]->MainThreadProcess();
			i++;
		}
		else if (uiJobState == VSResourceJob::JS_SUCCESS)
		{
			SAFE_DELETE(m_JobArray[i]);
			m_JobArray.Erase(i);
		}
		float t2 = (float)Timer::ms_pTimer->GetGamePlayTime() - t1;
		if (t2 > fTime)
		{
			break;
		}
	}
}
void VSASYNLoadManager::Update(double AppTime)
{
	ADD_TIME_PROFILE(VSASYNLoadManagerUpdate)
	PendingResource(10.0f);
	if (m_JobArray.GetNum() == 0)
	{
		for (uint32 i = 0; i < TASK_THREAD_NUM; i++)
		{
			m_AsynJobThread[i].Suspend();
		}
	}
}
void VSASYNLoadManager::AddJob(VSAsynJob * pJob)
{
	if (pJob->m_uiControllState == VSAsynJob::CS_DELETE)
	{
		return ;
	}

	if (pJob->m_uiControllState == VSAsynJob::CS_PENDING_JOB)
	{
		if (m_AsynJobThread[0].AddJob(pJob))
		{
			pJob->m_uiControllState = VSAsynJob::CS_NONE;
		}
	}
	else
	{
		if (!m_AsynJobThread[0].AddJob(pJob))
		{
			pJob->m_uiControllState = VSAsynJob::CS_PENDING_JOB;
		}
		m_JobArray.AddElement(pJob);
	}
	

}
void VSASYNLoadManager::AddHaveJob(VSAsynJob * pJob)
{
	if (pJob->m_uiControllState == VSAsynJob::CS_DELETE)
	{
		return;
	}
	if (!m_AsynJobThread[1].AddJob(pJob))
	{
		pJob->m_uiControllState = VSAsynJob::CS_PENDING_HAVE_JOB;
	}
	else
	{
		pJob->m_uiControllState = VSAsynJob::CS_NONE;
	}
}
void VSASYNLoadManager::DeleteLoadResource(VSFileName & FileName)
{
	for (uint32 i = 0; i < m_JobArray.GetNum(); i++)
	{
		if (m_JobArray[i]->GetJobType() == VSResource::RT_MAP)
		{
			VSResourceJob * pResourceJob = (VSResourceJob *)m_JobArray[i];
			if (pResourceJob->GetResourceType() == VSResource::RT_MAP)
			{
				if (pResourceJob->m_FileName == FileName)
				{
					pResourceJob->m_uiControllState = VSAsynJob::CS_DELETE;
					return;
				}
			}			
		}
	}
}