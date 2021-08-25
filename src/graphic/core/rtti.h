#pragma once


#include "graphic/core/rttimarc.h"
#include "datastruct/VSString.h"
#include "graphic/core/graphic.h"
#include "core/memorymanager.h"
#include "datastruct/VSArray.h"
#include "graphic/core/name.h"


namespace zq{


class VSObject;
typedef VSObject *(*CreateObjectFun)();
class VSProperty;
class VSFunction;
class GRAPHIC_API VSRtti : public MemObject
{
	//PRIORITY
	DECLARE_PRIORITY
	DECLARE_INITIAL_ONLY
public:
	VSRtti(const char * pcRttiName,VSRtti *pBase,CreateObjectFun COF);
	~VSRtti();

    inline const VSString& GetName()const
    {
        return m_cRttiName;
    }

    inline bool IsSameType(const VSRtti &Type) const
    {
        return  (&Type == this);
    }

    inline bool IsDerived(const VSRtti &Type) const
    {
        const VSRtti * pTemp = this;
        while (!pTemp->IsSameType(Type))
        {
            if (pTemp->m_pBase)
            {
                pTemp = pTemp->m_pBase;

            }
            else
            {
                return 0;
            }
        }

        return 1;
    }

	inline VSRtti* GetBase()const
	{
		return m_pBase;
	}

	VSProperty * GetProperty(uint32 uiIndex)const;
	VSProperty *GetProperty(const VSString & PropertyName)const;
	uint32 GetPropertyNum()const;
	void AddProperty(VSProperty * pProperty);
	void AddProperty(VSRtti & Rtti);
	friend class VSObject;
	typedef VSObject *(*CreateObjectFun)();
	void ClearProperty();

	VSFunction * GetFunction(uint32 uiIndex)const;
	uint32 GetFunctionNum()const;
	void AddFunction(VSFunction * pProperty);
	void AddFunction(VSRtti & Rtti);
	void ClearFunction();
	static const VSRtti * GetRttiByName(const VSUsedName & RttiName);
private:
	VSString	m_cRttiName;
	VSRtti*	m_pBase;
	VSArray<VSProperty *> m_PropertyArray;
	VSArray<VSFunction *> m_FunctionArray;
	CreateObjectFun m_CreateFun;
	static VSMap<VSUsedName, VSRtti *> ms_RttiMap;

	static VSArray<VSRtti*> &GetRttiArray()
	{
		static VSArray<VSRtti *> ms_RttiArray;
		return ms_RttiArray;
	}

	static bool InitialDefaultState();
	static bool TerminalDefaultState();
};


}
