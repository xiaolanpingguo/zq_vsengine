#pragma once


#include "graphic/core/object.h"
#include "graphic/render/membind.h"
#include "graphic/render/bindlockinterface.h"
#include "graphic/render/buffer/databuffer.h"
#include "math/maths.h"
#include "graphic/render/buffer/vertexformat.h"


namespace zq{


class VSStream;
class GRAPHIC_API VSVertexBuffer : public VSMemBind, public VSBindLockInterface
{
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	friend class VSVertexFormat;
	friend class VSResourceManager;
	VSVertexBuffer(bool bIsStatic);
	VSVertexBuffer(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE>& FormatArray,uint32 uiNum);
	virtual ~VSVertexBuffer();

    inline VSDataBuffer* GetPositionData(uint32 uiLevel)const
    {
        if (uiLevel < (unsigned int)m_pData[VSVertexFormat::VF_POSITION].GetNum())
            return m_pData[VSVertexFormat::VF_POSITION][uiLevel];
        else
            return NULL;
    }

    inline VSDataBuffer* GetNormalData(uint32 uiLevel)const
    {
        if (uiLevel < (unsigned int)m_pData[VSVertexFormat::VF_NORMAL].GetNum())
            return m_pData[VSVertexFormat::VF_NORMAL][uiLevel];
        else
            return NULL;
    }

    inline VSDataBuffer* GetPSizeData()const
    {
        if (m_pData[VSVertexFormat::VF_PSIZE].GetNum())
            return m_pData[VSVertexFormat::VF_PSIZE][0];
        else
            return NULL;
    }

    inline VSDataBuffer* GetColorData(uint32 uiLevel)const
    {
        if (uiLevel < (unsigned int)m_pData[VSVertexFormat::VF_COLOR].GetNum())
            return m_pData[VSVertexFormat::VF_COLOR][uiLevel];
        else
            return NULL;
    }

    inline VSDataBuffer* GetTexCoordData(uint32 uiLevel)const
    {
        if (uiLevel < (unsigned int)m_pData[VSVertexFormat::VF_TEXCOORD].GetNum())
            return m_pData[VSVertexFormat::VF_TEXCOORD][uiLevel];
        else
            return NULL;
    }

    inline VSDataBuffer* GetMaterialInstanceData(uint32 uiLevel)const
    {
        if (uiLevel < (unsigned int)m_pData[VSVertexFormat::VF_MATERIAL_INSTANCE].GetNum())
            return m_pData[VSVertexFormat::VF_MATERIAL_INSTANCE][uiLevel];
        else
            return NULL;
    }

    inline VSDataBuffer* GetBlendWeightData()const
    {
        if (m_pData[VSVertexFormat::VF_BLENDWEIGHT].GetNum())
            return m_pData[VSVertexFormat::VF_BLENDWEIGHT][0];
        else
            return NULL;
    }

    inline VSDataBuffer* GetBlendIndicesData(uint32 uiLevel)const
    {
        if (uiLevel < m_pData[VSVertexFormat::VF_BLENDINDICES].GetNum())
            return m_pData[VSVertexFormat::VF_BLENDINDICES][uiLevel];
        else
            return NULL;
    }

    inline VSDataBuffer* GetTangentData()const
    {
        if (m_pData[VSVertexFormat::VF_TANGENT].GetNum())
            return m_pData[VSVertexFormat::VF_TANGENT][0];
        else
            return NULL;
    }

    inline VSDataBuffer* GetBinormalData()const
    {
        if (m_pData[VSVertexFormat::VF_BINORMAL].GetNum())
            return m_pData[VSVertexFormat::VF_BINORMAL][0];
        else
            return NULL;
    }

    inline VSDataBuffer* GetFogData()const
    {
        if (m_pData[VSVertexFormat::VF_FOG].GetNum())
            return m_pData[VSVertexFormat::VF_FOG][0];
        else
            return NULL;
    }

    inline VSDataBuffer* GetVertexIDData()const
    {
        if (m_pData[VSVertexFormat::VF_VERTEX_ID].GetNum())
            return m_pData[VSVertexFormat::VF_VERTEX_ID][0];
        else
            return NULL;
    }

    inline uint32 GetPositionLevel()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_POSITION);
    }

    inline uint32 GetBlendIndicesLevel()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_BLENDINDICES);
    }

    inline uint32 GetColorLevel()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_COLOR);
    }

    inline uint32 GetNormalLevel()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_NORMAL);
    }

    inline uint32 GetTexCoordLevel()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_TEXCOORD);
    }

    inline uint32 GetMaterialInstanceLevel()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_MATERIAL_INSTANCE);
    }

    inline uint32 GetVertexNum()const
    {
        return m_uiVertexNum;
    }

    inline VSVertexFormat * VSVertexBuffer::GetVertexFormat()const
    {
        return m_pVertexFormat;
    }

    inline uint32 GetOneVertexSize()const
    {
        return m_uiOneVertexSize;
    }

    inline VSDataBuffer* GetData(uint32 uiVF, uint32 uiLevel)const
    {
        if (uiVF >= VSVertexFormat::VF_MAX)
            return NULL;
        if (uiLevel >= (unsigned int)m_pData[uiVF].GetNum())
            return NULL;
        return m_pData[uiVF][uiLevel];
    }

    inline uint32 GetLevel(uint32 uiVF)const
    {
        if (uiVF >= VSVertexFormat::VF_MAX)
            return 0;
        else
            return GetSemanticsNum(uiVF);
    }

    inline bool GetLockDataOffset(uint32 uiVF, uint32 uiLevel, uint32 &uiOffset)const
    {
        if (!m_pVertexFormat)
        {
            VSDataBuffer * pData = NULL;
            for (uint32 i = 0; i < uiVF; i++)
            {
                for (uint32 j = 0; j < m_pData[i].GetNum(); j++)
                {
                    pData = GetData(i, j);
                    if (pData)
                        uiOffset += pData->GetStride();
                }
            }
            for (uint32 j = 0; j < uiLevel; j++)
            {
                pData = GetData(uiVF, j);
                if (pData)
                    uiOffset += pData->GetStride();
            }
            if (!pData)
            {
                return false;
            }
            return true;
        }
        else
        {
            for (uint32 i = 0; i < m_pVertexFormat->m_FormatArray.GetNum(); i++)
            {
                if (m_pVertexFormat->m_FormatArray[i].Semantics == uiVF)
                {
                    if (m_pVertexFormat->m_FormatArray[i].SemanticsIndex == uiLevel)
                    {
                        uiOffset = m_pVertexFormat->m_FormatArray[i].Offset;
                        return true;
                    }
                }
            }

            return false;
        }
    }

    inline void *GetLockData(uint32 uiVF, uint32 uiLevel)const
    {
        if (uiVF >= VSVertexFormat::VF_MAX || !m_pLockData)
            return NULL;
        if (GetSemanticsNum(uiVF) <= uiLevel)
            return NULL;

        uint32 uiOffset = 0;
        if (!GetLockDataOffset(uiVF, uiLevel, uiOffset))
            return	NULL;

        return (unsigned char *)m_pLockData + uiOffset;
    }

    inline void *GetLockPositionData(uint32 uiLevel)const
    {
        return GetLockData(VSVertexFormat::VF_POSITION, uiLevel);
    }

    inline void *GetLockNormalData(uint32 uiLevel)const
    {
        return GetLockData(VSVertexFormat::VF_NORMAL, uiLevel);
    }

    inline void *GetLockPSizeData()const
    {
        return GetLockData(VSVertexFormat::VF_PSIZE, 0);
    }

    inline void *GetLockColorData(uint32 uiLevel)const
    {
        return GetLockData(VSVertexFormat::VF_COLOR, uiLevel);
    }

    inline void *GetLockBlendWeightData()const
    {
        return GetLockData(VSVertexFormat::VF_BLENDWEIGHT, 0);
    }

    inline void *GetLockBlendIndicesData(uint32 uiLevel)const
    {
        return GetLockData(VSVertexFormat::VF_BLENDINDICES, uiLevel);
    }

    inline void *GetLockTangentData()const
    {
        return GetLockData(VSVertexFormat::VF_TANGENT, 0);
    }

    inline void *GetLockBinormalData()const
    {
        return GetLockData(VSVertexFormat::VF_BINORMAL, 0);
    }

    inline void *GetLockFogData()const
    {
        return GetLockData(VSVertexFormat::VF_FOG, 0);
    }

    inline void *GetLockVertexIDData()const
    {
        return GetLockData(VSVertexFormat::VF_VERTEX_ID, 0);
    }

    inline void* GetLockTexCoordData(uint32 uiLevel)const
    {
        return GetLockData(VSVertexFormat::VF_TEXCOORD, uiLevel);
    }

    inline void* GetLockMaterialInstanceData(uint32 uiLevel)const
    {
        return GetLockData(VSVertexFormat::VF_MATERIAL_INSTANCE, uiLevel);
    }

    inline bool HavePositionInfo(uint32 uiLevel)const
    {
        return GetSemanticsNum(VSVertexFormat::VF_POSITION) > uiLevel;
    }

    inline bool HaveNormalInfo(uint32 uiLevel)const
    {
        return GetSemanticsNum(VSVertexFormat::VF_NORMAL) > uiLevel;
    }

    inline bool HavePSizeInfo()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_PSIZE) > 0;
    }

    inline bool HaveColorInfo(uint32 uiLevel)const
    {
        return GetSemanticsNum(VSVertexFormat::VF_COLOR) > uiLevel;
    }

    inline bool HaveBlendWeightInfo()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_BLENDWEIGHT) > 0;
    }

    inline bool HaveBlendIndicesInfo(uint32 uiLevel)const
    {
        return GetSemanticsNum(VSVertexFormat::VF_BLENDINDICES) > uiLevel;
    }

    inline bool HaveTangentInfo()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_TANGENT) > 0;
    }

    inline bool HaveBinormalInfo()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_BINORMAL) > 0;
    }

    inline bool HaveFogInfo()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_FOG) > 0;
    }

    inline bool HaveVertexIDInfo()const
    {
        return GetSemanticsNum(VSVertexFormat::VF_VERTEX_ID) > 0;
    }

    inline bool HaveTexCoordInfo(uint32 uiLevel)const
    {
        return GetSemanticsNum(VSVertexFormat::VF_TEXCOORD) > uiLevel;
    }

    inline bool HaveMaterialInstanceInfo(uint32 uiLevel)const
    {
        return GetSemanticsNum(VSVertexFormat::VF_MATERIAL_INSTANCE) > uiLevel;
    }

    inline uint32 PositionChannel(uint32 uiLevel)const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_POSITION, uiLevel);
    }

    inline uint32 NormalChannel(uint32 uiLevel)const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_NORMAL, uiLevel);
    }

    inline uint32 PSizeChannel()const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_PSIZE, 0);
    }

    inline uint32 ColorChannel(uint32 uiLevel)const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_COLOR, uiLevel);
    }

    inline uint32 BlendWeightChannel()const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_BLENDWEIGHT, 0);
    }

    inline uint32 BlendIndicesChannel(uint32 uiLevel)const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_BLENDINDICES, uiLevel);
    }

    inline uint32 TangentChannel()const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_TANGENT, 0);
    }

    inline uint32 BinormalChannel()const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_BINORMAL, 0);
    }

    inline uint32 FogChannel()const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_FOG, 0);
    }

    inline uint32 VertexIDChannel()const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_VERTEX_ID, 0);
    }

    inline uint32 TexCoordChannel(uint32 uiLevel)const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_TEXCOORD, uiLevel);
    }

    inline uint32 MaterialInstanceChannel(uint32 uiLevel)const
    {
        return GetSemanticsChannel(VSVertexFormat::VF_MATERIAL_INSTANCE, uiLevel);
    }

    inline uint32 NormalDataType(uint32 uiLevel)const
    {
        return GetSemanticsDataType(VSVertexFormat::VF_NORMAL, uiLevel);
    }

    inline uint32 TangentDataType()const
    {
        return GetSemanticsDataType(VSVertexFormat::VF_TANGENT, 0);
    }

    inline uint32 BinormalDataType()const
    {
        return GetSemanticsDataType(VSVertexFormat::VF_BINORMAL, 0);
    }

    inline void* GetLockDataPtr()const
    {
        return m_pLockData;
    }

    virtual	bool LoadResource(VSRenderer * pRender);
    virtual uint32 GetByteSize()const;

	bool SetData(VSDataBuffer * pData,uint32 uiVF);
	bool GetVertexFormat(VSArray<VSVertexFormat::VERTEXFORMAT_TYPE> &FormatArray);

	uint32 GetSemanticsNum(uint32 uiSemantics)const;
	uint32 GetSemanticsChannel(uint32 uiSemantics,uint32 uiLevel)const;
	uint32 GetSemanticsDataType(uint32 uiSemantics,uint32 uiLevel)const;

    void *Lock();
    void UnLock();
    virtual void ClearInfo();

protected:
	friend class VSVertexFormat;
	friend class VSGeometry;
	VSVertexBuffer();
	VSArray<VSDataBufferPtr> m_pData[VSVertexFormat::VF_MAX];
	uint32 m_uiVertexNum;
	uint32 m_uiOneVertexSize;

	VSVertexFormatPtr m_pVertexFormat;
		
	void * m_pLockData;

protected:

	virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
};

DECLARE_Ptr(VSVertexBuffer);
VSTYPE_MARCO(VSVertexBuffer);
}
