#include <dshow.h>
#include <stdio.h>
#include "ControlFilters.h"

void main()
{
	HRESULT hr;
	IGraphBuilder * pFGM = NULL;
	IBaseFilter * pSrcFilter = NULL;
	IBaseFilter * pWMAudioDecoderDMO = NULL;
	IBaseFilter * pDSoundRenderer = NULL;
	IMediaControl * pMC = NULL;
	IMediaEvent   *pEvent = NULL;

	const UINT nMaxLen = 256;
	WCHAR wszFileName[nMaxLen] = {0, };

	//---------------------------------------------------
	// COM 라이브러리를 초기화한다.

	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		printf("COM 라이브러리를 초기화할 수 없습니다.\n");
		return;
	}

	//---------------------------------------------------
	// 재생할 MP3 파일 이름을 사용자로부터 받는다.
	
	printf("재생할 파일의 이름을 입력하십시오: ");
	_getws_s(wszFileName, nMaxLen);

	//---------------------------------------------------
	// 필터 그래프 매니저 생성

	hr = CoCreateInstance(
		CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (LPVOID*)&pFGM);
	if (FAILED(hr))
	{
		printf("필터 그래프 매니저를 생성할 수 없습니다.\n");
		CoUninitialize();
		return;
	}

	printf("필터 그래프 매니저가 생성되었습니다.\n");

	//---------------------------------------------------
	// MP3 파일을 재생하기 위한 필터 생성

	hr = pFGM->AddSourceFilter(wszFileName, wszFileName, &pSrcFilter);
	if (FAILED(hr))
	{
		printf("소스 필터를 생성할 수 없습니다.\n");
		goto ReleaseAll;
	}

	hr = CoCreateInstance(
			CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter, (LPVOID*)&pDSoundRenderer);
	if (FAILED(hr))
	{
		printf("DirectSound Renderer 필터를 생성할 수 없습니다.\n");
		goto ReleaseAll;
	}

	printf("필터들이 생성되었습니다.\n");

	//---------------------------------------------------
	// 생성된 필터들을 필터 그래프에 추가한다.

	hr = pFGM->AddFilter(pDSoundRenderer, NULL);

	printf("필터들이 필터 그래프에 추가되었습니다.\n");

	//---------------------------------------------------
	// 생성된 필터들을 연결한다. (두 필터 연결시 중간 필터 생성)

	hr = ConnectFilters(pFGM, pSrcFilter, pDSoundRenderer);
	if (FAILED(hr))
	{
		printf("소스 필터와 WMAudio Decoder DMO 필터를 연결할 수 없습니다.\n");
		goto ReleaseAll;
	}

	printf("필터들을 연결했습니다.\n");

	//---------------------------------------------------
	// 필터 그래프 매니저로부터 IMediaControl 인터페이스를 구한다.
	
	hr = pFGM->QueryInterface(IID_IMediaControl, (void**)&pMC);
	if (FAILED(hr))
	{
		printf("필터 그래프 매니저로부터 IMediaControl 인터페이스를 가져올 수 없습니다.\n");
		goto ReleaseAll;
	}

	//---------------------------------------------------
	// 완성된 필터 그래프로 MP3 파일을 재생한다.
	
	hr = pMC->Run();
	if (FAILED(hr))
	{
		printf("IMediaControl 인터페이스로 MP3 파일을 재생할 수 없습니다.\n");
		goto ReleaseAll;
	}
	printf("재생 시작\n");

	//---------------------------------------------------
	// MP3 파일 재생의 종료를 기다린다.
	long evCode;
	hr = pFGM->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
	pEvent->WaitForCompletion(INFINITE, &evCode);

	//---------------------------------------------------
	// 프로그램을 종료하기 전에 각종 리소스들을 해제한다.
ReleaseAll:
	if (!pMC)
		pMC->Release();
	if (!pFGM)
		pFGM->Release();
	CoUninitialize();
}
