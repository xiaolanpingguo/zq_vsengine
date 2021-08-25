#pragma once
#include "graphic/core/object.h"
#include "graphic/render/membind.h"
#include "graphic/render/bindlockinterface.h"
#include "graphic/render/buffer/databuffer.h"
#include "graphic/render/slot.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSBufferResource : public VSMemBind, public VSBindLockInterface, public VSSlot
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		virtual ~VSBufferResource();
		VSBufferResource();
		VSBufferResource(uint32 uiNum, uint32 uiDT,uint32 uiStructStride = 0);
		bool SetData(VSDataBuffer * pData);
	public:


		virtual void *Lock();
		virtual void UnLock();

		virtual void ClearInfo();
		inline VSDataBuffer *GetDataBuffer()const { return m_pData; }

		inline uint32 GetNum()const
		{
			return m_uiNum;
		}
		virtual uint32 GetByteSize()const;
		inline uint32 GetDataType()const
		{
			return m_uiDT;
		}
		inline uint32 GetStructStride()const
		{
			return m_uiStructStride;
		}
	protected:
		friend class VSBufferUnorderAccess;
		bool SetOutput(class VSBufferUnorderAccess *pUnorderAccess);
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);
		VSDataBufferPtr m_pData;
		uint32 m_uiNum;
		uint32 m_uiDT;
		uint32 m_uiStructStride;
		class VSBufferUnorderAccess *m_pCurUnorderAccess;
	};
	DECLARE_Ptr(VSBufferResource);
	VSTYPE_MARCO(VSBufferResource);
}

