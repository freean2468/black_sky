///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : AnimationInfo.h
//
//   모 듈  이 름 : 애니메이션 정보
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ANIMATIONINFO_H__
#define __ANIMATIONINFO_H__

#include "SLinkedList.h"

typedef struct ANIINFOTag
{
	SLMEMBER slAnimation;	// 애니메이션 연결리스트
	DWORD	 dwLastTick;	// 애니메이션에 사용되는 지난 시간 정보 (지난 시간은 별도로 저장해야 함)
} ANIINFO;

#endif