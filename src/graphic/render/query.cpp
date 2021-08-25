#include "graphic/render/query.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
IMPLEMENT_RTTI(VSQuery, VSBind)
BEGIN_ADD_PROPERTY(VSQuery, VSBind)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSQuery)
IMPLEMENT_INITIAL_END
bool VSQuery::OnLoadResource(VSResourceIdentifier *&pID)
{
	if (!m_pUser)
		return 0;
	if (!m_pUser->OnLoadQuery(this, pID))
		return 0;
	return 1;
}
VSQuery::VSQuery(uint32 uiQueryType)
{
	m_uiQueryType = uiQueryType;
}
bool VSQuery::GetQueryData(void * pData, uint32 uiDataSize)
{
	return m_pUser->GetData(this, pData, uiDataSize);
}
VSQuery::VSQuery()
{

}
VSQuery::~VSQuery()
{
	ReleaseResource();
}