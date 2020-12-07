/////////////////////////////////////////////////////////
///
/// system_Menu.cpp : 메인 메뉴 관리 cpp
///
/////////////////////////////////////////////////////////
#include "stdafx.h"

extern SURFACEINFO g_sfBack;
extern SURFACEINFO g_sfMenu[MAX_MENU];		// 메 뉴 표면
extern SURFACEINFO g_sfLogo;
extern SURFACEINFO g_sfCredit;
extern SURFACEINFO g_sfHowToPlay[2];
extern SURFACEINFO g_sfAdjustBG;
extern SURFACEINFO g_sfGameBG[MAX_STAR];
extern SURFACEINFO g_sfStage[MAX_STAGE_FRAME];
extern SURFACEINFO g_sfGreen;

extern HWND g_hWnd;

extern SYSTEMSTATE SystemState;
extern GAMESTATE GameState;

extern int Scroll_X, Scroll_Y;

extern bool bIsActive;

extern TIMER TimerStage;

//메인 메뉴 상태 저장
MENU Menu;

POINTS g_ptMouseAbsolute;			// 마우스 좌표
POINTS g_ptMouseRelative;

bool bIsHowToPlay = false;

GMENU g_MenuFuncptr[MAX_MENU];

void MenuLoop(){
	// 마우스 좌표에 따른 메뉴 선택은 인터페이스에 따라 달라지므로 UI가 확정되면 그때 구현
	// 마우스를 이용한 메뉴 선택은, 키를 이용하지 못하게 된다.
	if(g_ptMouseAbsolute.x >= 285 && g_ptMouseAbsolute.y >= 230 && g_ptMouseAbsolute.x <= 325 && g_ptMouseAbsolute.y <= 275)		Menu.MenuState = MENU_START;
	else if(g_ptMouseAbsolute.x >= 75 && g_ptMouseAbsolute.y >= 325 && g_ptMouseAbsolute.x <= 108 && g_ptMouseAbsolute.y <= 358)	Menu.MenuState = MENU_CREDIT;
	else if(g_ptMouseAbsolute.x >= 500 && g_ptMouseAbsolute.y >= 350 && g_ptMouseAbsolute.x <= 532 && g_ptMouseAbsolute.y <= 380)	Menu.MenuState = MENU_EXIT;
	else if(g_ptMouseAbsolute.x >= 600 - Scroll_X && g_ptMouseAbsolute.y >= 680 - Scroll_Y && g_ptMouseAbsolute.x <= 639 - Scroll_X && g_ptMouseAbsolute.y <= 720 - Scroll_Y)	Menu.MenuState = MENU_HOWTOPLAY;
	else Menu.MenuState = MENU_TERM;

	g_MenuFuncptr[Menu.MenuState].fnAction();

//	__gePutSprite(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfGreen);

	RenewDC(0, 0, &g_sfBack);
}

void MenuInit(){
	//메인 메뉴 상태 관리 구조체 선언 & 초기화
	Menu.MenuState = MENU_TERM;
	Menu.nNowPosition = 0;

	g_MenuFuncptr[0].fnAction = Menu1;
	g_MenuFuncptr[1].fnAction = Menu2;
	g_MenuFuncptr[2].fnAction = Menu3;
	g_MenuFuncptr[3].fnAction = Menu4;
	g_MenuFuncptr[4].fnAction = Menu5;
}

int Menu1(){				// 첫번째 메뉴
	__gePutImage(g_sfBack.dcSurface, 0, 0, Scroll_X, Scroll_Y, &g_sfMenu[0]);		// 이미지 후면 표면으로 전송
	return true;
}

int Menu2(){				// 두번째 메뉴
	__gePutImage(g_sfBack.dcSurface, 0, 0, Scroll_X, Scroll_Y, &g_sfMenu[1]);		// 이미지 후면 표면으로 전송
	return true;
}

int Menu3(){				// 세번째 메뉴
	__gePutImage(g_sfBack.dcSurface, 0, 0, Scroll_X, Scroll_Y, &g_sfMenu[2]);		// 이미지 후면 표면으로 전송
	return true;
}

int Menu4(){				// 네번째 메뉴
	__gePutImage(g_sfBack.dcSurface, 0, 0, Scroll_X, Scroll_Y, &g_sfMenu[3]);		// 이미지 후면 표면으로 전송
	return true;
}

int Menu5(){				// 네번째 메뉴
	__gePutImage(g_sfBack.dcSurface, 0, 0, Scroll_X, Scroll_Y, &g_sfMenu[4]);		// 이미지 후면 표면으로 전송
	return true;
}

int Menu1_On(){				// 첫번째 메뉴(게임시작)가 활성화 되었을 때
	Sound_EffectOn();
	SystemState = GAME_START;	// 게임 스타트
	GameState = STAGE_SELECT;		// 스테이지 선택 화면
	FadeIn(&g_sfStage[0]);
	__tmInitTimer(&TimerStage, 200);
	return true;
}

int Menu2_On(){				// 두번째 메뉴(Credit)가 활성화 되었을 때
	Sound_EffectOn();
	bIsActive = !bIsActive;									// 메세지 루프을 중지시켜 계속해서 크레딧 화면을 띄운다.
	__gePutImage(g_sfBack.dcSurface,0,0,0, 0, &g_sfCredit);
	RenewDC(0, 0, &g_sfBack);

	return true;
}

int Menu3_On(){				// 세번째 메뉴(게임끝내기)가 활성화 되었을 때
	Sound_LogoOn();
	FadeInNew(&g_sfLogo);	
	SendMessage(g_hWnd, WM_DESTROY, NULL, NULL);	// 종료 메시지 보냄

	return true;
}

int Menu4_On(){				// 네번쨰 메뉴(HowToPlay)가 활성화 되었을 때
	Sound_EffectOn();
	bIsActive = !bIsActive;									// 메세지 루프을 중지시켜 계속해서 HowToPlay 화면을 띄운다.
	__gePutImage(g_sfBack.dcSurface,0,0,0, 0, &g_sfHowToPlay[0]);
	RenewDC(0, 0, &g_sfBack);
	bIsHowToPlay = true;
	SystemState = TERM;

	return true;
}