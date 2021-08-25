#pragma once


#include "graphic/render/texture/texture2d.h"
#include "graphic/render/depthstencil.h"
#include "graphic/render/scenemanager/spatial.h"
#include "graphic/render/scenemanager/scene.h"
#include "graphic/posteffect/posteffectset.h"
#include "graphic/render/debugdraw.h"
#include "graphic/render/outputresource/rendertarget.h"
#include "graphic/material/material.h"

#include "graphic/render/scenerendermarc.h"


namespace zq
{
	DECLARE_Ptr(VSCustomMaterial);
	class GRAPHIC_API VSSceneRenderInterface : public VSReference,public MemObject
	{
	public:
		VSSceneRenderInterface();
		virtual ~VSSceneRenderInterface() = 0;

		virtual VSRenderTarget * GetRenderTarget(uint32 uiIndex);

		inline void SetParam(uint32 uiClearFlag,VSColorRGBA ClearColorRGBA,float fClearDepth,
						uint32 uiClearStencil,bool bUseViewClear = false)
		{
			m_uiClearFlag = uiClearFlag;
			m_ClearColorRGBA = ClearColorRGBA;
			m_fClearDepth = fClearDepth;
			m_uiClearStencil = uiClearStencil;
			m_bUseViewPortClear = bUseViewClear;

		}
		virtual bool AddRenderTarget(VSRenderTarget * pTarget);
		
		void SetRenderTargets();
		void EndRenderTargets();
		virtual void ClearRTAndDepth() = 0;
	protected:
		VSArray<VSRenderTargetPtr> m_pTargetArray;
		
		uint32 m_uiClearFlag;
		VSColorRGBA m_ClearColorRGBA;
		float m_fClearDepth;
		uint32 m_uiClearStencil;
		bool  m_bUseViewPortClear;
		
		uint32 m_uiRTWidth;
		uint32 m_uiRTHeight;
		uint32 m_uiMulSample;
	};
	DECLARE_Ptr(VSSceneRenderInterface);
	
	class GRAPHIC_API VSSceneRender : public VSSceneRenderInterface
	{
	public:
		VSSceneRender();
		virtual ~VSSceneRender();
		
		inline VSDepthStencil * GetDepthStencil(uint32 uiRenderGroup)const
		{
			if (uiRenderGroup >= VSCuller::RG_MAX)
			{
				return false;
			}
			return m_pDepthStencil[uiRenderGroup];
		}	
		virtual bool SetDepthStencil(VSDepthStencil * pDepthStencil,uint32 uiRenderGroup);
		virtual bool Draw(VSCuller & Culler,double dAppTime);
		virtual void ClearRTAndDepth();
		virtual void DrawUseCurView(VSCuller & Culler,double dAppTime);
		virtual void DisableUseCurView(VSCuller & Culler,double dAppTime);
		void SetUseState(VSRenderState & RenderState, uint32 uiRenderStateInheritFlag);
		void ClearUseState();
	protected:
		virtual bool OnDraw(VSCuller & Culler,uint32 uiRenderGroup,double dAppTime) = 0;
		VSDepthStencilPtr m_pDepthStencil[VSCuller::RG_MAX];
		VSRenderState m_SaveRenderState;
		VSRenderState m_UseState;
		uint32 m_uiRenderStateInheritFlag;
	};
	DECLARE_Ptr(VSSceneRender);
	class GRAPHIC_API VSOcclusionQuerySceneRender : public VSSceneRender
	{
	public:
		VSOcclusionQuerySceneRender(VSCuller & Culler, VSDepthStencilPtr pDepthStencil[VSCuller::RG_MAX]);
		virtual ~VSOcclusionQuerySceneRender();
		virtual bool Draw(VSCuller & Culler, double dAppTime);
	protected:
		virtual bool OnDraw(VSCuller & Culler, uint32 uiRenderGroup, double dAppTime);
	};
	DECLARE_Ptr(VSOcclusionQuerySceneRender);

	class GRAPHIC_API VSMaterialSceneRender : public VSSceneRender
	{
	public:
		VSMaterialSceneRender();
		virtual ~VSMaterialSceneRender();
		virtual void DrawUseCurView(VSCuller & Culler,double dAppTime);
		virtual void DisableUseCurView(VSCuller & Culler,double dAppTime);
		void SetNormalDepthTexture(VS2DTexture * pNormalDepthTexture)
		{
			m_pNormalDepthTexture = pNormalDepthTexture;
		}
	protected:
		virtual bool OnDraw(VSCuller & Culler,uint32 uiRenderGroup,double dAppTime);
		VS2DTexturePtr m_pNormalDepthTexture;

		VSArray<VSRenderContext *> m_NormalAndDepth;
		VSArray<VSRenderContext *> m_NoNormalOrDepth;
		VSArray<VSRenderContext *> m_Combine;
		VSArray<VSRenderContext *> m_AlphaBlend;
		void GetGroup(VSCuller & Culler, uint32 uiRenderGroup);
		void DrawGroup(VSCuller & Culler, uint32 uiRenderGroup, VSArray<VSRenderContext *> & Group, double dAppTime);
		void DrawProjectShadow(VSCuller & Culler, uint32 uiRenderGroup, double dAppTime);
		void DrawVolumeShadow(VSCuller & Culler, uint32 uiRenderGroup, double dAppTime);
	};
	DECLARE_Ptr(VSMaterialSceneRender);

	class GRAPHIC_API VSDebugDrawSceneRender : public VSSceneRender
	{
	public:
		VSDebugDrawSceneRender();
		virtual ~VSDebugDrawSceneRender();
		inline VSDebugDraw * GetDebugDraw(uint32 uiRenderGroup)
		{
			if (uiRenderGroup >= VSCuller::RG_MAX)
			{
				return false;
			}
			return m_pDebugDraw[uiRenderGroup];
		}
	protected:
		virtual bool OnDraw(VSCuller & Culler,uint32 uiRenderGroup,double dAppTime);
		VSDebugDrawPtr m_pDebugDraw[VSCuller::RG_MAX];
	};
	DECLARE_Ptr(VSDebugDrawSceneRender);


	class GRAPHIC_API VSNormalDepthSceneRender : public VSSceneRender
	{
	public:
		VSNormalDepthSceneRender();
		virtual ~VSNormalDepthSceneRender();
	protected:
		virtual bool OnDraw(VSCuller & Culler,uint32 uiRenderGroup,double dAppTime);
	};
	DECLARE_Ptr(VSNormalDepthSceneRender);


	class GRAPHIC_API VSPreZSceneRender : public VSSceneRender
	{
	public:
		VSPreZSceneRender();
		virtual ~VSPreZSceneRender();
	protected:
		virtual bool OnDraw(VSCuller & Culler,uint32 uiRenderGroup,double dAppTime);
	};
	DECLARE_Ptr(VSPreZSceneRender);

	
	class GRAPHIC_API VSPostEffectSceneRender : public VSSceneRenderInterface
	{
	protected:
		VSRenderTarget * m_pFinalRenderTarget;
		virtual bool OnDraw(VSCuller & Culler, double dAppTime);
		VSCustomMaterialPtr m_pCustomMaterial;
		VSDepthStencilPtr m_pDepthStencil;
	public:
		virtual bool SetDepthStencil(VSDepthStencil * pDepthStencil);
		VSDepthStencil * GetDepthStencil()const
		{
			return m_pDepthStencil;
		}
		VSPostEffectSceneRender();
		virtual ~VSPostEffectSceneRender();
		virtual bool Draw(VSCuller & Culler,double dAppTime);
		virtual void ClearRTAndDepth();
		virtual void SetNoUseRTRenderSize(uint32 uiWidth,uint32 uiHeight);
		virtual bool AddRenderTarget(VSRenderTarget * pTarget);

		//Use By PostEffectFunction
		virtual void GetRT(uint32 uiWidth, uint32 uiHeight);
		virtual void DisableRT();
	};
	DECLARE_Ptr(VSPostEffectSceneRender);

	class GRAPHIC_API VSCommonPESceneRender : public VSPostEffectSceneRender
	{
	protected:
		VSTexAllStatePtr m_pTexAllState[POSTEFFECT_MAX_INPUT];
		virtual void OnUpdate(double dAppTime);
		virtual bool OnDraw(VSCuller & Culler, double dAppTime);
		float m_fGetRTScale = 1.0f;
	public:
		VSCommonPESceneRender();
		virtual ~VSCommonPESceneRender();
		virtual void SetSourceTarget(VSTexture * pTexture,uint32 i = 0);
		virtual void GetRT(uint32 uiWidth, uint32 uiHeight);
		virtual void ClearRTAndDepth();
		VSTexture * GetInputTexture(uint32 i) const;
	};
	DECLARE_Ptr(VSCommonPESceneRender);

	class GRAPHIC_API VSCombinePostEffectSceneRender : public VSCommonPESceneRender
	{
	public:
		VSCombinePostEffectSceneRender(uint32 uiPassNum)
		{
			m_CPEArray.SetBufferNum(uiPassNum);
		}
		~VSCombinePostEffectSceneRender();
		virtual bool Draw(VSCuller & Culler, double dAppTime);
		virtual void GetRT(uint32 uiWidth, uint32 uiHeight);
		virtual void DisableRT();
		virtual bool AddRenderTarget(VSRenderTarget * pTarget);
		virtual VSRenderTarget * GetRenderTarget(uint32 uiIndex);
		virtual void OnUpdateWithID(uint32 uiPassID,double dAppTime);
	protected:
		VSArray<VSCommonPESceneRenderPtr> m_CPEArray;
	};
	DECLARE_Ptr(VSCombinePostEffectSceneRender);

// 	class GRAPHIC_API VSPESSRSceneRender : public VSPostEffectSceneRender
// 	{
// 
// 	public:
// 		VSPESSRSceneRender();
// 		virtual ~VSPESSRSceneRender();	
// 		virtual void SetUseTexture(VSTexture * pNormalDepthTexture, VSTexture *pSceneColorTexture);
// 		virtual bool OnDraw(VSCuller & Culler, double dAppTime);
// 	protected:
// 		VSTexAllStatePtr m_pNormalDepthTexture;
// 		VSTexAllStatePtr m_pSceneColorTexture;
// 	};
// 	DECLARE_Ptr(VSPESSRSceneRender);

	class GRAPHIC_API VSPEMaterialSceneRender : public VSCommonPESceneRender
	{
	public:
		VSPEMaterialSceneRender();
		virtual ~VSPEMaterialSceneRender();
		VSMaterialInstance * m_pMaterialInstacne;
	protected:
		
		virtual bool OnDraw(VSCuller & Culler, double dAppTime);
	};
	DECLARE_Ptr(VSPEMaterialSceneRender);
	class GRAPHIC_API VSPEVolumeShadowMapSceneRender : public VSPostEffectSceneRender
	{
	public:
		VSPEVolumeShadowMapSceneRender();
		virtual ~VSPEVolumeShadowMapSceneRender();
	};
	DECLARE_Ptr(VSPEVolumeShadowMapSceneRender);

	class GRAPHIC_API VSShadowMapSceneRender : public VSSceneRender
	{
	public:
		enum //
		{
			SMT_CUB,
			SMT_SHADOWMAP,
			SMT_DUAL_PARABOLOID,
			SMT_MAX
		};
		VSShadowMapSceneRender(uint32 uiShadowMapType);
		virtual ~VSShadowMapSceneRender();	
		VSLocalLight * m_pLocalLight;
		virtual bool Draw(VSCuller & Culler,double dAppTime);
	protected:
		virtual bool OnDraw(VSCuller & Culler,uint32 uiRenderGroup,double dAppTime);
		uint32 m_uiShadowMapType;
	};
	DECLARE_Ptr(VSShadowMapSceneRender);

	class GRAPHIC_API VSLightMaterialSceneRender : public VSSceneRenderInterface
	{
	public:

		VSLightMaterialSceneRender();
		virtual ~VSLightMaterialSceneRender();	
		class VSMaterialInstance * m_pMaterialInstacne;
		virtual bool Draw(VSCuller & Culler,double dAppTime);
		virtual bool SetDepthStencil(VSDepthStencil * pDepthStencil);
		VSDepthStencil * GetDepthStencil()const
		{
			return m_pDepthStencil;
		}
		virtual void ClearRTAndDepth();

		VSLight * m_pLight;
	protected:
		VSDepthStencilPtr m_pDepthStencil;
		virtual bool OnDraw(VSCuller & Culler, double dAppTime);
	};
	DECLARE_Ptr(VSLightMaterialSceneRender);

	class GRAPHIC_API VSProjectShadowSceneRender : public VSSceneRender
	{
	public:

		VSProjectShadowSceneRender();
		virtual ~VSProjectShadowSceneRender();	
		VSLocalLight * m_pLocalLight;
		virtual bool Draw(VSCuller & Culler,double dAppTime);
		VS2DTexture * m_pNormalDepthTexture;
		float m_fLightRange;
		Vector3 m_LightWorldDirection;
		float m_Falloff;
		float m_Theta;
		float m_Phi;
	protected:
		virtual bool OnDraw(VSCuller & Culler,uint32 uiRenderGroup,double dAppTime);
		VSTexAllStatePtr m_pTexAllState;
		VSShaderKey m_DirectionLightShaderKey;
		VSShaderKey m_PointLightShaderKey;
		VSShaderKey m_SpotLightShaderKey;

	};
	DECLARE_Ptr(VSProjectShadowSceneRender);
	class GRAPHIC_API VSVolumeShadowSceneRender : public VSSceneRender
	{
	public:

		VSVolumeShadowSceneRender();
		virtual ~VSVolumeShadowSceneRender();
		VSLocalLight * m_pLocalLight;
		virtual bool Draw(VSCuller & Culler, double dAppTime);
	protected:
		virtual bool OnDraw(VSCuller & Culler, uint32 uiRenderGroup, double dAppTime);

	};
	DECLARE_Ptr(VSVolumeShadowSceneRender);

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(Gray,GetPostGray)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(GammaCorrect,GetGammaCorrect)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(ScreenQuad,GetPostScreenQuad)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(Saturate,GetPostSaturation)
	float Saturation = 0.5f;
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(OldPhoto,GetOldPhoto)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(GuassBlurH,GetGuassBlurH)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(GuassBlurV,GetGuassBlurV)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(Filter3X3, GetFilter3X3)
	POST_EFFECT_SCENE_RENDER_END

	POST_EFFECT_SCENE_RENDER_SCALE(DownFilter4, VSPEFilter3X3SceneRender, 0.25f)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(TexAdd,GetTexAdd)
	POST_EFFECT_SCENE_RENDER_END

	COMMON_POST_EFFECT_SCENE_RENDER_DECLARE_BEGIN(Bright,GetBright)
		float MiddleGray = 0.08f;
		float WhiteCutoff = 0.18f;
		float Luminance = 0.8f;
	POST_EFFECT_SCENE_RENDER_END
	
	POST_EFFECT_SCENE_RENDER_SCALE(BloomBright, VSPEBrightSceneRender, 0.25f)
	POST_EFFECT_SCENE_RENDER_END

	COMBINE_POST_EFFECT_SCENE_RENDER_DECLARE_TWO_BEGIN(GuassBlur,VSPEGuassBlurHSceneRender, VSPEGuassBlurVSceneRender)
	POST_EFFECT_SCENE_RENDER_END
	
	POST_EFFECT_SCENE_RENDER_SCALE(BloomGuassBlur, VSPEGuassBlurSceneRender, 0.25f)
	POST_EFFECT_SCENE_RENDER_END

	COMBINE_POST_EFFECT_SCENE_RENDER_DECLARE_FOUR_BEGIN(Bloom, VSPEDownFilter4SceneRender, VSPEBloomBrightSceneRender, VSPEBloomGuassBlurSceneRender,VSPETexAddSceneRender)
		float MiddleGray = 0.08f;
		float WhiteCutoff = 0.18f;
		float Luminance = 0.8f;
	POST_EFFECT_SCENE_RENDER_END
}