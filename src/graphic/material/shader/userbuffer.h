#pragma once


#include "core/memorymanager.h"
#include "math/maths.h"
#include "graphic/core/object.h"
#include "graphic/render/buffer/bufferresource.h"


namespace zq{


class VSStream;
class GRAPHIC_API VSUserBuffer : public VSObject
{
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	VSUserBuffer(const VSUsedName & Name,uint32 uiDataType, uint32 uiRegisterIndex, uint32 uiRegisterNum);
	virtual ~VSUserBuffer();

	inline uint32 GetDataType()const
	{
		return m_uiDT;
	}

	inline const VSUsedName & GetName()const
	{
		return m_Name;
	}
	inline void SetBufferResource(VSBufferResource * pBufferResource, uint32 uiIndex = 0)
	{
		if (uiIndex >= m_uiRegisterNum)
		{
			return;
		}
		m_pBufferResourceArray[uiIndex] = pBufferResource;
	}
	inline VSBufferResource * GetBufferResource(uint32 uiIndex)const
	{
		return m_pBufferResourceArray[uiIndex];
	}
	inline uint32 GetRegisterIndex()const
	{
		return m_uiRegisterIndex;
	}
	inline uint32 GetRegisterNum()const
	{
		return m_uiRegisterNum;
	}
	virtual bool PostLoad(VSStream* pStream);
	virtual bool PostClone(VSObject * pObjectSrc);
protected:
	friend class VSRenderer;
	VSUserBuffer();
private:

	uint32 m_uiDT;
	VSArray <VSBufferResourcePtr> m_pBufferResourceArray;
	VSUsedName		m_Name;
	uint32 m_uiRegisterIndex;
	uint32 m_uiRegisterNum;
};


VSTYPE_MARCO(VSUserBuffer);
DECLARE_Ptr(VSUserBuffer);


}

