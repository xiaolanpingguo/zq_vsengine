#ifdef PLATFORM_WIN


#include "graphic/dx11/shaderinclude_dx11.h"
#include "core/file.h"
#include "graphic/core/resourcemanager.h"
#include "graphic/material/shader/shader.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/config.h"


namespace zq{


VSDx11ShaderInclude::VSDx11ShaderInclude()
{

}
VSDx11ShaderInclude::~VSDx11ShaderInclude()
{

}
HRESULT VSDx11ShaderInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	VSMAC_ASSERT(D3D_INCLUDE_LOCAL == IncludeType);

	VSString FileName(pFileName);
	unsigned int uiIndex = m_FileMap.Find(FileName);
	if (uiIndex != m_FileMap.GetNum())
	{
		*ppData = (void *)m_FileMap[uiIndex].Value.GetBuffer();
		*pBytes = m_FileMap[uiIndex].Value.GetLength();
	}
	else
	{
		File DynamicShaderFile;
		VSString DynamicShaderPath = /*VSShader::ms_ResourcePath +*/ FileName;
		if(!DynamicShaderFile.Open(DynamicShaderPath.GetBuffer(),File::OM_RB))
		{
			VSString PreShaderPath = VSRenderer::GetRenderTypeShaderPath(VSRenderer::RAT_DIRECTX11);
			DynamicShaderPath = VSShaderMapCache::ms_ResourcePath + PreShaderPath + FileName;
			if (!DynamicShaderFile.Open(DynamicShaderPath.GetBuffer(),File::OM_RB))
			{
				return S_FALSE;
			}
		}
		unsigned int uiSize = DynamicShaderFile.GetFileSize();
		if (!uiSize)
		{
			return S_FALSE;
		}
		VSString VDynamicShaderString;
		VDynamicShaderString.SetTCHARBufferNum(uiSize);
		if(!DynamicShaderFile.Read(VDynamicShaderString.GetBuffer(),uiSize,1))
		{
			return S_FALSE;
		}
		m_FileMap.AddElement(FileName,VDynamicShaderString);
		*ppData = (void *)m_FileMap[m_FileMap.GetNum() - 1].Value.GetBuffer();
		*pBytes = m_FileMap[m_FileMap.GetNum() - 1].Value.GetLength();

	}
	

	return S_OK;
}

HRESULT VSDx11ShaderInclude::Close(LPCVOID pData)
{
	// do nothing
	return S_OK;
}

}
#endif