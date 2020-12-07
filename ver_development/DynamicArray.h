///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : DynamicArray.h
//
//   모 듈  이 름 : 동적배열 ( POINT를 요소로 받는 동적배열 )
//
//   제 작  목 적 : 교육용
//
//   제   작   일 : 2006년 3월 15일
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ___DYNAMICARRAY_H__
#define ___DYNAMICARRAY_H__

#include "TypeDef.h"	// POINTXY

#define DA_END -999

typedef struct DAMEMBERtag
{
	POINTXY*	pArray;		// 동적배열의 선두 주소
	int			nCount;		// 요소의 수
	int			nIndex;		// 인덱스
} DAMEMBER;

void __daInit(int nCount);
void __daSetMember(DAMEMBER* pdaMember);
void __daGetMember(DAMEMBER* pdaMember);
void __daRemoveAll();
int  __daGetCount();

void	 __daCopyArray(POINTXY* pArray, int nSize);
void     __daSetElement(int nIndex, POINTXY xyValue);
POINTXY  __daGetElement(int nIndex);
POINTXY  __daGetNext();
POINTXY* __daGetArray();

#endif