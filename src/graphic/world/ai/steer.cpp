#include "graphic/world/ai/steer.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI_NoCreateFun(VSSteerComponent,VSObject)
VSSteerComponent::VSSteerComponent()
{
	m_pSteer = NULL;
}
VSSteerComponent::~VSSteerComponent()
{
	m_pSteer = NULL;
}
IMPLEMENT_RTTI_NoCreateFun(VSSteer,VSObject)
VSSteer::VSSteer()
{
	m_pActor = NULL;
}
VSSteer::~VSSteer()
{
	m_pActor = NULL;
	m_ComponentArray.Clear();
}
Vector3 VSSteer::Compute()
{
	Vector3 Force;
	for (uint32 i = 0 ; i < m_ComponentArray.GetNum() ;i++)
	{
		if (!AddForce(Force,m_ComponentArray[i]->Compute() * m_ComponentArray[i]->m_fWeight))
		{
			return Force;
		}
	}
	return Force;
}
bool VSSteer::AddForce(Vector3 & CurForce,Vector3 & AddForce)
{
	float CurForceLen = CurForce.GetLength();
	float MaxDriverForce = GetOwner()->GetMaxDriverForce();
	float RemainForce = MaxDriverForce - CurForceLen;
	if (RemainForce <= 0.0f)
	{
		return false;
	}
	float AddForceLen = AddForce.GetLength();
	if (AddForceLen < RemainForce)
	{
		CurForce += AddForce;
	}
	else
	{
		AddForce.Normalize();
		CurForce += AddForce * RemainForce;
	}
	return true;
}