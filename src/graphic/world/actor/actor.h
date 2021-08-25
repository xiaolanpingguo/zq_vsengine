#pragma once


#include "graphic/world/ai/logic.h"
#include "graphic/core/object.h"
#include "math/vector3.h"
#include "math/quat.h"
#include "graphic/node/node.h"
#include "graphic/node/nodecomponent/nodecomponent.h"


namespace zq{


#define GET_TYPE_NODE(nodeclass)\
	friend class nodeclass;\
	nodeclass * GetTypeNode()const\
	{\
		VSNode * pNode = m_pNode; \
		return(nodeclass*)pNode;\
	}


DECLARE_Ptr(VSFSM);
DECLARE_Ptr(VSSteer);
class VSSceneMap;

class GRAPHIC_API VSActor : public VSObject,public VSResource
{
	//RTTI
	DECLARE_RTTI;

public:
	VSActor();
	virtual ~VSActor();

	DECLARE_INITIAL
public:
	virtual void SetWorldPos(const Vector3 & Pos);
	virtual void SetWorldScale(const Vector3 &Scale);
	virtual void SetWorldRotate(const Matrix3X3 & Rotate);
	virtual void SetWorldTransform(const VSTransform & WorldTransform);
	virtual void SetLocalPos(const Vector3 & Pos);
	virtual void SetLocalScale(const Vector3 &Scale);
	virtual void SetLocalRotate(const Matrix3X3 & Rotate);
	virtual void SetLocalTransform(const VSTransform & LocalTransform);

	const Vector3& GetWorldPos();
	const Vector3& GetWorldScale();
	const Matrix3X3& GetWorldRotate();
	const VSTransform & GetWorldTransform();

	const Vector3&  GetLocalPos();
	const Vector3&  GetLocalScale();
	const Matrix3X3&  GetLocalRotate();
	const VSTransform&  GetLocalTransform();
		
	virtual void ProcessInput(uint32 uiInputType,uint32 uiEvent,uint32 uiKey,int32 x, int32 y, int32 z);
		
	virtual bool HandleMessage(VSMessage & Message);



	VSNode* GetActorNode()const
	{
		return m_pNode;
	}
		
	Vector3 GetVelocity()const
	{
		return m_Velocity;
	}
	float GetMaxVelocity()const
	{
		return m_fMaxVelocity;
	}
	float GetMaxDriverForce()const
	{
		return m_fMaxDriverForce;
	}

	virtual void Update(double dAppTime);
	virtual bool PostClone(VSObject * pObjectSrc);
	friend class VSWorld;
	friend class VSSceneMap;
protected:
	Vector3	m_Velocity;
	float		m_fMaxVelocity;
	float		m_fMaxDriverForce;

	VSNodePtr		m_pNode;
	VSFSMPtr		m_pFSM;
	VSSteerPtr		m_pSteerPtr;
		
	VSActor *		m_pOwner;
public:
	VSUsedName		m_ActorName;
	VSSceneMap *	m_pSceneMap;

public:
	virtual void OnDestory();
	VSActor *GetOwner();
	virtual void AddChildActor(VSActor * pActor);
	virtual void DeleteChildActor(VSActor * pActor);
	virtual VSActor * GetChildActor(uint32 uiActorIndex);
	virtual void DeleteChildActor(uint32 uiActorIndex);
	template<typename T>
	T * AddComponentNode(VSNodeComponent* pParent = NULL);
	template<typename T>
	void GetComponentNode(VSArray<T*>& Node);
	void DeleteComponentNode(VSNodeComponent * pComponent);
	void ChangeComponentNodeParent(VSNodeComponent * pSource, VSNode * pParent = NULL);
	void AddActorNodeToNode(VSActor * pActor, VSNodeComponent * pNode);
protected:
	virtual void AddToSceneMap(VSSceneMap * pSceneMap);
	virtual void CreateDefaultComponentNode();

protected:
	VSArray<VSActor *> m_ChildActor;
		
	VSArray<VSNodeComponentPtr> m_pNodeComponentArray; // not include root node
	DECLARE_RESOURCE(VSActor, RA_ASYN_LOAD | RA_ENABLE_GC, RT_ACTOR,ACTOR, Resource/Actor, VSCacheResource)
};
DECLARE_Ptr(VSActor);
VSTYPE_MARCO(VSActor);
DECLARE_Proxy(VSActor);
template<typename T>
T * VSActor::AddComponentNode(VSNodeComponent* pParent)
{
	T * pNode = VSNodeComponent::CreateComponent<T>();
	if (pParent == NULL)
	{
		m_pNode->AddChild(pNode);
	}	
	m_pNodeComponentArray.AddElement(pNode);
	return pNode;
}
template<typename T>
void VSActor::GetComponentNode(VSArray<T*>& Node)
{
	for (uint32 i = 0; i < m_pNodeComponentArray.GetNum();i++)
	{
		if (m_pNodeComponentArray[i]->GetType().IsSameType(T::GetType()))
		{
			Node.AddElement(m_pNodeComponentArray[i]);
		}
	}
}


}
