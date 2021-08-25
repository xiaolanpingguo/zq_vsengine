#include "graphic/material/shader/gshader.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSGShader,VSShader)
BEGIN_ADD_PROPERTY(VSGShader,VSShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSGShader)
IMPLEMENT_INITIAL_END
VSPointer<VSGShader> VSGShader::ms_Default;
VSGShader::VSGShader()
{

}
VSGShader::~VSGShader()
{

}
VSGShader::VSGShader(const char * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(pBuffer,MainFunName,IsFromFile)
{

}
VSGShader::VSGShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(Buffer,MainFunName,IsFromFile)
{

}
bool VSGShader::OnLoadResource(VSResourceIdentifier *&pID)
{
	VSMAC_ASSERT(m_pUser);

	if(!m_pUser->OnLoadGShaderProgram(this,pID))
		return 0;

	return 1;
}