#pragma once
#include "graphic/world/actor/reflectcameraactor.h"
#include "graphic/world/actor/cameraactor.h"
#include "graphic/posteffect/posteffectset.h"
namespace zq
{
	class  GRAPHIC_API VSReflectCameraActor : public VSCameraActor
	{
		//PRIORITY

		//RTTI
		DECLARE_RTTI;
	public:
		VSReflectCameraActor();
		virtual ~VSReflectCameraActor();
		DECLARE_INITIAL

		virtual void Update(double dAppTime);

		void SetTargetCameraActor(VSCameraActor * pCameraActor);
		void SetPlane(Plane Plane);
	protected:
		VSCameraActor * m_pTargetCameraActor;
		Plane m_Plane;
	};
	DECLARE_Ptr(VSReflectCameraActor);
	VSTYPE_MARCO(VSReflectCameraActor);
}
