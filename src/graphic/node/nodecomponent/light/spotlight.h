#pragma once
#include "graphic/node/nodecomponent/light/light.h"
#include "graphic/render/scenemanager/scenerender.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSSpotLight : public VSLocalLight
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		enum //ShadowType
		{
			ST_NORMAL,
			ST_PROJECT,
			ST_MAX
		};
		VSSpotLight();
		virtual ~VSSpotLight();

	public:
		inline void Set(float		Range,
			float		Falloff,
			float		Theta,
			float		Phi)
		{
			m_Range = Range;
			m_Falloff = Falloff;
			m_Theta = Theta;
			m_Phi = Phi;
			GetLightRange();
		}
		inline float GetRange()const
		{
			return m_Range;
		}
		inline float GetFalloff()const
		{
			return m_Falloff;
		}
		inline float GetTheta()const
		{
			return m_Theta;
		}
		inline float GetPhi()const
		{
			return m_Phi;
		}
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual bool PostLoad(VSStream* pStream);
		virtual uint32 GetLightType()const{return LT_SPOT;}
		virtual bool Cullby(VSCuller & Culler);
		virtual bool IsRelative(VSGeometry * pGeometry);
		void ResetShadow();
		void SetShadowType(uint32 uiShadowType);
		virtual void DrawDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DrawNoDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableNoDependenceShadowMap(double dAppTime);
		inline uint32 GetShadowType()const
		{
			return m_uiShadowType;
		}
		virtual void DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
	protected:
		VSShadowMapSceneRenderPtr m_pShadowMapSceneRender;
		uint32 m_uiShadowType;
		void DrawNormalShadowMap(VSCuller & CurCuller,double dAppTime);		

		VSRenderTargetPtr m_pShadowRenderTarget;
		VSProjectShadowSceneRenderPtr m_pProjectShadowSceneRender;

		virtual void GetLightRange();
		AABB m_WorldRenderBV;
		float		m_Range;
		float		m_Falloff;
		float		m_Theta;
		float		m_Phi;
		VSShadowCuller m_ShadowCuller;
	};
	DECLARE_Ptr(VSSpotLight);
	VSTYPE_MARCO(VSSpotLight);

}
