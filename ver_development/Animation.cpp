///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : Animation.cpp
//
//   모 듈  이 름 : 애니메이션
//
//   제 작  목 적 : 애니메이션 연결 리스트를 이용해서 화면에 애니메이션을 연출할 때 사용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

extern SURFACEINFO g_sfTotalObjectRotate;
extern SURFACEINFO g_sfTotalObjectSprite;
extern SURFACEINFO g_sfStarTotalObjectRotate;
extern SURFACEINFO g_sfStarTotalObjectSprite;
extern SURFACEINFO g_sfAdjustFocus;

extern bool Info_On;

SURFACEINFO* __an_psInfo=0;	// 필요한 경우에 임시 변수로 사용
DWORD __an_dwThisTick=0;	// 필요한 경우에 임시 변수로 사용

// 애니메이션 연결 리스트를 헤드포지션으로 위치시킨다. 게임 오브젝트가 생성되거나 소멸할 때 초기화의 용도로 후출
void __anReset(SLMEMBER* pslmMember)
{	
	__slSetMember(pslmMember);
	__slSetHeadPosition();
	__slGetMember(pslmMember);
}

//// 현재 애니메이션 되고 있는 스프라이트의 사각영역 리턴
// 게임 오브젝트 간에 충돌 검사할 때 호출된다.
RECT __anGetCurSprRect(ANIINFO* pAi)
{
	RECT rcSpr =  {0,0,0,0};						
	__slSetMember(&(pAi->slAnimation));				
	// 애니메이션 연결 리스트의 요소를 얻는다. 이 때 애니메이션 되고 있다면 애니메이션되는 스프라이트를 담고 있는
	// SURFACEINFO 구조체를 얻을 수 있다.
	__slSetHeadPosition();							// 애니메이션이 되면서 충돌 체크가 제대로 되지 않기 때문에 처음 모습으로만 충돌 체크를 한다.
	__an_psInfo = (SURFACEINFO*)__slGetAt();
	// 노드가 초기화가 안되었거나 노드의 끝이면 리턴
	if (!__an_psInfo) return rcSpr;
	__slGetMember(&(pAi->slAnimation));										// 연결 리스트를 다 사용했으면 __slGetMember 함수 호출
	SetRect(&rcSpr, 0, 0, __an_psInfo->nWidth, __an_psInfo->nHeight);		// 스프라이트 크기로 사각영역 초기화
	// OffsetRect 함수는 rcSpr의 데이터 멤버들을 더하는 함수
	OffsetRect(&rcSpr, -__an_psInfo->nCentralX, -__an_psInfo->nCentralY);	// 중심 좌표를 적용해서 rcSpr을 재계산
	return rcSpr;	
}

//// 애니메이션 연결 리스트를 한 번만 애니메이션시킬 때 이 함수를 호출한다.
// Ex) 게임 오브젝트가 죽어서 소멸될때 등
// hdc는 출력할 dc이다. x,y는 출력 좌표이고 pAi는 애니메이션시킬 정보가 들어 있는 구조체 ANIINFO가 된다.
BOOL __anAnimateOnce(HDC hdc, int nX, int nY, ANIINFO* pAi)
{    
	__slSetMember(&(pAi->slAnimation));

	//// 현재 애니메이션되고 있는 연결리스트의 표면정보를 얻는다
	__an_psInfo = (SURFACEINFO*)__slGetAt();

	// 노드가 초기화가 안되었거나 노드의 끝이면 리턴
	if (!__an_psInfo) return FALSE;
	
	// GetTickCount함수로 1/1000초 단위로 현재 시간을 얻어 낸다.
	//// 스프라이트 프레임의 지연시간이 넘어갔는지?
	__an_dwThisTick = GetTickCount();	
	if (__an_dwThisTick - pAi->dwLastTick > __an_psInfo->dwDelayTime)	// 현재 시간에서 지난 시간을 빼서 지연 시간이 되었는지 검사
	{
		// 스프라이트 프레임 갱신
		__an_psInfo = (SURFACEINFO*)__slGetNext();			
		pAi->dwLastTick = __an_dwThisTick;								// 지난 시간에 현재 시간을 저장	
	}

	__slGetMember(&(pAi->slAnimation));	

	__gePutSprite(hdc, nX-__an_psInfo->nCentralX, nY-__an_psInfo->nCentralY, 0, 0, __an_psInfo);	// hdc에 중심 좌표를 계산해서 출력

	return TRUE;
}

//// 포커스 오브젝트 애니메이션 연결 리스트를 소멸하기 전까지 무한 반복시키는 함수
void __anAnimateFocus(HDC hdc, GOBJFOCUS_INFO* __pObjInfo)
{    
	__slSetMember(&(__pObjInfo->aiBase.slAnimation));

	//// 현재 표면정보를 얻는다
	__an_psInfo = (SURFACEINFO*)__slGetAt();

	// 노드가 초기화가 안되었거나 노드의 끝이면 리턴
	// 연결 리스트의 노드 끝에 도달하면 __slGetAt 함수는 NULL을 리턴하므로 이것에 착안하여 작성한 프로그램 코드
	// 즉, __an_psInfo가 NULL이면 연결 리스트의 포지션을 Head로 위치시키고 다시 __slGetAt함수로 SURFACEINFO를 구하는 것이다.
	// 결국 애니메이션이 반복된다.	
	if (!__an_psInfo) 
	{				
		__slSetHeadPosition();
		__an_psInfo = (SURFACEINFO*)__slGetAt();
		if (!__an_psInfo) return;
	}
	
	//// 스프라이트 프레임의 지연시간이 넘어갔는지?
	__an_dwThisTick = GetTickCount();	
	if (__an_dwThisTick - __pObjInfo->aiBase.dwLastTick > __an_psInfo->dwDelayTime)
	{
		// 스프라이트 프레임 갱신
		__an_psInfo = (SURFACEINFO*)__slGetNext();			
		__pObjInfo->aiBase.dwLastTick = __an_dwThisTick;			
	}

	__slGetMember(&(__pObjInfo->aiBase.slAnimation));	

	__gePutImageRotateObject(&g_sfTotalObjectRotate, __pObjInfo->nY, 0, &__pObjInfo->nXr, &__pObjInfo->nYr, __an_psInfo);	// 게임 오브젝트이미지 회전 출력
	__gePutSprite(g_sfTotalObjectSprite.dcSurface, 0, 0, 0, 0, &g_sfTotalObjectRotate);
	//// 블렌드 이미지 출력
	//__gePutImageBlend(g_sfAdjustFocus.dcSurface, nX-__an_psInfo->nCentralX, SCR_HEIGHT - (nY+__an_psInfo->nCentralY), __an_psInfo, 255);
}

//// 오브젝트 애니메이션 연결 리스트를 소멸하기 전까지 무한 반복시키는 함수
void __anAnimateObject(HDC hdc, GOBJOBJECT_INFO* __pObjInfo)
{    
	__slSetMember(&(__pObjInfo->aiBase.slAnimation));

	//// 현재 표면정보를 얻는다
	__an_psInfo = (SURFACEINFO*)__slGetAt();

	// 노드가 초기화가 안되었거나 노드의 끝이면 리턴
	// 연결 리스트의 노드 끝에 도달하면 __slGetAt 함수는 NULL을 리턴하므로 이것에 착안하여 작성한 프로그램 코드
	// 즉, __an_psInfo가 NULL이면 연결 리스트의 포지션을 Head로 위치시키고 다시 __slGetAt함수로 SURFACEINFO를 구하는 것이다.
	// 결국 애니메이션이 반복된다.	
	if (!__an_psInfo) 
	{				
		__slSetHeadPosition();
		__an_psInfo = (SURFACEINFO*)__slGetAt();
		if (!__an_psInfo) return;
	}
	
	//// 스프라이트 프레임의 지연시간이 넘어갔는지?
	__an_dwThisTick = GetTickCount();	
	if (__an_dwThisTick - __pObjInfo->aiBase.dwLastTick > __an_psInfo->dwDelayTime)
	{
		// 스프라이트 프레임 갱신
		__an_psInfo = (SURFACEINFO*)__slGetNext();			
		__pObjInfo->aiBase.dwLastTick = __an_dwThisTick;			
	}

	__slGetMember(&(__pObjInfo->aiBase.slAnimation));	

	if(!Info_On){
		__gePutImageRotateObject(&g_sfTotalObjectRotate, __pObjInfo->nY, __pObjInfo->theta, &__pObjInfo->nXr, &__pObjInfo->nYr, __an_psInfo);	// 게임 오브젝트이미지 회전 출력
		__gePutSprite(g_sfTotalObjectSprite.dcSurface, 0, 0, 0, 0, &g_sfTotalObjectRotate);
	}
	else{
		__gePutImageRotateObject(&g_sfStarTotalObjectRotate, __pObjInfo->nY, __pObjInfo->theta, &__pObjInfo->nXr, &__pObjInfo->nYr, __an_psInfo);	// 게임 오브젝트이미지 회전 출력
		__gePutSprite(g_sfStarTotalObjectSprite.dcSurface, 0, 0, 0, 0, &g_sfStarTotalObjectRotate);
	}
}
























