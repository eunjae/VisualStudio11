#include <dshow.h>
#include <stdio.h>
#include <conio.h>

#define PATH_MAXLEN 256

const GUID CLSID_MPEGLayer3Decoder = {
    0x4A2286E0, 0x7BEF, 0x11CE,
    {0x9B, 0xD9, 0x00, 0x00, 0xE2, 0x02, 0x59, 0x9C}
};

void main()
{
	char c = 0;
	WCHAR wszFileName[PATH_MAXLEN];
	HRESULT hr;
	IGraphBuilder * pFGM = NULL;
	IBaseFilter * pSrcFilter	= NULL;
	IBaseFilter * pSplitter		= NULL;
	IBaseFilter * pDecoder		= NULL;
	IBaseFilter * pRenderer		= NULL;;
	
	//--------------------------------------------------------------------------------
	// COM 라이브러리를 초기화한다.
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		printf("COM 라이브러리를 초기화할 수 없습니다.\n");
		return;
	}

	//--------------------------------------------------------------------------------
	// 재생할 파일을 사용자가 입력할 것을 기다린다.
	printf("재생할 MP3 파일의 이름을 입력하십시오: ");
	_getws_s(wszFileName, PATH_MAXLEN);

	//--------------------------------------------------------------------------------
	// 필터 그래프 매니저 컴포넌트를 생성하고, 인터페이스를 가져 온다.
	hr = CoCreateInstance(
		CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (LPVOID*)&pFGM);
	if (FAILED(hr))
	{
		printf("필터 그래프 매니저를 생성할 수 없습니다.\n");
		return;
	}

	printf("필터 그래프 매니저가 성공적으로 만들어지고, 필터 그래프에 추가되었습니다.\n");

	printf("MP3를 재생하기 위한 필터들을 필터 그래프에 추가합니다.\n");

	//--------------------------------------------------------------------------------
	// 소스 필터를 생성하고 이를 필터 그래프에 추가한다.
	hr = pFGM->AddSourceFilter(wszFileName, wszFileName, &pSrcFilter);
	if (FAILED(hr))
	{
		printf("소스 필터를 생성할 수 없습니다.\n");
		goto Terminate;
	}
	printf("소스 필터가 성공적으로 만들어졌습니다.\n");

	//--------------------------------------------------------------------------------
	// MPEG-1 스플리터 필터를 생성하고 이를 필터 그래프에 추가한다.
	hr = CoCreateInstance(
		CLSID_MPEG1Splitter, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (LPVOID*)&pSplitter);
	if (FAILED(hr))
	{
		printf("MPEG-1 Splitter 필터를 생성할 수 없습니다.\n");
		goto Terminate;
	}
	printf("MPEG-1 Splitter 필터가 성공적으로 만들어졌습니다.\n");

	hr = pFGM->AddFilter(pSplitter, NULL);
	if (FAILED(hr))
	{
		printf("MPEG-1 Splitter 필터를 필터 그래프에 추가할 수 없습니다.\n");
		goto Terminate;
	}
	printf("MPEG-1 Splitter 필터가 필터 그래프에 추가되었습니다.\n");

	//--------------------------------------------------------------------------------
	// MPEG Layer-3 디코더 필터를 생성하고 이를 필터 그래프에 추가한다.
	hr = CoCreateInstance(
		CLSID_MPEGLayer3Decoder, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (LPVOID*)&pDecoder);
	if (FAILED(hr))
	{
		printf("MPEG Layer-3 Decoder 필터를 생성할 수 없습니다.\n");
		goto Terminate;
	}
	printf("MPEG Layer-3 Decoder 필터가 성공적으로 만들어졌습니다.\n");

	hr = pFGM->AddFilter(pDecoder, NULL);
	if (FAILED(hr))
	{
		printf("MPEG Layer-3 Decoder 필터를 필터 그래프에 추가할 수 없습니다.\n");
		goto Terminate;
	}
	printf("MPEG Layer-3 Decoder 필터가 필터 그래프에 추가되었습니다.\n");

	//--------------------------------------------------------------------------------
	// 렌더러 필터를 생성하고 이를 필터 그래프에 추가한다.
	hr = CoCreateInstance(
		CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (LPVOID*)&pRenderer);
	if (FAILED(hr))
	{
		printf("DirectSound Renderer 필터를 생성할 수 없습니다.\n");
		goto Terminate;
	}
	printf("DirectSound Renderer 필터가 성공적으로 만들어졌습니다.\n");

	hr = pFGM->AddFilter(pRenderer, NULL);
	if (FAILED(hr))
	{
		printf("DirectSound Renderer 필터를 필터 그래프에 추가할 수 없습니다.\n");
		goto Terminate;
	}
	printf("DirectSound Renderer 필터가 필터 그래프에 추가되었습니다.\n");

Terminate:
	//--------------------------------------------------------------------------------
	// 생성된 필터들을 필터 그래프에 추가한 후에는 참조 카운트가 1씩 증가하므로
	// 이를 원복하려면 각 필터의 Release 멤버함수를 호출해야 한다.
	// 그래야 필터 그래프가 제거될 때 그에 속하는 필터들도 같이 제거된다.

	if (pSrcFilter)
		pSrcFilter->Release();
	if (pSplitter)
		pSplitter->Release();
	if (pDecoder)
		pDecoder->Release();
	if (pRenderer)
		pRenderer->Release();

	if (pFGM)
		pFGM->Release();
	CoUninitialize();
}