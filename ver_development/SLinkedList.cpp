///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : SLinkedList.cpp
//
//   모 듈  이 름 : 단일연결리스트
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SLinkedList.h"

NODE* __sl_pHead, *__sl_pTail, *__sl_pPosition;	// __sl_pPositon은 현재 연결 리스트에서 선택된 노드의 주소를 가리킨다.
int __sl_nCount;	// 연결 리스트에 있는 노드의 총 개수를 저장

//// 프로그램 실행시 최초에 한번만 실행한다
// 전역 변수를 초기화시키는 함수
void __slInit()
{
	__sl_pHead = NULL;
	__sl_pTail = NULL;
	__sl_pPosition = __sl_pHead;
	
	__sl_nCount = 0;
}

//// 연결리스트 멤버 선언시 최초에 구조체 데이터 멤버를 초기화
//// __slRemoveAll() 호출 후 __slInitMember()를 호출한다
void __slInitMember(SLMEMBER* pslMember)
{
	pslMember->pHead = NULL;
	pslMember->pTail = NULL;
	pslMember->pPosition = pslMember->pHead;
	pslMember->nCount   = 0;
}

//// 다른 함수 호출전에 반드시 호출한다
// 이 함수를 호출함으로써 전역 변수들이 pslMember가 가리키는 구조체의 데이터 멤버를 가리키게 된다.
// 결과적으로 SLinkedList의 다른 함수들이 pslMember가 가리키는 구조체의 데이터 멤버를 이용하여 연결 리스트를 관리한다.
void __slSetMember(SLMEMBER* pslMember)
{	
	__sl_pHead     = pslMember->pHead;
	__sl_pTail     = pslMember->pTail;
	__sl_pPosition = pslMember->pPosition;
	__sl_nCount    = pslMember->nCount;	
}

//// 다른 함수 호출후에 반드시 호출한다
//// 단, __slInitMember()를 호출하였으면 호출생략
// 이 함수를 호출함으로 pslmember의 구조체 데이터 멤버는 전역 변수를 가리키게 된다.
// 즉, SLinkedList 함수에 의해 전역 변수에 변경이 이루어졌다면
// __slGetMember 함수를 호출
void __slGetMember(SLMEMBER* pslMember)
{
	pslMember->pHead    = __sl_pHead;    
	pslMember->pTail    = __sl_pTail;   
	pslMember->pPosition = __sl_pPosition;
	pslMember->nCount   = __sl_nCount;
}

//// 노드 순서를 얻어냄
int __slGetCount()
{
	return __sl_nCount;
}

//// 노드가 있는지 없는지 판별
BOOL __slIsEmpty()
{
	if (__slGetCount())
		return TRUE;
	else
		return FALSE;
}

//// 노드 생성 함수
NODE* __slCreateNode(void* pData)
{
	NODE* pNode = (NODE*) malloc(sizeof(NODE)); // 동적 메모리 할당
	pNode->pData = pData; // pNode->pData는 인자로 넘어오는 pData를 가리킨다. 이 때, pData는 이미 동적 할당되어 있어야 한다.
	pNode->pNext = NULL;  // pNode->pNext는 반드시 NULL이어야 한다. 노드의 생성과 초기화가 되었으면 pNode를 반환

	return pNode;
}

//// 헤드노드를 장착
// 현재 노드의 주소를 가지고 있는 __sl_pPosition이 __sl_pHead를 가리키게 한다.
// 애니메이션을 지속하거나 연결 리스트의 노드를 모두 삭제하고 싶을 때 항상 미리 호출
void __slSetHeadPosition()
{	
	__sl_pPosition = __sl_pHead;
}

//// 현재 노드의 주소를 가지고 있는 __sl_pPosition의 pData 주소를 반환
void* __slGetAt()
{
	if (__sl_pPosition)
		return __sl_pPosition->pData;
	else
		return NULL;
}

//// 현재 __sl_pPosition의 pData를 반환하고 __sl_pPosition이 다음 노드를 가리키게 한다.
void* __slGetNext()
{
	if (__sl_pPosition)
	{
		NODE* pTemp = __sl_pPosition;
		__sl_pPosition = __sl_pPosition->pNext;
		return pTemp->pData;
	}
	else return NULL;
}

//// 헤드 데이타 리턴
void* __slGetHead()
{
	if (__sl_pHead)
		return __sl_pHead->pData;
	else
		return NULL;
}

//// Tail Postion Node의 pData를 리턴
void* __slGetTail()
{
	if (__sl_pTail)
		return __sl_pTail->pData;
	else
		return NULL;
}

//// 노드 테일 추가
// 생성된 노드를 연결 리스트의 끝에 추가
void __slAddTail(NODE* pNode)
{	
	NODE* pLast;

	//경우의 수는 세가지로 나뉜다.
	if (__sl_pHead == NULL)		// __sl_pHead가 NULL일 때(Node가 없을때)
	{
		__sl_pHead = pNode; // 생성된 pNode를 가리킨다.
	}

	if (__sl_pTail)				// __sl_pTail이 노드를 가리킬 떄(Node가 있을때)
	{
		pLast = __sl_pTail;	// 
		pLast->pNext = pNode;
		__sl_pTail = pNode;
	}
	else						// __sl_pTail이 NULL 일 때(Node가 없을때)
	{
		__sl_pTail = pNode;
	}

	__sl_nCount++;
}

//// 노드 헤드 삭제
void __slRemoveHead()
{
	//// Node가 하나일때
	if (__sl_pHead && __sl_pHead == __sl_pTail) // __sl_pHead가 NULL이 아니고 __sl_pHead와 __sl_pTail이 같을 때
	{		
		free(__sl_pHead);
		__sl_pHead = __sl_pTail = NULL;
		__sl_nCount--;
	}
	//// Node가 두개 이상일때
	else if (__sl_pHead)
	{
		NODE* pSecond = __sl_pHead->pNext;
		free(__sl_pHead);
		__sl_pHead = pSecond;
		__sl_nCount--;
	}
}

//// 노드 테일 삭제
void __slRemoveTail()
{
	//// Node가 하나일때
	if (__sl_pHead && __sl_pHead == __sl_pTail)
	{
		free(__sl_pHead);
		__sl_pHead = __sl_pTail = NULL;
		__sl_nCount--;
	}
	//// Node가 두개 이상일때
	else if (__sl_pHead)
	{
		int i;
		NODE* pNode = __sl_pHead;
		NODE* pNext;

		// 단일 연결 리스트에서는 이전 노드를 알 수 없기 때문에 처음부터 검색을 해야 한다.
		// 마지막에서 두번째 Node를 찾아낸다
		for (i=1; i < __sl_nCount-1; i++)
		{
			pNext = pNode->pNext;
			pNode = pNext;
		}

		pNode->pNext = NULL;
		free(__sl_pTail);
		__sl_pTail = pNode;
		__sl_nCount--;
	}
}

//// __slRemoveAll() 호출 후 __slInitMember()를 호출한다
void __slRemoveAll()
{
	NODE* pNext;
	NODE* pNode = __sl_pHead;

	while(pNode)
	{
		pNext = pNode->pNext;
		free(pNode);
		pNode = pNext;
	};
	__sl_nCount = 0;
}
