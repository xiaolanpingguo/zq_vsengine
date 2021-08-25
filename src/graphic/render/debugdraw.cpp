#include "graphic/render/debugdraw.h"
#include "math/aabb.h"
#include "math/sphere.h"
#include "math/obb.h"
#include "graphic/pass/materialpass.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
VSDebugDraw::VSDebugDraw()
{


	VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> FormatArray;
	VSVertexFormat::VERTEXFORMAT_TYPE Pos;
	Pos.DataType = VSDataBuffer::DT_FLOAT32_3;
	Pos.Offset = 0;
	Pos.Semantics = VSVertexFormat::VF_POSITION;
	Pos.SemanticsIndex = 0;
	FormatArray.AddElement(Pos);

	VSVertexFormat::VERTEXFORMAT_TYPE Color;
	Color.DataType = VSDataBuffer::DT_COLOR;
	Color.Offset = 12;
	Color.Semantics = VSVertexFormat::VF_COLOR;
	Color.SemanticsIndex = 0;
	FormatArray.AddElement(Color);


	m_pDrawVertexFormat = VSResourceManager::LoadVertexFormat(NULL, &FormatArray);

	m_pOnlyVertexColor = VSMaterialR::Create((VSMaterial *)VSMaterial::GetDefaultOnlyVertexColor());
	m_pOnlyVertexColorDisableDepth = VSMaterialR::Create((VSMaterial *)VSMaterial::GetDefaultOnlyVertexColorDisableDepth());

	m_bEnable = true;
}
VSDebugDraw::~VSDebugDraw()
{
	m_pOnlyVertexColor = NULL;
	m_pOnlyVertexColorDisableDepth = NULL;

	m_pDrawVertexFormat = NULL;
}

void VSDebugDraw::AddDebugLine(const Vector3 & P1,const Vector3 & P2,const uint32 &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}

	DebugVertexType V[2];
	V[0].Pos = P1;
	V[0].Color = Color;

	V[1].Pos = P2;
	V[1].Color = Color;
	if (bDepth)
	{
		DepthDebugLineArray.AddElement(V[0]);
		DepthDebugLineArray.AddElement(V[1]);

	}
	else
	{
		NoDepthDebugLineArray.AddElement(V[0]);
		NoDepthDebugLineArray.AddElement(V[1]);

	}

}
void VSDebugDraw::AddDebugTriangle(const Vector3 & P1,const Vector3 & P2,const Vector3 &P3,
								  const uint32 &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
}
void VSDebugDraw::AddDebugLineAABB(const AABB & AABB,const uint32 &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	Vector3 Point[8];
	AABB.GetPoint(Point);
	AddDebugLine(Point[0],Point[1],Color,bDepth);
	AddDebugLine(Point[1],Point[2],Color,bDepth);
	AddDebugLine(Point[2],Point[3],Color,bDepth);
	AddDebugLine(Point[3],Point[0],Color,bDepth);
	AddDebugLine(Point[4],Point[5],Color,bDepth);
	AddDebugLine(Point[5],Point[6],Color,bDepth);
	AddDebugLine(Point[6],Point[7],Color,bDepth);
	AddDebugLine(Point[7],Point[4],Color,bDepth);
	AddDebugLine(Point[0],Point[4],Color,bDepth);
	AddDebugLine(Point[1],Point[5],Color,bDepth);
	AddDebugLine(Point[2],Point[6],Color,bDepth);
	AddDebugLine(Point[3],Point[7],Color,bDepth);
}
void VSDebugDraw::AddDebugLineSphere(const Sphere & Sphere,const uint32 &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	for (uint32 i = 0 ; i < 360 ; i++)
	{
		Vector3 Point1 = Sphere.m_Center + Vector3(VSMathInstance::GetMathInstance().GetFastSin(i), VSMathInstance::GetMathInstance().GetFastCos(i),0.0f) * Sphere.m_fRadius;
		Vector3 Point2 = Sphere.m_Center + Vector3(VSMathInstance::GetMathInstance().GetFastSin(i + 1), VSMathInstance::GetMathInstance().GetFastCos(i + 1),0.0f) * Sphere.m_fRadius;
		AddDebugLine(Point1,Point2,Color,bDepth);
	}
	for (uint32 i = 0 ; i < 360 ; i++)
	{
		Vector3 Point1 = Sphere.m_Center + Vector3(VSMathInstance::GetMathInstance().GetFastSin(i),0.0f, VSMathInstance::GetMathInstance().GetFastCos(i)) * Sphere.m_fRadius;
		Vector3 Point2 = Sphere.m_Center + Vector3(VSMathInstance::GetMathInstance().GetFastSin(i + 1),0.0f, VSMathInstance::GetMathInstance().GetFastCos(i + 1)) * Sphere.m_fRadius;
		AddDebugLine(Point1,Point2,Color,bDepth);
	}
	for (uint32 i = 0 ; i < 360 ; i++)
	{
		Vector3 Point1 = Sphere.m_Center + Vector3(0.0f, VSMathInstance::GetMathInstance().GetFastSin(i), VSMathInstance::GetMathInstance().GetFastCos(i)) * Sphere.m_fRadius;
		Vector3 Point2 = Sphere.m_Center + Vector3(0.0f, VSMathInstance::GetMathInstance().GetFastSin(i + 1), VSMathInstance::GetMathInstance().GetFastCos(i + 1)) * Sphere.m_fRadius;
		AddDebugLine(Point1,Point2,Color,bDepth);
	}
}
void VSDebugDraw::AddDebugLineOBB(const OBB & OBB,const uint32 &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	Vector3 Point[8];
	OBB.GetPoint(Point);
	AddDebugLine(Point[0],Point[1],Color,bDepth);
	AddDebugLine(Point[1],Point[2],Color,bDepth);
	AddDebugLine(Point[2],Point[3],Color,bDepth);
	AddDebugLine(Point[3],Point[0],Color,bDepth);
	AddDebugLine(Point[4],Point[5],Color,bDepth);
	AddDebugLine(Point[5],Point[6],Color,bDepth);
	AddDebugLine(Point[6],Point[7],Color,bDepth);
	AddDebugLine(Point[7],Point[4],Color,bDepth);
	AddDebugLine(Point[0],Point[4],Color,bDepth);
	AddDebugLine(Point[1],Point[5],Color,bDepth);
	AddDebugLine(Point[2],Point[6],Color,bDepth);
	AddDebugLine(Point[3],Point[7],Color,bDepth);

}
void VSDebugDraw::AddDebugTriangleAABB(const AABB & AABB,const uint32 &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	Vector3 Point[8];
	AABB.GetPoint(Point);

}
void VSDebugDraw::AddDebugTriangleSphere(const Sphere & Sphere,const uint32 &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}

}
void VSDebugDraw::AddDebugTriangleOBB(const OBB & OBB,const uint32 &Color,bool bDepth)
{
	if (!m_bEnable)
	{
		return ;
	}
	Vector3 Point[8];
	OBB.GetPoint(Point);

}
void VSDebugDraw::DrawDebugInfo(VSCamera * pCamera)
{
	if (!m_bEnable)
	{
		return ;
	}

	
	VSDVGeometry *  pBuffer = VSResourceManager::GetDVGeometry(m_pDrawVertexFormat, VSMeshData::MDT_LINE, DepthDebugLineArray.GetNum() + NoDepthDebugLineArray.GetNum());

	if (pBuffer)
	{
		pBuffer->Add(DepthDebugLineArray.GetBuffer(), sizeof(DebugVertexType)* DepthDebugLineArray.GetNum());
		pBuffer->ClearAllMaterialInstance();
		pBuffer->AddMaterialInstance(m_pOnlyVertexColor);
		pBuffer->Draw(pCamera);

		pBuffer->Add(NoDepthDebugLineArray.GetBuffer(), sizeof(DebugVertexType)* NoDepthDebugLineArray.GetNum());
		pBuffer->ClearAllMaterialInstance();
		pBuffer->AddMaterialInstance(m_pOnlyVertexColorDisableDepth);
		pBuffer->Draw(pCamera);
	}
	

	DepthDebugLineArray.Clear();
	NoDepthDebugLineArray.Clear();

}