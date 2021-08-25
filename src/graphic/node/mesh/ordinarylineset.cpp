#include "graphic/node/mesh/ordinarylineset.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSOrdinaryLineSet,VSLineSet)
BEGIN_ADD_PROPERTY(VSOrdinaryLineSet,VSLineSet)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOrdinaryLineSet)
IMPLEMENT_INITIAL_END
VSOrdinaryLineSet::VSOrdinaryLineSet()
{

}
VSOrdinaryLineSet::~VSOrdinaryLineSet()
{

}
VSOrdinaryLineSet::VSOrdinaryLineSet(ControlCurve * pControlCurve,CurveSubdivision * pSubidvision)
{
	VSMAC_ASSERT(pSubidvision && pControlCurve);
	if(!pSubidvision->GetPoints())
	{
		pSubidvision->SetSubCurve(pControlCurve);
		pSubidvision->Subdivision();
	}
	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;

	pVertex->SetData(pSubidvision->GetPoints(), pSubidvision->GetPointNum(),VSDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);

	bool bResult = CreateIndex(LT_OPEN);
	VSMAC_ASSERT(bResult);
}
VSOrdinaryLineSet::VSOrdinaryLineSet(const Line & Line)
{
	Vector3 Verts[2];
	Verts[0] = Line.GetOrig() - Line.GetDir() * DRAW_MATH_ELEMENT_LENGTH;
	Verts[1] = Line.GetOrig() + Line.GetDir() * DRAW_MATH_ELEMENT_LENGTH;

	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;

	pVertex->SetData(Verts, 2,VSDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);
	bool bResult = CreateIndex(LT_OPEN);
	VSMAC_ASSERT(bResult);
}
VSOrdinaryLineSet::VSOrdinaryLineSet(const Ray & Ray)
{
	Vector3 Verts[2];
	Verts[0] = Ray.GetOrig();
	Verts[1] = Verts[0] + Ray.GetDir() * DRAW_MATH_ELEMENT_LENGTH;
	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;

	pVertex->SetData(Verts, 2,VSDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);
	bool bResult = CreateIndex(LT_OPEN);
	VSMAC_ASSERT(bResult);
}
VSOrdinaryLineSet::VSOrdinaryLineSet(const Segment & Segment)
{
	Vector3 Verts[2];
	Verts[0] = Segment.GetOrig();
	Verts[1] = Segment.GetEnd();
	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;

	pVertex->SetData(Verts, 2,VSDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);
	bool bResult = CreateIndex(LT_OPEN);
	VSMAC_ASSERT(bResult);

}
