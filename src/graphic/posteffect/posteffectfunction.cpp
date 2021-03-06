#include "graphic/posteffect/posteffectfunction.h"
#include "graphic/posteffect/posteffectset.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/stream/stream.h"
using namespace zq;
IMPLEMENT_RTTI_NoCreateFun(VSPostEffectFunction,VSObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(VSPostEffectFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(VSPostEffectFunction,VSObject)
REGISTER_PROPERTY(m_ShowName,ShowName,VSProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bEnable,Enable,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOwner,Owner,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pInput,Input,VSProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOutput,Output,VSProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
VSPostEffectFunction::VSPostEffectFunction()
{
	m_pInput.Clear();
	m_pOutput.Clear();
	m_bIsVisited = 1;
	m_pOwner = NULL;
	m_bEnable = true;
}
VSPostEffectFunction::VSPostEffectFunction(const VSUsedName & ShowName,VSPostEffectSet * pPostEffectSet)
{
	m_ShowName = ShowName;
	m_pInput.Clear();
	m_pOutput.Clear();
	VSMAC_ASSERT(pPostEffectSet);
	m_pOwner = pPostEffectSet;
	m_pOwner->AddPostEffectFunction(this);
	m_bEnable = true;
}
VSPostEffectFunction::~VSPostEffectFunction()
{
	for(uint32 i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		SAFE_DELETE(m_pInput[i]);
	}
	for(uint32 i = 0 ; i < m_pOutput.GetNum() ; i++)
	{
		SAFE_DELETE(m_pOutput[i]);
	}
	m_pOwner->DeletePostEffectFunction(this);
	m_pPostEffectRender = NULL;
}

VSInputNode * VSPostEffectFunction::GetInputNode(uint32 uiNodeID)const
{
	if(uiNodeID >=m_pInput.GetNum())
		return NULL;
	return m_pInput[uiNodeID];
}
VSInputNode * VSPostEffectFunction::GetInputNode(const VSString & NodeName)const
{
	for (uint32 i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pInput[i]->GetNodeName() == NodeName)
			return m_pInput[i];
	}
	return NULL;
}

VSOutputNode * VSPostEffectFunction::GetOutputNode(uint32 uiNodeID)const
{
	VSMAC_ASSERT(uiNodeID < m_pOutput.GetNum());
	return m_pOutput[uiNodeID];
}
VSOutputNode * VSPostEffectFunction::GetOutputNode(const VSString & NodeName)const
{
	for (uint32 i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pOutput[i]->GetNodeName() == NodeName)
			return m_pOutput[i];
	}
	return NULL;
}
bool VSPostEffectFunction::ClearFlag()
{
	if(m_bIsVisited == 0)
		return 1;
	else
	{
		CreateSceneRender();
		m_bIsVisited = 0;
		for(uint32 i = 0 ; i < m_pInput.GetNum(); i++)
		{
			if(m_pInput[i]->GetOutputLink() == NULL)
				continue;
			else
			{
				((VSPostEffectFunction *)m_pInput[i]->GetOutputLink()->GetOwner())->ClearFlag();

			}

		}
		return 1;
	}
}