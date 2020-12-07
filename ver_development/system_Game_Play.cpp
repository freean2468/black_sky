/////////////////////////////////////////////////////////
///
/// system.cpp : �� ���� ���� ���� �ڵ�
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

float theta = 0;			// ��� ȸ�� ����

TIMER TimeEarthquake[4];
TIMER TimeBigbang;

//���� ���� ����
void GameLoop()
{
	//// ������Ʈ ������ ������ ���ν�������

	// ��� ������Ʈ �ൿ (���ΰ� ����)
	__goObjActionAll();

	//// Ű �Է¿� ���� ���� ��ȯ �κ�
	// 0x80�� ��Ʈ ������ �ϴ� ������, �Էµ� ���� ASCII�� ��ϵǾ� �ִ� Ű���� Ȯ���ϱ� ���ؼ��̴�.(��Ȯ���� ����)
	if (GetKeyState('A')  & 0x80) theta += 2;
	if (GetKeyState('D') & 0x80) theta -= 2;
	// ���콺 ȸ�� ȭ��ǥ�� ���� ���� ��ȯ �κ�
	if(g_ptMouseAbsolute.x >= 75 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 114 && g_ptMouseAbsolute.y <= 266)	
		theta += 2;
	if(g_ptMouseAbsolute.x >= 528 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 570 && g_ptMouseAbsolute.y <= 266) 
		theta -= 2;
	
	if(theta < 0) theta = 360 + theta; 
    if(theta > 360) theta = theta - 360;

	// �������
	StarInfoCheck();
	fnTimeEvent(&StarInfo[StarStep]);

	// �浹����
	__ccCheckCollisionMousePointer(g_ptMouseAbsolute, MUD);				// ���� ����Ȯ���� ����� ����
	__ccCheckCollisionMousePointer(g_ptMouseAbsolute, MUDTREE_MUD);		// ������ ������ ����

	//// ���	
	// ���
	ShowBG();

	// ������Ʈ
	ShowObject();

	// ��Ŀ�� ������Ʈ
	if(nObject_ID) __gePutSprite(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfAdjustFocus); // 3. ���� �ĸ� ǥ�鿡 ��Ŀ�� ������Ʈ ��������Ʈ ���

	// UI
	ShowUI(g_sfBack.dcSurface, &g_sfUI[UiState], &g_sfInformation, &StarInfo[StarStep]);

#ifdef _DEBUG
	//// ���� ���� ���
	__bDisplayInfo();
#endif
	nfpsCount++;

	//// FPS ����		Sleep()�� �̿��Ͽ� ��Ȯ������ ������ ���� �ӵ� ����
	if(nfpsCount >= 20) Sleep(1000/600);

	// ���� ���� ����
	RandomMudCreate(PERCENT, nfpsCount);

	// ���� ���� ����
	POINTS ptEarthquake;
	ptEarthquake.x = 0; ptEarthquake.y = 0;
	ptEarthquake = Earthquake();

	// ��� üũ
	fnBigbang();

	// Complete ���� üũ
	fnComplete(g_sfBack.dcSurface, &StarInfo[StarStep]);

	//���� ȭ�� ��ũ�� ���
	RenewDC(ptEarthquake.x, ptEarthquake.y, &g_sfBack);			
}

void ShowBG(){
	if (GetKeyState('A')  & 0x80 || GetKeyState('D') & 0x80
		|| g_ptMouseAbsolute.x >= 75 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 114 && g_ptMouseAbsolute.y <= 266
		|| g_ptMouseAbsolute.x >= 528 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 570 && g_ptMouseAbsolute.y <= 266)			// Ű�� �Է¹޾��� ���� ��� ȸ��(�ӵ�������)
		__gePutImageRotateBG(&g_sfAdjustBG, theta, &g_sfGameBG[StarStep]);			// ���� ��׶����̹��� ȸ�� ���
	__gePutImage(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfAdjustBG);						// 1. ���� �ĸ� ǥ�鿡 BG ���
}

void ShowObject(){
	__gePutImage(g_sfTotalObjectRotate.dcSurface, 0, 0, 0, 0, &g_sfPlane);		// ��������Ʈ Į��Ű�� �����
	__gePutImage(g_sfTotalObjectSprite.dcSurface, 0, 0, 0, 0, &g_sfPlane);
	__gePutImage(g_sfAdjustFocus.dcSurface, 0, 0, 0, 0, &g_sfPlane);			//			``

	__goObjRenderAll(g_sfTotalObjectRotate.dcSurface);							// ������ ��� ������Ʈ�� �ִϸ��̼� & ȸ�� ���

	__gePutSprite(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfTotalObjectSprite);			// 2. ���� �ĸ� ǥ�鿡 ��� ������Ʈ ���� ��������Ʈ ���
}

void __bDisplayInfo(){
	////				
	char  strBuff[255];

	//// ��Ʈ�� ������ �������� ����Ѵ�
	SetBkMode(g_sfBack.dcSurface, TRANSPARENT);

	//// ��Ʈ������ ����
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

	//���� ���� ���������� �ִٸ�
	if(StarInfo[StarStep].bIsMudTemple)				
		n = 100;

	//FPS�ӵ��� ���� �־��� Ȯ���� �ٸ��� �Ѵ� (���ȭ�� ����)
	if(FPS >= 30) n += 800;		

	Percent = rand() % n;
	
	RandomTheta = (float)(rand() % 720);

	// ������ Ȯ���� ���� ������Ʈ ����
	if(Percent % PERCENT == 0 || Percent % (PERCENT/2) == 0){
		// �õ� ������ ������Ʈ ���̵� ��Ҵٰ� �ٽ� ��ȯ�Ѵ�.
		Old_Object_ID = nObject_ID;

		// �� �Լ��� ���� ���� ������Ʈ ���̵� �޶����� �����̴�.
		__stDispatchObject(ID_MUD, RandomTheta);
		
		nObject_ID = Old_Object_ID;
	}
}

// ������ �Ͼ�� ���� ȿ���� ���� ���� �Լ�
POINTS Earthquake(){
	POINTS pt;

	pt.x = 0, pt.y = 0;

	int temp = 0;
	int random = 0;

	if(__tmIsValidTimer(&TimeEarthquake[0])){			// ù���� ū ����
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
	else if(__tmIsValidTimer(&TimeEarthquake[2])){		// ������ ����
		__tmIsElapseTimer(&TimeEarthquake[2]);
		for(int i=0; i < 2; i++){
			random = rand() % 10;
			if(i == 0) pt.x = random;
			else if(i == 1) pt.y = random;			
		}	
		__tmInitTimer(&TimeEarthquake[3], 3000);
	}
	else if(__tmIsValidTimer(&TimeEarthquake[3])){		// ������ ����
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

//// ���� ������ ������ ������ �ľ��ϴ� �Լ�
bool CreateCheckMud(int StarStep)
{
	GOBJECT* pObjList = __goGetObjectList();
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		int nCreateCountMud = 0;

		// ��Ŀ�� ������Ʈ�� ���� ������Ʈ ���� �浹 ����	
		for (int ei=0; ei < nObjCount; ei++)
		{
 			if (pObjList[ei].nIdx != GO_EMPTY) 
			{
				if(pObjList[ei].StarStep == StarStep){
					if(pObjList[ei].Attribute == MUD){
						nCreateCountMud++;
						if(nCreateCountMud >= 3)
							return true;					// ���� ������ ������ ���� 3���� ���� �ʵ��� �Ѵ�.
					}
				}
			}
		}
	}
   	return false;		// ���� ���� 5�� ���� ����
}

void fnBigbang(){
	static int nCount = 0;

	// ���Ҹ�(���)ǥ��
	if(StarInfo[StarStep].NowEnergy <= 0 && !__tmIsValidTimer(&TimeBigbang)){	// ��ó�� �������� 0���ϰ� �Ǵ� ���� Ÿ�̸Ӹ� �ɾ��ش�.
		StarInfo[StarStep].bIsLife = true;
		__tmInitTimer(&TimeBigbang, 1000);
		nCount++;
		Sound_BigbangOn();
	}
	else if(__tmIsValidTimer(&TimeBigbang)){		
		while(1){
			if(__tmIsElapseTimer(&TimeBigbang)){	
				nCount++;

				if(nCount == 8){										// ��� �̹����� ��� ����ߴٸ�
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
							__tmInitTimer(&TimeBigbang, 0);						// �ʱ�ȭ
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

// ȸ�� �ܻ� ���ֱ�
//__gePutImage(g_sfTotalObjectMask.dcSurface, 0, 0, &g_sfTotalObjectRotate);	// ������ ���� �ʿ��� g_sfTotalObjectMask��Ʈ�� ����
//__gePutImageAND(g_sfTotalObjectSprite.dcSurface, 0, 0, &g_sfTotalObjectMask); // ���� ������ ���� ȸ�� �ܻ� ���ֱ�
//__gePutImageOR(g_sfTotalObjectSprite.dcSurface, 0, 0, &g_sfTotalObjectRotate);
