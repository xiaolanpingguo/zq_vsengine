#pragma once


#include "graphic/render/membind.h"
#include "graphic/render/renderer.h"
#include "datastruct/VSBitArray.h"
#include "graphic/render/slot.h"


namespace zq{


class VSStream;
class GRAPHIC_API VSTexture : public VSMemBind, public VSSlot
{
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL_NO_CLASS_FACTORY;
public:
	enum	//Tex Type
	{
		TT_1D,
		TT_2D,
		TT_3D,
		TT_CUBE,
		TT_2DARRAY,
		TT_MAX
	};
protected:
	VSTexture(uint32 uiFormatType, uint32 uiWidth,
		uint32 uiHeight, uint32 uiLength, uint32 uiArraySize = 1,
				uint32 uiMipLevel = 0,bool bIsStatic = true,bool bSRGB = false);
	VSTexture();
public:
	virtual ~VSTexture()= 0;


    inline uint32 GetFormatType()const
    {
        return m_uiFormatType;
    }

    inline uint32 GetBytePerPixel()const
    {
        return VSRenderer::GetBytesPerPixel(m_uiFormatType);
    }

    inline uint32 GetChannelPerPixel()const
    {
        return VSRenderer::GetChannelPerPixel(m_uiFormatType);
    }

    inline uint32 GetMipLevel()const
    {
        return m_uiMipLevel;
    }

    inline bool GetSRGB()const
    {
        return m_bSRGB;
    }

    inline bool IsCompress()const
    {
        if (m_uiFormatType >= VSRenderer::SFT_BC1 && m_uiFormatType <= VSRenderer::SFT_BC5)
        {
            return true;
        }

        return false;
    }

    inline bool IsHasAlpha()const
    {
        if (m_uiFormatType == VSRenderer::SFT_A8R8G8B8 ||
            m_uiFormatType == VSRenderer::SFT_A16B16G16R16F || m_uiFormatType == VSRenderer::SFT_A32B32G32R32F
            || m_uiFormatType == VSRenderer::SFT_BC2 || m_uiFormatType == VSRenderer::SFT_BC3)
        {
            return true;
        }

        return false;
    }

    virtual uint32 GetArraySize()const
    {
        return m_uiArraySize;
    }

    inline void* GetLockDataPtr(uint32 uiLevel = 0, uint32 uiFace = 0)const
    {
        VSMAC_ASSERT(uiLevel < GetMipLevel() && uiFace < 6);
        return m_pLockData[uiLevel][uiFace];
    }


	virtual uint32 GetTexType()const = 0;
	virtual void SetMipLevel() = 0;
	unsigned char * GetBuffer(uint32 uiLevel, uint32 uiFace = 0)const;
	virtual uint32 GetByteSize(uint32 uiLevel)const;
	virtual uint32 GetMemPitch(uint32 uiLevel)const;
	virtual uint32 GetMemSlicePitch(uint32 uiLevel)const;
	virtual uint32 GetByteSize()const;
	uint32 GetWidth(uint32 uiLevel)const;
	uint32 GetHeight(uint32 uiLevel)const;
	uint32 GetLength(uint32 uiLevel)const;
		
	void CreateRAMData();
	void CopyRAMData(const VSTexture* pSourceTexture);
		
	void *Lock(uint32 uiLevel = 0,uint32 uiFace = 0);
	void UnLock(uint32 uiLevel = 0,uint32 uiFace = 0);

	virtual void ClearInfo();
				
protected:
	VSArray<VSArray<unsigned char>> m_DataBufferArray;
	uint32 m_uiArraySize;
	uint32 m_uiFormatType;
	uint32 m_uiWidth;
	uint32 m_uiHeight;
	uint32 m_uiLength;
	uint32 m_uiMipLevel;		
	void * m_pLockData[MAX_MIP_LEVEL][6];
	bool	m_bSRGB;
protected:
	virtual bool OnLoadResource(VSResourceIdentifier *&pID);		
	virtual bool SetOutput(class VSOutputResource *pOutputResource);
	VSArray<class VSOutputResource *> m_pOutputResource;
	class VSTextureOutputInfo * GetTextureOutputInfo(class VSOutputResource * pOutputResource);
};

DECLARE_Ptr(VSTexture);
VSTYPE_MARCO(VSTexture);

class GRAPHIC_API VSTextureCache :public VSCacheResource
{
public:
	virtual ~VSTextureCache();
	VSTextureCache(VSObject * pCacheObject = NULL);
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL;
	DECLARE_CACHE_RESOURCE(CacheTexture, Resource/Texture, Cache/Texture, false,false)
	virtual bool SetCacheResource(VSObject* pOwner);
	virtual bool SetStreamResouce(VSResourceProxyBase* pOwner);
	inline const VSTexture* GetTexture() const
	{
		return m_pTex;
	}
protected:
	VSTexturePtr		m_pTex;
};


DECLARE_Ptr(VSTextureCache);
VSTYPE_MARCO(VSTextureCache);


}