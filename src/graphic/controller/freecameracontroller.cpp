#include "graphic/controller/freecameracontroller.h"
#include "graphic/node/nodecomponent/camera.h"
#include "math/quat.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"


using namespace zq;
IMPLEMENT_RTTI(VSFreeCameraController,VSController)
BEGIN_ADD_PROPERTY(VSFreeCameraController,VSController)
REGISTER_PROPERTY(m_RotXDelta,RotXDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotYDelta,RotYDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RotZDelta,RotZDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveDelta,MoveDelta,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_MoveDirection,MoveDirection,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSFreeCameraController)
IMPLEMENT_INITIAL_END
VSFreeCameraController::VSFreeCameraController()
{
	m_RotXDelta = 1.0;
	m_RotYDelta = 1.0;
	m_RotZDelta = 1.0;
	m_MoveDelta = 1.0;
}
VSFreeCameraController:: ~VSFreeCameraController()
{

}
bool VSFreeCameraController::UpdateEx(double dAppTime)
{
	if(!VSController::UpdateEx(dAppTime))
		return 0;


	//VSFreeCamera * Temp = (VSFreeCamera *)m_pObject;

	VSCamera* Temp = DynamicCast<VSCamera>(m_pObject);
	VSMAC_ASSERT(Temp);
	Temp->m_RotX += m_RotXDelta * (float)m_dIntervalTime;
	Temp->m_RotY += m_RotYDelta * (float)m_dIntervalTime;
	Temp->m_RotZ += m_RotZDelta * (float)m_dIntervalTime;
	
	
	if (Temp->m_RotX > MATH_2PI) Temp->m_RotX -= MATH_2PI;
	else if (Temp->m_RotX < -MATH_2PI) Temp->m_RotX += MATH_2PI;

	if (Temp->m_RotY > MATH_2PI) Temp->m_RotY -= MATH_2PI;
	else if (Temp->m_RotY < -MATH_2PI) Temp->m_RotY += MATH_2PI;

	if (Temp->m_RotZ > MATH_2PI) Temp->m_RotZ -= MATH_2PI;
	else if (Temp->m_RotZ < -MATH_2PI) Temp->m_RotZ += MATH_2PI;
	
	Vector3 LocalPos = Temp->GetLocalTranslate();
 	LocalPos += m_MoveDirection * m_MoveDelta * (float)m_dIntervalTime;
			
	Quat    qFrame(0,0,0,1);	
	qFrame.CreateEuler(Temp->m_RotZ, Temp->m_RotX, Temp->m_RotY);

	Matrix3X3 Mat;
	Mat.Identity();
	qFrame.GetMatrix(Mat);

	Temp->SetLocalRotate(Mat);
	Temp->SetLocalTranslate(LocalPos);
	return 1;
}



