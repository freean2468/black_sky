///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : SLinkedList.h
//
//   모 듈  이 름 : 단일연결리스트
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SLINKEDLIST_H__
#define __SLINKEDLIST_H__

//노드
typedef struct NODETag
{
	//pData는 BIN_SURFACEINFO가 될 것.
	void*			pData; // 어떤 형태의 데이터도 노드의 자료로 사용할 수 있다. 
	struct NODETag*	pNext; // 다음 노드 주소
} NODE;

// 전역 변수들이 여러 개 필요하여 만든 구조체
// 이 구조체의 데이터 멤버는 SLinkedList의 전역 벼눗와 동일
// 단지 구조체로 관리된다는 차이
typedef struct SLMEMBERtag
{
	NODE*	pHead;
	NODE*	pTail;
	NODE*	pPosition;
	int		nCount;
} SLMEMBER;

void  __slInit();
void  __slInitMember(SLMEMBER* pslMember);
void  __slSetMember(SLMEMBER* pslMember);
void  __slGetMember(SLMEMBER* pslMember);
int   __slGetCount();
BOOL  __slIsEmpty();
NODE* __slCreateNode(void* pData);
void  __slAddTail(NODE* pNode);

void  __slSetHeadPosition();
void* __slGetAt();
void* __slGetNext();
void* __slGetHead();
void* __slGetTail();

//// Node를 메모리 해제 (pData와 pData내에 또 다른 메모리 할당이 있다면 미리 Get함수로 제거해야한다)
void  __slRemoveHead();
void  __slRemoveTail();
void  __slRemoveAll();

#endif
