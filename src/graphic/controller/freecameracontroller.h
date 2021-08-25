#pragma once
#include "math/maths.h"
#include "math/vector3.h"
#include "graphic/controller/controller.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSFreeCameraController : public VSController
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VSFreeCameraController();
		virtual ~VSFreeCameraController();
		float m_RotXDelta;
		float m_RotYDelta;
		float m_RotZDelta;
		float m_MoveDelta;
		Vector3 m_MoveDirection;
		virtual bool UpdateEx(double dAppTime);

	};
	DECLARE_Ptr(VSFreeCameraController);
	VSTYPE_MARCO(VSFreeCameraController);

}
