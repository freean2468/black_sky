#include "Stdafx.h"

void Sound_GamePlayOn(){
	PlaySound("./Sound/Game_Play.wav",0,SND_ASYNC|SND_LOOP);
}

void Sound_StageOn(){
	PlaySound("./Sound/Stage_Select.wav",0,SND_ASYNC|SND_LOOP);
}

//void Sound_StageOn(){
//	PlaySound("./Sound/Nature.wav",0,SND_ASYNC|SND_LOOP);
//}

void Sound_EarthquakeOn(){
	PlaySound("./Sound/Earthquake.wav",0,SND_ASYNC|SND_LOOP);
}

void Sound_BigbangOn(){
	PlaySound("./Sound/Bigbang.wav",0,SND_ASYNC);
}

void Sound_CompleteOn(){
	PlaySound("./Sound/Complete.wav",0,SND_ASYNC);
}

void Sound_MenuPlayOn(){
	
}

void Sound_LogoOn(){
	PlaySound("./Sound/Logo.wav",0,SND_ASYNC);
}

void Sound_EffectOn(){
	PlaySoundEffect();
}


void Sound_Effect_CreateOn(){
	PlaySoundEffectCreate();
}