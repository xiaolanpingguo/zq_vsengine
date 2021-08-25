#pragma once


#include "graphic/core/object.h"
#include "graphic/render/membind.h"
#include "graphic/render/bindlockinterface.h"
#include "graphic/render/buffer/databuffer.h"


namespace zq{


class VSStream;
class GRAPHIC_API VSIndexBuffer : public VSMemBind, public VSBindLockInterface
{
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	virtual ~VSIndexBuffer();
	VSIndexBuffer();
	VSIndexBuffer(uint32 uiNum,uint32 uiDT = VSDataBuffer::DT_USHORT);
	bool SetData(VSDataBuffer * pData);
public:
		

	virtual void *Lock();
	virtual void UnLock();

	virtual void ClearInfo();
	inline VSDataBuffer *GetIndexData()const {return m_pData;}

	uint32 GetNum()const;
	virtual uint32 GetByteSize()const;
	inline void * GetLockDataPtr()const
	{
		return m_pLockData;
	}
	inline uint32 GetDataType()const
	{
		return m_uiDT;
	}
protected:
	virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
	VSDataBufferPtr m_pData;
	uint32 m_uiNum;
	uint32 m_uiDT;
	void * m_pLockData;
};


DECLARE_Ptr(VSIndexBuffer);
VSTYPE_MARCO(VSIndexBuffer);


}
