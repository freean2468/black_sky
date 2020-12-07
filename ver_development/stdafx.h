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


///////////////////////// ����� ���� �Լ�
/////////////////////////

// ������
#include "Main.h"

// �׷���
#include "GraphicEngine.h"
#include "effect.h"
#include "Surface.h"

// �ý���
#include "system_Game_Play.h"
#include "system_Main_Menu.h"
#include "system_Game_Stage.h"
#include "system_Star_Manager.h"
#include "system_Game_UI.h"
#include "system_Game_Event.h"

//EyeCatch ����
#include "EyeCatch.h"

// ���� ID
#include "Info_DiseaseID.h"

// �޽��� ����
#include "MainProc.h"

// Ÿ�̸�
#include "Timer.h"
#include "TypeDef.h"

// �˰���
#include "Matrix.h"
#include "SLinkedList.h"

// �ִϸ��̼�
#include "AnimationFile.h"
#include "AnimationInfo.h"
#include "Animation.h"

// ������Ʈ
#include "GOBJFocus.h"
#include "GObject.h"
#include "GOBJPrepare.h"
#include "GOBJObject.h"

// ������Ʈ ID
#include "ObjectID.h"

// ������Ʈ �Ӽ�
#include "ObjectAttribute.h"

// �浹üũ
#include "CollisionCheck.h"

// ����
#include "Sound.h"

// �ؽ�Ʈ
#include "Text.h"

// ���ҽ�
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
