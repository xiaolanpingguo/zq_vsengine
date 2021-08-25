#pragma once


#include "core/system.h"


namespace zq{


class SYSTEM_API Synchronize
{
public:
	Synchronize()
	{

	}
	virtual ~Synchronize()
	{
	}
	virtual void * GetHandle() = 0;
	enum
	{
		WF_OBJECT0 = 0,
		WF_TIMEOUT = 256,
		WF_FAILED = 0xFFFFFFFF
	};
	static uint32 WaitAll(Synchronize * * pSynchronize, uint32 uiNum, bool bWaitAll,uint32 dwMilliseconds = (DWORD)-1);
	static void VSSafeOutputDebugString(const char * pcString, ...);
};

class SYSTEM_API VSCriticalSection
{
#ifdef PLATFORM_WIN
	/**
	* The windows specific critical section
	*/
	CRITICAL_SECTION CriticalSection;
#endif
public:
	/**
	* Constructor that initializes the aggregated critical section
	*/
	VSCriticalSection(void)
	{
#ifdef PLATFORM_WIN
		InitializeCriticalSection(&CriticalSection);
		SetCriticalSectionSpinCount(&CriticalSection,4000);
#endif
	}

	/**
	* Destructor cleaning up the critical section
	*/
	 ~VSCriticalSection(void)
	{
#ifdef PLATFORM_WIN
		DeleteCriticalSection(&CriticalSection);
#endif
	}

	/**
	* Locks the critical section
	*/
	inline void Lock(void)
	{
#ifdef PLATFORM_WIN
		// Spin first before entering critical section, causing ring-0 transition and context switch.
		EnterCriticalSection(&CriticalSection);
#endif
	}

	/**
	* Attempt to take a lock and returns whether or not a lock was taken.
	*
	* @return true if a lock was taken, false otherwise.
	*/
	inline bool TryLock()
	{
		if (TryEnterCriticalSection(&CriticalSection))
		{
			return true;
		}
		return false;
	}
	/**
	* Releases the lock on the critical seciton
	*/
	inline void Unlock(void)
	{
#ifdef PLATFORM_WIN
		LeaveCriticalSection(&CriticalSection);
#endif
	}

	class Locker
	{
	public:
		Locker(VSCriticalSection & InCriticalSection)
			:CriticalSection(InCriticalSection)
		{
			CriticalSection.Lock();
		}
		~Locker()
		{
			CriticalSection.Unlock();
		}
	private:
		VSCriticalSection & CriticalSection;
	};
};

class SYSTEM_API VSSemaphore : public Synchronize
{
public:
	VSSemaphore (uint32 uiCount,uint32 MaxCount);
	virtual ~VSSemaphore ();

	virtual void Enter ();
	virtual void Leave (uint32 uiReleaseCount);
	virtual void * GetHandle()
	{
		return Semaphore;
	}
protected:
	void* Semaphore;
	uint32 m_uiMaxCount;
};
	
class SYSTEM_API VSMutex : public Synchronize
{
public:

	VSMutex ();
	virtual ~VSMutex ();

	virtual void Enter ();
	virtual void Leave ();
	virtual void * GetHandle()
	{
		return m_Mutex;
	}
protected:
	void* m_Mutex;

};
class SYSTEM_API VSEvent : public Synchronize
{
	/**
	* The handle to the event
	*/
	void * Event;

public:
	virtual void * GetHandle()
	{
		return Event;
	}
	/**
	* Constructor that zeroes the handle
	*/
	VSEvent(void);

	/**
	* Cleans up the event handle if valid
	*/
	virtual ~VSEvent(void);

	/**
	* Waits for the event to be signaled before returning
	*/
	virtual void Lock(void);

	/**
	* Triggers the event so any waiting threads are allowed access
	*/
	virtual void Unlock(void);

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
	virtual bool Create(bool bIsManualReset = FALSE,const char* InName = NULL);

	/**
	* Triggers the event so any waiting threads are activated
	*/
	virtual void Trigger(void);

	/**
	* Resets the event to an untriggered (waitable) state
	*/
	virtual void Reset(void);

	/**
	* Triggers the event and resets the triggered state NOTE: This behaves
	* differently for auto-reset versus manual reset events. All threads
	* are released for manual reset events and only one is for auto reset
	*/
	virtual void Pulse(void);

	/**
	* Waits for the event to be triggered
	*
	* @param WaitTime Time in milliseconds to wait before abandoning the event
	* (DWORD)-1 is treated as wait infinite
	*
	* @return TRUE if the event was signaled, FALSE if the wait timed out
	*/
	virtual bool Wait(uint32 WaitTime = (DWORD)-1);

	virtual bool IsTrigger();
};


class VSTlsValue
{
public:
	VSTlsValue();
	~VSTlsValue();
	enum
	{
		MAX_THREAD_VALUE = 256,
	};
	void SetThreadValue(void * pValue);
	void* GetThreadValue();
protected:
	uint32 m_uiSlot = 0XFFFFFFFF;
	class StackMemoryManager* m_pThreadValue[MAX_THREAD_VALUE];
	uint32 m_uiThreadValueNum = 0;
	VSCriticalSection m_CriticalSection;
};
	
}
