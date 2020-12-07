#ifndef __GObjFocus_H__
#define __GObjFocus_H__

#include "stdafx.h"

// ���ΰ� ������Ʈ�� �Ӽ� ����
// �Ӽ� ������, ���� ������Ʈ�� ������ ���� ���� ��ġ�鿡 ���� ����
typedef struct __GObjFOCUS_INFOtag
{
	int			nIdx;			// ������Ʈ ����Ʈ���� �ε���
	int			nX;				// ������Ʈ�� ��� XY ��ǥ
	int			nY;
	int			nXr;
	int			nYr;
	float		theta;			// ���� ����
	int			StarStep;		// �����ϴ� ��
	int			nPositionStep;	// ������Ʈ�� ���� �����ϴ� ��

	int			Attribute;		// ������Ʈ �Ӽ�
	
	// ������Ʈ �ִϸ��̼ǿ� ���� ����ü
	ANIINFO		aiBase;		
} GOBJFOCUS_INFO;

BOOL __GObjFocusInit(int nIdx, int Attribute, int nPositionStep,int nX, int nY, float theta, char* pAniFile, HDC hdc);		// �ʱ�ȭ
void __GObjFocusRelease();														// �Ҹ�
BOOL __GObjFocusAction();														// �̵�
BOOL __GObjFocusRender(HDC hdc);													// ���
void __GObjFocusInit();
RECT __GObjFocusGetRect();															// �浹����

//���� ������Ʈ ����
void* __GObjFocusCreate(int nSize);												// ����
void* __GObjFocusGetObjInfo();	
void  __GObjFocusSetMember(void* pObjInfo);
void  __GObjFocusGetMember(void* pObjInfo);

#endif