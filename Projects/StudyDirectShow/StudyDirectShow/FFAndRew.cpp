#include <dshow.h>
#include <stdio.h>
#include <conio.h>

#define PATH_MAXLEN 255
#define ONE_SEC 10000000

void main()
{
	char c = 0;
	WCHAR wszFileName[PATH_MAXLEN];
	HRESULT hr;
	IGraphBuilder * pFGM;
	IMediaControl * pMC;
	IMediaSeeking * pMS;
	LONGLONG curPos, stopPos;
	
	// COM ���̺귯���� �ʱ�ȭ�Ѵ�.
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		printf("COM ���̺귯���� �ʱ�ȭ�� �� �����ϴ�.\n");
		return;
	}

	// ���� �׷��� �Ŵ��� ������Ʈ�� �����ϰ�, �������̽��� ���� �´�.
	hr = CoCreateInstance(
		CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (LPVOID*)&pFGM);
	if (FAILED(hr))
	{
		printf("���� �׷��� �Ŵ����� ������ �� �����ϴ�.\n");
		return;
	}

	printf("���� �׷��� �Ŵ����� ���������� ����������ϴ�.\n");

	// ����� ������ ����ڰ� �Է��� ���� ��ٸ���.
	printf("����� ������ �̸��� �Է��Ͻʽÿ�: ");
	_getws_s(wszFileName, PATH_MAXLEN);
	

	// ���� �׷����� ����� ���� �� �������� �����Ѵ�.
	hr = pFGM->RenderFile(wszFileName, NULL);
	if (FAILED(hr))
	{
		printf("���� �׷����� ����µ� �����߽��ϴ�.\n");
		return;
	}

	printf("���� �׷����� ���������� ����������ϴ�.\n");

	// ���� �׷��� �Ŵ����κ��� �̵�� ���� �������̽��� �����´�.
	hr = pFGM->QueryInterface(IID_IMediaControl, (void**)&pMC);
	if (FAILED(hr))
	{
		printf("���� �׷��� �Ŵ����κ��� IMediaControl �������̽��� ������ �� �����ϴ�.\n");
		return;
	}

	// ���� �׷��� �Ŵ����κ��� �̵�� Ž�� �������̽��� �����´�.
	hr = pFGM->QueryInterface(IID_IMediaSeeking, (void**)&pMS);
	if (FAILED(hr))
	{
		printf("���� �׷��� �Ŵ����κ��� IMediaSeeking �������̽��� ������ �� �����ϴ�.\n");
		return;
	}

	// �Ʒ� �������� �̵�� ���� �������̽��� �̿��Ͽ� �̵�� ������ �����Ѵ�.
	while (c != 'q')
	{
		printf("Commands: r(run/resume), s(stop), p(pause), f(fast forward), w(rewind), q(quit)\n");
		printf("> ");
		c = _getche();

		switch (c)
		{
			case 'r':
				hr = pMC->Run();
				printf("\n�̵�� ������ ����մϴ�.\n");
				if (FAILED(hr))
					printf("�̵�� ������ ����� �� �����ϴ�.\n");
				break;

			case 's':
				pMC->Stop();
				printf("\n�̵�� ���� ����� �����մϴ�.\n");
				if (FAILED(hr))
					printf("�̵�� ���� ����� ������ �� �����ϴ�.\n");
				break;

			case 'p':
				pMC->Pause();
				printf("\n�̵�� ���� ����� �Ͻ������մϴ�.\n");
				if (FAILED(hr))
					printf("�̵�� ���� ����� �Ͻ������� �� �����ϴ�.\n");
				break;

			case 'f':
				pMS->GetPositions(&curPos, &stopPos);
				curPos += ONE_SEC * 10;
				pMS->SetPositions(&curPos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
				break;

			case 'w':
				pMS->GetPositions(&curPos, &stopPos);
				curPos -= ONE_SEC * 10;
				pMS->SetPositions(&curPos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
				break;

			case 'q':
				printf("\n���α׷��� �����մϴ�.\n");
				// Ư���� ��ġ�� �������� ����.
				break;

			default:
				;
		}

		printf("\n");
	}

	// ���� ���� ���ҽ� ����
	pMC->Stop();
	pMC->Release();
	pMS->Release();
	pFGM->Release();
	CoUninitialize();
}