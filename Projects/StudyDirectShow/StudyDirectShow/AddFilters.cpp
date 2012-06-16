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
	// COM ���̺귯���� �ʱ�ȭ�Ѵ�.
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		printf("COM ���̺귯���� �ʱ�ȭ�� �� �����ϴ�.\n");
		return;
	}

	//--------------------------------------------------------------------------------
	// ����� ������ ����ڰ� �Է��� ���� ��ٸ���.
	printf("����� MP3 ������ �̸��� �Է��Ͻʽÿ�: ");
	_getws_s(wszFileName, PATH_MAXLEN);

	//--------------------------------------------------------------------------------
	// ���� �׷��� �Ŵ��� ������Ʈ�� �����ϰ�, �������̽��� ���� �´�.
	hr = CoCreateInstance(
		CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (LPVOID*)&pFGM);
	if (FAILED(hr))
	{
		printf("���� �׷��� �Ŵ����� ������ �� �����ϴ�.\n");
		return;
	}

	printf("���� �׷��� �Ŵ����� ���������� ���������, ���� �׷����� �߰��Ǿ����ϴ�.\n");

	printf("MP3�� ����ϱ� ���� ���͵��� ���� �׷����� �߰��մϴ�.\n");

	//--------------------------------------------------------------------------------
	// �ҽ� ���͸� �����ϰ� �̸� ���� �׷����� �߰��Ѵ�.
	hr = pFGM->AddSourceFilter(wszFileName, wszFileName, &pSrcFilter);
	if (FAILED(hr))
	{
		printf("�ҽ� ���͸� ������ �� �����ϴ�.\n");
		goto Terminate;
	}
	printf("�ҽ� ���Ͱ� ���������� ����������ϴ�.\n");

	//--------------------------------------------------------------------------------
	// MPEG-1 ���ø��� ���͸� �����ϰ� �̸� ���� �׷����� �߰��Ѵ�.
	hr = CoCreateInstance(
		CLSID_MPEG1Splitter, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (LPVOID*)&pSplitter);
	if (FAILED(hr))
	{
		printf("MPEG-1 Splitter ���͸� ������ �� �����ϴ�.\n");
		goto Terminate;
	}
	printf("MPEG-1 Splitter ���Ͱ� ���������� ����������ϴ�.\n");

	hr = pFGM->AddFilter(pSplitter, NULL);
	if (FAILED(hr))
	{
		printf("MPEG-1 Splitter ���͸� ���� �׷����� �߰��� �� �����ϴ�.\n");
		goto Terminate;
	}
	printf("MPEG-1 Splitter ���Ͱ� ���� �׷����� �߰��Ǿ����ϴ�.\n");

	//--------------------------------------------------------------------------------
	// MPEG Layer-3 ���ڴ� ���͸� �����ϰ� �̸� ���� �׷����� �߰��Ѵ�.
	hr = CoCreateInstance(
		CLSID_MPEGLayer3Decoder, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (LPVOID*)&pDecoder);
	if (FAILED(hr))
	{
		printf("MPEG Layer-3 Decoder ���͸� ������ �� �����ϴ�.\n");
		goto Terminate;
	}
	printf("MPEG Layer-3 Decoder ���Ͱ� ���������� ����������ϴ�.\n");

	hr = pFGM->AddFilter(pDecoder, NULL);
	if (FAILED(hr))
	{
		printf("MPEG Layer-3 Decoder ���͸� ���� �׷����� �߰��� �� �����ϴ�.\n");
		goto Terminate;
	}
	printf("MPEG Layer-3 Decoder ���Ͱ� ���� �׷����� �߰��Ǿ����ϴ�.\n");

	//--------------------------------------------------------------------------------
	// ������ ���͸� �����ϰ� �̸� ���� �׷����� �߰��Ѵ�.
	hr = CoCreateInstance(
		CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (LPVOID*)&pRenderer);
	if (FAILED(hr))
	{
		printf("DirectSound Renderer ���͸� ������ �� �����ϴ�.\n");
		goto Terminate;
	}
	printf("DirectSound Renderer ���Ͱ� ���������� ����������ϴ�.\n");

	hr = pFGM->AddFilter(pRenderer, NULL);
	if (FAILED(hr))
	{
		printf("DirectSound Renderer ���͸� ���� �׷����� �߰��� �� �����ϴ�.\n");
		goto Terminate;
	}
	printf("DirectSound Renderer ���Ͱ� ���� �׷����� �߰��Ǿ����ϴ�.\n");

Terminate:
	//--------------------------------------------------------------------------------
	// ������ ���͵��� ���� �׷����� �߰��� �Ŀ��� ���� ī��Ʈ�� 1�� �����ϹǷ�
	// �̸� �����Ϸ��� �� ������ Release ����Լ��� ȣ���ؾ� �Ѵ�.
	// �׷��� ���� �׷����� ���ŵ� �� �׿� ���ϴ� ���͵鵵 ���� ���ŵȴ�.

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