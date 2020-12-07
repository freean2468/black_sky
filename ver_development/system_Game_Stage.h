#pragma once

const int MAX_STAGE_FRAME = 6;
typedef enum StageState {STAGE_PLAY = 0, STAGE_MENU} STAGESTATE; 

//함수 포인터 정의를 바꿈
typedef int (*ACTION)();		// int = (*ACTION)

//함수 포인터를 쉽게 사용하기 위한 구조체
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