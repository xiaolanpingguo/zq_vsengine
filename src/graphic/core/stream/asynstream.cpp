#include "graphic/core/stream/asynstream.h"
#include "graphic/core/config.h"
#include "graphic/core/object.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
VSAsynStream::VSAsynStream()
	:VSStream(ASYN_LOAD_FLAG)
{

}
bool VSAsynStream::LoadFromBuffer(unsigned char * pBuffer, uint32 uiSize)
{

	if (!pBuffer || !uiSize)
	{
		return false;
	}
	m_pVObjectArray.Clear();
	m_pmLoadMap.Clear();
	m_pcCurBufPtr = NULL;
	m_uiBufferSize = 0;

	//都加载到内存中
	m_uiBufferSize = uiSize;
	m_pcBuffer = pBuffer;

	m_pcCurBufPtr = m_pcBuffer;

	//加载版本号
	Read(&m_uiVersion, sizeof(unsigned int));
	//加载物体个数
	uint32 iObjectNum = 0;
	Read(&iObjectNum, sizeof(unsigned int));

	m_ObjectTable.SetBufferNum(iObjectNum);
	//Object Table
	for (uint32 i = 0; i < iObjectNum; i++)
	{
		VSObject * pObject = 0;
		//读取指针
		if (!Read(&m_ObjectTable[i].m_uiGUID, sizeof(unsigned int)))
		{
			return 0;
		}
		//读取RTTI
		if (!ReadString(m_ObjectTable[i].m_RttiName))
		{
			return 0;
		}
		VSConfig::GetClassReplace(m_ObjectTable[i].m_RttiName);
		if (!Read(&m_ObjectTable[i].m_uiObjectPropertySize, sizeof(unsigned int)))
		{
			return 0;
		}

		if (!Read(&m_ObjectTable[i].m_uiObjectPropertyNum, sizeof(unsigned int)))
		{
			return 0;
		}
	}
	return 1;
}
void VSAsynStream::CreateAndRegisterObject()
{
	for (uint32 i = 0; i < m_ObjectTable.GetNum(); i++)
	{
		VSObject * pObject = NULL;
		if (m_bLoadUseGC)
		{
			pObject = VSObject::GetInstance(m_ObjectTable[i].m_RttiName);
		}
		else
		{
			pObject = VSObject::GetNoGCInstance(m_ObjectTable[i].m_RttiName);
		}
		//创建空对象

		if (!pObject)
		{
			continue;
		}
		//创建加载映射表
		m_pmLoadMap.AddElement(m_ObjectTable[i].m_uiGUID, pObject);


		RegisterObject(pObject);
	}
}
void VSAsynStream::LoadAndLinkOjbect()
{
	//load object property table
	for (uint32 i = 0; i < m_ObjectTable.GetNum(); i++)
	{
		m_ObjectTable[i].m_ObjectPropertyTable.SetBufferNum(m_ObjectTable[i].m_uiObjectPropertyNum);
		VSMap<unsigned int, uint32 > *pPropertyReplace = NULL;
		VSConfig::GetPropertyReplace(m_ObjectTable[i].m_RttiName, pPropertyReplace);
		for (uint32 j = 0; j < m_ObjectTable[i].m_uiObjectPropertyNum; j++)
		{

			Read(&m_ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID, sizeof(unsigned int));
			if (pPropertyReplace)
			{
				VSConfig::GetPropertyReplace(pPropertyReplace, m_ObjectTable[i].m_ObjectPropertyTable[j].m_uiNameID);
			}
			Read(&m_ObjectTable[i].m_ObjectPropertyTable[j].m_uiOffset, sizeof(unsigned int));

		}
		m_pcCurBufPtr += m_ObjectTable[i].m_uiObjectPropertySize;
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
				for (uint32 k = 0; k < m_ObjectTable[uiTableID].m_ObjectPropertyTable.GetNum(); k++)
				{
					if (pProperty->GetName().GetNameCode() == m_ObjectTable[uiTableID].m_ObjectPropertyTable[k].m_uiNameID)
					{
						m_pcCurBufPtr = m_pcBuffer + m_ObjectTable[uiTableID].m_ObjectPropertyTable[k].m_uiOffset;
						pProperty->Archive(*this, m_pVObjectArray[i]);
						break;
					}
				}
			}
		}
	}

	//处理连接
	m_uiStreamFlag = AT_LINK;
	//必须从后往前便利，因为register的过程是递归的深度注册，所以从后往前link保证子节点先完成，然后父亲节点完成。 
	for (int32 i = m_pVObjectArray.GetNum() - 1; i >= 0; i--)
	{

		VSRtti &Rtti = m_pVObjectArray[i]->GetType();
		for (uint32 j = 0; j < Rtti.GetPropertyNum(); j++)
		{
			VSProperty * pProperty = Rtti.GetProperty(j);
			if (pProperty->GetFlag() & VSProperty::F_SAVE_LOAD)
			{
				pProperty->Archive(*this, m_pVObjectArray[i]);
			}
		}
	}
	m_pcBuffer = NULL;
}
bool VSAsynStream::ReadResource(VSResourceProxyBasePtr& Resource)
{
	bool IsNone = true;
	Read(&IsNone, sizeof(bool));
	if (IsNone == false)
	{
		VSString Name;
		ReadString(Name);
		Resource = VSResourceManager::LoadResource(Name.GetBuffer(), true);
		if (!Resource)
		{
			VSMAC_ASSERT(0);
			return false;
		}
	}

	return true;
}
const VSObject *VSAsynStream::GetObjectByRtti(const VSRtti &Rtti)
{
	VSObject * pObject = NULL;
	for (uint32 i = 0; i < m_pVObjectArray.GetNum(); i++)
	{
		if ((m_pVObjectArray[i]->GetType()).IsSameType(Rtti))
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
	return pObject;
}
void VSAsynStream::PostLoadObject(VSObject * pPostLoadObject)
{
	m_uiStreamFlag = AT_POSTLOAD;
	ArchiveAll(pPostLoadObject);
}