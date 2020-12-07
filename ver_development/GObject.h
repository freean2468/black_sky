///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : GObject.h
//
//   �� ��  �� �� : ���ӿ�����Ʈ(������)
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GOBJECT_H__
#define __GOBJECT_H__

#define GO_EMPTY	-1

//// �Լ�������
typedef void  (*RELEASE)(void);
typedef RECT  (*GETRECT)(void);				// �浹 �簢������ �����Ѵ�
typedef void  (*DAMAGE)(int nDecrement);	// ������ ó��

typedef void  (*INIT)(void);
typedef BOOL  (*ACTION)(void);				// ���ϰ��� FALSE�̸� ��ü�� �Ҹ�Ǿ��ٴ� �ǹ�
typedef BOOL  (*RENDER)(HDC hdc);

typedef void  (*SETMEMBER)(void*);
typedef void  (*GETMEMBER)(void*);

typedef struct GOBJECTtag
{
	int			nIdx;			// ���� ������Ʈ �ε���
	char*		pStr;			// ���� ������Ʈ ID ����
	int			Attribute;		// ������Ʈ�� �Ӽ�
	int			StarStep;		// ������ ��
	int			MudStep;		// �ʿ��� ���� ��(����)
	int			nPositionStep;	// �ʿ��� �����ϴ� �ڽ��� ����

	void*		pObjInfo;		// ���� ������Ʈ �Ӽ� ���� ����ü
								// (GOBJxxx_INFO)
	RELEASE		fnRelease;
	GETRECT		fnGetRect;
	ACTION		fnAction;
	RENDER		fnRender;
	INIT		fnInit;

	SETMEMBER	fnSetMember;
	GETMEMBER	fnGetMember;
} GOBJECT;

BOOL __goInit(int nMaxObject);
void __goRelease();
int  __goCreate(void* pObjInfo, char* pStr, int Attribute, int MudStep, int	nPositionStep, int StarStep, SETMEMBER fnSetMember, GETMEMBER fnGetMember,
			    RELEASE fnRelease, ACTION fnAction, GETRECT fnGetRect, RENDER fnRender, INIT fnInit);

GOBJECT* __goGetObject(int nIdx);
GOBJECT* __goGetObjectList();
void*	 __goGetObjInfo(int nIdx);
void	 __goRemoveAt(int nIdx);

int		 __goGetObjectMaxCount();
int		 __goGetObjectCount();

void __goObjInitAll();
void __goObjActionAll();
void __goObjRenderAll(HDC hdc);
void __goObjReleaseAll();
void __goObjReleaseFocus();
void __goObjReleaseMud();

static void __goCreateEmptyList(int nSize);
static void __goPush(int array, int nIdx);
static int  __goPop();


#endif