#pragma once

typedef struct COLLISIONINFOtag
{
	int		nObjIdx;	// 게임 오브젝트 인덱스
	RECT	rcCollsion;	// 충돌사각영역
} COLLISIONINFO;

void __ccCheckCollisionMousePointer(POINTS MousePoint, int Attribute);
bool __ccCheckCollisionFocus(GOBJFOCUS_INFO* __fo_ObjInfo, int Attribute, int StarStep);
void __ccCheckCollisionDestroy(POINTS MousePoint);
//void __cdDetectCollisionNN(int nObjGroup1, int nObjGroup2, int nDamageObj, int nDamageObjGroup);

void __cdGetObjCollisionInfo(int Attribute, COLLISIONINFO* pCInfo);
static int  __cdGetObjCollisionInfoCont(int Attribute, COLLISIONINFO* pCInfo, int nFrom);
static BOOL __cdIsCollide(RECT* prcObj1, RECT* prcObj2);
