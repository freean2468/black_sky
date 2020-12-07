/////////////////////////////////////////////////////////
///
/// Main.cpp : ������ ���� �ڵ�
///
/////////////////////////////////////////////////////////

/*///////////////////////////////////////////////////////
�����̸� : Black Sky
�帣 : Star Creating Simulation
������ : ������
���۱Ⱓ : 2009/07/24 ~ 2009/9/2 - ��������� �Ϸ�
������Ʈ ��¥ : ~ 2009/9/15 - ���� ���� ���� ���� �Ϸ�
�̰����� ���� ū ��������
- ������
- ȸ������ ���ؼ� �浹üũ�� ��Ȯ�� �̷�� ���� �ʴ´�.
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
	MyRegisterClass(hInstance); //������ Ŭ���� �ʱ�ȭ �� ���

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))  //������ ����
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
			switch(SystemState){					// �ý��� ���¿� ���� ���� ���� ������ �޸��Ѵ�.
			case MAIN_MENU: MenuLoop(); break;
			case GAME_START: 
				switch(GameState){					// ���� ���¿� ���� ���� ���� ������ �޸��Ѵ�.
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

//������ �ʱ�ȭ & ���
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

//������ ����
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
	//Ǯ��ũ�� ��� 
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