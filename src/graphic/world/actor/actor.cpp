#include "graphic/world/actor/actor.h"
#include "graphic/world/ai/aistate.h"
#include "graphic/world/ai/steer.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/world/world.h"


using namespace zq;
IMPLEMENT_RESOURCE(VSActor)
IMPLEMENT_RTTI(VSActor, VSObject)
BEGIN_ADD_PROPERTY(VSActor,VSObject)
REGISTER_PROPERTY(m_ChildActor, ChildActor, VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_pOwner, Owner, VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_pSceneMap, SceneMap, VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_ActorName, ActorName, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pNode, pNode, VSProperty::F_SAVE_LOAD_CLONE  | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pNodeComponentArray, NodeComponentArray, VSProperty::F_SAVE_LOAD_CLONE  | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSActor)
IMPLEMENT_INITIAL_END

VSActor::VSActor()
{
	m_pOwner = NULL;
	m_pNode = NULL;
	m_pSceneMap = NULL;
}
void VSActor::OnDestory()
{
	if (m_pSceneMap)
	{
		m_pSceneMap->DeleteActor(this);
	}
}
VSActor::~VSActor()
{
	m_pNode = NULL;
	m_pSceneMap = NULL;
}
void VSActor::ProcessInput(uint32 uiInputType,uint32 uiEvent,uint32 uiKey,int32 x, int32 y, int32 z)
{
	return;
}
void VSActor::SetWorldPos(const Vector3 & Pos)
{
	m_pNode->SetWorldTranslate(Pos);
}
void VSActor::SetWorldScale(const Vector3 &Scale)
{
	m_pNode->SetWorldScale(Scale);
}
void VSActor::SetWorldRotate(const Matrix3X3 & Rotate)
{
	m_pNode->SetWorldRotate(Rotate);
}
void VSActor::SetWorldTransform(const VSTransform & WorldTransform)
{
	m_pNode->SetWorldTransform(WorldTransform);
}
void VSActor::SetLocalTransform(const VSTransform & LocalTransform)
{
	m_pNode->SetLocalTransform(LocalTransform);
}
void VSActor::SetLocalPos(const Vector3 & Pos)
{

	m_pNode->SetLocalTranslate(Pos);

}
void VSActor::SetLocalScale(const Vector3 &Scale)
{
	
	m_pNode->SetLocalScale(Scale);

}
void VSActor::SetLocalRotate(const Matrix3X3 & Rotate)
{

	m_pNode->SetLocalRotate(Rotate);

}
const Vector3&  VSActor::GetWorldPos()
{

	return m_pNode->GetWorldTranslate();

}
const Vector3&  VSActor::GetWorldScale()
{

	return m_pNode->GetWorldScale();

}
const Matrix3X3&  VSActor::GetWorldRotate()
{

	return m_pNode->GetWorldRotate();

}
const VSTransform & VSActor::GetWorldTransform()
{
	return m_pNode->GetWorldTransform();
}
const Vector3&  VSActor::GetLocalPos()
{
	return m_pNode->GetLocalTranslate();
}
const Vector3&  VSActor::GetLocalScale()
{
	return m_pNode->GetLocalScale();
}
const VSTransform&  VSActor::GetLocalTransform()
{
	return m_pNode->GetLocalTransform();
}
const Matrix3X3&  VSActor::GetLocalRotate()
{
	return m_pNode->GetLocalRotate();
}

bool VSActor::HandleMessage(VSMessage & Message)
{
	if (m_pFSM)
	{
		m_pFSM->HandleMessage(Message);
	}
	return true;
}
void VSActor::Update(double dAppTime)
{

}
bool VSActor::PostClone(VSObject * pObjectSrc)
{
	VSResourceManager::AddGCObject(this);
	VSResourceManager::AddGCObject(m_pNode);
	for (uint32 i = 0; i < m_pNodeComponentArray.GetNum();i++)
	{
		VSResourceManager::AddGCObject(m_pNodeComponentArray[i]);
	}
	return true;
}
void VSActor::AddActorNodeToNode(VSActor * pActor, VSNodeComponent * pNode)
{
	bool Can = false;
	for (uint32 i = 0; i < m_pNodeComponentArray.GetNum(); i++)
	{
		if (m_pNodeComponentArray[i] == pNode)
		{
			Can = true;
			break;
		}
	}
	
	if (!Can)
	{
		return;
	}
	uint32 i = 0;
	for (; i < m_ChildActor.GetNum();i++)
	{
		if (pActor == m_ChildActor[i])
		{
			break;
		}
	}
	if (i == m_ChildActor.GetNum())
	{
		m_pSceneMap->AddActor(pActor);
		m_ChildActor.AddElement(pActor);
	}
	

	m_pSceneMap->GetScene()->DeleteObject(pActor->GetActorNode());
	pNode->AddChild(pActor->GetActorNode());
	pActor->m_pOwner = this;

}
void VSActor::AddChildActor(VSActor * pActor)
{
	if (pActor)
	{
		uint32 i = 0;
		for (; i < m_ChildActor.GetNum(); i++)
		{
			if (pActor == m_ChildActor[i])
			{
				break;
			}
		}
		if (i == m_ChildActor.GetNum())
		{
			m_pSceneMap->AddActor(pActor);
			m_ChildActor.AddElement(pActor);
		}
		VSNode * pNode = DynamicCast<VSNode>(m_pNode);
		m_pSceneMap->GetScene()->DeleteObject(pActor->GetActorNode());
		pNode->AddChild(pActor->GetActorNode());
		pActor->m_pOwner = this;

	}

}
void VSActor::DeleteChildActor(VSActor * pActor)
{
	for (uint32 i = 0; i < m_ChildActor.GetNum(); i++)
	{
		if (m_ChildActor[i] == pActor)
		{
			
			VSNode * pNode = DynamicCast<VSNode>(pActor->GetActorNode()->GetParent());
			pNode->DeleteChild(pActor->GetActorNode());
			pActor->m_pOwner = NULL;
			m_ChildActor.Erase(i);
			return;
		}
	}
}
VSActor * VSActor::GetChildActor(uint32 uiActorIndex)
{
	if (uiActorIndex < m_ChildActor.GetNum())
	{
		return m_ChildActor[uiActorIndex];
	}
	return NULL;
}
void VSActor::DeleteChildActor(uint32 uiActorIndex)
{
	if (uiActorIndex < m_ChildActor.GetNum())
	{
		VSNode * pNode = DynamicCast<VSNode>(m_ChildActor[uiActorIndex]->GetActorNode()->GetParent());
		pNode->DeleteChild(m_ChildActor[uiActorIndex]->GetActorNode());
		m_ChildActor[uiActorIndex]->m_pOwner = NULL;
		m_ChildActor.Erase(uiActorIndex);
	}
}
VSActor *VSActor::GetOwner()
{
	return m_pOwner;
}
void VSActor::CreateDefaultComponentNode()
{
	m_pNode = VSNodeComponent::CreateComponent<VSNodeComponent>();
}
void VSActor::AddToSceneMap(VSSceneMap * pSceneMap)
{
	m_pSceneMap = pSceneMap;
}
void VSActor::DeleteComponentNode(VSNodeComponent * pComponent)
{
	if (pComponent == m_pNode)
	{
		return;
	}

	VSNodeComponentPtr Temp = NULL;

	for (uint32 i = 0; i < m_pNodeComponentArray.GetNum(); i++)
	{
		if (m_pNodeComponentArray[i] == pComponent)
		{
			Temp = pComponent;
			m_pNodeComponentArray.Erase(i);
			break;
		}
	}
	if (Temp == NULL)
	{
		return;
	}
	pComponent->SetFlag(VSObject::OF_PendingKill);
	pComponent->OnDestroy();
	VSArray<VSSpatialPtr> ChildList = *pComponent->GetChildList();
	pComponent->DeleteAllChild();
	for (uint32 i = 0; i < ChildList.GetNum();i++)
	{
		VSNode *pNode = DynamicCast<VSNode>(pComponent->GetParent());
		pNode->AddChild(ChildList[i]);
	}
	
	
}
void VSActor::ChangeComponentNodeParent(VSNodeComponent * pSource, VSNode * pParent)
{
	if (pSource == m_pNode || pSource->GetParent() == pParent)
	{
		return;
	}
	if (m_pNodeComponentArray.FindElement(pSource) >= m_pNodeComponentArray.GetBufferNum())
	{
		return;
	}

	if (!pParent)
	{
		pParent = m_pNode;
	}

	if (pParent != m_pNode)
	{
		VSNodeComponent * pParentNode = DynamicCast<VSNodeComponent>(pParent);
		if (pParentNode)
		{
			if (m_pNodeComponentArray.FindElement(pParentNode) >= m_pNodeComponentArray.GetBufferNum())
			{
				return;
			}
		}
	}

	VSNode * pParentNode = DynamicCast<VSNode>(pSource->GetParent());
	VSSpatialPtr pTemp = pSource;
	pParentNode->DeleteChild(pSource);
	pParent->AddChild(pSource);
}






