#include "graphic/core/streamingtype.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/asyn/renderthread.h"
using namespace zq;
void VSStreamingType::ReleaseStreamObject()
{
	SAFE_DELETE(m_pToStreamObject);
}