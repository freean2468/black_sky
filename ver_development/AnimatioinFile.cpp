///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ȭ ��  �� �� : AnimationFile.cpp
//
//   �� ��  �� �� : �ִϸ��̼� ���� (save, load)
//
//   �� ��  �� �� : ������
//
//   ��   ��   �� : �� �� ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//extern SLMEMBER g_slmSprite;

// ��������Ʈ ���� ����Ʈ ������ ������ �ִ� pslm�� ���� �̸��� pstrFilename�� ���ڷ� �ް� �ִ�.
BOOL __afSave(SLMEMBER* pslm, char* pstrFilename)
{
	FILE				*fp;
	ANIFILEHEADER		AniFileHeader;
	BIN_SURFACEINFO*	pbsInfo;
	DWORD dwSizeSurface=0;
		
	////
	__slSetMember(pslm);						// pslm�� ����ϱ� ���� __slSetMember ����
	AniFileHeader.nFrameCount = __slGetCount();	// ��������Ʈ ���� ����Ʈ�� ��� ������ ���Ͽ� ����
	if (AniFileHeader.nFrameCount)	// 1�� �̻��̸� ���� ó��
	{
		memcpy(AniFileHeader.Symbol, ANIM_FILE_SYMBOL, sizeof(AniFileHeader.Symbol)); // 'ANIM'���� ����
		//// ���� ���� ���� �غ�
		fp = fopen(pstrFilename, "wb");
		fwrite(&AniFileHeader, sizeof(AniFileHeader), 1, fp); // AniFileHeader�� ��� ������ ��� �־� ����ü ũ�⸸ŭ ���Ͽ� ����.

		//// ���� ����Ʈ�� ���� ����� ��ġ�� Head�� ����
		__slSetHeadPosition(); 
		while(pbsInfo = (BIN_SURFACEINFO*)__slGetNext()) // __slGetNext�Լ��� ���� ����Ʈ�� ������ �ݺ�
		{
			dwSizeSurface = pbsInfo->nWidth * pbsInfo->nHeight * 3; // ǥ���� ũ�⸦ ���Ͽ� ����

			//pbsInfo�� ������ ������� �� ũ�⸸ŭ ���Ͽ� ��� ����.
			fwrite(&pbsInfo->nWidth, sizeof(pbsInfo->nWidth), 1, fp);
			fwrite(&pbsInfo->nHeight, sizeof(pbsInfo->nHeight), 1, fp);
			fwrite(&pbsInfo->nCentralX, sizeof(pbsInfo->nCentralX), 1, fp);
			fwrite(&pbsInfo->nCentralY, sizeof(pbsInfo->nCentralY), 1, fp);
			fwrite(&pbsInfo->crColorKey, sizeof(pbsInfo->crColorKey), 1, fp);
			fwrite(&pbsInfo->dwDelayTime, sizeof(pbsInfo->dwDelayTime), 1, fp);
			fwrite(pbsInfo->pSurface, dwSizeSurface, 1, fp); // ǥ���� ǥ���� ũ���� dwSizeSurface��ŭ ����.
		}
		__slGetMember(pslm);

		fclose(fp);

		return TRUE;
	}
	else return FALSE; // ���� ������ �����Ƿ� __slGetMember() ȣ�� ����
}

// ���� ���� ��������Ʈ ���� ����Ʈ�� ����ų pslm�� �о���� ���� �̸��� pstrFilename�� ���ڷ� �޴´�.
BOOL __afLoad(SLMEMBER* pslm, char* pstrFilename)
{
	FILE				*fp;
	ANIFILEHEADER		AniFileHeader;	
	BIN_SURFACEINFO*	pbsInfo;
	DWORD dwSizeSurface=0;
		
	////
	fp = fopen(pstrFilename, "rb");
	fread(&AniFileHeader, sizeof(AniFileHeader), 1, fp); // AniFileHeader�� �о���δ�.

	//// �ִϸ��̼��������� �˻��Ѵ�
	if (strcmp((char*)AniFileHeader.Symbol, ANIM_FILE_SYMBOL)) // 'ANIM'���� Ȯ��
	{
		__slSetMember(pslm);	// pslm ����
		__slSetHeadPosition();	// ���� ����Ʈ�� ���� ����� ��ġ�� Head�� ����
		for (int i=0; i < AniFileHeader.nFrameCount; i++) // AniFileHeader.nFrameCount ��ŭ BIN_SURFACEINFO�� �о���δ�.
		{
			// �����͸� �о���̱� ���� ���� __geCreateSprBinSurface �Լ��� �޸� �Ҵ�
			pbsInfo = __geCreateSprBinSurface();
			
			//pbsInfo�� ����ü ������ ������� ���ʴ�� ���Ϸκ��� �о���δ�.
			fread(&pbsInfo->nWidth, sizeof(pbsInfo->nWidth), 1, fp);
			fread(&pbsInfo->nHeight, sizeof(pbsInfo->nHeight), 1, fp);
			fread(&pbsInfo->nCentralX, sizeof(pbsInfo->nCentralX), 1, fp);
			fread(&pbsInfo->nCentralY, sizeof(pbsInfo->nCentralY), 1, fp);
			fread(&pbsInfo->crColorKey, sizeof(pbsInfo->crColorKey), 1, fp);
			fread(&pbsInfo->dwDelayTime, sizeof(pbsInfo->dwDelayTime), 1, fp);

			dwSizeSurface = pbsInfo->nWidth * pbsInfo->nHeight * 3;
			// pbsInfo->pSurface���� ǥ���� ����� �޸��� �ּҸ� ����Ű�Ƿ� ���� �޸𸮸� �Ҵ�
			pbsInfo->pSurface = (BYTE*) malloc(dwSizeSurface); 
			// �Ҵ��� �Ǿ����� ���Ϸκ��� ǥ�鿡 ���� �����͸� �о���δ�.
			fread(pbsInfo->pSurface, dwSizeSurface, 1, fp);		

			//pbsInfo�� ���� ����Ʈ�� �߰�
			__slAddTail(__slCreateNode(pbsInfo));
		}
		__slGetMember(pslm);

		fclose(fp);
		return TRUE;
	}
	else
	{
		fclose(fp);

		HWND hWnd = FindWindow("��������Ʈ ��", "��������Ʈ ��");
		MessageBox (hWnd, "�ִϸ��̼� ������ �ƴմϴ�",
			     	"���Ͽ���", MB_ICONEXCLAMATION | MB_OK) ;
		return FALSE;
	}
}

// ���� �����ͷ� ����� �ִϸ��̼� ������ �޸� DC�� �ø��� �Լ�
// �� �Լ��� ���� ���� ��������Ʈ ���� ����Ʈ�� ����ų pslm�� �о���� ���� �̸��� pstrFilename�� ���ڷ� �ް� �ִ�.
// ������ hdc�� ȭ�� DC �Ǵ� �ĸ� ǥ�� DC�� �� �� �ִ�.
BOOL __afLoad2MemDc(SLMEMBER* pslm, char* pstrFilename, HDC hdc)
{
	FILE				*fp;
	ANIFILEHEADER		AniFileHeader;	
	SURFACEINFO*		psInfo;
	BYTE*				pSurface;
	DWORD				dwSizeSurface=0;
		
	////
	fp = fopen(pstrFilename, "rb");
	
	// �ִϸ��̼� ������ ����� �д´�.
	fread(&AniFileHeader, sizeof(AniFileHeader), 1, fp);

	//// �ִϸ��̼��������� �˻��Ѵ�
	if (strcmp((char*)AniFileHeader.Symbol, ANIM_FILE_SYMBOL))
	{
		__slSetMember(pslm);			// pslm ����
		__slSetHeadPosition();			// ���� ����� ��ġ�� Head�� ����
		for (int i=0; i < AniFileHeader.nFrameCount; i++)	// SURFACEINFO�� nFrameCount��ŭ �о���δ�.
		{			
			psInfo = __geCreateSprSurface();							// SURFACEINFO �޸� �Ҵ�(����)			
																		// ���Ϸκ��� �����͸� �о� SURFACEINFO�� ä���.
			fread(&psInfo->nWidth, sizeof(psInfo->nWidth), 1, fp);
			fread(&psInfo->nHeight, sizeof(psInfo->nHeight), 1, fp);
			fread(&psInfo->nCentralX, sizeof(psInfo->nCentralX), 1, fp);
			fread(&psInfo->nCentralY, sizeof(psInfo->nCentralY), 1, fp);
			fread(&psInfo->crColorKey, sizeof(psInfo->crColorKey), 1, fp);
			fread(&psInfo->dwDelayTime, sizeof(psInfo->dwDelayTime), 1, fp);

			dwSizeSurface = psInfo->nWidth * psInfo->nHeight * 3;
			pSurface = (BYTE*) malloc(dwSizeSurface);					// �޸� �Ҵ�
			fread(pSurface, dwSizeSurface, 1, fp);						// ǥ�鿡 ���� ���� �����͸� �д´�

			psInfo->hBmp = __geSurface2BmpHandle(psInfo, pSurface);		// ��������Ʈ�� ���� ���� �����͸� ��Ʈ�� �ڵ�� ��ȯ
			__geLoadSurface(hdc, psInfo);								// psInfo�� dcSurface�� ��Ʈ�� ����
			free(pSurface);

			__slAddTail(__slCreateNode(psInfo));						// psInfo�� ���� ����Ʈ�� �߰�
		}
		__slGetMember(pslm);

		fclose(fp);
		return TRUE;
	}
	else
	{
		fclose(fp);
		return FALSE;
	}
}