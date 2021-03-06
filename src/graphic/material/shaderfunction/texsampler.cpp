#include "graphic/material/shaderfunction/texsampler.h"
#include "graphic/material/shaderstringfactory.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI_NoCreateFun(VSTexSampler,VSShaderFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSTexSampler)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSTexSampler,VSShaderFunction)
REGISTER_PROPERTY(m_pTexAllState,Texture,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiTexCoordLevel,TexCoordLevel,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiVEDecode,VEDecode,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiVESRGB,VESRGB,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_ENUM_PROPERTY(m_uiSamplerSource, SamplerSource, SamplerSource,VSProperty::F_SAVE_LOAD_CLONE)
BEGIN_ADD_ENUM
ADD_ENUM(SamplerSource, FROM_TEXTURE)
ADD_ENUM(SamplerSource, SHARE_CLAMP)
ADD_ENUM(SamplerSource, SHARE_WRAP)
END_ADD_ENUM
END_ADD_PROPERTY
VSTexSampler::~VSTexSampler()
{
	m_pTexAllState = NULL;
	m_uiTexCoordLevel = 0;
}
VSTexSampler::VSTexSampler(const VSUsedName & ShowName,VSMaterial * pMaterial)
			 :VSShaderFunction(ShowName,pMaterial)
{
	m_pTexAllState = NULL;
	m_uiTexCoordLevel = 0;
	m_uiVEDecode = 0;
	m_uiVESRGB = 0;
	m_uiSamplerSource = VSEngineFlag::SS_FROM_TEXTURE;
}
VSTexSampler::VSTexSampler()
{
	m_pTexAllState = NULL;
	m_uiTexCoordLevel = 0;
	m_uiVEDecode = 0;
	m_uiVESRGB = 0;
	m_uiSamplerSource = VSEngineFlag::SS_FROM_TEXTURE;
}
bool VSTexSampler::GetDeclareString(VSString &OutString, uint32 uiRegisterID, MaterialShaderPara &MSPara)const
{
	OutString += VSRenderer::ms_pRenderer->GetDeclareSampler(m_ShowName.GetString(), GetTexType(), uiRegisterID , m_uiSamplerSource);
	return true;
}
bool VSTexSampler::GetInputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSMAC_ASSERT(VSRenderer::ms_pRenderer);
	VSString Temp;
	if(m_pInput[0]->GetValueType() == VSPutNode::VT_1)
	{
		OutString +=VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("float ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if(m_pInput[0]->GetValueType() == VSPutNode::VT_2)
	{
		OutString +=VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ");*/
		Temp = VSRenderer::ms_pRenderer->Float2Const(_T("0"),_T("0"));/*_T("float2(0,0)");*/
	}
	else if(m_pInput[0]->GetValueType() == VSPutNode::VT_3)
	{
		OutString +=VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"),_T("0"),_T("0"));/*_T("float3(0,0,0)");*/
	}
	else if(m_pInput[0]->GetValueType() == VSPutNode::VT_4)
	{
		OutString +=VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		return 0;
	if(!m_pInput[0]->GetOutputLink())
	{
// 		if(!m_pTexAllState)
// 		{
// 			OutString += m_pInput[0]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");
// 			return 1;
// 		}
// 		else
// 		{
			OutString += m_pInput[0]->GetNodeName().GetString() + _T(" = ") + VSShaderStringFactory::ms_TextureInputCoordValue[m_uiTexCoordLevel] + _T(";\n");
			return 1;
/*		}*/
	}
// 	if(m_pInput[0]->GetValueType() != m_pInput[0]->GetOutputLink()->GetValueType())
// 		return 0;
	OutString += GetValueEqualString(m_pInput[0]->GetOutputLink(),m_pInput[0]);/*m_pInput[0]->GetNodeName() + _T(" = ") + m_pInput[0]->GetOutputLink()->GetNodeName() + _T(";\n");*/

	return 1;
}
