/////////////////////////////////////////////////////////
///
/// Main.cpp : 윈도우 관련 코드
///
/////////////////////////////////////////////////////////

/*///////////////////////////////////////////////////////
게임이름 : Black Sky
장르 : Star Creating Simulation
만든이 : 송훈일
제작기간 : 2009/07/24 ~ 2009/9/2 - 프로토버전 완료
업데이트 날짜 : ~ 2009/9/15 - 게임 배포 버전 개발 완료
이게임의 가장 큰 문제점들
- 느리다
- 회전으로 인해서 충돌체크가 정확히 이루어 지지 않는다.
- 
*////////////////////////////////////////////////////////

#include "stdafx.h"

HINSTANCE hInst;										// current instance
TCHAR szTitle[MAX_LOADSTRING]="Black Sky";			// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING] = "Black Sky";			// the main window class name

HWND g_hWnd;
MSG msg;

HCURSOR hNowCursor;

extern SYSTEMSTATE SystemState;
extern GAMESTATE   GameState;
extern STAGESTATE StageState;

extern bool bIsHowToPlay;
extern bool bIsActive;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
 	// TODO: Place code here.	
	MyRegisterClass(hInstance); //윈도우 클래스 초기화 및 등록

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))  //윈도우 생성
	{
		return FALSE;
	}

	// Main message loop:
	while(1){
		if(PeekMessage(&msg, NULL, 0, 0, 0)){
			if(GetMessage(&msg, NULL, 0, 0)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else{
				return (int) msg.wParam;
			}
		}
		else if(bIsActive){ 			
			switch(SystemState){					// 시스템 상태에 따라 게임 루프 진행을 달리한다.
			case MAIN_MENU: MenuLoop(); break;
			case GAME_START: 
				switch(GameState){					// 게임 상태에 따라 게임 루프 진행을 달리한다.
				case STAGE_SELECT:
					switch(StageState){
					case STAGE_PLAY:
						StageLoop();
						break;
					case STAGE_MENU:						
						StageMenu();
						break;
					}
					break;
				case GAME_PLAY:		GameLoop(); break;
				case GAME_MENU:					break;
				}
				break;
			}
		}
		else{
			WaitMessage();
		}
	}

	return (int) msg.wParam;
}

//윈도우 초기화 & 등록
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//윈도우 생성
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP,
      0, 0, SCR_WIDTH, SCR_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void FullScreen(){
	//풀스크린 모드 
	DEVMODE devmode;
	ZeroMemory(&devmode,sizeof(DEVMODE));
	devmode.dmSize = sizeof(DEVMODE);
	devmode.dmBitsPerPel =32;
	devmode.dmPelsWidth = 640;
	devmode.dmPelsHeight= 480;
	devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);

	//EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
}