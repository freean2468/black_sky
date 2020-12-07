#include "stdafx.h"

extern STARSTEP StarStep;

extern GAMESTATE GameState;
extern STARINFO StarInfo[MAX_STAR];

extern int nObject_ID;

extern POINTS g_ptMouseAbsolute;

GOBJOBJECT_INFO* __ob_pObjInfo[MAX_STAR];

BOOL __GObjObjectInit(int nIdx,int Attribute,  int MudStep, int nPositionStep, int nX, int nY, float theta, char* pAniFile, HDC hdc)
{
	__ob_pObjInfo[StarStep]->nIdx = nIdx;		// 소멸시 참조
	__ob_pObjInfo[StarStep]->nX = nX;
	__ob_pObjInfo[StarStep]->nY = nY;		
	__ob_pObjInfo[StarStep]->Create = (int)(360 - theta);
	if(nObject_ID == ID_MUD || nObject_ID == ID_MUDTREE_MUD) __ob_pObjInfo[StarStep]->theta = theta;	// 조물딱 진흙일 경우 원래 각도에 생성
	else __ob_pObjInfo[StarStep]->theta = 0;
	__ob_pObjInfo[StarStep]->StarStep = StarStep;
	__ob_pObjInfo[StarStep]->MudStep = MudStep;
	__ob_pObjInfo[StarStep]->nPositionStep = nPositionStep;
	__ob_pObjInfo[StarStep]->Attribute = Attribute;

	if(Attribute == MUDTREE){
		__tmInitTimer(&__ob_pObjInfo[StarStep]->TimeMud,10000);
		StarInfo[StarStep].Energy -= 1;	// 찰흙을 만들 때마다 별의 에너지를 깎는다.
	}
	else __tmInitTimer(&__ob_pObjInfo[StarStep]->TimeMud, 0);	// 찰흙나무가 아니라면 0으로 초기화

	//// 스프라이트 리스트 초기화 및 로드
	__slInitMember(&__ob_pObjInfo[StarStep]->aiBase.slAnimation);
	if (!__afLoad2MemDc(&__ob_pObjInfo[StarStep]->aiBase.slAnimation, pAniFile, hdc))
	{
		__GObjObjectRelease();
		return FALSE;
	}

	//// 애니메이션 초기화 (최초에 반드시 호출)
	__anReset(&__ob_pObjInfo[StarStep]->aiBase.slAnimation); 	

	return TRUE;
}
// 오브젝트가 죽어서 소멸할 경우 호출하는 함수
// 모든 연결 리스트 소멸
void __GObjObjectRelease()
{
	__ReleaseSurfaceAll(&__ob_pObjInfo[StarStep]->aiBase.slAnimation);
}

// 창조물 오브젝트의 이동에 대한 처리를 하는 함수
BOOL __GObjObjectAction()
{
	//// 창조물 오브젝트를 커서키 입력에 의해 각도 변환 부분
	// 0x80의 비트 연산을 하는 이유는, 입력된 값이 ASCII에 등록되어 있는 키인지 확인하기 위해서이다.(정확하지 않음)
	if(__ob_pObjInfo[StarStep]->Attribute == WIND) __ob_pObjInfo[StarStep]->theta += 0.8f;

	if (GetKeyState('A') & 0x80) __ob_pObjInfo[StarStep]->theta += 2;
	if (GetKeyState('D') & 0x80) __ob_pObjInfo[StarStep]->theta -= 2;
	
	if(g_ptMouseAbsolute.x >= 75 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 114 && g_ptMouseAbsolute.y <= 266)
		__ob_pObjInfo[StarStep]->theta += 2;
	if(g_ptMouseAbsolute.x >= 528 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 570 && g_ptMouseAbsolute.y <= 266) 
		__ob_pObjInfo[StarStep]->theta -= 2;

	if(__ob_pObjInfo[StarStep]->theta < 0) __ob_pObjInfo[StarStep]->theta = 360 + __ob_pObjInfo[StarStep]->theta; 
    if(__ob_pObjInfo[StarStep]->theta > 360) __ob_pObjInfo[StarStep]->theta = __ob_pObjInfo[StarStep]->theta - 360;

	// 찰흙나무에 일정시간마다 찰흙이 맺히게 한다.
	if(__ob_pObjInfo[StarStep]->Attribute == MUDTREE){
		if(__tmIsElapseTimer(&__ob_pObjInfo[StarStep]->TimeMud)){
			int Old_Object_ID;

			__tmInitTimer(&__ob_pObjInfo[StarStep]->TimeMud, 30000);
			StarInfo[StarStep].Energy -= 1;

			// 올드 변수에 오브젝트 아이디를 담았다가 다시 반환한다.
			Old_Object_ID = nObject_ID;

			// 이 함수로 인해 현재 오브젝트 아이디가 달라지기 때문이다.
			__stDispatchObject(ID_MUDTREE_MUD, __ob_pObjInfo[StarStep]->theta);
			
			nObject_ID = Old_Object_ID;
		}
	}

	return TRUE;
}

RECT __GObjObjectGetRect()
{	
	//// 애니메이션 되고 있는 스프라이트를 얻어 충돌영역을 구한다
	RECT rcCurSpr = __anGetCurSprRect(&__ob_pObjInfo[StarStep]->aiBase);
	// OffsetRect 함수는 RECT의 데이터 멤버들을 모두 nX, nY만큼 더하는데, 출력 좌표를 사각 영역에 더하기 위해서이다.
	OffsetRect(&rcCurSpr, __ob_pObjInfo[StarStep]->nXr, __ob_pObjInfo[StarStep]->nYr);			

	return rcCurSpr;
}

//// 적으로부터 공격받았을 경우 생명치를 감소시킨다.
//void __GObjObjectDamage(int nDecrement)	// 감소시킬 생명치
//{
//	if (__ob_pObjInfo->bGod) return;
//
//	////
//	__ob_pObjInfo->nLife -= nDecrement;
//
//	if (__ob_pObjInfo->nLife > 0)
//	{
//		__anReset(&__ob_pObjInfo->aiDamage.slAnimation);	// 울고 있는 애니메이션을 보여 주기 위해 연결 리스트를 초기화
//		__ob_pObjInfo->nState = ob_DAMAGE;					// 현재 상태를 변경
//		__ob_pObjInfo->bGod = TRUE;
//	}
//	else
//	{
//		__anReset(&__ob_pObjInfo->aiDeath.slAnimation);
//		__ob_pObjInfo->nState = ob_DEATH;
//		__ob_pObjInfo->bGod = TRUE;
//	}
//	// 나중에 Render 함수에서 애니메이션을 처리
//}

// 주인공 오브젝트를 화면에 출력시키는 함수.
// 현재 상태에 따른 ANIINFO의 주소를 얻어 내고 마지막에 Animate함수에 넘겨 애니메이션시키는 기능을 한다.
BOOL __GObjObjectRender(HDC hdc)
{	
	//  별 내부 진행에서 화면 밖의 오브젝트들은 그리지 않는다.(속도 때문에)
	if(__ob_pObjInfo[StarStep]->theta > 110 && __ob_pObjInfo[StarStep]->theta < 250 && GameState == GAME_PLAY) return true;

	// 오브젝트가 생성된 곳이 현재 선택된 별일 때만 그 별에 생성되었었던 오브젝트를 그린다.
	if(__ob_pObjInfo[StarStep]->StarStep == StarStep)
		__anAnimateObject(hdc, __ob_pObjInfo[StarStep]);

	return TRUE;
}

// 오브젝트 초기화 함수
void __GObjObjectInit(){
	__ob_pObjInfo[StarStep]->theta = (float)__ob_pObjInfo[StarStep]->Create;
}

////////////////////////////////////////////////////////////////////////////////////////
// 오브젝트를 관리하기 위한 함수
////////////////////////////////////////////////////////////////////////////////////////

void* __GObjObjectCreate(int nSize) // nSize == sizeof(GOBJMYSHIP_INFO)
{
	return malloc(nSize);			// 메모리 동적 할당
}

// 외부에서 주인공 오브젝트에 대한 정보를 얻거나 주인공 오브젝트가 유효한지 검사할 때 호출
void* __GObjObjectGetObjInfo()
{
	return __ob_pObjInfo[StarStep];
}

//// 최초에 한번 호출됨 또는 게임 오브젝트 관리자에서 게임 오브젝트의 함수를 일괄적으로 호출할 때 사용
void  __GObjObjectSetMember(void* pObjInfo)
{
	__ob_pObjInfo[StarStep] = (GOBJOBJECT_INFO*) pObjInfo;
}

//// GObject에서 일괄호출시 호출되나 의미 없음 (유일객체임으로)
void  __GObjObjectGetMember(void* pObjInfo)
{
	pObjInfo = (void*) __ob_pObjInfo;
}
