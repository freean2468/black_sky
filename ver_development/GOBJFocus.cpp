#include "stdafx.h"

extern STARSTEP StarStep;
extern POINTS g_ptMouseAbsolute;

GOBJFOCUS_INFO* __fo_pObjInfo[MAX_STAR];

// 매개 변수 nIdx는 직접 값을 넣지는 않는다. nX, nY는 최초 좌표가 된다. 
// 마지막에 hdc는 후면 표면 DC가 넘어온다.
BOOL __GObjFocusInit(int nIdx, int Attribute, int nPositionStep, int nX, int nY, float theta, char* pAniFile, HDC hdc)
{
	__fo_pObjInfo[StarStep]->nIdx = nIdx;		// 소멸시 참조
	__fo_pObjInfo[StarStep]->nX = nX;
	__fo_pObjInfo[StarStep]->nY = nY;		
	__fo_pObjInfo[StarStep]->theta = 0;
	__fo_pObjInfo[StarStep]->StarStep = StarStep;
	__fo_pObjInfo[StarStep]->nPositionStep = nPositionStep;
	__fo_pObjInfo[StarStep]->Attribute = Attribute;

	//// 스프라이트 리스트 초기화 및 로드
	__slInitMember(&__fo_pObjInfo[StarStep]->aiBase.slAnimation);
	if (!__afLoad2MemDc(&__fo_pObjInfo[StarStep]->aiBase.slAnimation, pAniFile, hdc))
	{
		__GObjFocusRelease();
		return FALSE;
	}

	//// 애니메이션 초기화 (최초에 반드시 호출)
	__anReset(&__fo_pObjInfo[StarStep]->aiBase.slAnimation); 	

	return TRUE;
}

// 포커스 오브젝트가 소멸할 경우 호출하는 함수
// 모든 연결 리스트 소멸
void __GObjFocusRelease()
{	
	if(__GObjFocusGetObjInfo()){
		__ReleaseSurfaceAll(&__fo_pObjInfo[StarStep]->aiBase.slAnimation);	// 호출 순서 주의
		__goRemoveAt(__fo_pObjInfo[StarStep]->nIdx);	
		__fo_pObjInfo[StarStep] = NULL;
	}
}

// 포커스 오브젝트의 이동에 대한 처리를 하는 함수
BOOL __GObjFocusAction()
{
	//// 포커스 오브젝트를 커서키 입력에 의해 각도 변환 부분
	// 0x80의 비트 연산을 하는 이유는, 입력된 값이 ASCII에 등록되어 있는 키인지 확인하기 위해서이다.(정확하지 않음)
	if (GetKeyState('A')  & 0x80) __fo_pObjInfo[StarStep]->theta += 2;
	if (GetKeyState('D') & 0x80) __fo_pObjInfo[StarStep]->theta -= 2;
	if(g_ptMouseAbsolute.x >= 75 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 104 && g_ptMouseAbsolute.y <= 261)	
		__fo_pObjInfo[StarStep]->theta += 2;
	if(g_ptMouseAbsolute.x >= 518 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 558 && g_ptMouseAbsolute.y <= 261) 
		__fo_pObjInfo[StarStep]->theta -= 2;

	if(__fo_pObjInfo[StarStep]->theta < 0) __fo_pObjInfo[StarStep]->theta = 360 + __fo_pObjInfo[StarStep]->theta; 
    if(__fo_pObjInfo[StarStep]->theta > 360) __fo_pObjInfo[StarStep]->theta = __fo_pObjInfo[StarStep]->theta - 360;

	return TRUE;
}

RECT __GObjFocusGetRect()
{	
	//// 애니메이션 되고 있는 스프라이트를 얻어 충돌영역을 구한다
	RECT rcCurSpr = __anGetCurSprRect(&__fo_pObjInfo[StarStep]->aiBase);
	// OffsetRect 함수는 RECT의 데이터 멤버들을 모두 nX, nY만큼 더하는데, 출력 좌표를 사각 영역에 더하기 위해서이다.
	OffsetRect(&rcCurSpr, __fo_pObjInfo[StarStep]->nXr, __fo_pObjInfo[StarStep]->nYr);	

	return rcCurSpr;
}

//// 적으로부터 공격받았을 경우 생명치를 감소시킨다.
//void __GObjFocusDamage(int nDecrement)	// 감소시킬 생명치
//{
//	if (__he_pObjInfo->bGod) return;
//
//	////
//	__he_pObjInfo->nLife -= nDecrement;
//
//	if (__he_pObjInfo->nLife > 0)
//	{
//		__anReset(&__he_pObjInfo->aiDamage.slAnimation);	// 울고 있는 애니메이션을 보여 주기 위해 연결 리스트를 초기화
//		__he_pObjInfo->nState = he_DAMAGE;					// 현재 상태를 변경
//		__he_pObjInfo->bGod = TRUE;
//	}
//	else
//	{
//		__anReset(&__he_pObjInfo->aiDeath.slAnimation);
//		__he_pObjInfo->nState = MS_DEATH;
//		__he_pObjInfo->bGod = TRUE;
//	}
//	// 나중에 Render 함수에서 애니메이션을 처리
//}

// 포커스 오브젝트를 화면에 출력시키는 함수.
// 현재 상태에 따른 ANIINFO의 주소를 얻어 내고 마지막에 Animate함수에 넘겨 애니메이션시키는 기능을 한다.
BOOL __GObjFocusRender(HDC hdc)
{
	// 오브젝트가 생성된 곳이 현재 선택된 별일 때만 그 별에 생성되었었던 오브젝트를 그린다.
	if(__fo_pObjInfo[StarStep]->StarStep == StarStep){
//		__anAnimateFocus(hdc, __fo_pObjInfo[StarStep]->nX, __fo_pObjInfo[StarStep]->nY, &__fo_pObjInfo[StarStep]->aiBase);
		__anAnimateFocus(hdc, __fo_pObjInfo[StarStep]);
	}

	return TRUE;
}

void __GObjFocusInit(){
	// 필요없다. 함수포인터 호환을 위해 만들어 둔 것이다.
}

////////////////////////////////////////////////////////////////////////////////////////
// 주인공 오브젝트를 관리하기 위한 함수
////////////////////////////////////////////////////////////////////////////////////////

void* __GObjFocusCreate(int nSize) // nSize == sizeof(GOBJFOCUS_INFO)
{
	return malloc(nSize);			// 메모리 동적 할당
}

// 외부에서 포커스 오브젝트에 대한 정보를 얻거나 포커스 오브젝트가 유효한지 검사할 때 호출
void* __GObjFocusGetObjInfo()
{
	return __fo_pObjInfo[StarStep];
}

//// 최초에 한번 호출됨 또는 게임 오브젝트 관리자에서 게임 오브젝트의 함수를 일괄적으로 호출할 때 사용
void  __GObjFocusSetMember(void* pObjInfo)
{
	__fo_pObjInfo[StarStep] = (GOBJFOCUS_INFO*) pObjInfo;
}

//// GObject에서 일괄호출시 호출되나 의미 없음 (유일객체임으로)
void  __GObjFocusGetMember(void* pObjInfo)
{
	pObjInfo = (void*) __fo_pObjInfo;
}





















