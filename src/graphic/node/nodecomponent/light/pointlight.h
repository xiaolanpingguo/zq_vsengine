#pragma once


#include "graphic/node/nodecomponent/light/light.h"
#include "graphic/render/texture/cubetexture.h"
#include "graphic/render/outputresource/rendertarget.h"
#include "graphic/render/depthstencil.h"
#include "graphic/render/scenemanager/scenerender.h"


namespace zq{


//cube shadow 限制在于光源距离物体太远，rt分辨率不够，存放具体精度不够
//volume 限制在于参与投影的物体必须是闭合的，表面是光滑的，不同光滑组有棱角的地方，要求内嵌三角形。
class VSStream;
class GRAPHIC_API VSPointLight : public VSLocalLight
{
	//PRIORITY
		
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	VSPointLight();
	virtual ~VSPointLight();
public:
		
	virtual bool PostClone(VSObject * pObjectSrc);
	virtual bool PostLoad(VSStream* pStream);
		
	inline void SetRange(float Range)
	{
		m_Range = Range;
		GetLightRange();
	}
	inline float GetRange()const
	{
		return m_Range;
	}
	virtual void BuildSceneInfo(VSScene * pScene);
	virtual uint32 GetLightType()const{return LT_POINT;}
	virtual void DrawDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
	virtual void DisableDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
	virtual void DrawNoDependenceShadowMap(VSCuller & CurCuller, double dAppTime);
	virtual void DisableNoDependenceShadowMap(double dAppTime);
	enum //ShadowType
	{
		ST_CUB,
		ST_VOLUME,
		ST_DUAL_PARABOLOID,
		ST_PROJECT,
		ST_MAX
	};
	virtual bool Cullby(VSCuller & Culler);
	void SetShadowType(uint32 uiShadowType);
	inline uint32 GetShadowType()const
	{
		return m_uiShadowType;
	}
	virtual bool IsRelative(VSGeometry * pGeometry);
	virtual void DrawPorjectShadow(VSCuller & CurCuller,double dAppTime,VS2DTexture * pNormalDepthTexture);
	virtual void DrawVolumeShadow(VSCuller & CurCuller, double dAppTime);
protected:
	void ResetShadow();

	void DrawNormalCubShadow(VSCuller & CurCuller,double dAppTime);
	void DrawFovCubShadow(VSCuller & CurCuller,double dAppTime);

	void DrawNormalProjectShadow(VSCuller & CurCuller,double dAppTime);
	void DrawFovProjectShadow(VSCuller & CurCuller,double dAppTime);

	void DrawDualParaboloidShadow(VSCuller & CurCuller, double dAppTime);


	VSProjectShadowSceneRenderPtr m_pProjectShadowSceneRender;
		
	uint32 m_uiShadowType;
	VSRenderTargetPtr m_pCubRenderTarget[VSCubeTexture::F_MAX];
		
	VSShadowMapSceneRenderPtr m_pShadowMapSceneRender;

	VSPEVolumeShadowMapSceneRenderPtr m_pPEVolumeSMSceneRender;
	VSVolumeShadowSceneRenderPtr m_pVolumeShadowSceneRender;

	VSRenderTargetPtr m_pVolumeShadowRenderTarget;
	VSRenderTargetPtr m_pVolumeShadowFirstPassRenderTarget;

#define m_pDualParaboloidRenderTarget0 m_pVolumeShadowFirstPassRenderTarget
#define m_pDualParaboloidRenderTarget1 m_pVolumeShadowRenderTarget

#define m_pProjectShadowRenderTarget m_pVolumeShadowRenderTarget

		
	virtual void GetLightRange();
	Sphere m_WorldRenderBV;
	float		m_Range;
	VSShadowCuller m_ShadowCuller[VSCubeTexture::F_MAX];
	VSVolumeShadowMapCuller m_VolumeShadowCuller;
};
DECLARE_Ptr(VSPointLight);
VSTYPE_MARCO(VSPointLight);


}
