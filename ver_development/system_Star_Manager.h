#pragma once

#include "TIMER.h"

#define MAX_STAR 6
#define FIRST 102
#define SECOND 103
#define SKY 104

typedef enum StarStep {TUTORIAL = 0, RED_STAR, YELLOW_STAR, BLUE_STAR, PINK_STAR, BROWN_STAR} STARSTEP;

typedef struct StarData_tag{
	// ��ǥ ����
	int nX;					// �������� ȭ�鿡���� XY ��ǥ
	int nY;					// 
	int r;					// ���� ������

	// ���� ���� ����
	int nCountMud;			// �� ������ ������ �ִ� ����
	int Attribute;			// ���� ���ϴ� �Ӽ�
	int Temperature;		// ���� ü��
	int Food;				// �ʱ� ������
	int Energy;				// �ʱ� ������
	int Environment;		// �ʱ� ȯ��
	int Feeling;			// �ʱ� ���

	int NowTemperature;		// ���� �µ�
	int NowFood;			// �ķ�
	int NowEnergy;			// ���
	int NowEnvironment;		// ȯ��
	int NowFeeling;			// ���

	int HopeTemperature;	// Complete ���ǵ�
	int HopeFood;
	int HopeEnergy;
	int HopeEnvironment;
	int HopeFeeling;

	char* Hope;			// ���� �Ҹ� ���ڿ� ����
	bool bIsHope;		// ���� �Ҹ��� �̷�������� Ȯ�� ����
	int Disease;		// ���� �ɸ� ������ ��Ÿ����.

	bool bIsLife;		// ���� �������� �� �ߴ��� ����

	// ������ ������ ��ġ�� ���� �ð��� ������ ����
	// ��,���������� ������ ��ģ��.
	TIMER TimeEnvironment;	
	TIMER TimeFood;
	TIMER TimeTemperature;
	TIMER TimeEnergy;
	TIMER TimeFeeling;

	TIMER TimeDisease;

	int VariableEnergy, VariableEnvironment, VariableFeeling, VariableFood, VariableTemperature, VariableDisease;

	// ������Ʈ ����
	bool bIsMudTemple;		// ���� ����
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