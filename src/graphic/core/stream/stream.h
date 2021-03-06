#pragma once
#include "graphic/core/rtti.h"
#include "graphic/core/pointer.h"
//#include "datastruct/VSArray.h"
//#include "VSMap.h"
//#include "VSString.h"
#include "core/file.h"
#include "graphic/core/type.h"
#include "graphic/core/resource.h"
#include "graphic/core/stream/customarchiveobject.h"
#include "graphic/core/object.h"
namespace zq
{
DECLARE_Ptr(VSObject);


// 默认clone指针类型VSType property 都是会重新创建一个实例然后再拷贝数据，为了让不重新创建，添加property的时候，这个property不要加入VSProperty::F_CLONE标志
//然后再postclone 函数中自己再重新设置 如果只是默认的值拷贝而非创建实例可以设置F_COPY
template<typename T>
void Copy(T & Dest,T & Src,VSMap<VSObject *,VSObject*>& CloneMap)
{
	if (TIsVSResourceProxyPointerType<T>::Value)
	{
		Dest = Src;
	}
	else if(TIsVSPointerType<T>::Value)
	{
		VSObject* & TempSrc = *(VSObject**)(void *)&Src;	
		VSObject* & TempDest = *(VSObject**)(void *)&Dest;
		TempDest = VSObject::_CloneCreateObject(TempSrc,CloneMap);
	}
	else if (TIsVSType<T>::Value)
	{
		VSObject* TempSrc = (VSObject *)&Src;	
		VSObject* TempDest = (VSObject *)&Dest;
		VSObject::_CloneObject(TempSrc,TempDest,CloneMap);
	}
	else if(TIsVSSmartPointerType<T>::Value)
	{
		VSObjectPtr & TempSrc = *(VSObjectPtr*)(void *)&Src;
		VSObjectPtr & TempDest = *(VSObjectPtr*)(void *)&Dest;
		TempDest = VSObject::_CloneCreateObject(TempSrc,CloneMap);
	}
	else if (TIsCustomType<T>::Value)
	{
		VSCustomArchiveObject * TempSrc = (VSCustomArchiveObject*)(void *)&Src;
		VSCustomArchiveObject * TempDest = (VSCustomArchiveObject*)(void *)&Dest;
		TempDest->CopyFrom(TempSrc,CloneMap);
	}
	else 
	{
		Dest = Src;
	}
}
template<typename T,class VSMemManagerClass>
void Copy(VSArray<T,VSMemManagerClass> & Dest, VSArray<T,VSMemManagerClass> & Src,VSMap<VSObject *,VSObject*>& CloneMap)
{
	Dest.Clear();
	Dest.SetBufferNum(Src.GetNum());
	for (uint32 i = 0 ; i < Src.GetNum() ;i++)
	{
		Copy(Dest[i],Src[i],CloneMap);
	}
}

template<class Key,class Value,class VSMemManagerClass>
void Copy(VSMap<Key,Value,VSMemManagerClass> & Dest, VSMap<Key,Value,VSMemManagerClass> & Src, VSMap<VSObject *,VSObject*>& CloneMap)
{
	Dest.Clear();
	Dest.SetBufferNum(Src.GetNum());
	for (uint32 i = 0 ; i < Src.GetNum() ;i++)
	{
		MapElement<Key,Value> &ElementSrc = Src[i];
		MapElement<Key,Value> &ElementDest = Dest[i];
		Copy(ElementDest.Key, ElementSrc.Key, CloneMap);
		Copy(ElementDest.Value, ElementSrc.Value, CloneMap);
	}
}
class GRAPHIC_API VSStream
{
public:
	enum // Archive Type
	{
		AT_SAVE,
		AT_LOAD,
		AT_LINK,
		AT_REGISTER,
		AT_SIZE,
		AT_POSTLOAD,
		AT_OBJECT_COLLECT_GC,
		AT_CLEAR_OBJECT_PROPERTY_GC,
		AT_LOAD_OBJECT_COLLECT_GC,
	};
	template<class T>
	void Archive(T & Io)
	{
		if (m_uiStreamFlag == AT_LOAD)
		{
			
			if (TIsVSResourceProxyPointerType<T>::Value)
			{
				VSResourceProxyBasePtr & Temp = *(VSResourceProxyBasePtr*)(void *)&Io;
				ReadResource(Temp);
			}
			else if(TIsVSPointerType<T>::Value)
			{
				VSObject* & Temp = *(VSObject**)(void *)&Io;			 
				ReadObjectGUID(Temp);
			}
			else if (TIsVSType<T>::Value)
			{
				VSObject* Key = (VSObject *)&Io;	
				VSObject * Value = NULL;
				ReadObjectGUID(Value);
				m_pmVSTypeLoadMap.AddElement(Key,Value);
			}
			else if(TIsVSSmartPointerType<T>::Value)
			{
				VSObjectPtr & Temp = *(VSObjectPtr*)(void *)&Io;
				ReadObjectGUID(Temp);
			}
			else if (TIsVSStringType<T>::Value)
			{
				VSString & Temp = *(VSString*)(void *)&Io;
				ReadString(Temp);
			}
			else if (TIsCustomType<T>::Value)
			{
				VSCustomArchiveObject * Temp = (VSCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}
				
			}
			else 
			{
				Read((void*)&Io,sizeof(T));	
			}
		}
		else if (m_uiStreamFlag == AT_SAVE)
		{
			if (TIsVSResourceProxyPointerType<T>::Value)
			{
				VSResourceProxyBasePtr & Temp = *(VSResourceProxyBasePtr*)(void *)&Io;
				WriteResource(Temp);
				
			}
			else if(TIsVSPointerType<T>::Value)
			{
				VSObject* & Temp = *(VSObject**)(void *)&Io;	
				WriteObjectGUID(Temp);
			}
			else if(TIsVSSmartPointerType<T>::Value)
			{
				VSObjectPtr & Temp = *(VSObjectPtr*)(void *)&Io;
				WriteObjectGUID(Temp);
			}
			else if (TIsVSStringType<T>::Value)
			{
				VSString & Temp = *(VSString*)(void *)&Io;
				WriteString(Temp);
			}
			else if (TIsCustomType<T>::Value)
			{
				VSCustomArchiveObject * Temp = (VSCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}
				
			}
			else if (TIsVSType<T>::Value)
			{
				VSObject*  Temp = (VSObject *)&Io;	
				WriteObjectGUID(Temp);
			}
			else 
			{
				Write((void*)&Io,sizeof(T));	
			}
		}	
		else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_POSTLOAD)
		{
			if(TIsVSPointerType<T>::Value)
			{
				VSObject* & Temp = *(VSObject**)(void *)&Io;	
				ArchiveAll(Temp);
			}
			else if(TIsVSSmartPointerType<T>::Value)
			{
				VSObjectPtr & Temp = *(VSObjectPtr*)(void *)&Io;
				ArchiveAll(Temp);
			}
			else if (TIsCustomType<T>::Value)
			{
				VSCustomArchiveObject * Temp = (VSCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}			
			}
			else if (TIsVSType<T>::Value)
			{
				VSObject*  Temp = (VSObject *)&Io;	
				ArchiveAll(Temp);
			}
		}
		else if (m_uiStreamFlag == AT_SIZE)
		{
			if (TIsVSResourceProxyPointerType<T>::Value)
			{
				VSResourceProxyBasePtr & Temp = *(VSResourceProxyBasePtr*)(void *)&Io;
				m_uiArchivePropertySize += SizeOfResource(Temp);
			}
			else if(TIsVSPointerType<T>::Value || TIsVSSmartPointerType<T>::Value || TIsVSType<T>::Value)
			{
				m_uiArchivePropertySize += 4;
			}
			else if (TIsVSStringType<T>::Value)
			{
				VSString & Temp = *(VSString*)(void *)&Io;
				m_uiArchivePropertySize += GetStrDistUse(Temp);
			}
			else if (TIsCustomType<T>::Value)
			{
				VSCustomArchiveObject * Temp = (VSCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}			
			}
			else
			{
				m_uiArchivePropertySize += sizeof(T);
			}
		}
		else if (m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC)
		{
			if (TIsVSPointerType<T>::Value)
			{
				VSObject* & Temp = *(VSObject**)(void *)&Io;
				
				if (Temp)
				{
					if (Temp->IsHasFlag(VSObject::OF_PendingKill))
					{
						Temp = NULL;
						return;
					}
					ArchiveAll(Temp);
				}
				
			}
			else if (TIsVSSmartPointerType<T>::Value)
			{
				VSObjectPtr & Temp = *(VSObjectPtr*)(void *)&Io;
				if (Temp)
				{
					if (Temp->IsHasFlag(VSObject::OF_PendingKill))
					{
						Temp = NULL;
						return;
					}
					ArchiveAll(Temp);
				}
				
				
			}
			else if (TIsCustomType<T>::Value)
			{
				VSCustomArchiveObject * Temp = (VSCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}
				
			}
			else if (TIsVSType<T>::Value)
			{
				VSObject*  Temp = (VSObject *)&Io;
				ArchiveAll(Temp);
			}
		}
		else if (m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
		{
			if (TIsVSPointerType<T>::Value)
			{
				VSObject* & Temp = *(VSObject**)(void *)&Io;
				Temp = NULL;
			}
			else if (TIsVSSmartPointerType<T>::Value)
			{
				VSObjectPtr & Temp = *(VSObjectPtr*)(void *)&Io;
				VSObject * LocalTemp = Temp;
				Temp = NULL;
				if (LocalTemp)
				{
					if (LocalTemp->IsHasFlag(VSObject::OF_REACH))
					{
						LocalTemp->ClearFlag(VSObject::OF_PendingKill);
					}
				}
				

			}
			else if (TIsCustomType<T>::Value)
			{
				VSCustomArchiveObject * Temp = (VSCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}
				
			}
			else if (TIsVSType<T>::Value)
			{
				VSObject*  Temp = (VSObject *)&Io;
				ArchiveAll(Temp);
			}
		}
		else if (m_uiStreamFlag == AT_LINK)
		{
			if(TIsVSPointerType<T>::Value)
			{
				VSObject* & Temp = *(VSObject**)(void *)&Io;
				LinkObjectPtr(Temp);
			}
			else if(TIsVSSmartPointerType<T>::Value)
			{
				VSObjectPtr & Temp = *(VSObjectPtr*)(void *)&Io;
				LinkObjectPtr(Temp);
			}
			else if (TIsVSType<T>::Value)
			{
				VSObject*  Key = (VSObject *)&Io;	
				VSObject * Value = NULL;
				Value = GetVSTypeMapValue(Key);
				LinkObjectPtr(Value);
				VSMAC_ASSERT(Value);
				VSObject::CloneObject(Value,Key);
// 				uint32 uiIndex = m_pVObjectArray.FindElement(Value);
// 				VSMAC_ASSERT(uiIndex < m_CopyUsed.GetNum());
// 				m_CopyUsed[uiIndex] = true;
			}
			else if (TIsCustomType<T>::Value)
			{
				VSCustomArchiveObject * Temp = (VSCustomArchiveObject*)(void *)&Io;
				if (Temp)
				{
					Temp->Archive(*this);
				}
				
			}
		}
	}
	template<class T,class VSMemManagerClass>
	void Archive(VSArray<T,VSMemManagerClass> & Io)
	{
		if (m_uiStreamFlag == AT_LOAD)
		{
			uint32 uiNum = 0;
			Archive(uiNum);
			Io.SetBufferNum(uiNum);
			if (TIsNoNeedLoop<T>::Value && uiNum)
			{
				Read(Io.GetBuffer(), sizeof(T) * uiNum);
			}
			else
			{
				for (uint32 i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
			
		}
		else if(m_uiStreamFlag == AT_SAVE)
		{
			uint32 uiNum = Io.GetNum();
			Archive(uiNum);
			if (TIsNoNeedLoop<T>::Value && uiNum)
			{
				Write(Io.GetBuffer(), sizeof(T) * uiNum);
			}
			else
			{
				for (uint32 i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
		}
		else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK 
			|| m_uiStreamFlag == AT_POSTLOAD || m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
			|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
		{
			if (!TIsNeedGC<T>::Value)
			{
				return;
			}
			uint32 uiNum = Io.GetNum();
			for (uint32 i = 0 ; i < uiNum ; i++)
			{
				Archive(Io[i]);
			}
		}
		else if (m_uiStreamFlag == AT_SIZE)
		{
			uint32 uiNum = Io.GetNum();
			Archive(uiNum);
			if (TIsNoNeedLoop<T>::Value && uiNum)
			{
				m_uiArchivePropertySize += sizeof(T) * uiNum;
			}
			else
			{
				for (uint32 i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
		}

	}

	template<class T,class VSMemManagerClass>
	void Archive(VSArrayOrder<T,VSMemManagerClass> & Io)
	{
		if (m_uiStreamFlag == AT_LOAD)
		{
			uint32 uiNum = 0;
			Archive(uiNum);
			Io.SetBufferNum(uiNum);
			if (TIsNoNeedLoop<T>::Value && uiNum)
			{
				Read(Io.GetBuffer(), sizeof(T) * uiNum);
			}
			else
			{
				for (uint32 i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
		}
		else if(m_uiStreamFlag == AT_SAVE)
		{
			uint32 uiNum = Io.GetNum();
			Archive(uiNum);
			if (TIsNoNeedLoop<T>::Value && uiNum)
			{
				Write(Io.GetBuffer(), sizeof(T) * uiNum);
			}
			else
			{
				for (uint32 i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
		}
		else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK 
			|| m_uiStreamFlag == AT_POSTLOAD || m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
			|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
		{
			if (!TIsNeedGC<T>::Value)
			{
				return;
			}
			uint32 uiNum = Io.GetNum();
			for (uint32 i = 0 ; i < uiNum ; i++)
			{
				Archive(Io[i]);
			}
		}
		else if (m_uiStreamFlag == AT_SIZE)
		{
			uint32 uiNum = Io.GetNum();
			Archive(uiNum);
			if (TIsNoNeedLoop<T>::Value && uiNum)
			{
				m_uiArchivePropertySize += sizeof(T) * uiNum;
			}
			else
			{
				for (uint32 i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
		}

	}

	template<class Key,class Value,class VSMemManagerClass>
	void Archive(VSMap<Key,Value,VSMemManagerClass> & Io)
	{
		if (m_uiStreamFlag == AT_LOAD)
		{
			uint32 uiNum = 0;
			Archive(uiNum);
			Io.SetBufferNum(uiNum);
			if (TIsNoNeedLoop<Key>::Value && TIsNoNeedLoop<Value>::Value && uiNum)
			{
				Read(Io.GetBuffer(), sizeof(MapElement<Key,Value>) * uiNum);
			}
			else
			{
				for (uint32 i = 0; i < uiNum; i++)
				{
					MapElement<Key, Value> &Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
		}
		else if(m_uiStreamFlag == AT_SAVE)
		{
			uint32 uiNum = Io.GetNum();
			Archive(uiNum);
			if (TIsNoNeedLoop<Key>::Value && TIsNoNeedLoop<Value>::Value && uiNum)
			{
				Write(Io.GetBuffer(), sizeof(MapElement<Key, Value>) * uiNum);
			}
			else
			{
				for (uint32 i = 0; i < uiNum; i++)
				{
					MapElement<Key, Value> &Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
		}
		else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK || m_uiStreamFlag == AT_POSTLOAD
			|| m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
			|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
		{
			if (!(TIsNeedGC<Key>::Value || TIsNeedGC<Value>::Value))
			{
				return;
			}
			uint32 uiNum = Io.GetNum();
			for (uint32 i = 0 ; i < uiNum ; i++)
			{
				MapElement<Key,Value> &Element = Io[i];
				if (TIsNeedGC<Key>::Value)
				{
					Archive(Element.Key);
				}
				if (TIsNeedGC<Value>::Value)
				{
					Archive(Element.Value);
				}			
			}
		}
		else if (m_uiStreamFlag == AT_SIZE)
		{
			uint32 uiNum = Io.GetNum();
			Archive(uiNum);
			if (TIsNoNeedLoop<Key>::Value && TIsNoNeedLoop<Value>::Value && uiNum)
			{
				m_uiArchivePropertySize += sizeof(MapElement<Key, Value>) * uiNum;
			}
			else
			{
				for (uint32 i = 0; i < uiNum; i++)
				{
					MapElement<Key, Value> &Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
		}
	}

    template<class Key, class Value, class VSMemManagerClass>
    void Archive(VSMapOrder<Key, Value, VSMemManagerClass> & Io)
    {
        if (m_uiStreamFlag == AT_LOAD)
        {
            unsigned int uiNum = 0;
            Archive(uiNum);
            Io.SetBufferNum(uiNum);
            if (TIsNoNeedLoop<Key>::Value && TIsNoNeedLoop<Value>::Value && uiNum)
            {
                Read(Io.GetBuffer(), sizeof(MapElement<Key, Value>) * uiNum);
            }
            else
            {
                for (unsigned int i = 0; i < uiNum; i++)
                {
                    MapElement<Key, Value> &Element = Io[i];
                    Archive(Element.Key);
                    Archive(Element.Value);
                }
            }
        }
        else if (m_uiStreamFlag == AT_SAVE)
        {
            unsigned int uiNum = Io.GetNum();
            Archive(uiNum);
            if (TIsNoNeedLoop<Key>::Value && TIsNoNeedLoop<Value>::Value && uiNum)
            {
                Write(Io.GetBuffer(), sizeof(MapElement<Key, Value>) * uiNum);
            }
            else
            {
                for (unsigned int i = 0; i < uiNum; i++)
                {
                    MapElement<Key, Value> &Element = Io[i];
                    Archive(Element.Key);
                    Archive(Element.Value);
                }
            }
        }
        else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK || m_uiStreamFlag == AT_POSTLOAD
            || m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
            || m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
        {
            if (!(TIsNeedGC<Key>::Value || TIsNeedGC<Value>::Value))
            {
                return;
            }
            unsigned int uiNum = Io.GetNum();
            for (unsigned int i = 0; i < uiNum; i++)
            {
                MapElement<Key, Value> &Element = Io[i];
                if (TIsNeedGC<Key>::Value)
                {
                    Archive(Element.Key);
                }
                if (TIsNeedGC<Value>::Value)
                {
                    Archive(Element.Value);
                }
            }
        }
        else if (m_uiStreamFlag == AT_SIZE)
        {
            unsigned int uiNum = Io.GetNum();
            Archive(uiNum);
            if (TIsNoNeedLoop<Key>::Value && TIsNoNeedLoop<Value>::Value && uiNum)
            {
                m_uiArchivePropertySize += sizeof(MapElement<Key, Value>) * uiNum;
            }
            else
            {
                for (unsigned int i = 0; i < uiNum; i++)
                {
                    MapElement<Key, Value> &Element = Io[i];
                    Archive(Element.Key);
                    Archive(Element.Value);
                }
            }
        }
    }
	
	bool WriteResource(VSResourceProxyBasePtr& Resource);

	virtual bool ReadResource(VSResourceProxyBasePtr& Resource);

	uint32 SizeOfResource(VSResourceProxyBasePtr& Resource);
	uint32 GetStreamFlag()const
	{
		return m_uiStreamFlag;
	}
	void SetStreamFlag(uint32 uiStreamFlag)
	{
		m_uiStreamFlag = uiStreamFlag;
	}
	void AddBufferSize(uint32 uiSize)
	{
		m_uiArchivePropertySize += uiSize;
	}

	struct ObjectPropertyTable
	{
		ObjectPropertyTable()
		{
			m_uiOffset = 0;
			m_uiSize = 0;
			m_uiNameID = 0;
		}
		VSString m_PropertyName;
		uint32 m_uiOffset;
		uint32 m_uiSize;
		uint32 m_uiNameID;
	};

	struct ObjectTableType
	{
		ObjectTableType()
		{
			m_uiGUID = 0;
			m_uiOffset = 0;
			m_uiObjectPropertySize = 0;
			m_uiObjectPropertyTableSize = 0;
			m_uiObjectPropertyNum = 0;
		}
		uint32 m_uiGUID;
		VSString m_RttiName;
		uint32 m_uiOffset;
		uint32 m_uiObjectPropertySize;
		uint32 m_uiObjectPropertyTableSize;
		uint32 m_uiObjectPropertyNum;
		VSArray<ObjectPropertyTable> m_ObjectPropertyTable;
	};
	bool m_bLoadUseGC;
protected:
	uint32 m_uiStreamFlag;
	uint32 m_uiArchivePropertySize;
public:
	virtual bool Load(const char * const pcFileName);
	virtual bool Save(const char * const pcFileName);

	virtual bool LoadFromBuffer(unsigned char * pBuffer,uint32 uiSize);

	bool ArchiveAll(VSObject * pObject);
public:
	bool RegisterObject(VSObject * pObject);

	bool RegisterPostLoadObject(VSObject * pObject);

	bool RegisterReachableObject(VSObject * pObject);

	VSStream(uint32 dwFlag = 0);
	~VSStream();
	bool Read(void * pvBuffer,uint32 uiSize);
	bool Write(const void * pvBuffer,uint32 uiSize);
	bool ReadString(VSString &str);
	bool WriteString(const VSString &str);

	static uint32 GetStrDistUse(const VSString &Str);
	static uint32 GetStrDistUse(const char * pCh);
	const VSObject *GetLoadMapValue(uint32 uiKey)const;
	uint32 GetSaveMapValue(VSObject * Key)const;
	VSObject *GetVSTypeMapValue(VSObject *pKey)const;
	virtual const VSObject *GetObjectByRtti(const VSRtti &Rtti);
	bool GetObjectArrayByRtti(const VSRtti &Rtti,VSArray<VSObject *> &ObjectArray,bool IsDerivedFrom = false);
	bool GetAllResourceObject(VSArray<VSObject *> &ObjectArray);
	void GetObjectFailed();
	inline uint32 GetVersion() const
	{
		return m_uiVersion;
	}

	static uint32 GetCurVersion()
	{
		return ms_uiCurVersion;
	}
	enum
	{
		UNICODE_FLAG = BIT(0),
		ASYN_LOAD_FLAG = BIT(1)
	};
	inline bool HasAnyFlag(uint32 dwFlagsToCheck)const
	{
		return (m_dwFlag & dwFlagsToCheck) != 0;
	}
	template<class T>
	bool ReadObjectGUID(VSPointer<T> & Pointer);

	template<class T>
	bool WriteObjectGUID(const VSPointer<T> & Pointer) ;

	template<class T>
	bool LinkObjectPtr(VSPointer<T> & Pointer);

	template<class T>
	bool ReadObjectGUID(T * &pObject);

	template<class T>
	bool WriteObjectGUID(T * const&pObject);

	template<class T>
	bool LinkObjectPtr(T * &pObject);
protected:
	unsigned char * m_pcCurBufPtr;
	unsigned char * m_pcBuffer;
	uint32 m_uiBufferSize;
	VSArray<VSObject *> m_pVObjectArray;
	VSArray<VSObject *> m_pPostLoadObject;

	VSMap<unsigned int,VSObject*> m_pmLoadMap;
	VSMap<VSObject*, unsigned int> m_pmSaveMap;
	VSMap<VSObject*,VSObject*> m_pmVSTypeLoadMap;
	
	static uint32 ms_uiCurVersion;
	uint32 m_uiVersion;
    uint32 m_dwFlag;
};

template<class T>
bool VSStream::ReadObjectGUID(VSPointer<T> & Pointer)
{
	uint32 uiGUID = 0;
	if(!Read( &uiGUID, sizeof(unsigned int) ))
		return 0;
	uint64 Temp = GUID32ToSizeType(uiGUID);
	Pointer.SetObject((T *)(Temp));
	return 1;
}
/********************************************************************************/
template<class T>
bool VSStream::WriteObjectGUID(const VSPointer<T> & Pointer)
{
	T* pP = Pointer;
	uint32 uiGUID =  GetSaveMapValue(pP);
	if(!Write( &uiGUID, sizeof(unsigned int) ))
		return 0;
	return 1;
}
/********************************************************************************/
template<class T>
bool VSStream::LinkObjectPtr(VSPointer<T> & Pointer)
{
	uint32 uiGUID = SizeTypeToGUID32((uint64)Pointer.GetObject());

	Pointer.SetObject(NULL);

	Pointer = (T *)GetLoadMapValue(uiGUID);
	return 1;
}
/********************************************************************************/
template<class T>
bool VSStream::ReadObjectGUID(T * &pObject)
{
	uint32 uiGUID = 0;
	if (!Read(&uiGUID, sizeof(unsigned int)))
		return 0;
	uint64 Temp = GUID32ToSizeType(uiGUID);
	pObject = (T*)(Temp);
	return 1;
}
/********************************************************************************/
template<class T>
bool VSStream::WriteObjectGUID(T * const &pObject)
{
	uint32 uiGUID = GetSaveMapValue(pObject);
	if(!Write( &uiGUID, sizeof(unsigned int) ))
		return 0;
	return 1;
}
/********************************************************************************/
template<class T>
bool VSStream::LinkObjectPtr(T * &pObject)
{
	uint32 uiGUID = SizeTypeToGUID32((uint64)pObject);
	pObject = (T *)GetLoadMapValue(uiGUID);
	return 1;
}	
}
