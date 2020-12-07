///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : Timer.cpp
//
//   �� ��  �� �� : Ÿ�̸�
//
//   �� ��  �� �� : �ð��� ����� ������ �� ����Ѵ�.
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Timer.h"

// Ÿ�̸� �ʱ�ȭ
// ó�� �ð��� ������ �� ȣ��
void __tmInitTimer(TIMER* pTimer, DWORD dwDelayTime)
{
	pTimer->dwOldTime = GetTickCount();		// ���� �ð� ����
	pTimer->dwDelayTime = dwDelayTime;
}

// �ð��� ���
// �ʱ⿡ ������ �ð��� ����Ǿ����� �����ϴ� �Լ�
BOOL __tmIsElapseTimer(TIMER* pTimer)
{	
	// GetTickCount�Լ� : OS�� ���۵� �ĺ��� ��������� �ð��� �и�������(1/1000��) ������ ��ȯ
	DWORD dwCurTime = GetTickCount();
	DWORD dwElapsed = dwCurTime - pTimer->dwOldTime;	

	if (dwElapsed >= pTimer->dwDelayTime)
	{		
		pTimer->dwDelayTime = 0;				// 0���� �ʱ�ȭ
		pTimer->dwOldTime = dwCurTime;			// ���� �ð����� �ʱ�ȭ
		return TRUE;							// ���� �ð��� ����Ǿ����� �˸�.
	}	

	return FALSE;
}

// Ÿ�̸Ӱ� ���� ��ȿ���� �����ϴ� �Լ�
// Timer�� ���������� �˻��ؾ� �Ѵٸ� �� �Լ��� ���
BOOL __tmIsValidTimer(TIMER* pTimer)
{
	if (pTimer->dwDelayTime)
		return TRUE;
	else 
		return FALSE;
}
















