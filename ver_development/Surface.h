#pragma once

#include "SLinkedList.h"

void __aInit(HDC hdc);
void __aDestroyAll();
void  __ReleaseSurfaceAll(SLMEMBER* pslm);
void RenewDC(int x, int y, SURFACEINFO* g_sfInfo);
void fnReleaseMenuSurface();
void fnSetMenuSurface(HDC hdc);
void fnSetGameBGSurface(HDC hdc);
void fnReleaseGameBGSurface();
void fnSetStageSurface(HDC hdc);
void fnReleaseStageSurface();
void fnSetStarEffectSurface(HDC hdc);
void fnReleaseStarEffectSurface();
void fnSetUISurface(HDC hdc);
void fnReleaseUISurface();
void fnSetInformationSurface(HDC hdc);
void fnReleaseInformationSurface();
void fnSetTipoSurface(HDC hdc);
void fnReleaseTipoSurface();
void fnSetObjectInformationSurface(HDC hdc);
void fnReleaseObjectInfomationSurface();
void fnSetHowToPlaySurface(HDC hdc);
void fnReleaseHowToPlaySurface();
void fnSetSpaceSurface(HDC hdc);
void fnReleaseSpaceSurface();
void fnSetStageArrowSurface(HDC hdc);
void fnReleaseStageArrowSurface();
void fnSetStageMenuSurface(HDC hdc);
void fnReleaseStageMenuSurface();
void fnSetHopeSurface(HDC hdc);
void fnReleaseHopeSurface();
void fnSetDiseaseSurface(HDC hdc);
void fnReleaseDiseaseSurface();
void fnSetBigbangSurface(HDC hdc);
void fnReleaseBigbangSurface();
void fnSetEyeCatchSurface(HDC hdc);
void fnReleaseEyeCatchSurface();