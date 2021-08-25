#include "core/synchronize.h"
#include "core/memorymanager.h"


namespace zq{


VSCriticalSection g_SafeOutputString;
void Synchronize::VSSafeOutputDebugString(const char * pcString, ...)
{
	VSCriticalSection::Locker Temp(g_SafeOutputString);
#if PLATFORM_WIN
	char *pArgs;
	pArgs = (char*) &pcString + sizeof(pcString);
	_vstprintf_s(VSSystem::ms_sLogBuffer, LOG_BUFFER_SIZE,pcString, pArgs) ;
	OutputDebugString(VSSystem::ms_sLogBuffer);
#else
	static_assert(0, "no thread implement");
#endif
}
uint32 Synchronize::WaitAll(Synchronize * * pSynchronize, uint32 uiNum, bool bWaitAll,uint32 dwMilliseconds)
{
#if PLATFORM_WIN
	VSMAC_ASSERT(uiNum >=1 && uiNum <= MAXIMUM_WAIT_OBJECTS);
	static HANDLE handle[MAXIMUM_WAIT_OBJECTS];
	for (uint32 i = 0 ; i < uiNum ; i++)
	{
		handle[i] = (HANDLE)pSynchronize[i]->GetHandle();
	}
	uint32 dw = WaitForMultipleObjects(uiNum,handle,bWaitAll,dwMilliseconds);
	switch(dw)
	{
	case WAIT_FAILED:
		return WF_FAILED;
	case WAIT_TIMEOUT:
		return WF_TIMEOUT;
	case WAIT_OBJECT_0:
		return WF_OBJECT0;

	}
	return WF_FAILED;
#else
	static_assert(0, "no thread implement");
#endif
}
VSEvent::VSEvent(void)
{
	Event = NULL;
}

/**
* Cleans up the event handle if valid
*/
VSEvent::~VSEvent(void)
{
	if (Event != NULL)
	{
#if PLATFORM_WIN
		CloseHandle(Event);
#else
		static_assert(0, "no thread implement");
#endif
	}
}

/**
* Waits for the event to be signaled before returning
*/
void VSEvent::Lock(void)
{
#if PLATFORM_WIN
	WaitForSingleObject(Event,INFINITE);
#else
	static_assert(0, "no thread implement");
#endif
}

/**
* Triggers the event so any waiting threads are allowed access
*/
void VSEvent::Unlock(void)
{
#if PLATFORM_WIN
	PulseEvent(Event);
#else
	static_assert(0, "no thread implement");
#endif
}

/**
* Creates the event. Manually reset events stay triggered until reset.
* Named events share the same underlying event.
*
* @param bIsManualReset Whether the event requires manual reseting or not
* @param InName Whether to use a commonly shared event or not. If so this
* is the name of the event to share.
*
* @return Returns TRUE if the event was created, FALSE otherwise
*/
bool VSEvent::Create(bool bIsManualReset,const char* InName)
{
	// Create the event and default it to non-signaled
#if PLATFORM_WIN
	Event = CreateEvent(NULL,bIsManualReset,0,InName);
	return Event != NULL;
#else
	static_assert(0, "no thread implement");
#endif
}

/**
* Triggers the event so any waiting threads are activated
*/
void VSEvent::Trigger(void)
{
#if PLATFORM_WIN
	SetEvent(Event);
#else
	static_assert(0, "no thread implement");
#endif
}

/**
* Resets the event to an untriggered (waitable) state
*/
void VSEvent::Reset(void)
{
#if PLATFORM_WIN
	ResetEvent(Event);
#else
	static_assert(0, "no thread implement");
#endif
}

/**
* Triggers the event and resets the triggered state NOTE: This behaves
* differently for auto-reset versus manual reset events. All threads
* are released for manual reset events and only one is for auto reset
*/
void VSEvent::Pulse(void)
{
#if PLATFORM_WIN
	PulseEvent(Event);
#else
	static_assert(0, "no thread implement");
#endif
}

/**
* Waits for the event to be triggered
*
* @param WaitTime Time in milliseconds to wait before abandoning the event
* (DWORD)-1 is treated as wait infinite
*
* @return TRUE if the event was signaled, FALSE if the wait timed out
*/
bool VSEvent::Wait(uint32 WaitTime)
{
#if PLATFORM_WIN
	return WaitForSingleObject(Event,WaitTime) == WAIT_OBJECT_0;
#else
	static_assert(0, "no thread implement");
#endif
}
bool VSEvent::IsTrigger()
{
	return Wait(0);
}
VSMutex::VSMutex()
{
#if PLATFORM_WIN
	m_Mutex = CreateMutex(NULL, FALSE, NULL);
#else
	static_assert(0, "no thread implement");
#endif
	VSMAC_ASSERT(m_Mutex);
}
//----------------------------------------------------------------------------
VSMutex::~VSMutex()
{
#if PLATFORM_WIN
	BOOL closed = CloseHandle((HANDLE)m_Mutex);	
	VSMAC_ASSERT(closed);
#else
	static_assert(0, "no thread implement");
#endif
	m_Mutex = NULL;
}
//----------------------------------------------------------------------------
void VSMutex::Enter ()
{
#if PLATFORM_WIN
	uint32 result = WaitForSingleObject((HANDLE)m_Mutex, INFINITE);
	VSMAC_ASSERT(result != WAIT_FAILED);
#else
static_assert(0, "no thread implement");
#endif
	// result:
	//   WAIT_ABANDONED (0x00000080)
	//   WAIT_OBJECT_0  (0x00000000), signaled
	//   WAIT_TIMEOUT   (0x00000102), [not possible with INFINITE]
	//   WAIT_FAILED    (0xFFFFFFFF), not signaled
}
//----------------------------------------------------------------------------
void VSMutex::Leave ()
{
#ifdef PLATFORM_WIN
	BOOL released = ReleaseMutex((HANDLE)m_Mutex);
	VSMAC_ASSERT(released);
#else
	static_assert(0, "no thread implement");
#endif
}

VSSemaphore::VSSemaphore(uint32 uiCount,uint32 MaxCount)
{
	VSMAC_ASSERT(uiCount <= MaxCount);
#if PLATFORM_WIN
	Semaphore = CreateSemaphore(NULL,uiCount,MaxCount,NULL);	
	VSMAC_ASSERT(Semaphore);
#else
	static_assert(0, "no thread implement");
#endif
	m_uiMaxCount = MaxCount;
}
//----------------------------------------------------------------------------
VSSemaphore::~VSSemaphore()
{
#if PLATFORM_WIN
	BOOL closed = CloseHandle((HANDLE)Semaphore);
	VSMAC_ASSERT(closed);
#else
	static_assert(0, "no thread implement");
#endif
	Semaphore = NULL;
}
//----------------------------------------------------------------------------
void VSSemaphore::Enter ()
{
#if PLATFORM_WIN
	uint32 result = WaitForSingleObject((HANDLE)Semaphore, INFINITE);
	VSMAC_ASSERT(result);
#else
	static_assert(0, "no thread implement");
#endif
	// result:
	//   WAIT_ABANDONED (0x00000080)
	//   WAIT_OBJECT_0  (0x00000000), signaled
	//   WAIT_TIMEOUT   (0x00000102), [not possible with INFINITE]
	//   WAIT_FAILED    (0xFFFFFFFF), not signaled
}
//----------------------------------------------------------------------------
void VSSemaphore::Leave (uint32 uiReleaseCount)
{
#if PLATFORM_WIN
	BOOL released = ReleaseSemaphore((HANDLE)Semaphore,uiReleaseCount,NULL);
	VSMAC_ASSERT(released);
#else
	static_assert(0, "no thread implement");
#endif
}
VSTlsValue::VSTlsValue()
{
	VSCriticalSection::Locker Temp(m_CriticalSection);
	m_uiSlot = VSTlsAlloc();
	VSMAC_ASSERT(m_uiSlot != 0XFFFFFFFF);
}
VSTlsValue::~VSTlsValue()
{
	VSMAC_ASSERT(m_uiSlot != 0XFFFFFFFF);
	for (uint32 i = 0; i < m_uiThreadValueNum; i++)
	{
        SAFE_DELETE(m_pThreadValue[i]);
	}
	VSTlsFree(m_uiSlot);
}
void VSTlsValue::SetThreadValue(void* pValue)
{
	VSCriticalSection::Locker Temp(m_CriticalSection);
	m_pThreadValue[m_uiThreadValueNum] = (StackMemoryManager *)pValue;
	m_uiThreadValueNum++;
	VSSetTlsValue(m_uiSlot, pValue);
}
void* VSTlsValue::GetThreadValue()
{
	return VSGetTlsValue(m_uiSlot);
}

}