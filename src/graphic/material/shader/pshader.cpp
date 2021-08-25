#include "graphic/material/shader/pshader.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSPShader,VSShader)
BEGIN_ADD_PROPERTY(VSPShader,VSShader)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSPShader)
IMPLEMENT_INITIAL_END
VSPointer<VSPShader> VSPShader::ms_Default;
VSPShader::VSPShader()
{

}
VSPShader::VSPShader(const char * pBuffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(pBuffer,MainFunName,IsFromFile)
{

}
VSPShader::VSPShader(const VSString &Buffer,const VSString & MainFunName,bool IsFromFile)
:VSShader(Buffer,MainFunName,IsFromFile)
{

}
VSPShader::~VSPShader()
{

}


bool VSPShader::OnLoadResource(VSResourceIdentifier *&pID)
{
	VSMAC_ASSERT(m_pUser);

	if(!m_pUser->OnLoadPShaderProgram (this,pID))
		return 0;

	return 1;
}
