///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : Timer.h
//
//   모 듈  이 름 : 타이머
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//#ifndef __TIMER_H__
//#define __TIMER_H__
#pragma once

#include <windows.h>

typedef struct 
{
	DWORD dwOldTime;			// 지난 시간을 담고 있는 변수
	DWORD dwDelayTime;			// 측정할 시간을 담고 있는 변수
} TIMER;

void __tmInitTimer(TIMER* pTimer, DWORD dwDelayTime);
BOOL __tmIsElapseTimer(TIMER* pTimer);
BOOL __tmIsValidTimer(TIMER* pTimer);

//#endif







