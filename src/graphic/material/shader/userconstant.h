#pragma once


#include "core/memorymanager.h"
#include "math/maths.h"
#include "graphic/core/object.h"
#include "graphic/core/name.h"


namespace zq{


class VSStream;
class GRAPHIC_API VSUserConstant : public VSObject
{
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	
	enum //Value Type
	{			
		VT_FLOAT,
		VT_BOOL,
		VT_INT,	
		VT_STRUCT,
		VT_MAX
	};

public:
	VSUserConstant(const VSUsedName & Name,uint32 uiSize, uint32 uiRegisterIndex,uint32 uiRegisterNum,uint32 uiValueType = VT_FLOAT);
	VSUserConstant(const VSUsedName & Name, const VSUsedName & NameInShader,uint32 uiSize, uint32 uiRegisterIndex, uint32 uiRegisterNum, uint32 uiValueType = VT_FLOAT);
	virtual ~VSUserConstant();

    inline void* GetData()const
    {
        return m_pData;
    }

    inline uint32 GetValueType()const
    {
        return m_uiValueType;
    }

    inline uint32 GetSize()const
    {
        return m_uiSize;
    }

    inline uint32 GetRegisterNum()const
    {
        return m_uiRegisterNum;
    }

    inline uint32 GetRegisterIndex()const
    {
        return m_uiRegisterIndex;
    }

	inline const VSUsedName & GetShowName()const
	{
		return m_Name;
	}

	inline const VSUsedName & GetNameInShader()const
	{
		return m_NameInShader;
	}

	virtual bool PostLoad(VSStream* pStream);
	virtual bool PostClone(VSObject * pObjectSrc);

    bool SetData(void *pElementData);

	uint32 m_uiSize;
	uint32 m_uiValueType;	
	uint32 m_uiRegisterIndex;
	uint32 m_uiRegisterNum;
protected:
	friend class VSRenderer;
	VSUserConstant();

private:
	
	unsigned char * m_pData;
	VSUsedName m_Name;
	VSUsedName m_NameInShader;
};

VSTYPE_MARCO(VSUserConstant);
DECLARE_Ptr(VSUserConstant);

}
