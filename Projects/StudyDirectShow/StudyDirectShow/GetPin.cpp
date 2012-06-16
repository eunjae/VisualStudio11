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
	// MPEG-1 Splitter ���� ����

	hr =  CoCreateInstance(
			CLSID_MPEG1Splitter, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter, (LPVOID*)&pSplitter);
	if (FAILED(hr))
	{
		printf("MPEG-1 Splitter ���͸� ������ �� �����ϴ�.\n");
		return;
	}
	printf("MPEG-1 Splitter ���͸� �����߽��ϴ�.\n");

	//---------------------------------------------------
	// MPEG-1 Splitter ���ͷκ��� �Է� ���� ���Ѵ�.

	hr = GetPin(pSplitter, PINDIR_INPUT, &pPin);
	if (FAILED(hr))
	{
		printf("MPEG-1 Splitter ���ͷκ��� �Է� ���� ���� �� �����ϴ�.\n");
		pSplitter->Release();
		return;
	}
	printf("MPEG-1 Splitter ���ͷκ��� �Է� ���� ���߽��ϴ�.\n");

	//---------------------------------------------------
	// ���α׷��� �����ϱ� ���� ���� ���ҽ����� �����Ѵ�.

	pSplitter->Release();
	pPin->Release();
	CoUninitialize();
}