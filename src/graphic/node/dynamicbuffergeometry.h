#pragma once


#include "graphic/node/geometry.h"
#include "graphic/node/usebuffer.h"
#include "graphic/render/buffer/vertexformat.h"


namespace zq{


class VSVertexBuffer;
class VSIndexBuffer;
class GRAPHIC_API VSDynamicBufferGeometry : public VSGeometry
{
	DECLARE_RTTI;
public:
	VSDynamicBufferGeometry();
	virtual ~VSDynamicBufferGeometry() = 0;
	virtual void ClearInfo() = 0 ;
	virtual bool HaveData() = 0;
	virtual uint32 UpdateGeometry() = 0;
	virtual void Draw(VSCamera * pCamera);
};


DECLARE_Ptr(VSDynamicBufferGeometry);


class GRAPHIC_API VSDVGeometry : public VSDynamicBufferGeometry
{
	DECLARE_RTTI;
public:
	VSDVGeometry();
	void Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray,
							uint32 uiMeshDataType,
							uint32 uiVertexNum);
	virtual ~VSDVGeometry();
	bool Add(const void * pVeretexData,uint32 uiVertexSize);
	void * NewGetV(uint32 uiVertexSize);
	//更新和视点无关的Mesh 信息
	virtual uint32 UpdateGeometry();
	virtual void ClearInfo();
	virtual bool HaveData()
	{
		if (!m_pVertexUseBuffer->GetElementNum())
		{
			return false;
		}
		return true;
	}
	uint32 GetMaxRenderVertexNum()const;
protected:
		
	VSUseBufferPtr m_pVertexUseBuffer;
	uint32 m_uiCurVUseBufferElementIndex;
	VSUseBufferPtr m_pVertexUseBufferRender;
};


DECLARE_Ptr(VSDVGeometry);

class GRAPHIC_API VSDVDIGeometry : public VSDynamicBufferGeometry
{
	DECLARE_RTTI;
public:
	VSDVDIGeometry();
	void Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray,
		uint32 uiMeshDataType,
		uint32 uiVertexNum,
		uint32 uiIndexNum);
	virtual ~VSDVDIGeometry();
	bool Add(const void * pVeretexData,uint32 uiVertexSize,
		const void * pIndexData,uint32 uiIndexSize);

	void * NewGetV(uint32 uiVertexSize);
	void * NewGetI(uint32 uiIndexSize);
	//更新和视点无关的Mesh 信息
	virtual uint32 UpdateGeometry();
	virtual void ClearInfo();
	virtual bool HaveData()
	{
		if (!m_pVertexUseBuffer->GetElementNum())
		{
			return false;
		}
		return true;
	}
protected:
	VSUseBufferPtr m_pVertexUseBuffer;
	VSUseBufferPtr m_pIndexUseBuffer;
	uint32 m_uiCurVUseBufferElementIndex;
	uint32 m_uiCurIUseBufferElementIndex;

	VSUseBufferPtr m_pVertexUseBufferRender;
	VSUseBufferPtr m_pIndexUseBufferRender;
};

DECLARE_Ptr(VSDVDIGeometry);

class GRAPHIC_API VSSVDIGeometry : public VSDynamicBufferGeometry
{
	DECLARE_RTTI;
public:
	VSSVDIGeometry();
	void Create(uint32 uiMeshDataType,VSVertexBuffer * pVertexBuffer,uint32 uiIndexNum);
	virtual ~VSSVDIGeometry();
	bool Add(const void * pIndexData,uint32 uiIndexSize);
	void * NewGetI(uint32 uiIndexSize);
	//更新和视点无关的Mesh 信息
	virtual uint32 UpdateGeometry();
	virtual void ClearInfo();
	virtual bool HaveData()
	{
		if (!m_pIndexUseBuffer->GetElementNum())
		{
			return false;
		}
		return true;
	}
protected:
		
	VSUseBufferPtr m_pIndexUseBuffer;
	uint32 m_uiCurIUseBufferElementIndex;
	VSUseBufferPtr m_pIndexUseBufferRender;
};


DECLARE_Ptr(VSSVDIGeometry);


class GRAPHIC_API VSDVSIGeometry : public VSDynamicBufferGeometry
{
	DECLARE_RTTI;
public:
	VSDVSIGeometry();
	void Create(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray,
				uint32 uiMeshDataType,
				uint32 uiVertexNum,
				VSIndexBuffer * pIndexBuffer);
	virtual ~VSDVSIGeometry();
	bool Add(const void * pVeretexData,uint32 uiVertexSize);
	void * NewGetV(uint32 uiVertexSize);
	//更新和视点无关的Mesh 信息
	virtual uint32 UpdateGeometry();
	virtual void ClearInfo();
	virtual bool HaveData()
	{
		if (!m_pVertexUseBuffer->GetElementNum())
		{
			return false;
		}
		return true;
	}
protected:

	VSUseBufferPtr m_pVertexUseBuffer;
	uint32 m_uiCurVUseBufferElementIndex;
	VSUseBufferPtr m_pVertexUseBufferRender;
};

DECLARE_Ptr(VSDVSIGeometry);


}
