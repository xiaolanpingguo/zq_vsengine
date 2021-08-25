#pragma once
#include "graphic/core/object.h"
#include "core/thread.h"
#include "graphic/asyn/enginethread.h"
//#define EVENT_UPDATE
#define RENDERBUFFER_UPDATE
#ifdef RENDERBUFFER_UPDATE
#endif
namespace zq
{
	
	class GRAPHIC_API VSRenderThreadBuffer : public MemObject
	{
	public:
		VSRenderThreadBuffer();
		~VSRenderThreadBuffer();
		friend class VSRenderThreadSys;
		friend class VSRenderThread;
	protected:
		unsigned char * Assign(uint32 uiSize);
		unsigned char * Assign(uint32 VTType,uint32 uiRegisterNum);
		void Clear();
		template<class T>
		VSThreadCommand * AssignCommand();
		void Execute(bool bMustFlust = false);
		VSConstantBuffer m_ConstantBuffer;
		VSArray<VSThreadCommand *> m_CommandList;
		VSCriticalSection m_Lock;
	};
	template<class T>
	VSThreadCommand * VSRenderThreadBuffer::AssignCommand()
	{	
		VSCriticalSection::Locker Temp(m_Lock); 
		VSThreadCommand* pRenderCommand = (VSThreadCommand *)Assign(sizeof(T));
		m_CommandList.AddElement(pRenderCommand);
		return pRenderCommand;
	}
	class GRAPHIC_API VSRenderThread : public Thread
	{
	public:
		VSRenderThread();
		~VSRenderThread();
		friend class VSRenderThreadSys;
	protected:
		virtual const char* GetThreadName()
		{
			return _T("VSRenderThread");
		}
		virtual void Run();	
		void SetRender(VSRenderThreadBuffer * pRenderBuffer);
#ifdef EVENT_UPDATE
		VSEvent m_Event;
#endif	
#ifdef 	RENDERBUFFER_UPDATE
		bool IsReady();
#endif
		VSRenderThreadBuffer * m_pRenderBuffer;
	};
	class GRAPHIC_API VSRenderThreadSys : public MemObject
	{
	public:
		VSRenderThreadSys();
		~VSRenderThreadSys();
		unsigned char * Assign(uint32 uiSize);
		unsigned char * Assign(uint32 VTType,uint32 uiRegisterNum);
		template<class T>
		VSThreadCommand * AssignCommand();
		void ExChange();
		void Begin();
		void Clear();
		static VSRenderThreadSys* ms_pRenderThreadSys;
		bool IsRunning()
		{
			return m_bIsRunning;
		}
	protected:
		VSRenderThreadBuffer * m_RenderBuffer;
		VSRenderThreadBuffer * m_UpdateBuffer;
		VSRenderThread			m_RenderThread;
		bool					m_bIsRunning;


	};
	template<class T>
	VSThreadCommand * VSRenderThreadSys::AssignCommand()
	{
		return m_UpdateBuffer->AssignCommand<T>();
	}
/**
 * Declares a rendering command type with 0 parameters.
 */

#define ENQUEUE_RENDER_COMMAND(TypeName,Params) \
	if(VSResourceManager::ms_bRenderThread && VSRenderThreadSys::ms_pRenderThreadSys->IsRunning()) \
	{ \
		TypeName * pCommand = (TypeName *)VSRenderThreadSys::ms_pRenderThreadSys->AssignCommand<TypeName>(); \
		MEM_NEW(pCommand)TypeName Params; \
	} \
	else \
	{\
		TypeName Temp Params;\
		Temp.Execute();\
	}
		

#define ENQUEUE_UNIQUE_RENDER_COMMAND(TypeName,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND(TypeName,Code) \
	ENQUEUE_RENDER_COMMAND(TypeName,)


/**
 * Declares a rendering command type with 1 parameters.
 */
#define ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Value1,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND_ONEPARAMETER(TypeName,ParamType1,ParamName1,Code) \
	ENQUEUE_RENDER_COMMAND(TypeName,(Value1))


/**
 * Declares a rendering command type with 2 parameters.
 */
#define ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND_TWOPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,Code) \
	ENQUEUE_RENDER_COMMAND(TypeName,(Value1,Value2))


/**
 * Declares a rendering command type with 3 parameters.
 */
#define ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,ParamType3,ParamName3,Value3,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND_THREEPARAMETER(TypeName,ParamType1,ParamName1,ParamType2,ParamName2,ParamType3,ParamName3,Code) \
	ENQUEUE_RENDER_COMMAND(TypeName,(Value1,Value2,Value3))

/**
* Declares a rendering command type with 4 parameters.
*/
#define ENQUEUE_UNIQUE_RENDER_COMMAND_FOURPARAMETER(TypeName,ParamType1,ParamName1,Value1,ParamType2,ParamName2,Value2,ParamType3,ParamName3,Value3,ParamType4,ParamName4,Value4,Code) \
	DECLARE_UNIQUE_THREAD_COMMAND_FOURPARAMETER(TypeName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, Code) \
	ENQUEUE_RENDER_COMMAND(TypeName, (Value1, Value2, Value3, Value4))

	template<typename TypeName,typename LAMBDA>
	void EnqueueUniqueRenderCommand(LAMBDA&& Lambda,bool bMustFlush = false)
	{
		typedef VSLambdaCommand < TypeName, LAMBDA> VSLambdaCommandTemp;
		ENQUEUE_RENDER_COMMAND(VSLambdaCommandTemp, (Forward<LAMBDA>(Lambda), bMustFlush));
	}
 #define ENQUEUE_LAMBDA_RENDER_COMMAND(TypeName) \
	struct TypeName\
	{\
		static const char * GetName() { return _T(#TypeName);} \
	};\
	EnqueueUniqueRenderCommand<TypeName>
}
