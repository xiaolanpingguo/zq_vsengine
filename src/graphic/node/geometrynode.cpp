#include "graphic/node/geometrynode.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSGeometryNode,VSNode)
BEGIN_ADD_PROPERTY(VSGeometryNode,VSNode)
REGISTER_PROPERTY(m_pMorphSet, MorphSet, VSProperty::F_SAVE_LOAD_COPY | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_fLODScreenSize, LODScreenSize, VSProperty::F_SAVE_LOAD_COPY | VSProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGeometryNode)
IMPLEMENT_INITIAL_END
VSGeometryNode::VSGeometryNode()
{
}
VSGeometryNode::~VSGeometryNode()
{
	if (m_pMorphSet)
	{
		m_pMorphSet->m_AddMorphEvent.RemoveMethod<VSGeometryNode, &VSGeometryNode::UpdateLocalAABB>(&(*this));
	}
}

VSGeometry * VSGeometryNode::GetGeometry(uint32 i)const
{
	if (i >= m_pChild.GetNum())
	{
		return NULL;
	}
	else
	{
		VSGeometry * pGeometry= DynamicCast<VSGeometry>(m_pChild[i]);
		return pGeometry;
	}
}
uint32 VSGeometryNode::GetNormalGeometryNum()const
{
	uint32 k = 0;

	for (uint32 i =0 ; i < m_pChild.GetNum() ;i++)
	{
		VSGeometry * pGeometry= DynamicCast<VSGeometry>(m_pChild[i]);
		if (pGeometry && pGeometry->GetGeometryUseType() == VSGeometry::GUT_NORMAL)
		{
			
			k++;
			

		}

	}
	return k ;
}
VSGeometry * VSGeometryNode::GetNormalGeometry(uint32 index)const
{
	uint32 k = 0;
	for (uint32 i =0 ; i < m_pChild.GetNum() ;i++)
	{
		VSGeometry * pGeometry= DynamicCast<VSGeometry>(m_pChild[i]);
		if (pGeometry && pGeometry->GetGeometryUseType() == VSGeometry::GUT_NORMAL)
		{
			if (k == index)
			{
				return pGeometry;
			}
			else
			{
				k++;
			}
			
		}
		
	}
	return NULL;
}
void VSGeometryNode::SetMorphSet(VSMorphSet * pMorphSet)
{
	if (!pMorphSet)
	{
		m_pMorphSet = NULL;
		return;
	}
	else
	{
		if (GetNormalGeometryNum() != pMorphSet->GetBufferNum())
		{
			return;
		}
		for (uint32 i = 0; i < pMorphSet->GetMorphNum(); i++)
		{
			for (uint32 j = 0; j < pMorphSet->GetBufferNum(); j++)
			{
				uint32 VertexNum = pMorphSet->GetMorph(i)->GetVertexNum(j);
				VSGeometry * pGeometry = (VSGeometry *)GetNormalGeometry(j);
				if (VertexNum && pGeometry)
				{
					if (VertexNum != pGeometry->GetVertexNum())
					{
						return;
					}
				}
			}

		}

	}

	if (m_pMorphSet)
	{
		m_pMorphSet->m_AddMorphEvent.RemoveMethod<VSGeometryNode, &VSGeometryNode::UpdateLocalAABB>(&(*this));
	}
	m_pMorphSet = pMorphSet;
	m_pMorphSet->m_AddMorphEvent.AddMethod<VSGeometryNode, &VSGeometryNode::UpdateLocalAABB>(&(*this));
	UpdateLocalAABB();
}
void VSGeometryNode::UpdateLocalAABB()
{
	if (!m_pMorphSet)
	{
		return;
	}
	for (uint32 j = 0; j < GetNormalGeometryNum(); j++)
	{
		VSGeometry * NormalGeometry = GetNormalGeometry(j);
		NormalGeometry->CreateLocalAABB();
	}
	for (uint32 i = 0; i < m_pMorphSet->GetMorphNum();i++)
	{
		VSMorph *  pMorph = m_pMorphSet->GetMorph(i);
		for (uint32 j = 0; j < GetNormalGeometryNum(); j++)
		{
			VSGeometry * NormalGeometry = GetNormalGeometry(j);
			NormalGeometry->AddMorphAABB(pMorph->GetBuffer(j));
		}
	}
}
bool VSGeometryNode::PostLoad(VSStream* pStream)
{
	if (VSNode::PostLoad(pStream) == false)
	{
		return false;
	}
	if (m_pMorphSet && m_pMorphSet->GetMorphNum() > 0)
	{
		for (uint32 j = 0; j < GetNormalGeometryNum(); j++)
		{
			VSGeometry * NormalGeometry = GetNormalGeometry(j);
			VSMap<unsigned int, VSVertexBuffer *> MorphDataSet;
			for (uint32 i = 0; i < m_pMorphSet->GetMorphNum(); i++)
			{
				VSMorph *  pMorph = m_pMorphSet->GetMorph(i);
				VSVertexBuffer * pVertexBuffer = pMorph->GetBuffer(j);
				MorphDataSet.AddElement(i, pVertexBuffer);
			}
			NormalGeometry->CreateMorphMeshData(MorphDataSet);
		}		
	}
	return true;
}
bool VSGeometryNode::PostClone(VSObject * pObjectSrc)
{
	if (VSNode::PostClone(pObjectSrc) == false)
	{
		return false;
	}
	return true;
}
void VSGeometryNode::SetMorphData(uint32 Index, float fData)
{
	if (!m_pMorphSet)
	{
		return;
	}
	if (Index >= m_pMorphSet->GetMorphNum())
	{
		return;
	}
	fData = Clamp(fData, 1.0f, -1.0f);
	for (uint32 j = 0; j < GetNormalGeometryNum(); j++)
	{
		VSGeometry * NormalGeometry = GetNormalGeometry(j);
		NormalGeometry->SetMorphData(Index, fData);
	}
}
void VSGeometryNode::SetMorphData(const VSUsedName & ShowName, float fData)
{
	if (!m_pMorphSet)
	{
		return;
	}
	uint32 Index = m_pMorphSet->GetMorphIndex(ShowName);
	SetMorphData(Index, fData);
}
void VSGeometryNode::GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation)const
{
	for (uint32 j = 0; j < GetNormalGeometryNum(); j++)
	{
		VSGeometry* NormalGeometry = GetNormalGeometry(j);
		NormalGeometry->GetStreamResource(pResourceProxy, StreamInformation);
	}
}