/////////////////////////////////////////////////////////
///
/// system.cpp : 별 내부 진행 관련 코드
///
/////////////////////////////////////////////////////////
#include "stdafx.h"

extern HWND g_hWnd;

extern SURFACEINFO g_sfPlane;
extern SURFACEINFO g_sfBack;
extern SURFACEINFO g_sfAdjustBG;
extern SURFACEINFO g_sfAdjustFocus;
extern SURFACEINFO g_sfGameBG[MAX_STAR];
extern SURFACEINFO g_sfTotalObjectRotate;
extern SURFACEINFO g_sfTotalObjectSprite;
extern SURFACEINFO g_sfTotalObjectMask;
extern SURFACEINFO g_sfUI[MAX_UI];
extern SURFACEINFO g_sfInformation;
extern SURFACEINFO g_sfTipo[MAX_OBJECT_KIND];
extern SURFACEINFO g_sfWarning;
extern SURFACEINFO g_sfBigbang[7];
extern SURFACEINFO g_sfWhiteBack;
extern SURFACEINFO g_sfWhitePlane;
extern SURFACEINFO g_sfTextBigbang;

SYSTEMSTATE SystemState;
GAMESTATE	GameState;
extern UISTATE UiState;
extern OBJECTKIND ObjectKind;
extern STARSTEP StarStep;
extern STARINFO StarInfo[MAX_STAR];

extern POINTS g_ptMouseAbsolute;
extern POINTS g_ptMouseRelative;

extern TIMER TimeSpace;

extern bool bIsEarthquake;

int FPS, nfpsCount = 0;

int nObject_ID = 0;

float theta = 0;			// 배경 회전 각도

TIMER TimeEarthquake[4];
TIMER TimeBigbang;

//게임 메인 루프
void GameLoop()
{
	//// 오브젝트 생성은 윈도우 프로시저에서

	// 모든 오브젝트 행동 (주인공 포함)
	__goObjActionAll();

	//// 키 입력에 의해 각도 변환 부분
	// 0x80의 비트 연산을 하는 이유는, 입력된 값이 ASCII에 등록되어 있는 키인지 확인하기 위해서이다.(정확하지 않음)
	if (GetKeyState('A')  & 0x80) theta += 2;
	if (GetKeyState('D') & 0x80) theta -= 2;
	// 마우스 회전 화살표에 의해 각도 변환 부분
	if(g_ptMouseAbsolute.x >= 75 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 114 && g_ptMouseAbsolute.y <= 266)	
		theta += 2;
	if(g_ptMouseAbsolute.x >= 528 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 570 && g_ptMouseAbsolute.y <= 266) 
		theta -= 2;
	
	if(theta < 0) theta = 360 + theta; 
    if(theta > 360) theta = theta - 360;

	// 정보계산
	StarInfoCheck();
	fnTimeEvent(&StarInfo[StarStep]);

	// 충돌검출
	__ccCheckCollisionMousePointer(g_ptMouseAbsolute, MUD);				// 땅에 일정확률로 생기는 찰흙
	__ccCheckCollisionMousePointer(g_ptMouseAbsolute, MUDTREE_MUD);		// 나무에 열리는 찰흙

	//// 출력	
	// 배경
	ShowBG();

	// 오브젝트
	ShowObject();

	// 포커스 오브젝트
	if(nObject_ID) __gePutSprite(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfAdjustFocus); // 3. 게임 후면 표면에 포커스 오브젝트 스프라이트 출력

	// UI
	ShowUI(g_sfBack.dcSurface, &g_sfUI[UiState], &g_sfInformation, &StarInfo[StarStep]);

#ifdef _DEBUG
	//// 각종 정보 출력
	__bDisplayInfo();
#endif
	nfpsCount++;

	//// FPS 조절		Sleep()을 이용하여 정확하지는 않지만 대충 속도 조절
	if(nfpsCount >= 20) Sleep(1000/600);

	// 찰흙 랜덤 생성
	RandomMudCreate(PERCENT, nfpsCount);

	// 랜덤 지진 생성
	POINTS ptEarthquake;
	ptEarthquake.x = 0; ptEarthquake.y = 0;
	ptEarthquake = Earthquake();

	// 빅뱅 체크
	fnBigbang();

	// Complete 조건 체크
	fnComplete(g_sfBack.dcSurface, &StarInfo[StarStep]);

	//최종 화면 스크린 출력
	RenewDC(ptEarthquake.x, ptEarthquake.y, &g_sfBack);			
}

void ShowBG(){
	if (GetKeyState('A')  & 0x80 || GetKeyState('D') & 0x80
		|| g_ptMouseAbsolute.x >= 75 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 114 && g_ptMouseAbsolute.y <= 266
		|| g_ptMouseAbsolute.x >= 528 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 570 && g_ptMouseAbsolute.y <= 266)			// 키를 입력받았을 때만 배경 회전(속도때문에)
		__gePutImageRotateBG(&g_sfAdjustBG, theta, &g_sfGameBG[StarStep]);			// 게임 백그라운드이미지 회전 출력
	__gePutImage(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfAdjustBG);						// 1. 게임 후면 표면에 BG 출력
}

void ShowObject(){
	__gePutImage(g_sfTotalObjectRotate.dcSurface, 0, 0, 0, 0, &g_sfPlane);		// 스프라이트 칼라키로 배경깔기
	__gePutImage(g_sfTotalObjectSprite.dcSurface, 0, 0, 0, 0, &g_sfPlane);
	__gePutImage(g_sfAdjustFocus.dcSurface, 0, 0, 0, 0, &g_sfPlane);			//			``

	__goObjRenderAll(g_sfTotalObjectRotate.dcSurface);							// 생성된 모든 오브젝트들 애니메이션 & 회전 출력

	__gePutSprite(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfTotalObjectSprite);			// 2. 게임 후면 표면에 모든 오브젝트 최종 스프라이트 출력
}

void __bDisplayInfo(){
	////				
	char  strBuff[255];

	//// 폰트의 투명한 배경색상을 사용한다
	SetBkMode(g_sfBack.dcSurface, TRANSPARENT);

	//// 폰트색상을 지정
	SetTextColor(g_sfBack.dcSurface, RGB(0, 0, 255));

	////
	wsprintf(strBuff, "FPS : %d", FPS);
	TextOut(g_sfBack.dcSurface, 5, 55, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "nObject_ID : %d", nObject_ID);
	TextOut(g_sfBack.dcSurface, 5, 65, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "BG's theta : %d", (int)theta);
	TextOut(g_sfBack.dcSurface, 5, 45, strBuff, (int)strlen(strBuff));
	//wsprintf(strBuff, "Hero's theta : %d", (int)theta);
	//TextOut(g_sfBack.dcSurface, 5, 45, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "nObject_ID : %d", nObject_ID);
	TextOut(g_sfBack.dcSurface, 5, 75, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "AbsolutePointer.x : %d", g_ptMouseAbsolute.x);
	TextOut(g_sfBack.dcSurface, 5, 85, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "AbsolutePointer.y : %d", g_ptMouseAbsolute.y);
	TextOut(g_sfBack.dcSurface, 5, 95, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "RelativePointer.x : %d", g_ptMouseRelative.x);
	TextOut(g_sfBack.dcSurface, 5, 105, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "RelativePointer.y : %d", g_ptMouseRelative.y);
	TextOut(g_sfBack.dcSurface, 5, 115, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "nCountMud : %d", StarInfo[StarStep].nCountMud);
	TextOut(g_sfBack.dcSurface, 5, 125, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "Environment : %d", StarInfo[StarStep].NowEnvironment);
	TextOut(g_sfBack.dcSurface, 5, 135, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "Food : %d", StarInfo[StarStep].NowFood);
	TextOut(g_sfBack.dcSurface, 5, 145, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "Temperature : %d", StarInfo[StarStep].NowTemperature);
	TextOut(g_sfBack.dcSurface, 5, 155, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "Feeling : %d", StarInfo[StarStep].NowFeeling);
	TextOut(g_sfBack.dcSurface, 5, 165, strBuff, (int)strlen(strBuff));
	wsprintf(strBuff, "Energy : %d", StarInfo[StarStep].NowEnergy);
	TextOut(g_sfBack.dcSurface, 5, 175, strBuff, (int)strlen(strBuff));
}

void RandomMudCreate(int n, int FPS){
	if(CreateCheckMud(StarStep)) return;

	unsigned int Percent;
	float RandomTheta;

	int Old_Object_ID = 0;

	//현재 별에 찰흙템플이 있다면
	if(StarInfo[StarStep].bIsMudTemple)				
		n = 100;

	//FPS속도에 따라 주어진 확률을 다르게 한다 (평균화를 위해)
	if(FPS >= 30) n += 800;		

	Percent = rand() % n;
	
	RandomTheta = (float)(rand() % 720);

	// 일정한 확률로 진흙 오브젝트 생성
	if(Percent % PERCENT == 0 || Percent % (PERCENT/2) == 0){
		// 올드 변수에 오브젝트 아이디를 담았다가 다시 반환한다.
		Old_Object_ID = nObject_ID;

		// 이 함수로 인해 현재 오브젝트 아이디가 달라지기 때문이다.
		__stDispatchObject(ID_MUD, RandomTheta);
		
		nObject_ID = Old_Object_ID;
	}
}

// 지진이 일어나는 듯한 효과를 내기 위한 함수
POINTS Earthquake(){
	POINTS pt;

	pt.x = 0, pt.y = 0;

	int temp = 0;
	int random = 0;

	if(__tmIsValidTimer(&TimeEarthquake[0])){			// 첫번쨰 큰 지진
		__tmIsElapseTimer(&TimeEarthquake[0]);
		for(int i=0; i < 2; i++){
			random = rand() % 40;
			if(i == 0) pt.x = random;
			else if(i == 1) pt.y = random;			
		}
		__tmInitTimer(&TimeEarthquake[1], 1000);
	}
	else if(__tmIsValidTimer(&TimeEarthquake[1])){
		__tmIsElapseTimer(&TimeEarthquake[1]);
		for(int i=0; i < 2; i++){
			random = rand() % 22;
			if(i == 0) pt.x = random;
			else if(i == 1) pt.y = random;			
		}
		__tmInitTimer(&TimeEarthquake[2], 1000);
	}
	else if(__tmIsValidTimer(&TimeEarthquake[2])){		// 마지막 여진
		__tmIsElapseTimer(&TimeEarthquake[2]);
		for(int i=0; i < 2; i++){
			random = rand() % 10;
			if(i == 0) pt.x = random;
			else if(i == 1) pt.y = random;			
		}	
		__tmInitTimer(&TimeEarthquake[3], 3000);
	}
	else if(__tmIsValidTimer(&TimeEarthquake[3])){		// 마지막 여진
		if(__tmIsElapseTimer(&TimeEarthquake[3])){
			Sound_GamePlayOn();
			bIsEarthquake = false;	
		}
		__gePutSprite(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfWarning);
	}
	else if(bIsEarthquake){
		Sound_EarthquakeOn();
		for(int i=0; i < 2; i++){
			random = rand() % 20;							
			if(i == 0) pt.x = random;
			else if(i == 1) pt.y = random;
		}
		__tmInitTimer(&TimeEarthquake[0], 2000);
	}

	return pt;
}

//// 현재 생성된 찰흙의 갯수를 파악하는 함수
bool CreateCheckMud(int StarStep)
{
	GOBJECT* pObjList = __goGetObjectList();
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		int nCreateCountMud = 0;

		// 포커스 오브젝트와 생성 오브젝트 간의 충돌 검출	
		for (int ei=0; ei < nObjCount; ei++)
		{
 			if (pObjList[ei].nIdx != GO_EMPTY) 
			{
				if(pObjList[ei].StarStep == StarStep){
					if(pObjList[ei].Attribute == MUD){
						nCreateCountMud++;
						if(nCreateCountMud >= 3)
							return true;					// 현재 생성된 찰흙의 수가 3개를 넘지 않도록 한다.
					}
				}
			}
		}
	}
   	return false;		// 찰흙 수가 5개 보다 적다
}

void fnBigbang(){
	static int nCount = 0;

	// 별소멸(빅뱅)표현
	if(StarInfo[StarStep].NowEnergy <= 0 && !__tmIsValidTimer(&TimeBigbang)){	// 맨처음 에너지가 0이하가 되는 순간 타이머를 걸어준다.
		StarInfo[StarStep].bIsLife = true;
		__tmInitTimer(&TimeBigbang, 1000);
		nCount++;
		Sound_BigbangOn();
	}
	else if(__tmIsValidTimer(&TimeBigbang)){		
		while(1){
			if(__tmIsElapseTimer(&TimeBigbang)){	
				nCount++;

				if(nCount == 8){										// 빅뱅 이미지를 모두 출력했다면
					TIMER TimeWhite;
					TIMER TimeTerm;

					__tmInitTimer(&TimeWhite, 7000);
					__tmInitTimer(&TimeTerm, 1);
					while(1){
						int x, y;		
						static int nAlpha = 0;							

						if(__tmIsElapseTimer(&TimeTerm)){
							__tmInitTimer(&TimeTerm, 1);
							nAlpha++;
						}

						if(nAlpha < 255){
							x = rand() % 20;
							y = rand() % 20;
						}
						else{
							x = 0;
							y = 0;
						}

						if(__tmIsElapseTimer(&TimeWhite)){
							fnStageInit();
							__tmInitTimer(&TimeBigbang, 0);						// 초기화
							nAlpha = 0;
							break;
						}
						__gePutImageBlend(g_sfBack.dcSurface, x, y, &g_sfWhitePlane, nAlpha); 	

						if(nAlpha >= 255){
							__gePutImage(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfTextBigbang);
						}

						RenewDC(x,y,&g_sfBack);
					}					
					break;
				}
				__tmInitTimer(&TimeBigbang,800);				
				__gePutImage(g_sfBack.dcSurface, 0,0,0,0,&g_sfBigbang[nCount-1]);				
			}
			int x, y;			

			if(nCount < 4){
				x = rand() % 20;
				y = rand() % 20;
			}
			else if(nCount < 8){
				x = rand() % 40;
				y = rand() % 40;
			}

			RenewDC(x,y,&g_sfBack);
		}
	}

	nCount = 0;
}

// 회전 잔상 없애기
//__gePutImage(g_sfTotalObjectMask.dcSurface, 0, 0, &g_sfTotalObjectRotate);	// 연산을 위해 필요한 g_sfTotalObjectMask비트맵 생성
//__gePutImageAND(g_sfTotalObjectSprite.dcSurface, 0, 0, &g_sfTotalObjectMask); // 각종 연산을 통해 회전 잔상 없애기
//__gePutImageOR(g_sfTotalObjectSprite.dcSurface, 0, 0, &g_sfTotalObjectRotate);
