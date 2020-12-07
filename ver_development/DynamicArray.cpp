///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : DynamicArray.cpp
//
//   �� ��  �� �� : �����迭 ( POINT�� ��ҷ� �޴� �����迭 )
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DynamicArray.h"

POINTXY* __da_pArray=0;
int __da_nCount=0;
int __da_nIndex=0;

//// �ʱ�ȭ
void __daInit(int nCount)
{
	__da_pArray = (POINTXY*) malloc(nCount*sizeof(POINTXY));
	__da_nCount = nCount;	
	__da_nIndex = 0;
}

//// �ٸ� �Լ� ȣ������ �ݵ�� ȣ���Ѵ�, __daInit() ȣ������ ȣ���ؾ� �Ѵ�
void __daSetMember(DAMEMBER* pdaMember)
{
	__da_pArray = pdaMember->pArray;
	__da_nCount = pdaMember->nCount;
	__da_nIndex = pdaMember->nIndex;
}

//// �ٸ� �Լ� ȣ���Ŀ� �ݵ�� ȣ���Ѵ�
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
		// ������ ����̸� (-999,-999)
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
		// ������ ����̸� (-999,-999)
		POINTXY xyLast = { (float)DA_END, (float)DA_END };
		return xyLast;				
	}	
}

POINTXY* __daGetArray()
{
	return __da_pArray;
}