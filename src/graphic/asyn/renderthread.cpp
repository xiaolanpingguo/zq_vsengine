#include "graphic/asyn/renderthread.h"
#include "graphic/material/shader/userconstant.h"
#include "graphic/render/bind.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/profiler.h"
using namespace zq;
//#define DEBUG_RENDER_THREAD
VSRenderThreadBuffer::VSRenderThreadBuffer()
{

}
VSRenderThreadBuffer::~VSRenderThreadBuffer()
{
	
}
unsigned char * VSRenderThreadBuffer::Assign(uint32 uiSize)
{
	return m_ConstantBuffer.Assign(uiSize);
}
unsigned char * VSRenderThreadBuffer::Assign(uint32 VTType,uint32 uiRegisterNum)
{
	return m_ConstantBuffer.Assign(VTType,uiRegisterNum);
}
void VSRenderThreadBuffer::Clear()
{
#ifdef DEBUG_RENDER_THREAD
	Synchronize::VSSafeOutputDebugString("**Clear**\n");
#endif
	for (uint32 i = 0 ; i < m_CommandList.GetNum() ; i++)
	{
		m_CommandList[i]->~VSThreadCommand();
	}
	m_ConstantBuffer.Clear();
	m_CommandList.Clear();
}

void VSRenderThreadBuffer::Execute(bool bMustFlust)
{
#ifdef DEBUG_RENDER_THREAD
	Synchronize::VSSafeOutputDebugString("==========\n");
	Synchronize::VSSafeOutputDebugString("Command Num : %d\n",m_CommandList.GetNum());
#endif
	if (bMustFlust)
	{
		for (uint32 i = 0; i < m_CommandList.GetNum(); i++)
		{
			if (m_CommandList[i]->MustFlush())
			{
				m_CommandList[i]->Execute();
			}
#ifdef DEBUG_RENDER_THREAD	
			Synchronize::VSSafeOutputDebugString("%d_%s\n", i, m_CommandList[i]->DescribeCommand());
#endif
		}
	}
	else
	{
		for (uint32 i = 0; i < m_CommandList.GetNum(); i++)
		{
			m_CommandList[i]->Execute();
#ifdef DEBUG_RENDER_THREAD	
			Synchronize::VSSafeOutputDebugString("%d_%s\n", i, m_CommandList[i]->DescribeCommand());
#endif
		}
	}
	
#ifdef DEBUG_RENDER_THREAD
	Synchronize::VSSafeOutputDebugString("==========\n");
#endif
}
VSRenderThreadSys * VSRenderThreadSys::ms_pRenderThreadSys = NULL;
VSRenderThreadSys::VSRenderThreadSys()
{
	VSMAC_ASSERT(!ms_pRenderThreadSys);
	ms_pRenderThreadSys = this;
	m_bIsRunning = false;
	m_RenderBuffer = MEM_NEW VSRenderThreadBuffer();
	m_UpdateBuffer = MEM_NEW VSRenderThreadBuffer();
	#ifdef 	RENDERBUFFER_UPDATE
		m_RenderThread.Start();
	#endif
}
VSRenderThreadSys::~VSRenderThreadSys()
{
	Clear();
	SAFE_DELETE(m_RenderBuffer);
	SAFE_DELETE(m_UpdateBuffer);
	m_RenderThread.Start();
	m_RenderThread.Stop();
}
unsigned char * VSRenderThreadSys::Assign(uint32 uiSize)
{
	return m_UpdateBuffer->Assign(uiSize);
}
unsigned char * VSRenderThreadSys::Assign(uint32 VTType,uint32 uiRegisterNum)
{
	return m_UpdateBuffer->Assign(VTType,uiRegisterNum);
}
DECLEAR_TIME_PROFILENODE(RenderThreadsysExChange, ApplicationUpdate)
void VSRenderThreadSys::ExChange()
{
	ADD_TIME_PROFILE(RenderThreadsysExChange)
#ifdef EVENT_UPDATE	
	m_RenderThread.m_Event.Wait();
#endif
#ifdef 	RENDERBUFFER_UPDATE
	while(1)
	{
		if (m_RenderThread.IsReady())
		{
			break;
		}
		Thread::Sleep(0);
	}
#endif
#ifdef DEBUG_RENDER_THREAD
	Synchronize::VSSafeOutputDebugString("**Exchange**\n");
#endif
/*#ifdef EVENT_UPDATE*/
	m_RenderThread.Suspend();
/*#endif*/
	m_RenderBuffer->Clear();
	Swap(m_UpdateBuffer,m_RenderBuffer);
	m_bIsRunning = false;
}
void VSRenderThreadSys::Begin()
{
#ifdef DEBUG_RENDER_THREAD
	Synchronize::VSSafeOutputDebugString("*************************Start****************\n");
#endif
	m_bIsRunning = true;
	m_RenderThread.SetRender(m_RenderBuffer);
/*#ifdef EVENT_UPDATE*/
	m_RenderThread.Start();
/*#endif*/
}
void VSRenderThreadSys::Clear()
{
#ifdef DEBUG_RENDER_THREAD
	Synchronize::VSSafeOutputDebugString("system clear\n");
#endif
	m_RenderBuffer->Execute(true);
	m_RenderBuffer->Clear();
	m_UpdateBuffer->Clear();
}
VSRenderThread::VSRenderThread()
{
#ifdef EVENT_UPDATE
	m_Event.Create(true,_T("RenderEvent"));
	m_Event.Reset();
#endif
	m_pRenderBuffer = NULL;
}
VSRenderThread::~VSRenderThread()
{

}

void VSRenderThread::Run()
{
	while(!IsStopTrigger())
	{
		if (m_pRenderBuffer)
		{
			m_pRenderBuffer->Execute();
			m_pRenderBuffer = NULL;
		#ifdef EVENT_UPDATE
			m_Event.Trigger();
		#endif
		}	
	}
}
void VSRenderThread::SetRender(VSRenderThreadBuffer * pRenderBuffer)
{
	m_pRenderBuffer = pRenderBuffer;
	#ifdef EVENT_UPDATE
		m_Event.Reset();
	#endif
	
}
#ifdef 	RENDERBUFFER_UPDATE
bool VSRenderThread::IsReady()
{
	return !m_pRenderBuffer;
}
#endif
