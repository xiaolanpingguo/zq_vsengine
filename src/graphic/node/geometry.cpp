#include "graphic/node/geometry.h"
#include "graphic/node/model/bonenode.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
#include "graphic/node/mesh/triangleset.h"
#include "graphic/asyn/renderthread.h"
using namespace zq;
IMPLEMENT_RTTI(VSGeometry,VSSpatial)
BEGIN_ADD_PROPERTY(VSGeometry,VSSpatial)
REGISTER_PROPERTY(m_LocalBV, LocalBV, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pMeshData,MeshData,VSProperty::F_SAVE_LOAD_COPY)
REGISTER_PROPERTY(m_BoneName, BoneName, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_GeometryName, GeometryName, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pMaterialInstance, MaterialInstance, VSProperty::F_SAVE_LOAD_CLONE | VSProperty::F_REFLECT_NAME );
REGISTER_PROPERTY(m_MorphData, MorphData, VSProperty::F_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGeometry)
ADD_PRIORITY(VSMaterial)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
VSPointer<VSGeometry> VSGeometry::ms_Quad = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultCube = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultCubCone = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultCone = NULL;
VSPointer<VSGeometry> VSGeometry::ms_DefaultRenderCube = NULL;
bool VSGeometry::InitialDefaultState()
{
	ms_Quad = MEM_NEW VSGeometry();
	if (!ms_Quad)
	{
		return false;
	}
	
	ms_DefaultCube = MEM_NEW VSGeometry();
	if (!ms_DefaultCube)
	{
		return false;
	}

	ms_DefaultCubCone = MEM_NEW VSGeometry();
	if (!ms_DefaultCubCone)
	{
		return false;
	}

	ms_DefaultCone = MEM_NEW VSGeometry();
	if (!ms_DefaultCone)
	{
		return false;
	}
	ms_DefaultRenderCube = MEM_NEW VSGeometry();
	if (!ms_DefaultRenderCube)
	{
		return false;
	}
	LoadDefault();
	return 1;
}
bool VSGeometry::TerminalDefaultState()
{
	ms_Quad = NULL;
	ms_DefaultCube = NULL;
	ms_DefaultCubCone = NULL;
	ms_DefaultCone = NULL;
	ms_DefaultRenderCube = NULL;
	return 1;
}
void VSGeometry::LoadDefault()
{
	//Quad
	{
		VSArray<Vector3W> VertexArray;
		VSArray<Vector2> m_TexCoordArray;
		VSArray<MATH_USHORT_INDEX> IndexArray;

		VertexArray.AddElement(Vector3W(-1.0f,  1.0f, 0.0f,1.0f));
		VertexArray.AddElement(Vector3W( 1.0f,  1.0f, 0.0f,1.0f));
		VertexArray.AddElement(Vector3W( 1.0f, -1.0f, 0.0f,1.0f));
		VertexArray.AddElement(Vector3W(-1.0f, -1.0f, 0.0f,1.0f));

		m_TexCoordArray.AddElement(Vector2(0.0f, 0.0f));
		m_TexCoordArray.AddElement(Vector2(1.0f, 0.0f));
		m_TexCoordArray.AddElement(Vector2(1.0f, 1.0f));
		m_TexCoordArray.AddElement(Vector2(0.0f, 1.0f));

		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);


		VSDataBufferPtr  pVertexData = MEM_NEW VSDataBuffer;
		pVertexData->SetData(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_4);

		VSDataBufferPtr pTexCoord = MEM_NEW VSDataBuffer;
		pTexCoord->SetData(&m_TexCoordArray[0],(unsigned int)m_TexCoordArray.GetNum(),VSDataBuffer::DT_FLOAT32_2);

		VSDataBufferPtr pIndex = MEM_NEW VSDataBuffer;
		pIndex->SetData(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = MEM_NEW VSVertexBuffer(true);
		pVertexBuffer->SetData(pVertexData,VSVertexFormat::VF_POSITION);
		pVertexBuffer->SetData(pTexCoord,VSVertexFormat::VF_TEXCOORD);

		VSIndexBufferPtr pIndexBuffer = MEM_NEW VSIndexBuffer();

		pIndexBuffer->SetData(pIndex);

		VSTriangleSetPtr pTriangleSetData = MEM_NEW VSTriangleSet();
		pTriangleSetData->SetVertexBuffer(pVertexBuffer);
		pTriangleSetData->SetIndexBuffer(pIndexBuffer);


		ms_Quad->SetMeshData(pTriangleSetData);
		ms_Quad->m_GeometryName = _T("DefaultQuad");
	}
	

	//Cub
	{
		VSArray<Vector3> VertexArray;

		VSArray<MATH_USHORT_INDEX> IndexArray;

		VertexArray.AddElement(Vector3(-1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(Vector3( 1.0f,  1.0f, 0.0f));
		VertexArray.AddElement(Vector3( 1.0f, -1.0f, 0.0f));
		VertexArray.AddElement(Vector3(-1.0f, -1.0f, 0.0f));

		VertexArray.AddElement(Vector3(-1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(Vector3( 1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(Vector3( 1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(Vector3(-1.0f, -1.0f, 1.0f));

		//front
		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);

		//back
		IndexArray.AddElement(4);
		IndexArray.AddElement(6);
		IndexArray.AddElement(5);
		IndexArray.AddElement(4);
		IndexArray.AddElement(7);
		IndexArray.AddElement(6);

		//left
		IndexArray.AddElement(0);
		IndexArray.AddElement(7);
		IndexArray.AddElement(4);
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(7);

		//right
		IndexArray.AddElement(1);
		IndexArray.AddElement(5);
		IndexArray.AddElement(6);
		IndexArray.AddElement(1);
		IndexArray.AddElement(6);
		IndexArray.AddElement(2);

		//up
		IndexArray.AddElement(0);
		IndexArray.AddElement(4);
		IndexArray.AddElement(5);
		IndexArray.AddElement(0);
		IndexArray.AddElement(5);
		IndexArray.AddElement(1);

		//bottom
		IndexArray.AddElement(3);
		IndexArray.AddElement(6);
		IndexArray.AddElement(7);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);
		IndexArray.AddElement(6);

		VSDataBufferPtr  pVertexData = MEM_NEW VSDataBuffer;
		pVertexData->SetData(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = MEM_NEW VSDataBuffer;
		pIndex->SetData(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = MEM_NEW VSVertexBuffer(true);
		pVertexBuffer->SetData(pVertexData,VSVertexFormat::VF_POSITION);

		VSIndexBufferPtr pIndexBuffer = MEM_NEW VSIndexBuffer();

		pIndexBuffer->SetData(pIndex);

		VSTriangleSetPtr pTriangleSetData = MEM_NEW VSTriangleSet();
		pTriangleSetData->SetVertexBuffer(pVertexBuffer);
		pTriangleSetData->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCube->SetMeshData(pTriangleSetData);
		ms_DefaultCube->m_GeometryName = _T("DefaultCub");
	}

	//Cub Cone
	{
		VSArray<Vector3> VertexArray;

		VSArray<MATH_USHORT_INDEX> IndexArray;

		VertexArray.AddElement(Vector3( 0.0f,  0.0f, 0.0f));
		

		VertexArray.AddElement(Vector3(-1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(Vector3( 1.0f,  1.0f, 1.0f));
		VertexArray.AddElement(Vector3( 1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(Vector3(-1.0f, -1.0f, 1.0f));

		//front
		IndexArray.AddElement(1);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);
		IndexArray.AddElement(1);
		IndexArray.AddElement(4);
		IndexArray.AddElement(3);


		//right
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(3);


		//left
		IndexArray.AddElement(0);
		IndexArray.AddElement(4);
		IndexArray.AddElement(1);


		//up
		IndexArray.AddElement(0);
		IndexArray.AddElement(1);
		IndexArray.AddElement(2);


		//bottom
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(4);


		VSDataBufferPtr  pVertexData = MEM_NEW VSDataBuffer;
		pVertexData->SetData(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = MEM_NEW VSDataBuffer;
		pIndex->SetData(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = MEM_NEW VSVertexBuffer(true);
		pVertexBuffer->SetData(pVertexData,VSVertexFormat::VF_POSITION);

		VSIndexBufferPtr pIndexBuffer = MEM_NEW VSIndexBuffer();

		pIndexBuffer->SetData(pIndex);

		VSTriangleSetPtr pTriangleSetData = MEM_NEW VSTriangleSet();
		pTriangleSetData->SetVertexBuffer(pVertexBuffer);
		pTriangleSetData->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCubCone->SetMeshData(pTriangleSetData);
		ms_DefaultCubCone->m_GeometryName = _T("DefaultCubCone");
	}

	//Cone
	{
		uint32 uiLevel = 1;
		uint32 CircleLevel = 5;


		VSArray<Vector3> VertexArray;

		VSArray<MATH_USHORT_INDEX> IndexArray;

		VertexArray.AddElement(Vector3( 0.0f,  0.0f, 0.0f));


		float RangeStep = 1.0f / uiLevel;
		for (uint32 i = 0 ; i < uiLevel ; i++)
		{
			float Z = RangeStep * (i + 1);
			for (uint32 j = 0 ; j < CircleLevel ; j++)
			{
				float Radio = ( j * 1.0f ) / CircleLevel * MATH_2PI;
				float X = Z * COS(Radio);
				float Y = Z * SIN(Radio);
				VertexArray.AddElement(Vector3(X,Y,Z));
			}
		}
		VertexArray.AddElement(Vector3(0.0f,0.0f, 1.0f));
		

		// first level
		for (uint32 i = 0 ; i < CircleLevel ; i++)
		{
			IndexArray.AddElement(0);

			uint32 Index1 = i + 2;
			uint32 Index2 = i + 1;
			if (Index1 = CircleLevel + 1)
			{
				Index1 = 1;
			}
			IndexArray.AddElement(Index1);
			IndexArray.AddElement(Index2);
		}

		//
		for (uint32 i = 1 ; i < uiLevel ; i++)
		{
			for (uint32 j = 0 ; j < CircleLevel ; j++)
			{
				uint32 TopIndex1 = (i - 1) * CircleLevel + 1 + j;
				uint32 TopIndex2 = (i - 1) * CircleLevel + 1 + j + 1;
				
				
				
				uint32 BottomIndex1 = i * CircleLevel + 1 + j;
				uint32 BottomIndex2 = i * CircleLevel + 1 + j + 1;

				if (j + 1 == CircleLevel)
				{
					TopIndex2 = (i - 1) * CircleLevel + 1 ;
					BottomIndex2 = i * CircleLevel + 1;
				}

				IndexArray.AddElement(TopIndex1);
				IndexArray.AddElement(TopIndex2);
				IndexArray.AddElement(BottomIndex1);
				IndexArray.AddElement(TopIndex2);
				IndexArray.AddElement(BottomIndex1);
				IndexArray.AddElement(BottomIndex2);
			}
		}

		// last level
		for (uint32 i = 0 ; i < CircleLevel ; i++)
		{
			IndexArray.AddElement(VertexArray.GetNum() - 1);

			uint32 Index1 = VertexArray.GetNum() - 3 - i;
			uint32 Index2 = VertexArray.GetNum() - 2 - i;
			
			if (i == CircleLevel - 1)
			{
				Index1 = VertexArray.GetNum() - 2;
			}
			IndexArray.AddElement(Index1);
			IndexArray.AddElement(Index2);
		}

		

		VSDataBufferPtr  pVertexData = MEM_NEW VSDataBuffer;
		pVertexData->SetData(&VertexArray[0],(unsigned int)VertexArray.GetNum(),VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = MEM_NEW VSDataBuffer;
		pIndex->SetData(&IndexArray[0],(unsigned int)IndexArray.GetNum(),VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = MEM_NEW VSVertexBuffer(true);
		pVertexBuffer->SetData(pVertexData,VSVertexFormat::VF_POSITION);

		VSIndexBufferPtr pIndexBuffer = MEM_NEW VSIndexBuffer();

		pIndexBuffer->SetData(pIndex);

		VSTriangleSetPtr pTriangleSetData = MEM_NEW VSTriangleSet();
		pTriangleSetData->SetVertexBuffer(pVertexBuffer);
		pTriangleSetData->SetIndexBuffer(pIndexBuffer);


		ms_DefaultCone->SetMeshData(pTriangleSetData);
		ms_DefaultCone->m_GeometryName = _T("DefaultCone");
	}

	//Quad Cub
	{
		VSArray<Vector3> VertexArray;
		VSArray<Vector2> TexCoordArray;
		VSArray<Vector3> NormalArray;
		VSArray<MATH_USHORT_INDEX> IndexArray;

		//pos uv
		VertexArray.AddElement(Vector3(-1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(Vector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(Vector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(Vector3(-1.0f, -1.0f, 1.0f));

		TexCoordArray.AddElement(Vector2(0.0f, 0.0f));
		TexCoordArray.AddElement(Vector2(0.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 0.0f));

		NormalArray.AddElement(Vector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(Vector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(Vector3(0.0f, 0.0f, 1.0f));
		NormalArray.AddElement(Vector3(0.0f, 0.0f, 1.0f));

		//pos uv
		VertexArray.AddElement(Vector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(Vector3(1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(Vector3(1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(Vector3(-1.0f, -1.0f, -1.0f));

		TexCoordArray.AddElement(Vector2(0.0f, 0.0f));
		TexCoordArray.AddElement(Vector2(0.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 0.0f));

		NormalArray.AddElement(Vector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(Vector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(Vector3(0.0f, 0.0f, -1.0f));
		NormalArray.AddElement(Vector3(0.0f, 0.0f, -1.0f));

		//pos uv
		VertexArray.AddElement(Vector3(-1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(Vector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(Vector3(-1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(Vector3(-1.0f, -1.0f, 1.0f));

		TexCoordArray.AddElement(Vector2(0.0f, 0.0f));
		TexCoordArray.AddElement(Vector2(0.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 0.0f));

		NormalArray.AddElement(Vector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(Vector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(Vector3(-1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(Vector3(-1.0f, 0.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(Vector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(Vector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(Vector3(1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(Vector3(1.0f, 1.0f, -1.0f));

		TexCoordArray.AddElement(Vector2(0.0f, 0.0f));
		TexCoordArray.AddElement(Vector2(0.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 0.0f));

		NormalArray.AddElement(Vector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(Vector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(Vector3(1.0f, 0.0f, 0.0f));
		NormalArray.AddElement(Vector3(1.0f, 0.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(Vector3(1.0f, 1.0f, 1.0f));
		VertexArray.AddElement(Vector3(1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(Vector3(-1.0f, 1.0f, -1.0f));
		VertexArray.AddElement(Vector3(-1.0f, 1.0f, 1.0f));

		TexCoordArray.AddElement(Vector2(0.0f, 0.0f));
		TexCoordArray.AddElement(Vector2(0.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 0.0f));

		NormalArray.AddElement(Vector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(Vector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(Vector3(0.0f, 1.0f, 0.0f));
		NormalArray.AddElement(Vector3(0.0f, 1.0f, 0.0f));

		//pos uv
		VertexArray.AddElement(Vector3(1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(Vector3(-1.0f, -1.0f, 1.0f));
		VertexArray.AddElement(Vector3(-1.0f, -1.0f, -1.0f));
		VertexArray.AddElement(Vector3(1.0f, -1.0f, -1.0f));

		TexCoordArray.AddElement(Vector2(0.0f, 0.0f));
		TexCoordArray.AddElement(Vector2(0.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 1.0f));
		TexCoordArray.AddElement(Vector2(1.0f, 0.0f));

		NormalArray.AddElement(Vector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(Vector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(Vector3(0.0f, -1.0f, 0.0f));
		NormalArray.AddElement(Vector3(0.0f, -1.0f, 0.0f));

		//front
		IndexArray.AddElement(0);
		IndexArray.AddElement(2);
		IndexArray.AddElement(1);
		IndexArray.AddElement(0);
		IndexArray.AddElement(3);
		IndexArray.AddElement(2);

		//back
		IndexArray.AddElement(4);
		IndexArray.AddElement(5);
		IndexArray.AddElement(6);
		IndexArray.AddElement(4);
		IndexArray.AddElement(6);
		IndexArray.AddElement(7);

		//left
		IndexArray.AddElement(8);
		IndexArray.AddElement(9);
		IndexArray.AddElement(10);
		IndexArray.AddElement(8);
		IndexArray.AddElement(10);
		IndexArray.AddElement(11);

		//right
		IndexArray.AddElement(12);
		IndexArray.AddElement(13);
		IndexArray.AddElement(14);
		IndexArray.AddElement(12);
		IndexArray.AddElement(14);
		IndexArray.AddElement(15);

		//up
		IndexArray.AddElement(16);
		IndexArray.AddElement(17);
		IndexArray.AddElement(18);
		IndexArray.AddElement(16);
		IndexArray.AddElement(18);
		IndexArray.AddElement(19);

		//bottom
		IndexArray.AddElement(20);
		IndexArray.AddElement(21);
		IndexArray.AddElement(22);
		IndexArray.AddElement(20);
		IndexArray.AddElement(22);
		IndexArray.AddElement(23);

		VSDataBufferPtr  pVertexData = MEM_NEW VSDataBuffer;
		pVertexData->SetData(&VertexArray[0], (unsigned int)VertexArray.GetNum(), VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pTexCoord = MEM_NEW VSDataBuffer;
		pTexCoord->SetData(&TexCoordArray[0], TexCoordArray.GetNum(), VSDataBuffer::DT_FLOAT32_2);

		VSDataBufferPtr  pNormalData = MEM_NEW VSDataBuffer;
		pNormalData->SetData(&NormalArray[0], (unsigned int)NormalArray.GetNum(), VSDataBuffer::DT_FLOAT32_3);

		VSDataBufferPtr pIndex = MEM_NEW VSDataBuffer;
		pIndex->SetData(&IndexArray[0], (unsigned int)IndexArray.GetNum(), VSDataBuffer::DT_USHORT);

		//创建顶点BUFFER
		VSVertexBufferPtr pVertexBuffer = MEM_NEW VSVertexBuffer(true);
		pVertexBuffer->SetData(pVertexData, VSVertexFormat::VF_POSITION);
		pVertexBuffer->SetData(pTexCoord, VSVertexFormat::VF_TEXCOORD);
		pVertexBuffer->SetData(pNormalData, VSVertexFormat::VF_NORMAL);

		VSIndexBufferPtr pIndexBuffer = MEM_NEW VSIndexBuffer();

		pIndexBuffer->SetData(pIndex);

		VSTriangleSetPtr pTriangleSetData = MEM_NEW VSTriangleSet();
		pTriangleSetData->SetVertexBuffer(pVertexBuffer);
		pTriangleSetData->SetIndexBuffer(pIndexBuffer);


		ms_DefaultRenderCube->SetMeshData(pTriangleSetData);
		ms_DefaultRenderCube->m_GeometryName = _T("DefaultRenderCube");

		ms_DefaultRenderCube->AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultResource());
	}
}
void VSGeometry::UpdateGeometryBeforeDrawCall()
{
	
}
void VSGeometry::SetMorphData(uint32 Index, float fData)
{
	uint32 i = m_MorphData.ValueMap.Find(Index);
	if (i != m_MorphData.ValueMap.GetNum())
	{
		m_MorphData.ValueMap[i].Value = fData;
	}
}
bool VSGeometry::HasMorphTarget()const
{
	return m_MorphData.MorphVDataTexture != NULL;
}
void VSGeometry::CreateMorphMeshData(VSMap<unsigned int, VSVertexBuffer *> & MorphDataSet)
{
	if (!m_pMeshData)
	{
		return;
	}
	uint32 DataNum = 1;
	VSVertexBuffer * pVertexBuffer = m_pMeshData->GetVertexBuffer();
	if (pVertexBuffer->GetNormalData(0))
	{
		DataNum++;
		if (pVertexBuffer->GetTangentData())
		{
			DataNum++;
			if (pVertexBuffer->GetBinormalData())
			{
				DataNum++;
			}
		}
	}
	uint32 MorphNum = 0;
	for (uint32 i = 0 ; i < MorphDataSet.GetNum() ; i++)
	{
		if (MorphDataSet[i].Value)
		{
			MorphNum++;
			m_MorphData.ValueMap.AddElement(i, 0.0f);
		}
	}	
	if (!MorphNum)
	{
		return;
	}
	uint32 TexSizeX = VSResourceManager::GetGpuMorphTextureSize();

	uint32 Layer = (pVertexBuffer->GetVertexNum() / TexSizeX) + 1;
	m_MorphData.Layer = Layer;
	uint32 MaxY = Layer * DataNum * MorphNum;
	uint32 TexSizeY = 0;
	for (uint32 i = 0; i <= 10; i++)
	{
		uint32 Datai = 1 << i;
		uint32 Datai_1 = (1 + 1) << i;
		if (MaxY > Datai && MaxY <= Datai_1)
		{
			TexSizeY = Datai_1;
			break;
		}
	}
	VSMAC_ASSERT(TexSizeY);
	m_MorphData.TextureSizeY = TexSizeY;
#define DEFINE_RGBA32_MORPH_DATA(Texture,GetSemanticsDataFunction)\
	{\
		Vector3W * pData = MEM_NEW Vector3W[TexSizeX * TexSizeY];\
		uint32 uiNum = 0;\
		for (uint32 i = 0; i < MorphDataSet.GetNum(); i++)\
		{\
			VSVertexBuffer * pMorphVertexBuffer = MorphDataSet[i].Value;\
			if (!pVertexBuffer)\
			{\
				continue;\
			}\
			Vector3W * pDataTemp = pData + uiNum * TexSizeX * Layer;\
			Vector3 *pTemp = (Vector3 *)pMorphVertexBuffer->GetSemanticsDataFunction->GetData();\
			for (uint32 j = 0; j < pMorphVertexBuffer->GetVertexNum(); j++)\
			{\
				pDataTemp->x = pTemp->x;\
				pDataTemp->y = pTemp->y;\
				pDataTemp->z = pTemp->z;\
				pDataTemp++; pTemp++;\
			}\
			uiNum++;\
		}\
		m_MorphData.Texture = VSResourceManager::Create2DTexture(TexSizeX, TexSizeY, VSRenderer::SFT_A32B32G32R32F, 1, pData);\
		m_MorphData.Texture->SetSamplerState((VSSamplerState*)VSSamplerState::GetClamp());\
		SAFE_DELETEA(pData);\
	}


#define DEFINE_RGBA8_MORPH_DATA(Texture,GetSemanticsDataFunction)\
	{\
		uint32 * pData = MEM_NEW uint32[TexSizeX * TexSizeY];\
		uint32 uiNum = 0;\
		for (uint32 i = 0; i < MorphDataSet.GetNum(); i++)\
		{\
			VSVertexBuffer * pMorphVertexBuffer = MorphDataSet[i].Value;\
			if (!pVertexBuffer)\
			{\
				continue;\
			}\
			uint32 * pDataTemp = pData + uiNum * TexSizeX * Layer;\
			uint32 *pTemp = (uint32*)pMorphVertexBuffer->GetSemanticsDataFunction->GetData();\
			for (uint32 j = 0; j < pMorphVertexBuffer->GetVertexNum(); j++)\
			{\
				*pDataTemp = *pTemp;\
				pDataTemp++; pTemp++;\
			}\
			uiNum++;\
		}\
		m_MorphData.Texture = VSResourceManager::Create2DTexture(TexSizeX, TexSizeY, VSRenderer::SFT_A8R8G8B8, 1, pData);\
		m_MorphData.Texture->SetSamplerState((VSSamplerState*)VSSamplerState::GetClamp());\
		SAFE_DELETEA(pData);\
	}


	DEFINE_RGBA32_MORPH_DATA(MorphVDataTexture, GetPositionData(0));
	VSDataBuffer * pNormal = pVertexBuffer->GetNormalData(0);
	if (pNormal->GetDT() == VSDataBuffer::DT_FLOAT32_3)
	{
		DEFINE_RGBA32_MORPH_DATA(MorphNDataTexture, GetNormalData(0));
	}
	else
	{
		DEFINE_RGBA8_MORPH_DATA(MorphNDataTexture, GetNormalData(0));
	}
	VSDataBuffer * pTangent = pVertexBuffer->GetTangentData();
	if (pTangent)
	{
		if (pTangent->GetDT() == VSDataBuffer::DT_FLOAT32_3)
		{
			DEFINE_RGBA32_MORPH_DATA(MorphTDataTexture, GetTangentData());
			DEFINE_RGBA32_MORPH_DATA(MorphBDataTexture, GetBinormalData());
		}
		else
		{
			DEFINE_RGBA8_MORPH_DATA(MorphTDataTexture, GetTangentData());
		}
	}
	if (!VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_VertexIDInShader))
	{
		VSDataBuffer * pIDBuffer = MEM_NEW VSDataBuffer();
		VSArray<float> IDArray;
		for (uint32 i = 0 ; i < pVertexBuffer->GetVertexNum() ; i++)
		{
			IDArray.AddElement(i * 1.0f);
		}
		pIDBuffer->SetData(&IDArray[0], IDArray.GetNum(), VSDataBuffer::DT_FLOAT32_1);
		pVertexBuffer->SetData(pIDBuffer, VSVertexFormat::VF_VERTEX_ID);
	}
}
void VSGeometry::AddMorphAABB(VSVertexBuffer * pMorphVertexBuffer)
{
	VSMAC_ASSERT(pMorphVertexBuffer);

	//LinkBoneNode();
	VSMAC_ASSERT(m_pMeshData && m_pMeshData->GetVertexBuffer())
	{
		AABB NewAABB;

		VSVertexBuffer * pVerBuffer = m_pMeshData->GetVertexBuffer();
		VSMAC_ASSERT(pMorphVertexBuffer->GetPositionData(0));


		Vector3 * pVer = (Vector3*)pMorphVertexBuffer->GetPositionData(0)->GetData();
		VSMAC_ASSERT(pVer);

		uint32 uiVertexNum = pVerBuffer->GetPositionData(0)->GetNum();
		VSTransform World = m_pParent->GetWorldTransform();
		if (GetAffectSkeleton())
		{
			VSDataBuffer* pBlendIndex = pVerBuffer->GetBlendIndicesData(0);
			VSDataBuffer* pBoneWeight = pVerBuffer->GetBlendWeightData();

			if (!pBlendIndex || !pBoneWeight)
			{
				return;
			}
			VSArray<Vector3>TempBuffer;
			TempBuffer.SetBufferNum(uiVertexNum);

			if (pBlendIndex->GetDT() == VSDataBuffer::DT_UBYTE4)
			{
				uint32 * pBlendIndexData = (uint32 *)pBlendIndex->GetData();
				uint32 * pBoneWeightData = (uint32 *)pBoneWeight->GetData();
				for (uint32 i = 0; i < uiVertexNum; i++)
				{
					Vector3W BoneWeight;
					BoneWeight.CreateFromABGR(pBoneWeightData[i]);
					unsigned char BlendIndex[4];
					ColorGetABGR(pBlendIndexData[i], BlendIndex[0], BlendIndex[1], BlendIndex[2], BlendIndex[3]);
					TempBuffer[i].Set(0.0f, 0.0f, 0.0f);
					for (uint32 k = 0; k < 4; k++)
					{

						VSBoneNode * pBone = GetAffectBone(BlendIndex[k]);
						if (pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							Matrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
							TempBuffer[i] += pVer[i] * TempBone * BoneWeight.m[k];
						}
					}

				}
			}
			else
			{
				Vector3W * pBlendIndexData = (Vector3W *)pBlendIndex->GetData();
				Vector3W * pBoneWeightData = (Vector3W *)pBoneWeight->GetData();
				for (uint32 i = 0; i < uiVertexNum; i++)
				{
					TempBuffer[i].Set(0.0f, 0.0f, 0.0f);
					for (uint32 k = 0; k < 4; k++)
					{
						uint32 BlendIndex = (unsigned int)pBlendIndexData[i].m[k];
						VSBoneNode * pBone = GetAffectBone(BlendIndex);
						if (pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							Matrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
							TempBuffer[i] += pVer[i] * TempBone * pBoneWeightData[i].m[k];
						}
					}

				}
			}

			NewAABB.CreateAABB(TempBuffer.GetBuffer(), uiVertexNum);
		}
		else
		{
			NewAABB.CreateAABB(pVer, uiVertexNum);
		}
		m_LocalBV = m_LocalBV.MergeAABB(NewAABB);

	}
}
void VSGeometry::CreateLocalAABB()
{
	LinkBoneNode();
	VSMAC_ASSERT(m_pMeshData && m_pMeshData->GetVertexBuffer())
	{
		AABB NewAABB;

		VSVertexBuffer * pVerBuffer = m_pMeshData->GetVertexBuffer();
		VSMAC_ASSERT(pVerBuffer->GetPositionData(0))


		Vector3 * pVer = (Vector3*)pVerBuffer->GetPositionData(0)->GetData();
		VSMAC_ASSERT(pVer);

		uint32 uiVertexNum = pVerBuffer->GetPositionData(0)->GetNum();
		VSTransform World  = m_pParent->GetWorldTransform();
		if (GetAffectSkeleton())
		{
			VSDataBuffer* pBlendIndex = pVerBuffer->GetBlendIndicesData(0);
			VSDataBuffer* pBoneWeight = pVerBuffer->GetBlendWeightData();

			if (!pBlendIndex || !pBoneWeight)
			{
				return ;
			}
			VSArray<Vector3>TempBuffer;
			TempBuffer.SetBufferNum(uiVertexNum);

			if (pBlendIndex->GetDT() == VSDataBuffer::DT_UBYTE4)
			{
				uint32 * pBlendIndexData = (uint32 *)pBlendIndex->GetData();
				uint32 * pBoneWeightData = (uint32 *)pBoneWeight->GetData();	
				for (uint32 i = 0; i < uiVertexNum ;i++)
				{
					Vector3W BoneWeight;
					BoneWeight.CreateFromABGR(pBoneWeightData[i]);
					unsigned char BlendIndex[4];
					ColorGetABGR(pBlendIndexData[i],BlendIndex[0],BlendIndex[1],BlendIndex[2],BlendIndex[3]);
					TempBuffer[i].Set(0.0f,0.0f,0.0f);
					for (uint32 k = 0 ; k < 4 ; k++)
					{

						VSBoneNode * pBone = GetAffectBone(BlendIndex[k]);
						if(pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							Matrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();	
							TempBuffer[i] += pVer[i] * TempBone * BoneWeight.m[k];
						}
					}
					
				}
			}
			else
			{
				Vector3W * pBlendIndexData = (Vector3W *)pBlendIndex->GetData();
				Vector3W * pBoneWeightData = (Vector3W *)pBoneWeight->GetData();	
				for (uint32 i = 0; i < uiVertexNum ;i++)
				{
					TempBuffer[i].Set(0.0f,0.0f,0.0f);
					for (uint32 k = 0 ; k < 4 ; k++)
					{
						uint32 BlendIndex = (unsigned int) pBlendIndexData[i].m[k];
						VSBoneNode * pBone = GetAffectBone(BlendIndex);
						if(pBone)
						{
							VSTransform BoneWorld = pBone->GetWorldTransform();

							Matrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();	
							TempBuffer[i] += pVer[i] * TempBone * pBoneWeightData[i].m[k];
						}
					}

				}
			}
			
			NewAABB.CreateAABB(TempBuffer.GetBuffer(),uiVertexNum);
		}
		else
		{		
			NewAABB.CreateAABB(pVer,uiVertexNum);
		}
		m_LocalBV = NewAABB;
		
	}
	
}
void VSGeometry::UpdateWorldBound(double dAppTime)
{
	m_WorldBV.Transform(m_LocalBV,m_World.GetCombine());
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;;
	}
	
}
void VSGeometry::UpdateNodeAll(double dAppTime)
{
	
	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}

	UpdateTransform(dAppTime);
	if (dAppTime > 0.0f)
	{
		UpdateOther(dAppTime);
	}
	if(m_bIsChanged)
	{
		UpdateWorldBound(dAppTime); 
	}
	m_bIsChanged = false;

	
}
void VSGeometry::UpdateView(VSCuller & Culler, double dAppTime)
{
	VSSpatial::UpdateView(Culler,dAppTime);
	VSCamera * pCamera = Culler.GetCamera();
	VSMAC_ASSERT(pCamera);

	if (Culler.GetCullerType() == VSCuller::CUT_MAIN)
	{
		
	}
}
void VSGeometry::ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime)
{
	if (Culler.CullGeometry(this))
	{
		return;
	}
	UpdateView(Culler,dAppTime);
	VSMeshNode * pMeshNode = GetMeshNode();
	VSMAC_ASSERT(pMeshNode);

	uint32 uiRenderGroup = pMeshNode->GetRenderGroup();
	VSMAC_ASSERT(uiRenderGroup < VSCuller::RG_MAX);

	VSMaterialInstance * pMaterialInstance = NULL;
	if (Culler.GetUseMaterialIndex() == -1 )
	{
		pMaterialInstance = GetUseMaterialInstance();
		if (!pMaterialInstance)
		{
			AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultResource());
			pMaterialInstance = GetUseMaterialInstance();
		}
	}
	else
	{
		pMaterialInstance = GetMaterialInstance(Culler.GetUseMaterialIndex());
	}
	if (!pMaterialInstance)
	{
		return;
	}

	VSMaterial * pMaterial = pMaterialInstance->GetMaterial();
	for (uint32 i = 0 ; i < pMaterial->GetShaderMainFunctionNum() ;i++)
	{
		
		VSRenderContext VisibleContext;
		VisibleContext.m_pGeometry = this;
		VisibleContext.m_pMaterialInstance = pMaterialInstance;
		VisibleContext.m_uiPassId = i;
		VisibleContext.m_pMaterial = pMaterial;
		VisibleContext.m_pMeshNode = pMeshNode;
		const VSBlendDesc & BlendDest = pMaterial->GetRenderState(i).GetBlendState()->GetBlendDesc();
		if (Culler.GetCullerType() == VSCuller::CUT_SHADOW)
		{

			if (BlendDest.IsBlendUsed())
			{
				return;
			}
			else
			{
				Culler.InsertObject(VisibleContext, VSCuller::VST_BASE, uiRenderGroup);
				return;
			}
		}
		else
		{
			if (pMaterial->GetCombine())
			{
				Culler.InsertObject(VisibleContext, VSCuller::VST_COMBINE, uiRenderGroup);
			}
			else
			{
				if (BlendDest.IsBlendUsed())
				{
					Culler.InsertObject(VisibleContext, VSCuller::VST_ALPHABLEND, uiRenderGroup);
				}
				else
					Culler.InsertObject(VisibleContext, VSCuller::VST_BASE, uiRenderGroup);
			}
		}	
	}
	
}
bool VSGeometry::IsSwapCull()
{
	VSMeshNode * pMeshNode = GetMeshNode();
	if (pMeshNode)
	{
		float fDet = pMeshNode->GetWorldTransform().GetRotate().Det();
		return (fDet < 0.0f);
	}
	else
	{
		return false;
	}

	
}
VSGeometry::VSGeometry()
{
	m_pMeshData = NULL;
	m_uiActiveNum = MATH_MAX_UINT32;
	m_uiCurUseMaterial = 0;
}
VSGeometry::~VSGeometry()
{
	m_pMeshData = NULL;
}
void VSGeometry::SetAffectBoneArray(const VSArray<VSBoneNode *> & pBoneNodeArray)
{
	if(pBoneNodeArray.GetNum())
	{
		m_pBoneNode.Clear();
		m_BoneName.Clear();
		m_pBoneNode = pBoneNodeArray;
		for (uint32 i = 0 ;i  < m_pBoneNode.GetNum(); i++)
		{
			if(m_pBoneNode[i])
				m_BoneName.AddElement(m_pBoneNode[i]->m_cName);
		}
	}

}
void VSGeometry::SetAffectBoneArray(const VSArray<VSUsedName> & BoneNodeArray)
{
	if(BoneNodeArray.GetNum())
	{
		m_pBoneNode.Clear();
		m_BoneName.Clear();
		m_BoneName = BoneNodeArray;
		//LinkBoneNode();
	}
}
bool VSGeometry::PostLoad(VSStream* pStream)
{
	if (!VSSpatial::PostLoad(pStream))
	{
		return false;
	}
	LinkBoneNode();
	if (m_pMaterialInstance.GetNum() == 0)
	{
		AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultResource());
	}
	return 1;
}
bool VSGeometry::PostClone(VSObject * pObjectSrc)
{
	if (!VSSpatial::PostClone(pObjectSrc))
	{
		return false;
	}
	LinkBoneNode();
	if (m_pMaterialInstance.GetNum() == 0)
	{
		AddMaterialInstance((VSMaterialR *)VSMaterial::GetDefaultResource());
	}
// 	VSGeometry * pGeometry = DynamicCast<VSGeometry>(pObjectSrc);
// 	m_MorphData = pGeometry->m_MorphData;
	return true;
}
void VSGeometry::LinkBoneNode()
{
	m_pBoneNode.Clear();
	if (m_BoneName.GetNum())
	{
		VSSkeleton * pSke = GetAffectSkeleton();
		if (pSke)
		{
			m_pBoneNode.Clear();
			for (uint32 i = 0 ; i < m_BoneName.GetNum() ; i++)
			{
				VSBoneNode * pBoneNode = pSke->GetBoneNode(m_BoneName[i]);
				if (!pBoneNode)
				{
					VSMAC_ASSERT(0);
				}
				m_pBoneNode.AddElement(pBoneNode);
			}
		}
	}
	m_SkinWeightBuffer.SetBufferNum(GetAffectBoneNum() * 3);
}
VSMeshNode * VSGeometry::GetMeshNode()const
{
	VSMeshNode * pMeshNode = NULL;
	VSSpatial* pParent = m_pParent;
	while(!pMeshNode && pParent)
	{
		pMeshNode = DynamicCast<VSMeshNode>(pParent);
		if (!pMeshNode)
		{
			pParent = pParent->GetParent();

		}
	}
	return pMeshNode;

}
VSSkeleton * VSGeometry::GetAffectSkeleton()const
{
	VSSkeletonMeshNode * pSkeMeshNode = NULL;
	VSSpatial* pParent = m_pParent;
	while(!pSkeMeshNode && pParent)
	{
		pSkeMeshNode = DynamicCast<VSSkeletonMeshNode>(pParent);
		if (!pSkeMeshNode)
		{
			pParent = pParent->GetParent();

		}
	}
	if (pSkeMeshNode)
	{
		return pSkeMeshNode->GetSkeleton();
	}
	else
	{
		return NULL;
	}
}
uint32 VSGeometry::GetVertexNum()const
{
	VSMAC_ASSERT(m_pMeshData);

	VSMAC_ASSERT(m_pMeshData->GetVertexBuffer());

	return m_pMeshData->GetVertexBuffer()->GetVertexNum();

}
void VSGeometry::SetMeshData(VSMeshData * pMeshData)
{
	m_pMeshData = pMeshData;
}
VSMeshData * VSGeometry::GetMeshData()const
{
	return m_pMeshData;
}
VSMeshData * VSGeometry::GetOriginMeshData()const
{
	return m_pMeshData;
}
uint32 VSGeometry::GetActiveNum()
{
	if (m_pMeshData)
	{
		if (m_pMeshData->GetIndexBuffer() && (m_uiActiveNum == MATH_MAX_UINT32))
			m_uiActiveNum = m_pMeshData->GetActiveNum();
		else if (m_uiActiveNum == MATH_MAX_UINT32)
			return 0;

		return m_uiActiveNum;

	}
	return 0;
}
bool VSGeometry::SetActiveNum(uint32 uiActiveNum)
{
	VSMAC_ASSERT(uiActiveNum <= GetTotalNum());
	m_uiActiveNum = uiActiveNum;
	return 1;

}
uint32 VSGeometry::GetTotalNum()const
{
	VSMAC_ASSERT(m_pMeshData);

	return m_pMeshData->GetTotalNum();
}
uint32 VSGeometry::GetMeshDataType()
{
	VSMAC_ASSERT(m_pMeshData);

	return m_pMeshData->GetMeshDataType();


}
uint32 VSGeometry::AddMaterialInstance(VSMaterialR * pMaterial)
{
	VSMAC_ASSERT(pMaterial);
	VSMaterialInstance * pMaterialInstance = MEM_NEW VSMaterialInstance(pMaterial);
	m_pMaterialInstance.AddElement(pMaterialInstance);

	return m_pMaterialInstance.GetNum();
}
uint32 VSGeometry::AddMaterialInstance(VSMaterialInstance * pMaterial)
{
	VSMAC_ASSERT(pMaterial && pMaterial->GetMaterial());
	VSMaterialInstance * pMaterialInstance = (VSMaterialInstance *)VSObject::CloneCreateObject(pMaterial);
	m_pMaterialInstance.AddElement(pMaterialInstance);

	return m_pMaterialInstance.GetNum();
}
bool VSGeometry::SetMaterialInstance(VSMaterialR * pMaterial, uint32 uiIndex)
{
	VSMAC_ASSERT(uiIndex < m_pMaterialInstance.GetNum() && pMaterial);

	VSMaterialInstance * pMaterialInstance = MEM_NEW VSMaterialInstance(pMaterial);
	m_pMaterialInstance[uiIndex] = pMaterialInstance;
	return true;

}
bool VSGeometry::SetMaterialInstance(VSMaterialInstance * pMaterial, uint32 uiIndex)
{
	VSMAC_ASSERT(uiIndex < m_pMaterialInstance.GetNum() && pMaterial);

	m_pMaterialInstance[uiIndex] = (VSMaterialInstance *)VSObject::CloneCreateObject(pMaterial);

	return true;
}
bool VSGeometry::SetUseMaterialInstance(uint32 uiIndex)
{
	VSMAC_ASSERT(uiIndex < m_pMaterialInstance.GetNum());
	m_uiCurUseMaterial = uiIndex;
	return true;
}
void VSGeometry::ClearAllMaterialInstance()
{
	m_pMaterialInstance.Clear();
}
void VSGeometry::DeleteMaterialInstance(uint32 i)
{
	VSMAC_ASSERT(i < m_pMaterialInstance.GetNum());
	m_pMaterialInstance[i] = NULL;
}
uint32 VSGeometry::DeleteMaterialInstance(VSMaterialInstance * pMaterial)
{
	VSMAC_ASSERT(pMaterial);
	for (uint32 i = 0; i < m_pMaterialInstance.GetNum(); i++)
	{
		if (pMaterial == m_pMaterialInstance[i])
		{
			m_pMaterialInstance[i] = NULL;
			return i;
		}
	}
	return MATH_MAX_UINT32;
}
VSMaterialInstance * VSGeometry::GetUseMaterialInstance()const
{
	VSMaterialInstance * pMaterialInstance = NULL;
	if (m_uiCurUseMaterial < m_pMaterialInstance.GetNum())
	{
		pMaterialInstance = m_pMaterialInstance[m_uiCurUseMaterial];
	}
	return pMaterialInstance;
}
VSMaterialInstance * VSGeometry::GetMaterialInstance(uint32 i)const
{
	VSMAC_ASSERT(i < m_pMaterialInstance.GetNum());
	return m_pMaterialInstance[i];
}
void VSGeometry::UpdateOther(double dAppTime)
{
	VSMAC_ASSERT(m_pMeshData);

	VSVertexBuffer * pVBuffer = GetMeshData()->GetVertexBuffer();
	if(GetAffectBoneNum() && pVBuffer->HaveBlendWeightInfo() && pVBuffer->HaveBlendIndicesInfo(0))
	{
		VSTransform World  = m_pParent->GetWorldTransform();
		
		for (uint32 i = 0 ; i < GetAffectBoneNum() ; i++)
		{
			VSBoneNode * pBone = GetAffectBone(i);
			if(pBone)
			{
				VSTransform BoneWorld = pBone->GetWorldTransform();
				Matrix3X3W TempBone = pBone->GetBoneOffsetMatrix() * BoneWorld.GetCombine() * World.GetCombineInverse();
				Vector3W ColumnVector[4];
				//TempBone.Identity();
				TempBone.GetColumnVector(ColumnVector);
				m_SkinWeightBuffer[i * 3] = ColumnVector[0];
				m_SkinWeightBuffer[i * 3 + 1] = ColumnVector[1];
				m_SkinWeightBuffer[i * 3 + 2] = ColumnVector[2];
			}
			else
			{
				m_SkinWeightBuffer[i * 3].Set(1.0f,0.0f,0.0f,0.0f);
				m_SkinWeightBuffer[i * 3 + 1].Set(0.0f,1.0f,0.0f,0.0f);
				m_SkinWeightBuffer[i * 3 + 2].Set(0.0f,0.0f,1.0f,0.0f);
			}

		}

	}
}
void VSGeometry::GetStreamResource(VSArray<VSResourceProxyBase*>& pResourceProxy, StreamInformation_TYPE& StreamInformation) const
{
	for (uint32 i = 0; i < m_pMaterialInstance.GetNum(); i++)
	{
		if (m_pMaterialInstance[i])
		{
			m_pMaterialInstance[i]->GetStreamResource(pResourceProxy, StreamInformation);
		}
	}
}
IMPLEMENT_RTTI(VSShadowVolumeGeometry,VSGeometry)
BEGIN_ADD_PROPERTY(VSShadowVolumeGeometry,VSGeometry)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSShadowVolumeGeometry)
IMPLEMENT_INITIAL_END
VSShadowVolumeGeometry::VSShadowVolumeGeometry()
{

}
VSShadowVolumeGeometry::~VSShadowVolumeGeometry()
{

}