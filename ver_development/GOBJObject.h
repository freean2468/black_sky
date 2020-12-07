#ifndef __GObjObject_H__
#define __GObjObject_H__

#include "stdafx.h"

// ������Ʈ�� �Ӽ� ����
// �Ӽ� ������, ���� ������Ʈ�� ������ ���� ���� ��ġ�鿡 ���� ����
typedef struct GOBJOBJECT_INFOtag
{
	int			nIdx;			// ������Ʈ ����Ʈ���� �ε���
	int			nX;				// ������Ʈ�� ��� XY ��ǥ
	int			nY;
	int			Create;			// ������ ����
	float		theta;			// ���� ����
	int			StarStep;		// �����ϴ� ��
	int			nXr;			// ȸ�� �� x ��ǥ
	int			nYr;			// ȸ�� �� y ��ǥ
	int			MudStep;		// �Ҹ� ���� ��
	int			nPositionStep;	// �ʿ��� �����ϴ� �ڽ��� ����
	int			Attribute;		// ������Ʈ �Ӽ�

	TIMER		TimeMud;		// MudTree ������Ʈ���� ��� Ÿ�̸�

	// ������Ʈ �ִϸ��̼ǿ� ���� ����ü
	ANIINFO		aiBase;				
} GOBJOBJECT_INFO;

BOOL __GObjObjectInit(int nIdx, int Attribute, int MudStep, int nPositionStep, int nX, int nY, float theta, char* pAniFile, HDC hdc);			// �ʱ�ȭ
void __GObjObjectInit();															// �ʱ�ȭ
void __GObjObjectRelease();															// �Ҹ�
BOOL __GObjObjectAction();															// �̵�
BOOL __GObjObjectRender(HDC hdc);													// ���
RECT __GObjObjectGetRect();															// �浹����

//���� ������Ʈ ����
void* __GObjObjectCreate(int nSize);												// ����
void* __GObjObjectGetObjInfo();	
void  __GObjObjectSetMember(void* pObjInfo);
void  __GObjObjectGetMember(void* pObjInfo);

#endif