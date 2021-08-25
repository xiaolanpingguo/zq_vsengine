#include "graphic/material/shader/hshader.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSHShader,VSShader)
BEGIN_ADD_PROPERTY(VSHShader,VSShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSHShader)
IMPLEMENT_INITIAL_END
VSPointer<VSHShader> VSHShader::ms_Default;
VSHShader::VSHShader()
{

}
VSHShader::~VSHShader()
{

}
VSHShader::VSHShader(const char * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(pBuffer,MainFunName,IsFromFile)
{

}
VSHShader::VSHShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(Buffer,MainFunName,IsFromFile)
{

}
bool VSHShader::OnLoadResource(VSResourceIdentifier *&pID)
{
	VSMAC_ASSERT(m_pUser);

	if(!m_pUser->OnLoadHShaderProgram(this,pID))
		return 0;

	return 1;
}