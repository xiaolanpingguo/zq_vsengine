#include "core/thread.h"


namespace zq{


unsigned long THREAD_CALLBACK Thread::ThreadProc(void* t)
{
	Thread* pThread = (Thread*)(t);
	SetThreadName(pThread->GetThreadName());
	pThread->Run();
	return 0;
}

//------------------------------------------------------------------------------
Thread::Thread()
	: m_hThread(NULL)
	, m_priority(Normal)
	, m_stackSize(0)
{
	VSMAC_ASSERT(!IsRunning());
#if PLATFORM_WIN
	m_hThread = ::CreateThread(0, m_stackSize, ThreadProc, this, CREATE_SUSPENDED, NULL);
#else
	static_assert(0, "no thread implement");
#endif
	VSMAC_ASSERT(m_hThread);
	m_ThreadState = TS_SUSPEND;
	SetPriority(m_priority);
	m_StopEvent.Create(true);
	m_StopEvent.Reset();
}

//------------------------------------------------------------------------------
Thread::~Thread()
{
	if (IsRunning())
	{
#if PLATFORM_WIN
		// force to exit
		TerminateThread(m_hThread, 0);
#else
		static_assert(0, "no thread implement");
#endif
	}
	if (m_hThread)
	{
#if PLATFORM_WIN
		CloseHandle(m_hThread);
#else
		static_assert(0, "no thread implement");
#endif
	}
}

//------------------------------------------------------------------------------
void Thread::SetPriority(Priority p)
{
	int32 nPriority = THREAD_PRIORITY_NORMAL;

	if (p == Low)
		nPriority = THREAD_PRIORITY_BELOW_NORMAL;
	else if (p == Normal)
		nPriority = THREAD_PRIORITY_NORMAL;
	else if (p == High)
		nPriority = THREAD_PRIORITY_ABOVE_NORMAL;
#if PLATFORM_WIN
	::SetThreadPriority(m_hThread, nPriority);
#else
	static_assert(0, "no thread implement");
#endif
}

//------------------------------------------------------------------------------
void Thread::Start()
{	
	if (m_ThreadState == TS_SUSPEND)
	{
#if PLATFORM_WIN
		ResumeThread((HANDLE)m_hThread);
#else
		static_assert(0, "no thread implement");
#endif
		m_ThreadState = TS_START;
	}
}
//------------------------------------------------------------------------------
void Thread::Suspend()
{
	if (m_ThreadState == TS_START)
	{
#if PLATFORM_WIN
		SuspendThread((HANDLE)m_hThread);
#else
		static_assert(0, "no thread implement");
#endif
		m_ThreadState = TS_SUSPEND;
	}
	
}
void Thread::Sleep(uint32 dwMillseconds)
{
#if PLATFORM_WIN
		::Sleep(dwMillseconds);
#else
		static_assert(0, "no thread implement");
#endif
}
//------------------------------------------------------------------------------
bool Thread::IsRunning() const
{
	if (NULL != m_hThread)
	{
#if PLATFORM_WIN
		DWORD exitCode = 0;
		if (GetExitCodeThread(m_hThread, &exitCode))
		{
			if (STILL_ACTIVE == exitCode)
			{
				return true;
			}
		}
#else
		static_assert(0, "no thread implement");
#endif
	}

	return false;
}

//------------------------------------------------------------------------------
void Thread::SetThreadName(const char* name)
{
#if PLATFORM_WIN
	// update the Windows thread name so that it shows up correctly
	// in the Debugger
	struct THREADNAME_INFO
	{
		uint32 dwType;     // must be 0x1000
		LPCSTR szName;    // pointer to name (in user address space)
		uint32 dwThreadID; // thread ID (-1 = caller thread)
		uint32 dwFlags;    // reserved for future use, must be zero
	};

	THREADNAME_INFO info;
	info.dwType     = 0x1000;
	info.szName     = name;
	info.dwThreadID = ::GetCurrentThreadId();
	info.dwFlags    = 0;
	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR*)&info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
#else
	static_assert(0, "no thread implement");
#endif
}

//------------------------------------------------------------------------------
const char* Thread::GetThreadName()
{
	return _T("Thread");
}
bool Thread::IsStopTrigger()
{
	return m_StopEvent.IsTrigger();
}
//------------------------------------------------------------------------------
void Thread::Stop()
{
	if (m_ThreadState == TS_START)
	{
		VSMAC_ASSERT(this->IsRunning());
		VSMAC_ASSERT(NULL != m_hThread);

		m_StopEvent.Trigger();
		m_ThreadState = TS_STOP;
#if PLATFORM_WIN
		// wait for the thread to terminate
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
#else
		static_assert(0, "no thread implement");
#endif
		m_hThread = NULL;
	}
}

}