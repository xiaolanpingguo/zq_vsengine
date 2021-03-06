#include "graphic/render/buffer/indexbuffer.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSIndexBuffer,VSBind)
BEGIN_ADD_PROPERTY(VSIndexBuffer,VSBind)
REGISTER_PROPERTY(m_pData,IndexData,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiNum,Num,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiDT,DataType,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSIndexBuffer)
IMPLEMENT_INITIAL_END
VSIndexBuffer::VSIndexBuffer()
{
	m_pData = NULL;
	m_uiNum = 0;
	m_uiDT = VSDataBuffer::DT_USHORT;
	m_pLockData = NULL;
}
VSIndexBuffer::~VSIndexBuffer()
{
	ReleaseResource();
	m_pData = NULL;
	m_pLockData = NULL;
}
void VSIndexBuffer::ClearInfo()
{
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
	{
		if (m_uiMemType == MT_VRAM)
		{
			m_pData = NULL;
		}
	}
}
bool VSIndexBuffer::OnLoadResource(VSResourceIdentifier *&pID)
{
	VSMAC_ASSERT(m_pUser);

	if(!m_pUser->OnLoadIBuffer(this,pID))
		return 0;
	return 1;
}
void *VSIndexBuffer::Lock()
{
	if (m_bIsStatic || !m_pUser)
	{
		return NULL;
	}

	VSMAC_ASSERT(m_pLockData == NULL);
	m_pLockData = m_pUser->Lock(this);


	return m_pLockData;
}
void VSIndexBuffer::UnLock()
{
	if (m_bIsStatic || !m_pUser)
	{
		return ;
	}

	VSMAC_ASSERT(m_pLockData);
	m_pUser->UnLock(this);
	m_pLockData = NULL;
}
bool VSIndexBuffer::SetData(VSDataBuffer * pData)
{
	VSMAC_ASSERT((pData->GetDT() == VSDataBuffer::DT_USHORT || pData->GetDT() == VSDataBuffer::DT_UINT)
		&& pData->GetNum() && pData->GetData());

	m_pData = pData;
	m_uiNum = pData->GetNum();
	m_uiDT = pData->GetDT();
	return 1;
}

uint32 VSIndexBuffer::GetNum()const
{
	return m_uiNum;
}
uint32 VSIndexBuffer::GetByteSize()const
{
	return VSDataBuffer::ms_uiDataTypeByte[m_uiDT] * m_uiNum;
}
VSIndexBuffer::VSIndexBuffer(uint32 uiNum,uint32 uiDT)
{
	VSMAC_ASSERT(uiNum);
	VSMAC_ASSERT(uiDT == VSDataBuffer::DT_USHORT || uiDT == VSDataBuffer::DT_UINT)
	m_pData = NULL;
	m_uiNum = uiNum;
	m_uiDT = uiDT;
	m_pLockData = NULL;
}