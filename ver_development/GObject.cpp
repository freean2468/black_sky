///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : GObject.cpp
//
//   모 듈  이 름 : 게임오브젝트(관리자)
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

int __go_nCount=0;			// 생성할 게임 오브젝트의 최대 개수 저장
GOBJECT* __go_pObjectList;	// 게임오브젝트 리스트(게임 오브젝트를 동적 배열의 요소처럼 나열한 목록)
// 비어있는 인덱스를 저장(Queue방식)
// __go_slmEmpty는 게임 오브젝트 리스트에서 사용하지 않는 인덱스를 담고 있는 연결 리스트이다.
// 게임 오브젝트 리스트를 우리가 마음대로 사용할 수 있는 것이 아니라, 사용할 수 있는
// 게임 오브젝트 리스트의 인덱스를 여기에 넣어 두었다가 게임 오브젝트를 만들 떄에 여기에서
// 사용할 수 있는 인덱스를 얻어 그 곳에 게임 오브젝트를 넣어 두는 것이다.
SLMEMBER __go_slmEmpty;	   

extern STARINFO StarInfo[MAX_STAR];
extern STARSTEP StarStep;

//// nMaxObject 만큼 __go_slmEmpty, __go_pObjectList 메모리 할당
BOOL __goInit(int nMaxObject)
{
	__go_nCount = nMaxObject;

	//// 연결 리스트 엠프티 생성
	__goCreateEmptyList(nMaxObject);

	//// 게임 오브젝트 리스트 생성
	__go_pObjectList=(GOBJECT*)malloc(sizeof(GOBJECT)*__go_nCount);
	if (!__go_pObjectList) return FALSE;

	//// 게임 오브젝트 리스트 초기화
	for (int i=0; i < __go_nCount; i++)
		__go_pObjectList[i].nIdx = GO_EMPTY;

	return TRUE;
}

// 생성된 모든 게임 오브젝트를 해제하고 게임 오브젝트 리스트와 연결 리스트 엠프티를 해제한다.
void __goRelease()
{
	if (__go_pObjectList)
	{
		//// 각각의 생성된 모든 게임 오브젝트 해제
		__goObjReleaseAll();

		//// 게임 오브젝트 리스트 메모리 해제
		free(__go_pObjectList);
		__go_pObjectList = 0;

		//// 동적 할당된 index // Linked List, Dynamic Array
		int* pIdx;

		// 연결 리스트 엠프티 해제
		__slSetMember(&__go_slmEmpty);
		__slSetHeadPosition();	// 반드시 헤드포지션으로 설정!
		while(pIdx = (int*) __slGetNext())
		{		
			free(pIdx);
			pIdx = 0;
		}
		__slRemoveAll();
		__slInitMember(&__go_slmEmpty);
	}
}

// 게임 오브젝트를 생성하여 게임 오브젝트 리스트의 요소로 초기화하는 함수
int __goCreate(void* pObjInfo, char* pStr,  int Attribute, int MudStep, int nPositionStep, int StarStep, SETMEMBER fnSetMember, GETMEMBER fnGetMember,
			   RELEASE fnRelease, ACTION fnAction, GETRECT fnGetRect, RENDER fnRender, INIT fnInit)
{
	//// 비어있는 인덱스를 얻어 사용
	int nIdx = __goPop();
	if (nIdx == GO_EMPTY) return GO_EMPTY;		// 비어 있는 인덱스가 있다면 바로 반환
	
	__go_pObjectList[nIdx].nIdx		 = nIdx;
	__go_pObjectList[nIdx].pStr		 = pStr;
	__go_pObjectList[nIdx].Attribute = Attribute;
	__go_pObjectList[nIdx].MudStep	 = MudStep;
	__go_pObjectList[nIdx].StarStep	 = StarStep;
	__go_pObjectList[nIdx].nPositionStep = nPositionStep;

	__go_pObjectList[nIdx].pObjInfo  = pObjInfo;
	__go_pObjectList[nIdx].fnRelease = fnRelease;
	__go_pObjectList[nIdx].fnAction  = fnAction;
	__go_pObjectList[nIdx].fnRender  = fnRender;
	__go_pObjectList[nIdx].fnInit	 = fnInit;
	__go_pObjectList[nIdx].fnGetRect = fnGetRect;

	__go_pObjectList[nIdx].fnSetMember = fnSetMember;
	__go_pObjectList[nIdx].fnGetMember = fnGetMember;

	return nIdx;
}

//오브젝트 최종 개수
int __goGetObjectMaxCount()
{
	return __go_nCount;
}

//오브젝트 번호
// 생성된 게임 오브젝트의 개수를 얻어 내는 함수
int __goGetObjectCount()
{
	int nCount=0;

	if (__go_pObjectList)
	{
		__slSetMember(&__go_slmEmpty);
		nCount = __go_nCount-__slGetCount(); //최대 생성 오브젝트 갯수 - 비어 있는 인덱스의 연결리스트 엠프티 갯수
		__slGetMember(&__go_slmEmpty);	
	}

	return nCount;
}

//번호와 매치되는 오브젝트를 반환
GOBJECT* __goGetObject(int nIdx)
{
	return &__go_pObjectList[nIdx];
}

//오브젝트 전체의 리스트
GOBJECT* __goGetObjectList()
{
	return __go_pObjectList;
}

// 이 함수는 GObject 모듈 내부에서만 불리는 함수이기 때문에 static 함수로 정의한다.
static void __goCreateEmptyList(int nSize)
{
	//// 빈 오브젝트 인덱스를 갖는 연결리스트
	__slInitMember(&__go_slmEmpty);
	__slSetMember(&__go_slmEmpty);

	//
	int* pIdx;
	for (int i=0; i < nSize; i++) 
	{
		pIdx = (int*) malloc(sizeof(int));
		*pIdx = i;
		__slAddTail(__slCreateNode(pIdx));
	}	

	//
	__slGetMember(&__go_slmEmpty);	
}

// 게임 오브젝트 리스트의 인덱스를 다시 연결 리스트 엠프티에 넣는 기능
static void __goPush(int nIdx)
{
	__slSetMember(&__go_slmEmpty);
	int* pIdx = (int*) malloc(sizeof(int));
	*pIdx = nIdx;
	__slAddTail(__slCreateNode(pIdx));
	__slGetMember(&__go_slmEmpty);
}

// 항상 연결 리스트 엠프티의 Head 요소를 얻어 온다.
static int __goPop()
{
	int nIdx;
	__slSetMember(&__go_slmEmpty);
	int* pIdx = (int*)__slGetHead();
	if (!pIdx) 
	{
		return GO_EMPTY;
	}
	else
	{
		nIdx = *pIdx;
		free(pIdx);
		pIdx = 0;
		__slRemoveHead();
		__slGetMember(&__go_slmEmpty);
	}	
	
	return nIdx;
}

void* __goGetObjInfo(int nIdx)
{
	return __go_pObjectList[nIdx].pObjInfo;
}

// 게임 오브젝트 리스트에서 게임 오브젝트를 제거하는 함수
void __goRemoveAt(int nIdx)
{
	if (__go_pObjectList[nIdx].pObjInfo)
		free(__go_pObjectList[nIdx].pObjInfo);

	__go_pObjectList[nIdx].Attribute = 0;
	__go_pObjectList[nIdx].MudStep	 = 0;
	__go_pObjectList[nIdx].StarStep  = 0;
	__go_pObjectList[nIdx].nPositionStep = 0;
	__go_pObjectList[nIdx].pStr		 = 0;

	__go_pObjectList[nIdx].pObjInfo  = 0;
	__go_pObjectList[nIdx].nIdx      = GO_EMPTY;
	__go_pObjectList[nIdx].fnRelease = 0;
	__go_pObjectList[nIdx].fnAction  = 0;
	__go_pObjectList[nIdx].fnRender  = 0;
	__go_pObjectList[nIdx].fnInit	 = 0;
	__go_pObjectList[nIdx].Attribute = 0;
	__go_pObjectList[nIdx].fnGetRect = 0;

	__go_pObjectList[nIdx].fnSetMember = 0;
	__go_pObjectList[nIdx].fnGetMember = 0;

	__goPush(nIdx);
}

void __goObjActionAll()
{
	if (__go_pObjectList)
	{
		void* pObjInfo;
		BOOL  bIsExist;

		for (int i=0; i < __go_nCount; i++)
		{
			//오브젝트가 있다면 if문을 수행
			if (__go_pObjectList[i].nIdx != GO_EMPTY) 
			{
				if(__go_pObjectList[i].StarStep == StarStep){			// 현재 플레이하고있는 별에 생성된 오브젝트만 행동시킨다.
					pObjInfo = __go_pObjectList[i].pObjInfo;
					__go_pObjectList[i].fnSetMember(pObjInfo);
					// fnAction 함수는 반환값이 있는데, 소멸 - false
					bIsExist = __go_pObjectList[i].fnAction();
					if (bIsExist)	// 소멸이 안되었다면
						__go_pObjectList[i].fnGetMember(pObjInfo);
				}
			}
		}
	}
}

void __goObjRenderAll(HDC hdc)
{
	if (__go_pObjectList)
	{
		void* pObjInfo;
		BOOL  bIsExist;

		for (int i=0; i < __go_nCount; i++)
		{
			if (__go_pObjectList[i].nIdx != GO_EMPTY)
			{
				pObjInfo = __go_pObjectList[i].pObjInfo;
				__go_pObjectList[i].fnSetMember(pObjInfo);			
				bIsExist = __go_pObjectList[i].fnRender(hdc);
				if (bIsExist)	// 소멸이 안되었다면
					__go_pObjectList[i].fnGetMember(pObjInfo);
			}
		}
	}
}

// 생성된 모든 게임 오브젝트 해제
void __goObjReleaseAll()
{
	if (__go_pObjectList)
	{
		void* pObjInfo;	

		for (int i=0; i < __go_nCount; i++)
		{
			if (__go_pObjectList[i].nIdx != GO_EMPTY)	// 게임 오브젝트가 생성되었으면 해제
			{
				pObjInfo = __go_pObjectList[i].pObjInfo;
				__go_pObjectList[i].fnSetMember(pObjInfo);			
				__go_pObjectList[i].fnRelease();
				__goRemoveAt(i);				
			}
		}
	}
}

// 생성된 모든 게임 오브젝트 초기화
void __goObjInitAll(){
	if (__go_pObjectList)
	{
		void* pObjInfo;

		for (int i=0; i < __go_nCount; i++)
		{
			//오브젝트가 있다면 if문을 수행
			if (__go_pObjectList[i].nIdx != GO_EMPTY) 
			{
				pObjInfo = __go_pObjectList[i].pObjInfo;
				__go_pObjectList[i].fnSetMember(pObjInfo);
				__go_pObjectList[i].fnInit();
				__go_pObjectList[i].fnGetMember(pObjInfo);
			}
		}
	}
}