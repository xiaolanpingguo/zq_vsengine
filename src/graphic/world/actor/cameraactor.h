#pragma once
#include "graphic/world/actor/actor.h"
#include "graphic/node/nodecomponent/camera.h"
#include "graphic/posteffect/posteffectset.h"
namespace zq
{
	class VSViewFamily;
	class GRAPHIC_API VSViewFamliyInfo : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		DECLARE_INITIAL
		VSViewFamliyInfo();
		~VSViewFamliyInfo();
		VSArray<VSString> m_SceneMapName;
		uint32 m_uiWidth;
		uint32 m_uiHeight;
		uint32 m_uiCaptureViewFamilyType;
		VSString	m_ViewFamilyName;
		VSPostEffectSetRPtr m_pPostEffect;
		VSViewFamily * m_pViewFamily;
		VSString	m_RenderMethodRTTIName;
		void GetScene();
	};
	DECLARE_Ptr(VSViewFamliyInfo);
	VSTYPE_MARCO(VSViewFamliyInfo);
	class  GRAPHIC_API VSCameraActor : public VSActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		VSCameraActor();
		virtual ~VSCameraActor();
		DECLARE_INITIAL

		VSArray<VSViewFamliyInfoPtr> m_ViewFamliyInfoArray;
		GET_TYPE_NODE(VSCamera)
		virtual void Update(double dAppTime);
		virtual void CreateDefaultComponentNode();
		void AddCustomCullPlane(const Plane& P);
		void ClearCustomCullPlane();
	};
	DECLARE_Ptr(VSCameraActor);
	VSTYPE_MARCO(VSCameraActor);
}
