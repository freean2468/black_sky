#include "Stdafx.h"

FONTSIZE FontSize = SMALL;

//
//void TextShow(char* str){
//	SDL_Load_Text(str);
////}
//
//void TextShow(int Num, int nX, int nY, HDC hdc, FONTSIZE Size){
//	int one = 0;
//
//	if(Num == 100){			// 100 이라면
//		__gePutSprite(hdc, nX, nY, 0, 0, &g_sfNumber[Size][0]);
//		__gePutSprite(hdc, nX-g_sfNumber[Size][1].nWidth, nY, 0, 0, &g_sfNumber[Size][0]);
//		__gePutSprite(hdc, nX-(g_sfNumber[Size][0].nWidth*2), nY, 0, 0, &g_sfNumber[Size][1]);
//	}
//	else if(Num >= 10){		// 10보다 크거나 같다면
//		one = Num % 10;
//
//		Num -= one;
//		Num = Num / 10;
//
//		__gePutSprite(hdc, nX, nY, 0, 0, &g_sfNumber[Size][one]);
//		__gePutSprite(hdc, nX-g_sfNumber[Size][Num].nWidth, nY, 0, 0, &g_sfNumber[Size][Num]);
//	}
//	else if(Num < 10){					// 10보다 작다면
//		__gePutSprite(hdc, nX, nY, 0, 0, &g_sfNumber[Size][Num]);
//	}
//}