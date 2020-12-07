#pragma once

const int MAX_STAGE_FRAME = 6;
typedef enum StageState {STAGE_PLAY = 0, STAGE_MENU} STAGESTATE; 

//�Լ� ������ ���Ǹ� �ٲ�
typedef int (*ACTION)();		// int = (*ACTION)

//�Լ� �����͸� ���� ����ϱ� ���� ����ü
typedef struct GSTAGE_tag{
	ACTION fnAction;
}GSTAGE;

void __bStageDisplayInfo();
void ScrollCheck();
void StageInit();
void StageLoop();
void StageMenu();
int Stage1();
int Stage2();
int Stage3();
int Stage4();
int Stage5();
int Stage6();
void GameStart();
void fnStageInit();