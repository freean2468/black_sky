///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : SLinkedList.cpp
//
//   �� ��  �� �� : ���Ͽ��Ḯ��Ʈ
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SLinkedList.h"

NODE* __sl_pHead, *__sl_pTail, *__sl_pPosition;	// __sl_pPositon�� ���� ���� ����Ʈ���� ���õ� ����� �ּҸ� ����Ų��.
int __sl_nCount;	// ���� ����Ʈ�� �ִ� ����� �� ������ ����

//// ���α׷� ����� ���ʿ� �ѹ��� �����Ѵ�
// ���� ������ �ʱ�ȭ��Ű�� �Լ�
void __slInit()
{
	__sl_pHead = NULL;
	__sl_pTail = NULL;
	__sl_pPosition = __sl_pHead;
	
	__sl_nCount = 0;
}

//// ���Ḯ��Ʈ ��� ����� ���ʿ� ����ü ������ ����� �ʱ�ȭ
//// __slRemoveAll() ȣ�� �� __slInitMember()�� ȣ���Ѵ�
void __slInitMember(SLMEMBER* pslMember)
{
	pslMember->pHead = NULL;
	pslMember->pTail = NULL;
	pslMember->pPosition = pslMember->pHead;
	pslMember->nCount   = 0;
}

//// �ٸ� �Լ� ȣ������ �ݵ�� ȣ���Ѵ�
// �� �Լ��� ȣ�������ν� ���� �������� pslMember�� ����Ű�� ����ü�� ������ ����� ����Ű�� �ȴ�.
// ��������� SLinkedList�� �ٸ� �Լ����� pslMember�� ����Ű�� ����ü�� ������ ����� �̿��Ͽ� ���� ����Ʈ�� �����Ѵ�.
void __slSetMember(SLMEMBER* pslMember)
{	
	__sl_pHead     = pslMember->pHead;
	__sl_pTail     = pslMember->pTail;
	__sl_pPosition = pslMember->pPosition;
	__sl_nCount    = pslMember->nCount;	
}

//// �ٸ� �Լ� ȣ���Ŀ� �ݵ�� ȣ���Ѵ�
//// ��, __slInitMember()�� ȣ���Ͽ����� ȣ�����
// �� �Լ��� ȣ�������� pslmember�� ����ü ������ ����� ���� ������ ����Ű�� �ȴ�.
// ��, SLinkedList �Լ��� ���� ���� ������ ������ �̷�����ٸ�
// __slGetMember �Լ��� ȣ��
void __slGetMember(SLMEMBER* pslMember)
{
	pslMember->pHead    = __sl_pHead;    
	pslMember->pTail    = __sl_pTail;   
	pslMember->pPosition = __sl_pPosition;
	pslMember->nCount   = __sl_nCount;
}

//// ��� ������ ��
int __slGetCount()
{
	return __sl_nCount;
}

//// ��尡 �ִ��� ������ �Ǻ�
BOOL __slIsEmpty()
{
	if (__slGetCount())
		return TRUE;
	else
		return FALSE;
}

//// ��� ���� �Լ�
NODE* __slCreateNode(void* pData)
{
	NODE* pNode = (NODE*) malloc(sizeof(NODE)); // ���� �޸� �Ҵ�
	pNode->pData = pData; // pNode->pData�� ���ڷ� �Ѿ���� pData�� ����Ų��. �� ��, pData�� �̹� ���� �Ҵ�Ǿ� �־�� �Ѵ�.
	pNode->pNext = NULL;  // pNode->pNext�� �ݵ�� NULL�̾�� �Ѵ�. ����� ������ �ʱ�ȭ�� �Ǿ����� pNode�� ��ȯ

	return pNode;
}

//// ����带 ����
// ���� ����� �ּҸ� ������ �ִ� __sl_pPosition�� __sl_pHead�� ����Ű�� �Ѵ�.
// �ִϸ��̼��� �����ϰų� ���� ����Ʈ�� ��带 ��� �����ϰ� ���� �� �׻� �̸� ȣ��
void __slSetHeadPosition()
{	
	__sl_pPosition = __sl_pHead;
}

//// ���� ����� �ּҸ� ������ �ִ� __sl_pPosition�� pData �ּҸ� ��ȯ
void* __slGetAt()
{
	if (__sl_pPosition)
		return __sl_pPosition->pData;
	else
		return NULL;
}

//// ���� __sl_pPosition�� pData�� ��ȯ�ϰ� __sl_pPosition�� ���� ��带 ����Ű�� �Ѵ�.
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

//// ��� ����Ÿ ����
void* __slGetHead()
{
	if (__sl_pHead)
		return __sl_pHead->pData;
	else
		return NULL;
}

//// Tail Postion Node�� pData�� ����
void* __slGetTail()
{
	if (__sl_pTail)
		return __sl_pTail->pData;
	else
		return NULL;
}

//// ��� ���� �߰�
// ������ ��带 ���� ����Ʈ�� ���� �߰�
void __slAddTail(NODE* pNode)
{	
	NODE* pLast;

	//����� ���� �������� ������.
	if (__sl_pHead == NULL)		// __sl_pHead�� NULL�� ��(Node�� ������)
	{
		__sl_pHead = pNode; // ������ pNode�� ����Ų��.
	}

	if (__sl_pTail)				// __sl_pTail�� ��带 ����ų ��(Node�� ������)
	{
		pLast = __sl_pTail;	// 
		pLast->pNext = pNode;
		__sl_pTail = pNode;
	}
	else						// __sl_pTail�� NULL �� ��(Node�� ������)
	{
		__sl_pTail = pNode;
	}

	__sl_nCount++;
}

//// ��� ��� ����
void __slRemoveHead()
{
	//// Node�� �ϳ��϶�
	if (__sl_pHead && __sl_pHead == __sl_pTail) // __sl_pHead�� NULL�� �ƴϰ� __sl_pHead�� __sl_pTail�� ���� ��
	{		
		free(__sl_pHead);
		__sl_pHead = __sl_pTail = NULL;
		__sl_nCount--;
	}
	//// Node�� �ΰ� �̻��϶�
	else if (__sl_pHead)
	{
		NODE* pSecond = __sl_pHead->pNext;
		free(__sl_pHead);
		__sl_pHead = pSecond;
		__sl_nCount--;
	}
}

//// ��� ���� ����
void __slRemoveTail()
{
	//// Node�� �ϳ��϶�
	if (__sl_pHead && __sl_pHead == __sl_pTail)
	{
		free(__sl_pHead);
		__sl_pHead = __sl_pTail = NULL;
		__sl_nCount--;
	}
	//// Node�� �ΰ� �̻��϶�
	else if (__sl_pHead)
	{
		int i;
		NODE* pNode = __sl_pHead;
		NODE* pNext;

		// ���� ���� ����Ʈ������ ���� ��带 �� �� ���� ������ ó������ �˻��� �ؾ� �Ѵ�.
		// ���������� �ι�° Node�� ã�Ƴ���
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

//// __slRemoveAll() ȣ�� �� __slInitMember()�� ȣ���Ѵ�
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
