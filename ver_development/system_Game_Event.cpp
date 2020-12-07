#include "StdAfx.h"

extern SURFACEINFO g_sfBack;

bool bIsEarthquake = false;

void fnTimeEvent(STARINFO* StarInfo){
	
	// 각각의 수치에 따라 딜레이 타임마다 다른 수치에 영향을 미친다.
	// 타이머가 설정되었다가 해제되었는데 다시 설정되어야할때 다시 설정이 되질 않는다.
	// 행복제조기 스페셜을 만들자.

	if(StarInfo->NowEnergy < 40){							// 에너지 수치가 40 미만이고
		if(StarInfo->VariableEnergy == 0){					// 에너지 관련 타이머가 초기화 되지 않았다면
			if(StarInfo->NowEnergy < 40){					// 수치값이 변함에 따라 초기화 되어야 한다.
				__tmInitTimer(&StarInfo->TimeEnergy, 120000);	// 
				StarInfo->VariableEnergy = 40;				// 그리고 초기화 되었다는 걸 알리기 위해 변경 수치 단계를 저장
			}
		}
		else if(__tmIsValidTimer(&StarInfo->TimeEnergy)){	// 타이머가 설정되어있고 타이머가 다 된다면 다시 초기화
			if(__tmIsElapseTimer(&StarInfo->TimeEnergy)){
				StarInfo->Feeling -= 2;
				StarInfo->Food -= 2;

				bIsEarthquake = true;
				
				__tmInitTimer(&StarInfo->TimeEnergy, 120000);	// 
			}
		}
	}
	else if(StarInfo->NowEnergy >= 40){	// 게임 진행 도중 스테이터스가 하락되어야할 수치가 아니라면 타이머를 없애버린다.
		__tmInitTimer(&StarInfo->TimeEnergy, 0);
		StarInfo->VariableEnergy = 0;			// 타이머가 해제되어있다는 걸 표시한다.
	}

	if(StarInfo->NowEnvironment > 60){								// 환경 수치가 수치인 60 이상이고
		if(StarInfo->VariableEnvironment == 0){						// 환경 관련 타이머가 초기화 되지 않았다면
			if(StarInfo->NowEnvironment > 60){						// 수치 값이 변함에 따라 초기화 되어야 한다.
				if(StarInfo->NowEnvironment > 80)
				__tmInitTimer(&StarInfo->TimeEnvironment, 120000);
				StarInfo->VariableEnvironment = 60;
			}
		}
		else if(__tmIsValidTimer(&StarInfo->TimeEnvironment)){	// 타이머가 설정되어있고 타이머가 다 된다면 다시 초기화
			if(__tmIsElapseTimer(&StarInfo->TimeEnvironment)){
				StarInfo->Energy += 2;
				StarInfo->Feeling += 2;

				__tmInitTimer(&StarInfo->TimeEnvironment, 120000);	// 
			}
		}
	}
	else if(StarInfo->NowEnvironment <= 60){	// 게인 진행 도중 환경 스테이터스가 증가되어야할 수치가 아니라면 타이머를 없애버린다.
		__tmInitTimer(&StarInfo->TimeEnvironment, 0);
		StarInfo->VariableEnvironment = 0;
	}

	if(StarInfo->NowFeeling < 40){							// 기분 수치가 40 미만이고
		if(StarInfo->VariableFeeling == 0){					// 기분 관련 타이머가 초기화 되지 않았다면
			if(StarInfo->NowFeeling < 40){					// 수치값이 변함에 따라 초기화 되어야 한다.
				__tmInitTimer(&StarInfo->TimeFeeling, 120000);	// 
				StarInfo->VariableFeeling = 40;				// 그리고 초기화 되었다는 걸 알리기 위해 변경 수치 단계를 저장
			}
		}
		else if(__tmIsValidTimer(&StarInfo->TimeFeeling)){	// 타이머가 설정되어있고 타이머가 다 된다면 다시 초기화
			if(__tmIsElapseTimer(&StarInfo->TimeFeeling)){
				StarInfo->Energy -= 2;
				StarInfo->Feeling -= 2;

				bIsEarthquake = true;
				
				__tmInitTimer(&StarInfo->TimeFeeling, 120000);	// 
			}
		}
	}
	else if(StarInfo->NowFeeling >= 40){	// 게임 진행 도중 기분 스테이터스가 하락되어야할 수치가 아니라면 타이머를 없애버린다.
		__tmInitTimer(&StarInfo->TimeFeeling, 0);
		StarInfo->VariableFeeling = 0;			// 타이머가 해제되어있다는 걸 표시한다.
	}

	if(StarInfo->NowFood < 40){								// 포만감 수치가 40 미만이고
		if(StarInfo->VariableFood == 0){					// 포만감 관련 타이머가 초기화 되지 않았다면
			if(StarInfo->NowFood < 40){						// 수치값이 변함에 따라 초기화 되어야 한다.
				__tmInitTimer(&StarInfo->TimeFood, 120000);	// 
				StarInfo->VariableFood = 40;				// 그리고 초기화 되었다는 걸 알리기 위해 변경 수치 단계를 저장
			}
		}
		else if(__tmIsValidTimer(&StarInfo->TimeFood)){	// 타이머가 설정되어있고 타이머가 다 된다면 다시 초기화
			if(__tmIsElapseTimer(&StarInfo->TimeFood)){
				StarInfo->Energy -= 2;
				StarInfo->Feeling -= 2;

				bIsEarthquake = true;
				
				__tmInitTimer(&StarInfo->TimeFood, 120000);	// 
			}
		}
	}
	else if(StarInfo->NowFood >= 40){	// 게임 진행 도중 포만감 스테이터스가 하락되어야할 수치가 아니라면 타이머를 없애버린다.
		__tmInitTimer(&StarInfo->TimeFood, 0);
		StarInfo->VariableFood = 0;			// 타이머가 해제되어있다는 걸 표시한다.
	}

	//if(__tmIsValidTimer(&StarInfo->TimeTemperature)){
	//	if(__tmIsElapseTimer(&StarInfo->TimeTemperature)){
	//	
	//	}
	//}

	if(__tmIsValidTimer(&StarInfo->TimeDisease)){
		if(__tmIsElapseTimer(&StarInfo->TimeDisease)){
			switch(StarInfo->Disease){
			case HEALTHY:
				StarInfo->Energy += 2;
				StarInfo->Feeling += 2;
				break;
			case COLD:
				StarInfo->Temperature += 2;
				StarInfo->Energy -= 2;
				StarInfo->Feeling -= 2;

				bIsEarthquake = true;
				break;
			case DEBILITY:
				StarInfo->Energy -= 3;
				StarInfo->Feeling -= 2;

				bIsEarthquake = true;
				break;
			case MELANCHOLY:
				StarInfo->Feeling -= 3;
				StarInfo->Energy -= 2;

				bIsEarthquake = true;
				break;
			case DIRTY:
				StarInfo->Environment -= 3;
				StarInfo->Feeling -= 1;

				bIsEarthquake = true;
				break;
			case OLD:
				StarInfo->Energy -= 3;
				StarInfo->Feeling -= 1;

				bIsEarthquake = true;
				break;
			case HUNGER:
				StarInfo->Food -= 3;
				StarInfo->Energy -= 2;

				bIsEarthquake = true;
				break;
			}
			//if(StarInfo->NowEnergy <= 0)
			//	StarInfo->NowEnergy = 0;
			//if(StarInfo->NowFeeling <= 0)
			//	StarInfo->NowFeeling = 0;
			//if(StarInfo->NowEnvironment <= 0)
			//	StarInfo->NowEnvironment = 0;
			//if(StarInfo->NowFood <= 0)
			//	StarInfo->NowFood = 0;

			__tmInitTimer(&StarInfo->TimeDisease, 90000);
		}
	}
}