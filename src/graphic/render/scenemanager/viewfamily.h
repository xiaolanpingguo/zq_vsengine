#pragma once
#include "core/system.h"
#include "graphic/core/reference.h"
#include "graphic/node/nodecomponent/camera.h"
#include "graphic/render/scenemanager/scenerender.h"
#include "graphic/render/texture/cubetexture.h"
namespace zq
{
	DECLARE_Proxy(VSPostEffectSet);
	class GRAPHIC_API VSSceneRenderMethod : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		VSSceneRenderMethod();
		virtual ~VSSceneRenderMethod() = 0;
		void SetPostEffect(VSPostEffectSetR* pPostEffectSet);
		virtual void Draw(VSCuller & Culler,double dAppTime);
		virtual void GetRT(VSCuller & Culler, uint32 uiWidth,uint32 uiHeight);
		virtual void DisableRT();
		inline void SetDebugDrawInfo(bool bDrawDebugInfo, bool bBeforePostDebugDraw = true)
		{
			m_bBeforePostDebugDraw = bBeforePostDebugDraw;
			m_bDrawDebugInfo = bDrawDebugInfo;
		}
		virtual void LoadedEvent(VSResourceProxyBase * pResourceProxy, void * Data = NULL);
		VSRenderTarget * GetFinalColorRT();

		VSPostEffectSet * GetPostEffectSet()const
		{
			return m_pPostEffectInstance;
		}
		VSDebugDraw * GetDebugDraw(uint32 uiRenderGroup);
		virtual void SetUseState(VSRenderState & RenderState,uint32 uiRenderStateInheritFlag) = 0;
		virtual void ClearUseState() = 0;

	protected:
		VSPostEffectSetRPtr m_pPostEffectSet;
		VSPostEffectSetPtr	m_pPostEffectInstance;
		VSDebugDrawSceneRenderPtr m_pDebugDrawSceneRender;
		bool m_bBeforePostDebugDraw;
		bool m_bDrawDebugInfo;
		VSDepthStencilPtr m_pDepthStencil[VSCuller::RG_MAX];
		VSRenderTargetPtr m_pColorRT;
		bool ColorRTIsOutSet;
	};
	DECLARE_Ptr(VSSceneRenderMethod);
	VSTYPE_MARCO(VSSceneRenderMethod);
	class GRAPHIC_API VSForwardHighEffectSceneRenderMethod : public VSSceneRenderMethod
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSForwardHighEffectSceneRenderMethod();
		~VSForwardHighEffectSceneRenderMethod();
		virtual void Draw(VSCuller & Culler,double dAppTime);
		virtual void GetRT(VSCuller & Culler, uint32 uiWidth,uint32 uiHeight);
		virtual void DisableRT();
		virtual void SetUseState(VSRenderState & RenderState,uint32 uiRenderStateInheritFlag);
		virtual void ClearUseState();
	protected:	
		VSMaterialSceneRenderPtr m_pMaterialSceneRenderder;
		VSNormalDepthSceneRenderPtr m_pNormalDepthSceneRender;
		VSPEGammaCorrectSceneRenderPtr m_pGammaCorrectSceneRender;
		//VSPESSRSceneRenderPtr m_pSSRSceneRender;
		VSRenderTargetPtr m_pMaterialRT;
		VSRenderTargetPtr m_pNormalDepthRT;
		VSRenderTargetPtr m_pSSRRT;
	};
	DECLARE_Ptr(VSForwardHighEffectSceneRenderMethod);
	VSTYPE_MARCO(VSForwardHighEffectSceneRenderMethod);
	
	class GRAPHIC_API VSForwardEffectSceneRenderMethod : public VSSceneRenderMethod
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSForwardEffectSceneRenderMethod();
		~VSForwardEffectSceneRenderMethod();
		virtual void Draw(VSCuller & Culler, double dAppTime);
		virtual void GetRT(VSCuller & Culler, uint32 uiWidth, uint32 uiHeight);
		virtual void DisableRT();
		virtual void SetUseState(VSRenderState & RenderState, uint32 uiRenderStateInheritFlag);
		virtual void ClearUseState();
	protected:
		VSMaterialSceneRenderPtr m_pMaterialSceneRenderder;
		VSPEGammaCorrectSceneRenderPtr m_pGammaCorrectSceneRender;
		VSRenderTargetPtr m_pMaterialRT;
	};
	DECLARE_Ptr(VSForwardEffectSceneRenderMethod);
	VSTYPE_MARCO(VSForwardEffectSceneRenderMethod);

	class GRAPHIC_API VSSimpleForwardEffectSceneRenderMethod : public VSSceneRenderMethod
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSSimpleForwardEffectSceneRenderMethod();
		~VSSimpleForwardEffectSceneRenderMethod();
		virtual void Draw(VSCuller & Culler, double dAppTime);
		virtual void GetRT(VSCuller & Culler, uint32 uiWidth, uint32 uiHeight);
		virtual void DisableRT();
		virtual void SetUseState(VSRenderState & RenderState, uint32 uiRenderStateInheritFlag);
		virtual void ClearUseState();
	protected:
		VSMaterialSceneRenderPtr m_pMaterialSceneRenderder;
	};
	DECLARE_Ptr(VSSimpleForwardEffectSceneRenderMethod);
	VSTYPE_MARCO(VSSimpleForwardEffectSceneRenderMethod);
	class GRAPHIC_API VSViewFamily : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		
		VSViewFamily(const VSString &ViewFamilyName, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const char * RenderMethodRTTIName);
		virtual ~VSViewFamily();
		inline void AddScene(VSScene* pScene);
		inline VSScene * GetScene(uint32 i)const;
		inline void ClearAllScene();
		inline void DeleteScene(uint32 i);
		inline void DeleteScene(VSScene * pScene);
		inline uint32 GetSceneNum()const;
		virtual void Draw(double dAppTime);
		virtual void OnDraw(double dAppTime);
		virtual void Update(double dAppTime);

		virtual void CreateRenderTargetBuffer(uint32 uiWidth,uint32 uiHeight);
		bool IsReCreate(uint32 uiWidth,uint32 uiHeight);

		VSString m_ViewFamilyName;
		bool m_bEnable;

		VSSceneRenderMethodPtr m_pSceneRenderMethod;

		virtual VSRenderTarget * GetFinalColorRT();

		virtual void SetCullUseID(int32 USeUseMaterialLayer)
		{
			m_Culler.SetUseMaterialIndex(USeUseMaterialLayer);
		}
		virtual bool GetStreamCameraInfo(Vector3 & CameraPos,Vector3W& ProjectInfo);
	protected:
		VSCamera* m_pCamera;
		VSArray<VSScene *> m_pScene;
		VSCuller	m_Culler;

	protected:
		uint32 m_uiWidth;
		uint32 m_uiHeight;	


		
	};
	inline void VSViewFamily::AddScene(VSScene * pSecen)
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
	inline VSScene * VSViewFamily::GetScene(uint32 i)const
	{
		if(i >= m_pScene.GetNum())
			return NULL;
		return m_pScene[i];
	}
	inline void VSViewFamily::ClearAllScene()
	{
		m_pScene.Clear();
	}
	inline void VSViewFamily::DeleteScene(uint32 i)
	{	
		if(i >= m_pScene.GetNum())
			return ;
		m_pScene.Erase(i);

	}
	inline void VSViewFamily::DeleteScene(VSScene * pScene)
	{
		if (!pScene)
		{
			return ;
		}
		for (uint32 i= 0 ; i <m_pScene.GetNum() ; i++)
		{
			if(pScene == m_pScene[i])
			{
				m_pScene.Erase(i);
			}
		}
	}

	inline uint32 VSViewFamily::GetSceneNum()const
	{
		return m_pScene.GetNum();
	}
	class GRAPHIC_API VSWindowViewFamily : public VSViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		enum 
		{
			VT_WINDOW_NORMAL,
			VT_WINDOW_MAX
		};
		VSWindowViewFamily(const VSString &ViewFamilyName, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const char * RenderMethodRTTIName, int32 iWindowID = -1);
		//VSWindowViewFamily(const VSString &ViewFamilyName,VSCamera * pCamera,int32 iWindowID = -1);
		virtual ~VSWindowViewFamily();
		virtual void Draw(double dAppTime);
		virtual void OnDraw(double dAppTime);
		virtual void CreateRenderTargetBuffer(uint32 uiWidth,uint32 uiHeight);

	protected:
		int32 m_iWindowID;
		VSPEScreenQuadSceneRenderPtr  m_pScreenQuadRenderer;
	};

	class GRAPHIC_API VSCaptureViewFamily : public VSViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		enum //ViewFamily Type
		{
			VT_CAPTURE_2D_CAPTURE_NORMAL,
			VT_CAPTURE_CUB_CAPTURE_NORMAL,
			VT_MAX
		};
		VSCaptureViewFamily(const VSString &ViewFamilyName, uint32 uiWidth, uint32 uiHeight, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const char * RenderMethodRTTIName);
		//VSCaptureViewFamily(const VSString &ViewFamilyName,uint32 uiWidth,uint32 uiHeight,VSCamera * pCamera);
		virtual ~VSCaptureViewFamily();
		virtual void OnDraw(double dAppTime);
		void SetSize(uint32 uiWidth,uint32 uiHeight);
		virtual void CreateRenderTargetBuffer(uint32 uiWidth,uint32 uiHeight);
		virtual VSTexture * GetTexture()const = 0;
		
		virtual void Draw(double dAppTime);
		friend class VSCaptureTexAllState;
	protected:
		VSPEScreenQuadSceneRenderPtr  m_pScreenQuadRenderer;
		VSCaptureTexAllState*  m_pTexOwner;
		bool m_OnlyUpdateOneTime;
		
	};
	class GRAPHIC_API VSCubCaptureViewFamily : public VSCaptureViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VSCubCaptureViewFamily(const VSString &ViewFamilyName, uint32 uiWidth, uint32 uiHeight, VSCamera * pCamera, VSPostEffectSetR* pPostEffectSet, const char * RenderMethodRTTIName);
		virtual ~VSCubCaptureViewFamily();
		virtual void OnDraw(double dAppTime);
		virtual void CreateRenderTargetBuffer(uint32 uiWidth,uint32 uiHeight);
		virtual void Update(double dAppTime);
		virtual VSTexture * GetTexture()const;
		virtual void SetCullUseID(int32 USeUseMaterialLayer)
		{
			for (uint32 i = 0 ; i < VSCubeTexture::F_MAX ;i++)
			{
				m_CubCuller[i].SetUseMaterialIndex(USeUseMaterialLayer);
			}
		}
		virtual bool GetStreamCameraInfo(Vector3& CameraPos, Vector3W& ProjectInfo);
	protected:
		VSCameraPtr CubCameraPtr[VSCubeTexture::F_MAX];
		VSCuller m_CubCuller[VSCubeTexture::F_MAX];
		VSRenderTargetPtr m_pCubRenderTarget[VSCubeTexture::F_MAX];
		
	};

	class GRAPHIC_API VS2DCaptureViewFamily : public VSCaptureViewFamily
	{
		//RTTI
		DECLARE_RTTI;
	public:
		VS2DCaptureViewFamily(const VSString &ViewFamilyName,uint32 uiWidth,uint32 uiHeight,VSCamera * pCamera,VSPostEffectSetR* pPostEffectSet,const char * RenderMethodRTTIName);
		virtual ~VS2DCaptureViewFamily();
		virtual void CreateRenderTargetBuffer(uint32 uiWidth,uint32 uiHeight);
		virtual VSTexture * GetTexture()const;
		virtual void OnDraw(double dAppTime);
	protected:
		VSRenderTargetPtr m_pRenderTarget;

	};

}