#include "core/file.h"


namespace zq{


char File::ms_cOpenMode[OM_MAX][5] = 
{
	_T("rb"),
	_T("wb"),
	_T("rt"),
	_T("wt"),
};
uint32 File::m_uiSeekFlag[] = 
{
	SEEK_CUR,
	SEEK_END,
	SEEK_SET
};
bool File::IsFileExists(const char * pFileName)
{
	struct _stat64i32 kStat;
	if (_tstat(pFileName, &kStat) != 0)
	{
		return false;
	}
	return true;
}
File::File()
{
	m_pFileHandle = NULL;
	m_uiOpenMode = OM_MAX;
	m_uiFileSize = 0;
}
File::~File()
{
	if (m_pFileHandle)
	{
		fclose(m_pFileHandle);
		m_pFileHandle = NULL;
	}
}
bool File::Seek(uint32 uiOffset,uint32 uiOrigin)
{
	VSMAC_ASSERT(m_pFileHandle);
	fseek(m_pFileHandle,uiOffset,uiOrigin);
	return true;
}
bool File::Open(const char * pFileName,uint32 uiOpenMode)
{
	if (m_pFileHandle)
	{
		fclose(m_pFileHandle);
	}
	VSMAC_ASSERT(!m_pFileHandle);
	VSMAC_ASSERT(uiOpenMode < OM_MAX);
	uint32 uiLen = VSStrLen(pFileName);
	if (uiLen < VSMAX_PATH - 1)
	{
		if(!VSMemcpy(m_tcFileName,pFileName,uiLen + 1))
			return false;
	}
	else
	{
		return false;
	}

	m_uiOpenMode = uiOpenMode;
	if (m_uiOpenMode == OM_RB || m_uiOpenMode == OM_RT)
	{
		struct _stat64i32 kStat;
		if (_tstat(pFileName,&kStat) != 0)
		{
			return false;
		}
		m_uiFileSize = kStat.st_size;
	}

	errno_t uiError = _tfopen_s(&m_pFileHandle,pFileName,ms_cOpenMode[m_uiOpenMode]);
	if (uiError)
	{
		return 0;
	}
	if (!m_pFileHandle)
	{
		return 0;
	}

	return true;
}
bool File::Write(const void *pBuffer,uint32 uiSize,uint32 uiCount)
{
	VSMAC_ASSERT(m_pFileHandle);
	VSMAC_ASSERT(pBuffer);
	VSMAC_ASSERT(uiSize);
	VSMAC_ASSERT(uiCount);
	fwrite(pBuffer,uiSize,uiCount,m_pFileHandle);
	return true;
}
bool File::GetLine(void * pBuffer,uint32 uiSize)
{
	VSMAC_ASSERT(m_pFileHandle);
	VSMAC_ASSERT(pBuffer);
	VSMAC_ASSERT(uiSize);
	if (!_fgetts((char *)pBuffer,uiSize, m_pFileHandle))
		return false;
	return true;
}
bool File::Read(void *pBuffer,uint32 uiSize,uint32 uiCount)
{
	VSMAC_ASSERT(m_pFileHandle);
	VSMAC_ASSERT(pBuffer);
	VSMAC_ASSERT(uiSize);
	VSMAC_ASSERT(uiCount);
	fread(pBuffer,uiSize,uiCount,m_pFileHandle);
	return true;
}
bool File::Flush()
{
	return(fflush(m_pFileHandle) == 0);
}

}