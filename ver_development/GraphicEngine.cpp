///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : GraphicEngine.cpp
//
//   모 듈  이 름 : 그래픽엔진
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphicEngine.h"

extern SURFACEINFO g_sfSprite;
extern SURFACEINFO g_sfStarInfoBack;

extern GAMESTATE GameState;

// msimg32.lib : TransparentBlt(), AlphaBlend()

//// 비트맵파일을 비트맵핸들로 변환하여 리턴한다
HBITMAP __geBmpFile2BmpHandle(char* pstrFilename)
{
	FILE  *fp;	
	DWORD dwSize;
	BITMAPFILEHEADER BitmapFileHeader;
	BITMAPINFO *pBitmapInfo;
	HBITMAP hBit;	
	void* pBitmap;

	//// 파일을 연다
	fp = fopen(pstrFilename, "rb");

	//// BITMAPFILEHEADER 	
	fread(&BitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);

	//// BITMAPINFO의 크기를 구하고 할당, 읽는다
	dwSize = BitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);	// 40 bytes
	//dwSize = sizeof(BITMAPINFO);									// 44 bytes
	pBitmapInfo = (BITMAPINFO*) malloc(dwSize);

	fread(pBitmapInfo, dwSize, 1, fp);

	//// DIB 섹션을 만들고 버퍼 메모리를 할당한다.
	hBit=CreateDIBSection(NULL,				// HDC, DIB_PAL_COLORS인 경우만 사용
		                  pBitmapInfo,		// BITMAPINFO 포인터
						  DIB_RGB_COLORS,	// 색상 사용 플래그
						  &pBitmap,			// 메모리 어드래스
						  NULL,				// 파일 매핑 개체 핸들
						  0);				// 파일 매핑 개체의 비트 오프셋

	//// 이미지 데이터를 읽어들인다.
	int nImageSize = BitmapFileHeader.bfSize - BitmapFileHeader.bfOffBits;		
	fread(pBitmap, nImageSize-2, 1, fp); // 오류가 걸려서 보정을 하였다 (-2)
	
	////
	free(pBitmapInfo);

	////
	fclose(fp);

	return hBit;
}

//// BITMAPINFO 구조체 멤버 데이터값 설정 ( 기본적으로 24bit 모드 지원 )
// 값을 설정하는 이유는 우리가 출력할 함수인 StretchDIBits에서 이 구조체 정보를 원하기 때문
void __geSetBitmapinfoBin(BIN_SURFACEINFO* pbsInfo, BITMAPINFO *pBitmapInfo, BOOL bMirror)
{	
	pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFO);
	pBitmapInfo->bmiHeader.biWidth = pbsInfo->nWidth;
	pBitmapInfo->bmiHeader.biHeight = bMirror ? -pbsInfo->nHeight : pbsInfo->nHeight;
	pBitmapInfo->bmiHeader.biPlanes = 1;
	pBitmapInfo->bmiHeader.biBitCount = 24;
	pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	pBitmapInfo->bmiHeader.biSizeImage = pbsInfo->nWidth * pbsInfo->nHeight * 3;
	pBitmapInfo->bmiHeader.biClrUsed = 0;
	pBitmapInfo->bmiHeader.biClrImportant = 0;
}

//// 이진표면으로 비트맵핸들을 얻는다. 
HBITMAP __geBinSurface2BmpHandle(BIN_SURFACEINFO* pbsInfo)
{
	HBITMAP hBit;
	BITMAPINFO BitmapInfo;	

	__geSetBitmapinfoBin(pbsInfo, &BitmapInfo, FALSE);

	void* pSurface;

	//// DIB 섹션을 만들고 버퍼 메모리를 할당한다.
	hBit=CreateDIBSection(NULL,					// HDC, DIB_PAL_COLORS인 경우만 사용
		                  &BitmapInfo,			// BITMAPINFO 포인터
						  DIB_RGB_COLORS,		// 색상 사용 플래그
						  &pSurface,			// 메모리 어드래스
						  NULL,					// 파일 매핑 개체 핸들
						  0);					// 파일 매핑 개체의 비트 오프셋	

	memcpy(pSurface, pbsInfo->pSurface, BitmapInfo.bmiHeader.biSizeImage);
	return hBit;
}

//// BITMAPINFO 스트럭쳐 초기화 ( 기본적으로 24bit 모드 지원 )
// 매개 변수 psInfo를 참조하여 pBitmapInfo의 데이터 멤버값을 설정한다. bMirror는 출력할 떄 미러 여부를 판단하는 용도로 사용
// __geSurface2BmpHandle 함수에서 호출
void __geSetBitmapinfo(SURFACEINFO* psInfo, BITMAPINFO *pBitmapInfo, BOOL bMirror)
{	
	pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFO);
	pBitmapInfo->bmiHeader.biWidth = psInfo->nWidth;
	pBitmapInfo->bmiHeader.biHeight = bMirror ? -psInfo->nHeight : psInfo->nHeight;
	pBitmapInfo->bmiHeader.biPlanes = 1;
	pBitmapInfo->bmiHeader.biBitCount = 24;
	pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	pBitmapInfo->bmiHeader.biSizeImage = psInfo->nWidth * psInfo->nHeight * 3;
	pBitmapInfo->bmiHeader.biClrUsed = 0;
	pBitmapInfo->bmiHeader.biClrImportant = 0;
}

//// 이진표면을 비트맵핸들로 변환한다 
// 매개 변수 psInfo는 비트맵 핸들을 얻을 SURFACEINFO가 되고 pSrcSurface는 이진 표면의 선두 주소가 된다.
// 매개 변수 pSrcSurface가 가리키는 메모리를 이용하여 비트맵 핸들을 얻어 낸다.
// 이 함수는 AnimationFile에서 __afLoad2MemDc 함수에서 호출된다.
HBITMAP __geSurface2BmpHandle(SURFACEINFO* psInfo, BYTE* pSrcSurface)
{
	HBITMAP hBit=0;
	BITMAPINFO BitmapInfo;	

	__geSetBitmapinfo(psInfo, &BitmapInfo, TRUE);

	void* pSurface;

	//// DIB 섹션을 만들고 버퍼 메모리를 할당한다.
	hBit=CreateDIBSection(NULL,					// HDC, DIB_PAL_COLORS인 경우만 사용
		                  &BitmapInfo,			// BITMAPINFO 포인터
						  DIB_RGB_COLORS,		// 색상 사용 플래그
						  &pSurface,			// 메모리 어드래스
						  NULL,					// 파일 매핑 개체 핸들
						  0);					// 파일 매핑 개체의 비트 오프셋	

	memcpy(pSurface, pSrcSurface, BitmapInfo.bmiHeader.biSizeImage);
	return hBit;
}

//// 비트맵파일을 이진표면 정보로 변환한다
BIN_SURFACEINFO* __geBmpFile2BinSurface(char* pstrFilename)
{
	FILE			 *fp;	
	DWORD			 dwSize;
	BITMAPFILEHEADER BitmapFileHeader;
	BITMAPINFO		 *pBitmapInfo;

	//// 파일을 연다
	fp = fopen(pstrFilename, "rb");

	// BITMAPFILEHEADER를 읽는다.
	fread(&BitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);

	//// BITMAPINFO의 크기를 구하고 할당, 읽는다
	dwSize = BitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);	    // 40 bytes
	//dwSize = sizeof(BITMAPINFO);										// 44 bytes
	pBitmapInfo = (BITMAPINFO*) malloc(dwSize);
	fread(pBitmapInfo, dwSize, 1, fp);

	//// 이미지 데이터를 읽어들인다.		
	int nImageSize = BitmapFileHeader.bfSize - BitmapFileHeader.bfOffBits;	

	// BIN_SURFACEINFO를 생성한다.
	BIN_SURFACEINFO* pbsfBitmap = __geCreateSprBinSurface(); // 메모리 동적 할당

	//이미지 크기만큼 메모리를 할당한다
	pbsfBitmap->pSurface = (BYTE*) malloc(nImageSize);

	//// 거꾸로 읽어들인다 ( 정방향 이미지로 바꾼다 )
	//fread(pbsfBitmap->pSurface, nImageSize, 1, fp);	// 그냥 읽는 경우
	int nBmpWidth  = pBitmapInfo->bmiHeader.biWidth;			// 폭
	int nBmpHeight = pBitmapInfo->bmiHeader.biHeight;			// 높이
	int nPixelSize = pBitmapInfo->bmiHeader.biBitCount / 8;		// 픽셀 바이트 크기
	int nBmpWidthByte = nBmpWidth * nPixelSize;					// 비트맵 가로 바이트 수
	int nHorizonOffset = nBmpHeight * nBmpWidthByte;			// 마지막 수평 오프셋 (역방향->정방향 재배열하기 위해)

	for (int iH=0; iH < nBmpHeight; iH++){
		nHorizonOffset -= nBmpWidthByte;
		fread(pbsfBitmap->pSurface+nHorizonOffset, nBmpWidthByte, 1, fp);
	}
	//파일 닫기
	fclose(fp);

	//// 스프라이트 툴에서 다룰 이진 표면 생성 & 초기화
	__geSetSprBinSurface(pbsfBitmap, pBitmapInfo->bmiHeader.biWidth, pBitmapInfo->bmiHeader.biHeight, 0);
	
	////
	free(pBitmapInfo);

	return pbsfBitmap;
}

//// hdc(화면DC)와 호환되는 후면표면DC 생성
// psInfo가 생성될 후면 표면의 SURFACEINFO가 된다. 즉, 아웃 파라미터가 된다.
BOOL __geCreateBackBuffer(HDC hdc, int nWidth, int nHeight, int nBPP, SURFACEINFO* psInfo, COLORREF crColorKey)
{
	psInfo->dcSurface = CreateCompatibleDC(hdc);		// 메모리 DC를 생성, 이 때, 생성한 메모리 DC는 아직 사용할 수 있는 크기를 가지지는 않는다. (즉 1X1 크기이다.)
	psInfo->hBmp	  = CreateCompatibleBitmap (hdc, nWidth, nHeight); // nWidth, nHeight 크기를 가지는 hdc와 호환되는 비트맵 생성
    psInfo->hOldBmp   = (HBITMAP) SelectObject (psInfo->dcSurface, psInfo->hBmp );	// hBmp를 메모리 DC// 평행사변형꼴로 출력 함수 선택, 그러면 메모리DC는 hBmp와 동일한 크기와 픽셀 크기, 포맷을 가진다.
	PatBlt(psInfo->dcSurface, 0,0, nWidth, nHeight, PATCOPY);
	psInfo->nWidth    = nWidth;
	psInfo->nHeight   = nHeight;
	psInfo->nCentralX = nWidth / 2;
	psInfo->nCentralY = nHeight / 2; 
	psInfo->crColorKey = crColorKey;

	return TRUE;
}

//// 표면 생성 및 로드
//// 각종 오브젝트 또는 비트맵 DC 생성 및 비트맵 로드
// 비트맵 핸들을 가지고 있는 SURFACEINFO로 메모리 DC를 얻어 SURFACEINFO 구조체의 데이터 멤버 dcSurface에 저장한다.
// psInfo는 메모리 DC를 얻을 SURFACEINFO가 된다. 단, 비트맵 핸들을 가지고 있어야 한다.
void __geLoadSurface(HDC hdc, SURFACEINFO* psInfo)
{	
	BITMAP  bit;
	
	psInfo->dcSurface = CreateCompatibleDC(hdc);	// 메모리 DC를 생성
	//hBmp를 메모리 DC인 dcSurface에 선택한다. 그러면 dcSurface는 hBmp와 동일한 크기와 픽셀 크기, 포맷을 가지게 되고 hBmp가 dcSurface로 적재된다.
	psInfo->hOldBmp = (HBITMAP) SelectObject(psInfo->dcSurface, psInfo->hBmp);

	// 비트맵의 크기를 얻어 pbInfo의 nWidth와 nHeight를 초기화한다.
	GetObject(psInfo->hBmp, sizeof(BITMAP), &bit);	
	psInfo->nWidth  = bit.bmWidth;
	psInfo->nHeight = bit.bmHeight;	
	psInfo->nCentralX = bit.bmWidth / 2;
	psInfo->nCentralY = bit.bmHeight/ 2;
}

//// 생성된 표면 해제
// 매개 변수 psInfo는 해제시킬 표면의 선두 주소가 된다.
void __geReleaseSurface(SURFACEINFO* psInfo)
{
	SelectObject(psInfo->dcSurface, psInfo->hOldBmp); //dcSurface에 이전의 비트맵 핸들 hOldBmp를 선택한다.
	DeleteDC(psInfo->dcSurface);					  // 메모리 삭제
	DeleteObject(psInfo->hBmp);						  // 비트맵 핸들 hBmp를 삭제
}

//// 생성된 표면 해제
void __geReleaseBinSurface(BIN_SURFACEINFO* pbsInfo)
{
	if (pbsInfo)
	{
		if (pbsInfo->pSurface) 
		{
			free(pbsInfo->pSurface);
			pbsInfo->pSurface = NULL;			// 나중에 참조될 경우를 생각해서 초기화
		}
		free(pbsInfo);
		pbsInfo = NULL;							//			``
	}
}

//
//
//
//
//
//
//
//		
//
//
//
//
//
//
//

//// 이미지 출력
// 표면 DC를 목적 DC로 BitBlt한다.
// 매개 변수 dcDst는 목적 DC가 된다. 화면 DC가 될 수도 있고 후면 표면 DC가 될 수도 있다. 아니면, 또다른 표면 DC가 될 수도 있다.
// x와 y는 출력 좌표가 된다. 마지막의 psInfo는 출력할 SURFACEINFO 구조체이다.
BOOL __gePutImage(HDC dcDst, int nx1, int ny1, int nx2, int ny2, SURFACEINFO* psInfo)
{
	if (psInfo)	// 만약 psInfo가 유효하면 BitBlt 함수로 표면 DC인 dcSurface를 그대로 dcDst에 전송한다.
		return BitBlt(dcDst, nx1, ny1, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, nx2, ny2, SRCCOPY);
	else return FALSE;
}

BOOL __gePutImageAND(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	if (psInfo)	// 만약 psInfo가 유효하면 BitBlt 함수로 표면 DC인 dcSurface를 AND연산하여 dcDst에 전송한다.
		return BitBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCAND);
	else return FALSE;
}

BOOL __gePutImageMASK(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	if (psInfo)	// 만약 psInfo가 유효하면 BitBlt 함수로 표면 DC인 dcSurface를 MASK연산하여 dcDst에 전송한다.
		return BitBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCMASK);
	else return FALSE;
}

BOOL __gePutImageOR(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	if (psInfo)	// 만약 psInfo가 유효하면 BitBlt 함수로 표면 DC인 dcSurface를 OR연산하여 dcDst에 전송한다.
		return BitBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCPAINT);
	else return FALSE;
}

//// BIN_SURFACEINFO 구조체를 이용하여 DeviceContext에 이미지를 출력하는 함수
int  __gePutImageBin(HDC dcDst, int x, int y, BIN_SURFACEINFO* pbsInfo)
{
	if (pbsInfo)
	{
		BITMAPINFO BitmapInfo;		
		//BitmapInfo를 얻어낸다. 
		//마지막 인자는 미러 여부 정방향 : TRUE
		//API의 비트맵 출력 함수들은 역방향으로 저장된 것을 가정하여 
		//미러 여부를 정하기 때문에 정방향으로 저장된 경우라면 TRUE
		__geSetBitmapinfoBin(pbsInfo, &BitmapInfo, TRUE);
		//이미지 출력
		return ::StretchDIBits(dcDst,
							   x, y, pbsInfo->nWidth, pbsInfo->nHeight,
							   0, 0, pbsInfo->nWidth, pbsInfo->nHeight,
							   pbsInfo->pSurface, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
	else return FALSE;
}

//// 이미지 블렌드 
// 표면 DC를 목적 DC로 투명도를 가지는 이미지로 AlphaBlend 한다.
// 마지막 매개 변수 nAlpha가 투명도를 자니는 값이다. 0~255의 범위값을 가지고 0이면 완전 투명 255이면 불투명이 된다.
BOOL __gePutImageBlend(HDC dcDst, int x, int y, SURFACEINFO* psInfo, int nAlpha)
{
	BLENDFUNCTION bf;

	//// BLENDFUNCTION 초기화
	bf.BlendOp     = AC_SRC_OVER;	//
	bf.BlendFlags  = 0;				//			
	bf.AlphaFormat = 0;				//
	bf.SourceConstantAlpha = nAlpha; // 0 - 255

	//// 알파 블렌드
	return 	AlphaBlend(dcDst, 0, 0, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, x, y, psInfo->nWidth, psInfo->nHeight, bf);
}

//// 스프라이트
// 표면 DC를 목적 DC로 스프라이트 출력한다.
BOOL __gePutSprite(HDC dcDst, int x, int y, int nx, int ny, SURFACEINFO* psInfo)
{	
	return TransparentBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, nx, ny, psInfo->nWidth, psInfo->nHeight,
						  psInfo->crColorKey);
}

//// hdc에 표면을 복사 (후면표면 --전송--> 화면DC)
// 후면 표면 DC를 화면 DC로 블리트하는 함수이다.
// 매개 변수 hdc는 화면 DC가 되고 psInfo는 후면 표면의 SURFACEINFO구조체가 된다.
BOOL __geCompleteBlt(HDC hdc, SURFACEINFO* psInfo)
{	
	BitBlt(hdc, 0, 0, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCCOPY);
	return TRUE;
}

BOOL __geCompleteBlt(HDC hdc, int x, int y, SURFACEINFO* psInfo)
{	
	BitBlt(hdc, 0, 0, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, x, y, SRCCOPY);
	return TRUE;
}


//// SURFACEINFO 이미지 초기화
// 생성된 표면을 초기화할 때 출력
void __geSetImgSurface(SURFACEINFO* psfInfo)
{
	memset(psfInfo, 0, sizeof(SURFACEINFO));
}

//// SURFACEINFO 스프라이트 초기화
// 생성된 표면을 초기화할 떄 출력, 컬러키를 매개 변수로 받아서 초기화해 주는 것만 다르다.
void __geSetSprSurface(SURFACEINFO* psfInfo, COLORREF crColorKey)
{
	memset(psfInfo, 0, sizeof(SURFACEINFO));	
	psfInfo->crColorKey = crColorKey;
}

BIN_SURFACEINFO* __geCreateSprBinSurface()
{
	return (BIN_SURFACEINFO*) malloc(sizeof(BIN_SURFACEINFO));
}

// SURFACEINFO 구조체 크기만큼 메모리를 할당하고 그 주소를 리턴, 처음 표면을 생성할 때 호출한다.
SURFACEINFO* __geCreateSprSurface()
{
	return (SURFACEINFO*) malloc(sizeof(SURFACEINFO));
}

//// BIN_SURFACEINFO 데이터 멤버 초기화
// pbsfInfo 구조체를 포인터로 선언하여 그 선두 주소를 받고 있는데, 구조체의 크기가 아무리 커도
// 주소가 인자로 넘어가므로 스택에 불필요한 메모리 할당과 복사가 이루어지지 않는다.
// 따라서, 구조체를 인자로 받아 처리하는 경우에는 포인터로 선언하여 주소를 받는 것이 속도나
// 메모리 사용 측면에서 효과적이다.
void __geSetSprBinSurface(BIN_SURFACEINFO* pbsfInfo, int nWidth, int nHeight, COLORREF crColorKey)
{
	pbsfInfo->nWidth = nWidth;
	pbsfInfo->nHeight = nHeight;
	pbsfInfo->crColorKey = crColorKey;

	pbsfInfo->nCentralX = nWidth / 2;
	pbsfInfo->nCentralY = nHeight / 2;
	pbsfInfo->dwDelayTime = 100;		//ms
}

// BIN_SURFACEINFO 이미지에서 원하는 위치의 픽셀 정보를 얻어 내는 함수
COLORREF __geGetPixel(BIN_SURFACEINFO* pbsInfo, int x, int y)
{
	COLORREF colorPixel = 0;	//32비트 크기이므로 복사하기에 항상 0으로 초기화

	BYTE* pSurface = pbsInfo->pSurface;	// pSurface는 이미지의 선두 주소를 가리키고 있다.
	//원하는 위치로부터 3바이트만큼 colorPixel로 복사
	//이미지는 메모리에 저장되어 있는데, 이 메모리는 1차원 배열과 동일하게 구성
	// 즉, 이미지가 2차원적으로 표시된다고 하더라도 메모리상에는 1차원적으로 저장되어 있다.
	// 이미지에서 원하는 위치의 픽셀의 값을 알아내기 위해 공식을 사용해야 한다.
	// 원하는 위치 : x, y
	// 공식 : pSurface(이미지의 선두 주소) + y*(이미지의 가로폭*(픽셀바이트수)) + (x*픽셀바이트수)
	memcpy(&colorPixel, pSurface + y * pbsInfo->nWidth*3 + x*3, 3);

	return colorPixel;
}

//// 위치의 포인터를 얻는다
BYTE* __geGetBitsPtr(BIN_SURFACEINFO* pbsInfo, int x, int y)
{
	return pbsInfo->pSurface + y * pbsInfo->nWidth*3 + x*3;
}

////
RECT __geGetCuttingRect(HWND hWnd, RECT rcDrag, BIN_SURFACEINFO* pbsInfo)
{	
	////
	COLORREF   colorPixel;

    //// 추출영역 제한 ( 비정상적 드래그 방지 )
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	rcClient.right--;
	rcClient.bottom--;
	
	IntersectRect(&rcDrag, &rcDrag, &rcClient);	

    ////
    int nMinX = rcDrag.right;
	int nMinY = rcDrag.bottom;
	int nMaxX = rcDrag.left;
	int nMaxY = rcDrag.top;

	int ix, iy;

    //// X
    for (iy=rcDrag.top; iy < rcDrag.bottom; iy++)
    {
        // nMinX (왼쪽 끝 면)
        for (ix=rcDrag.left; ix < rcDrag.right; ix++)
        {
            colorPixel = __geGetPixel(pbsInfo, ix, iy);
			if (colorPixel != pbsInfo->crColorKey && ix < nMinX)
			{
				nMinX = ix;
				break;
			}
        }
        // nMaxX (오른쪽 끝 면)
        for (ix=rcDrag.right; ix > rcDrag.left; ix--)
        {
            colorPixel = __geGetPixel(pbsInfo, ix, iy);
            if (colorPixel != pbsInfo->crColorKey && ix > nMaxX)
			{
				nMaxX = ix;
				break;
			}     
        }
    }

    //// Y
    for (ix=rcDrag.left; ix < rcDrag.right; ix++)
    {
        // nMinY (위쪽 끝 면) 
        for (iy=rcDrag.top; iy < rcDrag.bottom; iy++)
        {
            colorPixel = __geGetPixel(pbsInfo, ix, iy);
            if (colorPixel != pbsInfo->crColorKey && iy < nMinY)
			{
				nMinY = iy;
				break;
			}
        }
        // nMaxY (아래쪽 끝 면)
        for (iy=rcDrag.bottom; iy > rcDrag.top; iy--)
        {
            colorPixel = __geGetPixel(pbsInfo, ix, iy);
            if (colorPixel != pbsInfo->crColorKey && iy > nMaxY)
			{
				nMaxY = iy;
				break;
			}            
        }
    }

	//// 좌표보정
	RECT rcFrame = {nMinX, nMinY, nMaxX+1, nMaxY+1};

	return rcFrame;
}

//// 4의 배수가 되는 최소 표면을 만든다
void __geCreateCompactSurface(BIN_SURFACEINFO* pbsInfo, RECT rcCompact, BIN_SURFACEINFO* pbsInfoCompact)
{	
	int		nStep=0;
	int     nSurfaceSize;
	LPBYTE	pSurface;
	BYTE    btColorKey[3];

	btColorKey[0] = GetRValue(pbsInfo->crColorKey);
	btColorKey[1] = GetGValue(pbsInfo->crColorKey);
	btColorKey[2] = GetBValue(pbsInfo->crColorKey);

	int nWidthCompact  = rcCompact.right - rcCompact.left + 1;
	int nWidthRevision = nWidthCompact;
	int nHeight = rcCompact.bottom - rcCompact.top + 1; 
	
	nWidthRevision = ((nWidthRevision + 3) & ~3);		// 4의 배수
	nSurfaceSize   = nWidthRevision * nHeight * 3;
	pSurface       = (BYTE*) malloc(nSurfaceSize);

	//// 메모리 컬러키 초기화
	for (int i = 0; i < nSurfaceSize; i+=3) ::memcpy(pSurface+i, btColorKey, 3);

	//// 4의 배수 보정에 의한 가로폭 상이
	int nSrcCalcWidth = nWidthCompact * 3;
	int nDstCalcWidth = nWidthRevision * 3;

	for (int iy=rcCompact.top; iy < rcCompact.bottom; iy++)
	{
		::memcpy(pSurface+nStep,
			     __geGetBitsPtr(pbsInfo, rcCompact.left, iy), nSrcCalcWidth);
		nStep += nDstCalcWidth;
	}

	pbsInfoCompact->pSurface = pSurface;
	pbsInfoCompact->nWidth  = nWidthRevision;
	pbsInfoCompact->nHeight = nHeight;	
}   
  
int __gePutImageRotateBG(SURFACEINFO* psInfo, float theta, SURFACEINFO* psBGInfo){
	POINT apt[3] = {0};											// 평행사변형꼴 꼭지점 저장 변수

	float Radis     = DegreesToRads(theta);                     // 회전할 각도(360분법)를, 라디안단위의 각도로 변경   
    float cosine    = (float)cos(Radis);                        // 세타값의 변화에 따른 cos 값을 구함          
    float sine      = (float)sin(Radis);                        // 세타값의 변화에 따른 sin 값을 구함 
	
	double dblSizeRatio=1;										// 원본과 사이즈 비율(1이면 원본 크기, 0.5면 절반 크기 이런식이다.)
	double dblx, dbly;											// 회전축을 기준으로 얻은 세꼭지점의 상대좌표 저장 변수
	double dblWidth = psInfo->nWidth*dblSizeRatio;				// 출력할 비트맵 너비, 높이 크기
	double dblHeight = psInfo->nHeight*dblSizeRatio;   
	double dblxDest, dblyDest;									// 회전후 위치좌표(상대좌표) 저장 변수
	int ixDisplay = SCR_WIDTH/2;								// DC에 표시할 X좌표(편의상 축 좌표를 기준으로 비트맵을 DC에 그린다.)
	int iyDisplay = SCR_HEIGHT;									// DC에 표시할 Y좌표(편의상 축 좌표를 기준으로 비트맵을 DC에 그린다.)
	double ixRotate = psInfo->nWidth/2 * dblSizeRatio;			// 회전 X축, Y축
	double iyRotate = psInfo->nHeight/2 * dblSizeRatio;		

	// 스테이지 상에서 별 정보 출력시 그에 맞게 중심점을 잡아줘야 한다. DC 크기에 맞도록 변경
	if(GameState == STAGE_SELECT){
		ixDisplay = g_sfStarInfoBack.nWidth/2;
		iyDisplay = g_sfStarInfoBack.nHeight/2;		
	}

	// 게임 시스템 속도 최적화를 위해 넣어야할 변수를 약간 변경시켰다.
	// 실제 대입 변수는 아래이며, 차이가 궁금하다면 아래의 주석을 풀어서 비교해 보라.
	// double dblWidth = psBGInfo->nWidth*dblSizeRatio;		
	// double dblHeight = psBGInfo->nHeight*dblSizeRatio;   
	// ixRotate = psBGInfo->nWidth/2 * dblSizeRatio
	// iyRotate = psBGInfo->nheight/2 * dblSizeRatio
	
	// 1. 회전축을 기준으로 상대좌표를 구하고 
	// 2. 회전후 위치좌표(상대좌표)를 얻은 후
	// 3. 얻은 값을 원래의 좌표에 적용.
	for(int i=0 ; i<3 ; i++){
		if(i==0){dblx=-ixRotate, dbly=-iyRotate;}				// left up 꼭지점 부분
		else if(i==1){dblx=dblWidth-ixRotate, dbly=-iyRotate;}	// right up 꼭지점 부분
		else if(i==2){dblx=-ixRotate, dbly=dblHeight-iyRotate;} // left low 꼭지점 부분

		dblxDest=dblx*cosine-dbly*sine;
		dblyDest=dblx*sine+dbly*cosine;
		dblxDest+=ixRotate, dblyDest+=iyRotate;
		apt[i].x= ixDisplay - (long)ixRotate+(long)dblxDest;
		apt[i].y= iyDisplay -(long)iyRotate+(long)dblyDest;
	}

	// 평행사변형꼴로 출력 함수
	// 가장 중요한 인수는 두번째 인수
	// 세 점의 좌표에 따라 출력 모양을 결정한다.
	// 속도가 너무 느리다
	PlgBlt(psInfo->dcSurface, apt, psBGInfo->dcSurface, 0, 0, psBGInfo->nWidth, psBGInfo->nHeight, 0, 0, 0);	

	return true;
}


int __gePutImageRotateObject(SURFACEINFO* psInfo, int nY, float theta, int* nXr, int* nYr, SURFACEINFO* psObjectInfo){
	POINT apt[3] = {0};											// 평행사변형꼴 꼭지점 저장 변수

	float Radis     = DegreesToRads(theta);                     // 회전할 각도(360분법)를, 라디안단위의 각도로 변경   
    float cosine    = (float)cos(Radis);                        // 세타값의 변화에 따른 cos 값을 구함          
    float sine      = (float)sin(Radis);                        // 세타값의 변화에 따른 sin 값을 구함 
	
	double dblSizeRatio=1;										// 원본과 사이즈 비율(1이면 원본 크기, 0.5면 절반 크기 이런식이다.)
	double dblx, dbly;											// 회전축을 기준으로 얻은 세꼭지점의 상대좌표 저장 변수
	double dblWidth = psObjectInfo->nWidth*dblSizeRatio;		// 출력할 비트맵 너비, 높이 크기
	double dblHeight = psObjectInfo->nHeight*dblSizeRatio;   
	double dblxDest, dblyDest;									// 회전후 위치좌표(상대좌표) 저장 변수
	int ixDisplay = SCR_WIDTH/2;								// DC에 표시할 X좌표(편의상 축 좌표를 기준으로 비트맵을 DC에 그린다.)
	int iyDisplay = SCR_HEIGHT;									// DC에 표시할 Y좌표(편의상 축 좌표를 기준으로 비트맵을 DC에 그린다.)
	double ixRotate = psObjectInfo->nWidth/2 * dblSizeRatio;	// 회전 X축, Y축
	double iyRotate = nY+psObjectInfo->nHeight/2 * dblSizeRatio;		// 원점으로부터 거리를 떼어내기 위해 상수 연산
	
    // 스테이지 상에서 별 정보 출력시 그에 맞게 중심점을 잡아줘야 한다. DC 크기에 맞도록 변경
	if(GameState == STAGE_SELECT){
		ixDisplay = g_sfStarInfoBack.nWidth/2;
		iyDisplay = g_sfStarInfoBack.nHeight/2;		
	}

	// 1. 회전축을 기준으로 상대좌표를 구하고 
	// 2. 회전후 위치좌표(상대좌표)를 얻은 후
	// 3. 얻은 값을 원래의 좌표에 적용.
	for(int i=0 ; i<3 ; i++){
		if(i==0){dblx=-ixRotate, dbly=-iyRotate;}				// left up 꼭지점 부분
		else if(i==1){dblx=dblWidth-ixRotate, dbly=-iyRotate;}	// right up 꼭지점 부분
		else if(i==2){dblx=-ixRotate, dbly=dblHeight-iyRotate;} // left low 꼭지점 부분

		dblxDest=dblx*cosine-dbly*sine;
		dblyDest=dblx*sine+dbly*cosine;
		dblxDest+=ixRotate, dblyDest+=iyRotate;
		apt[i].x= ixDisplay -(long)ixRotate+(long)dblxDest;
		apt[i].y= iyDisplay -(long)iyRotate+(long)dblyDest;
		
		if(i==0){								// 회전 후 오브젝트의 좌표를 구한다. 중심 좌표를 감안해서 좌표를 구한다.(오차 없음)
			(*nXr) = apt[i].x + psObjectInfo->nCentralX, (*nYr) = apt[i].y + psObjectInfo->nCentralY;
		}			
	}

	// 평행사변형꼴로 출력 함수
	// 가장 중요한 인수는 두번째 인수
	// 세 점의 좌표에 따라 출력 모양을 결정한다.
	// 속도가 너무 느리다
	PlgBlt(psInfo->dcSurface, apt, psObjectInfo->dcSurface, 0, 0, psObjectInfo->nWidth, psObjectInfo->nHeight, 0, 0, 0);	

	return true;
}


















