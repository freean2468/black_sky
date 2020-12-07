///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : GObject.cpp
//
//   �� ��  �� �� : ���ӿ�����Ʈ(������)
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

int __go_nCount=0;			// ������ ���� ������Ʈ�� �ִ� ���� ����
GOBJECT* __go_pObjectList;	// ���ӿ�����Ʈ ����Ʈ(���� ������Ʈ�� ���� �迭�� ���ó�� ������ ���)
// ����ִ� �ε����� ����(Queue���)
// __go_slmEmpty�� ���� ������Ʈ ����Ʈ���� ������� �ʴ� �ε����� ��� �ִ� ���� ����Ʈ�̴�.
// ���� ������Ʈ ����Ʈ�� �츮�� ������� ����� �� �ִ� ���� �ƴ϶�, ����� �� �ִ�
// ���� ������Ʈ ����Ʈ�� �ε����� ���⿡ �־� �ξ��ٰ� ���� ������Ʈ�� ���� ���� ���⿡��
// ����� �� �ִ� �ε����� ��� �� ���� ���� ������Ʈ�� �־� �δ� ���̴�.
SLMEMBER __go_slmEmpty;	   

extern STARINFO StarInfo[MAX_STAR];
extern STARSTEP StarStep;

//// nMaxObject ��ŭ __go_slmEmpty, __go_pObjectList �޸� �Ҵ�
BOOL __goInit(int nMaxObject)
{
	__go_nCount = nMaxObject;

	//// ���� ����Ʈ ����Ƽ ����
	__goCreateEmptyList(nMaxObject);

	//// ���� ������Ʈ ����Ʈ ����
	__go_pObjectList=(GOBJECT*)malloc(sizeof(GOBJECT)*__go_nCount);
	if (!__go_pObjectList) return FALSE;

	//// ���� ������Ʈ ����Ʈ �ʱ�ȭ
	for (int i=0; i < __go_nCount; i++)
		__go_pObjectList[i].nIdx = GO_EMPTY;

	return TRUE;
}

// ������ ��� ���� ������Ʈ�� �����ϰ� ���� ������Ʈ ����Ʈ�� ���� ����Ʈ ����Ƽ�� �����Ѵ�.
void __goRelease()
{
	if (__go_pObjectList)
	{
		//// ������ ������ ��� ���� ������Ʈ ����
		__goObjReleaseAll();

		//// ���� ������Ʈ ����Ʈ �޸� ����
		free(__go_pObjectList);
		__go_pObjectList = 0;

		//// ���� �Ҵ�� index // Linked List, Dynamic Array
		int* pIdx;

		// ���� ����Ʈ ����Ƽ ����
		__slSetMember(&__go_slmEmpty);
		__slSetHeadPosition();	// �ݵ�� ������������� ����!
		while(pIdx = (int*) __slGetNext())
		{		
			free(pIdx);
			pIdx = 0;
		}
		__slRemoveAll();
		__slInitMember(&__go_slmEmpty);
	}
}

// ���� ������Ʈ�� �����Ͽ� ���� ������Ʈ ����Ʈ�� ��ҷ� �ʱ�ȭ�ϴ� �Լ�
int __goCreate(void* pObjInfo, char* pStr,  int Attribute, int MudStep, int nPositionStep, int StarStep, SETMEMBER fnSetMember, GETMEMBER fnGetMember,
			   RELEASE fnRelease, ACTION fnAction, GETRECT fnGetRect, RENDER fnRender, INIT fnInit)
{
	//// ����ִ� �ε����� ��� ���
	int nIdx = __goPop();
	if (nIdx == GO_EMPTY) return GO_EMPTY;		// ��� �ִ� �ε����� �ִٸ� �ٷ� ��ȯ
	
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

//������Ʈ ���� ����
int __goGetObjectMaxCount()
{
	return __go_nCount;
}

//������Ʈ ��ȣ
// ������ ���� ������Ʈ�� ������ ��� ���� �Լ�
int __goGetObjectCount()
{
	int nCount=0;

	if (__go_pObjectList)
	{
		__slSetMember(&__go_slmEmpty);
		nCount = __go_nCount-__slGetCount(); //�ִ� ���� ������Ʈ ���� - ��� �ִ� �ε����� ���Ḯ��Ʈ ����Ƽ ����
		__slGetMember(&__go_slmEmpty);	
	}

	return nCount;
}

//��ȣ�� ��ġ�Ǵ� ������Ʈ�� ��ȯ
GOBJECT* __goGetObject(int nIdx)
{
	return &__go_pObjectList[nIdx];
}

//������Ʈ ��ü�� ����Ʈ
GOBJECT* __goGetObjectList()
{
	return __go_pObjectList;
}

// �� �Լ��� GObject ��� ���ο����� �Ҹ��� �Լ��̱� ������ static �Լ��� �����Ѵ�.
static void __goCreateEmptyList(int nSize)
{
	//// �� ������Ʈ �ε����� ���� ���Ḯ��Ʈ
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

// ���� ������Ʈ ����Ʈ�� �ε����� �ٽ� ���� ����Ʈ ����Ƽ�� �ִ� ���
static void __goPush(int nIdx)
{
	__slSetMember(&__go_slmEmpty);
	int* pIdx = (int*) malloc(sizeof(int));
	*pIdx = nIdx;
	__slAddTail(__slCreateNode(pIdx));
	__slGetMember(&__go_slmEmpty);
}

// �׻� ���� ����Ʈ ����Ƽ�� Head ��Ҹ� ��� �´�.
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

// ���� ������Ʈ ����Ʈ���� ���� ������Ʈ�� �����ϴ� �Լ�
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
			//������Ʈ�� �ִٸ� if���� ����
			if (__go_pObjectList[i].nIdx != GO_EMPTY) 
			{
				if(__go_pObjectList[i].StarStep == StarStep){			// ���� �÷����ϰ��ִ� ���� ������ ������Ʈ�� �ൿ��Ų��.
					pObjInfo = __go_pObjectList[i].pObjInfo;
					__go_pObjectList[i].fnSetMember(pObjInfo);
					// fnAction �Լ��� ��ȯ���� �ִµ�, �Ҹ� - false
					bIsExist = __go_pObjectList[i].fnAction();
					if (bIsExist)	// �Ҹ��� �ȵǾ��ٸ�
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
				if (bIsExist)	// �Ҹ��� �ȵǾ��ٸ�
					__go_pObjectList[i].fnGetMember(pObjInfo);
			}
		}
	}
}

// ������ ��� ���� ������Ʈ ����
void __goObjReleaseAll()
{
	if (__go_pObjectList)
	{
		void* pObjInfo;	

		for (int i=0; i < __go_nCount; i++)
		{
			if (__go_pObjectList[i].nIdx != GO_EMPTY)	// ���� ������Ʈ�� �����Ǿ����� ����
			{
				pObjInfo = __go_pObjectList[i].pObjInfo;
				__go_pObjectList[i].fnSetMember(pObjInfo);			
				__go_pObjectList[i].fnRelease();
				__goRemoveAt(i);				
			}
		}
	}
}

// ������ ��� ���� ������Ʈ �ʱ�ȭ
void __goObjInitAll(){
	if (__go_pObjectList)
	{
		void* pObjInfo;

		for (int i=0; i < __go_nCount; i++)
		{
			//������Ʈ�� �ִٸ� if���� ����
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