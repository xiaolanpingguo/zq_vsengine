#pragma once
#include "graphic/pass/pass.h"
namespace zq
{

	class GRAPHIC_API VSPostEffectPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSPostEffectPass();
		VSPostEffectPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		void SetCustomConstant(MaterialShaderPara &MSPara, VSVShader * pVShader, VSPShader * pPShader);
	public:
		virtual bool Draw(VSRenderer * pRenderer);

		virtual uint32 GetPassType()	
		{
			return PT_POSTEFFECT;
		}
		uint32 m_uiRTWidth;
		uint32 m_uiRTHeight;
		VSTexAllState * m_PColorBuffer;
		VSRenderState m_RenderState;
	};
	DECLARE_Ptr(VSPostEffectPass);
	VSTYPE_MARCO(VSPostEffectPass);
}