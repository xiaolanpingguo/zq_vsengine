#pragma once
#include "graphic/pass/pass.h"
namespace zq
{

	class GRAPHIC_API VSLightFunPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSLightFunPass();
		VSLightFunPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();

	public:
		virtual bool Draw(VSRenderer * pRenderer);

		virtual uint32 GetPassType()
		{
			return PT_LIGHT_FUNCTION;
		}
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, VSLight * pLight,uint32 uiPassId)
		{
			VSPass::SetDrawContent(pMaterialInstance, pGeometry, pCamera, uiPassId);
			MSPara.pShadowLight = pLight;

		}
		VSRenderState m_RenderState;
	};
	DECLARE_Ptr(VSLightFunPass);
	VSTYPE_MARCO(VSLightFunPass);
}
