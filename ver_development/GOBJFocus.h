#ifndef __GObjFocus_H__
#define __GObjFocus_H__

#include "stdafx.h"

// 주인공 오브젝트의 속성 정보
// 속성 정보란, 게임 오브젝트가 가지는 여러 가지 수치들에 대한 정보
typedef struct __GObjFOCUS_INFOtag
{
	int			nIdx;			// 오브젝트 리스트에서 인덱스
	int			nX;				// 오브젝트의 출력 XY 좌표
	int			nY;
	int			nXr;
	int			nYr;
	float		theta;			// 현재 각도
	int			StarStep;		// 존재하는 곳
	int			nPositionStep;	// 오브젝트가 별에 존재하는 층

	int			Attribute;		// 오브젝트 속성
	
	// 오브젝트 애니메이션에 대한 구조체
	ANIINFO		aiBase;		
} GOBJFOCUS_INFO;

BOOL __GObjFocusInit(int nIdx, int Attribute, int nPositionStep,int nX, int nY, float theta, char* pAniFile, HDC hdc);		// 초기화
void __GObjFocusRelease();														// 소멸
BOOL __GObjFocusAction();														// 이동
BOOL __GObjFocusRender(HDC hdc);													// 출력
void __GObjFocusInit();
RECT __GObjFocusGetRect();															// 충돌영역

//게임 오브젝트 관리
void* __GObjFocusCreate(int nSize);												// 생성
void* __GObjFocusGetObjInfo();	
void  __GObjFocusSetMember(void* pObjInfo);
void  __GObjFocusGetMember(void* pObjInfo);

#endif