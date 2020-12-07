///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : DynamicArray.h
//
//   �� ��  �� �� : �����迭 ( POINT�� ��ҷ� �޴� �����迭 )
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : 2006�� 3�� 15��
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ___DYNAMICARRAY_H__
#define ___DYNAMICARRAY_H__

#include "TypeDef.h"	// POINTXY

#define DA_END -999

typedef struct DAMEMBERtag
{
	POINTXY*	pArray;		// �����迭�� ���� �ּ�
	int			nCount;		// ����� ��
	int			nIndex;		// �ε���
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