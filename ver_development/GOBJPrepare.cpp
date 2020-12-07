#include "Stdafx.h"

extern STARSTEP StarStep;
extern STARINFO StarInfo[MAX_STAR];

extern int nObject_ID;

extern SURFACEINFO g_sfBack;

// 오브젝트 생성 함수
void __stDispatchObject(int ObjectID, float theta)
{
	if (!__goGetObjectList()) return;

	nObject_ID = ObjectID;
	//// 
	switch(nObject_ID){
	case ID_MUD : __goPrepareObject("Mud", 0, SECOND, MUD, theta); break;
	case ID_MUDTREE_MUD : __goPrepareObject("Mud", 0, SECOND, MUDTREE_MUD, theta); break;
	case ID_TREES_0 : __goPrepareObject("Tree0", 1, SECOND, TREES, theta); break;
	case ID_TREES_1 : __goPrepareObject("Tree1", 3, SECOND, TREES, theta); break;
	case ID_TREES_2 : __goPrepareObject("Tree2", 5, SECOND, TREES, theta); break;
	case ID_FIRE_0 : __goPrepareObject("Fire0", 1, SECOND, FIRE, theta); break;
	case ID_FIRE_1 : __goPrepareObject("Fire1", 3, SECOND, FIRE, theta); break;
	case ID_FIRE_2 : __goPrepareObject("Fire2", 5, SECOND,  FIRE, theta); break;
	case ID_CLOUD_1 : __goPrepareObject("Cloud1", 1, SKY, CLOUD, theta); break;
	case ID_CLOUD_2 : __goPrepareObject("Cloud2", 3, SKY, CLOUD, theta); break;
	case ID_WATER_0 :__goPrepareObject("Water0", 1, SECOND, WATER, theta); break;
	case ID_WATER_1 : __goPrepareObject("Water1", 3, SECOND, WATER, theta); break;
	case ID_WATER_2 : __goPrepareObject("Water2", 3, FIRST, WATER, theta); break;
	case ID_WATER_3 : __goPrepareObject("Water3", 5, SECOND, WATER, theta); break;
	case ID_FOOD_0 : __goPrepareObject("Food0", 1, SECOND, FOOD, theta);   break;
	case ID_FOOD_1 : __goPrepareObject("Food1", 3, SECOND, FOOD, theta); break;
	case ID_FOOD_2 : __goPrepareObject("Food2", 5, SECOND, FOOD, theta); break;
	case ID_FOOD_3 : __goPrepareObject("Food3", 3, FIRST, FOOD, theta);   break;
	case ID_WIND_1 : __goPrepareObject("Wind1", 10, SECOND, WIND, theta); break;
	case ID_MUDTEMPLE : __goPrepareObject("MudTemple", 20, SECOND, MUDTEMPLE, theta); break;
	case ID_CHEESESLATE : __goPrepareObject("CheeseSlate", 20, SECOND, CHEESESLATE, theta); break;
	case ID_MUDTREE : __goPrepareObject("MudTree", 3, SECOND, MUDTREE, theta); break;
	}
}

// 포커스 오브젝트 생성 함수
void __stDispatchFocusObject(int ObjectID, float theta)
{
	if (!__goGetObjectList()) return;

	//// 
	switch(ObjectID){
	case ID_TREES_0 : __goPrepareFocus("Tree0", 1, SECOND, TREES, theta); break;
	case ID_TREES_1 : __goPrepareFocus("Tree1", 3, SECOND, TREES, theta); break;
	case ID_TREES_2 : __goPrepareFocus("Tree2", 5, SECOND, TREES, theta); break;
	case ID_FIRE_0 : __goPrepareFocus("Fire0", 1, SECOND, FIRE, theta); break;
	case ID_FIRE_1 : __goPrepareFocus("Fire1", 3, SECOND, FIRE, theta);   break;
	case ID_FIRE_2 : __goPrepareFocus("Fire2", 5, SECOND, FIRE, theta);   break;
	case ID_CLOUD_1 : __goPrepareFocus("Cloud1", 1, SKY, CLOUD, theta);   break;
	case ID_CLOUD_2 : __goPrepareFocus("Cloud2", 3, SKY, CLOUD, theta); break;
	case ID_WATER_0 :__goPrepareFocus("Water0", 1, SECOND, WATER, theta); break;
	case ID_WATER_1 : __goPrepareFocus("Water1", 3, SECOND, WATER, theta);   break;
	case ID_WATER_2 : __goPrepareFocus("Water2", 3, FIRST, WATER, theta);   break;
	case ID_WATER_3 : __goPrepareFocus("Water3", 5, SECOND, WATER, theta); break;
	case ID_FOOD_0 : __goPrepareFocus("Food0", 1, SECOND, FOOD, theta);   break;
	case ID_FOOD_1 : __goPrepareFocus("Food1", 3, SECOND, FOOD, theta);   break;
	case ID_FOOD_2 : __goPrepareFocus("Food2", 5, SECOND, FOOD, theta);   break;
	case ID_FOOD_3 : __goPrepareFocus("Food3", 3, FIRST, FOOD, theta);   break;
	case ID_WIND_1 : __goPrepareFocus("Wind1", 10, SECOND, WIND, theta);	break;
	case ID_MUDTEMPLE : __goPrepareFocus("MudTemple", 20, SECOND, MUDTEMPLE, theta);   break;
	case ID_CHEESESLATE : __goPrepareFocus("CheeseSlate", 20, SECOND, CHEESESLATE, theta); break;
	case ID_MUDTREE : __goPrepareFocus("MudTree", 3, SECOND, MUDTREE, theta); break;
	default : return;
	}
}

// 포커스 오브젝트 생성 및 초기화
void __goPrepareFocus(char* pStr, int MudStep, int nPositionStep, int Attribute, float theta)
{
	void* pGObjInfo;
	int nIdx;
	char cAniFile[255];
	int nVariable = 0;

	strcpy(cAniFile, PATH);			// 파일 경로를 파일 이름에 따라 동적으로 생성하고 있다.
	strcat(cAniFile, pStr);
	strcat(cAniFile, EXT_ANI);

	pGObjInfo = __GObjFocusCreate(sizeof(GOBJFOCUS_INFO));
	nIdx = __goCreate(pGObjInfo, NULL, FOCUS, MudStep, nPositionStep, StarStep, __GObjFocusSetMember, __GObjFocusGetMember,		// 포커스 오브젝트를 생성하고 인덱스를 얻는다.
						 __GObjFocusRelease, __GObjFocusAction, __GObjFocusGetRect, __GObjFocusRender, __GObjFocusInit);
	if (nIdx == GO_EMPTY)															// 생성 실패라면
	{
		free(pGObjInfo);															// 메모리 해제
		return;
	}
	__GObjFocusSetMember(pGObjInfo);												// 메모리 적재

	// 오브젝트의 위치에 따라 초기화 위치(생성 위치)를 달리해 준다.
	switch(nObject_ID){
		case ID_TREES_0 : nVariable = -22; break;
		case ID_TREES_1 : nVariable = 0; break;
		case ID_TREES_2 : nVariable = 0; break;
		case ID_FIRE_0 : nVariable = 0; break;
		case ID_FIRE_1 : nVariable = -10; break;
		case ID_FIRE_2 : nVariable = -20; break;
		case ID_CLOUD_1 : nVariable = 130; break;
		case ID_CLOUD_2 : nVariable = 130; break;
		case ID_WATER_0 : nVariable = -10; break;
		case ID_WATER_1 : nVariable = -20; break;
		case ID_WATER_2 : nVariable = -80; break;
		case ID_WATER_3 : nVariable = 0; break;
		case ID_FOOD_0 : nVariable = 0; break;
		case ID_FOOD_1 : nVariable = 0; break;
		case ID_FOOD_2 : nVariable = -20; break;
		case ID_FOOD_3 : nVariable = -90; break;
		case ID_WIND_1 : nVariable = 40; break;
		case ID_MUDTEMPLE : nVariable = -10; break;
		case ID_CHEESESLATE : nVariable = -25; break;
		case ID_MUDTREE : nVariable = 0; break;
	}	

	__GObjFocusInit(nIdx, Attribute, nPositionStep, SCR_WIDTH/2, StarInfo[StarStep].r + nVariable, theta, cAniFile,g_sfBack.dcSurface);
}

// 오브젝트 메모리 생성 및 초기화
// 변경된 데이터로 렌더 함수에 의해 그려지게 된다.
void __goPrepareObject(char* pStr, int MudStep, int nPositionStep, int Attribute, float theta)
{
	void* pGObjInfo;
	int nIdx;
	char cAniFile[255];
	int nVariable = 0;

	strcpy(cAniFile, PATH);			// 파일 경로를 파일 이름에 따라 동적으로 생성하고 있다.
	strcat(cAniFile, pStr);
	strcat(cAniFile, EXT_ANI);


	pGObjInfo = __GObjObjectCreate(sizeof(GOBJOBJECT_INFO));						
	nIdx = __goCreate(pGObjInfo, pStr, Attribute, MudStep, nPositionStep, StarStep, __GObjObjectSetMember, __GObjObjectGetMember,
					__GObjObjectRelease, __GObjObjectAction, __GObjObjectGetRect, __GObjObjectRender, __GObjObjectInit);
	if (nIdx == GO_EMPTY)
	{
		free(pGObjInfo);
		return;
	}
	__GObjObjectSetMember(pGObjInfo);

	// 오브젝트의 위치에 따라 초기화 위치(생성 위치)를 달리해 준다.
	switch(nObject_ID){
		case ID_MUD : nVariable =  -30; break;
		case ID_MUDTREE_MUD : nVariable =  27; break;
		case ID_TREES_0 : nVariable = -22; break;
		case ID_TREES_1 : nVariable = 0; break;
		case ID_TREES_2 : nVariable = 0; break;
		case ID_FIRE_0 : nVariable = 0; break;
		case ID_FIRE_1 : nVariable = -10; break;
		case ID_FIRE_2 : nVariable = -20; break;
		case ID_CLOUD_1 : nVariable = 130; break;
		case ID_CLOUD_2 : nVariable = 130; break;
		case ID_WATER_0 : nVariable = -10; break;
		case ID_WATER_1 : nVariable = -20; break;
		case ID_WATER_2 : nVariable = -80; break;
		case ID_WATER_3 : nVariable = 0; break;
		case ID_FOOD_0 : nVariable = 0; break;
		case ID_FOOD_1 : nVariable = 0; break;
		case ID_FOOD_2 : nVariable = -20; break;
		case ID_FOOD_3 : nVariable = -90; break;
		case ID_WIND_1 : nVariable = 40; break;
		case ID_MUDTEMPLE : nVariable = -10; break;
		case ID_CHEESESLATE : nVariable = -25; break;
		case ID_MUDTREE : nVariable = 0; break;
	}	

	__GObjObjectInit(nIdx, Attribute, MudStep, nPositionStep, SCR_WIDTH/2, StarInfo[StarStep].r + nVariable, theta, cAniFile,g_sfBack.dcSurface);

	if(nObject_ID != ID_MUD) nObject_ID = 0;										// 생성 후 ID 초기화
}

















