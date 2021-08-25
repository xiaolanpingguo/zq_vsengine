#include "graphic/render/bind.h"
#include "graphic/render/renderer.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
VSArray<VSBind *> VSBind::ms_BindArray;
IMPLEMENT_RTTI_NoCreateFun(VSBind,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSBind,VSObject)
END_ADD_PROPERTY
VSResourceIdentifier* VSBind::GetIdentifier ()
{
	if (!m_InfoArray.GetNum())
	{
		return NULL;
	}
	VSResourceIdentifier * pID = NULL;
	pID = m_InfoArray[0].ID;
	return pID;
}
uint32 VSBind::GetInfoQuantity () const
{
	return m_InfoArray.GetNum();
}
VSBind::VSBind()
{
	m_InfoArray.Clear();	
	m_pUser = NULL;
	for (uint32 i = 0 ; i < ms_BindArray.GetNum() ; i++)
	{
		if (ms_BindArray[i] == this)
		{
			VSMAC_ASSERT(0);
		}
	}
	ms_BindArray.AddElement(this);
}
VSBind::~VSBind()
{

	for (uint32 i = 0 ;i  < ms_BindArray.GetNum() ; i++)
	{
		if (ms_BindArray[i] == this)
		{
			ms_BindArray.Erase(i);
			//break;
		}
	}

}
void VSBind::Bind(VSResourceIdentifier* pID)
{
	if(!pID)
		return ;
	INFO_TYPE Info;
	Info.ID = pID;
	m_InfoArray.AddElement(Info);
}

bool VSBind::LoadResource(VSRenderer * pRender)
{
	if(!pRender)
		return 0;
	if (1 == m_InfoArray.GetNum())
	{
		return 1;
	}
	else
	{
		m_pUser = pRender;
		VSResourceIdentifier *pID = NULL;
		if (!OnLoadResource(pID))
			return 0;
		if (!pID)
			return 0;
		Bind(pID);
		return 1;	
	
	}
	
}
bool VSBind::ReleaseResource()
{
	for (uint32 i = 0 ; i < m_InfoArray.GetNum() ; i++)
	{
		INFO_TYPE &rInfo = m_InfoArray[i];
		if(!OnReleaseResource(rInfo.ID))
			return 0;
	}
	m_InfoArray.Clear();
	return 1;
}
bool VSBind::OnReleaseResource(VSResourceIdentifier *pID)
{
	if (!m_pUser)
		return 0;
	if (!m_pUser->OnReleaseResouce(pID))
		return 0;
	return 1;
}

