///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : DynamicArray.cpp
//
//   모 듈  이 름 : 동적배열 ( POINT를 요소로 받는 동적배열 )
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DynamicArray.h"

POINTXY* __da_pArray=0;
int __da_nCount=0;
int __da_nIndex=0;

//// 초기화
void __daInit(int nCount)
{
	__da_pArray = (POINTXY*) malloc(nCount*sizeof(POINTXY));
	__da_nCount = nCount;	
	__da_nIndex = 0;
}

//// 다른 함수 호출전에 반드시 호출한다, __daInit() 호출전에 호출해야 한다
void __daSetMember(DAMEMBER* pdaMember)
{
	__da_pArray = pdaMember->pArray;
	__da_nCount = pdaMember->nCount;
	__da_nIndex = pdaMember->nIndex;
}

//// 다른 함수 호출후에 반드시 호출한다
void __daGetMember(DAMEMBER* pdaMember)
{
	pdaMember->pArray = __da_pArray;
	pdaMember->nCount = __da_nCount;
	pdaMember->nIndex = __da_nIndex;
}

void __daRemoveAll()
{
	if (__da_pArray)
	{
		free(__da_pArray);
		__da_pArray=0;
		__da_nCount=0;
		__da_nIndex=0;
	}
}

int __daGetCount()
{
	return __da_nCount;
}

void __daCopyArray(POINTXY* pArray, int nCount)
{
	memcpy(__da_pArray, pArray, nCount*sizeof(POINTXY));
}

void __daSetElement(int nIndex, POINTXY xyValue)
{	
	if (nIndex+1 <= __da_nCount) *(__da_pArray+nIndex) = xyValue;
}

POINTXY __daGetElement(int nIndex)
{
	if (nIndex+1 <= __da_nCount)	
		return *(__da_pArray+nIndex);	
	else 
	{
		// 마지막 요소이면 (-999,-999)
		POINTXY xyLast = { (float)DA_END, (float)DA_END };
		return xyLast;				
	}
}

POINTXY __daGetNext()
{
	if (__da_nIndex+1 <= __da_nCount)	
		return *(__da_pArray+__da_nIndex++);
	else	
	{
		// 마지막 요소이면 (-999,-999)
		POINTXY xyLast = { (float)DA_END, (float)DA_END };
		return xyLast;				
	}	
}

POINTXY* __daGetArray()
{
	return __da_pArray;
}