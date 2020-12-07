///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : AnimationFile.h
//
//   모 듈  이 름 : 애니메이션 파일 (save, load)
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ANIMATIONFILE_H__
#define __ANIMATIONFILE_H__

#include "SLinkedList.h"

#define ANIM_FILE_SYMBOL "ANIM"

typedef struct ANIFILEHEADERtag
{
	BYTE       Symbol[4];           // "ANIM"						
    int        nFrameCount;         // 스프라이트 프레임 개수
} ANIFILEHEADER;

BOOL __afSave(SLMEMBER* pslm, char* pstrFilename);
BOOL __afLoad(SLMEMBER* pslm, char* pstrFilename); 
BOOL __afLoad2MemDc(SLMEMBER* pslm, char* pstrFilename, HDC hdc); 

#endif