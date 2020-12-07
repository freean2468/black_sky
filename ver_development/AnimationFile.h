///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : AnimationFile.h
//
//   �� ��  �� �� : �ִϸ��̼� ���� (save, load)
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ANIMATIONFILE_H__
#define __ANIMATIONFILE_H__

#include "SLinkedList.h"

#define ANIM_FILE_SYMBOL "ANIM"

typedef struct ANIFILEHEADERtag
{
	BYTE       Symbol[4];           // "ANIM"						
    int        nFrameCount;         // ��������Ʈ ������ ����
} ANIFILEHEADER;

BOOL __afSave(SLMEMBER* pslm, char* pstrFilename);
BOOL __afLoad(SLMEMBER* pslm, char* pstrFilename); 
BOOL __afLoad2MemDc(SLMEMBER* pslm, char* pstrFilename, HDC hdc); 

#endif