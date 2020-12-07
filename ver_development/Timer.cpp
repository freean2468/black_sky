///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : Timer.cpp
//
//   모 듈  이 름 : 타이머
//
//   제 작  목 적 : 시간의 경과를 측정할 때 사용한다.
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Timer.h"

// 타이머 초기화
// 처음 시간을 설정할 때 호출
void __tmInitTimer(TIMER* pTimer, DWORD dwDelayTime)
{
	pTimer->dwOldTime = GetTickCount();		// 현재 시간 대입
	pTimer->dwDelayTime = dwDelayTime;
}

// 시간의 경과
// 초기에 설정된 시간이 경과되었는지 질의하는 함수
BOOL __tmIsElapseTimer(TIMER* pTimer)
{	
	// GetTickCount함수 : OS가 시작된 후부터 현재까지의 시간을 밀리세컨드(1/1000초) 단위로 반환
	DWORD dwCurTime = GetTickCount();
	DWORD dwElapsed = dwCurTime - pTimer->dwOldTime;	

	if (dwElapsed >= pTimer->dwDelayTime)
	{		
		pTimer->dwDelayTime = 0;				// 0으로 초기화
		pTimer->dwOldTime = dwCurTime;			// 현재 시간으로 초기화
		return TRUE;							// 현재 시간이 경과되었음을 알림.
	}	

	return FALSE;
}

// 타이머가 아직 유효한지 질의하는 함수
// Timer를 지속적으로 검사해야 한다면 이 함수를 사용
BOOL __tmIsValidTimer(TIMER* pTimer)
{
	if (pTimer->dwDelayTime)
		return TRUE;
	else 
		return FALSE;
}
















