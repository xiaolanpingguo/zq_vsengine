#pragma once


#include "core/system.h"
#include "core/memorymanager.h"
#include "core/file.h"


namespace zq{


class SYSTEM_API Image
{
public:
	enum // Image Format
	{
		IF_BMP,
		IF_TGA,
		IF_MAX
	};
	static char ms_ImageFormat[IF_MAX][10];
	Image();
	virtual ~Image() = 0;
	virtual bool Load(const char *pFileName) = 0;
	virtual bool LoadFromBuffer(unsigned char * pBuffer,uint32 uiSize) = 0;
	virtual const unsigned char *GetPixel(uint32 x, uint32 y)const = 0;
	inline uint32 GetBPP()const
	{
		return m_uiBPP;
	}
	inline uint32 GetWidth()const
	{
		return m_uiWidth;
	}
	inline uint32 GetHeight()const
	{
		return m_uiHeight;
	}
	inline const unsigned char *GetImg()const
	{
		return m_pImageData;
	}
	inline uint32 GetImgDataSize()const
	{
		return m_uiDataSize;
	}
	inline unsigned char* GetPalette()const   // Return a pointer to VGA palette
	{
		return m_pPalette;
	}
protected:
	uint32 m_uiWidth;
	uint32 m_uiHeight;
	uint32 m_uiBPP;
	unsigned char *m_pImageData;
	uint32 m_uiDataSize;
	unsigned char *m_pData;
	unsigned char *m_pPalette;
};
class SYSTEM_API VSBMPImage : public Image
{
public:
	VSBMPImage();
	~VSBMPImage();
	virtual bool Load(const char* pFilename);
	virtual bool LoadFromBuffer(unsigned char * pBuffer,uint32 uiSize);
	virtual const unsigned char *GetPixel(uint32 x, uint32 y)const;
		

private:
		
	uint32 m_uiEnc;
		
	uint32 m_uiPlanes;
	uint32 uiImgOffset;
		
		

	// Internal workers
	bool GetFile(const char* pFilename);
	bool ReadBmpHeader();
	bool LoadBmpRaw();
	bool LoadBmpRLE8();
	bool LoadBmpPalette();
	void FlipImg(); // Inverts image data, BMP is stored in reverse scanline order
		
};
class SYSTEM_API VSTGAImage : public Image
{
public:
	VSTGAImage();
	~VSTGAImage();
	virtual bool Load(const char* pFilename);
	virtual bool LoadFromBuffer(unsigned char * pBuffer,uint32 uiSize);
	virtual const unsigned char *GetPixel(uint32 x, uint32 y)const;
private:

	unsigned char m_cEnc;
		

	// Internal workers
	bool ReadHeader();
	bool LoadRawData();
	bool LoadTgaRLEData();
	bool LoadTgaPalette();
	void BGRtoRGB();
	void FlipImg();
};
	
class SYSTEM_API VSDDSImage : public Image
{
public:
	VSDDSImage()
	{

	}
	~VSDDSImage()
	{

	}
};
}
