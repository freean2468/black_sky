#include "stdafx.h"

extern STARSTEP StarStep;
extern POINTS g_ptMouseAbsolute;

GOBJFOCUS_INFO* __fo_pObjInfo[MAX_STAR];

// �Ű� ���� nIdx�� ���� ���� ������ �ʴ´�. nX, nY�� ���� ��ǥ�� �ȴ�. 
// �������� hdc�� �ĸ� ǥ�� DC�� �Ѿ�´�.
BOOL __GObjFocusInit(int nIdx, int Attribute, int nPositionStep, int nX, int nY, float theta, char* pAniFile, HDC hdc)
{
	__fo_pObjInfo[StarStep]->nIdx = nIdx;		// �Ҹ�� ����
	__fo_pObjInfo[StarStep]->nX = nX;
	__fo_pObjInfo[StarStep]->nY = nY;		
	__fo_pObjInfo[StarStep]->theta = 0;
	__fo_pObjInfo[StarStep]->StarStep = StarStep;
	__fo_pObjInfo[StarStep]->nPositionStep = nPositionStep;
	__fo_pObjInfo[StarStep]->Attribute = Attribute;

	//// ��������Ʈ ����Ʈ �ʱ�ȭ �� �ε�
	__slInitMember(&__fo_pObjInfo[StarStep]->aiBase.slAnimation);
	if (!__afLoad2MemDc(&__fo_pObjInfo[StarStep]->aiBase.slAnimation, pAniFile, hdc))
	{
		__GObjFocusRelease();
		return FALSE;
	}

	//// �ִϸ��̼� �ʱ�ȭ (���ʿ� �ݵ�� ȣ��)
	__anReset(&__fo_pObjInfo[StarStep]->aiBase.slAnimation); 	

	return TRUE;
}

// ��Ŀ�� ������Ʈ�� �Ҹ��� ��� ȣ���ϴ� �Լ�
// ��� ���� ����Ʈ �Ҹ�
void __GObjFocusRelease()
{	
	if(__GObjFocusGetObjInfo()){
		__ReleaseSurfaceAll(&__fo_pObjInfo[StarStep]->aiBase.slAnimation);	// ȣ�� ���� ����
		__goRemoveAt(__fo_pObjInfo[StarStep]->nIdx);	
		__fo_pObjInfo[StarStep] = NULL;
	}
}

// ��Ŀ�� ������Ʈ�� �̵��� ���� ó���� �ϴ� �Լ�
BOOL __GObjFocusAction()
{
	//// ��Ŀ�� ������Ʈ�� Ŀ��Ű �Է¿� ���� ���� ��ȯ �κ�
	// 0x80�� ��Ʈ ������ �ϴ� ������, �Էµ� ���� ASCII�� ��ϵǾ� �ִ� Ű���� Ȯ���ϱ� ���ؼ��̴�.(��Ȯ���� ����)
	if (GetKeyState('A')  & 0x80) __fo_pObjInfo[StarStep]->theta += 2;
	if (GetKeyState('D') & 0x80) __fo_pObjInfo[StarStep]->theta -= 2;
	if(g_ptMouseAbsolute.x >= 75 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 104 && g_ptMouseAbsolute.y <= 261)	
		__fo_pObjInfo[StarStep]->theta += 2;
	if(g_ptMouseAbsolute.x >= 518 && g_ptMouseAbsolute.y >= 240 && g_ptMouseAbsolute.x <= 558 && g_ptMouseAbsolute.y <= 261) 
		__fo_pObjInfo[StarStep]->theta -= 2;

	if(__fo_pObjInfo[StarStep]->theta < 0) __fo_pObjInfo[StarStep]->theta = 360 + __fo_pObjInfo[StarStep]->theta; 
    if(__fo_pObjInfo[StarStep]->theta > 360) __fo_pObjInfo[StarStep]->theta = __fo_pObjInfo[StarStep]->theta - 360;

	return TRUE;
}

RECT __GObjFocusGetRect()
{	
	//// �ִϸ��̼� �ǰ� �ִ� ��������Ʈ�� ��� �浹������ ���Ѵ�
	RECT rcCurSpr = __anGetCurSprRect(&__fo_pObjInfo[StarStep]->aiBase);
	// OffsetRect �Լ��� RECT�� ������ ������� ��� nX, nY��ŭ ���ϴµ�, ��� ��ǥ�� �簢 ������ ���ϱ� ���ؼ��̴�.
	OffsetRect(&rcCurSpr, __fo_pObjInfo[StarStep]->nXr, __fo_pObjInfo[StarStep]->nYr);	

	return rcCurSpr;
}

//// �����κ��� ���ݹ޾��� ��� ����ġ�� ���ҽ�Ų��.
//void __GObjFocusDamage(int nDecrement)	// ���ҽ�ų ����ġ
//{
//	if (__he_pObjInfo->bGod) return;
//
//	////
//	__he_pObjInfo->nLife -= nDecrement;
//
//	if (__he_pObjInfo->nLife > 0)
//	{
//		__anReset(&__he_pObjInfo->aiDamage.slAnimation);	// ��� �ִ� �ִϸ��̼��� ���� �ֱ� ���� ���� ����Ʈ�� �ʱ�ȭ
//		__he_pObjInfo->nState = he_DAMAGE;					// ���� ���¸� ����
//		__he_pObjInfo->bGod = TRUE;
//	}
//	else
//	{
//		__anReset(&__he_pObjInfo->aiDeath.slAnimation);
//		__he_pObjInfo->nState = MS_DEATH;
//		__he_pObjInfo->bGod = TRUE;
//	}
//	// ���߿� Render �Լ����� �ִϸ��̼��� ó��
//}

// ��Ŀ�� ������Ʈ�� ȭ�鿡 ��½�Ű�� �Լ�.
// ���� ���¿� ���� ANIINFO�� �ּҸ� ��� ���� �������� Animate�Լ��� �Ѱ� �ִϸ��̼ǽ�Ű�� ����� �Ѵ�.
BOOL __GObjFocusRender(HDC hdc)
{
	// ������Ʈ�� ������ ���� ���� ���õ� ���� ���� �� ���� �����Ǿ����� ������Ʈ�� �׸���.
	if(__fo_pObjInfo[StarStep]->StarStep == StarStep){
//		__anAnimateFocus(hdc, __fo_pObjInfo[StarStep]->nX, __fo_pObjInfo[StarStep]->nY, &__fo_pObjInfo[StarStep]->aiBase);
		__anAnimateFocus(hdc, __fo_pObjInfo[StarStep]);
	}

	return TRUE;
}

void __GObjFocusInit(){
	// �ʿ����. �Լ������� ȣȯ�� ���� ����� �� ���̴�.
}

////////////////////////////////////////////////////////////////////////////////////////
// ���ΰ� ������Ʈ�� �����ϱ� ���� �Լ�
////////////////////////////////////////////////////////////////////////////////////////

void* __GObjFocusCreate(int nSize) // nSize == sizeof(GOBJFOCUS_INFO)
{
	return malloc(nSize);			// �޸� ���� �Ҵ�
}

// �ܺο��� ��Ŀ�� ������Ʈ�� ���� ������ ��ų� ��Ŀ�� ������Ʈ�� ��ȿ���� �˻��� �� ȣ��
void* __GObjFocusGetObjInfo()
{
	return __fo_pObjInfo[StarStep];
}

//// ���ʿ� �ѹ� ȣ��� �Ǵ� ���� ������Ʈ �����ڿ��� ���� ������Ʈ�� �Լ��� �ϰ������� ȣ���� �� ���
void  __GObjFocusSetMember(void* pObjInfo)
{
	__fo_pObjInfo[StarStep] = (GOBJFOCUS_INFO*) pObjInfo;
}

//// GObject���� �ϰ�ȣ��� ȣ��ǳ� �ǹ� ���� (���ϰ�ü������)
void  __GObjFocusGetMember(void* pObjInfo)
{
	pObjInfo = (void*) __fo_pObjInfo;
}





















