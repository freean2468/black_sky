#include "StdAfx.h"

extern MSG msg;
extern HINSTANCE hInst;
extern HCURSOR hNowCursor;

extern SURFACEINFO g_sfBack;

extern HWND g_hWnd;

extern STARINFO StarInfo[MAX_STAR];
extern STARSTEP StarStep;

extern UISTATE UiState;

//// ���콺 �����Ϳ� ����
void __ccCheckCollisionMousePointer(POINTS MousePoint, int Attribute)
{
	GOBJECT* pObjList = __goGetObjectList();
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		void* pObjObjectInfo;
		RECT rcObject;
		RECT rcMousePoint;

		rcMousePoint.top = MousePoint.y-30;				// ���� ���� ����
		rcMousePoint.left = MousePoint.x-30;
		rcMousePoint.bottom = MousePoint.y+30;
		rcMousePoint.right = MousePoint.x+30;

		// ���콺 ������ ������ ���� �浹 üũ	
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
					pObjList[ei].fnGetMember(pObjObjectInfo);		// �ǹ̾���

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
							__goRemoveAt(pObjList[ei].nIdx);						// ȣ�� ���� ����
							
							msg.message = NULL;

							hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)); // ������ Ŀ�� �ʱ�ȭ

							break;
						}
						break;
					}
					else if(UiState == DESTROY){
						hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR3)); // ������ Ŀ�� �ʱ�ȭ
					}
					else{
						hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)); // ������ Ŀ�� �ʱ�ȭ
					}
				}
			}
		}
	}
}

//// ���콺 �����Ϳ� ������Ʈ
void __ccCheckCollisionDestroy(POINTS MousePoint)
{
	GOBJECT* pObjList = __goGetObjectList();
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		void* pObjObjectInfo;
		RECT rcObject;
		RECT rcMousePoint;

		rcMousePoint.top = MousePoint.y-20;				// ���� ���� ����
		rcMousePoint.left = MousePoint.x-20;
		rcMousePoint.bottom = MousePoint.y+20;
		rcMousePoint.right = MousePoint.x+20;

		// ���콺 ������ ������ ������ ������ ������Ʈ �浹 üũ	
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
						pObjList[ei].fnGetMember(pObjObjectInfo);		// �ǹ̾���

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

								if(pObjList[ei].Attribute == MUDTEMPLE)	// ��������� �ı��Ѵٸ�
									StarInfo[StarStep].bIsMudTemple = false;

								StarInfo[StarStep].nCountMud += ObjInfo->MudStep;

								pObjList[ei].fnRelease;
								__goRemoveAt(pObjList[ei].nIdx);						// ȣ�� ���� ����
								
								msg.message = NULL;

								hNowCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)); // ������ Ŀ�� �ʱ�ȭ

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

//// ��Ŀ�� ������Ʈ�� ���� ������Ʈ �浹 üũ
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
		__cdGetObjCollisionInfo(Attribute, &cInfoObj);				// �浹 ��ü�� ã�� ���� cInfoObj�� �� ������ ��� ����.

		//if(__fo_ObjInfo->nPositionStep == SKY){				// �ϴ�
		//	cInfoObj.rcCollsion.top-=55;
		//	cInfoObj.rcCollsion.bottom-=70;
		//}
		//else if(__fo_ObjInfo->nPositionStep == FIRST){		// ����
		//	cInfoObj.rcCollsion.top+=40;
		//	cInfoObj.rcCollsion.bottom-=40;
		//}
		//else if(__fo_ObjInfo->nPositionStep == SECOND){		// ����
		//	cInfoObj.rcCollsion.top+= 50;
		//	cInfoObj.rcCollsion.bottom -= 30;
		//}
		//else if(__fo_ObjInfo->nPositionStep == CENTER){		// �߾�
		//
		//}

		// ��Ŀ�� ������Ʈ�� ���� ������Ʈ ���� �浹 ����	
		for (int ei=0; ei < nObjCount; ei++)
		{
 			if (pObjList[ei].nIdx != GO_EMPTY) 
			{
				if(pObjList[ei].StarStep == StarStep){	// ���� ���� ���� üũ
					if(pObjList[ei].nPositionStep == __fo_ObjInfo->nPositionStep && __fo_ObjInfo->Attribute != WIND){	// ���� ���� ���� üũ (�ٸ� ���� üũ���� �ʴ´�.
						if(pObjList[ei].Attribute != Attribute && pObjList[ei].Attribute != MUD){
							pObjObjectInfo = pObjList[ei].pObjInfo;
							pObjList[ei].fnSetMember(pObjObjectInfo);
							rcObject = pObjList[ei].fnGetRect();
							pObjList[ei].fnGetMember(pObjObjectInfo);		// �ǹ̾���		

							//�浹üũ�� �Ϻ��� �ǹǷ� �ʿ� ����.
							// ���� ������ �ٲپ� �ִ� ������ ���ӻ� ���� ���Ϸ� �״ܰ�� �����ϱ� �����̴�.
							// �� �ٸ� ������ �ִ� ������Ʈ���� ���ο� �浹üũ�� �Ǿ�� �� �ȴ�.
 							//rcObject.right-=25;
							//rcObject.left+=25;
							//if(pObjList[ei].nPositionStep == SKY){				// �ϴ�
							//	rcObject.top-=55;
							//	rcObject.bottom-=70;
							//}
							//else if(pObjList[ei].nPositionStep == FIRST){		// ����
							//	rcObject.top+=40;
							//	rcObject.bottom-=40;
							//}
							//else if(pObjList[ei].nPositionStep == SECOND){		// ����
							//	rcObject.top+= 50;
							//	rcObject.bottom -= 30;
							//}
							//else if(pObjList[ei].nPositionStep == CENTER){		// �߾�
							//
							//}

							/*HDC hdc;

							hdc = GetDC(g_hWnd);

							Rectangle(g_sfBack.dcSurface, rcObject.left, rcObject.top, rcObject.right, rcObject.bottom);

							Rectangle(g_sfBack.dcSurface, cInfoObj.rcCollsion.left, cInfoObj.rcCollsion.top, cInfoObj.rcCollsion.right, cInfoObj.rcCollsion.bottom);

							ReleaseDC(g_hWnd, hdc);*/

							
							if (__cdIsCollide(&cInfoObj.rcCollsion, &rcObject))	
 								return true;	// �浹�ϹǷ� ���� �� ����
						}
					}
				}
			}
		}
	}
   	return false;		// �浹���� ����
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
//			// nFrom�� ��ȿ������ 0 ~ nObjCount-1���� (�ε���), ������ �ε����̰ų� ������
//			if (nFrom >= nObjCount-1 || nFrom == GO_EMPTY) break;
//
//			// ���ΰ� ��ü�� �� ��ü �Ǵ� �� �Ѿ˰��� �浹 ����	
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
//						pObjList[ei].fnGetMember(pObjEnemyInfo);		// �ǹ̾���
//
//						//
//						if (__cdIsCollide(&cInfoObj.rcCollsion, &rcEnemy))
//						{
//							// ������ ó��
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

// ���� ������Ʈ ����Ʈ���� �浹 ��ü�� �Ǵ� ���� ������Ʈ�� ã�� ���� COLLISIONINFO�� ���� �Լ�
// nObject�� ã�� ������Ʈ ����Ʈ���� �ε����� �浹�簢���� ����
// �Ű� ���� pCinfo�� �ƿ� �Ķ����
static void __cdGetObjCollisionInfo(int Attribute, COLLISIONINFO* pCInfo)
{
	GOBJECT* pObjList = __goGetObjectList();				// ���� ������Ʈ ����Ʈ�� ���� �ּҸ� ����
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		void* pObjInfo;		

		// ��Ŀ�� ������Ʈ�� �簢������ ����
		for (int mi=0; mi < nObjCount; mi++)				// ���� ������Ʈ ����Ʈ�� ���ʷ� ������ ���� ������Ʈ�� ã�Ƴ���
		{													// �� �̻� �˻����� �ʰ� ����ü pCInfo�� ������ ������� ä�� ��ȯ�Ѵ�.
			if (pObjList[mi].nIdx != GO_EMPTY)				// ���� ������Ʈ ����Ʈ���� ���ǰ� �ִ� �迭�� �˻��Ѵ�.
			{
				if (pObjList[mi].Attribute == Attribute)			// �浹 �׷��� ã�� ���� ������Ʈ �̸� if�� ����
				{
					pObjInfo = pObjList[mi].pObjInfo;
					pObjList[mi].fnSetMember(pObjInfo);
					pCInfo->nObjIdx = mi;
 					pCInfo->rcCollsion = pObjList[mi].fnGetRect();
					pObjList[mi].fnGetMember(pObjInfo);		// �ǹ̾���				 
					return;
				}
			}
		}

		pCInfo->nObjIdx = GO_EMPTY;	// ã�� ���ϸ�
	}
}

// ������ �浹 ��ü�� COLLISIONINFO�� ��� ���� �Լ�
// �� �Լ��� __cdGetObjCollisionInfo �Լ��� ���������, �Լ��� ����� ����
// �˻��Ͽ� ã�� ���� ������Ʈ�� ���� �ε����� ��ȯ�Ѵٴ� ���� �ٸ���. �ٽ� �� �Լ��� ȣ��� ������
// ��ȯ���� �ε������� �ٽ� �˻��Ѵ�. �ֳ� �ϸ�, �浹 ��ü�� �����̱� �����̴�.
//// nObject�� ã�� ������Ʈ ����Ʈ���� �ε����� �浹�簢���� ����
// �Ű����� nFrom�� �˻��� ������ ���� ������Ʈ�� �ε����� �Ѿ�´�.
static int __cdGetObjCollisionInfoCont(int Attribute, COLLISIONINFO* pCInfo, int nFrom)
{
	GOBJECT* pObjList = __goGetObjectList();
	if (pObjList)
	{
		int nObjCount = __goGetObjectMaxCount();
		void* pObjInfo;		

		// ���ΰ� ��ü�� �簢������ ����
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
					pObjList[mi].fnGetMember(pObjInfo);		// �ǹ̾���
					if (mi < nObjCount-1)
						return mi+1;		// ���� �ε����� ����
					else					// ������ �ε����� ���
						return mi;
				}
			}
		}

		pCInfo->nObjIdx = GO_EMPTY;	// ã�� ���ϸ�
	}
	return GO_EMPTY;
}

// �浹 ������ �Ǵ��ϴ� �Լ�
static BOOL __cdIsCollide(RECT* prcObj1, RECT* prcObj2)
{
	RECT rcDst;										// ���� �����ϴ� �簢 ������ ���
	return IntersectRect(&rcDst, prcObj1, prcObj2);	// �� ���� �簢 ������ �����ϴ��� �˻�
}


