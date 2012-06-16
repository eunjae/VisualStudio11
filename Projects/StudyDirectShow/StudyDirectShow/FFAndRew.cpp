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
	
	// COM 라이브러리를 초기화한다.
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		printf("COM 라이브러리를 초기화할 수 없습니다.\n");
		return;
	}

	// 필터 그래프 매니저 컴포넌트를 생성하고, 인터페이스를 가져 온다.
	hr = CoCreateInstance(
		CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (LPVOID*)&pFGM);
	if (FAILED(hr))
	{
		printf("필터 그래프 매니저를 생성할 수 없습니다.\n");
		return;
	}

	printf("필터 그래프 매니저가 성공적으로 만들어졌습니다.\n");

	// 재생할 파일을 사용자가 입력할 것을 기다린다.
	printf("재생할 파일의 이름을 입력하십시오: ");
	_getws_s(wszFileName, PATH_MAXLEN);
	

	// 필터 그래프를 만들기 위해 널 렌더링을 실행한다.
	hr = pFGM->RenderFile(wszFileName, NULL);
	if (FAILED(hr))
	{
		printf("필터 그래프를 만드는데 실패했습니다.\n");
		return;
	}

	printf("필터 그래프가 성공적으로 만들어졌습니다.\n");

	// 필터 그래프 매니저로부터 미디어 제어 인터페이스를 가져온다.
	hr = pFGM->QueryInterface(IID_IMediaControl, (void**)&pMC);
	if (FAILED(hr))
	{
		printf("필터 그래프 매니저로부터 IMediaControl 인터페이스를 가져올 수 없습니다.\n");
		return;
	}

	// 필터 그래프 매니저로부터 미디어 탐색 인터페이스를 가져온다.
	hr = pFGM->QueryInterface(IID_IMediaSeeking, (void**)&pMS);
	if (FAILED(hr))
	{
		printf("필터 그래프 매니저로부터 IMediaSeeking 인터페이스를 가져올 수 없습니다.\n");
		return;
	}

	// 아래 루프에서 미디어 제어 인터페이스를 이용하여 미디어 파일을 제어한다.
	while (c != 'q')
	{
		printf("Commands: r(run/resume), s(stop), p(pause), f(fast forward), w(rewind), q(quit)\n");
		printf("> ");
		c = _getche();

		switch (c)
		{
			case 'r':
				hr = pMC->Run();
				printf("\n미디어 파일을 재생합니다.\n");
				if (FAILED(hr))
					printf("미디어 파일을 재생할 수 없습니다.\n");
				break;

			case 's':
				pMC->Stop();
				printf("\n미디어 파일 재생을 중지합니다.\n");
				if (FAILED(hr))
					printf("미디어 파일 재생을 중지할 수 없습니다.\n");
				break;

			case 'p':
				pMC->Pause();
				printf("\n미디어 파일 재생을 일시정지합니다.\n");
				if (FAILED(hr))
					printf("미디어 파일 재생을 일시정지할 수 없습니다.\n");
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
				printf("\n프로그램을 종료합니다.\n");
				// 특별한 조치를 취하지는 않음.
				break;

			default:
				;
		}

		printf("\n");
	}

	// 종료 전에 리소스 해제
	pMC->Stop();
	pMC->Release();
	pMS->Release();
	pFGM->Release();
	CoUninitialize();
}