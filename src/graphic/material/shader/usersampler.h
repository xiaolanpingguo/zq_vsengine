#pragma once
#include "core/memorymanager.h"
#include "math/maths.h"
#include "graphic/core/object.h"
#include "graphic/render/texture/texallstate.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSUserSampler : public VSObject
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		
		VSUserSampler(const VSUsedName & Name,uint32 uiTexType,uint32 uiRegisterIndex,uint32 uiRegisterNum);
		virtual ~VSUserSampler();

		inline uint32 GetTexType()const
		{
			return m_uiTexType;
		}
		
		inline VSTexAllState * GetTex(uint32 uiIndex)const
		{
			return m_pTextureArray[uiIndex];
		}
		inline const VSUsedName & GetName()const
		{
			return m_Name;
		}
		inline void SetTex(VSTexAllState * pTex,uint32 uiIndex = 0)
		{
			if (uiIndex >= m_uiRegisterNum)
			{
				return ;
			}
			m_pTextureArray[uiIndex] = pTex;
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
		VSUserSampler();
	private:

		uint32 m_uiTexType;
		VSArray<VSTexAllStatePtr> m_pTextureArray;
		VSUsedName		m_Name;
		uint32 m_uiRegisterIndex;
		uint32 m_uiRegisterNum;
	};
	VSTYPE_MARCO(VSUserSampler);
	DECLARE_Ptr(VSUserSampler);
}
