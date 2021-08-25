#pragma once


#include "graphic/render/inheritbind.h"


namespace zq{


class GRAPHIC_API VSMemBind : public VSInheritBind
{

	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL_NO_CLASS_FACTORY;
public:
		
	enum	//Mem Type
	{
		MT_BOTH,
		MT_RAM,
		MT_VRAM,
		MT_MAX
	};

	VSMemBind(uint32 uiMemType = MT_VRAM);
	virtual ~VSMemBind() = 0;

	virtual VSResourceIdentifier* GetIdentifier();
	//如果此资源需要lock，并且是多线程渲染，为了有效利用多线效率，lock函数如果在主线程使用请设置bmulthreadUse为true
	virtual void SetStatic(bool bIsStatic, bool bMulThreadUse = false);
	void ExChange();
protected:
	virtual void ClearInfo() = 0;
		
	uint32 m_uiSwapChainNum;
	uint32 m_uiCurID;
		

	uint32 m_uiMemType;
public:
	virtual	bool LoadResource(VSRenderer * pRender);
	virtual uint32 GetByteSize()const = 0;

	inline void SetMemType(uint32 uiMemType)
	{
		if (uiMemType < MT_MAX)
		{
			m_uiMemType = uiMemType;
		}
	}
	inline uint32 GetMemType()const
	{
		return m_uiMemType;
	}
	inline uint32 GetSwapChainNum()const
	{
		return m_uiSwapChainNum;
	}
};
DECLARE_Ptr(VSMemBind);
VSTYPE_MARCO(VSMemBind);

}
