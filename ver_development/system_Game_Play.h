#pragma once

#define SCR_WIDTH 640
#define SCR_HEIGHT 480

const int MAX_OBJECT = 800;						// ������Ʈ ������ �� �����̸鼭, ���� ������ ������ �� �ִ� �ִ� ���� ����
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