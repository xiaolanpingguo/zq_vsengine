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
#include "graphic/world/actor/terrainactor.h"
#include "graphic/node/terrain/clodterrainnode.h"
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
	bool m_bLMouseDowning;
	int		m_iDetMouseX;
	int		m_iDetMouseY;
	int		m_iLastMouseX;
	int		m_iLastMouseY;
	bool	m_bWireSwith;
	VSRasterizerStatePtr  m_pWireFrameStateEnable;

};
IMPLEMENT_APPLICATION(VSDemoWindowsApplication);
VSDemoWindowsApplication::VSDemoWindowsApplication()
{
	m_bWireSwith = true;
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
	m_bLMouseDowning = false;

	m_iDetMouseX = 0;
	m_iDetMouseY = 0;
	m_iLastMouseX = 0;
	m_iLastMouseY = 0;
	m_pWireFrameStateEnable = NULL;
	

	return true;
}
bool VSDemoWindowsApplication::OnInitial()
{
	VSWindowApplication::OnInitial();

	m_p1stCameraController = VSObject::GetInstance<VS1stCameraController>();
	m_pCameraActor = (VSCameraActor *)VSWorld::ms_pWorld->CreateActor<VSCameraActor>();
	m_pCameraActor->GetTypeNode()->AddController(m_p1stCameraController);
	Vector3 CameraPos(0.0f, 300.0f, 0.0f);
	Vector3 CameraDir(0.0f, 0.0f, 1.0f);
	m_pCameraActor->GetTypeNode()->CreateFromLookDir(CameraPos, CameraDir);
	m_pCameraActor->GetTypeNode()->SetPerspectiveFov(AngleToRadian(90.0f), (m_uiScreenWidth * 1.0f) / (m_uiScreenHeight), 1.0f, 80000.0f);

	m_pTestMap = VSWorld::ms_pWorld->CreateScene(_T("Test"));
	VSWorld::ms_pWorld->CreateActor(_T("NewMonsterLOD0.STMODEL"), Vector3(0, 0, 500), Matrix3X3::ms_Identity, Vector3::ms_One, m_pTestMap);
	VSCLodTerrainActor * pTerrainActor = (VSCLodTerrainActor *)VSWorld::ms_pWorld->CreateActor<VSCLodTerrainActor>(Vector3(-1300.0f, -2000.0f, -1300.0f), Matrix3X3::ms_Identity, Vector3::ms_One, m_pTestMap);
	pTerrainActor->GetTypeNode()->SetTerrainNodeType(VSCLodTerrainNode::TNT_QUAD);
	pTerrainActor->GetTypeNode()->SetCLODScale(3.0f);
	pTerrainActor->GetTypeNode()->CreateTerrainFromHeightMap(_T("heightdate.raw"),6,10.0f);
	VSSkyLightActor * pSkyLightActor = (VSSkyLightActor *)VSWorld::ms_pWorld->CreateActor<VSSkyLightActor>(Vector3::ms_Zero, Matrix3X3::ms_Identity, Vector3::ms_One, m_pTestMap);
	pSkyLightActor->GetTypeNode()->m_DownColor = VSColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	pSkyLightActor->GetTypeNode()->m_UpColor = VSColorRGBA(1.0f, 0.0f, 0.0f, 1.0f);

	m_pTestMap->GetScene()->Build();

	VSArray<VSString> SceneMap;
	SceneMap.AddElement(_T("Main"));
	SceneMap.AddElement(_T("Test"));
	VSWorld::ms_pWorld->AttachWindowViewFamilyToCamera(m_pCameraActor, VSWindowViewFamily::VT_WINDOW_NORMAL,
		_T("WindowUse"), SceneMap, VSForwardEffectSceneRenderMethod::ms_Type.GetName().GetBuffer());
	VSRasterizerDesc WireLessDesc;
	WireLessDesc.m_bWireEnable = true;
	m_pWireFrameStateEnable = VSResourceManager::CreateRasterizerState(WireLessDesc);

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
	if (m_bLMouseDowning)
	{
		m_p1stCameraController->m_RotXDelta = ((m_iDetMouseY)* 1.0f) * 0.001f/*VSTimer::ms_pTimer->GetDetTime() * 0.5f*/;
		m_p1stCameraController->m_RotYDelta = ((m_iDetMouseX)* 1.0f) * 0.001f/*VSTimer::ms_pTimer->GetDetTime() * 0.5f*/;
		m_iDetMouseX = 0;
		m_iDetMouseY = 0;
	}
	else
	{
		m_p1stCameraController->m_RotXDelta = 0.0f;
		m_p1stCameraController->m_RotYDelta = 0.0f;
	}

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
	if (uiKey == VSEngineInput::BK_UP)
	{
		m_p1stCameraController->m_MoveZDelta = 1.0f;
	}
	else if (uiKey == VSEngineInput::BK_DOWN)
	{
		m_p1stCameraController->m_MoveZDelta = -1.0f;
	}
	else if (uiKey == VSEngineInput::BK_LEFT)
	{
		m_p1stCameraController->m_MoveXDelta = -1.0f;
	}
	else if (uiKey == VSEngineInput::BK_RIGHT)
	{
		m_p1stCameraController->m_MoveXDelta = 1.0f;
	}
	else if (uiKey == VSEngineInput::BK_W)
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
	if (uiKey == VSEngineInput::BK_UP)
	{
		m_p1stCameraController->m_MoveZDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_DOWN)
	{
		m_p1stCameraController->m_MoveZDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_LEFT)
	{
		m_p1stCameraController->m_MoveXDelta = 0.0f;
	}
	else if (uiKey == VSEngineInput::BK_RIGHT)
	{
		m_p1stCameraController->m_MoveXDelta = 0.0f;
	}
}
void VSDemoWindowsApplication::OnLButtonDown(int xPos, int yPos)
{
	VSWindowApplication::OnLButtonDown(xPos, yPos);
	m_bLMouseDowning = true;
	m_iLastMouseX = xPos;
	m_iLastMouseY = yPos;
	m_iDetMouseX = 0;
	m_iDetMouseY = 0;
}
void VSDemoWindowsApplication::OnLButtonUp(int xPos, int yPos)
{
	VSWindowApplication::OnLButtonUp(xPos, yPos);
	m_bLMouseDowning = false;
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
	if (m_bLMouseDowning)
	{
		m_iDetMouseX = xPos - m_iLastMouseX;
		m_iDetMouseY = yPos - m_iLastMouseY;
		m_iLastMouseX = xPos;
		m_iLastMouseY = yPos;
	}

}
void VSDemoWindowsApplication::OnMouseWheel(int xPos, int yPos, int zDet)
{
	VSWindowApplication::OnMouseWheel(xPos, yPos, zDet);
}
