#pragma once
#include "graphic/node/nodecomponent/light/light.h"
#include "graphic/render/texture/cubetexture.h"
#include "graphic/render/outputresource/rendertarget.h"
#include "graphic/render/depthstencil.h"
#include "graphic/render/scenemanager/scenerender.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSDirectionLight : public VSLocalLight
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSDirectionLight();
		virtual ~VSDirectionLight();
	public:
		virtual bool PostClone(VSObject * pObjectSrc);
		virtual bool PostLoad(VSStream* pStream);
		virtual uint32 GetLightType()const{return LT_DIRECTION;}

		enum //ShadowType
		{
			ST_VOLUME,
			ST_OSM,
			ST_CSM,
			ST_PROJECT,
			ST_MAX
		};
		void SetShadowType(uint32 uiShadowType);
		inline uint32 GetShadowType()const
		{
			return m_uiShadowType;
		}
		virtual void DrawDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
		virtual void DrawVolumeShadow(VSCuller & CurCuller, double dAppTime);

		Matrix3X3W m_CSMLightShadowMatrix[3];



	protected:
	
		uint32 m_uiShadowType;

		VSProjectShadowSceneRenderPtr m_pProjectShadowSceneRender;
		VSShadowMapSceneRenderPtr m_pShadowMapSceneRender;
		VSPEVolumeShadowMapSceneRenderPtr m_pPEVolumeSMSceneRender;
		VSVolumeShadowSceneRenderPtr m_pVolumeShadowSceneRender;

		VSRenderTargetPtr m_pVolumeShadowRenderTarget;

#define m_pOSMShadowRenderTarget m_pVolumeShadowRenderTarget
#define m_pProjectShadowRenderTarget m_pVolumeShadowRenderTarget
		VSArray<VSRenderTargetPtr> m_pCSMRTArray;

		void DrawOSM(VSCuller & CurCuller,double dAppTime);
		void DrawCSM(VSCuller & CurCuller,double dAppTime);


		AABB GetMaxAABB(VSArray<AABB> &AABBArray);


		void ResetShadow();

		float m_fLightFunWidth;
		float m_fLightFunHeight;
		float m_fLightFunFar;
		float m_fLightFunNear;
		float m_fOSMDistance;
		virtual void GetLightRange();
		AABB m_WorldRenderBV;
		VSDirShadowMapCuller m_ShadowCuller;
		VSVolumeShadowMapCuller m_VolumeShadowCuller;
	public:
		inline void SetLightFuncParam(float fLightFunWidth,
			float fLightFunHeight,
			float fLightFunFar,
			float fLightFunNear = 1.0f)
		{
			m_fLightFunWidth = fLightFunWidth;
			m_fLightFunHeight = fLightFunHeight;
			m_fLightFunFar = fLightFunFar;
			m_fLightFunNear = fLightFunNear;
			GetLightRange();
		}
		virtual bool SetLightMaterial(VSMaterialR * pMaterial);
		virtual bool Cullby(VSCuller & Culler);
		virtual bool IsRelative(VSGeometry * pGeometry);
	};
	DECLARE_Ptr(VSDirectionLight);
	VSTYPE_MARCO(VSDirectionLight);

}
