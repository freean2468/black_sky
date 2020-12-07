///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   화 일  이 름 : AnimationFile.cpp
//
//   모 듈  이 름 : 애니메이션 파일 (save, load)
//
//   제 작  목 적 : 교육용
//
//   제   작   자 : 김 방 울
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//extern SLMEMBER g_slmSprite;

// 스프라이트 연결 리스트 정보를 가지고 있는 pslm과 파일 이름인 pstrFilename을 인자로 받고 있다.
BOOL __afSave(SLMEMBER* pslm, char* pstrFilename)
{
	FILE				*fp;
	ANIFILEHEADER		AniFileHeader;
	BIN_SURFACEINFO*	pbsInfo;
	DWORD dwSizeSurface=0;
		
	////
	__slSetMember(pslm);						// pslm을 사용하기 위해 __slSetMember 설정
	AniFileHeader.nFrameCount = __slGetCount();	// 스프라이트 연결 리스트의 노드 개수를 구하여 저장
	if (AniFileHeader.nFrameCount)	// 1개 이상이면 저장 처리
	{
		memcpy(AniFileHeader.Symbol, ANIM_FILE_SYMBOL, sizeof(AniFileHeader.Symbol)); // 'ANIM'으로 복사
		//// 이진 파일 생성 준비
		fp = fopen(pstrFilename, "wb");
		fwrite(&AniFileHeader, sizeof(AniFileHeader), 1, fp); // AniFileHeader는 모두 정보가 들어 있어 구조체 크기만큼 파일에 쓴다.

		//// 연결 리스트의 현재 노드의 위치를 Head로 설정
		__slSetHeadPosition(); 
		while(pbsInfo = (BIN_SURFACEINFO*)__slGetNext()) // __slGetNext함수로 연결 리스트의 끝까지 반복
		{
			dwSizeSurface = pbsInfo->nWidth * pbsInfo->nHeight * 3; // 표면의 크기를 구하여 저장

			//pbsInfo의 데이터 멤버들을 그 크기만큼 파일에 모두 쓴다.
			fwrite(&pbsInfo->nWidth, sizeof(pbsInfo->nWidth), 1, fp);
			fwrite(&pbsInfo->nHeight, sizeof(pbsInfo->nHeight), 1, fp);
			fwrite(&pbsInfo->nCentralX, sizeof(pbsInfo->nCentralX), 1, fp);
			fwrite(&pbsInfo->nCentralY, sizeof(pbsInfo->nCentralY), 1, fp);
			fwrite(&pbsInfo->crColorKey, sizeof(pbsInfo->crColorKey), 1, fp);
			fwrite(&pbsInfo->dwDelayTime, sizeof(pbsInfo->dwDelayTime), 1, fp);
			fwrite(pbsInfo->pSurface, dwSizeSurface, 1, fp); // 표면을 표면의 크기인 dwSizeSurface만큼 쓴다.
		}
		__slGetMember(pslm);

		fclose(fp);

		return TRUE;
	}
	else return FALSE; // 변동 사항이 없으므로 __slGetMember() 호출 생략
}

// 새로 만들 스프라이트 연결 리스트를 가리킬 pslm과 읽어들일 파일 이름인 pstrFilename을 인자로 받는다.
BOOL __afLoad(SLMEMBER* pslm, char* pstrFilename)
{
	FILE				*fp;
	ANIFILEHEADER		AniFileHeader;	
	BIN_SURFACEINFO*	pbsInfo;
	DWORD dwSizeSurface=0;
		
	////
	fp = fopen(pstrFilename, "rb");
	fread(&AniFileHeader, sizeof(AniFileHeader), 1, fp); // AniFileHeader를 읽어들인다.

	//// 애니메이션파일인지 검사한다
	if (strcmp((char*)AniFileHeader.Symbol, ANIM_FILE_SYMBOL)) // 'ANIM'인지 확인
	{
		__slSetMember(pslm);	// pslm 설정
		__slSetHeadPosition();	// 연결 리스트의 현재 노드의 위치를 Head로 설정
		for (int i=0; i < AniFileHeader.nFrameCount; i++) // AniFileHeader.nFrameCount 만큼 BIN_SURFACEINFO를 읽어들인다.
		{
			// 데이터를 읽어들이기 전에 먼저 __geCreateSprBinSurface 함수로 메모리 할당
			pbsInfo = __geCreateSprBinSurface();
			
			//pbsInfo의 구조체 데이터 멤버들을 차례대로 파일로부터 읽어들인다.
			fread(&pbsInfo->nWidth, sizeof(pbsInfo->nWidth), 1, fp);
			fread(&pbsInfo->nHeight, sizeof(pbsInfo->nHeight), 1, fp);
			fread(&pbsInfo->nCentralX, sizeof(pbsInfo->nCentralX), 1, fp);
			fread(&pbsInfo->nCentralY, sizeof(pbsInfo->nCentralY), 1, fp);
			fread(&pbsInfo->crColorKey, sizeof(pbsInfo->crColorKey), 1, fp);
			fread(&pbsInfo->dwDelayTime, sizeof(pbsInfo->dwDelayTime), 1, fp);

			dwSizeSurface = pbsInfo->nWidth * pbsInfo->nHeight * 3;
			// pbsInfo->pSurface에는 표면이 저장된 메모리의 주소를 가리키므로 역시 메모리를 할당
			pbsInfo->pSurface = (BYTE*) malloc(dwSizeSurface); 
			// 할당이 되었으면 파일로부터 표면에 대한 데이터를 읽어들인다.
			fread(pbsInfo->pSurface, dwSizeSurface, 1, fp);		

			//pbsInfo를 연결 리스트에 추가
			__slAddTail(__slCreateNode(pbsInfo));
		}
		__slGetMember(pslm);

		fclose(fp);
		return TRUE;
	}
	else
	{
		fclose(fp);

		HWND hWnd = FindWindow("스프라이트 툴", "스프라이트 툴");
		MessageBox (hWnd, "애니메이션 파일이 아닙니다",
			     	"파일열기", MB_ICONEXCLAMATION | MB_OK) ;
		return FALSE;
	}
}

// 이진 데이터로 저장된 애니메이션 파일을 메모리 DC에 올리는 함수
// 이 함수는 새로 만들 스프라이트 연결 리스트를 가리킬 pslm과 읽어들일 파일 이름인 pstrFilename을 인자로 받고 있다.
// 마지막 hdc는 화면 DC 또는 후면 표면 DC가 될 수 있다.
BOOL __afLoad2MemDc(SLMEMBER* pslm, char* pstrFilename, HDC hdc)
{
	FILE				*fp;
	ANIFILEHEADER		AniFileHeader;	
	SURFACEINFO*		psInfo;
	BYTE*				pSurface;
	DWORD				dwSizeSurface=0;
		
	////
	fp = fopen(pstrFilename, "rb");
	
	// 애니메이션 파일의 헤더를 읽는다.
	fread(&AniFileHeader, sizeof(AniFileHeader), 1, fp);

	//// 애니메이션파일인지 검사한다
	if (strcmp((char*)AniFileHeader.Symbol, ANIM_FILE_SYMBOL))
	{
		__slSetMember(pslm);			// pslm 설정
		__slSetHeadPosition();			// 현재 노드의 위치를 Head로 설정
		for (int i=0; i < AniFileHeader.nFrameCount; i++)	// SURFACEINFO를 nFrameCount만큼 읽어들인다.
		{			
			psInfo = __geCreateSprSurface();							// SURFACEINFO 메모리 할당(생성)			
																		// 파일로부터 데이터를 읽어 SURFACEINFO를 채운다.
			fread(&psInfo->nWidth, sizeof(psInfo->nWidth), 1, fp);
			fread(&psInfo->nHeight, sizeof(psInfo->nHeight), 1, fp);
			fread(&psInfo->nCentralX, sizeof(psInfo->nCentralX), 1, fp);
			fread(&psInfo->nCentralY, sizeof(psInfo->nCentralY), 1, fp);
			fread(&psInfo->crColorKey, sizeof(psInfo->crColorKey), 1, fp);
			fread(&psInfo->dwDelayTime, sizeof(psInfo->dwDelayTime), 1, fp);

			dwSizeSurface = psInfo->nWidth * psInfo->nHeight * 3;
			pSurface = (BYTE*) malloc(dwSizeSurface);					// 메모리 할당
			fread(pSurface, dwSizeSurface, 1, fp);						// 표면에 대한 이진 데이터를 읽는다

			psInfo->hBmp = __geSurface2BmpHandle(psInfo, pSurface);		// 스프라이트에 대한 이진 데이터를 비트맵 핸들로 변환
			__geLoadSurface(hdc, psInfo);								// psInfo의 dcSurface에 비트맵 적재
			free(pSurface);

			__slAddTail(__slCreateNode(psInfo));						// psInfo를 연결 리스트에 추가
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