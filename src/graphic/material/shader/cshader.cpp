#include "graphic/material/shader/cshader.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSCShader, VSShader)
BEGIN_ADD_PROPERTY(VSCShader, VSShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSCShader)
IMPLEMENT_INITIAL_END
VSPointer<VSCShader> VSCShader::ms_Default;
VSCShader::VSCShader()
{

}
VSCShader::~VSCShader()
{

}
VSCShader::VSCShader(const char * pBuffer, const VSString & MainFunName, bool IsFromFile)
	:VSShader(pBuffer, MainFunName, IsFromFile)
{

}
VSCShader::VSCShader(const VSString &Buffer, const VSString & MainFunName, bool IsFromFile)
	: VSShader(Buffer, MainFunName, IsFromFile)
{

}
bool VSCShader::OnLoadResource(VSResourceIdentifier *&pID)
{
	VSMAC_ASSERT(m_pUser);

	if (!m_pUser->OnLoadCShaderProgram(this, pID))
		return 0;

	return 1;
}