#pragma once
#include "graphic/pass/pass.h"

namespace zq
{
	class GRAPHIC_API VSIndirectRenderPass : public VSPass
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL

	public:
		VSIndirectRenderPass();
		virtual ~VSIndirectRenderPass();
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
			return PT_INDIRECT;
		}
	protected:
		VSArray<VSLight *> m_Light;
	public:
		virtual bool Draw(VSRenderer * pRenderer);


	};
	DECLARE_Ptr(VSIndirectRenderPass);
	VSTYPE_MARCO(VSIndirectRenderPass);
}
