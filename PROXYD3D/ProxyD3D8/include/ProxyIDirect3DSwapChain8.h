#ifndef IGAIDirect3DSwapChain8_H
#define IGAIDirect3DSwapChain8_H

#include <d3d8.h>


class IGAIDirect3DSwapChain8 : public IDirect3DSwapChain8
{
public:
    IGAIDirect3DSwapChain8(IDirect3DSwapChain8 *pkSwapChain);
    virtual ~IGAIDirect3DSwapChain8();

	/*** IUnknown methods ***/
	HRESULT STDMETHODCALLTYPE QueryInterface (REFIID riid, void** ppvObj);
	ULONG   STDMETHODCALLTYPE AddRef();
	ULONG   STDMETHODCALLTYPE Release();

	/*** IDirect3DSwapChain8 methods ***/
    HRESULT STDMETHODCALLTYPE Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
    HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface8** ppBackBuffer);

private:
    IDirect3DSwapChain8 *m_pkD3DSwapChain;
};

#endif

