#pragma once


#include "core/system.h"
#include "core/synchronize.h"
#include <new.h>
#include <type_traits>

#ifdef PLATFORM_WIN
#include <Windows.h>
#endif // PLATFORM_WIN


#define MEM_NEW new
#define MEM_DELETE delete


namespace zq{


template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };
template <typename T>
inline T&& Forward(typename TRemoveReference<T>::Type& Obj)
{
	return (T&&)Obj;
}

template <typename T>
inline T&& Forward(typename TRemoveReference<T>::Type&& Obj)
{
	return (T&&)Obj;
}


#define HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#define HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#define HAS_TRIVIAL_ASSIGN(T) __has_trivial_assign(T)
#define HAS_TRIVIAL_COPY(T) __has_trivial_copy(T)
#define IS_POD(T) __is_pod(T)
#define IS_ENUM(T) __is_enum(T)
#define IS_EMPTY(T) __is_empty(T)


/*-----------------------------------------------------------------------------
Type traits similar to TR1 (uses intrinsics supported by VC8)
Should be updated/revisited/discarded when compiler support for tr1 catches up.
-----------------------------------------------------------------------------*/

/**
* TIsFloatType
*/
template<typename T> struct TIsFloatType { enum { Value = false }; };

template<> struct TIsFloatType<float> { enum { Value = true }; };
template<> struct TIsFloatType<double> { enum { Value = true }; };
template<> struct TIsFloatType<long double> { enum { Value = true }; };

/**
* TIsIntegralType
*/
template<typename T> struct TIsIntegralType { enum { Value = false }; };

template<> struct TIsIntegralType<unsigned char> { enum { Value = true }; };
template<> struct TIsIntegralType<unsigned short> { enum { Value = true }; };
template<> struct TIsIntegralType<unsigned int> { enum { Value = true }; };
template<> struct TIsIntegralType<unsigned long> { enum { Value = true }; };

template<> struct TIsIntegralType<signed char> { enum { Value = true }; };
template<> struct TIsIntegralType<signed short> { enum { Value = true }; };
template<> struct TIsIntegralType<signed int> { enum { Value = true }; };
template<> struct TIsIntegralType<signed long> { enum { Value = true }; };

template<> struct TIsIntegralType<bool> { enum { Value = true }; };
template<> struct TIsIntegralType<char> { enum { Value = true }; };

// compilers we support define wchar_t as a native type
#if !_MSC_VER || defined(_NATIVE_WCHAR_T_DEFINED)
template<> struct TIsIntegralType<wchar_t> { enum { Value = true }; };
#endif

// C99, but all compilers we use support it
template<> struct TIsIntegralType<unsigned long long> { enum { Value = true }; };
template<> struct TIsIntegralType<signed long long> { enum { Value = true }; };
/**
* TIsArithmeticType
*/
template<typename T> struct TIsArithmeticType 
{ 
	enum { Value = TIsIntegralType<T>::Value || TIsFloatType<T>::Value } ;
};

/**
* TIsNativePointerType
* @todo - exclude member pointers
*/
template<typename T> struct TIsNativePointerType						{ enum { Value = false }; };
template<typename T> struct TIsNativePointerType<T*>					{ enum { Value = true }; };
template<typename T> struct TIsNativePointerType<const T*>				{ enum { Value = true }; };
template<typename T> struct TIsNativePointerType<const T* const>		{ enum { Value = true }; };
template<typename T> struct TIsNativePointerType<T* volatile>			{ enum { Value = true }; };
template<typename T> struct TIsNativePointerType<T* const volatile>		{ enum { Value = true }; };

/**
* TIsVoidType
*/
template<typename T> struct TIsVoidType { enum { Value = false }; };
template<> struct TIsVoidType<void> { enum { Value = true }; };
template<> struct TIsVoidType<void const> { enum { Value = true }; };
template<> struct TIsVoidType<void volatile> { enum { Value = true }; };
template<> struct TIsVoidType<void const volatile> { enum { Value = true }; };

/**
* TIsPODType
* @todo - POD array and member pointer detection
*/
template<typename T> struct TIsPODType 
{ 
	enum { Value = IS_POD(T) || IS_ENUM(T) || TIsArithmeticType<T>::Value || TIsNativePointerType<T>::Value };
};

/**
* TIsFundamentalType
*/
template<typename T> 
struct TIsFundamentalType 
{ 
	enum { Value = TIsArithmeticType<T>::Value || TIsVoidType<T>::Value };
};


template<typename T> struct ValueBase
{
	// WRH - 2007/11/28 - the compilers we care about do not produce equivalently efficient code when manually
	// calling the constructors of trivial classes. In array cases, we can call a single memcpy
	// to initialize all the members, but the compiler will call memcpy for each element individually,
	// which is slower the more elements you have. 
	enum { NeedsConstructor = !HAS_TRIVIAL_CONSTRUCTOR(T) && !TIsPODType<T>::Value };
	// WRH - 2007/11/28 - the compilers we care about correctly elide the destructor code on trivial classes
	// (effectively compiling down to nothing), so it is not strictly necessary that we have NeedsDestructor. 
	// It doesn't hurt, though, and retains for us the ability to skip destructors on classes without trivial ones
	// if we should choose.
	enum { NeedsDestructor = !HAS_TRIVIAL_DESTRUCTOR(T) && !TIsPODType<T>::Value };
};

template< class T > inline T Align( const T Ptr, uint64 Alignment )
{
	return (T)(((uint64)Ptr + Alignment - 1) & ~(Alignment-1));

}

template< class T > inline T Align1( const T Ptr, uint64 Alignment )
{
	return (T)((uint64)Ptr + Alignment - (Ptr & (Alignment-1)));
}


class SYSTEM_API MemoryManager
{
public:
	MemoryManager();
	virtual ~MemoryManager() = 0;
	virtual void* Allocate (uint64 uiSize, uint64 uiAlignment,bool bIsArray) = 0;
	virtual void Deallocate (char* pcAddr, uint64 uiAlignment,bool bIsArray) = 0;
protected:
	static VSCriticalSection *ms_MemLock;
};
class SYSTEM_API VSCMem : public MemoryManager
{
public:
	VSCMem();
	~VSCMem();
	virtual void* Allocate (uint64 uiSize, uint64 uiAlignment,bool bIsArray);
	virtual void Deallocate (char* pcAddr, uint64 uiAlignment,bool bIsArray);	
};


//this code copy from U3 FMallocWindows.h , if you want to kown how it to work,please
#if PLATFORM_WIN && !_WIN64 && !_DEBUG
class SYSTEM_API MemoryManagerWin32 : public MemoryManager
{
public:
	MemoryManagerWin32();
	~MemoryManagerWin32();
	virtual void* Allocate (uint64 uiSize, uint64 uiAlignment,bool bIsArray);
	virtual void Deallocate (char* pcAddr, uint64 uiAlignment,bool bIsArray);
private:
	// Counts.
	enum {POOL_COUNT = 42     };
	enum {POOL_MAX   = 32768+1};

	// Forward declares.
	struct FFreeMem;
	struct FPoolTable;

	// Memory pool info. 32 bytes.
	struct FPoolInfo
	{
		DWORD	    Bytes;		// Bytes allocated for pool.
		DWORD		OsBytes;	// Bytes aligned to page size.
		uint32       Taken;      // Number of allocated elements in this pool, when counts down to zero can free the entire pool.
		BYTE*       Mem;		// Memory base.
		FPoolTable* Table;		// Index of pool.
		FFreeMem*   FirstMem;   // Pointer to first free memory in this pool.
		FPoolInfo*	Next;
		FPoolInfo**	PrevLink;

		void Link( FPoolInfo*& Before )
		{
			if( Before )
			{
				Before->PrevLink = &Next;
			}
			Next     = Before;
			PrevLink = &Before;
			Before   = this;
		}
		void Unlink()
		{
			if( Next )
			{
				Next->PrevLink = PrevLink;
			}
			*PrevLink = Next;
		}
	};

	// Information about a piece of free memory. 8 bytes.
	struct FFreeMem
	{
		FFreeMem*	Next;		// Next or MemLastPool[], always in order by pool.
		DWORD		Blocks;		// Number of consecutive free blocks here, at least 1.
		FPoolInfo* GetPool();
	};

	// Pool table.
	struct FPoolTable
	{
		FPoolInfo* FirstPool;
		FPoolInfo* ExhaustedPool;
		uint32      BlockSize;
	};


	FPoolTable  PoolTable[POOL_COUNT], OsTable;
	FPoolInfo*	PoolIndirect[32];
	FPoolTable* MemSizeToPoolTable[POOL_MAX];
	INT			PageSize;

	FPoolInfo* CreateIndirect()
	{
		FPoolInfo* Indirect = (FPoolInfo*)VirtualAlloc( NULL, 2048*sizeof(FPoolInfo), MEM_COMMIT, PAGE_READWRITE );
		if( !Indirect )
		{
			return NULL;
		}
		return Indirect;
	}
};
#elif _DEBUG
class SYSTEM_API DebugMemoryManager : public MemoryManager
{
public:
	DebugMemoryManager();
	~DebugMemoryManager();

	virtual void* Allocate(uint64 uiSize, uint64 uiAlignment, bool bIsArray);
	virtual void Deallocate(char* pcAddr, uint64 uiAlignment, bool bIsArray);

private:
	enum
	{
		BEGIN_MASK = 0xDEADC0DE,
		END_MASK = 0xDEADC0DE,
		RECORD_NUM = 32, //必须大于2
		CALLSTACK_NUM = 32
	};


	class Block
	{
	public:
		Block()
		{
			for (uint32 i = 0; i < CALLSTACK_NUM; i++)
			{
				pAddr[i] = NULL;
			}
			m_pPrev = NULL;
			m_pNext = NULL;
			m_bAlignment = false;
			m_bIsArray = false;
			m_uiSize = 0;
			m_uiStackInfoNum = 0;
		}
		void * pAddr[CALLSTACK_NUM];	//申请内存时候的调用堆栈信息
		uint32 m_uiStackInfoNum;	//堆栈层数
		uint64	 m_uiSize;			//申请空间的大小
		bool m_bIsArray;				//是否是数组
		bool m_bAlignment;				//是否字节对齐
		Block * m_pPrev;				//前一个节点
		Block * m_pNext;				//后一个节点
	};
	uint32 m_uiNumNewCalls;
	uint32 m_uiNumDeleteCalls;
	Block* m_pHead;
	Block* m_pTail;
	uint32 m_uiNumBlocks;
	uint64 m_uiNumBytes;
	uint64 m_uiMaxNumBytes;
	uint32 m_uiMaxNumBlocks;
	uint32 m_uiSizeRecord[RECORD_NUM];
	void InsertBlock(Block* pBlock);
	void RemoveBlock(Block* pBlock);
	bool GetFileAndLine(const void *pAddress, char szFile[MAX_PATH], int32 &line);
	bool InitDbgHelpLib();
	void FreeLeakMem();
	void PrintInfo();
	void FreeDbgHelpLib();
};
#else
class SYSTEM_API MemoryManagerWin64 : public MemoryManager
{
public:
	MemoryManagerWin64();
	~MemoryManagerWin64();

	virtual void* Allocate(uint64 uiSize, uint64 uiAlignment, bool bIsArray);
	virtual void Deallocate(char* pcAddr, uint64 uiAlignment, bool bIsArray);
};
#endif


// this is Stack Mem ,it will be clear every tick ,now No considering thread safe .
// this code is modified by ue3
class SYSTEM_API StackMemoryManager :  public MemoryManager
{
public:
	StackMemoryManager(uint64 uiDefaultChunkSize = 65536);
	~StackMemoryManager();
	void* Allocate (uint64 uiSize, uint64 uiAlignment,bool bIsArray);
	void Deallocate (char* pcAddr, uint64 uiAlignment,bool bIsArray)
	{
		return;
	}
	template<class T>
	friend class StackMemoryAlloc;
	friend class StackMemoryTag;
	//每帧结束或者开始的时候调用
	void Clear();
private:

	// Types.
	struct FTaggedMemory
	{
		FTaggedMemory* Next;
		uint64 DataSize;
		BYTE Data[1];
	};

	// Variables.
	BYTE*			Top;				// Top of current chunk (Top<=End).
	BYTE*			End;				// End of current chunk.
	uint64		DefaultChunkSize;	// Maximum chunk size to allocate.
	FTaggedMemory*	TopChunk;			// Only chunks 0..ActiveChunks-1 are valid.

	/** The memory chunks that have been allocated but are currently unused. */
	FTaggedMemory*	UnusedChunks;

	/** The number of marks on this stack. */
	INT NumMarks;

	/**
	* Allocate a new chunk of memory of at least MinSize size,
	* and return it aligned to Align. Updates the memory stack's
	* Chunks table and ActiveChunks counter.
	*/
	BYTE* AllocateNewChunk(uint64 MinSize );

	/** Frees the chunks above the specified chunk on the stack. */
	/*移除这个chunk和这个chunk之前的所有chunk*/
	void FreeChunks( FTaggedMemory* NewTopChunk );
};

// if the class has memory alloc , you must inherit from MemObject , so you can use the MemManager any where 
class SYSTEM_API MemObject
{
public:
	MemObject();
	~MemObject();
	static StackMemoryManager& GetStackMemManager();
	static MemoryManager& GetMemManager();
	static MemoryManager& GetCMemManager();
};


template<class T>
class StackMemoryAlloc : public MemObject
{
public:
	// Constructors.
	StackMemoryAlloc(uint64 uiNum = 0, uint64 uiAlignment = 0)
	{
		if (uiNum > 0)
		{
			StackMemoryManager & StackMem = GetStackMemManager();
			m_uiNum = uiNum;			
			Top = StackMem.Top;
			SavedChunk = StackMem.TopChunk;		
			// Track the number of outstanding marks on the stack.
			StackMem.NumMarks++;
						
			m_pPtr = (T *)StackMem.Allocate(uiNum * sizeof(T),uiAlignment,0);
			VSMAC_ASSERT(m_pPtr);
			if (ValueBase<T>::NeedsConstructor)
			{
				for (uint32 i = 0 ; i < uiNum ; i++)
				{
					MEM_NEW(m_pPtr + i)T();
				}
			}
		}
	}

	/** Destructor. */
	~StackMemoryAlloc()
	{

		if (m_uiNum > 0)
		{
			StackMemoryManager & StackMem = GetStackMemManager();

			if (ValueBase<T>::NeedsDestructor)
			{
				for (uint32 i = 0 ; i < m_uiNum ; i++)
				{
					(m_pPtr + i)->~T();
				}
			}
			
			// Track the number of outstanding marks on the stack.
			--StackMem.NumMarks;

			// Unlock any new chunks that were allocated.
			if( SavedChunk != StackMem.TopChunk )
				StackMem.FreeChunks( SavedChunk );

			// Restore the memory stack's state.
			StackMem.Top = Top;

			// Ensure that the mark is only popped once by clearing the top pointer.
			Top = NULL;
		}
	}

	inline T * GetPtr()const
	{
		return m_pPtr;
	}
	inline uint64 size() const
	{
		return m_uiNum;
	}
private:
	BYTE* Top;
	StackMemoryManager::FTaggedMemory* SavedChunk;
	T * m_pPtr;
	uint64 m_uiNum;
};


class SYSTEM_API DefaultContainerMemory
{
public:
	DefaultContainerMemory()
	{

	}
	virtual ~DefaultContainerMemory()
	{

	}
	virtual void* Allocate(uint64 uiSize, uint64 uiAlignment, bool bIsArray)
	{
		return MemObject::GetMemManager().Allocate(uiSize, uiAlignment, bIsArray);
	}
	virtual void Deallocate(char* pcAddr, uint64 uiAlignment, bool bIsArray)
	{
		return MemObject::GetMemManager().Deallocate(pcAddr, uiAlignment, bIsArray);
	}
};


class SYSTEM_API StackContainerMemory
{
public:
	StackContainerMemory()
	{

	}
	virtual ~StackContainerMemory()
	{

	}
	virtual void* Allocate(uint32 uiSize, uint32 uiAlignment, bool bIsArray)
	{
		return MemObject::GetStackMemManager().Allocate(uiSize, uiAlignment, bIsArray);
	}
	virtual void Deallocate(char* pcAddr, uint32 uiAlignment, bool bIsArray)
	{
		return MemObject::GetStackMemManager().Deallocate(pcAddr, uiAlignment, bIsArray);
	}
};


class StackMemoryTag: public MemObject
{
public:
	// Constructors.
	StackMemoryTag()
	{
		StackMemoryManager & StackMem = GetStackMemManager();
		StackMem.NumMarks++;
		Top = StackMem.Top;
		SavedChunk = StackMem.TopChunk;
	}

	/** Destructor. */
	~StackMemoryTag()
	{
		StackMemoryManager & StackMem = GetStackMemManager();

		// Unlock any new chunks that were allocated.
		if (SavedChunk != StackMem.TopChunk)
			StackMem.FreeChunks(SavedChunk);

		// Restore the memory stack's state.
		StackMem.Top = Top;
		StackMem.NumMarks--;
	}
private:
	BYTE* Top;
	StackMemoryManager::FTaggedMemory* SavedChunk;
};


class VSDefaultContainerMem
{
public:
    VSDefaultContainerMem()
    {

    }
    virtual ~VSDefaultContainerMem()
    {

    }
    virtual void* Allocate(unsigned long long uiSize, unsigned long long uiAlignment, bool bIsArray)
    {
        return MemObject::GetMemManager().Allocate(uiSize, uiAlignment, bIsArray);
    }
    virtual void Deallocate(char* pcAddr, unsigned long long uiAlignment, bool bIsArray)
    {
        return MemObject::GetMemManager().Deallocate(pcAddr, uiAlignment, bIsArray);
    }
};


}



inline void* operator new(zq::uint64 uiSize)
{
	return zq::MemObject::GetMemManager().Allocate(uiSize, 0, false);
}

inline void* operator new[](zq::uint64 uiSize)
{
	return zq::MemObject::GetMemManager().Allocate(uiSize, 0, true);
}

inline void operator delete (void* pvAddr)
{
	return zq::MemObject::GetMemManager().Deallocate((char *)pvAddr, 0, false);
}

inline void operator delete[](void* pvAddr)
{
	return zq::MemObject::GetMemManager().Deallocate((char *)pvAddr, 0, true);
}

#define SAFE_DELETE(p) if(p){MEM_DELETE p; p = 0;}
#define SAFE_DELETEA(p) if(p){MEM_DELETE []p; p = 0;}
