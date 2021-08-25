#pragma once
#include "graphic/world/actor/actor.h"
namespace zq
{
	
	class VSSteerComponent : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
		friend class VSSteer;
	public:
		VSSteerComponent();
		virtual ~VSSteerComponent() = 0;
		inline VSSteer * GetOwner()const
		{
			return m_pSteer;
		}
		//º∆À„∫œ¡¶
		virtual Vector3 Compute() = 0;
	protected:
		VSSteer * m_pSteer;
		uint32 m_uiPrior;
		float		 m_fWeight;
	};
	DECLARE_Ptr(VSSteerComponent);

	class VSSteer : public VSObject
	{
		//RTTI
		DECLARE_RTTI;
	public:
		friend class VSActor;
		VSSteer();
		virtual	~VSSteer();
		inline VSActor * GetOwner()const
		{
			return m_pActor;
		}
		
		template<class T>
		T * GetSteerComponent();
		template<class T>
		void AddSteerComponent(uint32 uiPrior ,float fWeight);

		template<class T>
		void DeleteSteerComponent();
		virtual Vector3 Compute();
	protected:
		VSActor * m_pActor;
		VSArray<VSSteerComponentPtr> m_ComponentArray;
		bool AddForce(Vector3 & CurForce,Vector3 & AddForce);
		class PriorityCompare
		{
		public:
			inline bool operator()(VSSteerComponentPtr & e1,VSSteerComponentPtr& e2)
			{
				
				return e1->m_uiPrior <= e2->m_uiPrior;
			}

		};
		
	};
	DECLARE_Ptr(VSSteer);
	template<class T>
	void VSSteer::AddSteerComponent(uint32 uiPrior ,float fWeight)
	{
		if (!T::ms_Type.IsDerived(VSSteerComponent::ms_Type))
		{
			return ;
		}
		for (uint32 i = 0 ;i < m_ComponentArray.GetNum() ; i++)
		{
			if(T::ms_Type.IsSameType(m_ComponentArray[i]->GetType()))
			{
				continue;
			}
		}
		T * pComponent = MEM_NEW T();
		VSSteerComponent * pTemp = StaticCast<VSSteerComponent>(pComponent);
		pTemp->m_pSteer = this;
		pTemp->m_fWeight = fWeight;
		pTemp->m_uiPrior = uiPrior;
		m_ComponentArray.AddElement(pComponent);
		m_ComponentArray.SortAll(PriorityCompare())
	}
	template<class T>
	T * VSSteer::GetSteerComponent()
	{
		for (uint32 i = 0 ;i < m_ComponentArray.GetNum() ; i++)
		{
			if(T::ms_Type.IsSameType(m_ComponentArray[i]->GetType()))
			{
				return StaticCast<T>(m_ComponentArray[i]);
			}
		}
	}

	template<class T>
	void VSSteer::DeleteSteerComponent()
	{
		for (uint32 i = 0 ;i < m_ComponentArray.GetNum() ; i++)
		{
			if(T::ms_Type.IsSameType(m_ComponentArray[i]->GetType()))
			{
				m_ComponentArray[i] = NULL;
				m_ComponentArray.Erase(i);
				return ;
			}
		}
	}
}
