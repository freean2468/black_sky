#pragma once

#define SCR_WIDTH 640
#define SCR_HEIGHT 480

const int MAX_OBJECT = 800;						// 오브젝트 종류의 총 갯수이면서, 게임 내에서 존재할 수 있는 최대 갯수 설정
const int PERCENT	= 500;

typedef enum SystemState {MAIN_MENU = 0, GAME_START, TERM} SYSTEMSTATE;
typedef enum GameState	{STAGE_SELECT = 0, GAME_PLAY, GAME_MENU} GAMESTATE;

void GameLoop();
void __bDisplayInfo();
void ShowBG();
void ShowObject();
void RandomMudCreate(int n, int FPS);
POINTS Earthquake();
bool CreateCheckMud(int StarStep);
void fnBigbang();