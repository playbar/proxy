#ifndef IGAIDirect3DSwapChain9_H
#define IGAIDirect3DSwapChain9_H

#include <d3d9.h>
#include "..\D3D9Filter.h"
#include "ProxyIDirect3DDevice9.h"
/************************************************************************/
/*                                                                      */
/************************************************************************/
class ExIDirect3DSwapChain9 : public IDirect3DSwapChain9
{
public:
	ExIDirect3DSwapChain9(IDirect3DSwapChain9 *pOriginalChain,Filter_IDirect3DDevice9*    pFilter,IGAIDirect3DDevice9* pDevice);
	HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObj);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion,DWORD Flags);
	HRESULT __stdcall GetFrontBufferData(IDirect3DSurface9 *pDestSurface);
	HRESULT __stdcall GetBackBuffer(UINT Index, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 **ppBackSurface);
	HRESULT __stdcall GetRasterStatus(D3DRASTER_STATUS *pRasterStatus);
	HRESULT __stdcall GetDisplayMode(D3DDISPLAYMODE* pMode);
	HRESULT __stdcall GetDevice(IDirect3DDevice9 **ppDevice);
	HRESULT __stdcall GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters);
private:
	IDirect3DSwapChain9	*m_pOriginalChain;
	Filter_IDirect3DDevice9*   m_pFilter;
	IGAIDirect3DDevice9* m_pDevice;
};
#endif

