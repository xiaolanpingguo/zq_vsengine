#pragma once
#include "graphic/render/bind.h"
namespace zq
{
	class GRAPHIC_API VSInheritBind : public VSBind
	{

		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		enum	//Lock Flag
		{
			LF_NOOVERWRITE,
			LF_DISCARD,
			LF_READONLY,
			LF_MAX
		};
	public:
		VSInheritBind();
		virtual ~VSInheritBind() = 0;
		bool IsStatic()const { return m_bIsStatic; }
		virtual void SetLockFlag(uint32 uiLockFlag)
		{
			if (uiLockFlag > LF_MAX)
			{
				uiLockFlag = LF_NOOVERWRITE;
			}
			m_uiLockFlag = uiLockFlag;
		}
		inline uint32 GetLockFlag()const
		{
			return m_uiLockFlag;
		}		
	protected:
		bool m_bIsStatic;
		uint32 m_uiLockFlag;
	};
	DECLARE_Ptr(VSInheritBind);
	VSTYPE_MARCO(VSInheritBind);

}
