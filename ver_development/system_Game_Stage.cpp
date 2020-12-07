#include "Stdafx.h"

extern SURFACEINFO g_sfStageBack;
extern SURFACEINFO g_sfAdjustBG;
extern SURFACEINFO g_sfGameBG[MAX_STAR];
extern SURFACEINFO g_sfStage[MAX_STAGE_FRAME];
extern SURFACEINFO g_sfStar;
extern SURFACEINFO g_sfStageArrow[9];
extern SURFACEINFO g_sfBack;
extern SURFACEINFO g_sfStageMenu[4];
extern SURFACEINFO g_sfTextBroken;
extern SURFACEINFO g_sfEyeCatch[MAX_EYECATCH];

extern GAMESTATE GameState;
extern SYSTEMSTATE SystemState;
extern UISTATE UiState;

extern STARSTEP StarStep;
extern STARINFO StarInfo[MAX_STAR];

extern bool Info_On;

extern float theta;

extern POINTS g_ptMouseAbsolute;
extern POINTS g_ptMouseRelative;

extern TIMER TimeBroken;
extern TIMER TimeSpace;

extern int nObject_ID;

TIMER TimerStage;
TIMER TimeEyeCatch;

DWORD dwThisTick=0;	// ���� �ð��� ���� ����

int nPosition = 0;

int Scroll_X = SCR_WIDTH/2, Scroll_Y = SCR_HEIGHT/2;

int nCountEyeCatch = 0;

GSTAGE g_StageFuncptr[MAX_STAGE_FRAME];
STAGESTATE StageState = STAGE_PLAY;

void StageMenu(){
	if(g_ptMouseAbsolute.x >= 265 && g_ptMouseAbsolute.y >= 124 && g_ptMouseAbsolute.x <= 344 && g_ptMouseAbsolute.y <= 151)
		__gePutImage(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfStageMenu[1]);
	else if(g_ptMouseAbsolute.x >= 213 && g_ptMouseAbsolute.y >= 200 && g_ptMouseAbsolute.x <= 414 && g_ptMouseAbsolute.y <= 230)
		__gePutImage(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfStageMenu[2]);
	else if(g_ptMouseAbsolute.x >= 230 && g_ptMouseAbsolute.y >= 282 && g_ptMouseAbsolute.x <= 390 && g_ptMouseAbsolute.y <= 300)
		__gePutImage(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfStageMenu[3]);
	else
		__gePutImage(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfStageMenu[0]);

	RenewDC(0, 0, &g_sfBack);	
}

void StageLoop(){
	// �������� ��� �ִϸ��̼�
	if(__tmIsElapseTimer(&TimerStage)){
		__tmInitTimer(&TimerStage, 700);
		nPosition++;
		if(nPosition > MAX_STAGE_FRAME-1) nPosition = 0;
	}

	// �������� �ִϸ��̼� ��� ���
	g_StageFuncptr[nPosition].fnAction();

	// ���콺 ������ ��ġ�� ���� ���� �ٸ� �������� ������ ���� ��Ʈ�� ��� ����ϱ� ���� ���ǹ���
	// Ʃ�丮�� ���������� ���콺 �����Ͱ� ����Ű�� �ִٸ�
	if(g_ptMouseRelative.x >= StarInfo[0].nX && g_ptMouseRelative.y >= StarInfo[0].nY 
		&& g_ptMouseRelative.x <= StarInfo[0].nX + 35 && g_ptMouseRelative.y <= StarInfo[0].nY + 35){	// Tutorial
		StarStep = TUTORIAL;
		StarInfoShow(g_sfStageBack.dcSurface);
	}
	else if(g_ptMouseRelative.x >= StarInfo[1].nX && g_ptMouseRelative.y >= StarInfo[1].nY 
		&& g_ptMouseRelative.x <= StarInfo[1].nX + 35 && g_ptMouseRelative.y <= StarInfo[1].nY + 35){	// RED_STAR
		StarStep = RED_STAR;
		StarInfoShow(g_sfStageBack.dcSurface);
	}
	else if(g_ptMouseRelative.x >= StarInfo[2].nX && g_ptMouseRelative.y >= StarInfo[2].nY 
		&& g_ptMouseRelative.x <= StarInfo[2].nX+35 && g_ptMouseRelative.y <= StarInfo[2].nY + 35){	// YELLOW_STAR
		StarStep = YELLOW_STAR;
		StarInfoShow(g_sfStageBack.dcSurface);
	}
	else if(g_ptMouseRelative.x >= StarInfo[3].nX && g_ptMouseRelative.y >= StarInfo[3].nY 
		&& g_ptMouseRelative.x <= StarInfo[3].nX + 35 && g_ptMouseRelative.y <= StarInfo[3].nY + 35){	// BLUE_STAR
		StarStep = BLUE_STAR;
		StarInfoShow(g_sfStageBack.dcSurface);
	}
	else if(g_ptMouseRelative.x >= StarInfo[4].nX && g_ptMouseRelative.y >= StarInfo[4].nY 
		&& g_ptMouseRelative.x <= StarInfo[4].nX + 35 && g_ptMouseRelative.y <= StarInfo[4].nY + 35){	// PINK_STAR
		StarStep = PINK_STAR;
		StarInfoShow(g_sfStageBack.dcSurface);
	}	
	else if(g_ptMouseRelative.x >= StarInfo[5].nX && g_ptMouseRelative.y >= StarInfo[5].nY 
		&& g_ptMouseRelative.x <= StarInfo[5].nX + 35 && g_ptMouseRelative.y <= StarInfo[5].nY + 35){	// BROWN_STAR
		StarStep = BROWN_STAR;
		StarInfoShow(g_sfStageBack.dcSurface);
	}	

	ScrollCheck();

#if _DEBUG
	__bStageDisplayInfo();
#endif

	if(__tmIsValidTimer(&TimeBroken)){
		__tmIsElapseTimer(&TimeBroken);
		__gePutSprite(g_sfStageBack.dcSurface, Scroll_X, Scroll_Y, 0, 0, &g_sfTextBroken);
	}

	RenewDC(Scroll_X, Scroll_Y, &g_sfStageBack);
}

void __bStageDisplayInfo(){
	////				
	char  strBuff[255];

	//// ��Ʈ�� ������ �������� ����Ѵ�
	SetBkMode(g_sfStageBack.dcSurface, TRANSPARENT);

	//// ��Ʈ������ ����
	SetTextColor(g_sfStageBack.dcSurface, RGB(255, 255, 0));

	////
	wsprintf(strBuff, "AbsolutePointer.x : %d", g_ptMouseAbsolute.x);
	TextOut(g_sfStageBack.dcSurface, 5+Scroll_X, 85+Scroll_Y, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "AbsolutePointer.y : %d", g_ptMouseAbsolute.y);
	TextOut(g_sfStageBack.dcSurface, 5+Scroll_X, 95+Scroll_Y, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "RelativePointer.x : %d", g_ptMouseRelative.x);
	TextOut(g_sfStageBack.dcSurface, 5+Scroll_X, 105+Scroll_Y, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "RelativePointer.y : %d", g_ptMouseRelative.y);
	TextOut(g_sfStageBack.dcSurface, 5+Scroll_X, 115+Scroll_Y, strBuff, (int)strlen(strBuff));
}

// �� ��ũ�� üũ �Լ�
void ScrollCheck(){
	if(g_ptMouseAbsolute.x <= 30 && g_ptMouseAbsolute.y <= 20){											// �»�
		Scroll_X -= 3;
		Scroll_Y -= 3;
	}
	else if(g_ptMouseAbsolute.x >= SCR_WIDTH - 20 && g_ptMouseAbsolute.y <= 20){						// ���
		Scroll_X += 3;
		Scroll_Y -= 3;
	}
	else if(g_ptMouseAbsolute.y >= SCR_HEIGHT - 20 && g_ptMouseAbsolute.x <= 20){						// ����
		Scroll_X -= 3;
		Scroll_Y += 3;
	}
	else if(g_ptMouseAbsolute.x >= SCR_WIDTH - 20 && g_ptMouseAbsolute.y >= SCR_HEIGHT - 20){			// ����
		Scroll_X += 3;
		Scroll_Y += 3;
	}	
	else if(g_ptMouseAbsolute.x <= 20){																	// ��
		Scroll_X -= 3;
	}
	else if(g_ptMouseAbsolute.x >= SCR_WIDTH - 20){														// ��
		Scroll_X += 3;
	}
	else if(g_ptMouseAbsolute.y <= 20){																	// ��
		Scroll_Y -= 3;
	}
	else if(g_ptMouseAbsolute.y >= SCR_HEIGHT - 20){																	// ��
		Scroll_Y += 3;
	}

	if(Scroll_X <= 0) Scroll_X = 0;
	if(Scroll_Y <= 0) Scroll_Y = 0;
	if(Scroll_X >= SCR_WIDTH) Scroll_X = SCR_WIDTH;
	if(Scroll_Y >= SCR_HEIGHT) Scroll_Y = SCR_HEIGHT;

	// ��ũ�Ѹ��� ���� ���콺 ��� ��ǥ
	if(GameState == STAGE_SELECT && SystemState != MAIN_MENU) {g_ptMouseRelative.x = g_ptMouseAbsolute.x + Scroll_X, g_ptMouseRelative.y =  g_ptMouseAbsolute.y + Scroll_Y;}
	
	// ȭ�� ��ũ�Ѹ� ���� ǥ�� 
	if(Scroll_X >= 1 && Scroll_Y >= 1 && Scroll_X + SCR_WIDTH <= 1279 && Scroll_Y + SCR_HEIGHT <= 959)
		__gePutSprite(g_sfStageBack.dcSurface, Scroll_X, Scroll_Y, 0, 0, &g_sfStageArrow[0]);
	else if(Scroll_X <= 2 && Scroll_Y + SCR_HEIGHT >= 959)
		__gePutSprite(g_sfStageBack.dcSurface, Scroll_X, Scroll_Y, 0, 0, &g_sfStageArrow[5]);
	else if(Scroll_Y + SCR_HEIGHT >= 959 && Scroll_X + SCR_WIDTH >= 1279)
		__gePutSprite(g_sfStageBack.dcSurface, Scroll_X, Scroll_Y, 0, 0, &g_sfStageArrow[6]);
	else if(Scroll_X <= 2 && Scroll_Y <= 2)
		__gePutSprite(g_sfStageBack.dcSurface, Scroll_X, Scroll_Y, 0, 0, &g_sfStageArrow[7]);
	else if(Scroll_Y <= 2 && Scroll_X + SCR_WIDTH >= 1279)
		__gePutSprite(g_sfStageBack.dcSurface, Scroll_X, Scroll_Y, 0, 0, &g_sfStageArrow[8]);
	else if(Scroll_X <= 2)
		__gePutSprite(g_sfStageBack.dcSurface, Scroll_X, Scroll_Y, 0, 0, &g_sfStageArrow[1]);
	else if(Scroll_X + SCR_WIDTH >= 1279)
		__gePutSprite(g_sfStageBack.dcSurface, Scroll_X, Scroll_Y, 0, 0, &g_sfStageArrow[2]);
	else if(Scroll_Y <= 2)
		__gePutSprite(g_sfStageBack.dcSurface, Scroll_X, Scroll_Y, 0, 0, &g_sfStageArrow[3]);
	else if(Scroll_Y + SCR_HEIGHT >= 959)
		__gePutSprite(g_sfStageBack.dcSurface, Scroll_X, Scroll_Y, 0, 0, &g_sfStageArrow[4]);

}

void GameStart(){
	GameState = GAME_PLAY;
	UiState = BASE;
	Sound_GamePlayOn();
}

void StageInit(){
	g_StageFuncptr[0].fnAction = Stage1;
	g_StageFuncptr[1].fnAction = Stage2;
	g_StageFuncptr[2].fnAction = Stage3;
	g_StageFuncptr[3].fnAction = Stage4;
	g_StageFuncptr[4].fnAction = Stage5;
	g_StageFuncptr[5].fnAction = Stage6;
}

int Stage1(){				
	__gePutImage(g_sfStageBack.dcSurface, 0, 0, 0, 0, &g_sfStage[0]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

int Stage2(){				
	__gePutImage(g_sfStageBack.dcSurface, 0, 0, 0, 0, &g_sfStage[1]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

int Stage3(){				
	__gePutImage(g_sfStageBack.dcSurface, 0, 0, 0, 0, &g_sfStage[2]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

int Stage4(){				
	__gePutImage(g_sfStageBack.dcSurface, 0, 0, 0, 0, &g_sfStage[3]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

int Stage5(){				
	__gePutImage(g_sfStageBack.dcSurface, 0, 0, 0, 0, &g_sfStage[4]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

int Stage6(){				
	__gePutImage(g_sfStageBack.dcSurface, 0, 0, 0, 0, &g_sfStage[5]);		// �̹��� �ĸ� ǥ������ ����
	return true;
}

void fnStageInit(){
	__tmInitTimer(&TimeEyeCatch, 4000);	
	
	int temp = rand() % MAX_EYECATCH;
	nCountEyeCatch = temp;			

	// EyeCatch ���� �Լ�
	while(1){
		if(__tmIsElapseTimer(&TimeEyeCatch)){
			__tmInitTimer(&TimeEyeCatch, 4000);	
			break;
		}
		
		__gePutImage(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfEyeCatch[nCountEyeCatch]);
		RenewDC(0,0, &g_sfBack);

		// ���콺 Ŭ���� ���� ���ؼ�
		g_ptMouseRelative.x = -100;
		g_ptMouseRelative.y = -100;
	}

	__goObjInitAll();									// ������Ʈ ��ġ ������ ������ �ʱ�ȭ
	GameState = STAGE_SELECT;							// �������� ���� ȭ������
	StarImageInit();									// �� ���� ��¿� �µ��� �̹��� ����
	Sound_StageOn();
	if(nObject_ID){
		__GObjFocusRelease();
		nObject_ID = 0;										// ������Ʈ ID �ʱ�ȭ
	}
	__tmInitTimer(&TimeSpace, 700);
	nCountEyeCatch++;
}