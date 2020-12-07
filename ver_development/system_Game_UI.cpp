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

	// User Interface (�޴���)
	__gePutSprite(hdc, 0, 0, 0, 0, sfUI);

	// ���� ���
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

	// ȸ�� ȭ��ǥ ���
	__gePutSprite(hdc, 0, 0, 0, 0, &g_sfPlayArrow);

	// Infomation ���
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
		
		if(StarInfo->NowTemperature >= 80){ SetTextColor(hdc, RGB(255,0,0)); strcpy(str,"�ſ� ����"); }
		else if(StarInfo->NowTemperature >= 60){ SetTextColor(hdc, RGB(200,0,0)); strcpy(str,"����"); }
		else if(StarInfo->NowTemperature >= 40){ SetTextColor(hdc, RGB(0,100,0)); strcpy(str,"������"); }
		else if(StarInfo->NowTemperature >= 20){ SetTextColor(hdc, RGB(0,0,200)); strcpy(str,"����"); }
		else { SetTextColor(hdc, RGB(0,0,255)); strcpy(str,"�ſ� ����"); }

		TextOut(hdc, 200, 102, str, (int)strlen(str));

		if(StarInfo->NowEnergy >= 80){ SetTextColor(hdc, RGB(0,255,0)); strcpy(str,"����ռ���"); }
		else if(StarInfo->NowEnergy >= 60){ SetTextColor(hdc, RGB(0,200,0)); strcpy(str,"Ȱ����"); }
		else if(StarInfo->NowEnergy >= 40){ SetTextColor(hdc, RGB(0,100,0)); strcpy(str,"�����");	}
		else if(StarInfo->NowEnergy >= 20){ SetTextColor(hdc, RGB(200,0,0)); strcpy(str,"������"); }
		else { SetTextColor(hdc, RGB(255,0,0)); strcpy(str,"�Ҹ�����"); }

		TextOut(hdc, 200, 202, str, (int)strlen(str));

		if(StarInfo->NowFood >= 80){ SetTextColor(hdc, RGB(0,255,0)); strcpy(str,"��θ�"); }
		else if(StarInfo->NowFood >= 60){ SetTextColor(hdc, RGB(0,200,0)); strcpy(str,"����"); }
		else if(StarInfo->NowFood >= 40){  SetTextColor(hdc, RGB(0,100,0)); strcpy(str,"�����׷�"); }
		else if(StarInfo->NowFood >= 20){  SetTextColor(hdc, RGB(200,0,0)); strcpy(str,"�����"); }
		else {  SetTextColor(hdc, RGB(255,0,0)); strcpy(str,"�ſ� �����"); }

		TextOut(hdc, 200, 155, str, (int)strlen(str));

		if(StarInfo->NowEnvironment >= 80){ SetTextColor(hdc, RGB(0,255,0)); strcpy(str,"�ſ� ������"); }
		else if(StarInfo->NowEnvironment >= 60){ SetTextColor(hdc, RGB(0,200,0)); strcpy(str,"������"); }
		else if(StarInfo->NowEnvironment >= 40){ SetTextColor(hdc, RGB(0,100,0));strcpy(str,"�����"); }
		else if(StarInfo->NowEnvironment >= 20){ SetTextColor(hdc, RGB(200,0,0)); strcpy(str,"Ȳ����"); }
		else { SetTextColor(hdc, RGB(255,0,0)); strcpy(str,"�ſ� Ȳ����"); }

		TextOut(hdc, 435, 122, str, (int)strlen(str));

		if(StarInfo->NowFeeling >= 80){ SetTextColor(hdc, RGB(0,255,0)); strcpy(str,"�ſ� �ູ��"); }
		else if(StarInfo->NowFeeling >= 60){ SetTextColor(hdc, RGB(0,200,0)); strcpy(str,"�ູ��"); }
		else if(StarInfo->NowFeeling >= 40){ SetTextColor(hdc, RGB(0,100,0)); strcpy(str,"�����׷�"); }
		else if(StarInfo->NowFeeling >= 20){ SetTextColor(hdc, RGB(200,0,0)); strcpy(str,"����"); }
		else { SetTextColor(hdc, RGB(255,0,0)); strcpy(str,"�ſ� ����"); }

		TextOut(hdc, 435, 170, str, (int)strlen(str));

		// ���� �Ҹ�
		SetTextColor(hdc, RGB(0,0,0));
		if(!StarInfo->bIsHope) strcpy(str,StarInfo->Hope);
		else strcpy(str, "�ҿ�����");
		TextOut(hdc, 200, 272, str, (int)strlen(str));

		SetTextColor(hdc, RGB(255,0,0));
		// ���� ����
		switch(StarInfo->Disease){
		case HEALTHY: strcpy(str, "�ǰ���"); break;
		case COLD: strcpy(str, "����"); break;
		case DEBILITY: strcpy(str, "���"); break;
		case MELANCHOLY: strcpy(str, "���"); break;
		case DIRTY: strcpy(str, "������"); break;
		case OLD: strcpy(str, "����"); break;
		case HUNGER: strcpy(str, "���ָ�"); break;
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

		// ü��
		if(g_ptMouseAbsolute.x >= 200 && g_ptMouseAbsolute.y >=102 && g_ptMouseAbsolute.x <= 285 && g_ptMouseAbsolute.y <= 125){
			__gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfInfoBack);
			if(__tmIsValidTimer(&StarInfo->TimeDisease)){
				if(StarInfo->Disease == COLD){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "���⿡ ���� �����");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
				}
			}
		}

		// ������
		if(g_ptMouseAbsolute.x >= 200 && g_ptMouseAbsolute.y >=155 && g_ptMouseAbsolute.x <= 305 && g_ptMouseAbsolute.y <= 179){
			__gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfInfoBack);
			if(__tmIsValidTimer(&StarInfo->TimeEnergy)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "�������� ���� �϶���");
				TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
				ny += 16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeDisease)){
				if(StarInfo->Disease == HUNGER){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "���ָ��� ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
					ny += 16;
				}
			}
		}
		ny = 30;
		
		// ������
		if(g_ptMouseAbsolute.x >= 200 && g_ptMouseAbsolute.y >=202 && g_ptMouseAbsolute.x <= 258 && g_ptMouseAbsolute.y <= 228){
			__gePutSprite(hdc, g_ptMouseAbsolute.x, g_ptMouseAbsolute.y, 0, 0, &g_sfInfoBack);
			if(__tmIsValidTimer(&StarInfo->TimeEnvironment)){
				SetTextColor(hdc, RGB(0,255,0));
				strcpy(str, "ȯ���� ���� �����");
                TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeFeeling)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "����� ���� �϶���");
				TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeFood)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "����Ŀ� ���� �϶���");
				TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y + ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeDisease)){
				if(StarInfo->Disease == COLD){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "���⿡ ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
				else if(StarInfo->Disease == DEBILITY){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "��࿡ ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
				else if(StarInfo->Disease == MELANCHOLY){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "��￡ ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
				else if(StarInfo->Disease == OLD){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "�ľ������ ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
				else if(StarInfo->Disease == HUNGER){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "���ָ��� ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
				else if(StarInfo->Disease == HEALTHY){
					SetTextColor(hdc, RGB(0,255,0));
					strcpy(str, "�ǰ��Կ� ���� �����");
					TextOut(hdc, g_ptMouseAbsolute.x + nx, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
			}
		}
		ny = 30;

		// ȯ��
		if(g_ptMouseAbsolute.x >= 435 && g_ptMouseAbsolute.y >=122 && g_ptMouseAbsolute.x <= 538 && g_ptMouseAbsolute.y <= 150){
			__gePutSprite(hdc, g_ptMouseAbsolute.x-100, g_ptMouseAbsolute.y, 0, 0, &g_sfInfoBack);
			if(__tmIsValidTimer(&StarInfo->TimeDisease)){
				if(StarInfo->Disease == DIRTY){
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "������ ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx -100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny += 16;
				}
			}
		}
		ny = 30;

		// ���
		if(g_ptMouseAbsolute.x >= 435 && g_ptMouseAbsolute.y >=180 && g_ptMouseAbsolute.x <= 525 && g_ptMouseAbsolute.y <= 196){
			__gePutSprite(hdc, g_ptMouseAbsolute.x-100, g_ptMouseAbsolute.y, 0, 0, &g_sfInfoBack);
			if(__tmIsValidTimer(&StarInfo->TimeEnergy)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "�������� ���� �϶���");
				TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeEnvironment)){
				SetTextColor(hdc, RGB(0,255,0));
				strcpy(str, "ȯ���� ���� �����");
				TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeFeeling)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "����� ���� �϶���");
				TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeFood)){
				SetTextColor(hdc, RGB(255,0,0));
				strcpy(str, "����Ŀ� ���� �϶���");
				TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
				ny+=16;
			}
			if(__tmIsValidTimer(&StarInfo->TimeDisease)){
				switch(StarInfo->Disease){
				case COLD:
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "���⿡ ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				case DEBILITY:
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "��࿡ ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				case MELANCHOLY:
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "��￡ ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				case DIRTY:
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "������ ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				case OLD:
					SetTextColor(hdc, RGB(255,0,0));
					strcpy(str, "�ľ������ ���� �϶���");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				case HEALTHY:
					SetTextColor(hdc, RGB(0,255,0));
					strcpy(str, "�ǰ��Կ� ���� �����");
					TextOut(hdc, g_ptMouseAbsolute.x + nx-100, g_ptMouseAbsolute.y+ny, str, (int)strlen(str));
					ny+=16;
					break;
				}
			}
		}
		ny = 30;

		// �Ҹ�
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

		// ����
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

	// �׷��� Ÿ���� ���
	if(UiState == OBJECT && ObjectKind != NOTHING){
		switch(ObjectKind){
		case STAR_SPECIAL: Tipo_nx = 35, Tipo_ny = 45; break;
		case STAR_UNDERGROUND: Tipo_nx = 129, Tipo_ny = 45; break;
		case STAR_GROUND: Tipo_nx = 292, Tipo_ny = 45; break;
		case STAR_SKY:  Tipo_nx = 403, Tipo_ny = 45; break;
		}
		__gePutSprite(hdc, Tipo_nx, Tipo_ny, 0, 0, &g_sfTipo[ObjectKind]);

		// ������Ʈ ���� ���
		if(g_ptMouseAbsolute.x >= Tipo_nx && g_ptMouseAbsolute.y >= Tipo_ny &&			// 1�� 1��
			g_ptMouseAbsolute.x <= Tipo_nx+30 && g_ptMouseAbsolute.y <= Tipo_ny+30){
			switch(ObjectKind){
			case STAR_SPECIAL: __gePutSprite(hdc, Tipo_nx, Tipo_ny+30, 0, 0, &g_sfObjectInformation[4][0]); break;
			case STAR_UNDERGROUND:  break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][0]); break;
			case STAR_SKY:  __gePutSprite(hdc, Tipo_nx, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][1]); break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 60 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1�� 3��
			g_ptMouseAbsolute.x <= Tipo_nx+90 && g_ptMouseAbsolute.y <= Tipo_ny+30){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+60, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][3]); break;
			case STAR_SKY:  break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 120 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1�� 5��
			g_ptMouseAbsolute.x <= Tipo_nx+150 && g_ptMouseAbsolute.y <= Tipo_ny+30){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: __gePutSprite(hdc, Tipo_nx+120, Tipo_ny+30, 0, 0, &g_sfObjectInformation[3][2]);break;
			case STAR_GROUND:  __gePutSprite(hdc, Tipo_nx+120, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][2]); break;
			case STAR_SKY:  break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 180 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1�� 7��
			g_ptMouseAbsolute.x <= Tipo_nx+210 && g_ptMouseAbsolute.y <= Tipo_ny+30){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: __gePutSprite(hdc, Tipo_nx+180, Tipo_ny+30, 0, 0, &g_sfObjectInformation[3][4]); break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+150, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][4]); break;
			case STAR_SKY:  break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 240 && g_ptMouseAbsolute.y >= Tipo_ny &&		// 1�� 9��
			g_ptMouseAbsolute.x <= Tipo_nx+270 && g_ptMouseAbsolute.y <= Tipo_ny+30){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+150, Tipo_ny+30, 0, 0, &g_sfObjectInformation[0][5]); break;
			case STAR_SKY:  break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2�� 1��
			g_ptMouseAbsolute.x <= Tipo_nx+30 && g_ptMouseAbsolute.y <= Tipo_ny+60){
			switch(ObjectKind){
			case STAR_SPECIAL: __gePutSprite(hdc, Tipo_nx, Tipo_ny+60, 0, 0, &g_sfObjectInformation[4][1]); break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx, Tipo_ny+60, 0, 0, &g_sfObjectInformation[1][0]); break;
			case STAR_SKY:  __gePutSprite(hdc, Tipo_nx, Tipo_ny+60, 0, 0, &g_sfObjectInformation[1][1]); break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 60 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2�� 3��
			g_ptMouseAbsolute.x <= Tipo_nx+90 && g_ptMouseAbsolute.y <= Tipo_ny+60){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+60, Tipo_ny+60, 0, 0, &g_sfObjectInformation[1][3]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 120 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2�� 5��
			g_ptMouseAbsolute.x <= Tipo_nx+150 && g_ptMouseAbsolute.y <= Tipo_ny+60){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+120, Tipo_ny+60, 0, 0, &g_sfObjectInformation[1][2]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 180 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&	// 2�� 7��
			g_ptMouseAbsolute.x <= Tipo_nx+210 && g_ptMouseAbsolute.y <= Tipo_ny+60){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND:  __gePutSprite(hdc, Tipo_nx+150, Tipo_ny+60, 0, 0, &g_sfObjectInformation[1][4]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 240 && g_ptMouseAbsolute.y >= Tipo_ny+30 &&		// 2�� 9��
			g_ptMouseAbsolute.x <= Tipo_nx+270 && g_ptMouseAbsolute.y <= Tipo_ny+60){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+150, Tipo_ny+60, 0, 0, &g_sfObjectInformation[0][5]); break;
			case STAR_SKY:  break;
			}				
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3�� 1��
			g_ptMouseAbsolute.x <= Tipo_nx+30 && g_ptMouseAbsolute.y <= Tipo_ny+90){
			switch(ObjectKind){
			case STAR_SPECIAL: __gePutSprite(hdc, Tipo_nx, Tipo_ny+90, 0, 0, &g_sfObjectInformation[4][2]); break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx, Tipo_ny+90, 0, 0, &g_sfObjectInformation[2][0]); break;
			case STAR_SKY:   break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 60 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3�� 3��
			g_ptMouseAbsolute.x <= Tipo_nx+90 && g_ptMouseAbsolute.y <= Tipo_ny+90){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+60, Tipo_ny+90, 0, 0, &g_sfObjectInformation[2][3]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 120 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3�� 5��
			g_ptMouseAbsolute.x <= Tipo_nx+150 && g_ptMouseAbsolute.y <= Tipo_ny+90){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+120, Tipo_ny+90, 0, 0, &g_sfObjectInformation[2][2]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 180 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&	// 3�� 7��
			g_ptMouseAbsolute.x <= Tipo_nx+210 && g_ptMouseAbsolute.y <= Tipo_ny+90){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: __gePutSprite(hdc, Tipo_nx+150, Tipo_ny+90, 0, 0, &g_sfObjectInformation[2][4]); break;
			case STAR_SKY:  break;
			}
		}
		else if(g_ptMouseAbsolute.x >= Tipo_nx + 240 && g_ptMouseAbsolute.y >= Tipo_ny+60 &&		// 3�� 9��
			g_ptMouseAbsolute.x <= Tipo_nx+270 && g_ptMouseAbsolute.y <= Tipo_ny+90){
			switch(ObjectKind){
			case STAR_SPECIAL: break;
			case STAR_UNDERGROUND: break;
			case STAR_GROUND: break;
			case STAR_SKY:  break;
			}				
		}
	}

	// ���� ���� ���� ���
	if(__tmIsValidTimer(&TimeLack))
		if(!__tmIsElapseTimer(&TimeLack))
			__gePutSprite(hdc, 0, 0, 0, 0, &g_sfLack);
	
	// ���� ���� ���� ���	
	if(nObject_ID && __GObjFocusGetObjInfo()){					// ��Ŀ�� ������Ʈ�� �����Ǿ���
		if(!__ccCheckCollisionFocus((GOBJFOCUS_INFO*)__GObjFocusGetObjInfo(), FOCUS, StarStep)){			// �ٸ� ������Ʈ�� ��ġ�� �ʴ´ٸ�	
			static int __nCountSpace = 1;
			
			if(__tmIsElapseTimer(&TimeSpace)){					// Ÿ�̸Ӱ� �ð��� �ٵǸ� �ٽ� �ʱ�ȭ�� �ǵ��� �Ѵ�.
				nCountSpace++;
				__tmInitTimer(&TimeSpace, 500);
				if(nCountSpace >= 2) nCountSpace = 0;
			}				
			__gePutSprite(hdc, SCR_WIDTH/2-50, __fo_pObjInfo[StarStep]->nYr - 110, 0, 0, &g_sfSpace[nCountSpace%2]);
		}
		else													// �ٸ� ������Ʈ�� ��ģ�ٸ�
			__tmInitTimer(&TimeSpace,0);						// �ʱ�ȭ �����ش�
	}
}