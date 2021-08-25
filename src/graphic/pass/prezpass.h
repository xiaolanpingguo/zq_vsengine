#pragma once
#include "graphic/pass/pass.h"
namespace zq
{

	class GRAPHIC_API VSPreZPass : public VSPass
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSPreZPass();
		virtual ~VSPreZPass();
		

	protected:
		static VSPointer<VSPreZPass>	ms_Default;
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
	public:
		virtual bool Draw(VSRenderer * pRenderer);
		
		static VSPreZPass * GetDefault()
		{
			return ms_Default;
		}
		virtual uint32 GetPassType()
		{
			return PT_PREZ;
		}

	};
	DECLARE_Ptr(VSPreZPass);
	VSTYPE_MARCO(VSPreZPass);
}
