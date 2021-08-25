#include "graphic/core/profiler.h"
#include "core/timer.h"
#include "graphic/render/renderer.h"
#include "graphic/core/graphicinclude.h"
#include "graphic/asyn/renderthread.h"
using namespace zq;
VSProfilerNode::VSProfilerNode(const char *Name, const char * ParentName)
{
	m_Name = Name;
	if (ParentName)
	{
		m_ParentName = ParentName;
	}
	
	VSProfiler::AddProfileNode(this);
	m_pParentNode = NULL;
}
void VSProfilerNode::AddChild(VSProfilerNode * pChild)
{
	VSMAC_ASSERT(pChild && !pChild->m_pParentNode);
	pChild->m_pParentNode = this;
	m_pChildNode.AddElement(pChild);
}
void VSProfilerNode::Draw(uint32 uiLayer)
{
	OnDraw(uiLayer);
	VSProfiler::ms_uiCurCount++;
	for (uint32 i = 0; i < m_pChildNode.GetNum();i++)
	{
		m_pChildNode[i]->Draw(uiLayer + 1);
	}
}
IMPLEMENT_INITIAL_ONLY_BEGIN(VSProfiler);
ADD_INITIAL_FUNCTION(InitialDefaultState)
IMPLEMENT_INITIAL_ONLY_END
uint32 VSProfiler::ms_uiCurCount = 0;
VSArray<VSProfilerNode *> VSProfiler::m_pRoot;
char *  VSProfiler::ms_LayerFlag[] = { _T(""),
										_T("  "),
										_T("    "),
										_T("      "),
										_T("        "),
										_T("          "),
										_T("            "),
										_T("              "),
										_T("                "),
										_T("                  ") };
bool VSProfiler::InitialDefaultState()
{
	for (uint32 i = 0; i < GetProfilerNode().GetNum(); i++)
	{
		if (GetProfilerNode()[i]->m_ParentName == "")
		{
			m_pRoot.AddElement(GetProfilerNode()[i]);
		}
		else
		{
			bool bFound = false;
			for (uint32 j = 0; j < GetProfilerNode().GetNum(); j++)
			{
				if (GetProfilerNode()[i]->m_ParentName == GetProfilerNode()[j]->m_Name)
				{
					GetProfilerNode()[j]->AddChild(GetProfilerNode()[i]);
					bFound = true;
					break;
				}
			}
			if (bFound == false)
			{
				m_pRoot.AddElement(GetProfilerNode()[i]);
			}
			
		}
	}
	return 1;
}
void VSProfiler::Draw()
{
#ifdef PROFILER
	for (uint32 i = 0; i < m_pRoot.GetNum(); i++)
	{
		m_pRoot[i]->Draw( 0);
	}
	ClearAll();
#endif	
}
void VSProfiler::ClearAll()
{
	for (uint32 i = 0; i < GetProfilerNode().GetNum(); i++)
	{
		GetProfilerNode()[i]->Clear();
	}
	ms_uiCurCount = 0;
}

void VSProfiler::AddProfileNode(VSProfilerNode * pProfilerNode)
{
	GetProfilerNode().AddElement(pProfilerNode);
}
void VSProfiler::GetProfilerNameString(const VSString & Name, uint32 uiLayer, VSString & OutString)
{
	OutString = VSProfiler::ms_LayerFlag[uiLayer] + Name;
}
VSTimeProfilerNode::VSProfilerNodeTImer::VSProfilerNodeTImer(VSTimeProfilerNode * pProfilerNode)
{
	m_fBeginTime = (Timer::ms_pTimer != NULL ? (float)Timer::ms_pTimer->GetGamePlayTime() : 0.0f);
	m_pOwner = pProfilerNode;
}
VSTimeProfilerNode::VSProfilerNodeTImer::~VSProfilerNodeTImer()
{
	m_pOwner->m_fProfilerTime += (Timer::ms_pTimer != NULL ? (float)Timer::ms_pTimer->GetGamePlayTime() : 0.0f) - m_fBeginTime;
}
VSTimeProfilerNode::VSTimeProfilerNode(const char *Name, const char * ParentName)
:VSProfilerNode(Name, ParentName)
{
	m_fProfilerTime = 0.0f;
}
void VSTimeProfilerNode::OnDraw(uint32 uiLayer)
{
	VSString Out;
	VSProfiler::GetProfilerNameString(m_Name, uiLayer, Out);
	Out += _T(":%fms");
	VSRenderer::ms_pRenderer->DrawText(0, VSProfiler::ms_uiCurCount * 20, ColorABGR(255, 0, 255, 0), Out.GetBuffer(), m_fProfilerTime);
}


VSCountProfilerNode::VSProfilerNodeCounter::VSProfilerNodeCounter(VSCountProfilerNode * pProfilerNode, int32 uiCount)
{
	pProfilerNode->m_uiCounter += uiCount;
}

VSCountProfilerNode::VSCountProfilerNode(const char *Name, const char * ParentName)
:VSProfilerNode(Name, ParentName)
{
	m_uiCounter = 0;
}
void VSCountProfilerNode::OnDraw(uint32 uiLayer)
{
	VSString Out;
	VSProfiler::GetProfilerNameString(m_Name, uiLayer, Out);
	Out += _T(":%d");
	VSRenderer::ms_pRenderer->DrawText(0, VSProfiler::ms_uiCurCount * 20, ColorABGR(255, 255, 255, 255), Out.GetBuffer(), m_uiCounter);
}
VSNoClearCountProfilerNode::VSNoClearCountProfilerNode(const char *Name, const char * ParentName)
:VSCountProfilerNode(Name, ParentName)
{
	m_uiCounter = 0;
}
VSOnlyTimeProfilerNode::VSProfilerNodeOnlyTImer::VSProfilerNodeOnlyTImer(VSOnlyTimeProfilerNode * pProfilerNode, float fProfilerTime)
{
	pProfilerNode->m_fProfilerTime = fProfilerTime;
}

VSOnlyTimeProfilerNode::VSOnlyTimeProfilerNode(const char *Name, const char * ParentName)
:VSProfilerNode(Name, ParentName)
{
	m_fProfilerTime = 0.0f;
}
void VSOnlyTimeProfilerNode::OnDraw(uint32 uiLayer)
{
	VSString Out;
	VSProfiler::GetProfilerNameString(m_Name, uiLayer, Out);
	Out += _T(":%f");
	VSRenderer::ms_pRenderer->DrawText(0, VSProfiler::ms_uiCurCount * 20, ColorABGR(255, 255, 255, 0), Out.GetBuffer(), m_fProfilerTime);
}