#pragma once


#include "graphic/core/object.h"
#include "core/thread.h"
#include "graphic/asyn/enginethread.h"
#include "datastruct/VSDelegate.h"


namespace zq{

//#define DEBUG_UPDATE_THREAD
//#define EVENT_UPDATE
#define CONTEXT_UPDATE


class VSUpdateContext
{
public:
	VSUpdateContext();
	virtual ~VSUpdateContext();
	uint32 Execute();
	void Clear();
	void AddCommand(VSThreadCommand * pCommand);
	inline uint32 GetCommandNum()const
	{
		return m_CommandArray.GetNum();
	}
protected:
	VSArray<VSThreadCommand *> m_CommandArray;
};


class VSUpdateCenter
{
public:
	VSUpdateCenter(bool bIsMainRun,uint32 uiThreadNum);
	virtual ~VSUpdateCenter();
	VSUpdateContext * GetUpdateContext(uint32 i);

	inline bool IsMainRun()const
	{
		return m_bIsMainRun;
	}

	uint32 Execute();
	void Clear();
	template<class T>
	VSThreadCommand * AssignCommand(int32 iIndex = -1);
protected:
	VSUpdateContext *m_pContextArray;
	VSConstantBuffer m_ConstantBuffer;
	unsigned char * Assign(uint32 uiSize);
	bool m_bIsMainRun;
	uint32 m_uiContextNum;
	virtual uint32 GetIndex();
	int32 m_uiIndex;
};


template<class T>
VSThreadCommand * VSUpdateCenter::AssignCommand(int32 iIndex)
{	
	VSThreadCommand* pRenderCommand = (VSThreadCommand *)Assign(sizeof(T));
	if (iIndex == -1)
	{
		m_pContextArray[GetIndex()].AddCommand(pRenderCommand);
	}
	else
	{
		VSMAC_ASSERT(iIndex < (int)m_uiContextNum);
		m_pContextArray[iIndex].AddCommand(pRenderCommand);
	}

	return pRenderCommand;
}


class VSUpdateThread : public Thread
{
public:
	VSUpdateThread();
	~VSUpdateThread();
	friend class VSUpdateThreadSys;	
protected:
	VSUpdateContext * m_pUpdateContext;

	virtual const char* GetThreadName()
	{
		return m_ThreadName.GetBuffer();
	}

	virtual void Run();	
	void SetUpdateContext(VSUpdateContext * pUpdateContex);
	bool IsReady()const;
#ifdef EVENT_UPDATE
	VSEvent m_Event;
#endif
		
	uint32 m_uiThreadId;
	VSString m_ThreadName;
private:
};

class GRAPHIC_API VSUpdateThreadSys : public MemObject
{
public:
	enum //UpdateType
	{
		UPDATE_NODE,
		UPDATE_MAX
	};
	VSUpdateThreadSys(uint32 uiThreadNum);
	~VSUpdateThreadSys();

	inline void SetUpdateType(uint32 uiUpdateType)
	{
		m_uiCurUpdateType = uiUpdateType;
	}

	inline int32 GetMainThreadID()
	{
		if (m_UpdateCenter[m_uiCurUpdateType]->IsMainRun())
		{
			return m_uiThreadNum;
		}
		else
		{
			return 0;
		}
	}

	inline uint32 GetThreadNum()
	{
		return m_uiThreadNum;
	}

    void Begin();
    void ExChange();
    template<class T>
    VSThreadCommand * AssignCommand(int32 iIndex = -1);
    static VSUpdateThreadSys* ms_pUpdateThreadSys;

protected:
	uint32 m_uiThreadNum;
	VSArray<VSUpdateThread>m_UpdateThread;
#ifdef EVENT_UPDATE
	VSArray<Synchronize*>m_WaitEvent;
#endif
	uint32 m_uiCurUpdateType;
	VSUpdateCenter* m_UpdateCenter[UPDATE_MAX];

};


template<class T>
VSThreadCommand * VSUpdateThreadSys::AssignCommand(int32 iIndex)
{	
	VSThreadCommand* pRenderCommand = (VSThreadCommand *)m_UpdateCenter[m_uiCurUpdateType]->AssignCommand<T>(iIndex);
	return pRenderCommand;
};
#define ENQUEUE_UPDATE_COMMAND(TypeName,Params) \
if(VSResourceManager::ms_bUpdateThread) \
{ \
	TypeName * pCommand = (TypeName *)VSUpdateThreadSys::ms_pUpdateThreadSys->AssignCommand<TypeName>(); \
	MEM_NEW(pCommand)TypeName Params; \
}\
else \
{\
	TypeName Temp Params;\
	Temp.Execute();\
}

#define ENQUEUE_UPDATE_COMMAND_THREADID(TypeName,Params,ThreadID) \
if(VSResourceManager::ms_bUpdateThread) \
{ \
	TypeName * pCommand = (TypeName *)VSUpdateThreadSys::ms_pUpdateThreadSys->AssignCommand<TypeName>(ThreadID); \
	MEM_NEW(pCommand)TypeName Params; \
} \
else \
{\
	TypeName Temp Params;\
	Temp.Execute();\
}

#define ENQUEUE_UNIQUE_UPDATE_COMMAND(TypeName,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND(TypeName,Code) \
	ENQUEUE_UPDATE_COMMAND(TypeName,)

#define ENQUEUE_UNIQUE_UPDATE_COMMAND_THREADID(TypeName,ThreadID,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND(TypeName,Code) \
	ENQUEUE_UPDATE_COMMAND_THREADID(TypeName,,ThreadID)
/**
 * Declares a rendering command type with 1 parameters.
 */
#define ENQUEUE_UNIQUE_UPDATE_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Value1,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Code) \
	ENQUEUE_UPDATE_COMMAND(TypeName,(Value1))

#define ENQUEUE_UNIQUE_UPDATE_COMMAND_ONEPARAMETER_THREADID(TypeName,ParamType1,ParamName1,Value1,ThreadID,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Code) \
	ENQUEUE_UPDATE_COMMAND_THREADID(TypeName,(Value1),ThreadID)
/**
 * Declares a rendering command type with 2 parameters.
 */
#define ENQUEUE_UNIQUE_UPDATE_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,Code) \
	ENQUEUE_UPDATE_COMMAND(TypeName,(Value1,Value2))

#define ENQUEUE_UNIQUE_UPDATE_COMMAND_TWOPARAMETER_THREADID(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,ThreadID,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,Code) \
	ENQUEUE_UPDATE_COMMAND_THREADID(TypeName,(Value1,Value2),ThreadID)
/**
 * Declares a rendering command type with 3 parameters.
 */
#define ENQUEUE_UNIQUE_UPDATE_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,ParamType3,ParamName3,Value3,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,ParamType3,ParamName3,Code) \
	ENQUEUE_UPDATE_COMMAND(TypeName,(Value1,Value2,Value3))

#define ENQUEUE_UNIQUE_UPDATE_COMMAND_THREEPARAMETER_THREADID(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,ParamType3,ParamName3,Value3,ThreadID,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,ParamType3,ParamName3,Code) \
	ENQUEUE_UPDATE_COMMAND_THREADID(TypeName,(Value1,Value2,Value3),ThreadID)

	template<typename TypeName, typename LAMBDA>
	void EnqueueUniqueUpdateCommand(LAMBDA&& Lambda)
	{
		typedef VSLambdaCommand < TypeName, LAMBDA> VSLambdaCommandTemp;
		ENQUEUE_UPDATE_COMMAND(VSLambdaCommandTemp, (Forward<LAMBDA>(Lambda)));
	}
#define ENQUEUE_LAMBDA_UPDATE_COMMAND(TypeName) \
	struct TypeName\
	{\
		static const char * GetName() { return _T(#TypeName);} \
	};\
	EnqueueUniqueUpdateCommand<TypeName>

	template<typename TypeName,uint32 ThreadID, typename LAMBDA>
	void EnqueueUniqueUpdateCommandThreadID(LAMBDA&& Lambda)
	{
		typedef VSLambdaCommand < TypeName, LAMBDA> VSLambdaCommandTemp;
		ENQUEUE_UPDATE_COMMAND_THREADID(VSLambdaCommandTemp, (Forward<LAMBDA>(Lambda)), ThreadID);
	}
#define ENQUEUE_LAMBDA_UPDATE_COMMAND_THREADID(TypeName,ThreadID) \
	struct TypeName\
	{\
		static const char * GetName() { return _T(#TypeName);} \
	};\
	EnqueueUniqueUpdateCommandThreadID<TypeName,ThreadID>
}
