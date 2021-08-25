#include "graphic/core/stream/stream.h"
#include "graphic/core/object.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/config.h"
using namespace zq;
uint32 VSStream::ms_uiCurVersion = 0;
/********************************************************************************/
uint32 VSStream::GetStrDistUse(const VSString &Str)
{
	return sizeof(int) + (unsigned int)Str.GetLength() * sizeof(char);

}

uint32 VSStream::GetStrDistUse(const char* pCh)
{
	return sizeof(int) + (unsigned int)VSStrLen(pCh) * sizeof(char);
}

VSStream::VSStream(uint32 dwFlag)
{
	m_pVObjectArray.Clear();
	m_pmLoadMap.Clear();
	m_pmVSTypeLoadMap.Clear();
	m_pcCurBufPtr	= NULL;
	m_pcBuffer		= NULL;
	m_uiBufferSize	= 0;
	m_dwFlag = dwFlag;
	m_bLoadUseGC = false;
}

VSStream::~VSStream()
{

	m_pVObjectArray.Clear();
	m_pcCurBufPtr	= NULL;

}

bool VSStream::Read(void* pvBuffer,uint32 uiSize)
{

	if(!pvBuffer || !m_pcBuffer)
		return 0;

	if(unsigned int(m_pcCurBufPtr + uiSize - m_pcBuffer) > m_uiBufferSize)
		return 0;

	
	VSMemcpy(pvBuffer,m_pcCurBufPtr,uiSize);

	m_pcCurBufPtr +=uiSize;
	return 1;

}

bool VSStream::Write(const void * pvBuffer,uint32 uiSize)
{
	if(!pvBuffer || !m_pcBuffer)
		return 0;

	if(unsigned int(m_pcCurBufPtr + uiSize - m_pcBuffer) > m_uiBufferSize)
		return 0;


	VSMemcpy(m_pcCurBufPtr,pvBuffer,uiSize);

	m_pcCurBufPtr +=uiSize;
	return 1;
}

bool VSStream::ReadString(VSString & VSString)
{
	uint32 uiBufferSize = 0;
	uint32 uiStrLen = 0;
	if(!Read(&uiBufferSize,sizeof(unsigned int)))
		return 0;
	if (!uiBufferSize)
	{
		return 1;
	}
	char * pCh = 0;
	pCh= MEM_NEW char[uiBufferSize];
	if(!Read(pCh,uiBufferSize))
	{
		SAFE_DELETEA(pCh);
		return 0;
	}
	if (HasAnyFlag(UNICODE_FLAG))
	{
		uiStrLen = uiBufferSize >> 1;
		VSString.SetTCHARBufferNum(uiStrLen);
#ifdef	_UNICODE		
		VSMemcpy(VSString.GetBuffer(),pCh,uiBufferSize);
#else
		VSWcsToMbs(VSString.GetBuffer(),uiStrLen,(wchar_t *)pCh,uiStrLen);
#endif
	}
	else
	{
		uiStrLen = uiBufferSize;
		VSString.SetTCHARBufferNum(uiStrLen);
#ifdef	_UNICODE
		VSMbsToWcs((wchar_t *)VSString.GetBuffer(),uiStrLen,pCh,uiStrLen);
#else
		VSMemcpy(VSString.GetBuffer(),pCh,uiBufferSize);
#endif
	}
	SAFE_DELETEA(pCh);

	return 1;

}

bool VSStream::WriteString(const VSString & VSString)
{
	uint32 uiBufferLen = VSString.GetLength() * sizeof(char);
	if(!Write(&uiBufferLen,sizeof(unsigned int)))
		return 0;
	if(!Write(VSString.GetBuffer(),uiBufferLen))
		return 0;
	return 1;
}
bool VSStream::RegisterReachableObject(VSObject * pObject)
{
	if (pObject->IsHasFlag(VSObject::OF_REACH))
	{
		return false;
	}
	pObject->SetFlag(VSObject::OF_REACH);
	pObject->ClearFlag(VSObject::OF_UNREACH);

	return 1;
}
bool VSStream::RegisterPostLoadObject(VSObject * pObject)
{
	for(uint32 i = 0 ; i < (unsigned int)m_pPostLoadObject.GetNum(); i++)
	{
		if(m_pPostLoadObject[i] == pObject)
		{
			return 0;
		}

	}
	m_pPostLoadObject.AddElement((VSObject *)pObject);	
	return true;
}

bool VSStream::RegisterObject(VSObject * pObject)
{
	VSMAC_ASSERT(pObject);
	if(!pObject)
		return 0;

	for(uint32 i = 0 ; i < (unsigned int)m_pVObjectArray.GetNum(); i++)
	{
		if(m_pVObjectArray[i] == pObject)
		{
			return 0;
		}

	}
	m_pVObjectArray.AddElement((VSObject *)pObject);

	return 1;
}

VSObject *VSStream::GetVSTypeMapValue(VSObject *pKey)const
{
	uint32 i = m_pmVSTypeLoadMap.Find(pKey);
	if(i == m_pmVSTypeLoadMap.GetNum())
		return NULL;

	return m_pmVSTypeLoadMap[i].Value;
}

const VSObject *VSStream::GetLoadMapValue(uint32 uiKey)const
{
	uint32 i = m_pmLoadMap.Find(uiKey);
	if(i == m_pmLoadMap.GetNum())
		return NULL;
	
	return m_pmLoadMap[i].Value;
}
uint32 VSStream::GetSaveMapValue(VSObject * Key)const
{
	uint32 i = m_pmSaveMap.Find(Key);
	if (i == m_pmSaveMap.GetNum())
		return 0;

	return m_pmSaveMap[i].Value;
}

const VSObject *VSStream::GetObjectByRtti(const VSRtti &Rtti)
{
	VSObject * pObject = NULL;
	for(uint32 i = 0 ; i < m_pVObjectArray.GetNum() ; i++)
	{
		if((m_pVObjectArray[i]->GetType()).IsSameType(Rtti))
		{
			pObject = m_pVObjectArray[i];
			break;
		}
	}
	if (pObject == NULL)
	{
		for (uint32 i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			if ((m_pVObjectArray[i]->GetType()).IsDerived(Rtti))
			{

				pObject = m_pVObjectArray[i];
				break;
			}
		}
	}
	for (uint32 i = 0; i < m_pVObjectArray.GetNum(); i++)
	{
		VSObject * p = m_pVObjectArray[i];
		VSMAC_ASSERT(p != NULL);
		if (p)
		{
			p->ClearFlag(VSObject::OF_REACH);
			p->SetFlag(VSObject::OF_UNREACH);
		}
	}
	if (pObject)
	{
		VSStream GCCollectStream;
		GCCollectStream.SetStreamFlag(VSStream::AT_LOAD_OBJECT_COLLECT_GC);
		GCCollectStream.ArchiveAll(pObject);


		VSArray<VSObject *> CanGCObject;
		for (uint32 i = 0; i < m_pVObjectArray.GetNum();)
		{
			VSObject * p = m_pVObjectArray[i];
			if (p->IsHasFlag(VSObject::OF_UNREACH))
			{
				CanGCObject.AddElement(p);
				m_pVObjectArray.Erase(i);
			}
			else
			{
				i++;
			}
		}

		VSResourceManager::AddCanGCObject(CanGCObject);

	}
	else
	{
		GetObjectFailed();
	}
	if (pObject)
	{
		m_uiStreamFlag = AT_POSTLOAD;
		ArchiveAll(pObject);
	}

	return pObject;
}

void VSStream::GetObjectFailed()
{
	if (m_pVObjectArray.GetNum())
	{
		VSResourceManager::AddCanGCObject(m_pVObjectArray);
		m_pVObjectArray.Clear();
	}
}

bool VSStream::GetAllResourceObject(VSArray<VSObject *> &ObjectArray)
{
	ObjectArray.Clear();
	for (uint32 i = 0; i < m_pVObjectArray.GetNum(); i++)
	{
		if (VSResourceControl::IsResourceType(m_pVObjectArray[i]->GetType()))
		{

			ObjectArray.AddElement(m_pVObjectArray[i]);
		}
	}
	if (ObjectArray.GetNum() > 0)
	{
		for (uint32 i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			VSObject * p = m_pVObjectArray[i];
			VSMAC_ASSERT(p != NULL);
			if (p)
			{
				p->ClearFlag(VSObject::OF_REACH);
				p->SetFlag(VSObject::OF_UNREACH);
			}
		}

		VSStream GCCollectStream;
		GCCollectStream.SetStreamFlag(VSStream::AT_LOAD_OBJECT_COLLECT_GC);
		for (uint32 i = 0; i < ObjectArray.GetNum(); i++)
		{
			GCCollectStream.ArchiveAll(ObjectArray[i]);
		}

		VSArray<VSObject *> CanGCObject;
		for (uint32 i = 0; i < m_pVObjectArray.GetNum();)
		{
			VSObject * p = m_pVObjectArray[i];
			if (p->IsHasFlag(VSObject::OF_UNREACH))
			{
				CanGCObject.AddElement(p);
				m_pVObjectArray.Erase(i);
			}
			else
			{
				i++;
			}
		}

		VSResourceManager::AddCanGCObject(CanGCObject);
		m_uiStreamFlag = AT_POSTLOAD;
		for (uint32 i = 0; i < ObjectArray.GetNum(); i++)
		{
			ArchiveAll(ObjectArray[i]);
		}
		return 1;
	}
	else
	{
		GetObjectFailed();
		return 0;
	}
}

bool VSStream::GetObjectArrayByRtti(const VSRtti &Rtti, VSArray<VSObject *> &ObjectArray, bool IsDerivedFrom)
{
	ObjectArray.Clear();
	for(uint32 i = 0 ; i < m_pVObjectArray.GetNum() ; i++)
	{
		if ((m_pVObjectArray[i]->GetType()).IsSameType(Rtti) || ((m_pVObjectArray[i]->GetType()).IsDerived(Rtti) && IsDerivedFrom))
		{

			ObjectArray.AddElement(m_pVObjectArray[i]);
		}
	}
	if (ObjectArray.GetNum() > 0)
	{
		for (uint32 i = 0; i < m_pVObjectArray.GetNum(); i++)
		{
			VSObject * p = m_pVObjectArray[i];
			VSMAC_ASSERT(p != NULL);
			if (p)
			{
				p->ClearFlag(VSObject::OF_REACH);
				p->SetFlag(VSObject::OF_UNREACH);
			}
		}

		VSStream GCCollectStream;
		GCCollectStream.SetStreamFlag(VSStream::AT_LOAD_OBJECT_COLLECT_GC);
		for (uint32 i = 0; i < ObjectArray.GetNum(); i++)
		{
			GCCollectStream.ArchiveAll(ObjectArray[i]);
		}
		
		VSArray<VSObject *> CanGCObject;
		for (uint32 i = 0; i < m_pVObjectArray.GetNum();)
		{
			VSObject * p = m_pVObjectArray[i];
			if (p->IsHasFlag(VSObject::OF_UNREACH))
			{
				CanGCObject.AddElement(p);
				m_pVObjectArray.Erase(i);
			}
			else
			{
				i++;
			}
		}

		VSResourceManager::AddCanGCObject(CanGCObject);

		m_uiStreamFlag = AT_POSTLOAD;
		for (uint32 i = 0; i < ObjectArray.GetNum(); i++)
		{
			ArchiveAll(ObjectArray[i]);
		}

		return 1;
	}
	else
	{
		GetObjectFailed();
		return 0;
	}
}

/*******************************************************************************


									new version

*******************************************************************************/
uint32 VSStream::SizeOfResource(VSResourceProxyBasePtr& Resource)
{
	uint32 uiSize = 0;
	bool IsNone = true;
	if (Resource && !VSResourceControl::IsDefaultResource(Resource))
	{
		IsNone = false;
	}
	uiSize += sizeof(bool);
	if (!IsNone)
	{

		const VSString & FileName = Resource->GetResourceName().GetString();
		uiSize += GetStrDistUse(FileName);
	}

	return uiSize;
}

bool VSStream::WriteResource(VSResourceProxyBasePtr& Resource)
{
	bool IsNone = true;
	if (Resource && !VSResourceControl::IsDefaultResource(Resource))
	{
		IsNone = false;	
	}
	Write(&IsNone,sizeof(bool));
	if (!IsNone)
	{
		
		const VSString & FileName = Resource->GetResourceName().GetString();
		WriteString(FileName);
	}
	
	return true;
}

bool VSStream::ReadResource(VSResourceProxyBasePtr& Resource)
{
	bool IsNone = true;
	Read(&IsNone,sizeof(bool));
	if (IsNone == false)
	{
		VSString Name;
		ReadString(Name);
		Resource = VSResourceManager::LoadResource(Name.GetBuffer(), HasAnyFlag(ASYN_LOAD_FLAG));
		if (!Resource)
		{
			VSMAC_ASSERT(0);
			return false;
		}
	}
	
	return true;
}

bool VSStream::ArchiveAll(VSObject * pObject)
{
	if (!pObject)
	{
		return false;
	}
	
	if (m_uiStreamFlag == AT_REGISTER)
	{
		if(RegisterObject(pObject))
		{
			VSRtti &Rtti = pObject->GetType();
			for (uint32 j = 0 ; j < Rtti.GetPropertyNum() ; j++)
			{
				VSProperty * pProperty = Rtti.GetProperty(j);
				if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
				{
					pProperty->Archive(*this,pObject);
				}
			}
		}
	}
	else if (m_uiStreamFlag == AT_POSTLOAD)
	{
		if (RegisterPostLoadObject(pObject))
		{

			VSRtti &Rtti = pObject->GetType();
			for (uint32 j = 0 ; j < Rtti.GetPropertyNum() ; j++)
			{
				VSProperty * pProperty = Rtti.GetProperty(j);
				if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
				{
					pProperty->Archive(*this,pObject);
				}
			}
			pObject->PostLoad(this);
		}
	}
	else if (m_uiStreamFlag == AT_OBJECT_COLLECT_GC)
	{
		if (RegisterReachableObject(pObject))
		{
			VSRtti &Rtti = pObject->GetType();
			for (uint32 j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				VSProperty * pProperty = Rtti.GetProperty(j);
				if (!(pProperty->GetFlag() & VSProperty::F_NO_USE_GC))
				{
					pProperty->Archive(*this, pObject);
				}
			}		
		}
	}
	else if (m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC)
	{
		if (RegisterReachableObject(pObject))
		{
			VSRtti &Rtti = pObject->GetType();
			for (uint32 j = 0; j < Rtti.GetPropertyNum(); j++)
			{
				VSProperty * pProperty = Rtti.GetProperty(j);			
				pProperty->Archive(*this, pObject);
			}
		}
	}
	else if (m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
	{
		VSRtti &Rtti = pObject->GetType();
		for (uint32 j = 0; j < Rtti.GetPropertyNum(); j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);
			if (!(pProperty->GetFlag() & VSProperty::F_NO_USE_GC))
			{
				pProperty->Archive(*this, pObject);
			}
		}
	}
	return true;
}

bool VSStream::LoadFromBuffer(unsigned char * pBuffer,uint32 uiSize)
{

	if (!pBuffer || !uiSize)
	{
		return false;
	}
	m_pVObjectArray.Clear();
	m_pmLoadMap.Clear();
	m_pcCurBufPtr	= NULL;
	m_uiBufferSize	= 0;

	//�����ص��ڴ���
	m_uiBufferSize = uiSize;
	m_pcBuffer = pBuffer;

	m_pcCurBufPtr = m_pcBuffer;

	VSArray<ObjectTableType> ObjectTable;
	//���ذ汾��
	Read(&m_uiVersion,sizeof(unsigned int));
	//�����������
	uint32 iObjectNum = 0;
	Read(&iObjectNum,sizeof(unsigned int));

	ObjectTable.SetBufferNum(iObjectNum);
	//Object Table
	for(uint32 i = 0 ;i < iObjectNum ; i++)
	{
		VSObject * pObject = 0;
		//��ȡָ��
		if(!Read(&ObjectTable[i].m_uiGUID,sizeof(unsigned int)))
		{
			return 0;
		}
		//��ȡRTTI
		if(!ReadString(ObjectTable[i].m_RttiName))
		{			
			return 0;
		}
		VSConfig::GetClassReplace(ObjectTable[i].m_RttiName);
		if(!Read(&ObjectTable[i].m_uiObjectPropertySize,sizeof(unsigned int)))
		{
			return 0;
		}

		if(!Read(&ObjectTable[i].m_uiObjectPropertyNum,sizeof(unsigned int)))
		{
			return 0;
		}


	}

	//create object
	for(uint32 i = 0 ;i < iObjectNum ; i++)
	{
		VSObject * pObject = NULL;
		if (m_bLoadUseGC)
		{
			pObject = VSObject::GetInstance(ObjectTable[i].m_RttiName);
		}
		else
		{
			pObject = VSObject::GetNoGCInstance(ObjectTable[i].m_RttiName);
		}
		//�����ն���
		
		if(!pObject)
		{		
			continue;
		}
		//��������ӳ���
		m_pmLoadMap.AddElement(ObjectTable[i].m_uiGUID,pObject);


		RegisterObject(pObject);
	}

	//load object property table
	for (uint32 i = 0; i < iObjectNum; i++)
	{
		ObjectTable[i].m_ObjectPropertyTable.SetBufferNum(ObjectTable[i].m_uiObjectPropertyNum);
		VSMap<unsigned int, uint32 > *pPropertyReplace = NULL;
		VSConfig::GetPropertyReplace(ObjectTable[i].m_RttiName, pPropertyReplace);
		for (uint32 j = 0 ; j < ObjectTable[i].m_uiObjectPropertyNum ; j++)
		{
			
			Read(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID,sizeof(unsigned int));	
			if (pPropertyReplace)
			{
				VSConfig::GetPropertyReplace(pPropertyReplace, ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID);
			}
			Read(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiOffset,sizeof(unsigned int));

		}
		m_pcCurBufPtr += ObjectTable[i].m_uiObjectPropertySize;
	}

	m_uiStreamFlag = AT_LOAD;
	//load object property
	
	for (uint32 i = 0; i < m_pVObjectArray.GetNum(); i++)
	{
		VSRtti &Rtti = m_pVObjectArray[i]->GetType();
		uint32 uiTableID = i;
		for (uint32 j = 0; j < Rtti.GetPropertyNum(); j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);
			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				for (uint32 k = 0; k < ObjectTable[uiTableID].m_ObjectPropertyTable.GetNum(); k++)
				{
					if (pProperty->GetName().GetNameCode() == ObjectTable[uiTableID].m_ObjectPropertyTable[k].m_uiNameID)
					{
						m_pcCurBufPtr = m_pcBuffer + ObjectTable[uiTableID].m_ObjectPropertyTable[k].m_uiOffset;
						pProperty->Archive(*this, m_pVObjectArray[i]);
						break;
					}
				}
			}
		}
	}
	
	//��������
	m_uiStreamFlag = AT_LINK;
	//����Ӻ���ǰ��������Ϊregister�Ĺ����ǵݹ�����ע�ᣬ���ԴӺ���ǰlink��֤�ӽڵ�����ɣ�Ȼ���׽ڵ���ɡ� 
	for(int32 i = m_pVObjectArray.GetNum() - 1; i >= 0 ; i--)
	{

		VSRtti &Rtti = m_pVObjectArray[i]->GetType();
		for (uint32 j = 0 ; j < Rtti.GetPropertyNum() ; j++)
		{	
			VSProperty * pProperty = Rtti.GetProperty(j);
			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				pProperty->Archive(*this,m_pVObjectArray[i]);
			}
		}
	}
	m_pcBuffer = NULL;
	return 1;
}

bool VSStream::Load(const char * const pcFileName)
{
	File *pFile = MEM_NEW File();
	if (!pFile)
	{
		return 0;
	}

	//���ļ�
	if(!pFile->Open(pcFileName,File::OM_RB))
	{
		SAFE_DELETE(pFile);
		return 0;
	}

	//�����ص��ڴ���
	uint32 uiBufferSize = pFile->GetFileSize();
	unsigned char * pBuffer = MEM_NEW unsigned char[uiBufferSize];
	if(!pBuffer)
	{
		SAFE_DELETE(pFile);
		return 0;
	}
	if(!pFile->Read(pBuffer,uiBufferSize,1))
	{
		SAFE_DELETE(pFile);
		return 0;
	}
	SAFE_DELETE(pFile);

	if(!LoadFromBuffer(pBuffer,uiBufferSize))
	{
		SAFE_DELETEA(pBuffer);
		return false;
	}
	SAFE_DELETEA(pBuffer);

	return 1;
}

bool VSStream::Save(const char * const pcFileName)
{
	uint32 iObjectNum = m_pVObjectArray.GetNum();
	m_uiBufferSize = 0;

	//�汾��
	m_uiBufferSize += sizeof(uint32 );
	//��������ռ�
	m_uiBufferSize += sizeof(unsigned int);

	VSArray<ObjectTableType> ObjectTable;
	ObjectTable.SetBufferNum(iObjectNum);
	m_pmSaveMap.Clear();
	// obejct table
	for(uint32 i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		//guid
		m_uiBufferSize += sizeof(unsigned int);
		
		ObjectTable[i].m_uiGUID = i + 1;

		m_pmSaveMap.AddElement(m_pVObjectArray[i],ObjectTable[i].m_uiGUID);

		//rtti name
		m_uiBufferSize += GetStrDistUse(m_pVObjectArray[i]->GetType().GetName());

		ObjectTable[i].m_RttiName = m_pVObjectArray[i]->GetType().GetName();

		// object property size
		m_uiBufferSize += sizeof(unsigned int);

		// object property num
		m_uiBufferSize += sizeof(unsigned int);
	}
	
	uint32 uiObjectContentAddr = m_uiBufferSize;

	m_uiStreamFlag = AT_SIZE;
	//object property size 
	for(uint32 i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		ObjectTable[i].m_uiOffset = m_uiBufferSize;

		VSRtti &Rtti = m_pVObjectArray[i]->GetType();
		ObjectTable[i].m_ObjectPropertyTable.SetBufferNum(Rtti.GetPropertyNum());

		ObjectTable[i].m_uiObjectPropertyNum = 0;

		for (uint32 j = 0 ; j < Rtti.GetPropertyNum() ; j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);
			
			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				//name
				m_uiBufferSize += sizeof(unsigned int);
				ObjectTable[i].m_ObjectPropertyTable[j].m_PropertyName = pProperty->GetName().GetString();
				ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID = pProperty->GetName().GetNameCode();
				//offset
				m_uiBufferSize += sizeof(unsigned int);

				ObjectTable[i].m_uiObjectPropertyNum++;
			}
		}

		ObjectTable[i].m_uiObjectPropertyTableSize = m_uiBufferSize - ObjectTable[i].m_uiOffset;

		for (uint32 j = 0 ; j < Rtti.GetPropertyNum() ; j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);

			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				ObjectTable[i].m_ObjectPropertyTable[j].m_uiOffset = m_uiBufferSize;

				m_uiArchivePropertySize = 0;

				pProperty->Archive(*this,m_pVObjectArray[i]);

				ObjectTable[i].m_ObjectPropertyTable[j].m_uiSize = m_uiArchivePropertySize;

				m_uiBufferSize += m_uiArchivePropertySize;
			}
		}

		ObjectTable[i].m_uiObjectPropertySize = m_uiBufferSize - ObjectTable[i].m_uiObjectPropertyTableSize - ObjectTable[i].m_uiOffset;
	}

	for(uint32 i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		m_pVObjectArray[i]->BeforeSave(this);
	}
	//����ռ�
	SAFE_DELETEA(m_pcBuffer);
	m_pcBuffer = MEM_NEW unsigned char[m_uiBufferSize];	
	if(!m_pcBuffer)
	{
		return 0;
	}
	m_pcCurBufPtr = m_pcBuffer;

	m_uiVersion = ms_uiCurVersion;
	//�洢�汾��
	Write(&m_uiVersion,sizeof(unsigned int));

	//�洢�������
	Write(&iObjectNum,sizeof(unsigned int));

	// OBJECT TABLE
	for(uint32 i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		//addr
		if(!Write(&ObjectTable[i].m_uiGUID,sizeof(unsigned int)))
		{
			SAFE_DELETEA(m_pcBuffer);
			return 0;
		}

		//rtti name
		if(!WriteString(ObjectTable[i].m_RttiName))
		{
			SAFE_DELETEA(m_pcBuffer);
			return 0;
		}

		Write(&ObjectTable[i].m_uiObjectPropertySize,sizeof(unsigned int));

		Write(&ObjectTable[i].m_uiObjectPropertyNum,sizeof(unsigned int));
	}

	//OBJECT PROPERTY 
	m_uiStreamFlag = AT_SAVE;
	for(uint32 i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		VSRtti &Rtti = m_pVObjectArray[i]->GetType();
		for (uint32 j = 0 ; j < Rtti.GetPropertyNum() ; j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);

			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{

				Write(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID,sizeof(unsigned int));
				Write(&ObjectTable[i].m_ObjectPropertyTable[j].m_uiOffset,sizeof(unsigned int));
			}		
		}

		for (uint32 j = 0 ; j < Rtti.GetPropertyNum() ; j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);
			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				pProperty->Archive(*this,m_pVObjectArray[i]);
			}
		}
	}

	for(uint32 i = 0 ; i < m_pVObjectArray.GetNum();i++)
	{
		m_pVObjectArray[i]->PostSave(this);
	}

	File *pFile = MEM_NEW File();
	if (!pFile)
	{
		SAFE_DELETEA(m_pcBuffer);
		return 0;
	}
	//���ļ�
	if(!pFile->Open(pcFileName,File::OM_WB))
	{
		SAFE_DELETE(pFile);
		SAFE_DELETEA(m_pcBuffer);
		return 0;
	}

	if(!pFile->Write(m_pcBuffer,m_uiBufferSize,1))
	{
		SAFE_DELETE(pFile);
		SAFE_DELETEA(m_pcBuffer);
		return 0;
	}
	SAFE_DELETE(pFile);
	SAFE_DELETEA(m_pcBuffer);
	return 1;
}