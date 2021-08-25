#include "graphic/render/buffer/vertexformat.h"
#include "graphic/render/buffer/vertexbuffer.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSVertexFormat,VSBind)
BEGIN_ADD_PROPERTY(VSVertexFormat,VSBind)
REGISTER_PROPERTY(m_uiVertexFormatCode,VertexFormatCode,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_FormatArray,FormatArray,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSVertexFormat)
IMPLEMENT_INITIAL_END
VSVertexFormat::~VSVertexFormat()
{
	ReleaseResource();
 	
}
VSVertexFormat::VSVertexFormat()
{
	

}


bool VSVertexFormat::OnLoadResource(VSResourceIdentifier *&pID)
{
	if(!m_pUser->OnLoadVBufferFormat (this,pID))
		return 0;

	return 1;
}
