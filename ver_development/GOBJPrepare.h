#pragma once

#include "stdafx.h"

#define PATH			"./Image/Object/"
#define EXT_ANI			".ani"			// Extension Ȯ����

// ���� ������Ʈ ����
void __goPrepareFocus(char* pStr, int MudStep, int nPositionStep, int Attribute, float theta);
void __goPrepareObject(char* pStr, int MudStep, int nPositionStep, int Attribute, float theta);

void __stDispatchFocusObject(int ObjectID, float theta);
void __stDispatchObject(int ObjectID, float theta);

