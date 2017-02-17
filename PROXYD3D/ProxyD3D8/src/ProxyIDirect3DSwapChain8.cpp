#include "StdAfx.h"
#include "ProxyIDirect3DSwapChain8.h"


IGAIDirect3DSwapChain8::IGAIDirect3DSwapChain8(IDirect3DSwapChain8 *pkSwapChain)
{
	m_pkD3DSwapChain = pkSwapChain;
}

IGAIDirect3DSwapChain8::~IGAIDirect3DSwapChain8()
{
	m_pkD3DSwapChain = NULL;
}

HRESULT IGAIDirect3DSwapChain8::QueryInterface (REFIID riid, void** ppvObj)
{
	*ppvObj = NULL;

	HRESULT hRes = m_pkD3DSwapChain->QueryInterface(riid, ppvObj); 

	if(hRes == NOERROR)
		*ppvObj = this;

	return hRes;
}

ULONG IGAIDirect3DSwapChain8::AddRef()
{
	return m_pkD3DSwapChain->AddRef();
}

ULONG IGAIDirect3DSwapChain8::Release()
{
	ULONG ulCount = m_pkD3DSwapChain->Release();

	if(ulCount == 0)
		delete this;

	return ulCount;
}

HRESULT IGAIDirect3DSwapChain8::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	return m_pkD3DSwapChain->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT IGAIDirect3DSwapChain8::GetBackBuffer(UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface8** ppBackBuffer)
{
	return m_pkD3DSwapChain->GetBackBuffer(BackBuffer, Type, ppBackBuffer);
}
