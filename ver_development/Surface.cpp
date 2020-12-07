/////////////////////////////////////////////////////////
///
/// surface.cpp : ǥ�� ���� cpp
///
/////////////////////////////////////////////////////////
#include "stdafx.h"

extern HWND g_hWnd;
extern float theta;

HDC ScrHDC;								// ��ũ�� HDC

SURFACEINFO g_sfBack;					// �� �� ǥ��
SURFACEINFO g_sfStageBack;				// �������� �ĸ� ǥ��
SURFACEINFO g_sfAdjustBG;				// BG ����ȭ ����� ���� ǥ��
SURFACEINFO g_sfAdjustFocus;			// ��Ŀ��������Ʈ���� ���� ǥ��
SURFACEINFO g_sfPlane;					// �� �� ǥ��
SURFACEINFO g_sfWhitePlane;				// �Ͼ� �ٴ� ǥ��
SURFACEINFO g_sfMenu[MAX_MENU];			// �� �� ǥ��
SURFACEINFO g_sfLogo;					// �� �� ǥ��
SURFACEINFO g_sfCredit;					// ũ���� ǥ��
SURFACEINFO g_sfHowToPlay[3];			// How To Play ǥ��
SURFACEINFO g_sfGameBG[MAX_STAR];		// ���� BackGround ǥ��
SURFACEINFO g_sfTotalObjectRotate;		// ��� ������Ʈ�� ȸ�� ó���Ǿ� ��� ǥ��
SURFACEINFO g_sfTotalObjectSprite;		// ��� ������Ʈ�� ��������Ʈ ó���Ǿ� ��� ǥ��
SURFACEINFO g_sfStarTotalObjectRotate;	// StarInfo���� ��� ������Ʈ�� ȸ�� ó���Ǿ� ��� ǥ��
SURFACEINFO g_sfStarTotalObjectSprite;	// StarInfo���� ��� ������Ʈ�� ��������Ʈ ó���Ǿ� ��� ǥ��
SURFACEINFO g_sfInformation;			// �� Info ǥ��
SURFACEINFO g_sfStage[MAX_STAGE_FRAME];	// �������� ǥ��
SURFACEINFO g_sfStarInfo;				// �� ���� ǥ��
SURFACEINFO g_sfStarInfoBack;			// �� ���� �ĸ� ǥ��
SURFACEINFO g_sfUI[MAX_UI];				// ���� �������̽� ǥ��
SURFACEINFO g_sfTipo[MAX_OBJECT_KIND]; 	// Ÿ�����׷��� ǥ��	
SURFACEINFO g_sfObjectInformation[5][20];	// ������Ʈ ���� ǥ��
SURFACEINFO g_sfWhiteBack;				// �Ͼ�� ǥ��
SURFACEINFO g_sfLack;					// ���� ���� ���� ǥ��
SURFACEINFO g_sfSpace[2];				// �����̽� ǥ��
SURFACEINFO g_sfStageArrow[9];			// �������� ȭ�� ǥ��
SURFACEINFO g_sfPlayArrow;				// �÷��� ȭ�� ǥ��
SURFACEINFO g_sfStageMenu[4];			// �������� �޴� ǥ��
SURFACEINFO g_sfWarning;				// ��� ǥ��
SURFACEINFO g_sfInfoBack;				// InfoBack ǥ��
SURFACEINFO g_sfHopeInfoBack[MAX_STAR];	// HopeInfoBack ǥ��
SURFACEINFO g_sfDiseaseInfoBack[MAX_STAR+1];	// DiseaseInfoBack ǥ��
SURFACEINFO g_sfBigbang[7];				// Bigbang ǥ��
SURFACEINFO g_sfTextBigbang;			// TextBigbang ǥ��
SURFACEINFO g_sfBroken;					// Broken ǥ��
SURFACEINFO g_sfTextBroken;				// TextBroken ǥ��
SURFACEINFO g_sfComplete;				// Complete ǥ��
SURFACEINFO g_sfEyeCatch[MAX_EYECATCH]; // EyeCatch ǥ��

//�ϰ� ǥ�� ����
void __aInit(HDC hdc){
	__geCreateBackBuffer(hdc, 640, 480, 32, &g_sfBack, RGB(0, 0, 0));					// �ĸ���� ����
	__geCreateBackBuffer(hdc, 1280, 960, 32, &g_sfStageBack, RGB(0, 0, 0));				// �������� �ĸ���� ����
	// �� ������� ũ��� �׷��� ���ɰ� �¿�ȴ�.
	__geCreateBackBuffer(hdc, 960, 960, 32, &g_sfAdjustBG, RGB(0, 0, 0));				// ��� �ĸ� ���� ����
	__geCreateBackBuffer(hdc, 640, 480, 32, &g_sfAdjustFocus, RGB(0, 0, 0));			// ��Ŀ�� ������Ʈ �ĸ� ���� ����
	__geCreateBackBuffer(hdc, 280, 210, 32, &g_sfStarInfo, RGB(255, 255, 255));			// �� ���� ǥ�� ����
	__geCreateBackBuffer(hdc, 640, 480, 32, &g_sfTotalObjectSprite, RGB(0,0,0));		// ������Ʈ ��������Ʈ ǥ�� ����
	__geCreateBackBuffer(hdc, 640, 480, 32, &g_sfTotalObjectRotate, RGB(0,0,0));	// ������Ʈ ȸ�� ǥ�� ����

	// �� �Ʒ� �� ǥ���� �ʱ�ȭ ������ �Ȱ��� ����� �Ѵ�.
	// �ٴ� ǥ���� ����� �� �Ʒ� �ʱ�ȭ ���ٴ� Ŀ�� �Ѵ�.
	// �� �Ʒ� ������� ũ����� ���� �ӵ��� �¿�ȴ�.
	// ����x���� ������ 4*3
	__geCreateBackBuffer(hdc, 960, 720, 32, &g_sfStarTotalObjectRotate, RGB(0,0,0));	// ������Ʈ ȸ�� ǥ�� ����
	__geCreateBackBuffer(hdc, 960, 720, 32, &g_sfStarTotalObjectSprite, RGB(0,0,0));	// ������Ʈ ��������Ʈ ǥ�� ����
	__geCreateBackBuffer(hdc, 960, 720, 32, &g_sfStarInfoBack, RGB(0, 0, 0));			// �� ���� �ĸ� ǥ�� ����

	//�޴� ǥ�� ���� & �ʱ�ȭ
	fnSetMenuSurface(hdc);

	//�ΰ� ǥ�� ����
	__geSetImgSurface(&g_sfLogo);												// ǥ�� �ʱ�ȭ
	g_sfLogo.hBmp = __geBmpFile2BmpHandle("./Image/Logo/Logo_Sider.bmp");		// �ΰ� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfLogo);											// �ĸ� ǥ������ �ε�
	
	//�ٴ� ǥ�� ����
	__geSetImgSurface(&g_sfPlane);
	g_sfPlane.hBmp = __geBmpFile2BmpHandle("./Image/BG/Plane.bmp");
	__geLoadSurface(hdc, &g_sfPlane);

	//ũ���� ǥ�� ����
	__geSetImgSurface(&g_sfCredit);
	g_sfCredit.hBmp = __geBmpFile2BmpHandle("./Image/Credit/Credit.bmp");
	__geLoadSurface(hdc, &g_sfCredit);

	// How To Play ǥ�� ����
	fnSetHowToPlaySurface(hdc);

	//���� BG ǥ�� ����
	fnSetGameBGSurface(hdc);

	// �������� ǥ�� ����
	fnSetStageSurface(hdc);

	// UI ǥ�� ����
	fnSetUISurface(hdc);

	// Information ǥ�� ����
	fnSetInformationSurface(hdc);

	// Ÿ���� ǥ�� ����
	fnSetTipoSurface(hdc);

	/// ������Ʈ ǥ�� ����
	fnSetObjectInformationSurface(hdc);

	// �Ͼ�� ǥ�� ����
	__geSetSprSurface(&g_sfWhiteBack, RGB(3,4,3));
	g_sfWhiteBack.hBmp = __geBmpFile2BmpHandle("./Image/BG/White.bmp");
	__geLoadSurface(hdc, &g_sfWhiteBack);

	// ���� ���� ǥ�� ����
	__geSetSprSurface(&g_sfLack, RGB(0,0,0));
	g_sfLack.hBmp = __geBmpFile2BmpHandle("./Image/UI/Lack_Mud.bmp");
	__geLoadSurface(hdc, &g_sfLack);

	// �����̽��� ǥ�� ����
	fnSetSpaceSurface(hdc);

	// �������� ȭ��ǥ ǥ�� ����
	fnSetStageArrowSurface(hdc);

	// �÷��� ȭ�� ǥ�� ����
	__geSetSprSurface(&g_sfPlayArrow, RGB(0,0,0));
	g_sfPlayArrow.hBmp = __geBmpFile2BmpHandle("./Image/UI/PlayArrow/Arrow.bmp");
	__geLoadSurface(hdc, &g_sfPlayArrow);

	// �������� �޴� ǥ�� ����
	fnSetStageMenuSurface(hdc);

	// ��� ǥ�� ����
	__geSetSprSurface(&g_sfWarning, RGB(0,0,0));
	g_sfWarning.hBmp = __geBmpFile2BmpHandle("./Image/UI/Warning.bmp");
	__geLoadSurface(hdc, &g_sfWarning);

	// InfoBack ǥ�� ����
	__geSetSprSurface(&g_sfInfoBack, RGB(255,255,255));
	g_sfInfoBack.hBmp = __geBmpFile2BmpHandle("./Image/UI/InfoBack.bmp");
	__geLoadSurface(hdc, &g_sfInfoBack);

	// HopeInfoBack ǥ�� ����
	fnSetHopeSurface(hdc);

	// DiseaseInfoBack ǥ�� ����
	fnSetDiseaseSurface(hdc);

	// Bigbang ǥ�� ����
	fnSetBigbangSurface(hdc);

	// WhitePlane ǥ�� ����
	__geSetImgSurface(&g_sfWhitePlane);
	g_sfWhitePlane.hBmp = __geBmpFile2BmpHandle("./Image/BG/WhitePlane.bmp");
	__geLoadSurface(hdc, &g_sfWhitePlane);

	// TextBigbang ǥ�� ����
	__geSetImgSurface(&g_sfTextBigbang);
	g_sfTextBigbang.hBmp = __geBmpFile2BmpHandle("./Image/Text/Bigbang.bmp");
	__geLoadSurface(hdc, &g_sfTextBigbang);

	// broken ǥ�� ����
	__geSetImgSurface(&g_sfBroken);
	g_sfBroken.hBmp = __geBmpFile2BmpHandle("./Image/BG/Bigbang/8.bmp");
	__geLoadSurface(hdc, &g_sfBroken);

	// Textbroken ǥ�� ����
	__geSetSprSurface(&g_sfTextBroken, RGB(0,0,0));
	g_sfTextBroken.hBmp = __geBmpFile2BmpHandle("./Image/Text/Broken.bmp");
	__geLoadSurface(hdc, &g_sfTextBroken);

	// Complete ǥ�� ����
	__geSetSprSurface(&g_sfComplete, RGB(0,0,0));
	g_sfComplete.hBmp = __geBmpFile2BmpHandle("./Image/BG/Complete/Complete.bmp");
	__geLoadSurface(hdc, &g_sfComplete);

	// EyeCatch ǥ�� ����
	fnSetEyeCatchSurface(hdc);
}

//// �ϰ� ǥ�� �ı�
void __aDestroyAll(){				// �޸� ���� ������ �׻� �ݴ��(���ñ���)
	
	fnReleaseEyeCatchSurface();
	__geReleaseSurface(&g_sfComplete);
	__geReleaseSurface(&g_sfTextBroken);
	__geReleaseSurface(&g_sfBroken);
	__geReleaseSurface(&g_sfTextBigbang);
	__geReleaseSurface(&g_sfWhitePlane);
	fnReleaseBigbangSurface();
	fnReleaseDiseaseSurface();
	fnReleaseHopeSurface();
	__geReleaseSurface(&g_sfInfoBack);
	__geReleaseSurface(&g_sfWarning);
	fnReleaseStageMenuSurface();
	__geReleaseSurface(&g_sfPlayArrow);
	fnReleaseStageArrowSurface();
	fnReleaseSpaceSurface();
	__geReleaseSurface(&g_sfLack);
	__geReleaseSurface(&g_sfWhiteBack);
	fnReleaseObjectInfomationSurface();
	fnReleaseTipoSurface();
	fnReleaseInformationSurface();
	fnReleaseUISurface();
	fnReleaseStageSurface();
	fnReleaseGameBGSurface();
	fnReleaseHowToPlaySurface();
	__geReleaseSurface(&g_sfCredit);
	__geReleaseSurface(&g_sfPlane);
	__geReleaseSurface(&g_sfLogo);
	fnReleaseMenuSurface();
	__geReleaseSurface(&g_sfStarInfoBack);
	__geReleaseSurface(&g_sfStarInfo);
	__geReleaseSurface(&g_sfStarTotalObjectSprite);
	__geReleaseSurface(&g_sfStarTotalObjectRotate);
	__geReleaseSurface(&g_sfTotalObjectSprite);
	__geReleaseSurface(&g_sfTotalObjectRotate);
	__geReleaseSurface(&g_sfAdjustFocus);
	__geReleaseSurface(&g_sfAdjustBG);
	__geReleaseSurface(&g_sfBack);
}

// �ִϸ��̼� ���Ḯ��Ʈ �޸� ���� �Լ�
void __ReleaseSurfaceAll(SLMEMBER* pslm)
{
	//// �޸�DC ����Ʈ
	SURFACEINFO* psInfo;
	__slSetMember(pslm);
	__slSetHeadPosition();	// �ݵ�� ������������� ����!
	while(psInfo = (SURFACEINFO*) __slGetNext())
	{		
		__geReleaseSurface(psInfo);
		free(psInfo);		
		psInfo = 0;
	}
	__slRemoveAll();
	__slInitMember(pslm);
}

///ǥ�� ����
void RenewDC(int x, int y, SURFACEINFO* g_sfInfo){
	HDC hdc = GetDC(g_hWnd);						 

	// ��� �Ϸ� (�ĸ�ǥ��DC --����--> ����ǥ��DC/ȭ��DC)
	__geCompleteBlt(hdc, x, y, g_sfInfo);

	ReleaseDC(g_hWnd, hdc);
}

//�ϰ� �޴� ǥ�� ���� & �ʱ�ȭ
void fnSetMenuSurface(HDC hdc){
	for(int i = 0; i < MAX_MENU; i++) __geSetImgSurface(&g_sfMenu[i]);				// ǥ���ʱ�ȭ

	g_sfMenu[0].hBmp = __geBmpFile2BmpHandle("./Image/BG/Menu/MainBG2.bmp");		// Main �޴� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfMenu[0]);									// �ĸ� ǥ������ �ε�
	g_sfMenu[1].hBmp = __geBmpFile2BmpHandle("./Image/BG/Menu/MainBG2_Start.bmp");
	__geLoadSurface(hdc, &g_sfMenu[1]);
	g_sfMenu[2].hBmp = __geBmpFile2BmpHandle("./Image/BG/Menu/MainBG2_Credit.bmp");
	__geLoadSurface(hdc, &g_sfMenu[2]);
	g_sfMenu[3].hBmp = __geBmpFile2BmpHandle("./Image/BG/Menu/MainBG2_Exit.bmp");
	__geLoadSurface(hdc, &g_sfMenu[3]);
	g_sfMenu[4].hBmp = __geBmpFile2BmpHandle("./Image/BG/Menu/MainBG2_HowToPlay.bmp");
	__geLoadSurface(hdc, &g_sfMenu[4]);
}

//�ϰ� �޴� ǥ�� �ı�
void fnReleaseMenuSurface(){
	for(int i = 0; i < MAX_MENU; i++) __geReleaseSurface(&g_sfMenu[i]);
}

//�ϰ� �������� ǥ�� ���� & �ʱ�ȭ
void fnSetStageSurface(HDC hdc){
	for(int i = 0; i < MAX_STAGE_FRAME; i++) __geSetImgSurface(&g_sfStage[i]);				// ǥ���ʱ�ȭ

	g_sfStage[0].hBmp = __geBmpFile2BmpHandle("./Image/BG/BG_Animation/MainBG2_1-2.bmp");		// Main �޴� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfStage[0]);									// �ĸ� ǥ������ �ε�
	g_sfStage[1].hBmp = __geBmpFile2BmpHandle("./Image/BG/BG_Animation/MainBG2_1-2-3.bmp");
	__geLoadSurface(hdc, &g_sfStage[1]);
	g_sfStage[2].hBmp = __geBmpFile2BmpHandle("./Image/BG/BG_Animation/MainBG2_1-2.bmp");
	__geLoadSurface(hdc, &g_sfStage[2]);
	g_sfStage[3].hBmp = __geBmpFile2BmpHandle("./Image/BG/BG_Animation/MainBG2_1-2-4.bmp");
	__geLoadSurface(hdc, &g_sfStage[3]);
	g_sfStage[4].hBmp = __geBmpFile2BmpHandle("./Image/BG/BG_Animation/MainBG2_1-3.bmp");
	__geLoadSurface(hdc, &g_sfStage[4]);
	g_sfStage[5].hBmp = __geBmpFile2BmpHandle("./Image/BG/BG_Animation/MainBG2_1-3-4.bmp");
	__geLoadSurface(hdc, &g_sfStage[5]);
}

//�ϰ� �������� ǥ�� �ı�
void fnReleaseStageSurface(){
	for(int i = 0; i < MAX_STAGE_FRAME; i++) __geReleaseSurface(&g_sfStage[i]);
}

//�ϰ� �������� ȭ�� ǥ�� ���� & �ʱ�ȭ
void fnSetStageArrowSurface(HDC hdc){
	for(int i = 0; i < 9; i++) __geSetSprSurface(&g_sfStageArrow[i],RGB(0,0,0));				// ǥ���ʱ�ȭ

	g_sfStageArrow[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/StageArrow/Stage_Arrow.bmp");		// Main �޴� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfStageArrow[0]);									// �ĸ� ǥ������ �ε�
	g_sfStageArrow[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/StageArrow/Stage_Arrow_Left.bmp");
	__geLoadSurface(hdc, &g_sfStageArrow[1]);
	g_sfStageArrow[2].hBmp = __geBmpFile2BmpHandle("./Image/UI/StageArrow/Stage_Arrow_Right.bmp");
	__geLoadSurface(hdc, &g_sfStageArrow[2]);
	g_sfStageArrow[3].hBmp = __geBmpFile2BmpHandle("./Image/UI/StageArrow/Stage_Arrow_Up.bmp");
	__geLoadSurface(hdc, &g_sfStageArrow[3]);
	g_sfStageArrow[4].hBmp = __geBmpFile2BmpHandle("./Image/UI/StageArrow/Stage_Arrow_Down.bmp");
	__geLoadSurface(hdc, &g_sfStageArrow[4]);
	g_sfStageArrow[5].hBmp = __geBmpFile2BmpHandle("./Image/UI/StageArrow/Stage_Arrow_Left_DOWN.bmp");
	__geLoadSurface(hdc, &g_sfStageArrow[5]);
	g_sfStageArrow[6].hBmp = __geBmpFile2BmpHandle("./Image/UI/StageArrow/Stage_Arrow_Right_DOWN.bmp");
	__geLoadSurface(hdc, &g_sfStageArrow[6]);
	g_sfStageArrow[7].hBmp = __geBmpFile2BmpHandle("./Image/UI/StageArrow/Stage_Arrow_Left_UP.bmp");
	__geLoadSurface(hdc, &g_sfStageArrow[7]);
	g_sfStageArrow[8].hBmp = __geBmpFile2BmpHandle("./Image/UI/StageArrow/Stage_Arrow_Right_UP.bmp");
	__geLoadSurface(hdc, &g_sfStageArrow[8]);
}

//�ϰ� �������� ȭ�� ǥ�� �ı�
void fnReleaseStageArrowSurface(){
	for(int i = 0; i < 9; i++) __geReleaseSurface(&g_sfStageArrow[i]);
}

//�ϰ� ���� Information ǥ�� ���� & �ʱ�ȭ
void fnSetInformationSurface(HDC hdc){
	g_sfInformation.hBmp = __geBmpFile2BmpHandle("./Image/UI/New Interface/Info.bmp");	
	__geLoadSurface(hdc, &g_sfInformation);			
}

//�ϰ� ���� Information ǥ�� �ı�
void fnReleaseInformationSurface(){
	__geReleaseSurface(&g_sfInformation);
}

//�ϰ� ���� BackGround ǥ�� ���� & �ʱ�ȭ
void fnSetGameBGSurface(HDC hdc){
	for(int i = 0; i < MAX_STAR; i++) __geSetImgSurface(&g_sfGameBG[0]);		// ǥ���ʱ�ȭ

	g_sfGameBG[0].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Tutorial.bmp");	// ���� ��� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfGameBG[0]);										// �ĸ� ǥ������ �ε�
	g_sfGameBG[1].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Red_Star.bmp");	// ���� ��� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfGameBG[1]);										// �ĸ� ǥ������ �ε�	
	g_sfGameBG[2].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Yellow_Star.bmp");	// ���� ��� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfGameBG[2]);										// �ĸ� ǥ������ �ε�
	g_sfGameBG[3].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Blue_Star.bmp");// ���� ��� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfGameBG[3]);										// �ĸ� ǥ������ �ε�
	g_sfGameBG[4].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Pink_Star.bmp");// ���� ��� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfGameBG[4]);										// �ĸ� ǥ������ �ε�
	g_sfGameBG[5].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Brown_Star.bmp");// ���� ��� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfGameBG[5]);										// �ĸ� ǥ������ �ε�
}

//�ϰ� ���� BackGround ǥ�� �ı�
void fnReleaseGameBGSurface(){
	for(int i = 0; i < MAX_MENU; i++) __geReleaseSurface(&g_sfMenu[i]);
}

//�ϰ� �������� �޴� ǥ�� ���� & �ʱ�ȭ
void fnSetStageMenuSurface(HDC hdc){
	for(int i = 0; i < 4; i++) __geSetImgSurface(&g_sfStageMenu[i]);		// ǥ���ʱ�ȭ

	g_sfStageMenu[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/Menu/StageMenu.bmp");	
	__geLoadSurface(hdc, &g_sfStageMenu[0]);										// �ĸ� ǥ������ �ε�
	g_sfStageMenu[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/Menu/StageMenu1.bmp");
	__geLoadSurface(hdc, &g_sfStageMenu[1]);										// �ĸ� ǥ������ �ε�	
	g_sfStageMenu[2].hBmp = __geBmpFile2BmpHandle("./Image/UI/Menu/StageMenu2.bmp");	
	__geLoadSurface(hdc, &g_sfStageMenu[2]);										// �ĸ� ǥ������ �ε�
	g_sfStageMenu[3].hBmp = __geBmpFile2BmpHandle("./Image/UI/Menu/StageMenu3.bmp");	
	__geLoadSurface(hdc, &g_sfStageMenu[3]);										// �ĸ� ǥ������ �ε�
}

//�ϰ� �������� �޴� ǥ�� �ı�
void fnReleaseStageMenuSurface(){
	for(int i = 0; i < 4; i++) __geReleaseSurface(&g_sfMenu[i]);
}

//�ϰ� ���� HowToPlay ǥ�� ���� & �ʱ�ȭ
void fnSetHowToPlaySurface(HDC hdc){
	for(int i = 0; i < 3; i++) __geSetImgSurface(&g_sfHowToPlay[i]);				// ǥ���ʱ�ȭ

	g_sfHowToPlay[0].hBmp = __geBmpFile2BmpHandle("./Image/HowToPlay/HowToPlay1.bmp");
	__geLoadSurface(hdc, &g_sfHowToPlay[0]);										// �ĸ� ǥ������ �ε�
	g_sfHowToPlay[1].hBmp = __geBmpFile2BmpHandle("./Image/HowToPlay/HowToPlay2.bmp");
	__geLoadSurface(hdc, &g_sfHowToPlay[1]);										// �ĸ� ǥ������ �ε�	
	g_sfHowToPlay[2].hBmp = __geBmpFile2BmpHandle("./Image/HowToPlay/HowToPlay3.bmp");
	__geLoadSurface(hdc, &g_sfHowToPlay[2]);
}

//�ϰ� ���� HowToPlay ǥ�� �ı�
void fnReleaseHowToPlaySurface(){
	for(int i = 0; i < 3; i++) __geReleaseSurface(&g_sfHowToPlay[i]);
} 

//�ϰ� ���� Bigbang ǥ�� ���� & �ʱ�ȭ
void fnSetBigbangSurface(HDC hdc){
	for(int i = 0; i < 7; i++) __geSetImgSurface(&g_sfBigbang[i]);				// ǥ���ʱ�ȭ

	g_sfBigbang[0].hBmp = __geBmpFile2BmpHandle("./Image/BG/Bigbang/1.bmp");
	__geLoadSurface(hdc, &g_sfBigbang[0]);										
	g_sfBigbang[1].hBmp = __geBmpFile2BmpHandle("./Image/BG/Bigbang/2.bmp");
	__geLoadSurface(hdc, &g_sfBigbang[1]);
	g_sfBigbang[2].hBmp = __geBmpFile2BmpHandle("./Image/BG/Bigbang/3.bmp");
	__geLoadSurface(hdc, &g_sfBigbang[2]);	
	g_sfBigbang[3].hBmp = __geBmpFile2BmpHandle("./Image/BG/Bigbang/4.bmp");
	__geLoadSurface(hdc, &g_sfBigbang[3]);	
	g_sfBigbang[4].hBmp = __geBmpFile2BmpHandle("./Image/BG/Bigbang/5.bmp");
	__geLoadSurface(hdc, &g_sfBigbang[4]);	
	g_sfBigbang[5].hBmp = __geBmpFile2BmpHandle("./Image/BG/Bigbang/6.bmp");
	__geLoadSurface(hdc, &g_sfBigbang[5]);	
	g_sfBigbang[6].hBmp = __geBmpFile2BmpHandle("./Image/BG/Bigbang/7.bmp");
	__geLoadSurface(hdc, &g_sfBigbang[6]);	
}

//�ϰ� ���� Bigbang ǥ�� �ı�
void fnReleaseBigbangSurface(){
	for(int i = 0; i < 7; i++) __geReleaseSurface(&g_sfBigbang[i]);
} 

//�ϰ� ���� Space ǥ�� ���� & �ʱ�ȭ
void fnSetSpaceSurface(HDC hdc){
	for(int i = 0; i < 2; i++) __geSetImgSurface(&g_sfSpace[i]);				// ǥ���ʱ�ȭ

	g_sfSpace[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/Space bar/Space.bmp");
	__geLoadSurface(hdc, &g_sfSpace[0]);										// �ĸ� ǥ������ �ε�
	g_sfSpace[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/Space bar/Space1.bmp");
	__geLoadSurface(hdc, &g_sfSpace[1]);										// �ĸ� ǥ������ �ε�	
}

//�ϰ� ���� Space  ǥ�� �ı�
void fnReleaseSpaceSurface(){
	for(int i = 0; i < 2; i++) __geReleaseSurface(&g_sfSpace[i]);
} 

//�ϰ� ���� UI ǥ�� ���� & �ʱ�ȭ
void fnSetUISurface(HDC hdc){
	for(int i = 0; i < MAX_UI; i++) __geSetSprSurface(&g_sfUI[i], RGB(0, 0, 0));				// ǥ���ʱ�ȭ

	g_sfUI[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/UI_Base.bmp");			// ���� ��� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfUI[0]);											// �ĸ� ǥ������ �ε�	
	g_sfUI[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/UI_Object.bmp");			// ���� ��� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfUI[1]);											// �ĸ� ǥ������ �ε�	
	g_sfUI[2].hBmp = __geBmpFile2BmpHandle("./Image/UI/UI_Base.bmp");			// ���� ��� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfUI[2]);											// �ĸ� ǥ������ �ε�
	g_sfUI[3].hBmp = __geBmpFile2BmpHandle("./Image/UI/UI_Base.bmp");			// ���� ��� �̹��� �ε�
	__geLoadSurface(hdc, &g_sfUI[3]);											// �ĸ� ǥ������ �ε�
}

//�ϰ� ���� UI ǥ�� �ı�
void fnReleaseUISurface(){
	for(int i = 0; i < MAX_UI; i++) __geReleaseSurface(&g_sfUI[i]);
} 

//�ϰ� Ÿ���� �׷��� ��� ǥ�� ���� & �ʱ�ȭ
void fnSetTipoSurface(HDC hdc){
	for(int i = 0; i < MAX_OBJECT_KIND; i++) __geSetSprSurface(&g_sfTipo[i], RGB(0, 0, 0));				// ǥ���ʱ�ȭ

	g_sfTipo[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/TipoGraphic/Center.bmp");			
	__geLoadSurface(hdc, &g_sfTipo[0]);														
	g_sfTipo[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/TipoGraphic/UnderGround.bmp");			
	__geLoadSurface(hdc, &g_sfTipo[1]);														
	g_sfTipo[2].hBmp = __geBmpFile2BmpHandle("./Image/UI/TipoGraphic/Ground.bmp");			
	__geLoadSurface(hdc, &g_sfTipo[2]);														
	g_sfTipo[3].hBmp = __geBmpFile2BmpHandle("./Image/UI/TipoGraphic/Sky.bmp");			
	__geLoadSurface(hdc, &g_sfTipo[3]);														

}

//�ϰ� Ÿ���� �׷��� ǥ�� �ı�
void fnReleaseTipoSurface(){
	for(int i = 0; i < MAX_OBJECT_KIND; i++) __geReleaseSurface(&g_sfTipo[i]);
} 

//�ϰ� HopeInfoBack ��� ǥ�� ���� & �ʱ�ȭ
void fnSetHopeSurface(HDC hdc){
	for(int i = 0; i < MAX_STAR; i++) __geSetSprSurface(&g_sfHopeInfoBack[i], RGB(255, 255, 255));				// ǥ���ʱ�ȭ

	g_sfHopeInfoBack[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/HopeInfoBack/HopeInfoBack_Tutorial.bmp");			
	__geLoadSurface(hdc, &g_sfHopeInfoBack[0]);														
	g_sfHopeInfoBack[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/HopeInfoBack/HopeInfoBack_Red.bmp");			
	__geLoadSurface(hdc, &g_sfHopeInfoBack[1]);														
	g_sfHopeInfoBack[2].hBmp = __geBmpFile2BmpHandle("./Image/UI/HopeInfoBack/HopeInfoBack_Yellow.bmp");			
	__geLoadSurface(hdc, &g_sfHopeInfoBack[2]);														
	g_sfHopeInfoBack[3].hBmp = __geBmpFile2BmpHandle("./Image/UI/HopeInfoBack/HopeInfoBack_Blue.bmp");			
	__geLoadSurface(hdc, &g_sfHopeInfoBack[3]);	
	g_sfHopeInfoBack[4].hBmp = __geBmpFile2BmpHandle("./Image/UI/HopeInfoBack/HopeInfoBack_Pink.bmp");			
	__geLoadSurface(hdc, &g_sfHopeInfoBack[4]);	
	g_sfHopeInfoBack[5].hBmp = __geBmpFile2BmpHandle("./Image/UI/HopeInfoBack/HopeInfoBack_Brown.bmp");			
	__geLoadSurface(hdc, &g_sfHopeInfoBack[5]);	
}

//�ϰ� HopeInfoBack �׷��� ǥ�� �ı�
void fnReleaseHopeSurface(){
	for(int i = 0; i < MAX_STAR; i++) __geReleaseSurface(&g_sfHopeInfoBack[i]);
} 

//�ϰ� DiseaseInfoBack ��� ǥ�� ���� & �ʱ�ȭ
void fnSetDiseaseSurface(HDC hdc){
	for(int i = 0; i < MAX_STAR+1; i++) __geSetSprSurface(&g_sfDiseaseInfoBack[i], RGB(255, 255, 255));				// ǥ���ʱ�ȭ

	g_sfDiseaseInfoBack[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/DiseaseInfoBack/Cold.bmp");			
	__geLoadSurface(hdc, &g_sfDiseaseInfoBack[0]);														
	g_sfDiseaseInfoBack[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/DiseaseInfoBack/Old.bmp");			
	__geLoadSurface(hdc, &g_sfDiseaseInfoBack[1]);														
	g_sfDiseaseInfoBack[2].hBmp = __geBmpFile2BmpHandle("./Image/UI/DiseaseInfoBack/Dirty.bmp");			
	__geLoadSurface(hdc, &g_sfDiseaseInfoBack[2]);														
	g_sfDiseaseInfoBack[3].hBmp = __geBmpFile2BmpHandle("./Image/UI/DiseaseInfoBack/Hunger.bmp");			
	__geLoadSurface(hdc, &g_sfDiseaseInfoBack[3]);	
	g_sfDiseaseInfoBack[4].hBmp = __geBmpFile2BmpHandle("./Image/UI/DiseaseInfoBack/Melancholy.bmp");			
	__geLoadSurface(hdc, &g_sfDiseaseInfoBack[4]);	
	g_sfDiseaseInfoBack[5].hBmp = __geBmpFile2BmpHandle("./Image/UI/DiseaseInfoBack/Debility.bmp");			
	__geLoadSurface(hdc, &g_sfDiseaseInfoBack[5]);	
	g_sfDiseaseInfoBack[6].hBmp = __geBmpFile2BmpHandle("./Image/UI/DiseaseInfoBack/Healthy.bmp");			
	__geLoadSurface(hdc, &g_sfDiseaseInfoBack[6]);	
}

//�ϰ� DiseaseInfoBack �׷��� ǥ�� �ı�
void fnReleaseDiseaseSurface(){
	for(int i = 0; i < MAX_STAR + 1; i++) __geReleaseSurface(&g_sfDiseaseInfoBack[i]);
} 

//�ϰ� EyeCatch ��� ǥ�� ���� & �ʱ�ȭ
void fnSetEyeCatchSurface(HDC hdc){
	for(int i = 0; i < MAX_EYECATCH; i++) __geSetImgSurface(&g_sfEyeCatch[i]);				// ǥ���ʱ�ȭ

	g_sfEyeCatch[0].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/0.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[0]);														
	g_sfEyeCatch[1].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/1.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[1]);														
	g_sfEyeCatch[2].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/2.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[2]);														
	g_sfEyeCatch[3].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/3.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[3]);	
	g_sfEyeCatch[4].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/4.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[4]);	
	g_sfEyeCatch[5].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/5.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[5]);	
	g_sfEyeCatch[6].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/6.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[6]);
	g_sfEyeCatch[7].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/7.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[7]);														
	g_sfEyeCatch[8].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/8.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[8]);														
	g_sfEyeCatch[9].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/9.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[9]);														
	g_sfEyeCatch[10].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/10.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[10]);	
	g_sfEyeCatch[11].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/11.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[11]);	
	g_sfEyeCatch[12].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/12.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[12]);
	g_sfEyeCatch[13].hBmp = __geBmpFile2BmpHandle("./Image/BG/EyeCatch/13.bmp");			
	__geLoadSurface(hdc, &g_sfEyeCatch[13]);
}

//�ϰ� EyeCatch �׷��� ǥ�� �ı�
void fnReleaseEyeCatchSurface(){
	for(int i = 0; i < MAX_EYECATCH; i++) __geReleaseSurface(&g_sfEyeCatch[i]);
} 

//�ϰ� ������Ʈ ���� ��� ǥ�� ���� & �ʱ�ȭ
void fnSetObjectInformationSurface(HDC hdc){
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 20; j++)
			__geSetSprSurface(&g_sfObjectInformation[i][j], RGB(0, 0, 0));				// ǥ���ʱ�ȭ

	g_sfObjectInformation[0][0].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_0_tree_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[0][0]);	
	g_sfObjectInformation[0][1].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_1_Cloud_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[0][1]);	
	g_sfObjectInformation[0][2].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_0_water_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[0][2]);	
	g_sfObjectInformation[0][3].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_0_fire_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[0][3]);	
	g_sfObjectInformation[0][4].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_0_food_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[0][4]);	
	g_sfObjectInformation[0][5].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_1_wind_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[0][5]);	

	g_sfObjectInformation[1][0].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_1_tree_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[1][0]);	
	g_sfObjectInformation[1][1].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_2_Cloud_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[1][1]);	
	g_sfObjectInformation[1][2].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_1_water_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[1][2]);	
	g_sfObjectInformation[1][3].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_1_fire_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[1][3]);	
	g_sfObjectInformation[1][4].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_1_food_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[1][4]);

	g_sfObjectInformation[2][0].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_2_tree_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[2][0]);	
	//g_sfObjectInformation[2][1].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_2_Cloud_Information.bmp");			
	//__geLoadSurface(hdc, &g_sfObjectInformation[2][1]);	
	g_sfObjectInformation[2][2].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_3_water_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[2][2]);	
	g_sfObjectInformation[2][3].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_2_fire_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[2][3]);	
	g_sfObjectInformation[2][4].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_2_food_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[2][4]);

	//g_sfObjectInformation[3][0].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_1_tree_Information.bmp");			
	//__geLoadSurface(hdc, &g_sfObjectInformation[3][0]);	
	//g_sfObjectInformation[3][1].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_2_Cloud_Information.bmp");			
	//__geLoadSurface(hdc, &g_sfObjectInformation[3][1]);	
	g_sfObjectInformation[3][2].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_2_water_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[3][2]);	
	//g_sfObjectInformation[3][3].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_1_fire_Information.bmp");			
	//__geLoadSurface(hdc, &g_sfObjectInformation[3][3]);	
	g_sfObjectInformation[3][4].hBmp = __geBmpFile2BmpHandle("./image/UI/Object_3_food_Information.bmp");			
	__geLoadSurface(hdc, &g_sfObjectInformation[3][4]);

	g_sfObjectInformation[4][0].hBmp = __geBmpFile2BmpHandle("./Image/UI/Object_MudTemple_Information.bmp");		
	__geLoadSurface(hdc, &g_sfObjectInformation[4][0]);				
	g_sfObjectInformation[4][1].hBmp = __geBmpFile2BmpHandle("./Image/UI/Object_CheeseSlate_Information.bmp");		
	__geLoadSurface(hdc, &g_sfObjectInformation[4][1]);
	g_sfObjectInformation[4][2].hBmp = __geBmpFile2BmpHandle("./Image/UI/Object_MudTree_Information.bmp");		
	__geLoadSurface(hdc, &g_sfObjectInformation[4][2]);	
}

//�ϰ� ������Ʈ ���� ��� ǥ�� �ı�
void fnReleaseObjectInfomationSurface(){
	for(int i = 0; i < 5; i++) 
		for(int j = 0; j < 20; j++)
			__geReleaseSurface(&g_sfObjectInformation[i][j]);
} 