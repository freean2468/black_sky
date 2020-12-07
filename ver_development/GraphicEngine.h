///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : GraphicEngine.h
//
//   �� ��  �� �� : �׷��ȿ���
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GRAPHICENGINE_H__
#define __GRAPHICENGINE_H__

#include <windows.h>

#define SRCMASK 0x00220326 // ����ũ��NOT�����ѰͰ� �ٽ� AND����

typedef struct SURFACEINFOtag
{
	int			nWidth;
	int			nHeight;		
	int			nCentralX;		// �߽���ǥ X
	int			nCentralY;		// �߽���ǥ Y

	COLORREF	crColorKey;		// �÷�Ű
	DWORD		dwDelayTime;	// �����ð� (ms)

	HDC			dcSurface;		// ��Ʈ���� ������ִ� ǥ�� DC
	HBITMAP		hBmp;			// ��Ʈ�� �ڵ�
	HBITMAP		hOldBmp;		// ���� ��Ʈ�� �ڵ� ( dc ������ ���� )
} SURFACEINFO;

//�̹��� ������ == ǥ��
//������� ǥ�� ����
//�ϳ��� �̹����� ������ȭ�� ����ü
//�̹����� ��������Ʈ�� ����� ���� �ƴϸ� �ִϸ��̼� ��ų �� �׻� �� ����ü�� �̿��ؼ� ���
typedef struct BIN_SURFACEINFOtag
{	
	int			nWidth;			// ��
	int			nHeight;		// ����
	int			nCentralX;		// �߽���ǥ X
	int			nCentralY;		// �߽���ǥ Y
	COLORREF	crColorKey;		// �÷�Ű
	DWORD		dwDelayTime;	// �ִϸ��̼��� �ϱ� ���� �����ð� (ms)
	
	BYTE*		pSurface;		// ǥ���� �ּ�
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