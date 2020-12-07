#pragma once

#include "TIMER.h"

#define MAX_STAR 6
#define FIRST 102
#define SECOND 103
#define SKY 104

typedef enum StarStep {TUTORIAL = 0, RED_STAR, YELLOW_STAR, BLUE_STAR, PINK_STAR, BROWN_STAR} STARSTEP;

typedef struct StarData_tag{
	// 좌표 관련
	int nX;					// 스테이지 화면에서의 XY 좌표
	int nY;					// 
	int r;					// 별의 반지름

	// 별의 각종 정보
	int nCountMud;			// 이 별에서 가지고 있는 찰흙
	int Attribute;			// 별이 원하는 속성
	int Temperature;		// 별의 체온
	int Food;				// 초기 포만감
	int Energy;				// 초기 에너지
	int Environment;		// 초기 환경
	int Feeling;			// 초기 기분

	int NowTemperature;		// 별의 온도
	int NowFood;			// 식량
	int NowEnergy;			// 산소
	int NowEnvironment;		// 환경
	int NowFeeling;			// 기분

	int HopeTemperature;	// Complete 조건들
	int HopeFood;
	int HopeEnergy;
	int HopeEnvironment;
	int HopeFeeling;

	char* Hope;			// 별의 소망 문자열 저장
	bool bIsHope;		// 별의 소망이 이루어졌는지 확인 여부
	int Disease;		// 별이 걸린 질병을 나타낸다.

	bool bIsLife;		// 별의 에너지가 다 했는지 여부

	// 각각의 상태의 수치에 따라 시간이 지남에 따라
	// 직,간접적으로 영향을 미친다.
	TIMER TimeEnvironment;	
	TIMER TimeFood;
	TIMER TimeTemperature;
	TIMER TimeEnergy;
	TIMER TimeFeeling;

	TIMER TimeDisease;

	int VariableEnergy, VariableEnvironment, VariableFeeling, VariableFood, VariableTemperature, VariableDisease;

	// 오브젝트 관련
	bool bIsMudTemple;		// 흙의 신전
}STARINFO;

void StarInfoShow(HDC hdc);
void StarImageInit();

void Tutorial_Init(int nCount);
void Red_Star_Init(int nCount);
void Yellow_Star_Init(int nCount);
void Blue_Star_Init(int nCount);
void Pink_Star_Init(int nCount);
void Brown_Star_Init(int nCount);
void StarInfoInit();
void fnComplete(HDC hdc, STARINFO* StarInfo);

void StarInfoCheck();