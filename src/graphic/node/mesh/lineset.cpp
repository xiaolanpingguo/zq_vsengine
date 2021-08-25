#include "graphic/node/mesh/lineset.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSLineSet,VSMeshData)
BEGIN_ADD_PROPERTY(VSLineSet,VSMeshData)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSLineSet)
IMPLEMENT_INITIAL_END
VSLineSet::VSLineSet()
{
	m_uiMeshDataType = MDT_LINE;
}
VSLineSet::~VSLineSet()
{

}
bool VSLineSet::CreateIndex(uint32 uiLineType)
{
	VSMAC_ASSERT(m_pVertexBuffer);
	uint32 Num = m_pVertexBuffer->GetVertexNum();
	VSMAC_ASSERT(Num >= 2);
	if(uiLineType == LT_OPEN)
	{
		Num = (Num - 1) * 2;

	}
	else if(uiLineType == LT_CLOSE)
	{
		Num = Num * 2;
	}
	else if(uiLineType == LT_SEGMENT)
	{
		if(Num % 2)
		{
			return 0;
		}

	}
	else
		VSMAC_ASSERT(0);
	VSDataBuffer * pIndex = NULL;
	pIndex = new VSDataBuffer();
	VSMAC_ASSERT(pIndex);
	
	
	if(!pIndex->CreateEmptyBuffer(Num,VSDataBuffer::DT_USHORT))
		return 0;

	MATH_USHORT_INDEX * pIndexBuffer = (MATH_USHORT_INDEX *)pIndex->GetData();
	VSMAC_ASSERT(pIndexBuffer);

	if(uiLineType == LT_OPEN)
	{
		for(uint32 i = 0 ; i < Num / 2 ; i++)
		{
			pIndexBuffer[2 * i] = i;
			pIndexBuffer[2 * i + 1] = i + 1;
		}
	}
	else if(uiLineType == LT_CLOSE)
	{	
		uint32 i;
		for( i = 0; i < Num / 2 ; i++)
		{
			pIndexBuffer[2 * i] = i;
			pIndexBuffer[2 * i + 1] = i + 1;
		}
		//i已经递增，最后一个索引应该是0，所以修改 2 * ( i - 1) + 1 =  2 * i - 1
		pIndexBuffer[2 * i - 1] = 0;
	}
	else if(uiLineType == LT_SEGMENT)
	{
		for(uint32 i = 0 ; i < Num ; i++)
		{
			pIndexBuffer[i] = i;

		}
	}
	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = MEM_NEW VSIndexBuffer();
		VSMAC_ASSERT(m_pIndexBuffer);
	}

	m_pIndexBuffer->SetData(pIndex);
	
	return 1;

}
uint32 VSLineSet::GetTotalNum()const
{	
	if (!m_pIndexBuffer && !m_pVertexBuffer)
	{
		return 0;
	}
	else if(!m_pIndexBuffer)
	{
		return m_pVertexBuffer->GetVertexNum() / 2;
	}
	else
		return m_pIndexBuffer->GetNum() / 2;
}
uint32 VSLineSet::GetGirdNum(uint32 uiInputNum)const
{
	return uiInputNum / 2;
}
uint32 VSLineSet::GetActiveNum()const
{
	return GetTotalNum();
}