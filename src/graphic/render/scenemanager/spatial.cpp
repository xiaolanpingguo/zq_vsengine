#include "graphic/render/scenemanager/spatial.h"
#include "graphic/controller/controller.h"
#include "graphic/render/renderer.h"
#include "graphic/node/geometry.h"
#include "graphic/node/node.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/material/material.h"
using namespace zq;
IMPLEMENT_RTTI_NoCreateFun(VSSpatial,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSSpatial)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSSpatial, VSObject)
REGISTER_PROPERTY(m_WorldBV,WorldBV,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiCullMode,CullMode,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_World, WorldTransform, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_Local, LocalTransform, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_pParent, Parent, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bInheritScale, InheritScale, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_bInheritRotate, InheritRotate, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_bInheritTranslate, InheritTranslate, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_bEnable, Enable, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME);
REGISTER_PROPERTY(m_ControllerArray, ControllerArray, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME );
REGISTER_PROPERTY(m_bIsStatic, IsStatic, VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_bIsChanged, IsChanged, VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSSpatial::VSSpatial()
{
	

	m_uiCullMode = CM_DYNAMIC;

	m_bIsVisibleUpdate = false;

	//m_bForceNoCull = false;
	m_pParent = NULL;
	m_bInheritScale = true;
	m_bInheritRotate = true;
	m_bInheritTranslate = true;
	m_ControllerArray.Clear();
	m_bIsStatic = 1;
	m_bIsChanged = true;
	m_bEnable = true;
}
VSSpatial::~VSSpatial()
{
	m_pParent = NULL;

	DeleteAllController();
}

void VSSpatial::UpdateAll(double dAppTime)
{
	UpdateNodeAll(dAppTime);

	if (m_bIsVisibleUpdate)
	{
		m_bEnable = false;
	}

}
void VSSpatial::ComputeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
	if(m_uiCullMode == CM_ALAWAYS)
		return ;
	if(m_uiCullMode == CM_NEVER)
		bNoCull = true;

	uint32 uiSavePlaneState = Culler.GetPlaneState();
	if(bNoCull)
	{
		ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);
		if (m_bIsVisibleUpdate)
		{
			m_bEnable = true;
		}
	}
	else
	{
		uint32 uiVSF = Culler.IsVisible(m_WorldBV);
		if (uiVSF == VSCuller::VSF_ALL)
		{
			bNoCull = true;
			ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);
			if (m_bIsVisibleUpdate)
			{
				m_bEnable = true;
			}
		}
		else if (uiVSF == VSCuller::VSF_PARTIAL || Culler.ForceNoCull(this))
		{
			ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);	
			if (m_bIsVisibleUpdate)
			{
				m_bEnable = true;
			}
		}

	}
	Culler.SetPlaneState(uiSavePlaneState);

}
void VSSpatial::UpdateView(VSCuller & Culler,double dAppTime)
{
}
void VSSpatial::SetIsVisibleUpdate(bool bIsVisibleUpdate)
{
	m_bIsVisibleUpdate = bIsVisibleUpdate;
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
}

void VSSpatial::UpdateTransform(double dAppTime)
{
	if (m_pParent)
	{
		if (!m_pParent->m_bIsStatic)
		{
			m_bIsStatic = 0;
		}
		if (m_pParent->m_bIsChanged)
		{
			m_bIsChanged = true;
		}
	}
	if (m_bIsChanged)
	{
		if (m_pParent)
		{
			uint32 TransformFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranslate << 2);
			m_World.Product(m_Local, m_pParent->m_World, TransformFlag);

		}
		else
			m_World = m_Local;
	}
	
	/*updateNodeController*/
}

void VSSpatial::GetWorldDir(Vector3 &Dir, Vector3 &Up, Vector3 & Right)
{
	m_World.GetDir(Dir, Up, Right);
}
const Vector3 & VSSpatial::GetWorldScale()
{
	return m_World.GetScale();
}
const Vector3 & VSSpatial::GetWorldTranslate()
{
	return m_World.GetTranslate();
}
const Matrix3X3 & VSSpatial::GetWorldRotate()
{
	return m_World.GetRotate();
}
const VSTransform & VSSpatial::GetWorldTransform()
{
	m_World.GetCombine();
	return m_World;
}
void VSSpatial::SetLocalTransform(const VSTransform & LocalTransform)
{
	m_bIsChanged = true;
	m_Local = LocalTransform;
	UpdateAll(0.0f);
}
void VSSpatial::SetLocalMat(const Matrix3X3W VSMat)
{
	m_bIsChanged = true;
	m_Local.SetMatrix(VSMat);
	UpdateAll(0.0f);
}
void VSSpatial::SetLocalScale(const Vector3 & fScale)
{
	m_bIsChanged = true;
	m_Local.SetScale(fScale);
	UpdateAll(0.0f);

}
void VSSpatial::SetLocalTranslate(const Vector3& Translate)
{
	m_bIsChanged = true;
	m_Local.SetTranslate(Translate);
	UpdateAll(0.0f);

}
void VSSpatial::SetLocalRotate(const Matrix3X3 & Rotate)
{
	m_bIsChanged = true;
	m_Local.SetRotate(Rotate);
	UpdateAll(0.0f);

}
void VSSpatial::SetWorldScale(const Vector3 & fScale)
{
	VSSpatial * pParent = GetParent();
	if (pParent)
	{
		VSTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		VSTransform NewWorld;
		NewWorld = GetWorldTransform();
		NewWorld.SetScale(fScale);
		VSTransform NewLocal;
		uint32 TransformFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranslate << 2);
		NewLocal.Product(NewWorld, Inv, TransformFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalScale(fScale);
	}
}
void VSSpatial::SetWorldTranslate(const Vector3& Translate)
{
	m_bIsChanged = true;

	VSSpatial * pParent = GetParent();
	if (pParent)
	{
		VSTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		VSTransform NewWorld;
		NewWorld = GetWorldTransform();
		NewWorld.SetTranslate(Translate);
		VSTransform NewLocal;
		uint32 TransformFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranslate << 2);
		NewLocal.Product(NewWorld, Inv, TransformFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalTranslate(Translate);
	}
}
void VSSpatial::SetWorldRotate(const Matrix3X3 & Rotate)
{
	VSSpatial * pParent = GetParent();
	if (pParent)
	{
		VSTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		VSTransform NewWorld;
		NewWorld = GetWorldTransform();
		NewWorld.SetRotate(Rotate);
		VSTransform NewLocal;
		uint32 TransformFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranslate << 2);
		NewLocal.Product(NewWorld, Inv, TransformFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalRotate(Rotate);
	}
}
void VSSpatial::SetWorldTransform(const VSTransform & LocalTransform)
{
	VSSpatial * pParent = GetParent();
	if (pParent)
	{
		VSTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);

		VSTransform NewWorld;
		NewWorld = LocalTransform;

		VSTransform NewLocal;
		uint32 TransformFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranslate << 2);
		NewLocal.Product(NewWorld, Inv, TransformFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalTransform(LocalTransform);
	}
}
void VSSpatial::SetWorldMat(const Matrix3X3W VSMat)
{
	VSSpatial * pParent = GetParent();
	if (pParent)
	{
		VSTransform Inv;
		pParent->GetWorldTransform().Inverse(Inv);
		VSTransform NewWorld;
		NewWorld.SetMatrix(VSMat);
		VSTransform NewLocal;
		uint32 TransformFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranslate << 2);
		NewLocal.Product(NewWorld, Inv, TransformFlag);
		SetLocalTransform(NewLocal);
	}
	else
	{
		SetLocalMat(VSMat);
	}
}

void VSSpatial::SetDynamic(bool bIsDynamic)
{
	if (!m_ControllerArray.GetNum())
	{
		m_bIsStatic = !bIsDynamic;
	}
}

bool VSSpatial::AddController(VSController * pController)
{
	if (!pController)
		return false;
	for (uint32 i = 0; i < m_ControllerArray.GetNum(); i++)
	{
		if (m_ControllerArray[i] == pController)
			return 1;
	}
	if (!pController->SetObject(this))
		return false;
	m_ControllerArray.AddElement(pController);

	m_bIsStatic = 0;
	return 1;
}

VSController * VSSpatial::DeleteController(uint32 ID)
{
	if (ID >= m_ControllerArray.GetNum())
		return NULL;
	VSController * Temp = m_ControllerArray[ID];
	m_ControllerArray.Erase(ID);

	Temp->SetObject(NULL);
	return Temp;
}

uint32 VSSpatial::DeleteController(VSController *pController)
{
	if (!pController)
		return MATH_MAX_UINT32;
	for (uint32 i = 0; i < m_ControllerArray.GetNum(); i++)
	{

		if (m_ControllerArray[i] == pController)
		{
			pController->SetObject(NULL);
			m_ControllerArray.Erase(i);

			return i;
		}
	}

	return MATH_MAX_UINT32;
}

void VSSpatial::DeleteAllController()
{
	m_ControllerArray.Clear();
}

VSController *VSSpatial::GetController(uint32 ID)const
{
	if (ID >= m_ControllerArray.GetNum())
		return NULL;
	return m_ControllerArray[ID];
}

void VSSpatial::UpdateController(double dAppTime)
{

	for (uint32 i = 0; i < m_ControllerArray.GetNum(); i++)
	{
		if (m_ControllerArray[i]->m_bIsVisibleUpdate)
		{
			if (!m_bEnable)
			{
				continue;
			}
		}
		m_ControllerArray[i]->SetObject(this);
		m_ControllerArray[i]->Update(dAppTime);

	}

}

void VSSpatial::ClearAllTime()
{
	for (uint32 i = 0; i < m_ControllerArray.GetNum(); i++)
	{
		m_ControllerArray[i]->ClearTime();
	}
}
