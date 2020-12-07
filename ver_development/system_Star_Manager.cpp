#include "Stdafx.h"

extern SURFACEINFO g_sfStarTotalObjectRotate;
extern SURFACEINFO g_sfStarTotalObjectSprite;
extern SURFACEINFO g_sfPlane;
extern SURFACEINFO g_sfAdjustBG;
extern SURFACEINFO g_sfGameBG[MAX_STAR];
extern SURFACEINFO g_sfStageBack;
extern SURFACEINFO g_sfStarInfo;
extern SURFACEINFO g_sfStarInfoBack;
extern SURFACEINFO g_sfWhiteBack;
extern SURFACEINFO g_sfBroken;
extern SURFACEINFO g_sfComplete;
extern SURFACEINFO g_sfInformation;
extern SURFACEINFO g_sfUI[MAX_UI];
extern SURFACEINFO g_sfBack;

extern GOBJOBJECT_INFO* __ob_pObjInfo[MAX_STAR];

extern GAMESTATE GameState;

extern float theta;
extern int Scroll_X, Scroll_Y;

extern HWND g_hWnd;

STARSTEP StarStep = TUTORIAL;
STARINFO StarInfo[MAX_STAR];

bool Info_On = false;

TIMER TimeComplete;

// 별의 정보 초기화
void StarInfoInit(){
	for(int i = 0; i < MAX_STAR; i++){
		switch(i){
			case TUTORIAL:		Tutorial_Init(i); break;
			case RED_STAR:		Red_Star_Init(i);break;
			case YELLOW_STAR:	Yellow_Star_Init(i);break;
			case BLUE_STAR:		Blue_Star_Init(i);break;
			case PINK_STAR:		Pink_Star_Init(i);break;
			case BROWN_STAR:	Brown_Star_Init(i);break;
		}
	}
}

// 별의 이미지 초기화
void StarImageInit(){
	theta = 0;	

	if(__ob_pObjInfo[StarStep]){
		for(int i = 0; i < MAX_OBJECT; i++){
			__ob_pObjInfo[StarStep]->theta = (float)__ob_pObjInfo[StarStep]->Create;
		}
	}

	if(!StarInfo[StarStep].bIsLife) __gePutImageRotateBG(&g_sfAdjustBG, theta, &g_sfGameBG[StarStep]);			// 게임 백그라운드이미지 회전 출력
	else __gePutImageRotateBG(&g_sfAdjustBG, theta, &g_sfBroken);
}

void StarInfoShow(HDC hdc){	
	Info_On = true;
	StarImageInit();

	if(!StarInfo[StarStep].bIsLife && GameState == STAGE_SELECT){
		__gePutImage(g_sfStarInfoBack.dcSurface, 0, 0, 0, 0, &g_sfAdjustBG);		// 1. 별 정보 후면 표면에 BG 출력

		__gePutImage(g_sfStarTotalObjectRotate.dcSurface, 0, 0, 0, 0, &g_sfPlane);		// 스프라이트 칼라키로 배경깔기
		__gePutImage(g_sfStarTotalObjectSprite.dcSurface, 0, 0, 0, 0, &g_sfPlane);

		__goObjRenderAll(g_sfStarTotalObjectRotate.dcSurface);							// 생성된 모든 오브젝트들 애니메이션 & 회전 출력

		__gePutSprite(g_sfStarInfoBack.dcSurface, 0, 0, 0, 0, &g_sfStarTotalObjectSprite);	// 별 정보 후면 표면에 모든 오브젝트 최종 스프라이트 출력

		// 축소 출력
		// 이미지 축소시 이미지가 깨지는 것을 달리하기 위해 그래픽 모드를 바꿔 준다.
		SetStretchBltMode(g_sfStarInfo.dcSurface, COLORONCOLOR);// COLORONCOLOR : 논리연산 하지 않음

		StretchBlt(g_sfStarInfo.dcSurface, 0, 0, g_sfStarInfo.nWidth, g_sfStarInfo.nHeight, 
			g_sfWhiteBack.dcSurface, 0, 0,g_sfWhiteBack.nWidth,g_sfWhiteBack.nHeight,SRCCOPY);	

		StretchBlt(g_sfStarInfo.dcSurface, 30, 30, g_sfStarInfo.nWidth-60, g_sfStarInfo.nHeight-60, 
			g_sfStarInfoBack.dcSurface, 0, 0,g_sfStarInfoBack.nWidth,g_sfStarInfoBack.nHeight,SRCCOPY);		

		TransparentBlt(hdc, SCR_WIDTH/3 + Scroll_X - 40, SCR_HEIGHT/3+Scroll_Y - 30, g_sfStarInfo.nWidth, g_sfStarInfo.nHeight,
			g_sfStarInfo.dcSurface, 0,0, g_sfStarInfo.nWidth, g_sfStarInfo.nHeight, RGB(3,4,3));
	}
	else if(StarInfo[StarStep].bIsLife && GameState == STAGE_SELECT){
		// 축소 출력
		// 이미지 축소시 이미지가 깨지는 것을 달리하기 위해 그래픽 모드를 바꿔 준다.
		__gePutImage(g_sfStarInfoBack.dcSurface, 0, 0, 0, 0, &g_sfAdjustBG);		

		SetStretchBltMode(g_sfStarInfo.dcSurface, COLORONCOLOR);// COLORONCOLOR : 논리연산 하지 않음

		StretchBlt(g_sfStarInfo.dcSurface, 0, 0, g_sfStarInfo.nWidth, g_sfStarInfo.nHeight, 
			g_sfWhiteBack.dcSurface, 0, 0,g_sfWhiteBack.nWidth,g_sfWhiteBack.nHeight,SRCCOPY);	

		StretchBlt(g_sfStarInfo.dcSurface, 30, 30, g_sfStarInfo.nWidth-60, g_sfStarInfo.nHeight-60, 
			g_sfStarInfoBack.dcSurface, 0, 0,g_sfStarInfoBack.nWidth,g_sfStarInfoBack.nHeight,SRCCOPY);		

		TransparentBlt(hdc, SCR_WIDTH/3 + Scroll_X - 40, SCR_HEIGHT/3+Scroll_Y - 30, g_sfStarInfo.nWidth, g_sfStarInfo.nHeight,
			g_sfStarInfo.dcSurface, 0,0, g_sfStarInfo.nWidth, g_sfStarInfo.nHeight, RGB(3,4,3));
	}
	else if(GameState == GAME_PLAY){
		__gePutImage(g_sfStarInfoBack.dcSurface, 0, 0, 0, 0, &g_sfAdjustBG);		// 1. 별 정보 후면 표면에 BG 출력

		__gePutImage(g_sfStarTotalObjectRotate.dcSurface, 0, 0, 0, 0, &g_sfPlane);		// 스프라이트 칼라키로 배경깔기
		__gePutImage(g_sfStarTotalObjectSprite.dcSurface, 0, 0, 0, 0, &g_sfPlane);

		__goObjRenderAll(g_sfStarTotalObjectRotate.dcSurface);							// 생성된 모든 오브젝트들 애니메이션 & 회전 출력

		__gePutSprite(g_sfStarInfoBack.dcSurface, 0, 0, 0, 0, &g_sfStarTotalObjectSprite);	// 별 정보 후면 표면에 모든 오브젝트 최종 스프라이트 출력

		// 축소 출력
		// 이미지 축소시 이미지가 깨지는 것을 달리하기 위해 그래픽 모드를 바꿔 준다.
		SetStretchBltMode(g_sfStarInfo.dcSurface, COLORONCOLOR);// COLORONCOLOR : 논리연산 하지 않음

		StretchBlt(g_sfStarInfo.dcSurface, 0, 0, g_sfStarInfo.nWidth, g_sfStarInfo.nHeight, 
			g_sfWhiteBack.dcSurface, 0, 0,g_sfWhiteBack.nWidth,g_sfWhiteBack.nHeight,SRCCOPY);	

		StretchBlt(g_sfStarInfo.dcSurface, 30, 30, g_sfStarInfo.nWidth-60, g_sfStarInfo.nHeight-60, 
			g_sfStarInfoBack.dcSurface, 00, 00,g_sfStarInfoBack.nWidth,g_sfStarInfoBack.nHeight,SRCCOPY);		

		TransparentBlt(hdc, SCR_WIDTH/3, SCR_HEIGHT/3, g_sfStarInfo.nWidth, g_sfStarInfo.nHeight,
			g_sfStarInfo.dcSurface, 0,0, g_sfStarInfo.nWidth, g_sfStarInfo.nHeight, RGB(3,4,3));
	}
}

void Tutorial_Init(int nCount){
	// 별의 속성 정보 초기화
	StarInfo[nCount].nX = 605;
	StarInfo[nCount].nY = 680;
	StarInfo[nCount].r = 210;
	StarInfo[nCount].Attribute = TREES;
	
	StarInfo[nCount].nCountMud = 0;

	StarInfo[nCount].bIsMudTemple = false;
	
	StarInfo[nCount].Temperature = 50;
	StarInfo[nCount].Energy = 10;
	StarInfo[nCount].Food = 0;
	StarInfo[nCount].Feeling = 10;
	StarInfo[nCount].Environment = 0;

	StarInfo[nCount].NowEnvironment = 0;
	StarInfo[nCount].NowFeeling = 0;
	StarInfo[nCount].NowFood = 0;
	StarInfo[nCount].NowEnergy = 0;
	StarInfo[nCount].NowTemperature = 0;

	StarInfo[nCount].HopeTemperature = 0;
	StarInfo[nCount].HopeEnergy = 0;
	StarInfo[nCount].HopeFood = 0;
	StarInfo[nCount].HopeFeeling = 0;
	StarInfo[nCount].HopeEnvironment = 80;

	StarInfo[nCount].Hope = "푸른 별이 되는 것";
	StarInfo[nCount].bIsHope = false;

	StarInfo[nCount].Disease = DIRTY;

	StarInfo[nCount].bIsLife = false;
	
	// 수치에 따라서 타이머의 딜레이 시간이 바뀌어야 한다.
	if(StarInfo[nCount].Energy < 40){
		__tmInitTimer(&StarInfo[nCount].TimeEnergy, 90000);	// 
		StarInfo[nCount].VariableEnergy = 40;
	}
	else StarInfo[nCount].VariableEnergy = 0;

	if(StarInfo[nCount].Environment > 60){
		__tmInitTimer(&StarInfo[nCount].TimeEnvironment, 120000);
		StarInfo[nCount].VariableEnvironment = 60;
	}
	else StarInfo[nCount].VariableEnvironment = 0;

	if(StarInfo[nCount].Feeling < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFeeling, 150000);
		StarInfo[nCount].VariableFeeling = 40;
	}
	else StarInfo[nCount].VariableFeeling = 0;

	if(StarInfo[nCount].Food < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFood, 180000);
		StarInfo[nCount].VariableFood = 40;
	}
	else StarInfo[nCount].VariableFood = 0;

	__tmInitTimer(&StarInfo[nCount].TimeDisease, 110000);

	// 별의 이미지 초기화
	__gePutImageRotateBG(&g_sfAdjustBG, theta, &g_sfGameBG[nCount]);			// 게임 백그라운드이미지 회전 출력
}

void Red_Star_Init(int nCount){
	// 별의 속성 정보 초기화
	StarInfo[nCount].nX = 610;
	StarInfo[nCount].nY = 473;
	StarInfo[nCount].r = 207;
	StarInfo[nCount].Attribute = FIRE;

	StarInfo[nCount].nCountMud = 0;

	StarInfo[nCount].bIsMudTemple = false;

	StarInfo[nCount].Temperature = 60;
	StarInfo[nCount].Energy = 45;
	StarInfo[nCount].Food = 0;
	StarInfo[nCount].Feeling = 21;
	StarInfo[nCount].Environment = 10;

	StarInfo[nCount].NowEnvironment = 0;
	StarInfo[nCount].NowFeeling = 0;
	StarInfo[nCount].NowFood = 0;
	StarInfo[nCount].NowEnergy = 0;
	StarInfo[nCount].NowTemperature = 0;

	StarInfo[nCount].HopeTemperature = 80;
	StarInfo[nCount].HopeEnergy = 80;
	StarInfo[nCount].HopeFood = 0;
	StarInfo[nCount].HopeFeeling = 0;
	StarInfo[nCount].HopeEnvironment = 0;

	StarInfo[nCount].Hope = "태양이 되는 것";
	StarInfo[nCount].bIsHope = false;
	
	StarInfo[nCount].Disease = DEBILITY;

	StarInfo[nCount].bIsLife = false;

	// 수치에 따라서 타이머의 딜레이 시간이 바뀌어야 한다.
	if(StarInfo[nCount].Energy < 40){
		__tmInitTimer(&StarInfo[nCount].TimeEnergy, 90000);	// 
		StarInfo[nCount].VariableEnergy = 40;
	}
	else StarInfo[nCount].VariableEnergy = 0;

	if(StarInfo[nCount].Environment > 60){
		__tmInitTimer(&StarInfo[nCount].TimeEnvironment, 120000);
		StarInfo[nCount].VariableEnvironment = 60;
	}
	else StarInfo[nCount].VariableEnvironment = 0;

	if(StarInfo[nCount].Feeling < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFeeling, 150000);
		StarInfo[nCount].VariableFeeling = 40;
	}
	else StarInfo[nCount].VariableFeeling = 0;

	if(StarInfo[nCount].Food < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFood, 180000);
		StarInfo[nCount].VariableFood = 40;
	}
	else StarInfo[nCount].VariableFood = 0;

	__tmInitTimer(&StarInfo[nCount].TimeDisease, 110000);
	
	// 별의 이미지 초기화
	__gePutImageRotateBG(&g_sfAdjustBG, theta, &g_sfGameBG[nCount]);			// 게임 백그라운드이미지 회전 출력
}

void Yellow_Star_Init(int nCount){
	// 별의 속성 정보 초기화
	StarInfo[nCount].nX = 400;
	StarInfo[nCount].nY = 564;
	StarInfo[nCount].r = 210;
	StarInfo[nCount].Attribute = FOOD;

	StarInfo[nCount].nCountMud = 0;

	StarInfo[nCount].bIsMudTemple = false;

	StarInfo[nCount].Temperature = 50;
	StarInfo[nCount].Energy = 30;
	StarInfo[nCount].Food = 20;
	StarInfo[nCount].Feeling = 50;
	StarInfo[nCount].Environment = 10;

	StarInfo[nCount].NowEnvironment = 0;
	StarInfo[nCount].NowFeeling = 0;
	StarInfo[nCount].NowFood = 0;
	StarInfo[nCount].NowEnergy = 0;
	StarInfo[nCount].NowTemperature = 0;

	StarInfo[nCount].HopeTemperature = 0;
	StarInfo[nCount].HopeEnergy = 0;
	StarInfo[nCount].HopeFood = 0;
	StarInfo[nCount].HopeFeeling = 80;
	StarInfo[nCount].HopeEnvironment = 60;

	StarInfo[nCount].Hope = "부드러운 별이 되는 것";
	StarInfo[nCount].bIsHope = false;
	StarInfo[nCount].Disease = HUNGER;

	StarInfo[nCount].bIsLife = false;

	// 수치에 따라서 타이머의 딜레이 시간이 바뀌어야 한다.
	if(StarInfo[nCount].Energy < 40){
		__tmInitTimer(&StarInfo[nCount].TimeEnergy, 90000);	// 
		StarInfo[nCount].VariableEnergy = 40;
	}
	else StarInfo[nCount].VariableEnergy = 0;

	if(StarInfo[nCount].Environment > 60){
		__tmInitTimer(&StarInfo[nCount].TimeEnvironment, 120000);
		StarInfo[nCount].VariableEnvironment = 60;
	}
	else StarInfo[nCount].VariableEnvironment = 0;

	if(StarInfo[nCount].Feeling < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFeeling, 150000);
		StarInfo[nCount].VariableFeeling = 40;
	}
	else StarInfo[nCount].VariableFeeling = 0;

	if(StarInfo[nCount].Food < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFood, 180000);
		StarInfo[nCount].VariableFood = 40;
	}
	else StarInfo[nCount].VariableFood = 0;


	
	// 별의 이미지 초기화
	__gePutImageRotateBG(&g_sfAdjustBG, theta, &g_sfGameBG[nCount]);			// 게임 백그라운드이미지 회전 출력
}

void Blue_Star_Init(int nCount){
	// 별의 속성 정보 초기화
	StarInfo[nCount].nX = 826;
	StarInfo[nCount].nY = 590;
	StarInfo[nCount].r = 213;
	StarInfo[nCount].Attribute = WATER;

	StarInfo[nCount].nCountMud = 0;

	StarInfo[nCount].bIsMudTemple = false;

	StarInfo[nCount].Temperature = 40;
	StarInfo[nCount].Energy = 10;
	StarInfo[nCount].Food = 55;
	StarInfo[nCount].Feeling = 30;
	StarInfo[nCount].Environment = 30;

	StarInfo[nCount].NowEnvironment = 0;
	StarInfo[nCount].NowFeeling = 0;
	StarInfo[nCount].NowFood = 0;
	StarInfo[nCount].NowEnergy = 0;
	StarInfo[nCount].NowTemperature = 0;

	StarInfo[nCount].HopeTemperature = 19;
	StarInfo[nCount].HopeEnergy = 0;
	StarInfo[nCount].HopeFood = 80;
	StarInfo[nCount].HopeFeeling = 0;
	StarInfo[nCount].HopeEnvironment = 0;

	StarInfo[nCount].Hope = "예쁜 얼음별이 되는 것";
	StarInfo[nCount].bIsHope = false;

	StarInfo[nCount].Disease = COLD;

	StarInfo[nCount].bIsLife = false;

	// 수치에 따라서 타이머의 딜레이 시간이 바뀌어야 한다.
	if(StarInfo[nCount].Energy < 40){
		__tmInitTimer(&StarInfo[nCount].TimeEnergy, 90000);	// 
		StarInfo[nCount].VariableEnergy = 40;
	}
	else StarInfo[nCount].VariableEnergy = 0;

	if(StarInfo[nCount].Environment > 60){
		__tmInitTimer(&StarInfo[nCount].TimeEnvironment, 120000);
		StarInfo[nCount].VariableEnvironment = 60;
	}
	else StarInfo[nCount].VariableEnvironment = 0;

	if(StarInfo[nCount].Feeling < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFeeling, 150000);
		StarInfo[nCount].VariableFeeling = 40;
	}
	else StarInfo[nCount].VariableFeeling = 0;

	if(StarInfo[nCount].Food < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFood, 180000);
		StarInfo[nCount].VariableFood = 40;
	}
	else StarInfo[nCount].VariableFood = 0;

	__tmInitTimer(&StarInfo[nCount].TimeDisease, 110000);

	// 별의 이미지 초기화
	__gePutImageRotateBG(&g_sfAdjustBG, theta, &g_sfGameBG[nCount]);			// 게임 백그라운드이미지 회전 출력
}

void Pink_Star_Init(int nCount){
	// 별의 속성 정보 초기화
	StarInfo[nCount].nX = 387;
	StarInfo[nCount].nY = 116;
	StarInfo[nCount].r = 230;
	StarInfo[nCount].Attribute = CLOUD;

	StarInfo[nCount].nCountMud = 0;

	StarInfo[nCount].bIsMudTemple = false;

	StarInfo[nCount].Temperature = 420;
	StarInfo[nCount].Energy = 10;
	StarInfo[nCount].Food = 40;
	StarInfo[nCount].Feeling = 51;
	StarInfo[nCount].Environment = 30;

	StarInfo[nCount].NowEnvironment = 0;
	StarInfo[nCount].NowFeeling = 0;
	StarInfo[nCount].NowFood = 0;
	StarInfo[nCount].NowEnergy = 0;
	StarInfo[nCount].NowTemperature = 0;

	StarInfo[nCount].HopeTemperature = 0;
	StarInfo[nCount].HopeEnergy = 0;
	StarInfo[nCount].HopeFood = 80;
	StarInfo[nCount].HopeFeeling = 80;
	StarInfo[nCount].HopeEnvironment = 0;

	StarInfo[nCount].Hope = "달콤한 게 잔뜩있는 별이 되는 것";
	StarInfo[nCount].bIsHope = false;

	StarInfo[nCount].Disease = OLD;

	StarInfo[nCount].bIsLife = false;

	// 수치에 따라서 타이머의 딜레이 시간이 바뀌어야 한다.
	if(StarInfo[nCount].Energy < 40){
		__tmInitTimer(&StarInfo[nCount].TimeEnergy, 90000);	// 
		StarInfo[nCount].VariableEnergy = 40;
	}
	else StarInfo[nCount].VariableEnergy = 0;

	if(StarInfo[nCount].Environment > 60){
		__tmInitTimer(&StarInfo[nCount].TimeEnvironment, 120000);
		StarInfo[nCount].VariableEnvironment = 60;
	}
	else StarInfo[nCount].VariableEnvironment = 0;

	if(StarInfo[nCount].Feeling < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFeeling, 150000);
		StarInfo[nCount].VariableFeeling = 40;
	}
	else StarInfo[nCount].VariableFeeling = 0;

	if(StarInfo[nCount].Food < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFood, 180000);
		StarInfo[nCount].VariableFood = 40;
	}
	else StarInfo[nCount].VariableFood = 0;

	__tmInitTimer(&StarInfo[nCount].TimeDisease, 110000);
	
	// 별의 이미지 초기화
	__gePutImageRotateBG(&g_sfAdjustBG, theta, &g_sfGameBG[nCount]);			// 게임 백그라운드이미지 회전 출력
}

void Brown_Star_Init(int nCount){
	// 별의 속성 정보 초기화
	StarInfo[nCount].nX = 239;
	StarInfo[nCount].nY = 786;
	StarInfo[nCount].r = 213;
	StarInfo[nCount].Attribute = ALL;

	StarInfo[nCount].nCountMud = 0;

	StarInfo[nCount].bIsMudTemple = false;

	StarInfo[nCount].Temperature = 45;
	StarInfo[nCount].Energy = 10;
	StarInfo[nCount].Food = 0;
	StarInfo[nCount].Feeling = -20;
	StarInfo[nCount].Environment = 0;

	StarInfo[nCount].NowEnvironment = 0;
	StarInfo[nCount].NowFeeling = 0;
	StarInfo[nCount].NowFood = 0;
	StarInfo[nCount].NowEnergy = 0;
	StarInfo[nCount].NowTemperature = 0;

	StarInfo[nCount].HopeTemperature = 0;
	StarInfo[nCount].HopeEnergy = 80;
	StarInfo[nCount].HopeFood = 0;
	StarInfo[nCount].HopeFeeling = 80;
	StarInfo[nCount].HopeEnvironment = 0;

	StarInfo[nCount].Hope = "예쁜 별이 되는 것";
	StarInfo[nCount].bIsHope = false;

	StarInfo[nCount].Disease = MELANCHOLY;

	StarInfo[nCount].bIsLife = false;

	// 수치에 따라서 타이머의 딜레이 시간이 바뀌어야 한다.
	if(StarInfo[nCount].Energy < 40){
		__tmInitTimer(&StarInfo[nCount].TimeEnergy, 90000);	// 
		StarInfo[nCount].VariableEnergy = 40;
	}
	else StarInfo[nCount].VariableEnergy = 0;

	if(StarInfo[nCount].Environment > 60){
		__tmInitTimer(&StarInfo[nCount].TimeEnvironment, 120000);
		StarInfo[nCount].VariableEnvironment = 60;
	}
	else StarInfo[nCount].VariableEnvironment = 0;

	if(StarInfo[nCount].Feeling < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFeeling, 150000);
		StarInfo[nCount].VariableFeeling = 40;
	}
	else StarInfo[nCount].VariableFeeling = 0;

	if(StarInfo[nCount].Food < 40){
		__tmInitTimer(&StarInfo[nCount].TimeFood, 180000);
		StarInfo[nCount].VariableFood = 40;
	}
	else StarInfo[nCount].VariableFood = 0;

	__tmInitTimer(&StarInfo[nCount].TimeDisease, 110000);
	
	// 별의 이미지 초기화
	__gePutImageRotateBG(&g_sfAdjustBG, theta, &g_sfGameBG[nCount]);			// 게임 백그라운드이미지 회전 출력
}

//// 생성된 오브젝트에 따라 각각의 별에서 정보가 달라진다.
// 별의 현재 상태에 따라 정보에 영향을 끼친다.
void StarInfoCheck()
{
	GOBJECT* pObjList = __goGetObjectList();

	int Environment = 0;
	int Food = 0;
	int Temperature = 0;
	int Energy = 0;
	int Feeling = 0;


	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();

		// 오브젝트 정보 체크
		for (int ei=0; ei < nObjCount; ei++)
		{
 			if (pObjList[ei].nIdx != GO_EMPTY) 
			{
				if(pObjList[ei].Attribute != FOCUS && pObjList[ei].Attribute != MUD){
					if(pObjList[ei].StarStep == StarStep){			
						if(strcmp(pObjList[ei].pStr, "Tree0") == 0){
							Environment += 2;
							Energy += 2;
							Feeling += 2;
						}
						else if(strcmp(pObjList[ei].pStr, "Tree1") == 0){
							Environment += 5;
							Energy += 4;
							Feeling += 3;
						}
						else if(strcmp(pObjList[ei].pStr, "Tree2") == 0){
							Environment += 10;
							Energy += 6;
							Feeling += 4;
						}
						else if(strcmp(pObjList[ei].pStr, "Fire0") == 0){
							Temperature += 2;
							Environment -= 1;
							Feeling += 2;
						}
						else if(strcmp(pObjList[ei].pStr, "Fire1") == 0){
							Temperature += 5;
							Environment -= 2;
							Feeling += 3;
						}
						else if(strcmp(pObjList[ei].pStr, "Fire2") == 0){
							Temperature += 10;
							Environment -= 3;
							Feeling += 4;
						}
						else if(strcmp(pObjList[ei].pStr, "Cloud1") == 0){
							Environment += 1;
							Energy += 1;
							Feeling += 2;
						}
						else if(strcmp(pObjList[ei].pStr, "Cloud2") == 0){
							Environment += 3;
							Energy += 3;
							Feeling += 3;
						}						
						else if(strcmp(pObjList[ei].pStr, "Water0") == 0){
							Environment += 3;
							Temperature -= 2;
							Feeling += 2;
						}
						else if(strcmp(pObjList[ei].pStr, "Water1") == 0){
							Environment += 4;
							Temperature -= 5;
							Feeling += 2;
						}
						else if(strcmp(pObjList[ei].pStr, "Water2") == 0){
							Environment += 3;
							Energy += 2;
							Temperature -= 7;
							Feeling += 3;
						}
						else if(strcmp(pObjList[ei].pStr, "Water3") == 0){
							Environment += 5;
							Temperature -= 10;
							Feeling += 3;
						}
						else if(strcmp(pObjList[ei].pStr, "Food0") == 0){
							Environment -= 1;
							Food += 2;
							Feeling += 2;
						}
						else if(strcmp(pObjList[ei].pStr, "Food1") == 0){
							Environment -= 1;
							Food += 5;
							Feeling += 3;
						}
						else if(strcmp(pObjList[ei].pStr, "Food2") == 0){
							Environment -= 1;
							Food += 8;
							Feeling += 4;
						}						
						else if(strcmp(pObjList[ei].pStr, "Food3") == 0){
							Environment -= 2;
							Food += 10;
							Feeling += 3;
						}
						else if(strcmp(pObjList[ei].pStr, "Wind1") == 0){
							Environment += 2;
							Energy += 3;
							Temperature -= 1;
							Feeling += 4;
						}
						else if(strcmp(pObjList[ei].pStr, "MudTemple") == 0){
							Environment -= 20;
							Feeling -= 20;
						}
						else if(strcmp(pObjList[ei].pStr, "CheeseSlate") == 0){
							Environment -= 5;
							Food += 30;
							Feeling += 10;
						}
						else if(strcmp(pObjList[ei].pStr, "MudTree") == 0){
							Environment -= 5;
							Energy -= 5;
							Feeling -= 5;
						}
					}
				}
			}
		}
	}

	StarInfo[StarStep].NowEnvironment = StarInfo[StarStep].Environment + Environment;
	StarInfo[StarStep].NowFood = StarInfo[StarStep].Food + Food;
	StarInfo[StarStep].NowTemperature = StarInfo[StarStep].Temperature + Temperature;
	StarInfo[StarStep].NowEnergy = StarInfo[StarStep].Energy + Energy;
	StarInfo[StarStep].NowFeeling = StarInfo[StarStep].Feeling + Feeling;

	switch(StarStep){
		case TUTORIAL:
			if(StarInfo[StarStep].NowFeeling >= 40 && StarInfo[StarStep].NowEnvironment >= 60){
				StarInfo[StarStep].Disease = HEALTHY;
			}
			break;
		case RED_STAR:
			if(StarInfo[StarStep].NowFeeling >= 40 && StarInfo[StarStep].NowEnergy >= 60){
				StarInfo[StarStep].Disease = HEALTHY;
			}
			break;
		case BLUE_STAR:
			if(StarInfo[StarStep].NowEnvironment >= 40 && StarInfo[StarStep].NowFeeling >= 40 && StarInfo[StarStep].NowEnergy >= 40){
				StarInfo[StarStep].Disease = HEALTHY;
			}
			break;
		case YELLOW_STAR:
			if(StarInfo[StarStep].NowFeeling >= 40 && StarInfo[StarStep].NowEnergy >= 60){
				StarInfo[StarStep].Disease = HEALTHY;
			}
			break;
		case PINK_STAR:
			if(StarInfo[StarStep].NowFood >= 60 && StarInfo[StarStep].NowEnergy >= 40){
				StarInfo[StarStep].Disease = HEALTHY;
			}
			break;
		case BROWN_STAR:
			if(StarInfo[StarStep].NowEnvironment >= 40 && StarInfo[StarStep].NowFeeling >= 60 && StarInfo[StarStep].NowEnergy >= 40){
				StarInfo[StarStep].Disease = HEALTHY;
			}
			break;
	}		
}

void fnComplete(HDC hdc, STARINFO* StarInfo){
	static int nAlpha = 0;

	if(StarInfo->NowEnergy >= StarInfo->HopeEnergy && StarInfo->NowEnvironment >= StarInfo->HopeEnvironment &&
		StarInfo->NowFeeling >= StarInfo->HopeFeeling && StarInfo->NowFood >= StarInfo->HopeFood && 
		StarInfo->NowTemperature >= StarInfo->HopeTemperature && !StarInfo->bIsHope){		
		if(!__tmIsValidTimer(&TimeComplete)){
			__tmInitTimer(&TimeComplete, 6000);
			Sound_CompleteOn();
		}

		if(!__tmIsElapseTimer(&TimeComplete)){		
			__gePutImageBlend(hdc, 0, 0, &g_sfComplete, nAlpha);

			nAlpha+=2;

			if(nAlpha >= 255) nAlpha = 255;
		}
		else{
			nAlpha = 0;
			StarInfo->bIsHope = true;
			__tmInitTimer(&TimeComplete, 0);
			Sound_GamePlayOn();
		}
	}
	else if(StarStep == BLUE_STAR && StarInfo->NowEnergy >= StarInfo->HopeEnergy && StarInfo->NowEnvironment >= StarInfo->HopeEnvironment &&
		StarInfo->NowFeeling >= StarInfo->HopeFeeling && StarInfo->NowFood >= StarInfo->HopeFood && 
		StarInfo->NowTemperature <= StarInfo->HopeTemperature && !StarInfo->bIsHope){
		if(!__tmIsValidTimer(&TimeComplete)){
			__tmInitTimer(&TimeComplete, 6000);
			Sound_CompleteOn();
		}

		if(!__tmIsElapseTimer(&TimeComplete)){
			nAlpha++;

			if(nAlpha >= 255) nAlpha = 255;
			
			__gePutImageBlend(hdc, 0, 0, &g_sfComplete, nAlpha);
		}
		else{
			nAlpha = 0;
			StarInfo->bIsHope = true;
			__tmInitTimer(&TimeComplete, 0);
			Sound_GamePlayOn();
		}	
	}
}