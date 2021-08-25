#include "graphic/core/main.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/core/resourcemanager.h"


namespace zq{


VSArray<VSMain::Element> *VSMain::ms_pInitialArray = NULL;
VSArray<VSMain::Element> *VSMain::ms_pTerminalArray = NULL;
VSArray<FunctionProperty> *VSMain::ms_pInitialPropertyArray = NULL;
VSArray<Function> *VSMain::ms_pTerminalPropertyArray = NULL;
uint32 VSMain::ms_uiInitialObject = 0;
uint32 VSMain::ms_uiTerminalObject = 0;

void VSMain::AddInitialPropertyFunction(FunctionProperty Func)
{
	if(!Func)
		return;
	if(!ms_pInitialPropertyArray)
	{
		ms_pInitialPropertyArray = MEM_NEW VSArray<FunctionProperty>;
	}
	ms_pInitialPropertyArray->AddElement(Func);
}

void VSMain::AddTerminalPropertyFunction(Function Func)
{
	if(!Func)
		return;
	if(!ms_pTerminalPropertyArray)
	{
		ms_pTerminalPropertyArray = MEM_NEW VSArray<Function>;
	}
	ms_pTerminalPropertyArray->AddElement(Func);
}

void VSMain::AddInitialFunction(Function Func)
{
	AddInitialFunction(Func,NULL);
}

void VSMain::AddInitialFunction(Function Func,VSPriority *pPriority)
{
	if(!Func)
		return;

	if(!ms_pInitialArray)
	{
		ms_pInitialArray = MEM_NEW VSArray<Element>;
	}

	Element e;
	e.Func = Func;
	e.pPriority = pPriority;
	ms_pInitialArray->AddElement(e);
}

void VSMain::AddTerminalFunction(Function Func)
{
	AddTerminalFunction(Func,NULL);
}

void VSMain::AddTerminalFunction(Function Func,VSPriority *pPriority)
{
	if(!Func)
		return;
	if(!ms_pTerminalArray)
	{
		ms_pTerminalArray = MEM_NEW VSArray<Element>;
	}
	Element e;
	e.Func = Func;
	e.pPriority = pPriority;
	ms_pTerminalArray->AddElement(e);
}

bool VSMain::Initialize()
{
	for(uint32 i = 0 ; i < ms_pInitialPropertyArray->GetNum(); i++)
	{
		/*(*( (*ms_pInitialArray)[i].Func ))();*/
		if( !(*( (*ms_pInitialPropertyArray)[i] ))(NULL) )
		{
			VSMAC_ASSERT(0);
			return 0;
		}
	}
	ms_pInitialArray->Sort(0,ms_pInitialArray->GetNum() - 1,PriorityCompare());
	for(uint32 i = 0 ; i < ms_pInitialArray->GetNum(); i++)
	{
		/*(*( (*ms_pInitialArray)[i].Func ))();*/
		if( !(*( (*ms_pInitialArray)[i].Func ))() )
		{
			VSMAC_ASSERT(0);
			return 0;
		}
	}

	ms_pInitialArray->Clear();
	SAFE_DELETE(ms_pInitialArray);
	ms_uiInitialObject = VSObject::GetObjectManager().GetObjectNum();
	ms_pInitialPropertyArray->Clear();
	SAFE_DELETE(ms_pInitialPropertyArray);
	return 1;
}

bool VSMain::Terminate()
{
	ms_pTerminalArray->Sort(0,ms_pTerminalArray->GetNum(),PriorityCompare());
	ms_uiTerminalObject = VSObject::GetObjectManager().GetObjectNum();

	for (int32 i = ms_pTerminalArray->GetNum() - 1; i >= 0; i--)
	{
		/*Function fun = NULL;
		fun = (*ms_pTerminalArray)[i].Func;
		(*fun)();
		//(*( (*ms_pTerminalArray)[i].Func ))();*/
		if( !(*( (*ms_pTerminalArray)[i].Func ))() )
		{
			VSMAC_ASSERT(0);
			return 0;
		}

	}
	ms_pTerminalArray->Clear();
	SAFE_DELETE(ms_pTerminalArray);

	for(uint32 i = 0 ; i < ms_pTerminalPropertyArray->GetNum(); i++)
	{

		if( !(*( (*ms_pTerminalPropertyArray)[i]))() )
		{
			VSMAC_ASSERT(0);
			return 0;
		}
	}
	ms_pTerminalPropertyArray->Clear();
	SAFE_DELETE(ms_pTerminalPropertyArray);
	VSResourceManager::GCObject();
	VSResourceManager::RunAllGCTask();
	VSMAC_ASSERT(VSResourceManager::IsReleaseAll());
	VSFastObjectManager& Temp = VSObject::GetObjectManager();
	ms_uiTerminalObject = VSObject::GetObjectManager().GetObjectNum();
	VSMAC_ASSERT(ms_uiTerminalObject == 0);
	return 1;
}

}