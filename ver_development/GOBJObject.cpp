#include "stdafx.h"

extern STARSTEP StarStep;

extern GAMESTATE GameState;
extern STARINFO StarInfo[MAX_STAR];

extern int nObject_ID;

extern POINTS g_ptMouseAbsolute;

GOBJOBJECT_INFO* __ob_pObjInfo[MAX_STAR];

BOOL __GObjObjectInit(int nIdx,int Attribute,  int MudStep, int nPositionStep, int nX, int nY, float theta, char* pAniFile, HDC hdc)
{
	__ob_pObjInfo[StarStep]->nIdx = nIdx;		// �Ҹ�� ����
	__ob_pObjInfo[StarStep]->nX = nX;
	__ob_pObjInfo[StarStep]->nY = nY;		
	__ob_pObjInfo[StarStep]->Create = (int)(360 - theta);
	if(nObject_ID == ID_MUD || nObject_ID == ID_MUDTREE_MUD) __ob_pObjInfo[StarStep]->theta = theta;	// ������ ������ ��� ���� ������ ����
	else __ob_pObjInfo[StarStep]->theta = 0;
	__ob_pObjInfo[StarStep]->StarStep = StarStep;
	__ob_pObjInfo[StarStep]->MudStep = MudStep;
	__ob_pObjInfo[StarStep]->nPositionStep = nPositionStep;
	__ob_pObjInfo[StarStep]->Attribute = Attribute;

	if(Attribute == MUDTREE){
		__tmInitTimer(&__ob_pObjInfo[StarStep]->TimeMud,10000);
		StarInfo[StarStep].Energy -= 1;	// ������ ���� ������ ���� �������� ��´�.
	}
	else __tmInitTimer(&__ob_pObjInfo[StarStep]->TimeMud, 0);	// ���볪���� �ƴ϶�� 0���� �ʱ�ȭ

	//// ��������Ʈ ����Ʈ �ʱ�ȭ �� �ε�
	__slInitMember(&__ob_pObjInfo[StarStep]->aiBase.slAnimation);
	if (!__afLoad2MemDc(&__ob_pObjInfo[StarStep]->aiBase.slAnimation, pAniFile, hdc))
	{
		__GObjObjectRelease();
		return FALSE;
	}

	//// �ִϸ��̼� �ʱ�ȭ (���ʿ� �ݵ�� ȣ��)
	__anReset(&__ob_pObjInfo[StarStep]->aiBase.slAnimation); 	

	return TRUE;
}
// ������Ʈ�� �׾ �Ҹ��� ��� ȣ���ϴ� �Լ�
// ��� ���� ����Ʈ �Ҹ�
void __GObjObjectRelease()
{
	__ReleaseSurfaceAll(&__ob_pObjInfo[StarStep]->aiBase.slAnimation);
}

// â���� ������Ʈ�� �̵��� ���� ó���� �ϴ� �Լ�
BOOL __GObjObjectAction()
{
	//// â���� ������Ʈ�� Ŀ��Ű �Է¿� ���� ���� ��ȯ �κ�
	// 0x80�� ��Ʈ ������ �ϴ� ������, �Էµ� ���� ASCII�� ��ϵǾ� �ִ� Ű���� Ȯ���ϱ� ���ؼ��̴�.(��Ȯ���� ����)
	if(__ob_pObjInfo[StarStep]->Attribute == WIND) __ob_pObjInfo[StarStep]->theta += 0.8f;

	if (GetKeyState('A') & 0x80) __ob_pObjInfo[StarStep]->theta += 2;
	if (GetKeyState('D') & 0x80) __ob_pObjInfo[StarStep]->theta -= 2;
	
	if(g_ptMouseAbsolute.x >= 75 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 114 && g_ptMouseAbsolute.y <= 266)
		__ob_pObjInfo[StarStep]->theta += 2;
	if(g_ptMouseAbsolute.x >= 528 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 570 && g_ptMouseAbsolute.y <= 266) 
		__ob_pObjInfo[StarStep]->theta -= 2;

	if(__ob_pObjInfo[StarStep]->theta < 0) __ob_pObjInfo[StarStep]->theta = 360 + __ob_pObjInfo[StarStep]->theta; 
    if(__ob_pObjInfo[StarStep]->theta > 360) __ob_pObjInfo[StarStep]->theta = __ob_pObjInfo[StarStep]->theta - 360;

	// ���볪���� �����ð����� ������ ������ �Ѵ�.
	if(__ob_pObjInfo[StarStep]->Attribute == MUDTREE){
		if(__tmIsElapseTimer(&__ob_pObjInfo[StarStep]->TimeMud)){
			int Old_Object_ID;

			__tmInitTimer(&__ob_pObjInfo[StarStep]->TimeMud, 30000);
			StarInfo[StarStep].Energy -= 1;

			// �õ� ������ ������Ʈ ���̵� ��Ҵٰ� �ٽ� ��ȯ�Ѵ�.
			Old_Object_ID = nObject_ID;

			// �� �Լ��� ���� ���� ������Ʈ ���̵� �޶����� �����̴�.
			__stDispatchObject(ID_MUDTREE_MUD, __ob_pObjInfo[StarStep]->theta);
			
			nObject_ID = Old_Object_ID;
		}
	}

	return TRUE;
}

RECT __GObjObjectGetRect()
{	
	//// �ִϸ��̼� �ǰ� �ִ� ��������Ʈ�� ��� �浹������ ���Ѵ�
	RECT rcCurSpr = __anGetCurSprRect(&__ob_pObjInfo[StarStep]->aiBase);
	// OffsetRect �Լ��� RECT�� ������ ������� ��� nX, nY��ŭ ���ϴµ�, ��� ��ǥ�� �簢 ������ ���ϱ� ���ؼ��̴�.
	OffsetRect(&rcCurSpr, __ob_pObjInfo[StarStep]->nXr, __ob_pObjInfo[StarStep]->nYr);			

	return rcCurSpr;
}

//// �����κ��� ���ݹ޾��� ��� ����ġ�� ���ҽ�Ų��.
//void __GObjObjectDamage(int nDecrement)	// ���ҽ�ų ����ġ
//{
//	if (__ob_pObjInfo->bGod) return;
//
//	////
//	__ob_pObjInfo->nLife -= nDecrement;
//
//	if (__ob_pObjInfo->nLife > 0)
//	{
//		__anReset(&__ob_pObjInfo->aiDamage.slAnimation);	// ��� �ִ� �ִϸ��̼��� ���� �ֱ� ���� ���� ����Ʈ�� �ʱ�ȭ
//		__ob_pObjInfo->nState = ob_DAMAGE;					// ���� ���¸� ����
//		__ob_pObjInfo->bGod = TRUE;
//	}
//	else
//	{
//		__anReset(&__ob_pObjInfo->aiDeath.slAnimation);
//		__ob_pObjInfo->nState = ob_DEATH;
//		__ob_pObjInfo->bGod = TRUE;
//	}
//	// ���߿� Render �Լ����� �ִϸ��̼��� ó��
//}

// ���ΰ� ������Ʈ�� ȭ�鿡 ��½�Ű�� �Լ�.
// ���� ���¿� ���� ANIINFO�� �ּҸ� ��� ���� �������� Animate�Լ��� �Ѱ� �ִϸ��̼ǽ�Ű�� ����� �Ѵ�.
BOOL __GObjObjectRender(HDC hdc)
{	
	//  �� ���� ���࿡�� ȭ�� ���� ������Ʈ���� �׸��� �ʴ´�.(�ӵ� ������)
	if(__ob_pObjInfo[StarStep]->theta > 110 && __ob_pObjInfo[StarStep]->theta < 250 && GameState == GAME_PLAY) return true;

	// ������Ʈ�� ������ ���� ���� ���õ� ���� ���� �� ���� �����Ǿ����� ������Ʈ�� �׸���.
	if(__ob_pObjInfo[StarStep]->StarStep == StarStep)
		__anAnimateObject(hdc, __ob_pObjInfo[StarStep]);

	return TRUE;
}

// ������Ʈ �ʱ�ȭ �Լ�
void __GObjObjectInit(){
	__ob_pObjInfo[StarStep]->theta = (float)__ob_pObjInfo[StarStep]->Create;
}

////////////////////////////////////////////////////////////////////////////////////////
// ������Ʈ�� �����ϱ� ���� �Լ�
////////////////////////////////////////////////////////////////////////////////////////

void* __GObjObjectCreate(int nSize) // nSize == sizeof(GOBJMYSHIP_INFO)
{
	return malloc(nSize);			// �޸� ���� �Ҵ�
}

// �ܺο��� ���ΰ� ������Ʈ�� ���� ������ ��ų� ���ΰ� ������Ʈ�� ��ȿ���� �˻��� �� ȣ��
void* __GObjObjectGetObjInfo()
{
	return __ob_pObjInfo[StarStep];
}

//// ���ʿ� �ѹ� ȣ��� �Ǵ� ���� ������Ʈ �����ڿ��� ���� ������Ʈ�� �Լ��� �ϰ������� ȣ���� �� ���
void  __GObjObjectSetMember(void* pObjInfo)
{
	__ob_pObjInfo[StarStep] = (GOBJOBJECT_INFO*) pObjInfo;
}

//// GObject���� �ϰ�ȣ��� ȣ��ǳ� �ǹ� ���� (���ϰ�ü������)
void  __GObjObjectGetMember(void* pObjInfo)
{
	pObjInfo = (void*) __ob_pObjInfo;
}
