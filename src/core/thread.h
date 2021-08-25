#pragma once


#include "core/system.h"
#include "core/synchronize.h"


namespace zq{


class SYSTEM_API Thread
{
public:
	enum Priority
	{
		Low,
		Normal,
		High,
	};
	enum ThreadState
	{
		TS_START,
		TS_SUSPEND,
		TS_STOP,
	};
public:
	Thread();
	virtual ~Thread();
		
	void SetPriority(Priority p);
		
	Priority GetPriority() const;
		
	void SetStackSize(uint32 uiSize);
		
	uint32 GetStackSize() const;


	void Start();

	void Suspend();

	bool IsRunning() const;
	static void Sleep(uint32 dwMillseconds);

	bool IsStopTrigger();
	void Stop();
public:
		
	static void SetThreadName(const char* name);
	inline ThreadState GetThreadState()
	{
		return m_ThreadState;
	}
protected:
	virtual void Run() = 0;
	virtual const char* GetThreadName();
private:
	static unsigned long THREAD_CALLBACK ThreadProc(void* t);
private:
	void* m_hThread;
	Priority m_priority;
	uint32 m_stackSize;
protected:
	ThreadState m_ThreadState;
	VSEvent m_StopEvent;
};
}

