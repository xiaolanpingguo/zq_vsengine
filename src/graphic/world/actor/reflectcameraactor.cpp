#include "graphic/world/actor/reflectcameraactor.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/world/world.h"
#include "graphic/render/scenemanager/viewfamily.h"
#include "math/line.h"


using namespace zq;
IMPLEMENT_RTTI(VSReflectCameraActor, VSActor)
BEGIN_ADD_PROPERTY(VSReflectCameraActor, VSActor)
REGISTER_PROPERTY(m_pTargetCameraActor, TargetCameraActor, VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSReflectCameraActor)
IMPLEMENT_INITIAL_END
VSReflectCameraActor::VSReflectCameraActor()
{

}
VSReflectCameraActor::~VSReflectCameraActor()
{

}
void VSReflectCameraActor::Update(double dAppTime)
{
	VSCameraActor::Update(dAppTime);

	if (m_pTargetCameraActor)
	{
		VSCamera * pTargetCamera = m_pTargetCameraActor->GetTypeNode();
		VSCamera * pCamera = GetTypeNode();
		if (pCamera && pTargetCamera)
		{
			pCamera->SetPerspectiveFov(pTargetCamera->GetFov(), pTargetCamera->GetAspect(), pTargetCamera->GetZNear(), pTargetCamera->GetZFar());

			Vector3 TargetCameraWorldPos =  pTargetCamera->GetWorldTranslate();

			Vector3 N = m_Plane.GetN();

			Line Line1(TargetCameraWorldPos, N);

			float t1 = 0;
			m_Plane.RelationWith(Line1, false, t1);

			Vector3 k1 = Line1.GetParameterPoint(t1);

			Vector3 CameraWorldPos = k1 + k1 - TargetCameraWorldPos;

			Vector3 Dir, Up, Right;
			pTargetCamera->GetWorldDir(Dir, Up, Right);


			Vector3 CameraLookDir = m_Plane.ReflectDir(Dir);
			CameraLookDir.Normalize();

			Vector3 CameraUp = m_Plane.ReflectDir(Up);
			CameraUp.Normalize();
			
			pCamera->CreateFromLookDirWorld(CameraWorldPos, CameraLookDir, CameraUp);
		}
	}
	
}
void VSReflectCameraActor::SetTargetCameraActor(VSCameraActor * pCameraActor)
{
	m_pTargetCameraActor = DynamicCast<VSReflectCameraActor>(pCameraActor);
	if (m_pTargetCameraActor == NULL)
	{
		m_pTargetCameraActor = pCameraActor;
	}
}
void VSReflectCameraActor::SetPlane(Plane Plane)
{
	m_Plane = Plane;
	AddCustomCullPlane(m_Plane);
}