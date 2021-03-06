#include "core/application.h"
#include "graphic/dx11/engineinput_dx.h"
#include "graphic/world/actor/cameraactor.h"
#include "graphic/world/world.h"
#include "graphic/controller/cameracontroller1st.h"
#include "graphic/world/actor/staticactor.h"
#include "graphic/world/actor/lightactor.h"
#include "graphic/node/nodecomponent/light/skylight.h"
#include "graphic/render/scenemanager/viewfamily.h"
#include "core/timer.h"
#include "graphic/render/scenemanager/scenemanager.h"
#include "graphic/world/actor/skeletonactor.h"

#include "VSMonsterActor2.h"
using namespace zq;
class VSDemoWindowsApplication : public VSWindowApplication
{

	DLCARE_APPLICATION(VSDemoWindowsApplication);
public:
	VSDemoWindowsApplication();
	~VSDemoWindowsApplication();
	virtual bool PreInitial();
	virtual bool OnInitial();
	virtual bool OnTerminal();
	virtual bool PostUpdate();
	virtual bool PreUpdate();
	virtual bool OnDraw();
	virtual void OnMove(int xPos, int yPos);
	virtual void OnReSize(int iWidth, int iHeight);
	virtual void OnKeyDown(unsigned int uiKey);
	virtual void OnKeyUp(unsigned int uiKey);
	virtual void OnLButtonDown(int xPos, int yPos);
	virtual void OnLButtonUp(int xPos, int yPos);
	virtual void OnRButtonDown(int xPos, int yPos);
	virtual void OnRButtonUp(int xPos, int yPos);
	virtual void OnMButtonDown(int xPos, int yPos);
	virtual void OnMButtonUp(int xPos, int yPos);
	virtual void OnMouseMove(int xPos, int yPos);
	virtual void OnMouseWheel(int xPos, int yPos, int zDet);
protected:
	VSCameraActor* m_pCameraActor;
	VS1stCameraController* m_p1stCameraController;
	VSSceneMap * m_pTestMap;
	
	bool	m_bWireSwith;
	VSRasterizerStatePtr  m_pWireFrameStateEnable;


};
IMPLEMENT_APPLICATION(VSDemoWindowsApplication);
VSDemoWindowsApplication::VSDemoWindowsApplication()
{
	m_bWireSwith = false;

}
VSDemoWindowsApplication::~VSDemoWindowsApplication()
{

}
bool VSDemoWindowsApplication::PreInitial()
{
	VSWindowApplication::PreInitial();
	m_uiInputAPIType = VSEngineInput::IAT_WINDOWS;
	//m_uiInputAPIType = VSEngineInput::IAT_DX;
	VSResourceManager::ms_bUpdateThread = false;
	VSResourceManager::ms_bRenderThread = false;
	
	m_pWireFrameStateEnable = NULL;
	

	return true;
}
bool VSDemoWindowsApplication::OnInitial()
{
	VSWindowApplication::OnInitial();



	m_pTestMap = VSWorld::ms_pWorld->CreateScene(_T("Test"));

	VSMonsterActor2 * pMonsterActor2 = (VSMonsterActor2 *)VSWorld::ms_pWorld->CreateActor<VSMonsterActor2>(Vector3(0, 0, 0), Matrix3X3::ms_Identity, Vector3::ms_One, m_pTestMap);

	VSWorld::ms_pWorld->CreateActor(_T("NewOceanPlane.STMODEL"), Vector3(0, 0, 0), Matrix3X3::ms_Identity, Vector3(100.0f, 100.f, 100.0f), m_pTestMap);
	VSSkyLightActor * pSkyLightActor = (VSSkyLightActor *)VSWorld::ms_pWorld->CreateActor<VSSkyLightActor>(Vector3::ms_Zero, Matrix3X3::ms_Identity, Vector3::ms_One, m_pTestMap);
	pSkyLightActor->GetTypeNode()->m_DownColor = VSColorRGBA(0.5f, 0.5f, 0.5f, 1.0f);
	pSkyLightActor->GetTypeNode()->m_UpColor = VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);

	m_pTestMap->GetScene()->Build();

	VSArray<VSString> SceneMap;
	SceneMap.AddElement(_T("Main"));
	SceneMap.AddElement(_T("Test"));
	pMonsterActor2->AddCanSeeSence(SceneMap);


	VSRasterizerDesc WireLessDesc;
	WireLessDesc.m_bWireEnable = true;
	m_pWireFrameStateEnable = VSResourceManager::CreateRasterizerState(WireLessDesc);

	return true;
}
bool VSDemoWindowsApplication::OnTerminal()
{
	VSWindowApplication::OnTerminal();
	m_pWireFrameStateEnable = NULL;
	return true;
}
bool VSDemoWindowsApplication::PostUpdate()
{
	VSWindowApplication::PostUpdate();


	return true;
}
bool VSDemoWindowsApplication::PreUpdate()
{
	VSWindowApplication::PreUpdate();
	return true;
}
bool VSDemoWindowsApplication::OnDraw()
{
	VSWindowApplication::OnDraw();
	VSRenderer::ms_pRenderer->DrawText(300, 640, ColorABGR(255, 255, 255, 255), _T("Use Key 'Left' And 'Right' or Put Down Left Mouse Button to Change Direction"));
	VSRenderer::ms_pRenderer->DrawText(300, 660, ColorABGR(255, 255, 255, 255), _T("Use Key 'Up' to Move"));
	VSRenderer::ms_pRenderer->DrawText(300, 680, ColorABGR(255, 255, 255, 255), _T("Use Key 'A' to Attack"));
	return true;
}
void VSDemoWindowsApplication::OnMove(int xPos, int yPos)
{

}
void VSDemoWindowsApplication::OnReSize(int iWidth, int iHeight)
{

}
void VSDemoWindowsApplication::OnKeyDown(unsigned int uiKey)
{
	VSApplication::OnKeyDown(uiKey);
	if (uiKey == VSEngineInput::BK_W)
	{
		m_bWireSwith = !m_bWireSwith;

		VSViewFamily * pViewFamily = VSSceneManager::ms_pSceneManager->GetViewFamily(_T("WindowUse"));
		VSSceneRenderMethod * pRM = pViewFamily->m_pSceneRenderMethod;
		if (m_bWireSwith)
		{
			VSRenderState RenderState;
			RenderState.SetRasterizerState(m_pWireFrameStateEnable);

			pRM->SetUseState(RenderState, VSRenderState::IF_WIRE_ENABLE);
		}
		else
		{
			pRM->ClearUseState();
		}


	}
	else if (uiKey == VSEngineInput::BK_F1)
	{
		ChangeScreenSize(640, 480, true, false);
	}
	else if (uiKey == VSEngineInput::BK_F2)
	{
		ChangeScreenSize(640, 480, true, true);
	}
	else if (uiKey == VSEngineInput::BK_F3)
	{
		ChangeScreenSize(1024, 768, false);
	}

}
void VSDemoWindowsApplication::OnKeyUp(unsigned int uiKey)
{
	VSApplication::OnKeyUp(uiKey);

}
void VSDemoWindowsApplication::OnLButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnLButtonDown(xPos, yPos);
	
}
void VSDemoWindowsApplication::OnLButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnLButtonUp(xPos, yPos);
	
}
void VSDemoWindowsApplication::OnRButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnRButtonDown(xPos, yPos);
}
void VSDemoWindowsApplication::OnRButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnRButtonUp(xPos, yPos);
}
void VSDemoWindowsApplication::OnMButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnMButtonDown(xPos, yPos);
}
void VSDemoWindowsApplication::OnMButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnMButtonUp(xPos, yPos);
}
void VSDemoWindowsApplication::OnMouseMove(int xPos, int yPos)
{
	VSWindowApplication::OnMouseMove(xPos, yPos);
	

}
void VSDemoWindowsApplication::OnMouseWheel(int xPos, int yPos, int zDet)
{
	VSWindowApplication::OnMouseWheel(xPos, yPos, zDet);
}
