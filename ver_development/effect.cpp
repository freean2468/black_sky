/////////////////////////////////////////////////////////
///
/// effect.cpp : ���� ȿ�� ���� cpp
///
/////////////////////////////////////////////////////////
#include "stdafx.h"

extern SURFACEINFO g_sfBack;
extern SURFACEINFO g_sfStageBack;
extern SURFACEINFO g_sfPlane;
extern SURFACEINFO g_sfMenu[MAX_MENU];

extern int Scroll_X, Scroll_Y;

TIMER TimerInterval;
int nAlpha = 0;	// ���ĺ��� ��ġ

void FadeInNew(SURFACEINFO *sfTemp){
	__gePutImage(g_sfBack.dcSurface, 0, 0, 0, 0, &g_sfPlane);	// ȭ���� ������ �ϱ� ���� ������ �ٴ� ���

	__tmInitTimer(&TimerInterval,3000);		// ��ü Ÿ�̸� �ʱ�ȭ (3��)
	
	// �ѹ��� ����
	do{	
		if(__tmIsElapseTimer(&TimerInterval)) break; // Ÿ�̸� ���� �ð��� �� �Ǿ��ٸ� �������´�.
											 
		nAlpha++;		
		Sleep(50);

		__gePutImageBlend(g_sfBack.dcSurface, 0, 0, sfTemp, nAlpha);
		RenewDC(0, 0, &g_sfBack);
	}while(__tmIsValidTimer(&TimerInterval));	// Ÿ�̸Ӱ� ���� �ð��� �� �� ������ (�� do~while�� �����δ� �� ������ ������ �ʴ´�)

	nAlpha = 0;	// �ٽ� 0���� �ʱ�ȭ
}

void FadeIn(SURFACEINFO *sfTemp){
	__tmInitTimer(&TimerInterval,3000);		// ��ü Ÿ�̸� �ʱ�ȭ (3��)
	
	__gePutImage(g_sfStageBack.dcSurface, 0, 0, 0, 0, &g_sfMenu[0]);
	// �ѹ��� ����
	do{	
		if(__tmIsElapseTimer(&TimerInterval)) break; // Ÿ�̸� ���� �ð��� �� �Ǿ��ٸ� �������´�.
											 
		nAlpha++;		
		Sleep(50);

		__gePutImageBlend(g_sfStageBack.dcSurface, 0, 0, sfTemp, nAlpha);
		RenewDC(Scroll_X, Scroll_Y, &g_sfStageBack);
	}while(__tmIsValidTimer(&TimerInterval));	// Ÿ�̸Ӱ� ���� �ð��� �� �� ������ (�� do~while�� �����δ� �� ������ ������ �ʴ´�)

	nAlpha = 0;	// �ٽ� 0���� �ʱ�ȭ
}