#include "graphic/node/mesh/controlpointpatch.h"
#include "graphic/node/node.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSControlPointPatch, VSMeshData)
BEGIN_ADD_PROPERTY(VSControlPointPatch, VSMeshData)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSControlPointPatch)
IMPLEMENT_INITIAL_END
VSControlPointPatch::VSControlPointPatch()
{
	m_uiMeshDataType = MDT_CONTROL_POINT_4;
}
VSControlPointPatch::~VSControlPointPatch()
{

}
uint32 VSControlPointPatch::GetTotalNum()const
{
	if (!m_pIndexBuffer && !m_pVertexBuffer)
	{
		return 0;
	}
	else if (!m_pIndexBuffer)
	{
		return m_pVertexBuffer->GetVertexNum() / 3;
	}
	else
	{
		return m_pIndexBuffer->GetNum() / 3;
	}
}
uint32 VSControlPointPatch::GetGirdNum(uint32 uiInputNum)const
{

	return uiInputNum / 3;
}
uint32 VSControlPointPatch::GetActiveNum()const
{
	return GetTotalNum();
}