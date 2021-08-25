#pragma once


#include "core/system.h"


namespace zq{


class SYSTEM_API File
{
public:
	enum //Open Mode
	{
		OM_RB,
		OM_WB,
		OM_RT,
		OM_WT,
		OM_MAX
	};
	enum 
	{
		VSMAX_PATH = 256
	};
	enum	//Seek Flag
	{
		SF_CUR,
		SF_END,
		SF_SET,
		SF_MAX

	};
	File();
	~File();
	bool Flush();

	bool Seek(uint32 uiOffset,uint32 uiOrigin);
	bool Open(const char * pFileName,uint32 uiOpenMode);
	bool Write(const void *pBuffer,uint32 uiSize,uint32 uiCount);
	bool Read(void *pBuffer,uint32 uiSize,uint32 uiCount);
	//这个函数返回pbuffer中，如果buffer大小大于取得的字符，则最后一个字符是回车，
	//倒数第2个是 LF    (Line Feed)
	bool GetLine(void * pBuffer,uint32 uiSize);
	inline uint32 GetFileSize()const
	{
		return m_uiFileSize;
	}
	static bool IsFileExists(const char * pFileName);
protected:
	static char ms_cOpenMode[OM_MAX][5];
	static uint32 m_uiSeekFlag[];
	FILE  *m_pFileHandle;
	uint32 m_uiOpenMode;
	uint32 m_uiFileSize;
	char m_tcFileName[VSMAX_PATH];
		
};
}
