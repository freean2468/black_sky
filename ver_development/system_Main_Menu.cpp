/////////////////////////////////////////////////////////
///
/// system_Menu.cpp : ���� �޴� ���� cpp
///
/////////////////////////////////////////////////////////
#include "stdafx.h"

extern SURFACEINFO g_sfBack;
extern SURFACEINFO g_sfMenu[MAX_MENU];		// �� �� ǥ��
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

//���� �޴� ���� ����
MENU Menu;

POINTS g_ptMouseAbsolute;			// ���콺 ��ǥ
POINTS g_ptMouseRelative;

bool bIsHowToPlay = false;

GMENU g_MenuFuncptr[MAX_MENU];

void MenuLoop(){
	// ���콺 ��ǥ�� ���� �޴� ������ �������̽��� ���� �޶����Ƿ� UI�� Ȯ���Ǹ� �׶� ����
	// ���콺�� �̿��� �޴� ������, Ű�� �̿����� ���ϰ� �ȴ�.
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
	//���� �޴� ���� ���� ����ü ���� & �ʱ�ȭ
	Menu.MenuState = MENU_TERM;
	Menu.nNowPosition = 0;

	g_MenuFuncptr[0].fnAction = Menu1;
	g_MenuFuncptr[1].fnAction = Menu2;
	g_MenuFuncptr[2].fnAction = Menu3;
	g_MenuFuncptr[3].fnAction = Menu4;
	g_MenuFuncptr[4].fnAction = Menu5;
}

int Menu1(){				// ù��° �޴�
	__gePutImage(g_sfBack.dcSurface, 0, 0, Scroll_X, Scroll_Y, &g_sfMenu[0]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

int Menu2(){				// �ι�° �޴�
	__gePutImage(g_sfBack.dcSurface, 0, 0, Scroll_X, Scroll_Y, &g_sfMenu[1]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

int Menu3(){				// ����° �޴�
	__gePutImage(g_sfBack.dcSurface, 0, 0, Scroll_X, Scroll_Y, &g_sfMenu[2]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

int Menu4(){				// �׹�° �޴�
	__gePutImage(g_sfBack.dcSurface, 0, 0, Scroll_X, Scroll_Y, &g_sfMenu[3]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

int Menu5(){				// �׹�° �޴�
	__gePutImage(g_sfBack.dcSurface, 0, 0, Scroll_X, Scroll_Y, &g_sfMenu[4]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

int Menu1_On(){				// ù��° �޴�(���ӽ���)�� Ȱ��ȭ �Ǿ��� ��
	Sound_EffectOn();
	SystemState = GAME_START;	// ���� ��ŸƮ
	GameState = STAGE_SELECT;		// �������� ���� ȭ��
	FadeIn(&g_sfStage[0]);
	__tmInitTimer(&TimerStage, 200);
	return true;
}

int Menu2_On(){				// �ι�° �޴�(Credit)�� Ȱ��ȭ �Ǿ��� ��
	Sound_EffectOn();
	bIsActive = !bIsActive;									// �޼��� ������ �������� ����ؼ� ũ���� ȭ���� ����.
	__gePutImage(g_sfBack.dcSurface,0,0,0, 0, &g_sfCredit);
	RenewDC(0, 0, &g_sfBack);

	return true;
}

int Menu3_On(){				// ����° �޴�(���ӳ�����)�� Ȱ��ȭ �Ǿ��� ��
	Sound_LogoOn();
	FadeInNew(&g_sfLogo);	
	SendMessage(g_hWnd, WM_DESTROY, NULL, NULL);	// ���� �޽��� ����

	return true;
}

int Menu4_On(){				// �׹��� �޴�(HowToPlay)�� Ȱ��ȭ �Ǿ��� ��
	Sound_EffectOn();
	bIsActive = !bIsActive;									// �޼��� ������ �������� ����ؼ� HowToPlay ȭ���� ����.
	__gePutImage(g_sfBack.dcSurface,0,0,0, 0, &g_sfHowToPlay[0]);
	RenewDC(0, 0, &g_sfBack);
	bIsHowToPlay = true;
	SystemState = TERM;

	return true;
}