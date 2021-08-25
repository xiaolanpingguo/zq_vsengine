#include "graphic/material/shader/vshader.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSVShader,VSShader)
BEGIN_ADD_PROPERTY(VSVShader,VSShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSVShader)
IMPLEMENT_INITIAL_END
VSPointer<VSVShader> VSVShader::ms_Default;
VSVShader::VSVShader()
{

}
VSVShader::~VSVShader()
{

}
VSVShader::VSVShader(const char * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(pBuffer,MainFunName,IsFromFile)
{

}
VSVShader::VSVShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(Buffer,MainFunName,IsFromFile)
{

}
bool VSVShader::OnLoadResource(VSResourceIdentifier *&pID)
{
	VSMAC_ASSERT(m_pUser);


	if(!m_pUser->OnLoadVShaderProgram(this,pID))
		return 0;

	return 1;
}