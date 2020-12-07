#include "StdAfx.h"

extern SURFACEINFO g_sfBack;

bool bIsEarthquake = false;

void fnTimeEvent(STARINFO* StarInfo){
	
	// ������ ��ġ�� ���� ������ Ÿ�Ӹ��� �ٸ� ��ġ�� ������ ��ģ��.
	// Ÿ�̸Ӱ� �����Ǿ��ٰ� �����Ǿ��µ� �ٽ� �����Ǿ���Ҷ� �ٽ� ������ ���� �ʴ´�.
	// �ູ������ ������� ������.

	if(StarInfo->NowEnergy < 40){							// ������ ��ġ�� 40 �̸��̰�
		if(StarInfo->VariableEnergy == 0){					// ������ ���� Ÿ�̸Ӱ� �ʱ�ȭ ���� �ʾҴٸ�
			if(StarInfo->NowEnergy < 40){					// ��ġ���� ���Կ� ���� �ʱ�ȭ �Ǿ�� �Ѵ�.
				__tmInitTimer(&StarInfo->TimeEnergy, 120000);	// 
				StarInfo->VariableEnergy = 40;				// �׸��� �ʱ�ȭ �Ǿ��ٴ� �� �˸��� ���� ���� ��ġ �ܰ踦 ����
			}
		}
		else if(__tmIsValidTimer(&StarInfo->TimeEnergy)){	// Ÿ�̸Ӱ� �����Ǿ��ְ� Ÿ�̸Ӱ� �� �ȴٸ� �ٽ� �ʱ�ȭ
			if(__tmIsElapseTimer(&StarInfo->TimeEnergy)){
				StarInfo->Feeling -= 2;
				StarInfo->Food -= 2;

				bIsEarthquake = true;
				
				__tmInitTimer(&StarInfo->TimeEnergy, 120000);	// 
			}
		}
	}
	else if(StarInfo->NowEnergy >= 40){	// ���� ���� ���� �������ͽ��� �϶��Ǿ���� ��ġ�� �ƴ϶�� Ÿ�̸Ӹ� ���ֹ�����.
		__tmInitTimer(&StarInfo->TimeEnergy, 0);
		StarInfo->VariableEnergy = 0;			// Ÿ�̸Ӱ� �����Ǿ��ִٴ� �� ǥ���Ѵ�.
	}

	if(StarInfo->NowEnvironment > 60){								// ȯ�� ��ġ�� ��ġ�� 60 �̻��̰�
		if(StarInfo->VariableEnvironment == 0){						// ȯ�� ���� Ÿ�̸Ӱ� �ʱ�ȭ ���� �ʾҴٸ�
			if(StarInfo->NowEnvironment > 60){						// ��ġ ���� ���Կ� ���� �ʱ�ȭ �Ǿ�� �Ѵ�.
				if(StarInfo->NowEnvironment > 80)
				__tmInitTimer(&StarInfo->TimeEnvironment, 120000);
				StarInfo->VariableEnvironment = 60;
			}
		}
		else if(__tmIsValidTimer(&StarInfo->TimeEnvironment)){	// Ÿ�̸Ӱ� �����Ǿ��ְ� Ÿ�̸Ӱ� �� �ȴٸ� �ٽ� �ʱ�ȭ
			if(__tmIsElapseTimer(&StarInfo->TimeEnvironment)){
				StarInfo->Energy += 2;
				StarInfo->Feeling += 2;

				__tmInitTimer(&StarInfo->TimeEnvironment, 120000);	// 
			}
		}
	}
	else if(StarInfo->NowEnvironment <= 60){	// ���� ���� ���� ȯ�� �������ͽ��� �����Ǿ���� ��ġ�� �ƴ϶�� Ÿ�̸Ӹ� ���ֹ�����.
		__tmInitTimer(&StarInfo->TimeEnvironment, 0);
		StarInfo->VariableEnvironment = 0;
	}

	if(StarInfo->NowFeeling < 40){							// ��� ��ġ�� 40 �̸��̰�
		if(StarInfo->VariableFeeling == 0){					// ��� ���� Ÿ�̸Ӱ� �ʱ�ȭ ���� �ʾҴٸ�
			if(StarInfo->NowFeeling < 40){					// ��ġ���� ���Կ� ���� �ʱ�ȭ �Ǿ�� �Ѵ�.
				__tmInitTimer(&StarInfo->TimeFeeling, 120000);	// 
				StarInfo->VariableFeeling = 40;				// �׸��� �ʱ�ȭ �Ǿ��ٴ� �� �˸��� ���� ���� ��ġ �ܰ踦 ����
			}
		}
		else if(__tmIsValidTimer(&StarInfo->TimeFeeling)){	// Ÿ�̸Ӱ� �����Ǿ��ְ� Ÿ�̸Ӱ� �� �ȴٸ� �ٽ� �ʱ�ȭ
			if(__tmIsElapseTimer(&StarInfo->TimeFeeling)){
				StarInfo->Energy -= 2;
				StarInfo->Feeling -= 2;

				bIsEarthquake = true;
				
				__tmInitTimer(&StarInfo->TimeFeeling, 120000);	// 
			}
		}
	}
	else if(StarInfo->NowFeeling >= 40){	// ���� ���� ���� ��� �������ͽ��� �϶��Ǿ���� ��ġ�� �ƴ϶�� Ÿ�̸Ӹ� ���ֹ�����.
		__tmInitTimer(&StarInfo->TimeFeeling, 0);
		StarInfo->VariableFeeling = 0;			// Ÿ�̸Ӱ� �����Ǿ��ִٴ� �� ǥ���Ѵ�.
	}

	if(StarInfo->NowFood < 40){								// ������ ��ġ�� 40 �̸��̰�
		if(StarInfo->VariableFood == 0){					// ������ ���� Ÿ�̸Ӱ� �ʱ�ȭ ���� �ʾҴٸ�
			if(StarInfo->NowFood < 40){						// ��ġ���� ���Կ� ���� �ʱ�ȭ �Ǿ�� �Ѵ�.
				__tmInitTimer(&StarInfo->TimeFood, 120000);	// 
				StarInfo->VariableFood = 40;				// �׸��� �ʱ�ȭ �Ǿ��ٴ� �� �˸��� ���� ���� ��ġ �ܰ踦 ����
			}
		}
		else if(__tmIsValidTimer(&StarInfo->TimeFood)){	// Ÿ�̸Ӱ� �����Ǿ��ְ� Ÿ�̸Ӱ� �� �ȴٸ� �ٽ� �ʱ�ȭ
			if(__tmIsElapseTimer(&StarInfo->TimeFood)){
				StarInfo->Energy -= 2;
				StarInfo->Feeling -= 2;

				bIsEarthquake = true;
				
				__tmInitTimer(&StarInfo->TimeFood, 120000);	// 
			}
		}
	}
	else if(StarInfo->NowFood >= 40){	// ���� ���� ���� ������ �������ͽ��� �϶��Ǿ���� ��ġ�� �ƴ϶�� Ÿ�̸Ӹ� ���ֹ�����.
		__tmInitTimer(&StarInfo->TimeFood, 0);
		StarInfo->VariableFood = 0;			// Ÿ�̸Ӱ� �����Ǿ��ִٴ� �� ǥ���Ѵ�.
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