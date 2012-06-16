#include <dshow.h>
#include <stdio.h>
#include "ControlFilters.h"

void main()
{
	HRESULT hr;
	IBaseFilter * pSplitter = NULL;
	IPin * pPin = NULL;

	//---------------------------------------------------

	CoInitialize(NULL);

	//---------------------------------------------------
	// MPEG-1 Splitter 필터 생성

	hr =  CoCreateInstance(
			CLSID_MPEG1Splitter, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter, (LPVOID*)&pSplitter);
	if (FAILED(hr))
	{
		printf("MPEG-1 Splitter 필터를 생성할 수 없습니다.\n");
		return;
	}
	printf("MPEG-1 Splitter 필터를 생성했습니다.\n");

	//---------------------------------------------------
	// MPEG-1 Splitter 필터로부터 입력 핀을 구한다.

	hr = GetPin(pSplitter, PINDIR_INPUT, &pPin);
	if (FAILED(hr))
	{
		printf("MPEG-1 Splitter 필터로부터 입력 핀을 구할 수 없습니다.\n");
		pSplitter->Release();
		return;
	}
	printf("MPEG-1 Splitter 필터로부터 입력 핀을 구했습니다.\n");

	//---------------------------------------------------
	// 프로그램을 종료하기 전에 각종 리소스들을 해제한다.

	pSplitter->Release();
	pPin->Release();
	CoUninitialize();
}