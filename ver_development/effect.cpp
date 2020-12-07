/////////////////////////////////////////////////////////
///
/// effect.cpp : 각종 효과 관리 cpp
///
/////////////////////////////////////////////////////////
#include "stdafx.h"

extern SURFACEINFO g_sfBack;
extern SURFACEINFO g_sfStageBack;
extern SURFACEINFO g_sfPlane;
extern SURFACEINFO g_sfMenu[MAX_MENU];

extern int Scroll_X, Scroll_Y;

TIMER TimerInterval;
int nAlpha = 0;	// 알파블렌딩 수치

void FadeInNew(SURFACEINFO *sfTemp){
	__gePutImage(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfPlane);	// 화면을 깨끗이 하기 위해 검은색 바닥 깔기

	__tmInitTimer(&TimerInterval,3000);		// 객체 타이머 초기화 (3초)
	
	// 한번은 실행
	do{	
		if(__tmIsElapseTimer(&TimerInterval)) break; // 타이머 지연 시간이 다 되었다면 빠져나온다.
											 
		nAlpha++;		
		Sleep(50);

		__gePutImageBlend(g_sfBack.dcSurface, 0, 0, sfTemp, nAlpha);
		RenewDC(0, 0, &g_sfBack);
	}while(__tmIsValidTimer(&TimerInterval));	// 타이머가 지연 시간이 다 될 때까지 (이 do~while문 만으로는 이 루프가 끝나지 않는다)

	nAlpha = 0;	// 다시 0으로 초기화
}

void FadeIn(SURFACEINFO *sfTemp){
	__tmInitTimer(&TimerInterval,3000);		// 객체 타이머 초기화 (3초)
	
	__gePutImage(g_sfStageBack.dcSurface, 0, 0, 0, 0, &g_sfMenu[0]);
	// 한번은 실행
	do{	
		if(__tmIsElapseTimer(&TimerInterval)) break; // 타이머 지연 시간이 다 되었다면 빠져나온다.
											 
		nAlpha++;		
		Sleep(50);

		__gePutImageBlend(g_sfStageBack.dcSurface, 0, 0, sfTemp, nAlpha);
		RenewDC(Scroll_X, Scroll_Y, &g_sfStageBack);
	}while(__tmIsValidTimer(&TimerInterval));	// 타이머가 지연 시간이 다 될 때까지 (이 do~while문 만으로는 이 루프가 끝나지 않는다)

	nAlpha = 0;	// 다시 0으로 초기화
}