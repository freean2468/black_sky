#include "StdAfx.h"

extern MSG msg;
extern HINSTANCE hInst;
extern HCURSOR hNowCursor;

extern SURFACEINFO g_sfBack;

extern HWND g_hWnd;

extern STARINFO StarInfo[MAX_STAR];
extern STARSTEP StarStep;

extern UISTATE UiState;

//// 마우스 포인터와 찰흙
void __ccCheckCollisionMousePointer(POINTS MousePoint, int Attribute)
{
	GOBJECT* pObjList = __goGetObjectList();
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		void* pObjObjectInfo;
		RECT rcObject;
		RECT rcMousePoint;

		rcMousePoint.top = MousePoint.y-30;				// 오차 범위 포함
		rcMousePoint.left = MousePoint.x-30;
		rcMousePoint.bottom = MousePoint.y+30;
		rcMousePoint.right = MousePoint.x+30;

		// 마우스 포인터 영역과 진흙 충돌 체크	
		for (int ei=0; ei < nObjCount; ei++)
		{
			if (pObjList[ei].nIdx != GO_EMPTY) 
			{
				if (pObjList[ei].Attribute == Attribute)
				{
					//
					pObjObjectInfo = pObjList[ei].pObjInfo;
					pObjList[ei].fnSetMember(pObjObjectInfo);
					rcObject = pObjList[ei].fnGetRect();
					pObjList[ei].fnGetMember(pObjObjectInfo);		// 의미없음

					//HDC hdc;

					//hdc = GetDC(g_hWnd);

					//Rectangle(g_sfBack.dcSurface, rcObject.left, rcObject.top, rcObject.right, rcObject.bottom);

					//Rectangle(g_sfBack.dcSurface, rcMousePoint.left, rcMousePoint.top, rcMousePoint.right, rcMousePoint.bottom);

					//ReleaseDC(g_hWnd, hdc);

					//RenewDC();

					//
					if (__cdIsCollide(&rcMousePoint, &rcObject))
					{
						hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR2));

						if(msg.message == WM_LBUTTONDOWN){
							Sound_EffectOn();
							StarInfo[StarStep].nCountMud++;
							pObjList[ei].fnRelease;
							__goRemoveAt(pObjList[ei].nIdx);						// 호출 순서 주의
							
							msg.message = NULL;

							hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)); // 윈도우 커서 초기화

							break;
						}
						break;
					}
					else if(UiState == DESTROY){
						hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR3)); // 윈도우 커서 초기화
					}
					else{
						hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)); // 윈도우 커서 초기화
					}
				}
			}
		}
	}
}

//// 마우스 포인터와 오브젝트
void __ccCheckCollisionDestroy(POINTS MousePoint)
{
	GOBJECT* pObjList = __goGetObjectList();
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		void* pObjObjectInfo;
		RECT rcObject;
		RECT rcMousePoint;

		rcMousePoint.top = MousePoint.y-20;				// 오차 범위 포함
		rcMousePoint.left = MousePoint.x-20;
		rcMousePoint.bottom = MousePoint.y+20;
		rcMousePoint.right = MousePoint.x+20;

		// 마우스 포인터 영역과 진흙을 제외한 오브젝트 충돌 체크	
		for (int ei=0; ei < nObjCount; ei++)
		{
			if (pObjList[ei].nIdx != GO_EMPTY) 
			{
				if(StarStep == pObjList[ei].StarStep){
					if (pObjList[ei].Attribute != MUD && pObjList[ei].Attribute != FOCUS)
					{
						//
						pObjObjectInfo = pObjList[ei].pObjInfo;
						pObjList[ei].fnSetMember(pObjObjectInfo);
						rcObject = pObjList[ei].fnGetRect();
						pObjList[ei].fnGetMember(pObjObjectInfo);		// 의미없음

						//HDC hdc;

						//hdc = GetDC(g_hWnd);

						//Rectangle(g_sfBack.dcSurface, rcObject.left, rcObject.top, rcObject.right, rcObject.bottom);

						//Rectangle(g_sfBack.dcSurface, rcMousePoint.left, rcMousePoint.top, rcMousePoint.right, rcMousePoint.bottom);

						//ReleaseDC(g_hWnd, hdc);

						//RenewDC();

						//
						if (__cdIsCollide(&rcMousePoint, &rcObject))
						{
							if(msg.message == WM_LBUTTONDOWN){
								Sound_EffectOn();
								GOBJOBJECT_INFO* ObjInfo = (GOBJOBJECT_INFO*)__GObjObjectGetObjInfo();							

								if(pObjList[ei].Attribute == MUDTEMPLE)	// 찰흙신전을 파괴한다면
									StarInfo[StarStep].bIsMudTemple = false;

								StarInfo[StarStep].nCountMud += ObjInfo->MudStep;

								pObjList[ei].fnRelease;
								__goRemoveAt(pObjList[ei].nIdx);						// 호출 순서 주의
								
								msg.message = NULL;

								hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)); // 윈도우 커서 초기화

								break;
							}
							break;
						}
					}
				}
			}
		}
	}
}

//// 포커스 오브젝트와 생성 오브젝트 충돌 체크
bool __ccCheckCollisionFocus(GOBJFOCUS_INFO* __fo_ObjInfo, int Attribute, int StarStep)
{
	GOBJECT* pObjList = __goGetObjectList();
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		COLLISIONINFO cInfoObj;
		void* pObjObjectInfo;
		RECT rcObject;

		////
		__cdGetObjCollisionInfo(Attribute, &cInfoObj);				// 충돌 주체를 찾아 내고 cInfoObj에 그 정보를 얻어 낸다.

		//if(__fo_ObjInfo->nPositionStep == SKY){				// 하늘
		//	cInfoObj.rcCollsion.top-=55;
		//	cInfoObj.rcCollsion.bottom-=70;
		//}
		//else if(__fo_ObjInfo->nPositionStep == FIRST){		// 지하
		//	cInfoObj.rcCollsion.top+=40;
		//	cInfoObj.rcCollsion.bottom-=40;
		//}
		//else if(__fo_ObjInfo->nPositionStep == SECOND){		// 지상
		//	cInfoObj.rcCollsion.top+= 50;
		//	cInfoObj.rcCollsion.bottom -= 30;
		//}
		//else if(__fo_ObjInfo->nPositionStep == CENTER){		// 중앙
		//
		//}

		// 포커스 오브젝트와 생성 오브젝트 간의 충돌 검출	
		for (int ei=0; ei < nObjCount; ei++)
		{
 			if (pObjList[ei].nIdx != GO_EMPTY) 
			{
				if(pObjList[ei].StarStep == StarStep){	// 같은 별일 떄만 체크
					if(pObjList[ei].nPositionStep == __fo_ObjInfo->nPositionStep && __fo_ObjInfo->Attribute != WIND){	// 같은 층일 떄만 체크 (다른 층은 체크하지 않는다.
						if(pObjList[ei].Attribute != Attribute && pObjList[ei].Attribute != MUD){
							pObjObjectInfo = pObjList[ei].pObjInfo;
							pObjList[ei].fnSetMember(pObjObjectInfo);
							rcObject = pObjList[ei].fnGetRect();
							pObjList[ei].fnGetMember(pObjObjectInfo);		// 의미없음		

							//충돌체크가 완벽히 되므로 필요 없다.
							// 상하 영역을 바꾸어 주는 이유는 게임상 맵을 상하로 네단계로 구분하기 떄문이다.
							// 이 다른 구역에 있는 오브젝트들은 서로와 충돌체크가 되어서는 안 된다.
 							//rcObject.right-=25;
							//rcObject.left+=25;
							//if(pObjList[ei].nPositionStep == SKY){				// 하늘
							//	rcObject.top-=55;
							//	rcObject.bottom-=70;
							//}
							//else if(pObjList[ei].nPositionStep == FIRST){		// 지하
							//	rcObject.top+=40;
							//	rcObject.bottom-=40;
							//}
							//else if(pObjList[ei].nPositionStep == SECOND){		// 지상
							//	rcObject.top+= 50;
							//	rcObject.bottom -= 30;
							//}
							//else if(pObjList[ei].nPositionStep == CENTER){		// 중앙
							//
							//}

							/*HDC hdc;

							hdc = GetDC(g_hWnd);

							Rectangle(g_sfBack.dcSurface, rcObject.left, rcObject.top, rcObject.right, rcObject.bottom);

							Rectangle(g_sfBack.dcSurface, cInfoObj.rcCollsion.left, cInfoObj.rcCollsion.top, cInfoObj.rcCollsion.right, cInfoObj.rcCollsion.bottom);

							ReleaseDC(g_hWnd, hdc);*/

							
							if (__cdIsCollide(&cInfoObj.rcCollsion, &rcObject))	
 								return true;	// 충돌하므로 지을 수 없음
						}
					}
				}
			}
		}
	}
   	return false;		// 충돌하지 않음
}

//
//// n:n
//void __cdDetectCollisionNN(int nObjGroup1, int nObjGroup2, int nDamageObj, int nDamageObjGroup)
//{
//	GOBJECT* pObjList = __goGetObjectList();
//	if (pObjList)
//	{
//		int nObjCount = __goGetObjectMaxCount();
//		COLLISIONINFO cInfoObj;
//		void* pObjEnemyInfo;
//		RECT rcEnemy;
//		int nFrom=0;
//
//		////
//		while(1)
//		{
//			////
//			nFrom = __cdGetObjCollisionInfoCont(nObjGroup1, &cInfoObj, nFrom);
//			// nFrom의 유효범위는 0 ~ nObjCount-1까지 (인덱스), 마지막 인덱스이거나 없으면
//			if (nFrom >= nObjCount-1 || nFrom == GO_EMPTY) break;
//
//			// 주인공 기체와 적 기체 또는 적 총알과의 충돌 검출	
//			for (int ei=0; ei < nObjCount; ei++)
//			{
//				if (pObjList[ei].nIdx != GO_EMPTY) 
//				{
//					if (pObjList[ei].nCGroup == nObjGroup2)
//					{
//						//
//						pObjEnemyInfo = pObjList[ei].pObjInfo;
//						pObjList[ei].fnSetMember(pObjEnemyInfo);
//						rcEnemy = pObjList[ei].fnGetRect();
//						pObjList[ei].fnGetMember(pObjEnemyInfo);		// 의미없음
//
//						//
//						if (__cdIsCollide(&cInfoObj.rcCollsion, &rcEnemy))
//						{
//							// 데미지 처리
//							pObjList[cInfoObj.nObjIdx].fnDamage(nDamageObj);
//							pObjList[ei].fnDamage(nDamageObjGroup);
//							break;
//						}
//					}
//				}
//			}
//		}
//	}
//}

// 게임 오브젝트 리스트에서 충돌 주체가 되는 게임 오브젝트를 찾아 내어 COLLISIONINFO를 얻어내는 함수
// nObject를 찾아 오브젝트 리스트에서 인덱스와 충돌사각영역 구함
// 매개 변수 pCinfo는 아웃 파라미터
static void __cdGetObjCollisionInfo(int Attribute, COLLISIONINFO* pCInfo)
{
	GOBJECT* pObjList = __goGetObjectList();				// 게임 오브젝트 리스트의 선두 주소를 얻음
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		void* pObjInfo;		

		// 포커스 오브젝트의 사각영역을 구함
		for (int mi=0; mi < nObjCount; mi++)				// 게임 오브젝트 리스트를 차례로 뒤져서 게임 오브젝트를 찾아내면
		{													// 더 이상 검사하지 않고 구조체 pCInfo의 데이터 멤버들을 채워 반환한다.
			if (pObjList[mi].nIdx != GO_EMPTY)				// 게임 오브젝트 리스트에서 사용되고 있는 배열만 검사한다.
			{
				if (pObjList[mi].Attribute == Attribute)			// 충돌 그룹이 찾는 게임 오브젝트 이면 if문 진입
				{
					pObjInfo = pObjList[mi].pObjInfo;
					pObjList[mi].fnSetMember(pObjInfo);
					pCInfo->nObjIdx = mi;
 					pCInfo->rcCollsion = pObjList[mi].fnGetRect();
					pObjList[mi].fnGetMember(pObjInfo);		// 의미없음				 
					return;
				}
			}
		}

		pCInfo->nObjIdx = GO_EMPTY;	// 찾지 못하면
	}
}

// 복수의 충돌 주체의 COLLISIONINFO를 얻어 내는 함수
// 이 함수는 __cdGetObjCollisionInfo 함수와 비슷하지만, 함수가 종료될 때에
// 검색하여 찾은 게임 오브젝트의 다음 인덱스를 반환한다는 점이 다르다. 다시 이 함수가 호출될 때에는
// 반환받은 인덱스부터 다시 검색한다. 왜냐 하면, 충돌 주체가 복수이기 때문이다.
//// nObject를 찾아 오브젝트 리스트에서 인덱스와 충돌사각영역 구함
// 매개변수 nFrom은 검색을 시작할 게임 오브젝트의 인덱스가 넘어온다.
static int __cdGetObjCollisionInfoCont(int Attribute, COLLISIONINFO* pCInfo, int nFrom)
{
	GOBJECT* pObjList = __goGetObjectList();
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		void* pObjInfo;		

		// 주인공 기체의 사각영역을 구함
		for (int mi=nFrom; mi < nObjCount; mi++)
		{
			if (pObjList[mi].nIdx != GO_EMPTY) 
			{
				if (pObjList[mi].Attribute == Attribute)
				{
					pObjInfo = pObjList[mi].pObjInfo;
					pObjList[mi].fnSetMember(pObjInfo);
					pCInfo->nObjIdx = mi;
					pCInfo->rcCollsion = pObjList[mi].fnGetRect();
					pObjList[mi].fnGetMember(pObjInfo);		// 의미없음
					if (mi < nObjCount-1)
						return mi+1;		// 다음 인덱스를 리턴
					else					// 마지막 인덱스인 경우
						return mi;
				}
			}
		}

		pCInfo->nObjIdx = GO_EMPTY;	// 찾지 못하면
	}
	return GO_EMPTY;
}

// 충돌 유무를 판단하는 함수
static BOOL __cdIsCollide(RECT* prcObj1, RECT* prcObj2)
{
	RECT rcDst;										// 서로 교차하는 사각 영역이 담김
	return IntersectRect(&rcDst, prcObj1, prcObj2);	// 두 개의 사각 영역이 교차하는지 검사
}


