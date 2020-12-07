#pragma once

#include "stdafx.h"

#define PATH			"./Image/Object/"
#define EXT_ANI			".ani"			// Extension 확장자

// 게임 오브젝트 생성
void __goPrepareFocus(char* pStr, int MudStep, int nPositionStep, int Attribute, float theta);
void __goPrepareObject(char* pStr, int MudStep, int nPositionStep, int Attribute, float theta);

void __stDispatchFocusObject(int ObjectID, float theta);
void __stDispatchObject(int ObjectID, float theta);

