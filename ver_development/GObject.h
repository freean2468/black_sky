///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : GObject.h
//
//   모 듈  이 름 : 게임오브젝트(관리자)
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GOBJECT_H__
#define __GOBJECT_H__

#define GO_EMPTY	-1

//// 함수포인터
typedef void  (*RELEASE)(void);
typedef RECT  (*GETRECT)(void);				// 충돌 사각영역을 리턴한다
typedef void  (*DAMAGE)(int nDecrement);	// 데미지 처리

typedef void  (*INIT)(void);
typedef BOOL  (*ACTION)(void);				// 리턴값이 FALSE이면 객체가 소멸되었다는 의미
typedef BOOL  (*RENDER)(HDC hdc);

typedef void  (*SETMEMBER)(void*);
typedef void  (*GETMEMBER)(void*);

typedef struct GOBJECTtag
{
	int			nIdx;			// 게임 오브젝트 인덱스
	char*		pStr;			// 게임 오브젝트 ID 저장
	int			Attribute;		// 오브젝트의 속성
	int			StarStep;		// 생성된 곳
	int			MudStep;		// 필요한 진흙 수(레벨)
	int			nPositionStep;	// 맵에서 존재하는 자신의 구역

	void*		pObjInfo;		// 게임 오브젝트 속성 정보 구조체
								// (GOBJxxx_INFO)
	RELEASE		fnRelease;
	GETRECT		fnGetRect;
	ACTION		fnAction;
	RENDER		fnRender;
	INIT		fnInit;

	SETMEMBER	fnSetMember;
	GETMEMBER	fnGetMember;
} GOBJECT;

BOOL __goInit(int nMaxObject);
void __goRelease();
int  __goCreate(void* pObjInfo, char* pStr, int Attribute, int MudStep, int	nPositionStep, int StarStep, SETMEMBER fnSetMember, GETMEMBER fnGetMember,
			    RELEASE fnRelease, ACTION fnAction, GETRECT fnGetRect, RENDER fnRender, INIT fnInit);

GOBJECT* __goGetObject(int nIdx);
GOBJECT* __goGetObjectList();
void*	 __goGetObjInfo(int nIdx);
void	 __goRemoveAt(int nIdx);

int		 __goGetObjectMaxCount();
int		 __goGetObjectCount();

void __goObjInitAll();
void __goObjActionAll();
void __goObjRenderAll(HDC hdc);
void __goObjReleaseAll();
void __goObjReleaseFocus();
void __goObjReleaseMud();

static void __goCreateEmptyList(int nSize);
static void __goPush(int array, int nIdx);
static int  __goPop();


#endif