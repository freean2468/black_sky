///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : GraphicEngine.cpp
//
//   �� ��  �� �� : �׷��ȿ���
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphicEngine.h"

extern SURFACEINFO g_sfSprite;
extern SURFACEINFO g_sfStarInfoBack;

extern GAMESTATE GameState;

// msimg32.lib : TransparentBlt(), AlphaBlend()

//// ��Ʈ�������� ��Ʈ���ڵ�� ��ȯ�Ͽ� �����Ѵ�
HBITMAP __geBmpFile2BmpHandle(char* pstrFilename)
{
	FILE  *fp;	
	DWORD dwSize;
	BITMAPFILEHEADER BitmapFileHeader;
	BITMAPINFO *pBitmapInfo;
	HBITMAP hBit;	
	void* pBitmap;

	//// ������ ����
	fp = fopen(pstrFilename, "rb");

	//// BITMAPFILEHEADER 	
	fread(&BitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);

	//// BITMAPINFO�� ũ�⸦ ���ϰ� �Ҵ�, �д´�
	dwSize = BitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);	// 40 bytes
	//dwSize = sizeof(BITMAPINFO);									// 44 bytes
	pBitmapInfo = (BITMAPINFO*) malloc(dwSize);

	fread(pBitmapInfo, dwSize, 1, fp);

	//// DIB ������ ����� ���� �޸𸮸� �Ҵ��Ѵ�.
	hBit=CreateDIBSection(NULL,				// HDC, DIB_PAL_COLORS�� ��츸 ���
		                  pBitmapInfo,		// BITMAPINFO ������
						  DIB_RGB_COLORS,	// ���� ��� �÷���
						  &pBitmap,			// �޸� ��巡��
						  NULL,				// ���� ���� ��ü �ڵ�
						  0);				// ���� ���� ��ü�� ��Ʈ ������

	//// �̹��� �����͸� �о���δ�.
	int nImageSize = BitmapFileHeader.bfSize - BitmapFileHeader.bfOffBits;		
	fread(pBitmap, nImageSize-2, 1, fp); // ������ �ɷ��� ������ �Ͽ��� (-2)
	
	////
	free(pBitmapInfo);

	////
	fclose(fp);

	return hBit;
}

//// BITMAPINFO ����ü ��� �����Ͱ� ���� ( �⺻������ 24bit ��� ���� )
// ���� �����ϴ� ������ �츮�� ����� �Լ��� StretchDIBits���� �� ����ü ������ ���ϱ� ����
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

//// ����ǥ������ ��Ʈ���ڵ��� ��´�. 
HBITMAP __geBinSurface2BmpHandle(BIN_SURFACEINFO* pbsInfo)
{
	HBITMAP hBit;
	BITMAPINFO BitmapInfo;	

	__geSetBitmapinfoBin(pbsInfo, &BitmapInfo, FALSE);

	void* pSurface;

	//// DIB ������ ����� ���� �޸𸮸� �Ҵ��Ѵ�.
	hBit=CreateDIBSection(NULL,					// HDC, DIB_PAL_COLORS�� ��츸 ���
		                  &BitmapInfo,			// BITMAPINFO ������
						  DIB_RGB_COLORS,		// ���� ��� �÷���
						  &pSurface,			// �޸� ��巡��
						  NULL,					// ���� ���� ��ü �ڵ�
						  0);					// ���� ���� ��ü�� ��Ʈ ������	

	memcpy(pSurface, pbsInfo->pSurface, BitmapInfo.bmiHeader.biSizeImage);
	return hBit;
}

//// BITMAPINFO ��Ʈ���� �ʱ�ȭ ( �⺻������ 24bit ��� ���� )
// �Ű� ���� psInfo�� �����Ͽ� pBitmapInfo�� ������ ������� �����Ѵ�. bMirror�� ����� �� �̷� ���θ� �Ǵ��ϴ� �뵵�� ���
// __geSurface2BmpHandle �Լ����� ȣ��
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

//// ����ǥ���� ��Ʈ���ڵ�� ��ȯ�Ѵ� 
// �Ű� ���� psInfo�� ��Ʈ�� �ڵ��� ���� SURFACEINFO�� �ǰ� pSrcSurface�� ���� ǥ���� ���� �ּҰ� �ȴ�.
// �Ű� ���� pSrcSurface�� ����Ű�� �޸𸮸� �̿��Ͽ� ��Ʈ�� �ڵ��� ��� ����.
// �� �Լ��� AnimationFile���� __afLoad2MemDc �Լ����� ȣ��ȴ�.
HBITMAP __geSurface2BmpHandle(SURFACEINFO* psInfo, BYTE* pSrcSurface)
{
	HBITMAP hBit=0;
	BITMAPINFO BitmapInfo;	

	__geSetBitmapinfo(psInfo, &BitmapInfo, TRUE);

	void* pSurface;

	//// DIB ������ ����� ���� �޸𸮸� �Ҵ��Ѵ�.
	hBit=CreateDIBSection(NULL,					// HDC, DIB_PAL_COLORS�� ��츸 ���
		                  &BitmapInfo,			// BITMAPINFO ������
						  DIB_RGB_COLORS,		// ���� ��� �÷���
						  &pSurface,			// �޸� ��巡��
						  NULL,					// ���� ���� ��ü �ڵ�
						  0);					// ���� ���� ��ü�� ��Ʈ ������	

	memcpy(pSurface, pSrcSurface, BitmapInfo.bmiHeader.biSizeImage);
	return hBit;
}

//// ��Ʈ�������� ����ǥ�� ������ ��ȯ�Ѵ�
BIN_SURFACEINFO* __geBmpFile2BinSurface(char* pstrFilename)
{
	FILE			 *fp;	
	DWORD			 dwSize;
	BITMAPFILEHEADER BitmapFileHeader;
	BITMAPINFO		 *pBitmapInfo;

	//// ������ ����
	fp = fopen(pstrFilename, "rb");

	// BITMAPFILEHEADER�� �д´�.
	fread(&BitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);

	//// BITMAPINFO�� ũ�⸦ ���ϰ� �Ҵ�, �д´�
	dwSize = BitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);	    // 40 bytes
	//dwSize = sizeof(BITMAPINFO);										// 44 bytes
	pBitmapInfo = (BITMAPINFO*) malloc(dwSize);
	fread(pBitmapInfo, dwSize, 1, fp);

	//// �̹��� �����͸� �о���δ�.		
	int nImageSize = BitmapFileHeader.bfSize - BitmapFileHeader.bfOffBits;	

	// BIN_SURFACEINFO�� �����Ѵ�.
	BIN_SURFACEINFO* pbsfBitmap = __geCreateSprBinSurface(); // �޸� ���� �Ҵ�

	//�̹��� ũ�⸸ŭ �޸𸮸� �Ҵ��Ѵ�
	pbsfBitmap->pSurface = (BYTE*) malloc(nImageSize);

	//// �Ųٷ� �о���δ� ( ������ �̹����� �ٲ۴� )
	//fread(pbsfBitmap->pSurface, nImageSize, 1, fp);	// �׳� �д� ���
	int nBmpWidth  = pBitmapInfo->bmiHeader.biWidth;			// ��
	int nBmpHeight = pBitmapInfo->bmiHeader.biHeight;			// ����
	int nPixelSize = pBitmapInfo->bmiHeader.biBitCount / 8;		// �ȼ� ����Ʈ ũ��
	int nBmpWidthByte = nBmpWidth * nPixelSize;					// ��Ʈ�� ���� ����Ʈ ��
	int nHorizonOffset = nBmpHeight * nBmpWidthByte;			// ������ ���� ������ (������->������ ��迭�ϱ� ����)

	for (int iH=0; iH < nBmpHeight; iH++){
		nHorizonOffset -= nBmpWidthByte;
		fread(pbsfBitmap->pSurface+nHorizonOffset, nBmpWidthByte, 1, fp);
	}
	//���� �ݱ�
	fclose(fp);

	//// ��������Ʈ ������ �ٷ� ���� ǥ�� ���� & �ʱ�ȭ
	__geSetSprBinSurface(pbsfBitmap, pBitmapInfo->bmiHeader.biWidth, pBitmapInfo->bmiHeader.biHeight, 0);
	
	////
	free(pBitmapInfo);

	return pbsfBitmap;
}

//// hdc(ȭ��DC)�� ȣȯ�Ǵ� �ĸ�ǥ��DC ����
// psInfo�� ������ �ĸ� ǥ���� SURFACEINFO�� �ȴ�. ��, �ƿ� �Ķ���Ͱ� �ȴ�.
BOOL __geCreateBackBuffer(HDC hdc, int nWidth, int nHeight, int nBPP, SURFACEINFO* psInfo, COLORREF crColorKey)
{
	psInfo->dcSurface = CreateCompatibleDC(hdc);		// �޸� DC�� ����, �� ��, ������ �޸� DC�� ���� ����� �� �ִ� ũ�⸦ �������� �ʴ´�. (�� 1X1 ũ���̴�.)
	psInfo->hBmp	  = CreateCompatibleBitmap (hdc, nWidth, nHeight); // nWidth, nHeight ũ�⸦ ������ hdc�� ȣȯ�Ǵ� ��Ʈ�� ����
    psInfo->hOldBmp   = (HBITMAP) SelectObject (psInfo->dcSurface, psInfo->hBmp );	// hBmp�� �޸� DC// ����纯���÷� ��� �Լ� ����, �׷��� �޸�DC�� hBmp�� ������ ũ��� �ȼ� ũ��, ������ ������.
	PatBlt(psInfo->dcSurface, 0,0, nWidth, nHeight, PATCOPY);
	psInfo->nWidth    = nWidth;
	psInfo->nHeight   = nHeight;
	psInfo->nCentralX = nWidth / 2;
	psInfo->nCentralY = nHeight / 2; 
	psInfo->crColorKey = crColorKey;

	return TRUE;
}

//// ǥ�� ���� �� �ε�
//// ���� ������Ʈ �Ǵ� ��Ʈ�� DC ���� �� ��Ʈ�� �ε�
// ��Ʈ�� �ڵ��� ������ �ִ� SURFACEINFO�� �޸� DC�� ��� SURFACEINFO ����ü�� ������ ��� dcSurface�� �����Ѵ�.
// psInfo�� �޸� DC�� ���� SURFACEINFO�� �ȴ�. ��, ��Ʈ�� �ڵ��� ������ �־�� �Ѵ�.
void __geLoadSurface(HDC hdc, SURFACEINFO* psInfo)
{	
	BITMAP  bit;
	
	psInfo->dcSurface = CreateCompatibleDC(hdc);	// �޸� DC�� ����
	//hBmp�� �޸� DC�� dcSurface�� �����Ѵ�. �׷��� dcSurface�� hBmp�� ������ ũ��� �ȼ� ũ��, ������ ������ �ǰ� hBmp�� dcSurface�� ����ȴ�.
	psInfo->hOldBmp = (HBITMAP) SelectObject(psInfo->dcSurface, psInfo->hBmp);

	// ��Ʈ���� ũ�⸦ ��� pbInfo�� nWidth�� nHeight�� �ʱ�ȭ�Ѵ�.
	GetObject(psInfo->hBmp, sizeof(BITMAP), &bit);	
	psInfo->nWidth  = bit.bmWidth;
	psInfo->nHeight = bit.bmHeight;	
	psInfo->nCentralX = bit.bmWidth / 2;
	psInfo->nCentralY = bit.bmHeight/ 2;
}

//// ������ ǥ�� ����
// �Ű� ���� psInfo�� ������ų ǥ���� ���� �ּҰ� �ȴ�.
void __geReleaseSurface(SURFACEINFO* psInfo)
{
	SelectObject(psInfo->dcSurface, psInfo->hOldBmp); //dcSurface�� ������ ��Ʈ�� �ڵ� hOldBmp�� �����Ѵ�.
	DeleteDC(psInfo->dcSurface);					  // �޸� ����
	DeleteObject(psInfo->hBmp);						  // ��Ʈ�� �ڵ� hBmp�� ����
}

//// ������ ǥ�� ����
void __geReleaseBinSurface(BIN_SURFACEINFO* pbsInfo)
{
	if (pbsInfo)
	{
		if (pbsInfo->pSurface) 
		{
			free(pbsInfo->pSurface);
			pbsInfo->pSurface = NULL;			// ���߿� ������ ��츦 �����ؼ� �ʱ�ȭ
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

//// �̹��� ���
// ǥ�� DC�� ���� DC�� BitBlt�Ѵ�.
// �Ű� ���� dcDst�� ���� DC�� �ȴ�. ȭ�� DC�� �� ���� �ְ� �ĸ� ǥ�� DC�� �� ���� �ִ�. �ƴϸ�, �Ǵٸ� ǥ�� DC�� �� ���� �ִ�.
// x�� y�� ��� ��ǥ�� �ȴ�. �������� psInfo�� ����� SURFACEINFO ����ü�̴�.
BOOL __gePutImage(HDC dcDst, int nx1, int ny1, int nx2, int ny2, SURFACEINFO* psInfo)
{
	if (psInfo)	// ���� psInfo�� ��ȿ�ϸ� BitBlt �Լ��� ǥ�� DC�� dcSurface�� �״�� dcDst�� �����Ѵ�.
		return BitBlt(dcDst, nx1, ny1, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, nx2, ny2, SRCCOPY);
	else return FALSE;
}

BOOL __gePutImageAND(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	if (psInfo)	// ���� psInfo�� ��ȿ�ϸ� BitBlt �Լ��� ǥ�� DC�� dcSurface�� AND�����Ͽ� dcDst�� �����Ѵ�.
		return BitBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCAND);
	else return FALSE;
}

BOOL __gePutImageMASK(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	if (psInfo)	// ���� psInfo�� ��ȿ�ϸ� BitBlt �Լ��� ǥ�� DC�� dcSurface�� MASK�����Ͽ� dcDst�� �����Ѵ�.
		return BitBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCMASK);
	else return FALSE;
}

BOOL __gePutImageOR(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	if (psInfo)	// ���� psInfo�� ��ȿ�ϸ� BitBlt �Լ��� ǥ�� DC�� dcSurface�� OR�����Ͽ� dcDst�� �����Ѵ�.
		return BitBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCPAINT);
	else return FALSE;
}

//// BIN_SURFACEINFO ����ü�� �̿��Ͽ� DeviceContext�� �̹����� ����ϴ� �Լ�
int  __gePutImageBin(HDC dcDst, int x, int y, BIN_SURFACEINFO* pbsInfo)
{
	if (pbsInfo)
	{
		BITMAPINFO BitmapInfo;		
		//BitmapInfo�� ����. 
		//������ ���ڴ� �̷� ���� ������ : TRUE
		//API�� ��Ʈ�� ��� �Լ����� ���������� ����� ���� �����Ͽ� 
		//�̷� ���θ� ���ϱ� ������ ���������� ����� ����� TRUE
		__geSetBitmapinfoBin(pbsInfo, &BitmapInfo, TRUE);
		//�̹��� ���
		return ::StretchDIBits(dcDst,
							   x, y, pbsInfo->nWidth, pbsInfo->nHeight,
							   0, 0, pbsInfo->nWidth, pbsInfo->nHeight,
							   pbsInfo->pSurface, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
	else return FALSE;
}

//// �̹��� ���� 
// ǥ�� DC�� ���� DC�� ������ ������ �̹����� AlphaBlend �Ѵ�.
// ������ �Ű� ���� nAlpha�� ������ �ڴϴ� ���̴�. 0~255�� �������� ������ 0�̸� ���� ���� 255�̸� �������� �ȴ�.
BOOL __gePutImageBlend(HDC dcDst, int x, int y, SURFACEINFO* psInfo, int nAlpha)
{
	BLENDFUNCTION bf;

	//// BLENDFUNCTION �ʱ�ȭ
	bf.BlendOp     = AC_SRC_OVER;	//
	bf.BlendFlags  = 0;				//			
	bf.AlphaFormat = 0;				//
	bf.SourceConstantAlpha = nAlpha; // 0 - 255

	//// ���� ����
	return 	AlphaBlend(dcDst, 0, 0, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, x, y, psInfo->nWidth, psInfo->nHeight, bf);
}

//// ��������Ʈ
// ǥ�� DC�� ���� DC�� ��������Ʈ ����Ѵ�.
BOOL __gePutSprite(HDC dcDst, int x, int y, int nx, int ny, SURFACEINFO* psInfo)
{	
	return TransparentBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, nx, ny, psInfo->nWidth, psInfo->nHeight,
						  psInfo->crColorKey);
}

//// hdc�� ǥ���� ���� (�ĸ�ǥ�� --����--> ȭ��DC)
// �ĸ� ǥ�� DC�� ȭ�� DC�� ��Ʈ�ϴ� �Լ��̴�.
// �Ű� ���� hdc�� ȭ�� DC�� �ǰ� psInfo�� �ĸ� ǥ���� SURFACEINFO����ü�� �ȴ�.
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


//// SURFACEINFO �̹��� �ʱ�ȭ
// ������ ǥ���� �ʱ�ȭ�� �� ���
void __geSetImgSurface(SURFACEINFO* psfInfo)
{
	memset(psfInfo, 0, sizeof(SURFACEINFO));
}

//// SURFACEINFO ��������Ʈ �ʱ�ȭ
// ������ ǥ���� �ʱ�ȭ�� �� ���, �÷�Ű�� �Ű� ������ �޾Ƽ� �ʱ�ȭ�� �ִ� �͸� �ٸ���.
void __geSetSprSurface(SURFACEINFO* psfInfo, COLORREF crColorKey)
{
	memset(psfInfo, 0, sizeof(SURFACEINFO));	
	psfInfo->crColorKey = crColorKey;
}

BIN_SURFACEINFO* __geCreateSprBinSurface()
{
	return (BIN_SURFACEINFO*) malloc(sizeof(BIN_SURFACEINFO));
}

// SURFACEINFO ����ü ũ�⸸ŭ �޸𸮸� �Ҵ��ϰ� �� �ּҸ� ����, ó�� ǥ���� ������ �� ȣ���Ѵ�.
SURFACEINFO* __geCreateSprSurface()
{
	return (SURFACEINFO*) malloc(sizeof(SURFACEINFO));
}

//// BIN_SURFACEINFO ������ ��� �ʱ�ȭ
// pbsfInfo ����ü�� �����ͷ� �����Ͽ� �� ���� �ּҸ� �ް� �ִµ�, ����ü�� ũ�Ⱑ �ƹ��� Ŀ��
// �ּҰ� ���ڷ� �Ѿ�Ƿ� ���ÿ� ���ʿ��� �޸� �Ҵ�� ���簡 �̷������ �ʴ´�.
// ����, ����ü�� ���ڷ� �޾� ó���ϴ� ��쿡�� �����ͷ� �����Ͽ� �ּҸ� �޴� ���� �ӵ���
// �޸� ��� ���鿡�� ȿ�����̴�.
void __geSetSprBinSurface(BIN_SURFACEINFO* pbsfInfo, int nWidth, int nHeight, COLORREF crColorKey)
{
	pbsfInfo->nWidth = nWidth;
	pbsfInfo->nHeight = nHeight;
	pbsfInfo->crColorKey = crColorKey;

	pbsfInfo->nCentralX = nWidth / 2;
	pbsfInfo->nCentralY = nHeight / 2;
	pbsfInfo->dwDelayTime = 100;		//ms
}

// BIN_SURFACEINFO �̹������� ���ϴ� ��ġ�� �ȼ� ������ ��� ���� �Լ�
COLORREF __geGetPixel(BIN_SURFACEINFO* pbsInfo, int x, int y)
{
	COLORREF colorPixel = 0;	//32��Ʈ ũ���̹Ƿ� �����ϱ⿡ �׻� 0���� �ʱ�ȭ

	BYTE* pSurface = pbsInfo->pSurface;	// pSurface�� �̹����� ���� �ּҸ� ����Ű�� �ִ�.
	//���ϴ� ��ġ�κ��� 3����Ʈ��ŭ colorPixel�� ����
	//�̹����� �޸𸮿� ����Ǿ� �ִµ�, �� �޸𸮴� 1���� �迭�� �����ϰ� ����
	// ��, �̹����� 2���������� ǥ�õȴٰ� �ϴ��� �޸𸮻󿡴� 1���������� ����Ǿ� �ִ�.
	// �̹������� ���ϴ� ��ġ�� �ȼ��� ���� �˾Ƴ��� ���� ������ ����ؾ� �Ѵ�.
	// ���ϴ� ��ġ : x, y
	// ���� : pSurface(�̹����� ���� �ּ�) + y*(�̹����� ������*(�ȼ�����Ʈ��)) + (x*�ȼ�����Ʈ��)
	memcpy(&colorPixel, pSurface + y * pbsInfo->nWidth*3 + x*3, 3);

	return colorPixel;
}

//// ��ġ�� �����͸� ��´�
BYTE* __geGetBitsPtr(BIN_SURFACEINFO* pbsInfo, int x, int y)
{
	return pbsInfo->pSurface + y * pbsInfo->nWidth*3 + x*3;
}

////
RECT __geGetCuttingRect(HWND hWnd, RECT rcDrag, BIN_SURFACEINFO* pbsInfo)
{	
	////
	COLORREF   colorPixel;

    //// ���⿵�� ���� ( �������� �巡�� ���� )
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
        // nMinX (���� �� ��)
        for (ix=rcDrag.left; ix < rcDrag.right; ix++)
        {
            colorPixel = __geGetPixel(pbsInfo, ix, iy);
			if (colorPixel != pbsInfo->crColorKey && ix < nMinX)
			{
				nMinX = ix;
				break;
			}
        }
        // nMaxX (������ �� ��)
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
        // nMinY (���� �� ��) 
        for (iy=rcDrag.top; iy < rcDrag.bottom; iy++)
        {
            colorPixel = __geGetPixel(pbsInfo, ix, iy);
            if (colorPixel != pbsInfo->crColorKey && iy < nMinY)
			{
				nMinY = iy;
				break;
			}
        }
        // nMaxY (�Ʒ��� �� ��)
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

	//// ��ǥ����
	RECT rcFrame = {nMinX, nMinY, nMaxX+1, nMaxY+1};

	return rcFrame;
}

//// 4�� ����� �Ǵ� �ּ� ǥ���� �����
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
	
	nWidthRevision = ((nWidthRevision + 3) & ~3);		// 4�� ���
	nSurfaceSize   = nWidthRevision * nHeight * 3;
	pSurface       = (BYTE*) malloc(nSurfaceSize);

	//// �޸� �÷�Ű �ʱ�ȭ
	for (int i = 0; i < nSurfaceSize; i+=3) ::memcpy(pSurface+i, btColorKey, 3);

	//// 4�� ��� ������ ���� ������ ����
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
	POINT apt[3] = {0};											// ����纯���� ������ ���� ����

	float Radis     = DegreesToRads(theta);                     // ȸ���� ����(360�й�)��, ���ȴ����� ������ ����   
    float cosine    = (float)cos(Radis);                        // ��Ÿ���� ��ȭ�� ���� cos ���� ����          
    float sine      = (float)sin(Radis);                        // ��Ÿ���� ��ȭ�� ���� sin ���� ���� 
	
	double dblSizeRatio=1;										// ������ ������ ����(1�̸� ���� ũ��, 0.5�� ���� ũ�� �̷����̴�.)
	double dblx, dbly;											// ȸ������ �������� ���� ���������� �����ǥ ���� ����
	double dblWidth = psInfo->nWidth*dblSizeRatio;				// ����� ��Ʈ�� �ʺ�, ���� ũ��
	double dblHeight = psInfo->nHeight*dblSizeRatio;   
	double dblxDest, dblyDest;									// ȸ���� ��ġ��ǥ(�����ǥ) ���� ����
	int ixDisplay = SCR_WIDTH/2;								// DC�� ǥ���� X��ǥ(���ǻ� �� ��ǥ�� �������� ��Ʈ���� DC�� �׸���.)
	int iyDisplay = SCR_HEIGHT;									// DC�� ǥ���� Y��ǥ(���ǻ� �� ��ǥ�� �������� ��Ʈ���� DC�� �׸���.)
	double ixRotate = psInfo->nWidth/2 * dblSizeRatio;			// ȸ�� X��, Y��
	double iyRotate = psInfo->nHeight/2 * dblSizeRatio;		

	// �������� �󿡼� �� ���� ��½� �׿� �°� �߽����� ������ �Ѵ�. DC ũ�⿡ �µ��� ����
	if(GameState == STAGE_SELECT){
		ixDisplay = g_sfStarInfoBack.nWidth/2;
		iyDisplay = g_sfStarInfoBack.nHeight/2;		
	}

	// ���� �ý��� �ӵ� ����ȭ�� ���� �־���� ������ �ణ ������״�.
	// ���� ���� ������ �Ʒ��̸�, ���̰� �ñ��ϴٸ� �Ʒ��� �ּ��� Ǯ� ���� ����.
	// double dblWidth = psBGInfo->nWidth*dblSizeRatio;		
	// double dblHeight = psBGInfo->nHeight*dblSizeRatio;   
	// ixRotate = psBGInfo->nWidth/2 * dblSizeRatio
	// iyRotate = psBGInfo->nheight/2 * dblSizeRatio
	
	// 1. ȸ������ �������� �����ǥ�� ���ϰ� 
	// 2. ȸ���� ��ġ��ǥ(�����ǥ)�� ���� ��
	// 3. ���� ���� ������ ��ǥ�� ����.
	for(int i=0 ; i<3 ; i++){
		if(i==0){dblx=-ixRotate, dbly=-iyRotate;}				// left up ������ �κ�
		else if(i==1){dblx=dblWidth-ixRotate, dbly=-iyRotate;}	// right up ������ �κ�
		else if(i==2){dblx=-ixRotate, dbly=dblHeight-iyRotate;} // left low ������ �κ�

		dblxDest=dblx*cosine-dbly*sine;
		dblyDest=dblx*sine+dbly*cosine;
		dblxDest+=ixRotate, dblyDest+=iyRotate;
		apt[i].x= ixDisplay - (long)ixRotate+(long)dblxDest;
		apt[i].y= iyDisplay -(long)iyRotate+(long)dblyDest;
	}

	// ����纯���÷� ��� �Լ�
	// ���� �߿��� �μ��� �ι�° �μ�
	// �� ���� ��ǥ�� ���� ��� ����� �����Ѵ�.
	// �ӵ��� �ʹ� ������
	PlgBlt(psInfo->dcSurface, apt, psBGInfo->dcSurface, 0, 0, psBGInfo->nWidth, psBGInfo->nHeight, 0, 0, 0);	

	return true;
}


int __gePutImageRotateObject(SURFACEINFO* psInfo, int nY, float theta, int* nXr, int* nYr, SURFACEINFO* psObjectInfo){
	POINT apt[3] = {0};											// ����纯���� ������ ���� ����

	float Radis     = DegreesToRads(theta);                     // ȸ���� ����(360�й�)��, ���ȴ����� ������ ����   
    float cosine    = (float)cos(Radis);                        // ��Ÿ���� ��ȭ�� ���� cos ���� ����          
    float sine      = (float)sin(Radis);                        // ��Ÿ���� ��ȭ�� ���� sin ���� ���� 
	
	double dblSizeRatio=1;										// ������ ������ ����(1�̸� ���� ũ��, 0.5�� ���� ũ�� �̷����̴�.)
	double dblx, dbly;											// ȸ������ �������� ���� ���������� �����ǥ ���� ����
	double dblWidth = psObjectInfo->nWidth*dblSizeRatio;		// ����� ��Ʈ�� �ʺ�, ���� ũ��
	double dblHeight = psObjectInfo->nHeight*dblSizeRatio;   
	double dblxDest, dblyDest;									// ȸ���� ��ġ��ǥ(�����ǥ) ���� ����
	int ixDisplay = SCR_WIDTH/2;								// DC�� ǥ���� X��ǥ(���ǻ� �� ��ǥ�� �������� ��Ʈ���� DC�� �׸���.)
	int iyDisplay = SCR_HEIGHT;									// DC�� ǥ���� Y��ǥ(���ǻ� �� ��ǥ�� �������� ��Ʈ���� DC�� �׸���.)
	double ixRotate = psObjectInfo->nWidth/2 * dblSizeRatio;	// ȸ�� X��, Y��
	double iyRotate = nY+psObjectInfo->nHeight/2 * dblSizeRatio;		// �������κ��� �Ÿ��� ����� ���� ��� ����
	
    // �������� �󿡼� �� ���� ��½� �׿� �°� �߽����� ������ �Ѵ�. DC ũ�⿡ �µ��� ����
	if(GameState == STAGE_SELECT){
		ixDisplay = g_sfStarInfoBack.nWidth/2;
		iyDisplay = g_sfStarInfoBack.nHeight/2;		
	}

	// 1. ȸ������ �������� �����ǥ�� ���ϰ� 
	// 2. ȸ���� ��ġ��ǥ(�����ǥ)�� ���� ��
	// 3. ���� ���� ������ ��ǥ�� ����.
	for(int i=0 ; i<3 ; i++){
		if(i==0){dblx=-ixRotate, dbly=-iyRotate;}				// left up ������ �κ�
		else if(i==1){dblx=dblWidth-ixRotate, dbly=-iyRotate;}	// right up ������ �κ�
		else if(i==2){dblx=-ixRotate, dbly=dblHeight-iyRotate;} // left low ������ �κ�

		dblxDest=dblx*cosine-dbly*sine;
		dblyDest=dblx*sine+dbly*cosine;
		dblxDest+=ixRotate, dblyDest+=iyRotate;
		apt[i].x= ixDisplay -(long)ixRotate+(long)dblxDest;
		apt[i].y= iyDisplay -(long)iyRotate+(long)dblyDest;
		
		if(i==0){								// ȸ�� �� ������Ʈ�� ��ǥ�� ���Ѵ�. �߽� ��ǥ�� �����ؼ� ��ǥ�� ���Ѵ�.(���� ����)
			(*nXr) = apt[i].x + psObjectInfo->nCentralX, (*nYr) = apt[i].y + psObjectInfo->nCentralY;
		}			
	}

	// ����纯���÷� ��� �Լ�
	// ���� �߿��� �μ��� �ι�° �μ�
	// �� ���� ��ǥ�� ���� ��� ����� �����Ѵ�.
	// �ӵ��� �ʹ� ������
	PlgBlt(psInfo->dcSurface, apt, psObjectInfo->dcSurface, 0, 0, psObjectInfo->nWidth, psObjectInfo->nHeight, 0, 0, 0);	

	return true;
}


















