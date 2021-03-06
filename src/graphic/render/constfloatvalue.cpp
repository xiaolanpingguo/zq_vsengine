#include "graphic/render/constfloatvalue.h"
#include "graphic/material/shaderstringfactory.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI(VSConstFloatValue,VSConstValue)
BEGIN_ADD_PROPERTY(VSConstFloatValue,VSConstValue)
REGISTER_PROPERTY(m_Value,Value,VSProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VSConstFloatValue)
IMPLEMENT_INITIAL_END


VSConstFloatValue::VSConstFloatValue()
{

}
VSConstFloatValue::VSConstFloatValue(const VSUsedName & ShowName,VSMaterial * pMaterial,uint32 uiValueNum,bool bIsCustom)
:VSConstValue(ShowName,pMaterial,uiValueNum,bIsCustom)
{
	VSMAC_ASSERT(uiValueNum >0 && uiValueNum < 5);

	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("ConstFloatValue") + OutputID;
	VSOutputNode * pOutputNode = NULL;
	pOutputNode = MEM_NEW VSOutputNode(uiValueNum - 1,OutputName,this);
	VSMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);
	VSShaderStringFactory::ms_ShaderValueIndex++;
	if(uiValueNum >=1)
	{
		VSString OutputNameR = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE),VSRenderer::VE_R);/*OutputName + _T(".x") ;*/
		VSOutputNode * pOutputNode = NULL;
		pOutputNode = MEM_NEW VSOutputNode(VSPutNode::VT_1,OutputNameR,this);
		VSMAC_ASSERT(pOutputNode);
		m_pOutput.AddElement(pOutputNode);
	}

	if(uiValueNum >=2)
	{
		VSString OutputNameG = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE),VSRenderer::VE_G);/*OutputName + _T(".y");*/
		VSOutputNode * pOutputNode = NULL;
		pOutputNode = MEM_NEW VSOutputNode(VSPutNode::VT_1,OutputNameG,this);
		VSMAC_ASSERT(pOutputNode);
		m_pOutput.AddElement(pOutputNode);
	}

	if(uiValueNum >=3)
	{
		VSString OutputNameB = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE),VSRenderer::VE_B);/*OutputName + _T(".z");*/
		VSOutputNode * pOutputNode = NULL;
		pOutputNode = MEM_NEW VSOutputNode(VSPutNode::VT_1,OutputNameB,this);
		VSMAC_ASSERT(pOutputNode);
		m_pOutput.AddElement(pOutputNode);
	}

	if(uiValueNum >=4)
	{

		VSString OutputNameA = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE),VSRenderer::VE_A);/*OutputName + _T(".w") ;*/
		VSOutputNode * pOutputNode = NULL;
		pOutputNode = MEM_NEW VSOutputNode(VSPutNode::VT_1,OutputNameA,this);
		VSMAC_ASSERT(pOutputNode);
		m_pOutput.AddElement(pOutputNode);

	}
	m_Value.Clear();
	m_Value.SetBufferNum(uiValueNum);
	for (uint32 i = 0 ; i < uiValueNum ; i++)
	{
		m_Value[i] = 0;
	}
	m_bIsCustom = bIsCustom;
}
VSConstFloatValue::~VSConstFloatValue()
{
	m_Value.Clear();
}
void VSConstFloatValue::ResetInShaderName(MaterialShaderPara &MSPara)
{


	VSString OutputID = IntToString(VSShaderStringFactory::ms_ShaderValueIndex);
	VSString OutputName = _T("ConstFloatValue") + OutputID;
	m_pOutput[0]->SetNodeName(OutputName);
	VSShaderStringFactory::ms_ShaderValueIndex++;
	if (m_Value.GetNum() >= 1)
	{
		VSString OutputNameR = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE),VSRenderer::VE_R);
		m_pOutput[1]->SetNodeName(OutputNameR);
	}

	if (m_Value.GetNum() >= 2)
	{
		VSString OutputNameG = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE),VSRenderer::VE_G);
		m_pOutput[2]->SetNodeName(OutputNameG);
	}

	if (m_Value.GetNum() >= 3)
	{
		VSString OutputNameB = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE),VSRenderer::VE_B);
		m_pOutput[3]->SetNodeName(OutputNameB);
	}

	if (m_Value.GetNum() >= 4)
	{
		VSString OutputNameA = VSRenderer::GetValueElement(GetOutputNode(VSOutputNode::ONI_VALUE),VSRenderer::VE_A);
		m_pOutput[4]->SetNodeName(OutputNameA);
	}

}
bool VSConstFloatValue::GetDeclareString(VSString &OutString, uint32 uiRegisterID, MaterialShaderPara &MSPara)const
{
	if(!m_bIsCustom)
		return 1;

	if (MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
	{
		return 1;
	}
	OutString += VSRenderer::ms_pRenderer->GetDeclareValue(m_pOutput[0]->GetNodeName().GetString(), m_pOutput[0]->GetValueType(), uiRegisterID);


	return 1;
}
bool VSConstFloatValue::GetOutputValueString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	
	if (m_bIsCustom && !MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
		return 1;

	if(m_pOutput[0]->GetValueType() == VSPutNode::VT_1)
	{
		OutString +=VSRenderer::ms_pRenderer->Float() + _T(" ");/*_T("float ");*/
	}
	else if(m_pOutput[0]->GetValueType() == VSPutNode::VT_2)
	{
		OutString +=VSRenderer::ms_pRenderer->Float2() + _T(" ");/*_T("float2 ")*/;
	}
	else if(m_pOutput[0]->GetValueType() == VSPutNode::VT_3)
	{
		OutString +=VSRenderer::ms_pRenderer->Float3() + _T(" ");/*_T("float3 ");*/
	}
	else if(m_pOutput[0]->GetValueType() == VSPutNode::VT_4)
	{
		OutString +=VSRenderer::ms_pRenderer->Float4() + _T(" ");/*_T("float4 ");*/
	}
	else
		return 0;

	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(";\n");

	return 1;
}
bool VSConstFloatValue::GetFunctionString(VSString &OutString, MaterialShaderPara &MSPara)const
{
	if (m_bIsCustom && !MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type))
		return 1;
	
	OutString += m_pOutput[0]->GetNodeName().GetString() + _T(" = ");

	if (m_bIsCustom && MSPara.pGeometry->IsSameType(VSInstanceGeometry::ms_Type) && VSRenderer::ms_pRenderer->IsSupportFeature(VSRenderer::SF_MaterialValueInstance))
	{
		int32 Index = MSPara.pMaterialInstance->GetMaterial()->GetInstanceIndex(this);
		OutString += VSShaderStringFactory::ms_InputMaterialInstance[Index];
	}
	else
	{
		VSString Value[4];
		for (uint32 i = 0; i < m_Value.GetNum(); i++)
		{
			Value[i] = RealToString(m_Value[i]);
		}
		if (m_pOutput[0]->GetValueType() == VSPutNode::VT_1)
		{
			OutString += VSRenderer::ms_pRenderer->FloatConst(Value[0]);
		}
		else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_2)
		{
			OutString += VSRenderer::ms_pRenderer->Float2Const(Value[0], Value[1]);
		}
		else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_3)
		{
			OutString += VSRenderer::ms_pRenderer->Float3Const(Value[0], Value[1], Value[2]);
		}
		else if (m_pOutput[0]->GetValueType() == VSPutNode::VT_4)
		{
			OutString += VSRenderer::ms_pRenderer->Float4Const(Value[0], Value[1], Value[2], Value[3]);
		}
	}
	

	OutString += _T(";\n");

	return 1;
}
void VSConstFloatValue::SetValue(uint32 uiIndex,float Value)
{
	VSMAC_ASSERT(uiIndex < m_Value.GetNum());
	m_Value[uiIndex] = Value;
}