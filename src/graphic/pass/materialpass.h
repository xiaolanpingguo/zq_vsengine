#pragma once
#include "graphic/pass/pass.h"

namespace zq
{
	class VSMaterialInstance;
	class GRAPHIC_API VSMaterialPass : public VSPass
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		VSMaterialPass();
		virtual ~VSMaterialPass();
	public:

		inline void AddLight(VSLight * pLight)
		{
			if (pLight)
			{
				for (uint32 i = 0 ; i < m_Light.GetNum() ;i++)
				{
					if (m_Light[i] == pLight)
					{
						return ;
					}
				}
				m_Light.AddElement(pLight);
			}
		}
		
		virtual uint32 GetPassType()
		{
			return PT_MATERIAL;
		}
	protected:
		VSArray<VSLight *> m_Light;
		VSRenderState m_RenderSceondPassUsed;
		
	public:
		

	public:
		virtual bool Draw(VSRenderer * pRenderer);


	};
	DECLARE_Ptr(VSMaterialPass);
	VSTYPE_MARCO(VSMaterialPass);
}
