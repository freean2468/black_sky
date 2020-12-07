#pragma once

const int MAX_MENU = 5;

typedef enum MenuState {MENU_TERM = 0, MENU_START, MENU_CREDIT, MENU_EXIT, MENU_HOWTOPLAY} MENUSTATE;

//함수 포인터 정의를 바꿈
typedef int (*ACTION)();		// int = (*ACTION)

//함수 포인터를 쉽게 사용하기 위한 구조체
typedef struct GMENU_tag{
	ACTION fnAction;
}GMENU;

//메인 메뉴 상태 관리 구조체
typedef struct _tag{
	MENUSTATE MenuState;
	int nNowPosition;
}MENU;

void MenuInit();
void MenuLoop();
int Menu1();
int Menu2();
int Menu3();
int Menu4();
int Menu5();
int Menu1_On();
int Menu2_On();
int Menu3_On();
int Menu4_On();