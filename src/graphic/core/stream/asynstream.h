#pragma once
#include "graphic/core/stream/stream.h"
namespace zq
{
	class GRAPHIC_API VSAsynStream : public VSStream
	{
	private:
		VSArray<ObjectTableType> m_ObjectTable;		
	public:
		VSAsynStream();
		virtual bool LoadFromBuffer(unsigned char * pBuffer, uint32 uiSize);
		void CreateAndRegisterObject();
		void LoadAndLinkOjbect();
		virtual bool ReadResource(VSResourceProxyBasePtr& Resource);
		virtual const VSObject *GetObjectByRtti(const VSRtti &Rtti);
		template <typename T> friend class VSTResourceJob;
		template<typename CacheType> friend class VSCacheResourceJobBase;
	protected:
		void PostLoadObject(VSObject * pPostLoadObject);
	};
}