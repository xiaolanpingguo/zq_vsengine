#include "graphic/asyn/updatethread.h"
using namespace zq;
VSUpdateContext::VSUpdateContext()
{

}
VSUpdateContext::~VSUpdateContext()
{
	
}
void VSUpdateContext::AddCommand(VSThreadCommand * pCommand)
{
	if (pCommand)
	{
		m_CommandArray.AddElement(pCommand);
	}
}
void VSUpdateContext::Clear()
{
	for (uint32 i = 0 ; i < m_CommandArray.GetNum() ; i++)
	{
		m_CommandArray[i]->~VSThreadCommand();
	}
	m_CommandArray.Clear();
}
uint32 VSUpdateContext::Execute()
{
	for (uint32 i = 0 ; i < m_CommandArray.GetNum() ; i++)
	{
		m_CommandArray[i]->Execute();
	}
	return 1;
}
VSUpdateCenter::VSUpdateCenter(bool bIsMainRun,uint32 uiThreadNum)
{
	m_uiContextNum = uiThreadNum;
	m_bIsMainRun = bIsMainRun;
	if (m_bIsMainRun)
	{
		m_uiContextNum++;
	}
	m_pContextArray = MEM_NEW VSUpdateContext[m_uiContextNum];
	m_uiIndex = -1;
}
VSUpdateCenter::~VSUpdateCenter()
{
	SAFE_DELETEA(m_pContextArray);
}
unsigned char * VSUpdateCenter::Assign(uint32 uiSize)
{
	return m_ConstantBuffer.Assign(uiSize);
}
uint32 VSUpdateCenter::GetIndex()
{
	m_uiIndex++;
	m_uiIndex = m_uiIndex % m_uiContextNum;
	return m_uiIndex;
}
VSUpdateContext * VSUpdateCenter::GetUpdateContext(uint32 i)
{
	return &m_pContextArray[i];
}
uint32 VSUpdateCenter::Execute()
{
	if (m_bIsMainRun)
	{
		m_pContextArray[m_uiContextNum - 1].Execute();
	}
	return 1;
}
void VSUpdateCenter::Clear()
{
	
	for (uint32 i = 0 ; i < m_uiContextNum ; i++)
	{
		m_pContextArray[i].Clear();
	}
	m_ConstantBuffer.Clear();
	m_uiIndex = -1;
}
VSUpdateThread::VSUpdateThread()
{
	static uint32 s_uiThreadID = 0;
	m_uiThreadId = s_uiThreadID;
	s_uiThreadID++;
	static char TempString[128];
	VSSprintf(TempString,128,_T("VSUpdateThread%d"),m_uiThreadId);
	m_ThreadName = TempString;
#ifdef EVENT_UPDATE
	VSSprintf(TempString,128,"VSUpdateEvent%d",m_uiThreadId);
	m_Event.Create(true,TempString);
	m_Event.Reset();
#endif
	m_pUpdateContext = NULL;
}

VSUpdateThread::~VSUpdateThread()
{
	m_pUpdateContext = NULL;
}
void VSUpdateThread::Run()
{
	while(!IsStopTrigger())
	{
		if (m_pUpdateContext)
		{
			#ifdef DEBUG_UPDATE_THREAD
				Synchronize::VSSafeOutputDebugString("Execute Begin : %d\n",m_uiThreadId);
			#endif
			m_pUpdateContext->Execute();		
			m_pUpdateContext = NULL;	
			#ifdef DEBUG_UPDATE_THREAD
				Synchronize::VSSafeOutputDebugString("Execute End : %d\n",m_uiThreadId);
			#endif
			#ifdef EVENT_UPDATE
				m_Event.Trigger();
			#endif
			
		}	
	}
}
void VSUpdateThread::SetUpdateContext(VSUpdateContext * pUpdateContex)
{
	m_pUpdateContext = pUpdateContex;
	#ifdef EVENT_UPDATE
		m_Event.Reset();
	#endif
}
bool VSUpdateThread::IsReady()const
{
	return !m_pUpdateContext;
}
VSUpdateThreadSys * VSUpdateThreadSys::ms_pUpdateThreadSys = NULL;
VSUpdateThreadSys::VSUpdateThreadSys(uint32 uiThreadNum)
{
	VSMAC_ASSERT(uiThreadNum);
	VSMAC_ASSERT(!ms_pUpdateThreadSys);
	ms_pUpdateThreadSys = this;

	m_uiThreadNum = uiThreadNum;
	m_UpdateThread.SetBufferNum(m_uiThreadNum);
#ifdef DEBUG_UPDATE_THREAD
	for (uint32 i = 0 ; i < m_UpdateThread.GetNum() ; i++)
	{
		m_UpdateThread[i].m_uiThreadId = i;
	}
#endif
#ifdef EVENT_UPDATE
	m_WaitEvent.SetBufferNum(m_uiThreadNum);
	for (uint32 i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_WaitEvent[i] = &m_UpdateThread[i].m_Event;
	}
#endif
	m_uiCurUpdateType = UPDATE_NODE;
	m_UpdateCenter[UPDATE_NODE] = MEM_NEW VSUpdateCenter(true,m_uiThreadNum);
#ifdef CONTEXT_UPDATE
	for (uint32 i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Start();
	}
#endif
}
VSUpdateThreadSys::~VSUpdateThreadSys()
{
	for (uint32 i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Start();
	}
	for (uint32 i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Stop();
	}
	SAFE_DELETE(m_UpdateCenter[UPDATE_NODE]);
}
void VSUpdateThreadSys::Begin()
{

#ifdef DEBUG_UPDATE_THREAD
	Synchronize::VSSafeOutputDebugString("*************************Start****************\n");
#endif
	for (uint32 i = 0 ; i < m_uiThreadNum; i++)
	{
		VSUpdateContext* pUpdateContex = m_UpdateCenter[m_uiCurUpdateType]->GetUpdateContext(i);
		m_UpdateThread[i].SetUpdateContext(pUpdateContex->GetCommandNum() ? pUpdateContex : NULL);
	}
/*#ifdef EVENT_UPDATE*/
	for (uint32 i = 0 ; i < m_uiThreadNum ; i++)
	{
		if (!m_UpdateThread[i].IsReady())
		{
			m_UpdateThread[i].Start();
		}
	}
/*#endif*/
	m_UpdateCenter[m_uiCurUpdateType]->Execute();

	
}
void VSUpdateThreadSys::ExChange()
{
#ifdef EVENT_UPDATE
	// 	for (uint32 i = 0 ; i < m_uiThreadNum ; i++)
	// 	{
	// 		m_UpdateThread[i].m_Event.Wait();
	// 
	// 	}
	Synchronize::WaitAll(m_WaitEvent.GetBuffer(),m_uiThreadNum,true);
#endif
#ifdef DEBUG_UPDATE_THREAD
	Synchronize::VSSafeOutputDebugString("**Exchange**\n");
#endif
#ifdef CONTEXT_UPDATE
	for (uint32 i = 0 ; i < m_uiThreadNum ; i++)
	{
		while(1)
		{
			if (m_UpdateThread[i].IsReady())
			{
				break;
			}
			Thread::Sleep(0);
		}
	}
#endif
/*#ifdef EVENT_UPDATE*/
	for (uint32 i = 0 ; i < m_uiThreadNum ; i++)
	{
		m_UpdateThread[i].Suspend();
	}
/*#endif*/
#ifdef DEBUG_UPDATE_THREAD
	Synchronize::VSSafeOutputDebugString("system clear\n");
#endif
	m_UpdateCenter[m_uiCurUpdateType]->Clear();
}
