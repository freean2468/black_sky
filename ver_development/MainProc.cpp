/////////////////////////////////////////////////////////
///
/// MainProc.cpp : ������ ���ν��� ���� �ڵ�
///
/////////////////////////////////////////////////////////

#include "stdafx.h"

extern HDC ScrHDC;
extern POINTS g_ptMouseAbsolute;
extern POINTS g_ptMouseRelative;

extern MENU Menu;
extern SYSTEMSTATE SystemState;
extern GAMESTATE GameState;
extern UISTATE UiState;
extern OBJECTKIND ObjectKind;
extern STARSTEP	StarStep;
extern STARINFO StarInfo[MAX_STAR];
extern STAGESTATE StageState;

extern SURFACEINFO g_sfLogo;
extern SURFACEINFO g_sfPlane;
extern SURFACEINFO g_sfStageBack;
extern SURFACEINFO g_sfBack;
extern SURFACEINFO g_sfHowToPlay[3];
extern SURFACEINFO g_sfLack;
extern SURFACEINFO g_sfStageMenu[4];

extern int FPS;
extern int nfpsCount;
extern int nStarStepNow;
extern float theta;
extern int Tipo_nx, Tipo_ny;
extern int nObject_ID;
extern int Scroll_X, Scroll_Y;

extern bool bIsHowToPlay;
extern bool Info_On;

extern TIMER TimeSpace;
extern TIMER TimeEarthquake[4];
extern TIMER TimeComplete;
extern TIMER TimeEyeCatch;

extern GOBJOBJECT_INFO* __ob_pObjInfo[MAX_STAR];

extern HINSTANCE hInst;

extern HCURSOR hNowCursor;

bool bIsActive = false;

TIMER TimeLack;
TIMER TimeBroken;

//���� ������ ���ν���
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{	
	switch(iMessage)
	{
	case WM_CREATE:
		return OnCreate(hWnd, wParam, lParam);
	case WM_KEYDOWN:
		return OnKeyDown(hWnd, wParam, lParam);
	case WM_MOUSEMOVE:
		return OnMouseMove(hWnd, wParam, lParam);
	case WM_LBUTTONDOWN:
		return OnLButtonDown(hWnd, wParam, lParam);
	case WM_SETCURSOR:
		return OnSetCursor(hWnd, wParam, lParam);
	case WM_TIMER:
		return OnTimer(hWnd, wParam, lParam);
	case WM_DESTROY:		
		return OnDestroy(hWnd, wParam, lParam);
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

//WM_CREATE�޼��� ȣ��
//������ ���� �޽��� ó��, �⺻���� �ʱ�ȭ �۾��� �����Ѵ�
int OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam){
 	FullScreen();
	SystemState = MAIN_MENU;					// �ý��� ���´� ���θ޴���

	//// �ʱ�ȭ
	ScrHDC = GetDC(hWnd);	// ��ũ�� HDC

	__aInit(ScrHDC);						// �ĸ� ǥ�� �ʱ�ȭ
	
	// ���� ���� �ʱ�ȭ
	SDL_Init();

	// ���� �޴� ���� �ʱ�ȭ 
	MenuInit();

	// �������� ���� �ʱ�ȭ
	StageInit();

	// �� ���� ���� �ʱ�ȭ
	StarInfoInit();

	//��� �ʱ�ȭ
	__goInit(MAX_OBJECT);	

	ReleaseDC(hWnd, ScrHDC);

	SetTimer(hWnd, 0, 1000, NULL);

	srand((unsigned)time(NULL));

	// ���� ����
	bIsActive = true;

	hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)); // ������ Ŀ�� �ʱ�ȭ
	
	//Timer �ʱ�ȭ
	__tmInitTimer(&TimeSpace, 1000);

	// ����� FadeInNew ȿ��
	Sound_LogoOn();
	FadeInNew(&g_sfLogo);

	Sound_StageOn();

	return 0;
}

//WM_DESTROY�޼��� ȣ��
//������ ���� �޽��� ó��, ������ ����� �޸𸮿� �����ִ� ���� ���־� �Ѵ�.
int OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	__aDestroyAll();
	__goRelease();
	SDL_Release();
	KillTimer(hWnd, 0);
	PostQuitMessage(0);

	return true;
}

int OnSetCursor(HWND hWnd, WPARAM wParam, LPARAM lParam){
	if(LOWORD(lParam) == HTCLIENT){
		SetCursor(hNowCursor);
		return true;
	}
	return false;
}

int OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam){
	static int bIsPosition = 0;

	switch(SystemState){
	case MAIN_MENU: return OnMainMenuLButtonDown(hWnd, wParam, lParam);
	case GAME_START:
		switch(GameState){
		case STAGE_SELECT: return OnStageLButtonDown(hWnd, wParam, lParam);
		case GAME_PLAY:	   return OnPlayLbuttonDown(hWnd, wParam, lParam);
		case GAME_MENU:	   return true;
		}
	case TERM:
		if(bIsHowToPlay){
			if(g_ptMouseAbsolute.x >= 50 && g_ptMouseAbsolute.y >= 420 && g_ptMouseAbsolute.x <= 115 && g_ptMouseAbsolute.y <= 440){
				PlaySoundEffect();
				bIsHowToPlay = false;
				SystemState = MAIN_MENU;
				if(StageState == STAGE_MENU)
					SystemState = GAME_START;
				bIsActive = true;
			}
			else if(g_ptMouseAbsolute.x >= 400 && g_ptMouseAbsolute.y >= 415 && g_ptMouseAbsolute.x <= 474 && g_ptMouseAbsolute.y <= 445 && bIsPosition == 0){
				PlaySoundEffect();
				bIsPosition = 1;	
				__gePutImage(g_sfBack.dcSurface,0,0,0, 0, &g_sfHowToPlay[bIsPosition]);
				RenewDC(0, 0, &g_sfBack);
			}
			else if(g_ptMouseAbsolute.x >= 400 && g_ptMouseAbsolute.y >= 415 && g_ptMouseAbsolute.x <= 474 && g_ptMouseAbsolute.y <= 445 && bIsPosition == 1){
				PlaySoundEffect();
				bIsPosition = 2;	
				__gePutImage(g_sfBack.dcSurface,0,0,0, 0, &g_sfHowToPlay[bIsPosition]);
				RenewDC(0, 0, &g_sfBack);
			}
			else if(g_ptMouseAbsolute.x >= 221 && g_ptMouseAbsolute.y >= 414 && g_ptMouseAbsolute.x <= 310 && g_ptMouseAbsolute.y <= 446 && bIsPosition == 1){
				PlaySoundEffect();
				bIsPosition = 0;		
				__gePutImage(g_sfBack.dcSurface,0,0,0, 0, &g_sfHowToPlay[bIsPosition]);
				RenewDC(0, 0, &g_sfBack);
			}
			else if(g_ptMouseAbsolute.x >= 221 && g_ptMouseAbsolute.y >= 414 && g_ptMouseAbsolute.x <= 310 && g_ptMouseAbsolute.y <= 446 && bIsPosition == 2){
				PlaySoundEffect();
				bIsPosition = 1;		
				__gePutImage(g_sfBack.dcSurface,0,0,0, 0, &g_sfHowToPlay[bIsPosition]);
				RenewDC(0, 0, &g_sfBack);
			}
		}
		break;
	}	

	return true;
}

int OnStageLButtonDown(HWND hWnd, WPARAM wParam, LPARAM){

	// ���� ������ Ŭ���Ѵٸ�
	if(g_ptMouseRelative.x >= StarInfo[0].nX && g_ptMouseRelative.y >= StarInfo[0].nY 
		&& g_ptMouseRelative.x <= StarInfo[0].nX + 35 && g_ptMouseRelative.y <= StarInfo[0].nY + 35 ||	// Ʃ�丮��
		g_ptMouseRelative.x >= StarInfo[1].nX && g_ptMouseRelative.y >= StarInfo[1].nY 
		&& g_ptMouseRelative.x <= StarInfo[1].nX + 35 && g_ptMouseRelative.y <= StarInfo[1].nY + 35 ||	// Red_Star
		g_ptMouseRelative.x >= StarInfo[2].nX && g_ptMouseRelative.y >= StarInfo[2].nY 
		&& g_ptMouseRelative.x <= StarInfo[2].nX + 35 && g_ptMouseRelative.y <= StarInfo[2].nY + 35 ||	// Yellow_Star
		g_ptMouseRelative.x >= StarInfo[3].nX && g_ptMouseRelative.y >= StarInfo[3].nY					// Blue_Star
		&& g_ptMouseRelative.x <= StarInfo[3].nX + 35 && g_ptMouseRelative.y <= StarInfo[3].nY + 35 ||
		g_ptMouseRelative.x >= StarInfo[4].nX && g_ptMouseRelative.y >= StarInfo[4].nY					// Pink_Star
		&& g_ptMouseRelative.x <= StarInfo[4].nX + 35 && g_ptMouseRelative.y <= StarInfo[4].nY + 35||
		g_ptMouseRelative.x >= StarInfo[5].nX && g_ptMouseRelative.y >= StarInfo[5].nY					// Brown_Star
		&& g_ptMouseRelative.x <= StarInfo[5].nX + 35 && g_ptMouseRelative.y <= StarInfo[5].nY + 35){	

		if(g_ptMouseRelative.x >= StarInfo[0].nX && g_ptMouseRelative.y >= StarInfo[0].nY 
		&& g_ptMouseRelative.x <= StarInfo[0].nX + 35 && g_ptMouseRelative.y <= StarInfo[0].nY + 35		// Ʃ�丮��
		&& StarInfo[0].bIsLife){
			__tmInitTimer(&TimeBroken, 3000);
		}
		else if(g_ptMouseRelative.x >= StarInfo[1].nX && g_ptMouseRelative.y >= StarInfo[1].nY 
		&& g_ptMouseRelative.x <= StarInfo[1].nX + 35 && g_ptMouseRelative.y <= StarInfo[1].nY + 35		// Red_Star
		&& StarInfo[1].bIsLife){
			__tmInitTimer(&TimeBroken, 3000);
		}
		else if(g_ptMouseRelative.x >= StarInfo[2].nX && g_ptMouseRelative.y >= StarInfo[2].nY 
		&& g_ptMouseRelative.x <= StarInfo[2].nX + 35 && g_ptMouseRelative.y <= StarInfo[2].nY + 35		// Yellow_Star
		&& StarInfo[2].bIsLife){
			__tmInitTimer(&TimeBroken, 3000);
		}
		else if(g_ptMouseRelative.x >= StarInfo[3].nX && g_ptMouseRelative.y >= StarInfo[3].nY			// Blue_Star
		&& g_ptMouseRelative.x <= StarInfo[3].nX + 35 && g_ptMouseRelative.y <= StarInfo[3].nY + 35
		&& StarInfo[3].bIsLife){
			__tmInitTimer(&TimeBroken, 3000);
		}
		else if(g_ptMouseRelative.x >= StarInfo[4].nX && g_ptMouseRelative.y >= StarInfo[4].nY			// Pink_Star
		&& g_ptMouseRelative.x <= StarInfo[4].nX + 35 && g_ptMouseRelative.y <= StarInfo[4].nY + 35
		&& StarInfo[4].bIsLife){
			__tmInitTimer(&TimeBroken, 3000);
		}
		else if(g_ptMouseRelative.x >= StarInfo[5].nX && g_ptMouseRelative.y >= StarInfo[5].nY			// Brown_Star
		&& g_ptMouseRelative.x <= StarInfo[5].nX + 35 && g_ptMouseRelative.y <= StarInfo[5].nY + 35
		&& StarInfo[5].bIsLife){
			__tmInitTimer(&TimeBroken, 3000);
		}
		else{
			GameStart();
			// ���� �̹��� �ʱ�ȭ
			StarImageInit();	
			Info_On = false;
		}
	}

	if(StageState == STAGE_MENU){
		if(g_ptMouseAbsolute.x >= 265 && g_ptMouseAbsolute.y >= 124 && g_ptMouseAbsolute.x <= 344 && g_ptMouseAbsolute.y <= 151)
			StageState = STAGE_PLAY;
		else if(g_ptMouseAbsolute.x >= 213 && g_ptMouseAbsolute.y >= 200 && g_ptMouseAbsolute.x <= 414 && g_ptMouseAbsolute.y <= 230)
			Menu4_On();
		else if(g_ptMouseAbsolute.x >= 230 && g_ptMouseAbsolute.y >= 282 && g_ptMouseAbsolute.x <= 390 && g_ptMouseAbsolute.y <= 300)
			Menu3_On();
	}

	return true;
}

int OnMainMenuLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam){
	switch(Menu.MenuState){
	case MENU_START:	return Menu1_On();
	case MENU_CREDIT:	return Menu2_On();
	case MENU_EXIT:		return Menu3_On();
	case MENU_HOWTOPLAY:return Menu4_On();
	}
	return true;
}

//WM_MOUSEMOVE�޼��� ȣ��
//���� ���콺 ������ ��ġ ����
int OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam){	// ���콺�� ������ ����
	g_ptMouseAbsolute.x = LOWORD(lParam);					// ���콺 ���� ��ǥ�� �ǽð� ����
	g_ptMouseAbsolute.y = HIWORD(lParam);
	
	return true;
}

//WM_KEYDOWN�޼��� ȣ��
int OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam){
	// �ý��� ���¿� ���� �ٸ� KeyDown �Լ��� ȣ��
	switch(SystemState){
	case MAIN_MENU: return OnMainMenuKeyDown(hWnd, wParam, lParam);
	case GAME_START:return OnGameStartKeyDown(hWnd, wParam, lParam);
	}
	return -1;
}

// ���� �÷��� ȭ�� Űüũ �Լ�
int OnGameStartKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam){
	switch(GameState){
	case STAGE_SELECT:  return OnStageSelectKeyDown(hWnd, wParam, lParam);
	case GAME_PLAY:		return OnPlayKeyDown(hWnd, wParam, lParam);
	case GAME_MENU:		return OnGameMenuKeyDown(hWnd, wParam, lParam);
	}

	return true;
}

// �������� ȭ�� Űüũ �Լ�
int OnStageSelectKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam){
	switch(wParam){
	case VK_ESCAPE:
		if(StageState == STAGE_PLAY)
			StageState = STAGE_MENU;
		else if(StageState == STAGE_MENU)
			StageState = STAGE_PLAY;
		break;
	}
	return true;
}

// ���� �÷��� Űüũ �Լ�
int OnPlayKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam){
	switch(wParam){			// ������ ���ν������� ��ǥ����� �ϸ� �� ���� ������ ������ ������ƮAction �Լ����� ó���� �Ѵ�.
	case '1':
		Sound_EffectOn();
		if(UiState != OBJECT) UiState = OBJECT;
		else ObjectKind = STAR_SPECIAL;
		break;
	case '2':
		Sound_EffectOn();
		if(UiState == OBJECT) ObjectKind = STAR_UNDERGROUND;
		else{
			UiState = DESTROY;
			hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR3));
		}
		break;
	case '3':
		Sound_EffectOn();
		if(UiState == OBJECT) ObjectKind = STAR_GROUND;
		else UiState = INFO;
		break;
	case '4':
		Sound_EffectOn();
		if(UiState == OBJECT) ObjectKind = STAR_SKY;
		else{
			if(__tmIsValidTimer(&TimeEarthquake[0]))	  return 0;	// ù���� ū ����
			else if(__tmIsValidTimer(&TimeEarthquake[1])) return 0;
			else if(__tmIsValidTimer(&TimeEarthquake[2])) return 0;	
			else if(__tmIsValidTimer(&TimeEarthquake[3])) return 0;	// ������ ����

			fnStageInit();
		}
		break;
	case 'D': 
//		__he_pObjInfo->theta-=2;
//		if(__he_pObjInfo->theta < 0)
//			__he_pObjInfo->theta = 360 + __he_pObjInfo->theta; 
		return true;
	case 'A':
//		__he_pObjInfo->theta+=2;
//		if(__he_pObjInfo->theta > 360)
//			__he_pObjInfo->theta = __he_pObjInfo->theta - 360;
		return true;
	case VK_SPACE: 
		// �ƹ��͵� ���� ���¿��� �����̽��� ������ ����� ���� ����
		if(nObject_ID && __GObjFocusGetObjInfo()){
			int MudCount = 0;
			switch(nObject_ID){
			case ID_TREES_0: MudCount = 1; break;
			case ID_FIRE_0: MudCount = 1; break;
			case ID_WATER_0: MudCount = 1; break;
			case ID_FOOD_0: MudCount = 1; break;
			case ID_TREES_1: MudCount = 3; break;
			case ID_FIRE_1: MudCount = 3; break;
			case ID_CLOUD_1: MudCount = 1; break;
			case ID_WATER_1: MudCount = 3; break;
			case ID_FOOD_1: MudCount = 1; break;
			case ID_TREES_2: MudCount = 5; break;
			case ID_FIRE_2: MudCount = 5; break;
			case ID_CLOUD_2: MudCount = 3; break;
			case ID_WATER_2: MudCount = 3; break;
			case ID_FOOD_2:	MudCount = 5; break;
			case ID_FOOD_3: MudCount = 3; break;
			case ID_WATER_3 : MudCount = 5; break;
			case ID_WIND_1: MudCount = 10; break;
			case ID_MUDTEMPLE: MudCount = 20; break;
			case ID_CHEESESLATE: MudCount = 20; break;
			case ID_MUDTREE: MudCount = 3; break;
			}			
			if(StarInfo[StarStep].nCountMud != 0 && StarInfo[StarStep].nCountMud >= MudCount){			// ���� ���� ���� 0�� �ƴϰų� �Ҹ� ������ ���� ��
				if(__tmIsValidTimer(&TimeSpace)){					// ���� ������Ʈ���� ��ġ�� �ʴ´ٴ� �� ��Ÿ���� Ÿ�̸Ӱ� �����Ѵٸ�
					if(nObject_ID == ID_MUDTEMPLE)
						StarInfo[StarStep].bIsMudTemple = true;
					Sound_Effect_CreateOn();
					__stDispatchObject(nObject_ID, theta);			// ������Ʈ ����
 					__GObjFocusRelease();							// ��Ŀ�� ������Ʈ�� ������Ʈ ����Ʈ���� ����
					StarInfo[StarStep].nCountMud -= __ob_pObjInfo[StarStep]->MudStep;
				}
			}
			else __tmInitTimer(&TimeLack, 2000);		// ���� ���� �����ѵ� �������� �ߴٸ� �׿� ���� ������ ���� ���� Ÿ�̸� �ʱ�ȭ
		}
		return true;			
	case VK_ESCAPE: 
		if(__tmIsValidTimer(&TimeEarthquake[0]))	  return 0;	// ù���� ū ����
		else if(__tmIsValidTimer(&TimeEarthquake[1])) return 0;
		else if(__tmIsValidTimer(&TimeEarthquake[2])) return 0;	
		else if(__tmIsValidTimer(&TimeEarthquake[3])) return 0;	// ������ ����

		fnStageInit();
		return true;					
	}
	return true;
}

// ���� �÷��� ���콺 ���� ��ư üũ �Լ�
int OnPlayLbuttonDown(HWND hWnd, WPARAM wParam, LPARAM lParam){
	if(__GObjFocusGetObjInfo() && nObject_ID) {
 		__GObjFocusRelease();	// �� ���� ��Ŀ�� ������Ʈ�� �����Ѵ�.
		nObject_ID=0;
	}

	if(UiState == DESTROY) __ccCheckCollisionDestroy(g_ptMouseAbsolute);
	else hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1));

	if(g_ptMouseAbsolute.x >= 0 && g_ptMouseAbsolute.y >= 0 && g_ptMouseAbsolute.x <= 95 && g_ptMouseAbsolute.y <= 35 && UiState == BASE){// Create������ Ŭ���ߴٸ�
		Sound_EffectOn();
		UiState = OBJECT;
	}	
	else if(g_ptMouseAbsolute.x >= 131 && g_ptMouseAbsolute.y >= 0 && g_ptMouseAbsolute.x <= 235 && g_ptMouseAbsolute.y <= 35 && UiState == BASE){// Destroy������ Ŭ���ߴٸ�
		Sound_EffectOn();
		UiState = DESTROY;
		hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR3));
	}
	else if(g_ptMouseAbsolute.x >= 276 && g_ptMouseAbsolute.y >= 0 && g_ptMouseAbsolute.x <= 337 && g_ptMouseAbsolute.y <= 35 && UiState == BASE){// Info������ Ŭ���ߴٸ�
		Sound_EffectOn();
		UiState = INFO;
	}
	else if(g_ptMouseAbsolute.x >= 385 && g_ptMouseAbsolute.y >= 0 && g_ptMouseAbsolute.x <= 460 && g_ptMouseAbsolute.y <= 35 && UiState == BASE && !__tmIsValidTimer(&TimeEarthquake[0]) && !__tmIsValidTimer(&TimeEarthquake[1]) && !__tmIsValidTimer(&TimeEarthquake[2]) && !__tmIsValidTimer(&TimeEarthquake[3])){// Back������ Ŭ���ߴٸ�		
		Sound_EffectOn();
		fnStageInit();
	}
	else if(g_ptMouseAbsolute.x >= 30 && g_ptMouseAbsolute.y >= 4 && g_ptMouseAbsolute.x <= 97 && g_ptMouseAbsolute.y <= 30 && UiState == OBJECT){// TEMPLE�� Ŭ���ߴٸ�
		Sound_EffectOn();
		ObjectKind = STAR_SPECIAL;
	}
	else if(g_ptMouseAbsolute.x >= 129 && g_ptMouseAbsolute.y >= 4 && g_ptMouseAbsolute.x <= 252 && g_ptMouseAbsolute.y <= 30 && UiState == OBJECT){// UNDERGROUND�� Ŭ���ߴٸ�
		Sound_EffectOn();
		ObjectKind = STAR_UNDERGROUND;
	}
	else if(g_ptMouseAbsolute.x >= 289 && g_ptMouseAbsolute.y >= 4 && g_ptMouseAbsolute.x <= 364 && g_ptMouseAbsolute.y <= 30 && UiState == OBJECT){// GROUND�� Ŭ���ߴٸ�
		Sound_EffectOn();
		ObjectKind = STAR_GROUND;
	}
	else if(g_ptMouseAbsolute.x >= 400 && g_ptMouseAbsolute.y >= 4 && g_ptMouseAbsolute.x <= 440 && g_ptMouseAbsolute.y <= 30 && UiState == OBJECT){// SKY�� Ŭ���ߴٸ�
		Sound_EffectOn();
		ObjectKind = STAR_SKY;
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx && g_ptMouseAbsolute.y >= Tipo_ny &&				// 1�� 1��
		g_ptMouseAbsolute.x <= Tipo_nx+30 && g_ptMouseAbsolute.y <= Tipo_ny+30 && UiState == OBJECT){	
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 nObject_ID = ID_MUDTEMPLE; break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_TREES_0; break;
		case STAR_SKY:			 nObject_ID = ID_CLOUD_1; break;
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}	
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 60 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1�� 3��
			g_ptMouseAbsolute.x <= Tipo_nx+90 && g_ptMouseAbsolute.y <= Tipo_ny+30 && UiState == OBJECT){
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_FIRE_0; break;
		case STAR_SKY:			 break;
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}	
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 120 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1�� 5��
			g_ptMouseAbsolute.x <= Tipo_nx+150 && g_ptMouseAbsolute.y <= Tipo_ny+30 && UiState == OBJECT){
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 nObject_ID = ID_WATER_2; break;
		case STAR_GROUND:		 nObject_ID = ID_WATER_0; break;
		case STAR_SKY:			 break;
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}	
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 180 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1�� 7��
			g_ptMouseAbsolute.x <= Tipo_nx+210 && g_ptMouseAbsolute.y <= Tipo_ny+30 && UiState == OBJECT){
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 nObject_ID = ID_FOOD_3;break;
		case STAR_GROUND:		 nObject_ID = ID_FOOD_0; break;
		case STAR_SKY:			 break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 240 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1�� 9��
			g_ptMouseAbsolute.x <= Tipo_nx+270 && g_ptMouseAbsolute.y <= Tipo_ny+30 && UiState == OBJECT){
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_WIND_1; break;
		case STAR_SKY:			 break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2�� 1��
			g_ptMouseAbsolute.x <= Tipo_nx+30 && g_ptMouseAbsolute.y <= Tipo_ny+60 && UiState == OBJECT){
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 nObject_ID = ID_CHEESESLATE; break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_TREES_1; break;
		case STAR_SKY:			 nObject_ID = ID_CLOUD_2; break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 60 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2�� 3��
			g_ptMouseAbsolute.x <= Tipo_nx+90 && g_ptMouseAbsolute.y <= Tipo_ny+60 && UiState == OBJECT){ 
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_FIRE_1; break;
		case STAR_SKY:			 break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 120 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2�� 5��
			g_ptMouseAbsolute.x <= Tipo_nx+150 && g_ptMouseAbsolute.y <= Tipo_ny+60 && UiState == OBJECT){ 
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_WATER_1; break;
		case STAR_SKY:			 break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 180 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2�� 7��
			g_ptMouseAbsolute.x <= Tipo_nx+210 && g_ptMouseAbsolute.y <= Tipo_ny+60 && UiState == OBJECT){ 
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_FOOD_1; break;
		case STAR_SKY:			 nObject_ID = ID_CLOUD_2; break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 240 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&		// 2�� 9��
			g_ptMouseAbsolute.x <= Tipo_nx+270 && g_ptMouseAbsolute.y <= Tipo_ny+60 && UiState == OBJECT){
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_WIND_1; break;
		case STAR_SKY:			 break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3�� 1��
			g_ptMouseAbsolute.x <= Tipo_nx+30 && g_ptMouseAbsolute.y <= Tipo_ny+90 && UiState == OBJECT){
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 nObject_ID = ID_MUDTREE; break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_TREES_2; break;
		case STAR_SKY:			 break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 60 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3�� 3��
			g_ptMouseAbsolute.x <= Tipo_nx+90 && g_ptMouseAbsolute.y <= Tipo_ny+90 && UiState == OBJECT){ 
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_FIRE_2; break;
		case STAR_SKY:			 break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 120 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3�� 5��
			g_ptMouseAbsolute.x <= Tipo_nx+150 && g_ptMouseAbsolute.y <= Tipo_ny+90 && UiState == OBJECT){ 
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_WATER_3; break;
		case STAR_SKY:			 break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 180 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3�� 7��
			g_ptMouseAbsolute.x <= Tipo_nx+210 && g_ptMouseAbsolute.y <= Tipo_ny+90 && UiState == OBJECT){ 
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_FOOD_2; break;
		case STAR_SKY:			 nObject_ID = ID_CLOUD_2; break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(g_ptMouseAbsolute.x >= Tipo_nx + 240 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3�� 9��
			g_ptMouseAbsolute.x <= Tipo_nx+270 && g_ptMouseAbsolute.y <= Tipo_ny+90 && UiState == OBJECT){
		Sound_EffectOn();
		switch(ObjectKind){
		case STAR_SPECIAL:		 break;
		case STAR_UNDERGROUND:	 break;
		case STAR_GROUND:		 nObject_ID = ID_WIND_1; break;
		case STAR_SKY:			 break;	
		}
		__stDispatchFocusObject(nObject_ID, theta);
	}
	else if(UiState == OBJECT && ObjectKind != NOTHING){
		ObjectKind = NOTHING;
	}
	else if(UiState == OBJECT && ObjectKind == NOTHING){
		UiState = BASE;
	}
	else if(UiState == INFO || UiState == DESTROY){
		UiState = BASE;
	}

	return true;
}

// ���� �޴� Űüũ �Լ�
int OnGameMenuKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam){

	return true;
}

//���θ޴� Űüũ �Լ�
int OnMainMenuKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam){		// Ű���� Ű�� �������� ��
	switch(wParam){
	case VK_RETURN:		// ����
		switch(Menu.MenuState){
		case MENU_START: Menu1_On(); break;
		case MENU_CREDIT:Menu2_On(); break;
		case MENU_EXIT:  Menu3_On(); break;
		case MENU_HOWTOPLAY: Menu4_On(); break;
		}
		break;
	case VK_UP:	
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		MenuKeyCheck(wParam); // ���� �޴� ȭ���϶��� �Լ� MenuKeyCheck
		break;
	}
	return true;
}

int MenuKeyCheck(WPARAM wParam){													
	switch(wParam){
	case VK_UP:
	case VK_LEFT:
		Menu.nNowPosition--;											// �޴� ���� ���� -> �� �޴��� �̵�

		switch(Menu.nNowPosition){
		case MENU_START: Menu.MenuState = MENU_START; break;
		case MENU_CREDIT:Menu.MenuState = MENU_CREDIT; break;
		case MENU_EXIT:  Menu.MenuState = MENU_EXIT; break;
		default:
			if(Menu.nNowPosition < MENU_START){							// �ּ� �޴��ε����� �Ѿ��
				Menu.MenuState = MENU_START;							// ó�� ��ġ�� �ѵ� ����
				Menu.nNowPosition = 1;
			}
		}
		break;
	case VK_RIGHT:
	case VK_DOWN:
		Menu.nNowPosition++;											// �޴� ���� ���� -> �Ʒ� �޴��� �̵�
		
		switch(Menu.nNowPosition){
		case MENU_START: Menu.MenuState = MENU_START; break;
		case MENU_CREDIT:Menu.MenuState = MENU_CREDIT; break;
		case MENU_EXIT:  Menu.MenuState = MENU_EXIT; break;
		default:
			if(Menu.nNowPosition > MAX_MENU-1){							// �ִ� �޴��ε����� �Ѿ��
				Menu.MenuState = MENU_EXIT;								// ������ ��ġ�� �ѵ� ����
				Menu.nNowPosition = MAX_MENU-1;
			}
		}
		break;
	}
	return true;
}

int OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam){
	FPS = nfpsCount;
	nfpsCount =0;
	
	return true;
}