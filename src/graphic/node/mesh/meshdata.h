#pragma once


#include "graphic/render/buffer/vertexbuffer.h"
#include "graphic/render/buffer/indexbuffer.h"


namespace zq{


class VSStream;
class GRAPHIC_API VSMeshData : public VSObject
{
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL_NO_CLASS_FACTORY;
public:
	enum //MeshData Type
	{
		MDT_POINT,
		MDT_LINE,
		MDT_TRIANGLE,
		MDT_CONTROL_POINT_4,
		MDT_MAX
	};

    VSMeshData();
	virtual ~VSMeshData() = 0;

    inline VSIndexBuffer* GetIndexBuffer()const
    {
        return m_pIndexBuffer;
    }

    inline VSVertexBuffer* GetVertexBuffer()const
    {
        return m_pVertexBuffer;
    }

    inline uint32 GetMeshDataType()
    {
        return m_uiMeshDataType;
    };

	bool SetIndexBuffer(VSIndexBuffer * pIndexBuffer);
	bool SetVertexBuffer(VSVertexBuffer *pVertexBuffer);

	virtual uint32 GetTotalNum()const = 0;
	virtual uint32 GetActiveNum()const{ return GetTotalNum();}
	virtual uint32 GetGirdNum(uint32 uiInputNum)const = 0;
protected:
		
	friend class VSGeometry;
	enum
	{
		DRAW_MATH_ELEMENT_LENGTH = 10000
	};
	VSVertexBufferPtr	m_pVertexBuffer;
	VSIndexBufferPtr	m_pIndexBuffer;
	uint32 m_uiMeshDataType;
};


DECLARE_Ptr(VSMeshData);
VSTYPE_MARCO(VSMeshData);
}

