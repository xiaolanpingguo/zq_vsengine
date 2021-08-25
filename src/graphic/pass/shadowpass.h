#pragma once
#include "graphic/pass/pass.h"
namespace zq
{
	class VSPointLight;
	class GRAPHIC_API VSCubeShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSCubeShadowPass();
		VSCubeShadowPass();
	public:


	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
	public:
		virtual bool Draw(VSRenderer * pRenderer);
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, VSLight * pPointLight, uint32 uiPassId)
		{
			MSPara.pShadowLight = pPointLight;
			VSPass::SetDrawContent(pMaterialInstance,pGeometry,pCamera,uiPassId);			
		}
		virtual uint32 GetPassType()
		{
			return PT_POINT_CUBE_SHADOW;
		}


	};

	class GRAPHIC_API VSVolumeShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSVolumeShadowPass();
		VSVolumeShadowPass();

	protected:

		DECLARE_INITIAL
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
	protected:
		VSRenderState m_RenderState;
		void SetCustomConstant(MaterialShaderPara &MSPara, VSGShader * pGShader);
	public:
		virtual bool Draw(VSRenderer * pRenderer);
		virtual uint32 GetPassType()
		{
			if (MSPara.pShadowLight->GetLightType() == VSLight::LT_POINT)
			{
				return PT_POINT_VOLUME_SHADOW;
			} 
			else
			{
				return PT_DIRECT_VOLUME_SHADOW;
			}
			
		}
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, VSLight * pLocalLight, uint32 uiPassId)
		{
			MSPara.pShadowLight = pLocalLight;
			VSPass::SetDrawContent(pMaterialInstance, pGeometry, pCamera, uiPassId);			
		}


	};
	class GRAPHIC_API VSShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSShadowPass();
		VSShadowPass();
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
			return PT_SHADOW;
		}
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, VSLight * pLocalLight, uint32 uiPassId)
		{
			MSPara.pShadowLight = pLocalLight;
			VSPass::SetDrawContent(pMaterialInstance, pGeometry, pCamera, uiPassId);
		}
	}; 

	class GRAPHIC_API VSDualParaboloidShadowPass : public VSPass
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		virtual ~VSDualParaboloidShadowPass();
		VSDualParaboloidShadowPass();
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
			return PT_DUAL_PARABOLOID_SHADOW;
		}
		inline void SetDrawContent(VSMaterialInstance * pMaterialInstance, VSGeometry * pGeometry,
			VSCamera * pCamera, VSLight * pLocalLight, uint32 uiPassId)
		{
			MSPara.pShadowLight = pLocalLight;
			VSPass::SetDrawContent(pMaterialInstance, pGeometry, pCamera, uiPassId);
		}
	}; 

}
