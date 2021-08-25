#pragma once
#include "graphic/render/buffer/bufferresource.h"
#include "graphic/render/outputresource/unorderaccess.h"
namespace zq
{
	class GRAPHIC_API VSBufferUnorderAccess : public VSUnorderAccess
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		friend class VSResourceManager;
		friend class VSBufferResource;
		virtual ~VSBufferUnorderAccess();
		virtual void *Lock();
		virtual void UnLock();
		virtual uint32 GetOutputType()const { return OT_BUFFER_UNORDER_ACCESS; }
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
		VSBufferUnorderAccess(uint32 uiNum, uint32 uiDT, uint32 uiStructStride = 0, bool CPURead = false);
		VSBufferUnorderAccess(VSBufferResource * pCreateBy, bool CPURead = false);
		VSBufferUnorderAccess();
		
		virtual bool OnLoadResource(VSResourceIdentifier *&pID);
		
		uint32 m_uiNum;
		uint32 m_uiDT;
		uint32 m_uiStructStride;
	};
	DECLARE_Ptr(VSBufferUnorderAccess);
	VSTYPE_MARCO(VSBufferUnorderAccess);
}

