#pragma once

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
#include <windows.h>
#include <iostream>


///////////////////////// 사용자 정의 함수
/////////////////////////

// 윈도우
#include "Main.h"

// 그래픽
#include "GraphicEngine.h"
#include "effect.h"
#include "Surface.h"

// 시스템
#include "system_Game_Play.h"
#include "system_Main_Menu.h"
#include "system_Game_Stage.h"
#include "system_Star_Manager.h"
#include "system_Game_UI.h"
#include "system_Game_Event.h"

//EyeCatch 관련
#include "EyeCatch.h"

// 질병 ID
#include "Info_DiseaseID.h"

// 메시지 루프
#include "MainProc.h"

// 타이머
#include "Timer.h"
#include "TypeDef.h"

// 알고리즘
#include "Matrix.h"
#include "SLinkedList.h"

// 애니메이션
#include "AnimationFile.h"
#include "AnimationInfo.h"
#include "Animation.h"

// 오브젝트
#include "GOBJFocus.h"
#include "GObject.h"
#include "GOBJPrepare.h"
#include "GOBJObject.h"

// 오브젝트 ID
#include "ObjectID.h"

// 오브젝트 속성
#include "ObjectAttribute.h"

// 충돌체크
#include "CollisionCheck.h"

// 사운드
#include "Sound.h"

// 텍스트
#include "Text.h"

// 리소스
#include "Resource.h"
#include "resource1.h"


#define PI          3.141592654  

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// SDL
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#pragma comment(lib, "sdl.lib")
#pragma comment(lib, "sdlmain.lib")
#pragma comment(lib, "sdl_mixer.lib")
#pragma comment(lib, "sdl_ttf.lib")

#include "SDL_Base.h"
