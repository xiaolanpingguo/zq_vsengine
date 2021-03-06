#pragma once


#include "core/system.h"
#include "graphic/core/main.h"
#include "math/maths.h"
#include "core/memorymanager.h"
#include "core/command.h"
#include "graphic/render/renderer.h"
#include "graphic/core/graphicinclude.h"


namespace zq{


#define DLCARE_APPLICATION(classname)\
	public:\
		static bool RegisterMainFactory();\
	private:\
		static bool InitialApplcation();\
		static bool TerminalApplcation();\
		static bool ms_bRegisterMainFactory;

#define IMPLEMENT_APPLICATION(classname)\
	static bool gs_bStreamRegistered_classname= classname::RegisterMainFactory ();\
 	bool classname::ms_bRegisterMainFactory = false;\
	bool classname::RegisterMainFactory()\
	{\
		if (!ms_bRegisterMainFactory)\
		{\
			VSMain::AddInitialFunction(classname::InitialApplcation);\
			VSMain::AddTerminalFunction(classname::TerminalApplcation);\
			ms_bRegisterMainFactory = true;\
		}\
		return ms_bRegisterMainFactory;\
	}\
	bool classname::InitialApplcation()\
	{\
		classname::ms_pApplication = MEM_NEW classname();\
		if(!classname::ms_pApplication)\
			return false;\
		return true;\
	}\
	bool classname::TerminalApplcation()\
	{\
		if (classname::ms_pApplication)\
		{\
			MEM_DELETE(classname::ms_pApplication);\
		}\
		return true;\
	}

	
class VSApplication : public MemObject
{
public:
	VSApplication();
	virtual ~VSApplication();
	static VSApplication * ms_pApplication;

protected:

	VSCommand * m_pCommand;
	uint32 m_uiRenderAPIType;
	uint32 m_uiScreenWidth;
	uint32 m_uiScreenHeight;	
	uint32 m_uiAnisotropy;
	uint32 m_uiMultisample;
	uint32 m_iUpdateThreadNum;
		
	virtual bool CreateAppWindow();

	virtual bool CreateRenderer();
	virtual bool ReleaseRenderer();

	virtual bool CreateInput();
	virtual bool ReleaseInput();

	virtual bool CreateTimer();
	virtual bool ReleaseTimer();

	virtual bool CreateSceneManager();
	virtual bool ReleaseSceneManager();

	virtual bool CreateWorld();
	virtual bool ReleaseWorld();
		
	virtual bool CreateMonitor();
	virtual bool ReleaseMonitor();

	virtual bool CreateASYNLoader();
	virtual bool ReleaseASYNLoader();

	virtual bool CreateRenderThread();
	virtual bool ReleaseRenderThread();

	virtual bool CreateUpdateThread();
	virtual bool ReleaseUpdateThread();
		
	virtual bool CreateStreamingManager();
	virtual bool ReleaseStreamingManager();

	virtual bool Run();
		
	virtual  bool CreateEngine();
	virtual  bool ReleaseEngine();
public:
	virtual bool Main(HINSTANCE hInst,LPSTR lpCmdLine, int32 nCmdShow);
	virtual bool Main(int32 argc, char* argv[]);
	virtual bool PreInitial();
	virtual bool OnInitial();	
	virtual bool OnTerminal();
	virtual bool PostUpdate();
	virtual bool PreUpdate();
	virtual bool OnDraw();
	virtual void OnMove(int32 xPos,int32 yPos);
	virtual void OnReSize(int32 iWidth,int32 iHeight);
	virtual void OnKeyDown(uint32 uiKey);
	virtual void OnKeyUp(uint32 uiKey);
	virtual void OnLButtonDown(int32 xPos,int32 yPos);
	virtual void OnLButtonUp(int32 xPos,int32 yPos);
	virtual void OnRButtonDown(int32 xPos,int32 yPos);
	virtual void OnRButtonUp(int32 xPos,int32 yPos);
	virtual void OnMButtonDown(int32 xPos,int32 yPos);
	virtual void OnMButtonUp(int32 xPos,int32 yPos);
	virtual void OnMouseMove(int32 xPos,int32 yPos);
	virtual void OnMouseWheel(int32 xPos,int32 yPos,int32 zDet);

	virtual void ChangeScreenSize(uint32 uiWidth,uint32 uiHeight,bool bWindow,bool IsMaxScreen = false);
	static void InputMsgProc(uint32 uiInputType, uint32 uiEvent, uint32 uiKey, int32 x, int32 y, int32 z);
	bool m_bIsActive;
	bool m_bIsRunning;
};


#ifdef PLATFORM_WIN
class VSWindowApplication : public VSApplication
{
public:
	VSWindowApplication();
	virtual ~VSWindowApplication();
protected:
	virtual bool CreateRenderer();
	virtual bool CreateInput();
	virtual bool CreateMonitor();
	virtual bool ReleaseMonitor();
	virtual bool CreateDx11();
	virtual bool CreateAppWindow();

	static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		
	VSString m_Tile;
	VSString m_WindowClassName;
	uint32 m_uiInputAPIType;
	static uint32 ms_WindowKeyToVS[];


	uint32 m_dwWindowedStyle;
	uint32 m_dwFullscreenStyle;
	bool		m_bIsWindowed;
	HWND	m_MainHwnd;
    VSArray<VSRenderer::ChildWindowInfo>	m_ArrayChildHwnd;
	HINSTANCE m_hInst;
public:
	virtual bool Main(HINSTANCE hInst, LPSTR lpCmdLine, int32 nCmdShow);
	virtual bool PreInitial();
	virtual void ChangeScreenSize(uint32 uiWidth, uint32 uiHeight, bool bWindow, bool IsMaxScreen = false);
	uint32 CheckVirtualKeyDown(uint32 VK);
	uint32 CheckVirtualKeyUp(uint32 VK);
};


class VSConsoleApplication : public VSApplication
{
public:
	VSConsoleApplication();
	virtual ~VSConsoleApplication();
protected:
	virtual bool CreateRenderer();
	virtual bool CreateInput();

	virtual bool CreateAppWindow();
	virtual bool CreateDx11();
public:
	virtual bool Main(int32 argc, char* argv[]);
	virtual bool PreInitial();
	virtual void ChangeScreenSize(uint32 uiWidth, uint32 uiHeight, bool bWindow, bool IsMaxScreen = false);

};
#endif
}