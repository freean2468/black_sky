///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : Animation.h
//
//   �� ��  �� �� : �ִϸ��̼�
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "StdAfx.h"
#include "GOBJFocus.h"
#include "GOBJObject.h"

void __anReset(SLMEMBER* pslmMember);
RECT __anGetCurSprRect(ANIINFO* pAi);	
BOOL __anAnimateOnce(HDC hdc, int nX, int nY, ANIINFO* pAi);
void __anAnimateFocus(HDC hdc, GOBJFOCUS_INFO* __pObjInfo);
void __anAnimateObject(HDC hdc, GOBJOBJECT_INFO* __pObjInfo);

#endif