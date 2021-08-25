#pragma once
#include "math/maths.h"
#include "math/vector3.h"
#include "graphic/controller/controller.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VS1stCameraController : public VSController
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:
		VS1stCameraController();
		virtual ~VS1stCameraController();
		float m_RotXDelta;
		float m_RotYDelta;
		float m_MoveXDelta;
		float m_MoveYDelta;
		float m_MoveZDelta;
		virtual bool UpdateEx(double dAppTime);

	};
	DECLARE_Ptr(VS1stCameraController);
	VSTYPE_MARCO(VS1stCameraController);

}
