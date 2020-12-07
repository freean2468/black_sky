///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : Animation.cpp
//
//   �� ��  �� �� : �ִϸ��̼�
//
//   �� ��  �� �� : �ִϸ��̼� ���� ����Ʈ�� �̿��ؼ� ȭ�鿡 �ִϸ��̼��� ������ �� ���
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

extern SURFACEINFO g_sfTotalObjectRotate;
extern SURFACEINFO g_sfTotalObjectSprite;
extern SURFACEINFO g_sfStarTotalObjectRotate;
extern SURFACEINFO g_sfStarTotalObjectSprite;
extern SURFACEINFO g_sfAdjustFocus;

extern bool Info_On;

SURFACEINFO* __an_psInfo=0;	// �ʿ��� ��쿡 �ӽ� ������ ���
DWORD __an_dwThisTick=0;	// �ʿ��� ��쿡 �ӽ� ������ ���

// �ִϸ��̼� ���� ����Ʈ�� ������������� ��ġ��Ų��. ���� ������Ʈ�� �����ǰų� �Ҹ��� �� �ʱ�ȭ�� �뵵�� ����
void __anReset(SLMEMBER* pslmMember)
{	
	__slSetMember(pslmMember);
	__slSetHeadPosition();
	__slGetMember(pslmMember);
}

//// ���� �ִϸ��̼� �ǰ� �ִ� ��������Ʈ�� �簢���� ����
// ���� ������Ʈ ���� �浹 �˻��� �� ȣ��ȴ�.
RECT __anGetCurSprRect(ANIINFO* pAi)
{
	RECT rcSpr =  {0,0,0,0};						
	__slSetMember(&(pAi->slAnimation));				
	// �ִϸ��̼� ���� ����Ʈ�� ��Ҹ� ��´�. �� �� �ִϸ��̼� �ǰ� �ִٸ� �ִϸ��̼ǵǴ� ��������Ʈ�� ��� �ִ�
	// SURFACEINFO ����ü�� ���� �� �ִ�.
	__slSetHeadPosition();							// �ִϸ��̼��� �Ǹ鼭 �浹 üũ�� ����� ���� �ʱ� ������ ó�� ������θ� �浹 üũ�� �Ѵ�.
	__an_psInfo = (SURFACEINFO*)__slGetAt();
	// ��尡 �ʱ�ȭ�� �ȵǾ��ų� ����� ���̸� ����
	if (!__an_psInfo) return rcSpr;
	__slGetMember(&(pAi->slAnimation));										// ���� ����Ʈ�� �� ��������� __slGetMember �Լ� ȣ��
	SetRect(&rcSpr, 0, 0, __an_psInfo->nWidth, __an_psInfo->nHeight);		// ��������Ʈ ũ��� �簢���� �ʱ�ȭ
	// OffsetRect �Լ��� rcSpr�� ������ ������� ���ϴ� �Լ�
	OffsetRect(&rcSpr, -__an_psInfo->nCentralX, -__an_psInfo->nCentralY);	// �߽� ��ǥ�� �����ؼ� rcSpr�� ����
	return rcSpr;	
}

//// �ִϸ��̼� ���� ����Ʈ�� �� ���� �ִϸ��̼ǽ�ų �� �� �Լ��� ȣ���Ѵ�.
// Ex) ���� ������Ʈ�� �׾ �Ҹ�ɶ� ��
// hdc�� ����� dc�̴�. x,y�� ��� ��ǥ�̰� pAi�� �ִϸ��̼ǽ�ų ������ ��� �ִ� ����ü ANIINFO�� �ȴ�.
BOOL __anAnimateOnce(HDC hdc, int nX, int nY, ANIINFO* pAi)
{    
	__slSetMember(&(pAi->slAnimation));

	//// ���� �ִϸ��̼ǵǰ� �ִ� ���Ḯ��Ʈ�� ǥ�������� ��´�
	__an_psInfo = (SURFACEINFO*)__slGetAt();

	// ��尡 �ʱ�ȭ�� �ȵǾ��ų� ����� ���̸� ����
	if (!__an_psInfo) return FALSE;
	
	// GetTickCount�Լ��� 1/1000�� ������ ���� �ð��� ��� ����.
	//// ��������Ʈ �������� �����ð��� �Ѿ����?
	__an_dwThisTick = GetTickCount();	
	if (__an_dwThisTick - pAi->dwLastTick > __an_psInfo->dwDelayTime)	// ���� �ð����� ���� �ð��� ���� ���� �ð��� �Ǿ����� �˻�
	{
		// ��������Ʈ ������ ����
		__an_psInfo = (SURFACEINFO*)__slGetNext();			
		pAi->dwLastTick = __an_dwThisTick;								// ���� �ð��� ���� �ð��� ����	
	}

	__slGetMember(&(pAi->slAnimation));	

	__gePutSprite(hdc, nX-__an_psInfo->nCentralX, nY-__an_psInfo->nCentralY, 0, 0, __an_psInfo);	// hdc�� �߽� ��ǥ�� ����ؼ� ���

	return TRUE;
}

//// ��Ŀ�� ������Ʈ �ִϸ��̼� ���� ����Ʈ�� �Ҹ��ϱ� ������ ���� �ݺ���Ű�� �Լ�
void __anAnimateFocus(HDC hdc, GOBJFOCUS_INFO* __pObjInfo)
{    
	__slSetMember(&(__pObjInfo->aiBase.slAnimation));

	//// ���� ǥ�������� ��´�
	__an_psInfo = (SURFACEINFO*)__slGetAt();

	// ��尡 �ʱ�ȭ�� �ȵǾ��ų� ����� ���̸� ����
	// ���� ����Ʈ�� ��� ���� �����ϸ� __slGetAt �Լ��� NULL�� �����ϹǷ� �̰Ϳ� �����Ͽ� �ۼ��� ���α׷� �ڵ�
	// ��, __an_psInfo�� NULL�̸� ���� ����Ʈ�� �������� Head�� ��ġ��Ű�� �ٽ� __slGetAt�Լ��� SURFACEINFO�� ���ϴ� ���̴�.
	// �ᱹ �ִϸ��̼��� �ݺ��ȴ�.	
	if (!__an_psInfo) 
	{				
		__slSetHeadPosition();
		__an_psInfo = (SURFACEINFO*)__slGetAt();
		if (!__an_psInfo) return;
	}
	
	//// ��������Ʈ �������� �����ð��� �Ѿ����?
	__an_dwThisTick = GetTickCount();	
	if (__an_dwThisTick - __pObjInfo->aiBase.dwLastTick > __an_psInfo->dwDelayTime)
	{
		// ��������Ʈ ������ ����
		__an_psInfo = (SURFACEINFO*)__slGetNext();			
		__pObjInfo->aiBase.dwLastTick = __an_dwThisTick;			
	}

	__slGetMember(&(__pObjInfo->aiBase.slAnimation));	

	__gePutImageRotateObject(&g_sfTotalObjectRotate, __pObjInfo->nY, 0, &__pObjInfo->nXr, &__pObjInfo->nYr, __an_psInfo);	// ���� ������Ʈ�̹��� ȸ�� ���
	__gePutSprite(g_sfTotalObjectSprite.dcSurface, 0, 0, 0, 0, &g_sfTotalObjectRotate);
	//// ���� �̹��� ���
	//__gePutImageBlend(g_sfAdjustFocus.dcSurface, nX-__an_psInfo->nCentralX, SCR_HEIGHT - (nY+__an_psInfo->nCentralY), __an_psInfo, 255);
}

//// ������Ʈ �ִϸ��̼� ���� ����Ʈ�� �Ҹ��ϱ� ������ ���� �ݺ���Ű�� �Լ�
void __anAnimateObject(HDC hdc, GOBJOBJECT_INFO* __pObjInfo)
{    
	__slSetMember(&(__pObjInfo->aiBase.slAnimation));

	//// ���� ǥ�������� ��´�
	__an_psInfo = (SURFACEINFO*)__slGetAt();

	// ��尡 �ʱ�ȭ�� �ȵǾ��ų� ����� ���̸� ����
	// ���� ����Ʈ�� ��� ���� �����ϸ� __slGetAt �Լ��� NULL�� �����ϹǷ� �̰Ϳ� �����Ͽ� �ۼ��� ���α׷� �ڵ�
	// ��, __an_psInfo�� NULL�̸� ���� ����Ʈ�� �������� Head�� ��ġ��Ű�� �ٽ� __slGetAt�Լ��� SURFACEINFO�� ���ϴ� ���̴�.
	// �ᱹ �ִϸ��̼��� �ݺ��ȴ�.	
	if (!__an_psInfo) 
	{				
		__slSetHeadPosition();
		__an_psInfo = (SURFACEINFO*)__slGetAt();
		if (!__an_psInfo) return;
	}
	
	//// ��������Ʈ �������� �����ð��� �Ѿ����?
	__an_dwThisTick = GetTickCount();	
	if (__an_dwThisTick - __pObjInfo->aiBase.dwLastTick > __an_psInfo->dwDelayTime)
	{
		// ��������Ʈ ������ ����
		__an_psInfo = (SURFACEINFO*)__slGetNext();			
		__pObjInfo->aiBase.dwLastTick = __an_dwThisTick;			
	}

	__slGetMember(&(__pObjInfo->aiBase.slAnimation));	

	if(!Info_On){
		__gePutImageRotateObject(&g_sfTotalObjectRotate, __pObjInfo->nY, __pObjInfo->theta, &__pObjInfo->nXr, &__pObjInfo->nYr, __an_psInfo);	// ���� ������Ʈ�̹��� ȸ�� ���
		__gePutSprite(g_sfTotalObjectSprite.dcSurface, 0, 0, 0, 0, &g_sfTotalObjectRotate);
	}
	else{
		__gePutImageRotateObject(&g_sfStarTotalObjectRotate, __pObjInfo->nY, __pObjInfo->theta, &__pObjInfo->nXr, &__pObjInfo->nYr, __an_psInfo);	// ���� ������Ʈ�̹��� ȸ�� ���
		__gePutSprite(g_sfStarTotalObjectSprite.dcSurface, 0, 0, 0, 0, &g_sfStarTotalObjectRotate);
	}
}
























