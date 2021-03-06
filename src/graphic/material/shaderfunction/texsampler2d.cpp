#include "graphic/material/shaderfunction/texsampler2d.h"
#include "graphic/material/shaderstringfactory.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;

IMPLEMENT_RTTI(VS2DTexSampler,VSTexSampler)
BEGIN_ADD_PROPERTY(VS2DTexSampler,VSTexSampler)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VS2DTexSampler)
IMPLEMENT_INITIAL_END

VS2DTexSampler::VS2DTexSampler(const VSUsedName & ShowName,VSMaterial * pMaterial)
				:VSTexSampler(ShowName,pMaterial)
{
	
	

	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("Tex2DSamplerInput") + InputID;
	VSInputNode * pInputNode = NULL;
	pInputNode = MEM_NEW VSInputNode(VSPutNode::VT_2,InputName,this);
	VSMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	
	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("Tex2DSamplerOutput") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = MEM_NEW VSOutputNode(VSPutNode::VT_4,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputNameR = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR),VSRenderer::VE_R);

	pOutputNode = MEM_NEW VSOutputNode(VSPutNode::VT_1,OutputNameR,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameG = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR),VSRenderer::VE_G);

	pOutputNode = MEM_NEW VSOutputNode(VSPutNode::VT_1,OutputNameG,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameB = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR),VSRenderer::VE_B);

	pOutputNode = MEM_NEW VSOutputNode(VSPutNode::VT_1,OutputNameB,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	VSString OutputNameA = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR),VSRenderer::VE_A);

	pOutputNode = MEM_NEW VSOutputNode(VSPutNode::VT_1,OutputNameA,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

}
void VS2DTexSampler::ResetInShaderName(MaterialShaderPara &MSPara)
{
	VSString InputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString InputName = _T("Tex2DSamplerInput") + InputID;
	m_pInput[0]->SetNodeName(InputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("Tex2DSamplerOutput") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;


	VSString OutputNameR = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR),VSRenderer::VE_R);
	m_pOutput[1]->SetNodeName(OutputNameR);

	VSString OutputNameG = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR),VSRenderer::VE_G);
	m_pOutput[2]->SetNodeName(OutputNameG);


	VSString OutputNameB = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR),VSRenderer::VE_B);
	m_pOutput[3]->SetNodeName(OutputNameB);

	VSString OutputNameA = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR),VSRenderer::VE_A);
	m_pOutput[4]->SetNodeName(OutputNameA);
}
VS2DTexSampler::VS2DTexSampler()
{

}
VS2DTexSampler::~VS2DTexSampler()
{

}
bool VS2DTexSampler::GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if(VSRenderer::ms_pRenderer)
		OutString += VSRenderer::ms_pRenderer->Tex2D(this, MSPara.uiCompileShaderType);

	if (m_uiVESRGB)
	{
		VSString NodeString = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR),m_uiVESRGB);

		OutString += NodeString + _T(" = ");
		VSRenderer::ms_pRenderer->GreaterZeroPow(NodeString,2.2f,OutString);
		OutString += _T(";\n");
	}

	if (m_uiVEDecode)
	{
		VSString NodeString = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_COLOR),m_uiVEDecode);

		OutString += NodeString + _T(" = ");
		VSRenderer::ms_pRenderer->DecodeNormal1(NodeString,OutString);
		OutString += _T(";\n");
	}
	
	
	
	return 1;
}
void VS2DTexSampler::SetTexture(VSTexAllStateR * pTexAllState)
{
	VSMAC_ASSERT(pTexAllState);
	VSMAC_ASSERT(pTexAllState->GetResource()->GetTexture()->GetTexType() == VSTexture::TT_2D);
	m_pTexAllState = pTexAllState;
}
bool VS2DTexSampler::GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	VSString Temp;
	if(m_pOutput[0]->GetValueType() == VSPutNode::VT_1)
	{
		OutString +=VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("float ");*/
		Temp = VSRenderer::ms_pRenderer->FloatConst(_T("0"));
	}
	else if(m_pOutput[0]->GetValueType() == VSPutNode::VT_2)
	{
		OutString +=VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
		Temp = VSRenderer::ms_pRenderer->Float2Const(_T("0"),_T("0"));/*_T("float2(0,0)");*/
	}
	else if(m_pOutput[0]->GetValueType() == VSPutNode::VT_3)
	{
		OutString +=VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
		Temp = VSRenderer::ms_pRenderer->Float3Const(_T("0"),_T("0"),_T("0"));/*_T("float3(0,0,0)");*/
	}
	else if(m_pOutput[0]->GetValueType() == VSPutNode::VT_4)
	{
		OutString +=VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
		Temp = VSRenderer::ms_pRenderer->Float4Const(_T("0"),_T("0"),_T("0"),_T("1"));/*_T("float4(0,0,0,1)");*/
	}
	else
		VSMAC_ASSERT(0);

	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(" = ") + Temp + _T(";\n");

	return 1;
}
