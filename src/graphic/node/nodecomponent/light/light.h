#pragma once
#include "graphic/node/nodecomponent/nodecomponent.h"
#include "math/vector3.h"
#include "math/vector3w.h"
#include "graphic/core/resource.h"
#include "math/vector2.h"
#include "graphic/render/scenemanager/culler.h"
namespace zq
{
	class VSShadowMapSceneRender;
	class VSStream;
	class VS2DTexture;
	class VSCamera;
	DECLARE_Ptr(VSTexAllState);
	DECLARE_Ptr(VSMaterialInstance);
	DECLARE_Ptr(VSMaterial);
	DECLARE_Ptr(VSRenderTarget);
	class GRAPHIC_API VSLight : public VSNodeComponent
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		friend class VSSpatial;
		friend class VSScene;
		enum	//LightType
		{
			LT_POINT,
			LT_SPOT,
			LT_DIRECTION,
			LT_SKY,
			LT_MAX
		};
		VSLight();
		virtual ~VSLight() = 0;
	public:
		virtual void UpdateAll(double dAppTime);
		virtual uint32 GetLightType()const = 0;
		virtual bool IsRelative(VSGeometry * pGeometry);
		virtual bool Cullby(VSCuller & Culler);
		virtual void UpdateLightState(double dAppTime);
		virtual void GetLightRange() = 0;
		virtual void UpdateTransform(double dAppTime);
		virtual void BuildSceneInfo(VSScene * pScene);
	};
	DECLARE_Ptr(VSLight);
	VSTYPE_MARCO(VSLight);
	
	DECLARE_Ptr(VSLightMaterialSceneRender);
	class GRAPHIC_API VSLocalLight : public VSLight
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		VSLocalLight();
		virtual ~VSLocalLight() = 0;
		VSColorRGBA m_Diffuse;
		VSColorRGBA m_Specular;
		virtual void DrawNoDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableNoDependenceShadowMap( double dAppTime);
		virtual void DrawDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DisableDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
		virtual void DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
		virtual void DrawVolumeShadow(VSCuller & CurCuller, double dAppTime);
		virtual void UpdateAll(double dAppTime);
		virtual void DrawLightMaterial(double dAppTime);
		void SetCastShadow(bool bIsCastShadow);
		inline bool GetCastShadow()
		{
			return m_bIsCastShadow;
		}
		inline void AddShadowScene(VSScene * pSecen)
		{
			if(pSecen)
			{
				for (uint32 i = 0 ; i < m_pScene.GetNum() ;i++)
				{
					if (m_pScene[i] == pSecen)
					{
						return ;
					}
				}
				m_pScene.AddElement(pSecen);
			}
		}
		
		inline void ClearAllShadowScene()
		{
			m_pScene.Clear();
		}
		virtual VSTexAllState * GetShadowTexture(uint32 ID = 0)
		{
			return m_pShadowTexture[ID];	
		}
		uint32 GetShadowTextureNum()
		{
			return m_pShadowTexture.GetNum();
		}
		uint32 GetShadowResolution()const
		{
			return m_uiRTWidth;
		}
		inline VSTexAllState * GetLightFunDiffuseTexture()const
		{
			return m_pLightFunDiffuseTexture;
		}
		virtual bool SetLightMaterial(VSMaterialR * pMaterial);
		float m_ZBias;
		Matrix3X3W m_LightShadowMatrix;
		void SetLightMaterialRtWidth(uint32 uiRtWidth);

		inline VSMaterialInstance * GetMaterialInstance()const
		{
			return m_pLightMaterial;
		}
		inline bool HaveLightFun()const
		{
			return (m_pLightMaterial != NULL);
		}
		uint32 GetLightMaterialRtWidth()
		{
			return m_uiLightMaterialRTWidth;
		}
		inline Matrix3X3W &GetWVP()
		{
			return m_WVP;
		}
		virtual void BuildSceneInfo(VSScene * pScene);
		Vector2 m_LightFunScale;
		Vector2 m_LightFunOffset;
		VSColorRGBA m_ProjectShadowColor;
		float m_ProjectShadowFallOff;
	protected:  
		bool m_bIsCastShadow;
		uint32 m_uiRTWidth;
		VSArray<VSScene *> m_pScene;
		VSArray<VSTexAllStatePtr> m_pShadowTexture;
		VSTexAllStatePtr m_pLightFunDiffuseTexture;
		VSMaterialInstancePtr	m_pLightMaterial;
		uint32 m_uiLightMaterialRTWidth;
		VSLightMaterialSceneRenderPtr m_pLMSceneRender;
		VSRenderTargetPtr m_pLightFunDiffuseRenderTarget;
		VSRenderTargetPtr m_pLightFunSpecularRenderTarget;
		Matrix3X3W m_WVP;
		virtual void GetLightRange() = 0;
		void GetCullerAABBArray(VSCuller & Culler, VSArray<AABB> & AABBArray);
		bool m_bShadowMapDrawEnd;
	};
	DECLARE_Ptr(VSLocalLight);
	VSTYPE_MARCO(VSLocalLight);
	class GRAPHIC_API VSIndirectLight : public VSLight
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		VSIndirectLight();
		virtual ~VSIndirectLight() = 0;
		virtual bool IsRelative(VSGeometry * pGeometry);
		virtual bool Cullby(VSCuller & Culler);
		virtual void SetLocalScale(const Vector3 & fScale);
		virtual void SetLocalRotate(const Matrix3X3 & Rotate);
		virtual void SetLocalTransform(const VSTransform & LocalTransform);
		virtual void SetLocalMat(const Matrix3X3W VSMat);
		inline void SetRange(Vector3 & Range)
		{
			m_Range.x = Range.x > 1.0f ? Range.x : m_Range.x;
			m_Range.y = Range.y > 1.0f ? Range.y : m_Range.y;
			m_Range.z = Range.z > 1.0f ? Range.z : m_Range.z;
			GetLightRange();
		}
	protected:
		Vector3 m_Range;
		virtual void GetLightRange();
		AABB m_WorldRenderBV;
	};
	DECLARE_Ptr(VSIndirectLight);
	VSTYPE_MARCO(VSIndirectLight);
}
