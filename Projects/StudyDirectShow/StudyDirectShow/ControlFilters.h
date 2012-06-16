#ifndef _CONTROLFILTERS_H_
#define _CONTROLFILTERS_H_

#include <dshow.h>

HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);
HRESULT GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);
HRESULT ConnectFilters(IGraphBuilder *pGraph, IPin *pOut, IBaseFilter *pDest);
HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest);

#endif