///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : GraphicEngine.h
//
//   모 듈  이 름 : 그래픽엔진
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GRAPHICENGINE_H__
#define __GRAPHICENGINE_H__

#include <windows.h>

#define SRCMASK 0x00220326 // 마스크를NOT연산한것과 다시 AND연산

typedef struct SURFACEINFOtag
{
	int			nWidth;
	int			nHeight;		
	int			nCentralX;		// 중심좌표 X
	int			nCentralY;		// 중심좌표 Y

	COLORREF	crColorKey;		// 컬러키
	DWORD		dwDelayTime;	// 지연시간 (ms)

	HDC			dcSurface;		// 비트맵이 적재되있는 표면 DC
	HBITMAP		hBmp;			// 비트맵 핸들
	HBITMAP		hOldBmp;		// 이전 비트맵 핸들 ( dc 해제시 참조 )
} SURFACEINFO;

//이미지 데이터 == 표면
//↓↓이진 표면 정보
//하나의 이미지를 데이터화한 구조체
//이미지나 스프라이트를 출력할 때가 아니면 애니메이션 시킬 때 항상 이 구조체를 이용해서 출력
typedef struct BIN_SURFACEINFOtag
{	
	int			nWidth;			// 폭
	int			nHeight;		// 높이
	int			nCentralX;		// 중심좌표 X
	int			nCentralY;		// 중심좌표 Y
	COLORREF	crColorKey;		// 컬러키
	DWORD		dwDelayTime;	// 애니메이션을 하기 위한 지연시간 (ms)
	
	BYTE*		pSurface;		// 표면의 주소
} BIN_SURFACEINFO;

HBITMAP				__geBmpFile2BmpHandle(char* ptstrFilename);
BIN_SURFACEINFO*	__geBmpFile2BinSurface(char* ptstrFilename);
void				__geSetBitmapinfoBIn(BIN_SURFACEINFO* pbsInfo, BOOL bMirror);
HBITMAP				__geBinSurface2BmpHandle(BIN_SURFACEINFO* pbsInfo);
void				__geSetBitmapinfo(SURFACEINFO* psInfo, BOOL bMirror);				// SURFACEINFO
HBITMAP				__geSurface2BmpHandle(SURFACEINFO* psInfo, BYTE* pSurface);			//

BOOL	__geCreateBackBuffer(HDC hdc, int nWidth, int nHeight, int nBPP, SURFACEINFO* psInfo, COLORREF crColorKey = NULL);
void	__geLoadSurface(HDC hdc, SURFACEINFO* psInfo);
void	__geReleaseSurface(SURFACEINFO* psInfo);
void	__geReleaseBinSurface(BIN_SURFACEINFO* pbsInfo);

BOOL	__gePutImage(HDC dcDst, int n1x, int ny1, int nx2, int ny2, SURFACEINFO* psInfo);
BOOL	__gePutImageAND(HDC dcDst, int x, int y, SURFACEINFO* psInfo);
BOOL	__gePutImageMASK(HDC dcDst, int x, int y, SURFACEINFO* psInfo);
BOOL	__gePutImageOR(HDC dcDst, int x, int y, SURFACEINFO* psInfo);

int		__gePutImageBin(HDC dcDst, int x, int y, BIN_SURFACEINFO* pbsInfo);
BOOL	__gePutImageBlend(HDC dcDst, int x, int y, SURFACEINFO* psInfo, int nAlpha);
BOOL	__gePutSprite(HDC dcDst, int x, int y, int nx, int ny, SURFACEINFO* psInfo);
BOOL	__geCompleteBlt(HDC hdc, SURFACEINFO* psInfo);
BOOL	__geCompleteBlt(HDC hdc, int x, int y, SURFACEINFO* psInfo);

COLORREF __geGetPixel(BIN_SURFACEINFO* pbsInfo, int ix, int iy);
BYTE*    __geGetBitsPtr(BIN_SURFACEINFO* pbsInfo, int x, int y);

void	__geSetImgSurface(SURFACEINFO* psfInfo);
void	__geSetSprSurface(SURFACEINFO* psfInfo, COLORREF crColorKey);
void	__geSetImgBinSurface(BIN_SURFACEINFO* psfInfo);
void	__geSetSprBinSurface(BIN_SURFACEINFO* pbsfInfo, int nWidth, int nHeight, COLORREF crColorKey);

SURFACEINFO*	 __geCreateSprSurface();
BIN_SURFACEINFO* __geCreateSprBinSurface();

void __geCreateCompactSurface(BIN_SURFACEINFO* pbsInfo, RECT rcCompact, BIN_SURFACEINFO* pbsInfoCompact);
RECT __geGetCuttingRect(HWND hWnd, RECT rcDrag, BIN_SURFACEINFO* pbsInfo);

int __gePutImageRotateBG(SURFACEINFO* psInfo, float theta, SURFACEINFO* psObjectInfo);
int __gePutImageRotateObject(SURFACEINFO* psInfo, int nY, float theta, int* nXr, int* nYr, SURFACEINFO* psObjectInfo);

#endif