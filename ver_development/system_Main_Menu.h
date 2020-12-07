#pragma once

const int MAX_MENU = 5;

typedef enum MenuState {MENU_TERM = 0, MENU_START, MENU_CREDIT, MENU_EXIT, MENU_HOWTOPLAY} MENUSTATE;

//�Լ� ������ ���Ǹ� �ٲ�
typedef int (*ACTION)();		// int = (*ACTION)

//�Լ� �����͸� ���� ����ϱ� ���� ����ü
typedef struct GMENU_tag{
	ACTION fnAction;
}GMENU;

//���� �޴� ���� ���� ����ü
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