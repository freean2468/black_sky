/////////////////////////////////////////////////////////
///
/// surface.cpp : 표면 관리 cpp
///
/////////////////////////////////////////////////////////
#include "stdafx.h"

extern HWND g_hWnd;
extern float theta;

HDC ScrHDC;								// 스크린 HDC

SURFACEINFO g_sfBack;					// 후 면 표면
SURFACEINFO g_sfStageBack;				// 스테이지 후면 표면
SURFACEINFO g_sfAdjustBG;				// BG 최적화 출력을 위한 표면
SURFACEINFO g_sfAdjustFocus;			// 포커스오브젝트만을 위한 표면
SURFACEINFO g_sfPlane;					// 바 닥 표면
SURFACEINFO g_sfWhitePlane;				// 하얀 바닥 표면
SURFACEINFO g_sfMenu[MAX_MENU];			// 메 뉴 표면
SURFACEINFO g_sfLogo;					// 로 고 표면
SURFACEINFO g_sfCredit;					// 크레딧 표면
SURFACEINFO g_sfHowToPlay[3];			// How To Play 표면
SURFACEINFO g_sfGameBG[MAX_STAR];		// 게임 BackGround 표면
SURFACEINFO g_sfTotalObjectRotate;		// 모든 오브젝트가 회전 처리되어 담길 표면
SURFACEINFO g_sfTotalObjectSprite;		// 모든 오브젝트가 스프라이트 처리되어 담길 표면
SURFACEINFO g_sfStarTotalObjectRotate;	// StarInfo에서 모든 오브젝트가 회전 처리되어 담길 표면
SURFACEINFO g_sfStarTotalObjectSprite;	// StarInfo에서 모든 오브젝트가 스프라이트 처리되어 담길 표면
SURFACEINFO g_sfInformation;			// 별 Info 표면
SURFACEINFO g_sfStage[MAX_STAGE_FRAME];	// 스테이지 표면
SURFACEINFO g_sfStarInfo;				// 별 정보 표면
SURFACEINFO g_sfStarInfoBack;			// 별 정보 후면 표면
SURFACEINFO g_sfUI[MAX_UI];				// 유저 인터페이스 표면
SURFACEINFO g_sfTipo[MAX_OBJECT_KIND]; 	// 타이포그래픽 표면	
SURFACEINFO g_sfObjectInformation[5][20];	// 오브젝트 정보 표면
SURFACEINFO g_sfWhiteBack;				// 하얀색 표면
SURFACEINFO g_sfLack;					// 찰흙 부족 정보 표면
SURFACEINFO g_sfSpace[2];				// 스페이스 표면
SURFACEINFO g_sfStageArrow[9];			// 스테이지 화살 표면
SURFACEINFO g_sfPlayArrow;				// 플레이 화살 표면
SURFACEINFO g_sfStageMenu[4];			// 스테이지 메뉴 표면
SURFACEINFO g_sfWarning;				// 경고 표면
SURFACEINFO g_sfInfoBack;				// InfoBack 표면
SURFACEINFO g_sfHopeInfoBack[MAX_STAR];	// HopeInfoBack 표면
SURFACEINFO g_sfDiseaseInfoBack[MAX_STAR+1];	// DiseaseInfoBack 표면
SURFACEINFO g_sfBigbang[7];				// Bigbang 표면
SURFACEINFO g_sfTextBigbang;			// TextBigbang 표면
SURFACEINFO g_sfBroken;					// Broken 표면
SURFACEINFO g_sfTextBroken;				// TextBroken 표면
SURFACEINFO g_sfComplete;				// Complete 표면
SURFACEINFO g_sfEyeCatch[MAX_EYECATCH]; // EyeCatch 표면

//일괄 표면 생성
void __aInit(HDC hdc){
	__geCreateBackBuffer(hdc, 640, 480, 32, &g_sfBack, RGB(0, 0, 0));					// 후면버퍼 생성
	__geCreateBackBuffer(hdc, 1280, 960, 32, &g_sfStageBack, RGB(0, 0, 0));				// 스테이지 후면버퍼 생성
	// 이 백버퍼의 크기는 그래픽 성능과 좌우된다.
	__geCreateBackBuffer(hdc, 960, 960, 32, &g_sfAdjustBG, RGB(0, 0, 0));				// 배경 후면 버퍼 생성
	__geCreateBackBuffer(hdc, 640, 480, 32, &g_sfAdjustFocus, RGB(0, 0, 0));			// 포커스 오브젝트 후면 버퍼 생성
	__geCreateBackBuffer(hdc, 280, 210, 32, &g_sfStarInfo, RGB(255, 255, 255));			// 별 정보 표면 생성
	__geCreateBackBuffer(hdc, 640, 480, 32, &g_sfTotalObjectSprite, RGB(0,0,0));		// 오브젝트 스프라이트 표면 생성
	__geCreateBackBuffer(hdc, 640, 480, 32, &g_sfTotalObjectRotate, RGB(0,0,0));	// 오브젝트 회전 표면 생성

	// 이 아래 세 표면은 초기화 설정을 똑같이 해줘야 한다.
	// 바닥 표면의 사이즈도 이 아래 초기화 보다는 커야 한다.
	// 이 아래 백버퍼의 크기들은 게임 속도와 좌우된다.
	// 가로x세로 비율은 4*3
	__geCreateBackBuffer(hdc, 960, 720, 32, &g_sfStarTotalObjectRotate, RGB(0,0,0));	// 오브젝트 회전 표면 생성
	__geCreateBackBuffer(hdc, 960, 720, 32, &g_sfStarTotalObjectSprite, RGB(0,0,0));	// 오브젝트 스프라이트 표면 생성
	__geCreateBackBuffer(hdc, 960, 720, 32, &g_sfStarInfoBack, RGB(0, 0, 0));			// 별 정보 후면 표면 생성

	//메뉴 표면 생성 & 초기화
	fnSetMenuSurface(hdc);

	//로고 표면 생성
	__geSetImgSurface(&g_sfLogo);												// 표면 초기화
	g_sfLogo.hBmp = __geBmpFile2BmpHandle("./Image/Logo/Logo_Sider.bmp");		// 로고 이미지 로딩
	__geLoadSurface(hdc, &g_sfLogo);											// 후면 표면으로 로딩
	
	//바닥 표면 생성
	__geSetImgSurface(&g_sfPlane);
	g_sfPlane.hBmp = __geBmpFile2BmpHandle("./Image/BG/Plane.bmp");
	__geLoadSurface(hdc, &g_sfPlane);

	//크레딧 표면 생성
	__geSetImgSurface(&g_sfCredit);
	g_sfCredit.hBmp = __geBmpFile2BmpHandle("./Image/Credit/Credit.bmp");
	__geLoadSurface(hdc, &g_sfCredit);

	// How To Play 표면 생성
	fnSetHowToPlaySurface(hdc);

	//게임 BG 표면 생성
	fnSetGameBGSurface(hdc);

	// 스테이지 표면 생성
	fnSetStageSurface(hdc);

	// UI 표면 생성
	fnSetUISurface(hdc);

	// Information 표면 생성
	fnSetInformationSurface(hdc);

	// 타이포 표면 생성
	fnSetTipoSurface(hdc);

	/// 오브젝트 표면 생성
	fnSetObjectInformationSurface(hdc);

	// 하얀색 표면 생성
	__geSetSprSurface(&g_sfWhiteBack, RGB(3,4,3));
	g_sfWhiteBack.hBmp = __geBmpFile2BmpHandle("./Image/BG/White.bmp");
	__geLoadSurface(hdc, &g_sfWhiteBack);

	// 찰흙 부족 표면 생성
	__geSetSprSurface(&g_sfLack, RGB(0,0,0));
	g_sfLack.hBmp = __geBmpFile2BmpHandle("./Image/UI/Lack_Mud.bmp");
	__geLoadSurface(hdc, &g_sfLack);

	// 스페이스바 표면 생성
	fnSetSpaceSurface(hdc);

	// 스테이지 화살표 표면 생성
	fnSetStageArrowSurface(hdc);

	// 플레이 화살 표면 생성
	__geSetSprSurface(&g_sfPlayArrow, RGB(0,0,0));
	g_sfPlayArrow.hBmp = __geBmpFile2BmpHandle("./Image/UI/PlayArrow/Arrow.bmp");
	__geLoadSurface(hdc, &g_sfPlayArrow);

	// 스테이지 메뉴 표면 생성
	fnSetStageMenuSurface(hdc);

	// 경고 표면 생성
	__geSetSprSurface(&g_sfWarning, RGB(0,0,0));
	g_sfWarning.hBmp = __geBmpFile2BmpHandle("./Image/UI/Warning.bmp");
	__geLoadSurface(hdc, &g_sfWarning);

	// InfoBack 표면 생성
	__geSetSprSurface(&g_sfInfoBack, RGB(255,255,255));
	g_sfInfoBack.hBmp = __geBmpFile2BmpHandle("./Image/UI/InfoBack.bmp");
	__geLoadSurface(hdc, &g_sfInfoBack);

	// HopeInfoBack 표면 생성
	fnSetHopeSurface(hdc);

	// DiseaseInfoBack 표면 생성
	fnSetDiseaseSurface(hdc);

	// Bigbang 표면 생성
	fnSetBigbangSurface(hdc);

	// WhitePlane 표면 생성
	__geSetImgSurface(&g_sfWhitePlane);
	g_sfWhitePlane.hBmp = __geBmpFile2BmpHandle("./Image/BG/WhitePlane.bmp");
	__geLoadSurface(hdc, &g_sfWhitePlane);

	// TextBigbang 표면 생성
	__geSetImgSurface(&g_sfTextBigbang);
	g_sfTextBigbang.hBmp = __geBmpFile2BmpHandle("./Image/Text/Bigbang.bmp");
	__geLoadSurface(hdc, &g_sfTextBigbang);

	// broken 표면 생성
	__geSetImgSurface(&g_sfBroken);
	g_sfBroken.hBmp = __geBmpFile2BmpHandle("./Image/BG/Bigbang/8.bmp");
	__geLoadSurface(hdc, &g_sfBroken);

	// Textbroken 표면 생성
	__geSetSprSurface(&g_sfTextBroken, RGB(0,0,0));
	g_sfTextBroken.hBmp = __geBmpFile2BmpHandle("./Image/Text/Broken.bmp");
	__geLoadSurface(hdc, &g_sfTextBroken);

	// Complete 표면 생성
	__geSetSprSurface(&g_sfComplete, RGB(0,0,0));
	g_sfComplete.hBmp = __geBmpFile2BmpHandle("./Image/BG/Complete/Complete.bmp");
	__geLoadSurface(hdc, &g_sfComplete);

	// EyeCatch 표면 생성
	fnSetEyeCatchSurface(hdc);
}

//// 일괄 표면 파괴
void __aDestroyAll(){				// 메모리 해제 순서는 항상 반대로(스택구조)
	
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

// 애니메이션 연결리스트 메모리 해제 함수
void __ReleaseSurfaceAll(SLMEMBER* pslm)
{
	//// 메모리DC 리스트
	SURFACEINFO* psInfo;
	__slSetMember(pslm);
	__slSetHeadPosition();	// 반드시 헤드포지션으로 설정!
	while(psInfo = (SURFACEINFO*) __slGetNext())
	{		
		__geReleaseSurface(psInfo);
		free(psInfo);		
		psInfo = 0;
	}
	__slRemoveAll();
	__slInitMember(pslm);
}

///표면 갱신
void RenewDC(int x, int y, SURFACEINFO* g_sfInfo){
	HDC hdc = GetDC(g_hWnd);						 

	// 출력 완료 (후면표면DC --전송--> 전면표면DC/화면DC)
	__geCompleteBlt(hdc, x, y, g_sfInfo);

	ReleaseDC(g_hWnd, hdc);
}

//일괄 메뉴 표면 생성 & 초기화
void fnSetMenuSurface(HDC hdc){
	for(int i = 0; i < MAX_MENU; i++) __geSetImgSurface(&g_sfMenu[i]);				// 표면초기화

	g_sfMenu[0].hBmp = __geBmpFile2BmpHandle("./Image/BG/Menu/MainBG2.bmp");		// Main 메뉴 이미지 로딩
	__geLoadSurface(hdc, &g_sfMenu[0]);									// 후면 표면으로 로딩
	g_sfMenu[1].hBmp = __geBmpFile2BmpHandle("./Image/BG/Menu/MainBG2_Start.bmp");
	__geLoadSurface(hdc, &g_sfMenu[1]);
	g_sfMenu[2].hBmp = __geBmpFile2BmpHandle("./Image/BG/Menu/MainBG2_Credit.bmp");
	__geLoadSurface(hdc, &g_sfMenu[2]);
	g_sfMenu[3].hBmp = __geBmpFile2BmpHandle("./Image/BG/Menu/MainBG2_Exit.bmp");
	__geLoadSurface(hdc, &g_sfMenu[3]);
	g_sfMenu[4].hBmp = __geBmpFile2BmpHandle("./Image/BG/Menu/MainBG2_HowToPlay.bmp");
	__geLoadSurface(hdc, &g_sfMenu[4]);
}

//일괄 메뉴 표면 파괴
void fnReleaseMenuSurface(){
	for(int i = 0; i < MAX_MENU; i++) __geReleaseSurface(&g_sfMenu[i]);
}

//일괄 스테이지 표면 생성 & 초기화
void fnSetStageSurface(HDC hdc){
	for(int i = 0; i < MAX_STAGE_FRAME; i++) __geSetImgSurface(&g_sfStage[i]);				// 표면초기화

	g_sfStage[0].hBmp = __geBmpFile2BmpHandle("./Image/BG/BG_Animation/MainBG2_1-2.bmp");		// Main 메뉴 이미지 로딩
	__geLoadSurface(hdc, &g_sfStage[0]);									// 후면 표면으로 로딩
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

//일괄 스테이지 표면 파괴
void fnReleaseStageSurface(){
	for(int i = 0; i < MAX_STAGE_FRAME; i++) __geReleaseSurface(&g_sfStage[i]);
}

//일괄 스테이지 화살 표면 생성 & 초기화
void fnSetStageArrowSurface(HDC hdc){
	for(int i = 0; i < 9; i++) __geSetSprSurface(&g_sfStageArrow[i],RGB(0,0,0));				// 표면초기화

	g_sfStageArrow[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/StageArrow/Stage_Arrow.bmp");		// Main 메뉴 이미지 로딩
	__geLoadSurface(hdc, &g_sfStageArrow[0]);									// 후면 표면으로 로딩
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

//일괄 스테이지 화살 표면 파괴
void fnReleaseStageArrowSurface(){
	for(int i = 0; i < 9; i++) __geReleaseSurface(&g_sfStageArrow[i]);
}

//일괄 게임 Information 표면 생성 & 초기화
void fnSetInformationSurface(HDC hdc){
	g_sfInformation.hBmp = __geBmpFile2BmpHandle("./Image/UI/New Interface/Info.bmp");	
	__geLoadSurface(hdc, &g_sfInformation);			
}

//일괄 게임 Information 표면 파괴
void fnReleaseInformationSurface(){
	__geReleaseSurface(&g_sfInformation);
}

//일괄 게임 BackGround 표면 생성 & 초기화
void fnSetGameBGSurface(HDC hdc){
	for(int i = 0; i < MAX_STAR; i++) __geSetImgSurface(&g_sfGameBG[0]);		// 표면초기화

	g_sfGameBG[0].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Tutorial.bmp");	// 게임 배경 이미지 로딩
	__geLoadSurface(hdc, &g_sfGameBG[0]);										// 후면 표면으로 로딩
	g_sfGameBG[1].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Red_Star.bmp");	// 게임 배경 이미지 로딩
	__geLoadSurface(hdc, &g_sfGameBG[1]);										// 후면 표면으로 로딩	
	g_sfGameBG[2].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Yellow_Star.bmp");	// 게임 배경 이미지 로딩
	__geLoadSurface(hdc, &g_sfGameBG[2]);										// 후면 표면으로 로딩
	g_sfGameBG[3].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Blue_Star.bmp");// 게임 배경 이미지 로딩
	__geLoadSurface(hdc, &g_sfGameBG[3]);										// 후면 표면으로 로딩
	g_sfGameBG[4].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Pink_Star.bmp");// 게임 배경 이미지 로딩
	__geLoadSurface(hdc, &g_sfGameBG[4]);										// 후면 표면으로 로딩
	g_sfGameBG[5].hBmp = __geBmpFile2BmpHandle("./Image/BG/Star/Brown_Star.bmp");// 게임 배경 이미지 로딩
	__geLoadSurface(hdc, &g_sfGameBG[5]);										// 후면 표면으로 로딩
}

//일괄 게임 BackGround 표면 파괴
void fnReleaseGameBGSurface(){
	for(int i = 0; i < MAX_MENU; i++) __geReleaseSurface(&g_sfMenu[i]);
}

//일괄 스테이지 메뉴 표면 생성 & 초기화
void fnSetStageMenuSurface(HDC hdc){
	for(int i = 0; i < 4; i++) __geSetImgSurface(&g_sfStageMenu[i]);		// 표면초기화

	g_sfStageMenu[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/Menu/StageMenu.bmp");	
	__geLoadSurface(hdc, &g_sfStageMenu[0]);										// 후면 표면으로 로딩
	g_sfStageMenu[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/Menu/StageMenu1.bmp");
	__geLoadSurface(hdc, &g_sfStageMenu[1]);										// 후면 표면으로 로딩	
	g_sfStageMenu[2].hBmp = __geBmpFile2BmpHandle("./Image/UI/Menu/StageMenu2.bmp");	
	__geLoadSurface(hdc, &g_sfStageMenu[2]);										// 후면 표면으로 로딩
	g_sfStageMenu[3].hBmp = __geBmpFile2BmpHandle("./Image/UI/Menu/StageMenu3.bmp");	
	__geLoadSurface(hdc, &g_sfStageMenu[3]);										// 후면 표면으로 로딩
}

//일괄 스테이지 메뉴 표면 파괴
void fnReleaseStageMenuSurface(){
	for(int i = 0; i < 4; i++) __geReleaseSurface(&g_sfMenu[i]);
}

//일괄 게임 HowToPlay 표면 생성 & 초기화
void fnSetHowToPlaySurface(HDC hdc){
	for(int i = 0; i < 3; i++) __geSetImgSurface(&g_sfHowToPlay[i]);				// 표면초기화

	g_sfHowToPlay[0].hBmp = __geBmpFile2BmpHandle("./Image/HowToPlay/HowToPlay1.bmp");
	__geLoadSurface(hdc, &g_sfHowToPlay[0]);										// 후면 표면으로 로딩
	g_sfHowToPlay[1].hBmp = __geBmpFile2BmpHandle("./Image/HowToPlay/HowToPlay2.bmp");
	__geLoadSurface(hdc, &g_sfHowToPlay[1]);										// 후면 표면으로 로딩	
	g_sfHowToPlay[2].hBmp = __geBmpFile2BmpHandle("./Image/HowToPlay/HowToPlay3.bmp");
	__geLoadSurface(hdc, &g_sfHowToPlay[2]);
}

//일괄 게임 HowToPlay 표면 파괴
void fnReleaseHowToPlaySurface(){
	for(int i = 0; i < 3; i++) __geReleaseSurface(&g_sfHowToPlay[i]);
} 

//일괄 게임 Bigbang 표면 생성 & 초기화
void fnSetBigbangSurface(HDC hdc){
	for(int i = 0; i < 7; i++) __geSetImgSurface(&g_sfBigbang[i]);				// 표면초기화

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

//일괄 게임 Bigbang 표면 파괴
void fnReleaseBigbangSurface(){
	for(int i = 0; i < 7; i++) __geReleaseSurface(&g_sfBigbang[i]);
} 

//일괄 게임 Space 표면 생성 & 초기화
void fnSetSpaceSurface(HDC hdc){
	for(int i = 0; i < 2; i++) __geSetImgSurface(&g_sfSpace[i]);				// 표면초기화

	g_sfSpace[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/Space bar/Space.bmp");
	__geLoadSurface(hdc, &g_sfSpace[0]);										// 후면 표면으로 로딩
	g_sfSpace[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/Space bar/Space1.bmp");
	__geLoadSurface(hdc, &g_sfSpace[1]);										// 후면 표면으로 로딩	
}

//일괄 게임 Space  표면 파괴
void fnReleaseSpaceSurface(){
	for(int i = 0; i < 2; i++) __geReleaseSurface(&g_sfSpace[i]);
} 

//일괄 게임 UI 표면 생성 & 초기화
void fnSetUISurface(HDC hdc){
	for(int i = 0; i < MAX_UI; i++) __geSetSprSurface(&g_sfUI[i], RGB(0, 0, 0));				// 표면초기화

	g_sfUI[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/UI_Base.bmp");			// 게임 배경 이미지 로딩
	__geLoadSurface(hdc, &g_sfUI[0]);											// 후면 표면으로 로딩	
	g_sfUI[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/UI_Object.bmp");			// 게임 배경 이미지 로딩
	__geLoadSurface(hdc, &g_sfUI[1]);											// 후면 표면으로 로딩	
	g_sfUI[2].hBmp = __geBmpFile2BmpHandle("./Image/UI/UI_Base.bmp");			// 게임 배경 이미지 로딩
	__geLoadSurface(hdc, &g_sfUI[2]);											// 후면 표면으로 로딩
	g_sfUI[3].hBmp = __geBmpFile2BmpHandle("./Image/UI/UI_Base.bmp");			// 게임 배경 이미지 로딩
	__geLoadSurface(hdc, &g_sfUI[3]);											// 후면 표면으로 로딩
}

//일괄 게임 UI 표면 파괴
void fnReleaseUISurface(){
	for(int i = 0; i < MAX_UI; i++) __geReleaseSurface(&g_sfUI[i]);
} 

//일괄 타이포 그래픽 출력 표면 생성 & 초기화
void fnSetTipoSurface(HDC hdc){
	for(int i = 0; i < MAX_OBJECT_KIND; i++) __geSetSprSurface(&g_sfTipo[i], RGB(0, 0, 0));				// 표면초기화

	g_sfTipo[0].hBmp = __geBmpFile2BmpHandle("./Image/UI/TipoGraphic/Center.bmp");			
	__geLoadSurface(hdc, &g_sfTipo[0]);														
	g_sfTipo[1].hBmp = __geBmpFile2BmpHandle("./Image/UI/TipoGraphic/UnderGround.bmp");			
	__geLoadSurface(hdc, &g_sfTipo[1]);														
	g_sfTipo[2].hBmp = __geBmpFile2BmpHandle("./Image/UI/TipoGraphic/Ground.bmp");			
	__geLoadSurface(hdc, &g_sfTipo[2]);														
	g_sfTipo[3].hBmp = __geBmpFile2BmpHandle("./Image/UI/TipoGraphic/Sky.bmp");			
	__geLoadSurface(hdc, &g_sfTipo[3]);														

}

//일괄 타이포 그래픽 표면 파괴
void fnReleaseTipoSurface(){
	for(int i = 0; i < MAX_OBJECT_KIND; i++) __geReleaseSurface(&g_sfTipo[i]);
} 

//일괄 HopeInfoBack 출력 표면 생성 & 초기화
void fnSetHopeSurface(HDC hdc){
	for(int i = 0; i < MAX_STAR; i++) __geSetSprSurface(&g_sfHopeInfoBack[i], RGB(255, 255, 255));				// 표면초기화

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

//일괄 HopeInfoBack 그래픽 표면 파괴
void fnReleaseHopeSurface(){
	for(int i = 0; i < MAX_STAR; i++) __geReleaseSurface(&g_sfHopeInfoBack[i]);
} 

//일괄 DiseaseInfoBack 출력 표면 생성 & 초기화
void fnSetDiseaseSurface(HDC hdc){
	for(int i = 0; i < MAX_STAR+1; i++) __geSetSprSurface(&g_sfDiseaseInfoBack[i], RGB(255, 255, 255));				// 표면초기화

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

//일괄 DiseaseInfoBack 그래픽 표면 파괴
void fnReleaseDiseaseSurface(){
	for(int i = 0; i < MAX_STAR + 1; i++) __geReleaseSurface(&g_sfDiseaseInfoBack[i]);
} 

//일괄 EyeCatch 출력 표면 생성 & 초기화
void fnSetEyeCatchSurface(HDC hdc){
	for(int i = 0; i < MAX_EYECATCH; i++) __geSetImgSurface(&g_sfEyeCatch[i]);				// 표면초기화

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

//일괄 EyeCatch 그래픽 표면 파괴
void fnReleaseEyeCatchSurface(){
	for(int i = 0; i < MAX_EYECATCH; i++) __geReleaseSurface(&g_sfEyeCatch[i]);
} 

//일괄 오브젝트 정보 출력 표면 생성 & 초기화
void fnSetObjectInformationSurface(HDC hdc){
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 20; j++)
			__geSetSprSurface(&g_sfObjectInformation[i][j], RGB(0, 0, 0));				// 표면초기화

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

//일괄 오브젝트 정보 출력 표면 파괴
void fnReleaseObjectInfomationSurface(){
	for(int i = 0; i < 5; i++) 
		for(int j = 0; j < 20; j++)
			__geReleaseSurface(&g_sfObjectInformation[i][j]);
} 