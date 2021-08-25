#pragma once


#include "core/system.h"
#include "graphic/core/reference.h"
#include "graphic/core/priority.h"
#include "graphic/core/rtti.h"
#include "core/log.h"
#include "graphic/core/main.h"
#include "datastruct/VSMap.h"
#include "datastruct/VSString.h"
#include "graphic/core/name.h"
#include "graphic/core/initialmarc.h"
#include "datastruct/VSList.h"
#include "datastruct/VSHash.h"
#include "graphic/core/streamingtype.h"


namespace zq{


typedef VSObject *(*FactoryFunction)();
class VSObject;
class VSRenderer;
class VSResourceIdentifier;
class VSStream;
class VSResourceProxyBase;
class VSFastObjectManager
{
public:
	VSFastObjectManager();
	~VSFastObjectManager();
	void AddObject(VSObject * p);
	void DeleteObject(VSObject * p);
	bool IsClear();
	void PrepareForGC();
	uint32 GetObjectNum();
protected:
	VSHashTree<VSObject *> ObjectHashTree;
	uint32 m_uiObjectNum;
};


class GRAPHIC_API VSObject : public VSReference , public MemObject
{			
public:
	
	friend class VSStream;
	friend class VSAsynStream;
	virtual ~VSObject() = 0;
	VSObject(const VSObject & object);
	VSObject & operator =(const VSObject &object);
	VSObject();

//RTTI
	DECLARE_RTTI;
public:
	bool IsSameType(const VSObject *pObject)const;
	bool IsDerived(const VSObject *pObject)const;
	bool IsSameType(const VSRtti &Type)const;
	bool IsDerived(const VSRtti &Type)const;

	DECLARE_INITIAL_NO_CLASS_FACTORY;
	

//Stream
public:
	static VSObject * GetInstance(const VSString& sRttiName);
	static VSObject * GetInstance(const VSRtti& Rtti);
	template<typename T>
	static T * GetInstance()
	{
		return (T *)GetInstance(T::ms_Type);
	}
	virtual bool BeforeSave(VSStream* pStream);
	virtual bool PostSave(VSStream* pStream);
	virtual bool PostLoad(VSStream* pStream);
protected:
	static VSMap<VSUsedName,FactoryFunction> ms_ClassFactory;
	static VSObject * GetNoGCInstance(const VSString& sRttiName);
	
public:
	friend class VSFastObjectManager;
	static VSFastObjectManager & GetObjectManager()
	{
		static VSFastObjectManager ms_ObjectManager;
		return  ms_ObjectManager;
	}
	virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data = NULL);
	virtual void GetStreamResource(VSArray<VSResourceProxyBase *>& pResourceProxy, StreamInformation_TYPE & StreamInformation)const;
	
//debug
public:
	bool DebugLevel(Log & log)const;
	static VSObject* _CloneCreateObject(VSObject * pObject,VSMap<VSObject *,VSObject*>& CloneMap);
	static void _CloneObject(VSObject * pObjectSrc,VSObject * pObjectDest,VSMap<VSObject *,VSObject*>& CloneMap);

	static VSObject* CloneCreateObject(VSObject * pObject);
	static void CloneObject(VSObject * pObjectSrc,VSObject * pObjectDest);
	

	virtual bool PostClone(VSObject * pObjectSrc);
	virtual void ValueChange(VSString & Name);
	bool Process(VSUsedName & FunName, void * para, void *ret = NULL , int32 ParaNum = -1);

	void CallVoidFun(VSUsedName &FunName)
	{
		Process(FunName, NULL);
	}
	template<class Type1>
	void CallVoidFun(VSUsedName &FunName,Type1& t1)
	{
		struct MyStruct
		{
			Type1 t1;
		}Temp;
		Temp.t1 = t1;
		Process(FunName, (void *)&Temp, NULL, 1);
	}
	template<class Type1, class Type2>
	void CallVoidFun(VSUsedName &FunName, Type1& t1, Type2& t2)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Process(FunName, (void *)&Temp, NULL, 2);
	}

	template<class Type1, class Type2, class Type3>
	void CallVoidFun(VSUsedName &FunName, Type1& t1, Type2& t2, Type3& t3)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
			Type3 t3;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Temp.t3 = t3;
		Process(FunName, (void *)&Temp, NULL, 3);
	}

	template<class Type1, class Type2, class Type3,class Type4>
	void CallVoidFun(VSUsedName &FunName, Type1& t1, Type2& t2, Type3& t3, Type4& t4)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
			Type3 t3;
			Type4 t4;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Temp.t3 = t3;
		Temp.t4 = t4;
		Process(FunName, (void *)&Temp, NULL, 4);
	}
	template<class ReturnType>
	void CallFun(VSUsedName &FunName, ReturnType& ReturnValue)
	{
		Process(FunName, NULL, (void *)&ReturnValue);
	}
	template<class ReturnType,class Type1>
	void CallFun(VSUsedName &FunName, ReturnType& ReturnValue, Type1& t1)
	{
		struct MyStruct
		{
			Type1 t1;
		}Temp;
		Temp.t1 = t1;
		Process(FunName, (void *)&Temp, (void *)&ReturnValue, 1);
	}
	template<class ReturnType,class Type1, class Type2>
	void CallFun(VSUsedName &FunName, ReturnType& ReturnValue, Type1& t1, Type2& t2)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Process(FunName, (void *)&Temp, (void *)&ReturnValue, 2);
	}

	template<class ReturnType,class Type1, class Type2, class Type3>
	void CallFun(VSUsedName &FunName, ReturnType& ReturnValue, Type1& t1, Type2& t2, Type3& t3)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
			Type3 t3;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Temp.t3 = t3;
		Process(FunName, (void *)&Temp, (void *)&ReturnValue, 3);
	}

	template<class ReturnType,class Type1, class Type2, class Type3, class Type4>
	void CallFun(VSUsedName &FunName, ReturnType& ReturnValue,Type1& t1, Type2& t2, Type3& t3, Type4& t4)
	{
		struct MyStruct
		{
			Type1 t1;
			Type2 t2;
			Type3 t3;
			Type4 t4;
		}Temp;
		Temp.t1 = t1;
		Temp.t2 = t2;
		Temp.t3 = t3;
		Temp.t4 = t4;
		Process(FunName, (void *)&Temp, (void *)&ReturnValue, 4);
	}
public:
	enum //Object Flag
	{
		OF_REACH = 0x01,
		OF_UNREACH = 0x02,
		OF_PendingKill = 0x04,
		OF_GCObject = 0x08,
		OF_RootObject = 0x10,
		OF_MAX
	};
	inline void SetFlag(uint32 uiFlag)
	{

		m_uiFlag |= uiFlag;
	}
	inline void ClearFlag(uint32 uiFlag)
	{
		m_uiFlag &= ~uiFlag;
	}
	inline bool IsHasFlag(uint32 uiFlag)
	{
		return (m_uiFlag & uiFlag) != 0;
	}
	uint32 m_uiFlag;
	inline void DecreRef()
	{
		VSLockedDecrement((long *)&m_iReference);
		if (!m_iReference)
		{
			if (IsHasFlag(OF_GCObject))
			{
				SetFlag(OF_PendingKill);
			}
			else
			{
				MEM_DELETE this;
			}
			
		}
	}
	
};


DECLARE_Ptr(VSObject);
VSTYPE_MARCO(VSObject);

template <class T>
T* StaticCast (VSObject* pkObj)
{
	return (T*)pkObj;
}

template <class T>
const T* StaticCast (const VSObject* pkObj)
{
	return (const T*)pkObj;
}

template<class T>
T * DynamicCast(VSObject * pObj)
{
	return pObj && pObj->IsDerived(T::ms_Type)?(T*)pObj:0;
}

template<class T>
const T * DynamicCast(const VSObject * pObj)
{
	return pObj && pObj->IsDerived(T::ms_Type)?(const T*)pObj:0;
}


}
