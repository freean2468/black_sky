#pragma once

const int MAX_NUMBER = 10;

typedef enum FontSize {SMALL = 0, BIG} FONTSIZE;

void TextShow(char* str);
void TextShow(int Num, int nX, int nY, HDC hdc, FONTSIZE Size);