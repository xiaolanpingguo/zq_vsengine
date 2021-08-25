#pragma once


#include "core/platform.h"
#include "stl/stl.h"


#ifdef PLATFORM_WIN
#include <stdio.h>
#include <tchar.h>
#include <memory.h>
#include <assert.h>
#include <sys/stat.h>
#include <atlsimpstr.h>
#include <intrin.h>
#pragma warning(disable:4251)
#pragma warning(disable:4595) 
#endif



#ifdef USE_DLL

#ifdef VSSYSTEM_EXPORTS
	#define SYSTEM_API __declspec(dllexport) 
#else 
	#define SYSTEM_API __declspec(dllimport) 
#endif

#else
#define SYSTEM_API
#endif


namespace zq{


#ifndef _DEBUG
#define _DEBUG 0
#endif 

#ifdef _DEBUG
#define VSMAC_ASSERT(Expression)\
	{\
		 assert(Expression);\
	}
#else
#define VSMAC_ASSERT(Expression)
#endif

#define THREAD_CALLBACK __stdcall
	inline uint32 SizeTypeToGUID32(uint64 Size_Type)
	{
		return (Size_Type & 0xffffffff);
	}
	inline uint64 GUID32ToSizeType(uint32 uiGUID)
	{
#ifdef PLATFORM_WIN
	#if _WIN64
		return (uiGUID & 0x00000000ffffffff);
	#else
		return (uiGUID & 0xffffffff);
	#endif
#else
		static_assert(0, "No Implement!");;
#endif
	}
	inline int32 ComparePointer(void * Point1, void *Point2)
	{
#if PLATFORM_WIN

		if (((uint64)Point1) == ((uint64)Point2))
		{
			return 0;
		}
		else if (((uint64)Point1) > ((uint64)Point2))
		{
			return 1;
		}
		else
		{
			return -1;
		}		
#else
		static_assert(0, "No Implement!");;
#endif
	}

	#define LOG_BUFFER_SIZE 65536	 
	class SYSTEM_API VSSystem
	{
	public:
		static char ms_sLogBuffer[LOG_BUFFER_SIZE];
		static uint32 ms_dwMainThreadID;
	};
	inline bool VSMemcpy(void * pDest,const void * pSrc, uint64 uiCountSize, uint64 uiDestBufferSize = 0)
	{
		if (!uiDestBufferSize)
		{
			uiDestBufferSize = uiCountSize;
		}
#ifdef PLATFORM_WIN
		return (memcpy_s(pDest,uiDestBufferSize,pSrc,uiCountSize) == 0);
#else
		static_assert(0, "No Implement!");;
		return false;
#endif

	}
	inline uint32 VSStrLen(const char* pStr)
	{
#ifdef PLATFORM_WIN
		return (unsigned int)strlen(pStr);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedCompareExchange(long* pDestination,long Exchange, long Comparand)
	{
#ifdef PLATFORM_WIN
		return _InterlockedCompareExchange(pDestination, Exchange, Comparand);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedAdd(long* pRefCount, long Value)
	{
#ifdef PLATFORM_WIN
		return _InterlockedExchangeAdd(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedExchange(long* pRefCount, long Value)
	{
#ifdef PLATFORM_WIN
		return _InterlockedExchange(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedAnd(long* pRefCount, long Value)
	{
#ifdef PLATFORM_WIN
		return _InterlockedAnd(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedOr(long* pRefCount, long Value)
	{
#ifdef PLATFORM_WIN
		return _InterlockedOr(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSlockedXor(long* pRefCount, long Value)
	{
#ifdef PLATFORM_WIN
		return _InterlockedXor(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSAtomicRead(long* pRefCount)
	{
#ifdef PLATFORM_WIN
		return VSlockedCompareExchange(pRefCount, 0,0);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSAtomicWrite(long* pRefCount, long Value)
	{
#ifdef PLATFORM_WIN
		return VSlockedExchange(pRefCount, Value);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSLockedIncrement(long * pRefCount)
	{
#ifdef PLATFORM_WIN
		return _InterlockedIncrement(pRefCount);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}
	inline long VSLockedDecrement(long * pRefCount)
	{
#ifdef PLATFORM_WIN
		return _InterlockedDecrement(pRefCount);
#else
		static_assert(0, "No Implement!");;
		return 0;
#endif
	}

	inline bool VSIsSpace(int32 c)
	{
#ifdef PLATFORM_WIN
		return _istspace(c);
#else
		static_assert(0, "No Implement!");;
		return false;
#endif
	}
	inline void VSStrcat(char * pDest,uint32 uiCount,const char * pSource)
	{
#ifdef PLATFORM_WIN
		_tcscat_s(pDest,uiCount,pSource);
#else
		static_assert(0, "No Implement!");;
		return ;
#endif
	}
	//Find the next token in a string.
	inline char * VSStrtok(char *strToken, const char *strDelimit ,char ** pContext)
	{
#ifdef PLATFORM_WIN
		return _tcstok_s(strToken, strDelimit,pContext);
#else
		static_assert(0, "No Implement!");;
		return NULL;
#endif
	}
	//Scan a string for the last occurrence of a character.
	inline const char * VSCsrchr(const char *pString,int32 c )
	{
#ifdef PLATFORM_WIN
		return _tcsrchr(pString,c);
#else
		static_assert(0, "No Implement!");;
		return NULL;
#endif
	}
	inline void VSOutputDebugString(const char * pcString, ...)
	{
#ifdef PLATFORM_WIN
		char *pArgs;
		pArgs = (char*) &pcString + sizeof(pcString);
		_vstprintf_s(VSSystem::ms_sLogBuffer, LOG_BUFFER_SIZE,pcString, pArgs) ;
		OutputDebugString(VSSystem::ms_sLogBuffer);
#else
		static_assert(0, "No Implement!");;
		return ;
#endif
	}
	inline void VSSprintf(char * _DstBuf, uint32 _SizeInBytes,const char * _Format, ...)
	{
#ifdef PLATFORM_WIN
		char *pArgs;
		pArgs = (char*) &_Format + sizeof(_Format);
		_vstprintf_s(_DstBuf, _SizeInBytes,_Format, pArgs) ;
#else
		static_assert(0, "No Implement!");;
		return ;
#endif
	}
	inline void VSSprintf(char * _DstBuf, uint32 _SizeInBytes, const char * _Format, va_list pArgs)
	{
#ifdef PLATFORM_WIN
		_vstprintf_s(_DstBuf, _SizeInBytes, _Format, pArgs);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline void VSScanf(char * Buf, const char * _Format, va_list pArgs)
	{
#ifdef PLATFORM_WIN
		_stscanf_s(Buf, _Format, pArgs);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline void VSStrCopy(char * pDest,uint32 uiCount,const char * pSource)
	{
#ifdef PLATFORM_WIN
		_tcscpy_s(pDest,uiCount,pSource);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline int32 VSStrCmp(const char *String1,const char *String2)
	{
#ifdef PLATFORM_WIN
		return _tcscmp(String1,String2);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline int32 VSStrnCmp(const char *String1, const char *String2,uint32 uiMaxNum)
	{
#ifdef PLATFORM_WIN
		return _tcsncmp(String1, String2, uiMaxNum);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline void VSMbsToWcs(wchar_t * Dest,uint32 uiSizeInWord,const char * Source,uint32 uiSizeInByte)
	{
#ifdef PLATFORM_WIN
		mbstowcs_s(0,Dest,uiSizeInWord,Source,uiSizeInByte);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline void VSWcsToMbs(char * Dest,uint32 uiSizeInByte,const wchar_t * Source,uint32 uiSizeInWord)
	{
#ifdef PLATFORM_WIN
		wcstombs_s(0,Dest,uiSizeInByte,Source,uiSizeInWord);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
	inline bool VSIstalnum(char c)
	{
#ifdef PLATFORM_WIN
		return _istalnum(c);
#else
		static_assert(0, "No Implement!");;
		return false;
#endif
	}
	inline uint32 VSGetCpuNum()
	{
#ifdef PLATFORM_WIN
		SYSTEM_INFO SystemInfo;
#if _WIN64
		GetNativeSystemInfo(&SystemInfo);
#else
		GetSystemInfo(&SystemInfo);
#endif
		return SystemInfo.dwNumberOfProcessors;
#else
		static_assert(0, "No Implement!");;
		return 1;
#endif
	}
	inline bool VSIsMainThread()
	{
#ifdef PLATFORM_WIN
		return VSSystem::ms_dwMainThreadID == GetCurrentThreadId();
#else
		static_assert(0, "No Implement!");;
		return false;
#endif
	}
	inline void VSGetCurrentDirectory(char * CurDir)
	{
#ifdef PLATFORM_WIN
		GetCurrentDirectory(MAX_PATH, CurDir);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}

	inline char * GetCurPlatformName()
	{
#if PLATFORM_WIN
		return _T("Windows");
#else
		static_assert(0, "No Implement!");
		return _T("");
#endif
	}
	inline void VSSetCurrentDirectory(char * CurDir)
	{
#ifdef PLATFORM_WIN
		SetCurrentDirectory(CurDir);
#else
		static_assert(0, "No Implement!");
		return;
#endif
	}
	inline void VSInitSystem()
	{
#ifdef PLATFORM_WIN
		VSSystem::ms_dwMainThreadID = GetCurrentThreadId();
#else
		static_assert(0, "No Implement!");
		return;
#endif
	}
	inline uint32 VSTlsAlloc()
	{
#ifdef PLATFORM_WIN
		return TlsAlloc();
#else
		static_assert(0, "No Implement!");
		return 0;
#endif
	}
	inline void* VSGetTlsValue(uint32 TlsSolt)
	{
#ifdef PLATFORM_WIN
		return TlsGetValue(TlsSolt);
#else
		static_assert(0, "No Implement!");
		return NULL;
#endif
	}
	inline bool VSSetTlsValue(uint32 TlsSolt,void * TlsValue)
	{
#ifdef PLATFORM_WIN
		return TlsSetValue(TlsSolt, TlsValue);
#else
		static_assert(0, "No Implement!");
		return false;
#endif
	}
	inline bool VSTlsFree(uint32 TlsSolt)
	{
#ifdef PLATFORM_WIN
		return TlsFree(TlsSolt);
#else
		static_assert(0, "No Implement!");
		return false;
#endif
	}
#ifdef PLATFORM_WIN
#define ALIGN(n) __declspec(align(n))
#else
#define ALIGN(n)
#endif
	template<typename T>
	inline T ABS(T t)
	{
		return t < 0 ? -t : t;
	}
	template<typename T>
	inline T Min(T t0, T t1)
	{
		return t0 < t1 ? t0 : t1;
	}
	template<typename T>
	inline T Max(T t0, T t1)
	{
		return t0 > t1 ? t0 : t1;
	}
	template<typename T>
	inline T Clamp(T Value, T Max, T Min)
	{
		if (Value >= Max)
		{
			return Max;
		}
		if (Value <= Min)
		{
			return Min;
		}
		return Value;
	}
	template <class T>
	inline void Swap(T &t1, T &t2)
	{
		T temp;
		temp = t1;
		t1 = t2;
		t2 = temp;
	}
#define BIT(i) (1 << i)
}