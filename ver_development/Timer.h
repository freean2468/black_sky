///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : Timer.h
//
//   �� ��  �� �� : Ÿ�̸�
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//#ifndef __TIMER_H__
//#define __TIMER_H__
#pragma once

#include <windows.h>

typedef struct 
{
	DWORD dwOldTime;			// ���� �ð��� ��� �ִ� ����
	DWORD dwDelayTime;			// ������ �ð��� ��� �ִ� ����
} TIMER;

void __tmInitTimer(TIMER* pTimer, DWORD dwDelayTime);
BOOL __tmIsElapseTimer(TIMER* pTimer);
BOOL __tmIsValidTimer(TIMER* pTimer);

//#endif







