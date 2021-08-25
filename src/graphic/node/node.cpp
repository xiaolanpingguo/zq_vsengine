#include "graphic/node/node.h"
#include "math/maths.h"
#include "graphic/node/geometry.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/node/nodecomponent/nodecomponent.h"
using namespace zq;
IMPLEMENT_RTTI_NoCreateFun(VSNode, VSSpatial)
BEGIN_ADD_PROPERTY(VSNode,VSSpatial)
REGISTER_PROPERTY(m_pChild, Child, VSProperty::F_SAVE_LOAD_CLONE  | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
VSNode::VSNode()
{
	DeleteAllChild();

}
VSNode::~VSNode()
{
	m_pChild.Clear();
}

uint32 VSNode::AddChild(VSSpatial * pChild)
{
	VSMAC_ASSERT(pChild);
	for (uint32 i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		if (m_pChild[i] == pChild)
		{
			return i;
		}
	}
	pChild->SetParent(this);	
	m_pChild.AddElement(pChild);
	return m_pChild.GetNum() - 1;
}
uint32 VSNode::DeleteChild(VSSpatial *pChild)
{
	VSMAC_ASSERT(pChild);
	for(uint32 i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		
		if(m_pChild[i] == pChild)
		{
			
			pChild->SetParent(NULL);
			m_pChild.Erase(i);
			return i;
		}
	}

	return MATH_MAX_UINT32;
}
bool VSNode::DeleteChild(uint32 i)
{
	VSMAC_ASSERT(i < m_pChild.GetNum());
	VSSpatial * Temp = m_pChild[i];
	
	Temp->SetParent(NULL);
	m_pChild.Erase(i);


	return 1;
}

VSSpatial * VSNode::GetChild(uint32 i)const
{
	VSMAC_ASSERT(i < m_pChild.GetNum());
	return m_pChild[i];
}

void VSNode::DeleteAllChild()
{
	
	for(uint32 i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		m_pChild[i]->SetParent(NULL);
	}
	m_pChild.Clear();
}
void VSNode::CreateLocalAABB()
{
	for (uint32 i = 0; i < m_pChild.GetNum(); i++) 
	{ 	
		if(m_pChild[i])
		{
			m_pChild[i]->CreateLocalAABB();
		}

	} 

}
void VSNode::UpdateWorldBound(double dAppTime)
{
	bool bFoundFirstBound = false; 
	for (uint32 i = 0; i < m_pChild.GetNum(); i++) 
	{ 	
		if(m_pChild[i])
		{
			if(!bFoundFirstBound)
			{
				m_WorldBV = m_pChild[i]->m_WorldBV;
				bFoundFirstBound = true; 
			}
			else
			{

				m_WorldBV = m_WorldBV.MergeAABB(m_pChild[i]->m_WorldBV);
			}
		}

	} 
	if (!bFoundFirstBound)
	{
		float fA[3];
		m_WorldBV.GetfA(fA);
		m_WorldBV.Set(GetWorldTranslate(), fA);
	}
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;
	}
	
}
void VSNode::UpdateNodeAll(double dAppTime)
{
	
	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}
	

	UpdateTransform(dAppTime);
		

	for (uint32 i = 0; i < m_pChild.GetNum(); i++)	
	{ 
		if (m_pChild[i]) 
			m_pChild[i]->UpdateNodeAll(dAppTime); 
	} 


	if(m_bIsChanged)
	{
		UpdateWorldBound(dAppTime); 
	}
	m_bIsChanged = false;
	
}
void VSNode::ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{

	UpdateView(Culler,dAppTime);

	for (uint32 i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
		{

			m_pChild[i]->ComputeVisibleSet(Culler, bNoCull, dAppTime);

		}

	}
}

void VSNode::SetIsVisibleUpdate(bool bIsVisibleUpdate)
{
	for(uint32 i = 0 ; i < m_pChild.GetNum() ; i++)
	{

		m_pChild[i]->SetIsVisibleUpdate(bIsVisibleUpdate);

	}
	m_bIsVisibleUpdate = bIsVisibleUpdate;
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
}
