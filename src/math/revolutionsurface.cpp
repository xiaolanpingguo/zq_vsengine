#include "math/revolutionsurface.h"


namespace zq{


RevolutionSurface::RevolutionSurface()
{
	m_pCurve = NULL;

}
/*----------------------------------------------------------------*/
RevolutionSurface::~RevolutionSurface()
{
	m_pCurve = NULL;

}
/*----------------------------------------------------------------*/
bool RevolutionSurface::SetCurve(Curve * pCurve,uint32 Axis,uint32 uiPlane)
{
	if(!pCurve)
		return 0;
	m_pCurve = pCurve;
	m_uiAxis = Axis;
	m_uiPlane = uiPlane;
	return 1;
}
/*----------------------------------------------------------------*/
Vector3 RevolutionSurface::GetPoint(float U,float V)
{
	Vector3 Temp = m_pCurve->GetPoint(U);
	if(m_uiAxis == XAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return Vector3(Temp.x,Temp.y * COS(MATH_2PI * V),Temp.y *SIN(MATH_2PI * V));
		}
		else if(m_uiPlane == XZPLANE)
		{
			return Vector3(Temp.x,Temp.z * COS(MATH_2PI * V),Temp.z *SIN(MATH_2PI * V));
		}
		else
			return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
	else if(m_uiAxis == YAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return Vector3(Temp.x * COS(MATH_2PI * V),Temp.y ,Temp.x *SIN(MATH_2PI * V));
		}
		else if(m_uiPlane == YZPLANE)
		{
			return Vector3(Temp.z * COS(MATH_2PI * V),Temp.y ,Temp.z *SIN(MATH_2PI * V));
		}
		else
			return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
	else if(m_uiAxis == ZAXIS)
	{
		if(m_uiPlane == XZPLANE)
		{
			return Vector3(Temp.x * COS(MATH_2PI * V),Temp.x * SIN(MATH_2PI * V),Temp.z);
		}
		else if(m_uiPlane == YZPLANE)
		{
			return Vector3(Temp.y * COS(MATH_2PI * V),Temp.y * SIN(MATH_2PI * V),Temp.z );
		}
		else
			return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
}
/*----------------------------------------------------------------*/
Vector3 RevolutionSurface::GetUPartialDerivative(float U,float V)
{
	Vector3 Temp = m_pCurve->GetFirstDerivative(U);
	if(m_uiAxis == XAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return Vector3(Temp.x,Temp.y * COS(MATH_2PI * V),Temp.y *SIN(MATH_2PI * V));
		}
		else if(m_uiPlane == XZPLANE)
		{
			return Vector3(Temp.x,Temp.z * COS(MATH_2PI * V),Temp.z *SIN(MATH_2PI * V));
		}
		else
			return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
	else if(m_uiAxis == YAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return Vector3(Temp.x * COS(MATH_2PI * V),Temp.y ,Temp.x *SIN(MATH_2PI * V));
		}
		else if(m_uiPlane == YZPLANE)
		{
			return Vector3(Temp.z * COS(MATH_2PI * V),Temp.y ,Temp.z *SIN(MATH_2PI * V));
		}
		else
			return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
	else if(m_uiAxis == ZAXIS)
	{
		if(m_uiPlane == XZPLANE)
		{
			return Vector3(Temp.x * COS(MATH_2PI * V),Temp.x * SIN(MATH_2PI * V),Temp.z);
		}
		else if(m_uiPlane == YZPLANE)
		{
			return Vector3(Temp.y * COS(MATH_2PI * V),Temp.y * SIN(MATH_2PI * V),Temp.z );
		}
		else
			return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
}
/*----------------------------------------------------------------*/
Vector3 RevolutionSurface::GetVPartialDerivative(float U,float V)
{
	Vector3 Temp = m_pCurve->GetPoint(U);
	if(m_uiAxis == XAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return Vector3(Temp.x,Temp.y * -COS(MATH_2PI * V),Temp.y *COS(MATH_2PI * V));
		}
		else if(m_uiPlane == XZPLANE)
		{
			return Vector3(Temp.x,Temp.z * -COS(MATH_2PI * V),Temp.z *COS(MATH_2PI * V));
		}
		else
			return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
	else if(m_uiAxis == YAXIS)
	{
		if(m_uiPlane == XYPLANE)
		{
			return Vector3(Temp.x * -COS(MATH_2PI * V),Temp.y ,Temp.x *COS(MATH_2PI * V));
		}
		else if(m_uiPlane == YZPLANE)
		{
			return Vector3(Temp.z * -COS(MATH_2PI * V),Temp.y ,Temp.z *COS(MATH_2PI * V));
		}
		else
			return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
	else if(m_uiAxis == ZAXIS)
	{
		if(m_uiPlane == XZPLANE)
		{
			return Vector3(Temp.x * -COS(MATH_2PI * V),Temp.x * COS(MATH_2PI * V),Temp.z);
		}
		else if(m_uiPlane == YZPLANE)
		{
			return Vector3(Temp.y * -COS(MATH_2PI * V),Temp.y * COS(MATH_2PI * V),Temp.z );
		}
		else
			return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}
	else
	{
		return Vector3(MATH_MAX_FLOAT,MATH_MAX_FLOAT,MATH_MAX_FLOAT);
	}

}

}