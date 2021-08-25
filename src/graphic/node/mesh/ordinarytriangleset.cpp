#include "graphic/node/mesh/ordinarytriangleset.h"
#include "graphic/core/graphicinclude.h"


namespace zq{


IMPLEMENT_RTTI(VSOrdinaryTriangleSet,VSTriangleSet)
BEGIN_ADD_PROPERTY(VSOrdinaryTriangleSet,VSTriangleSet)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSOrdinaryTriangleSet)
IMPLEMENT_INITIAL_END


VSOrdinaryTriangleSet::VSOrdinaryTriangleSet()
{

}
VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const AABB& aabb)
{
	Vector3 VertexData[8];
    aabb.GetPoint(VertexData);
	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;
	pVertex->SetData(VertexData,8,VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);
	MATH_USHORT_INDEX Index[36];
	//前面的2个三角形
	Index[0]=0;  Index[1]=1;  Index[2]=2;
	Index[3]=0;  Index[4]=2;  Index[5]=3;
	
	//后面的2个三角形
	Index[6]=4;   Index[7]=6;   Index[8]=5;
	Index[9]=4;   Index[10]=7;  Index[11]=6;
	
	//右侧面的2个三角形
	Index[12]=0;  Index[13]=3;  Index[14]=7;
	Index[15]=0;  Index[16]=7;  Index[17]=4;
	//左侧面的2个三角形
	Index[18]=1;  Index[19]=5;  Index[20]=6;
	Index[21]=1;  Index[22]=6;  Index[23]=2;
	//顶面的2个三角形	
	Index[24]=0;  Index[25]=5;  Index[26]=1;
	Index[27]=0;  Index[28]=4;  Index[29]=5;
	//底面的2个三角形
	Index[30]=2;  Index[31]=6;  Index[32]=7;
	Index[33]=2;  Index[34]=7;  Index[35]=3;
	
	VSDataBuffer *pIndexDataBuffer = MEM_NEW VSDataBuffer;
	pIndexDataBuffer->SetData(Index,36,VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = MEM_NEW VSIndexBuffer();
	}

	m_pIndexBuffer->SetData(pIndexDataBuffer);
}

VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const OBB& obb)
{
	Vector3 VertexData[8];
    obb.GetPoint(VertexData);
	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;
	pVertex->SetData(VertexData,8,VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);
	MATH_USHORT_INDEX Index[36];
	//前面的2个三角形
	Index[0]=0;  Index[1]=1;  Index[2]=2;
	Index[3]=0;  Index[4]=2;  Index[5]=3;

	//后面的2个三角形
	Index[6]=4;   Index[7]=6;   Index[8]=5;
	Index[9]=4;   Index[10]=7;  Index[11]=6;

	//右侧面的2个三角形
	Index[12]=0;  Index[13]=3;  Index[14]=7;
	Index[15]=0;  Index[16]=7;  Index[17]=4;
	//左侧面的2个三角形
	Index[18]=1;  Index[19]=5;  Index[20]=6;
	Index[21]=1;  Index[22]=6;  Index[23]=2;
	//顶面的2个三角形	
	Index[24]=0;  Index[25]=5;  Index[26]=1;
	Index[27]=0;  Index[28]=4;  Index[29]=5;
	//底面的2个三角形
	Index[30]=2;  Index[31]=6;  Index[32]=7;
	Index[33]=2;  Index[34]=7;  Index[35]=3;

	VSDataBuffer *pIndexDataBuffer = MEM_NEW VSDataBuffer;
	pIndexDataBuffer->SetData(Index,36,VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = MEM_NEW VSIndexBuffer();

	}

	m_pIndexBuffer->SetData(pIndexDataBuffer);
}

VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const Plane& plane)
{
	Matrix3X3W Mat;
	Mat.CreateFromLookDir(plane.GetPoint(), plane.GetN());

	Vector3 A0(Mat.M[0][0],Mat.M[0][1],Mat.M[0][2]);
	Vector3 A1(Mat.M[1][0],Mat.M[1][1],Mat.M[1][2]);

	Vector3 VertexData[4];
	VertexData[0] = plane.GetPoint() + A0 * DRAW_MATH_ELEMENT_LENGTH + A1 * DRAW_MATH_ELEMENT_LENGTH;
	VertexData[1] = plane.GetPoint() - A0 * DRAW_MATH_ELEMENT_LENGTH + A1 * DRAW_MATH_ELEMENT_LENGTH;
	VertexData[2] = plane.GetPoint() - A0 * DRAW_MATH_ELEMENT_LENGTH - A1 * DRAW_MATH_ELEMENT_LENGTH;
	VertexData[3] = plane.GetPoint() + A0 * DRAW_MATH_ELEMENT_LENGTH - A1 * DRAW_MATH_ELEMENT_LENGTH;

	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;
	pVertex->SetData(VertexData,4,VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);

	MATH_USHORT_INDEX Index[6];

	Index[0]=0;  Index[1]=1;  Index[2]=2;
	Index[3]=0;  Index[4]=2;  Index[5]=3;

	VSDataBuffer *pIndexDataBuffer = MEM_NEW VSDataBuffer;
	pIndexDataBuffer->SetData(Index,6,VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = MEM_NEW VSIndexBuffer();
	}

	m_pIndexBuffer->SetData(pIndexDataBuffer);
}

VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const Triangle& triangle)
{
	Vector3 VertexData[3];
    triangle.GetPoint(VertexData);
	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;
	pVertex->SetData(VertexData,3,VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);

	MATH_USHORT_INDEX Index[3];
	Index[0]=0;  Index[1]=1;  Index[2]=2;
	VSDataBuffer *pIndexDataBuffer = MEM_NEW VSDataBuffer;
	pIndexDataBuffer->SetData(Index,3,VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = MEM_NEW VSIndexBuffer();

	}

	m_pIndexBuffer->SetData(pIndexDataBuffer);
}

VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const Rectangle& rectangle)
{
	Vector3 VertexData[4];
	rectangle.GetPoint(VertexData);
	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;
	pVertex->SetData(VertexData,4,VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);

	MATH_USHORT_INDEX Index[6];

	Index[0]=0;  Index[1]=2;  Index[2]=1;
	Index[3]=0;  Index[4]=3;  Index[5]=2;

	VSDataBuffer *pIndexDataBuffer = MEM_NEW VSDataBuffer;
	pIndexDataBuffer->SetData(Index,6,VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = MEM_NEW VSIndexBuffer();
	}

	m_pIndexBuffer->SetData(pIndexDataBuffer);
}

VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const Polygon& polyon)
{ 
	VSMAC_ASSERT(polyon.GetpPoint() && polyon.GetPointNum());

	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;
	pVertex->SetData(polyon.GetpPoint(), polyon.GetPointNum(),VSDataBuffer::DT_FLOAT32_3);
	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);

	VSDataBuffer *pIndexDataBuffer = MEM_NEW VSDataBuffer;
	pIndexDataBuffer->SetData(polyon.GetpIndex(), polyon.GetIndexNum(),VSDataBuffer::DT_USHORT);

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = MEM_NEW VSIndexBuffer();
	}

	m_pIndexBuffer->SetData(pIndexDataBuffer);
}

VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(const Sphere& sphere)
{
	float LongitudeAngle = 10;//经度线上的划分角
	float LatitudeAngle = 10;//维度线上的划分角

	float AnglePI = 180.0f;
	uint32 LongitudeNum = 0;
	while(AnglePI > 0)
	{
		LongitudeNum++ ;
		AnglePI -= LongitudeAngle;
	}

	LongitudeNum++;
	uint32 LatitudeNum = unsigned int(360.0f / LatitudeAngle) + 1;

	VSDataBuffer* pVertex = MEM_NEW VSDataBuffer;
	pVertex->CreateEmptyBuffer(LongitudeNum * LatitudeNum,VSDataBuffer::DT_FLOAT32_3);

	Vector3* pVertexData = (Vector3 *)pVertex->GetData();
	float i = 0 ;
	for(uint32 k = 0 ; k < LongitudeNum ; k++)
	{
		if(i > 180)
			i = 180;

		float iRadian = AngleToRadian(i);
		for(float j = 0 ; j <= 360.0f; j = j + LatitudeAngle)
		{
			float jRadian = AngleToRadian(j);
			pVertexData->x = sphere.m_fRadius * SIN(iRadian) * COS(jRadian);
			pVertexData->z = sphere.m_fRadius * SIN(iRadian) * SIN(jRadian);
			pVertexData->y = sphere.m_fRadius * COS(iRadian);

			*pVertexData += sphere.m_Center;
			pVertexData++;
		}

		i = i + LongitudeAngle;
	}

	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);

	VSDataBuffer *pIndexDataBuffer = MEM_NEW VSDataBuffer;
	pIndexDataBuffer->CreateEmptyBuffer((LongitudeNum - 1) * LatitudeNum * 2 * 3,VSDataBuffer::DT_USHORT);
	MATH_USHORT_INDEX * pIndexData = (MATH_USHORT_INDEX *)pIndexDataBuffer->GetData();
	for (uint32 i = 0 ; i < LongitudeNum - 1 ; i++)
	{
		uint32 z_width_poly= i * LatitudeNum ;
		uint32 z_width = i * LatitudeNum;
		for(uint32 j = 0 ; j < LatitudeNum ; j++)
		{
			uint32 z_width_x = z_width + j;
			uint32 z_width_xX2 = (z_width_poly + j) * 2;
			if(j != LatitudeNum - 1)
			{
				pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
				pIndexData[z_width_xX2 * 3 + 2] = z_width_x + LatitudeNum;
				pIndexData[z_width_xX2 * 3 + 1] = z_width_x + 1;

				pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1 + LatitudeNum;
				pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x + 1;
				pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + LatitudeNum;
			}
			else
			{
				pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
				pIndexData[z_width_xX2 * 3 + 2] = z_width_x + LatitudeNum;
				pIndexData[z_width_xX2 * 3 + 1] = z_width_x - LatitudeNum + 1;

				pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1;
				pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x - LatitudeNum + 1;
				pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + LatitudeNum;
			}
		}
	}

	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = MEM_NEW VSIndexBuffer();

	}

	m_pIndexBuffer->SetData(pIndexDataBuffer);
}

VSOrdinaryTriangleSet::~VSOrdinaryTriangleSet()
{

}

VSOrdinaryTriangleSet::VSOrdinaryTriangleSet(Surface* pSurface ,SurfaceSubdivision* pSubidvision)
{
	VSMAC_ASSERT(pSubidvision);
	VSMAC_ASSERT(pSurface);
	if(!pSubidvision->GetPoints())
	{
		pSubidvision->SetSurface(pSurface);
		bool bResult = pSubidvision->Subdivision();
		VSMAC_ASSERT(bResult);
	}
	VSDataBuffer * pVertex = MEM_NEW VSDataBuffer;
	uint32 Row = pSubidvision->GetRow();
	uint32 Column = pSubidvision->GetColumn();
	pVertex->SetData(pSubidvision->GetPoints(), Row * Column,VSDataBuffer::DT_FLOAT32_3);

	m_pVertexBuffer = MEM_NEW VSVertexBuffer(true);
	m_pVertexBuffer->SetData(pVertex,VSVertexFormat::VF_POSITION);

	VSDataBuffer *pIndexDataBuffer = MEM_NEW VSDataBuffer;
	pIndexDataBuffer->CreateEmptyBuffer((Row - 1) * (Column - 1) * 2 * 3,VSDataBuffer::DT_USHORT);
	MATH_USHORT_INDEX * pIndexData = (MATH_USHORT_INDEX *)pIndexDataBuffer->GetData();

	for (uint32 i = 0 ; i < Row - 1 ; i++)
	{
		uint32 z_width_poly= i * (Column - 1);
		uint32 z_width = i * Column;
		for(uint32 j = 0 ; j < Column - 1 ; j++)
		{
			uint32 z_width_x = z_width + j;
			uint32 z_width_xX2 = (z_width_poly + j) * 2;
			pIndexData[z_width_xX2 * 3 + 0] = z_width_x;
			pIndexData[z_width_xX2 * 3 + 2] = z_width_x + Column;
			pIndexData[z_width_xX2 * 3 + 1] = z_width_x + 1;
			pIndexData[(z_width_xX2 + 1) * 3 + 0] = z_width_x + 1 + Column;
			pIndexData[(z_width_xX2 + 1) * 3 + 2] = z_width_x + 1;
			pIndexData[(z_width_xX2 + 1) * 3 + 1] = z_width_x + Column;
		}
	}
	
	if (!m_pIndexBuffer)
	{
		m_pIndexBuffer = MEM_NEW VSIndexBuffer();

	}

	m_pIndexBuffer->SetData(pIndexDataBuffer);
}

}
