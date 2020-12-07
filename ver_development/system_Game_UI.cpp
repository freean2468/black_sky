#include "Stdafx.h"

extern SURFACEINFO g_sfTipo[MAX_OBJECT_KIND];
extern SURFACEINFO g_sfObjectInformation[5][20];
extern SURFACEINFO g_sfClear;
extern SURFACEINFO g_sfLack;
extern SURFACEINFO g_sfSpace[2];
extern SURFACEINFO g_sfPlayArrow;
extern SURFACEINFO g_sfInfoBack;
extern SURFACEINFO g_sfHopeInfoBack[MAX_STAR];
extern SURFACEINFO g_sfDiseaseInfoBack[MAX_STAR+1];

extern int nObject_ID;

extern STARSTEP StarStep;

extern POINTS	g_ptMouseAbsolute;

extern GOBJFOCUS_INFO* __fo_pObjInfo[MAX_STAR];

extern TIMER TimeLack;
TIMER TimeSpace;
int nCountSpace;

UISTATE UiState;
OBJECTKIND ObjectKind = NOTHING;

#ifdef _DEBUG
//	int nCountMud = 100;
#endif

int Tipo_nx=0, Tipo_ny=0;

void ShowUI(HDC hdc, SURFACEINFO* sfUI, SURFACEINFO* sfInfo, STARINFO* StarInfo){
	HFONT hFont, OldFont;
	LOGFONT LogFont;
	char str[255];
	int nx = 40, ny = 30;

	// User Interface (메뉴바)
	__gePutSprite(hdc, 0, 0, 0, 0, sfUI);

	// 정보 출력
	LogFont.lfHeight = 20;
	LogFont.lfWidth = 0;
	LogFont.lfEscapement = 0;
	LogFont.lfOrientation = 0;
	LogFont.lfWeight = FW_HEAVY;
	LogFont.lfItalic = 0;
	LogFont.lfUnderline = 0;
	LogFont.lfStrikeOut = 0;
	LogFont.lfCharSet = HANGEUL_CHARSET;
	LogFont.lfOutPrecision = 0;
	LogFont.lfClipPrecision = 0;
	LogFont.lfQuality = 0;
	LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	lstrcpy(LogFont.lfFaceName, TEXT("Meiryo Bold"));

	hFont = CreateFontIndirect(&LogFont);

	OldFont = (HFONT)SelectObject(hdc, hFont);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255,255,255));

	wsprintf(str, "%d", StarInfo->nCountMud);
	TextOut(hdc, 560, 2, str, (int)strlen(str));

	SelectObject(hdc, OldFont);
	DeleteObject(hFont);

	// 회전 화살표 출력
	__gePutSprite(hdc, 0, 0, 0, 0, &g_sfPlayArrow);

	// Infomation 출력
	if(UiState == INFO){
		__gePutSprite(hdc, 0, 0, 0, 0, sfInfo);

		LogFont.lfHeight = 20;
		LogFont.lfWidth = 0;
		LogFont.lfEscapement = 0;
		LogFont.lfOrientation = 0;
		LogFont.lfWeight = FW_HEAVY;
		LogFont.lfItalic = 0;
		LogFont.lfUnderline = 0;
		LogFont.lfStrikeOut = 0;
		LogFont.lfCharSet = HANGEUL_CHARSET;
		LogFont.lfOutPrecision = 0;
		LogFont.lfClipPrecision = 0;
		LogFont.lfQuality = 0;
		LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(LogFont.lfFaceName, TEXT("Meiryob Bold"));

		hFont = CreateFontIndirect(&LogFont);

		OldFont = (HFONT)SelectObject(hdc, hFont);
		
		if(StarInfo->NowTemperature >= 80){ SetTextColor(hdc, RGB(255,0,0)); strcpy(str,"매우 높음"); }
		else if(StarInfo->NowTemperature >= 60){ SetTextColor(hdc, RGB(200,0,0)); strcpy(str,"높음"); }
		else if(StarInfo->NowTemperature >= 40){ SetTextColor(hdc, RGB(0,100,0)); strcpy(str,"적당함"); }
		else if(StarInfo->NowTemperature >= 20){ SetTextColor(hdc, RGB(0,0,200)); strcpy(str,"낮음"); }
		else { SetTextColor(hdc, RGB(0,0,255)); strcpy(str,"매우 낮음"); }

		TextOut(hdc, 200, 102, str, (int)strlen(str));

		if(StarInfo->NowEnergy >= 80){ SetTextColor(hdc, RGB(0,255,0)); strcpy(str,"혈기왕성함"); }
		else if(StarInfo->NowEnergy >= 60){ SetTextColor(hdc, RGB(0,200,0)); strcpy(str,"활기참"); }
		else if(StarInfo->NowEnergy >= 40){ SetTextColor(hdc, RGB(0,100,0)); strcpy(str,"평범함");	}
		else if(StarInfo->NowEnergy >= 20){ SetTextColor(hdc, RGB(200,0,0)); strcpy(str,"힘없음"); }
		else { SetTextColor(hdc, RGB(255,0,0)); strcpy(str,"소멸직전"); }

		TextOut(hdc, 200, 202, str, (int)strlen(str));

		if(StarInfo->NowFood >= 80){ SetTextColor(hdc, RGB(0,255,0)); strcpy(str,"배부름"); }
		else if(StarInfo->NowFood >= 60){ SetTextColor(hdc, RGB(0,200,0)); strcpy(str,"만족"); }
		else if(StarInfo->NowFood >= 40){  SetTextColor(hdc, RGB(0,100,0)); strcpy(str,"그저그럼"); }
		else if(StarInfo->NowFood >= 20){  SetTextColor(hdc, RGB(200,0,0)); strcpy(str,"배고픔"); }
		else {  SetTextColor(hdc, RGB(255,0,0)); strcpy(str,"매우 배고픔"); }

		TextOut(hdc, 200, 155, str, (int)strlen(str));

		if(StarInfo->NowEnvironment >= 80){ SetTextColor(hdc, RGB(0,255,0)); strcpy(str,"매우 깨끗함"); }
		else if(StarInfo->NowEnvironment >= 60){ SetTextColor(hdc, RGB(0,200,0)); strcpy(str,"깨끗함"); }
		else if(StarInfo->NowEnvironment >= 40){ SetTextColor(hdc, RGB(0,100,0));strcpy(str,"평범함"); }
		else if(StarInfo->NowEnvironment >= 20){ SetTextColor(hdc, RGB(200,0,0)); strcpy(str,"황폐함"); }
		else { SetTextColor(hdc, RGB(255,0,0)); strcpy(str,"매우 황폐함"); }

		TextOut(hdc, 435, 122, str, (int)strlen(str));

		if(StarInfo->NowFeeling >= 80){ SetTextColor(hdc, RGB(0,255,0)); strcpy(str,"매우 행복함"); }
		else if(StarInfo->NowFeeling >= 60){ SetTextColor(hdc, RGB(0,200,0)); strcpy(str,"행복함"); }
		else if(StarInfo->NowFeeling >= 40){ SetTextColor(hdc, RGB(0,100,0)); strcpy(str,"그저그럼"); }
		else if(StarInfo->NowFeeling >= 20){ SetTextColor(hdc, RGB(200,0,0)); strcpy(str,"나쁨"); }
		else { SetTextColor(hdc, RGB(255,0,0)); strcpy(str,"매우 나쁨"); }

		TextOut(hdc, 435, 170, str, (int)strlen(str));

		// 별의 소망
		SetTextColor(hdc, RGB(0,0,0));
		if(!StarInfo->bIsHope) strcpy(str,StarInfo->Hope);
		else strcpy(str, "소원성취");
		TextOut(hdc, 200, 272, str, (int)strlen(str));

		SetTextColor(hdc, RGB(255,0,0));
		// 별의 상태
		switch(StarInfo->Disease){
		case HEALTHY: strcpy(str, "건강함"); break;
		case COLD: strcpy(str, "감기"); break;
		case DEBILITY: strcpy(str, "쇠약"); break;
		case MELANCHOLY: strcpy(str, "우울"); break;
		case DIRTY: strcpy(str, "더러움"); break;
		case OLD: strcpy(str, "늙음"); break;
		case HUNGER: strcpy(str, "굶주림"); break;
		}

		if(StarInfo->Disease == HEALTHY) SetTextColor(hdc, RGB(0,255,0));
		TextOut(hdc, 200, 322, str, (int)strlen(str));

		SelectObject(hdc, OldFont);

		LogFont.lfHeight = 15;
		LogFont.lfWidth = 0;
		LogFont.lfEscapement = 0;
		LogFont.lfOrientation = 0;
		LogFont.lfWeight = FW_HEAVY;
		LogFont.lfItalic = 0;
		LogFont.lfUnderline = 0;
		LogFont.lfStrikeOut = 0;
		LogFont.lfCharSet = HANGEUL_CHARSET;
		LogFont.lfOutPrecision = 0;
		LogFont.lfClipPrecision = 0;
		LogFont.lfQuality = 0;
		LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(LogFont.lfFaceName, TEXT("Meiryo Bold"));

		hFont = CreateFontIndirect(&LogFont);

		OldFont = (HFONT)SelectObject(hdc, hFont);

		// 체온
		if(g_ptMouseAbsolute.x >= 200 && g_ptMouseAbsolute.y >=102 && g_ptMouseAbsolute.x <= 285 && g_ptMouseAbsolute.y <= 125){
			__gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfInfoBack);
			if(__tmIsValidTimer(&StarInfo->TimeDisease)){
				if(StarInfo->Disease == COLD){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "감기에 의해 상승중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
				}
			}
		}

		// 포만감
		if(g_ptMouseAbsolute.x >= 200 && g_ptMouseAbsolute.y >=155 && g_ptMouseAbsolute.x <= 305 && g_ptMouseAbsolute.y <= 179){
			__gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfInfoBack);
			if(__tmIsValidTimer(&StarInfo->TimeEnergy)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "에너지가 없어 하락중");
				TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
				ny += 16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeDisease)){
				if(StarInfo->Disease == HUNGER){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "굶주림에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
					ny += 16;
				}
			}
		}
		ny = 30;
		
		// 에너지
		if(g_ptMouseAbsolute.x >= 200 && g_ptMouseAbsolute.y >=202 && g_ptMouseAbsolute.x <= 258 && g_ptMouseAbsolute.y <= 228){
			__gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfInfoBack);
			if(__tmIsValidTimer(&StarInfo->TimeEnvironment)){
				SetTextColor(hdc, RGB(0,255,0));
				strcpy(str, "환경이 좋아 상승중");
                TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeFeeling)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "기분이 나빠 하락중");
				TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeFood)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "배고픔에 의해 하락중");
				TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeDisease)){
				if(StarInfo->Disease == COLD){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "감기에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
				else if(StarInfo->Disease == DEBILITY){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "쇠약에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
				else if(StarInfo->Disease == MELANCHOLY){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "우울에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
				else if(StarInfo->Disease == OLD){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "늙어버림에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
				else if(StarInfo->Disease == HUNGER){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "굶주림에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
				else if(StarInfo->Disease == HEALTHY){
					SetTextColor(hdc, RGB(0,255,0));
					strcpy(str, "건강함에 의해 상승중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
			}
		}
		ny = 30;

		// 환경
		if(g_ptMouseAbsolute.x >= 435 && g_ptMouseAbsolute.y >=122 && g_ptMouseAbsolute.x <= 538 && g_ptMouseAbsolute.y <= 150){
			__gePutSprite(hdc, g_ptMouseAbsolute.x-100, g_ptMouseAbsolute.y, 0, 0, &g_sfInfoBack);
			if(__tmIsValidTimer(&StarInfo->TimeDisease)){
				if(StarInfo->Disease == DIRTY){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "더러움에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx -100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
			}
		}
		ny = 30;

		// 기분
		if(g_ptMouseAbsolute.x >= 435 && g_ptMouseAbsolute.y >=180 && g_ptMouseAbsolute.x <= 525 && g_ptMouseAbsolute.y <= 196){
			__gePutSprite(hdc, g_ptMouseAbsolute.x-100, g_ptMouseAbsolute.y, 0, 0, &g_sfInfoBack);
			if(__tmIsValidTimer(&StarInfo->TimeEnergy)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "에너지가 없어 하락중");
				TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeEnvironment)){
				SetTextColor(hdc, RGB(0,255,0));
				strcpy(str, "환경이 좋아 상승중");
				TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeFeeling)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "기분이 나빠 하락중");
				TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeFood)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "배고픔에 의해 하락중");
				TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeDisease)){
				switch(StarInfo->Disease){
				case COLD:
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "감기에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				case DEBILITY:
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "쇠약에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				case MELANCHOLY:
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "우울에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				case DIRTY:
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "더러움에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				case OLD:
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "늙어버림에 의해 하락중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				case HEALTHY:
					SetTextColor(hdc, RGB(0,255,0));
					strcpy(str, "건강함에 의해 상승중");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				}
			}
		}
		ny = 30;

		// 소망
		if(g_ptMouseAbsolute.x >= 200 && g_ptMouseAbsolute.y >=282 && g_ptMouseAbsolute.x <= 333 && g_ptMouseAbsolute.y <= 304){
			if(!StarInfo->bIsHope){
				switch(StarStep){
					case TUTORIAL: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfHopeInfoBack[0]); break;
					case RED_STAR: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfHopeInfoBack[1]); break;
					case YELLOW_STAR: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfHopeInfoBack[2]);	break;
					case BLUE_STAR:	__gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfHopeInfoBack[3]); break;
					case PINK_STAR:	__gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfHopeInfoBack[4]); break;
					case BROWN_STAR: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfHopeInfoBack[5]); break;
				}
			}

		}
		ny = 30;

		// 상태
		if(g_ptMouseAbsolute.x >= 200 && g_ptMouseAbsolute.y >=332 && g_ptMouseAbsolute.x <= 244 && g_ptMouseAbsolute.y <= 348){
			switch(StarInfo->Disease){
			case COLD: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfDiseaseInfoBack[0]); break;
			case DEBILITY: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfDiseaseInfoBack[5]); break;
			case MELANCHOLY: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfDiseaseInfoBack[4]); break;
			case DIRTY: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfDiseaseInfoBack[2]); break;
			case OLD: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfDiseaseInfoBack[1]); break;
			case HUNGER: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfDiseaseInfoBack[3]); break;
			case HEALTHY: __gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfDiseaseInfoBack[6]); break;
			}
		}

		SelectObject(hdc, OldFont);
		DeleteObject(hFont);
	}	

	// 그래픽 타이포 출력
	if(UiState == OBJECT && ObjectKind != NOTHING){
		switch(ObjectKind){
		case STAR_SPECIAL: Tipo_nx = 35, Tipo_ny = 45; break;
		case STAR_UNDERGROUND: Tipo_nx = 129, Tipo_ny = 45; break;
		case STAR_GROUND: Tipo_nx = 292, Tipo_ny = 45; break;
		case STAR_SKY:  Tipo_nx = 403, Tipo_ny = 45; break;
		}
		__gePutSprite(hdc, Tipo_nx, Tipo_ny, 0, 0, &g_sfTipo[ObjectKind]);

		// 오브젝트 정보 출력
		if(g_ptMouseAbsolute.x >= Tipo_nx && g_ptMouseAbsolute.y >= Tipo_ny &&			// 1행 1열
			g_ptMouseAbsolute.x <= Tipo_nx+30 && g_ptMouseAbsolute.y <= Tipo_ny+30){
			switch(ObjectKind){
			case STAR_SPECIAL: __gePutSprite(hdc, Tipo_nx, Tipo_ny+30, 0, 0, &g_sfObjectInformation[4][0]); break;
			case STAR_UNDERGROUND:  break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][0]); break;
			case STAR_SKY:  __gePutSprite(hdc, Tipo_nx, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][1]); break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 60 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1행 3열
			g_ptMouseAbsolute.x <= Tipo_nx+90 && g_ptMouseAbsolute.y <= Tipo_ny+30){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+60, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][3]); break;
			case STAR_SKY:  break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 120 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1행 5열
			g_ptMouseAbsolute.x <= Tipo_nx+150 && g_ptMouseAbsolute.y <= Tipo_ny+30){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: __gePutSprite(hdc, Tipo_nx+120, Tipo_ny+30, 0, 0, &g_sfObjectInformation[3][2]);break;
			case STAR_GROUND:  __gePutSprite(hdc, Tipo_nx+120, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][2]); break;
			case STAR_SKY:  break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 180 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1행 7열
			g_ptMouseAbsolute.x <= Tipo_nx+210 && g_ptMouseAbsolute.y <= Tipo_ny+30){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: __gePutSprite(hdc, Tipo_nx+180, Tipo_ny+30, 0, 0, &g_sfObjectInformation[3][4]); break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+150, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][4]); break;
			case STAR_SKY:  break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 240 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1행 9열
			g_ptMouseAbsolute.x <= Tipo_nx+270 && g_ptMouseAbsolute.y <= Tipo_ny+30){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+150, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][5]); break;
			case STAR_SKY:  break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2행 1열
			g_ptMouseAbsolute.x <= Tipo_nx+30 && g_ptMouseAbsolute.y <= Tipo_ny+60){
			switch(ObjectKind){
			case STAR_SPECIAL: __gePutSprite(hdc, Tipo_nx, Tipo_ny+60, 0, 0, &g_sfObjectInformation[4][1]); break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx, Tipo_ny+60, 0, 0, &g_sfObjectInformation[1][0]); break;
			case STAR_SKY:  __gePutSprite(hdc, Tipo_nx, Tipo_ny+60, 0, 0, &g_sfObjectInformation[1][1]); break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 60 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2행 3열
			g_ptMouseAbsolute.x <= Tipo_nx+90 && g_ptMouseAbsolute.y <= Tipo_ny+60){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+60, Tipo_ny+60, 0, 0, &g_sfObjectInformation[1][3]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 120 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2행 5열
			g_ptMouseAbsolute.x <= Tipo_nx+150 && g_ptMouseAbsolute.y <= Tipo_ny+60){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+120, Tipo_ny+60, 0, 0, &g_sfObjectInformation[1][2]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 180 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2행 7열
			g_ptMouseAbsolute.x <= Tipo_nx+210 && g_ptMouseAbsolute.y <= Tipo_ny+60){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND:  __gePutSprite(hdc, Tipo_nx+150, Tipo_ny+60, 0, 0, &g_sfObjectInformation[1][4]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 240 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&		// 2행 9열
			g_ptMouseAbsolute.x <= Tipo_nx+270 && g_ptMouseAbsolute.y <= Tipo_ny+60){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+150, Tipo_ny+60, 0, 0, &g_sfObjectInformation[0][5]); break;
			case STAR_SKY:  break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3행 1열
			g_ptMouseAbsolute.x <= Tipo_nx+30 && g_ptMouseAbsolute.y <= Tipo_ny+90){
			switch(ObjectKind){
			case STAR_SPECIAL: __gePutSprite(hdc, Tipo_nx, Tipo_ny+90, 0, 0, &g_sfObjectInformation[4][2]); break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx, Tipo_ny+90, 0, 0, &g_sfObjectInformation[2][0]); break;
			case STAR_SKY:   break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 60 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3행 3열
			g_ptMouseAbsolute.x <= Tipo_nx+90 && g_ptMouseAbsolute.y <= Tipo_ny+90){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+60, Tipo_ny+90, 0, 0, &g_sfObjectInformation[2][3]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 120 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3행 5열
			g_ptMouseAbsolute.x <= Tipo_nx+150 && g_ptMouseAbsolute.y <= Tipo_ny+90){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+120, Tipo_ny+90, 0, 0, &g_sfObjectInformation[2][2]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 180 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3행 7열
			g_ptMouseAbsolute.x <= Tipo_nx+210 && g_ptMouseAbsolute.y <= Tipo_ny+90){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+150, Tipo_ny+90, 0, 0, &g_sfObjectInformation[2][4]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 240 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&		// 3행 9열
			g_ptMouseAbsolute.x <= Tipo_nx+270 && g_ptMouseAbsolute.y <= Tipo_ny+90){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: break;
			case STAR_SKY:  break;
			}				
		}
	}

	// 진흙 부족 정보 출력
	if(__tmIsValidTimer(&TimeLack))
		if(!__tmIsElapseTimer(&TimeLack))
			__gePutSprite(hdc, 0, 0, 0, 0, &g_sfLack);
	
	// 생성 여부 정보 출력	
	if(nObject_ID && __GObjFocusGetObjInfo()){					// 포커스 오브젝트가 생성되었고
		if(!__ccCheckCollisionFocus((GOBJFOCUS_INFO*)__GObjFocusGetObjInfo(), FOCUS, StarStep)){			// 다른 오브젝트와 겹치지 않는다면	
			static int __nCountSpace = 1;
			
			if(__tmIsElapseTimer(&TimeSpace)){					// 타이머가 시간이 다되면 다시 초기화가 되도록 한다.
				nCountSpace++;
				__tmInitTimer(&TimeSpace, 500);
				if(nCountSpace >= 2) nCountSpace = 0;
			}				
			__gePutSprite(hdc, SCR_WIDTH/2-50, __fo_pObjInfo[StarStep]->nYr - 110, 0, 0, &g_sfSpace[nCountSpace%2]);
		}
		else													// 다른 오브젝트와 겹친다면
			__tmInitTimer(&TimeSpace,0);						// 초기화 시켜준다
	}
}