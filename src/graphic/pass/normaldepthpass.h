#pragma once
#include "graphic/pass/pass.h"
namespace zq
{

	class GRAPHIC_API VSNormalDepthPass : public VSPass
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSNormalDepthPass();
		VSNormalDepthPass();
	public:

		
	protected:
		
		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
	public:
		virtual bool Draw(VSRenderer * pRenderer);

		virtual uint32 GetPassType()
		{
			return PT_NORMALDEPTH;
		}

	};
	DECLARE_Ptr(VSNormalDepthPass);
	VSTYPE_MARCO(VSNormalDepthPass);
}
