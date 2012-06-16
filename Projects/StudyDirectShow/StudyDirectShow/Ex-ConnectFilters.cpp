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
	// COM ���̺귯���� �ʱ�ȭ�Ѵ�.

	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		printf("COM ���̺귯���� �ʱ�ȭ�� �� �����ϴ�.\n");
		return;
	}

	//---------------------------------------------------
	// ����� MP3 ���� �̸��� ����ڷκ��� �޴´�.
	
	printf("����� ������ �̸��� �Է��Ͻʽÿ�: ");
	_getws_s(wszFileName, nMaxLen);

	//---------------------------------------------------
	// ���� �׷��� �Ŵ��� ����

	hr = CoCreateInstance(
		CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (LPVOID*)&pFGM);
	if (FAILED(hr))
	{
		printf("���� �׷��� �Ŵ����� ������ �� �����ϴ�.\n");
		CoUninitialize();
		return;
	}

	printf("���� �׷��� �Ŵ����� �����Ǿ����ϴ�.\n");

	//---------------------------------------------------
	// MP3 ������ ����ϱ� ���� ���� ����

	hr = pFGM->AddSourceFilter(wszFileName, wszFileName, &pSrcFilter);
	if (FAILED(hr))
	{
		printf("�ҽ� ���͸� ������ �� �����ϴ�.\n");
		goto ReleaseAll;
	}

	hr = CoCreateInstance(
			CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter, (LPVOID*)&pDSoundRenderer);
	if (FAILED(hr))
	{
		printf("DirectSound Renderer ���͸� ������ �� �����ϴ�.\n");
		goto ReleaseAll;
	}

	printf("���͵��� �����Ǿ����ϴ�.\n");

	//---------------------------------------------------
	// ������ ���͵��� ���� �׷����� �߰��Ѵ�.

	hr = pFGM->AddFilter(pDSoundRenderer, NULL);

	printf("���͵��� ���� �׷����� �߰��Ǿ����ϴ�.\n");

	//---------------------------------------------------
	// ������ ���͵��� �����Ѵ�. (�� ���� ����� �߰� ���� ����)

	hr = ConnectFilters(pFGM, pSrcFilter, pDSoundRenderer);
	if (FAILED(hr))
	{
		printf("�ҽ� ���Ϳ� WMAudio Decoder DMO ���͸� ������ �� �����ϴ�.\n");
		goto ReleaseAll;
	}

	printf("���͵��� �����߽��ϴ�.\n");

	//---------------------------------------------------
	// ���� �׷��� �Ŵ����κ��� IMediaControl �������̽��� ���Ѵ�.
	
	hr = pFGM->QueryInterface(IID_IMediaControl, (void**)&pMC);
	if (FAILED(hr))
	{
		printf("���� �׷��� �Ŵ����κ��� IMediaControl �������̽��� ������ �� �����ϴ�.\n");
		goto ReleaseAll;
	}

	//---------------------------------------------------
	// �ϼ��� ���� �׷����� MP3 ������ ����Ѵ�.
	
	hr = pMC->Run();
	if (FAILED(hr))
	{
		printf("IMediaControl �������̽��� MP3 ������ ����� �� �����ϴ�.\n");
		goto ReleaseAll;
	}
	printf("��� ����\n");

	//---------------------------------------------------
	// MP3 ���� ����� ���Ḧ ��ٸ���.
	long evCode;
	hr = pFGM->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
	pEvent->WaitForCompletion(INFINITE, &evCode);

	//---------------------------------------------------
	// ���α׷��� �����ϱ� ���� ���� ���ҽ����� �����Ѵ�.
ReleaseAll:
	if (!pMC)
		pMC->Release();
	if (!pFGM)
		pFGM->Release();
	CoUninitialize();
}
