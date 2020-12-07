#ifndef __GObjObject_H__
#define __GObjObject_H__

#include "stdafx.h"

// 오브젝트의 속성 정보
// 속성 정보란, 게임 오브젝트가 가지는 여러 가지 수치들에 대한 정보
typedef struct GOBJOBJECT_INFOtag
{
	int			nIdx;			// 오브젝트 리스트에서 인덱스
	int			nX;				// 오브젝트의 출력 XY 좌표
	int			nY;
	int			Create;			// 생성된 각도
	float		theta;			// 현재 각도
	int			StarStep;		// 존재하는 곳
	int			nXr;			// 회전 후 x 좌표
	int			nYr;			// 회전 후 y 좌표
	int			MudStep;		// 소모 진흙 수
	int			nPositionStep;	// 맵에서 존재하는 자신의 구역
	int			Attribute;		// 오브젝트 속성

	TIMER		TimeMud;		// MudTree 오브젝트에서 사용 타이머

	// 오브젝트 애니메이션에 대한 구조체
	ANIINFO		aiBase;				
} GOBJOBJECT_INFO;

BOOL __GObjObjectInit(int nIdx, int Attribute, int MudStep, int nPositionStep, int nX, int nY, float theta, char* pAniFile, HDC hdc);			// 초기화
void __GObjObjectInit();															// 초기화
void __GObjObjectRelease();															// 소멸
BOOL __GObjObjectAction();															// 이동
BOOL __GObjObjectRender(HDC hdc);													// 출력
RECT __GObjObjectGetRect();															// 충돌영역

//게임 오브젝트 관리
void* __GObjObjectCreate(int nSize);												// 생성
void* __GObjObjectGetObjInfo();	
void  __GObjObjectSetMember(void* pObjInfo);
void  __GObjObjectGetMember(void* pObjInfo);

#endif