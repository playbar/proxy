#include "StdAfx.h"
//#include <d3dx9.h>
#include "ProxyIDirect3DSwapChain9.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
ExIDirect3DSwapChain9::ExIDirect3DSwapChain9(IDirect3DSwapChain9 *pOriginalChain,
											 Filter_IDirect3DDevice9*    pFilter,
											 IGAIDirect3DDevice9* pDevice)
{
	m_pOriginalChain = pOriginalChain;
	m_pFilter = pFilter;
	m_pDevice = pDevice;
}

HRESULT __stdcall ExIDirect3DSwapChain9::QueryInterface(REFIID riid, void **ppvObj)
{
	HRESULT res = m_pOriginalChain->QueryInterface(riid, ppvObj);
	if (res == NOERROR)
	{
		*ppvObj = this;
	}
	return	res;
}
ULONG __stdcall ExIDirect3DSwapChain9::AddRef()
{
	return	m_pOriginalChain->AddRef();
}
ULONG __stdcall ExIDirect3DSwapChain9::Release()
{
	ULONG count = m_pOriginalChain->Release();
	if(!count)
	{
		delete	this;
		return	0;
	}
	return	count;
}
HRESULT __stdcall ExIDirect3DSwapChain9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion, DWORD Flags)
{
	//OutputDebugString("ExIDirect3DSwapChain9::Present");
	if  ( NULL != m_pFilter)
        m_pFilter->OnPresent_Before(m_pDevice,pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	//m_pDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DXCOLOR(1,1,0,0),1.0f,0);

	HRESULT hr = m_pOriginalChain->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion,Flags);
	
	if  ( NULL != m_pFilter)
        m_pFilter->OnPresent_After(m_pDevice,pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	return	hr;
}
HRESULT __stdcall ExIDirect3DSwapChain9::GetFrontBufferData(IDirect3DSurface9 *pDestSurface)
{
	//TRACE_DX(L"ExIDirect3DSwapChain9::GetFrontBufferData(%p)...", pDestSurface);
	return	m_pOriginalChain->GetFrontBufferData(pDestSurface);
}
HRESULT __stdcall ExIDirect3DSwapChain9::GetRasterStatus(D3DRASTER_STATUS *pRasterStatus)
{
	//TRACE_DX(L"ExIDirect3DSwapChain9::GetRasterStatus(%p)...", pRasterStatus);
	return	m_pOriginalChain->GetRasterStatus(pRasterStatus);
}
HRESULT __stdcall ExIDirect3DSwapChain9::GetDisplayMode(D3DDISPLAYMODE* pMode)
{
	//TRACE_DX(L"ExIDirect3DSwapChain9::GetDisplayMode(%p)...", pMode);
	return	m_pOriginalChain->GetDisplayMode(pMode);
}
HRESULT __stdcall ExIDirect3DSwapChain9::GetDevice(IDirect3DDevice9 **ppDevice)
{
	//TRACE_DX(L"ExIDirect3DSwapChain9::GetDevice(%p)...", *ppDevice);
	return	m_pOriginalChain->GetDevice(ppDevice);
}
HRESULT __stdcall ExIDirect3DSwapChain9::GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	//TRACE_DX(L"ExIDirect3DSwapChain9::GetPresentParameters(%p)...", pPresentationParameters);
	return	m_pOriginalChain->GetPresentParameters(pPresentationParameters);
}

HRESULT __stdcall ExIDirect3DSwapChain9::GetBackBuffer(UINT Index, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 **ppBackSurface)
{
	//TRACE_DX(L"ExIDirect3DSwapChain9::GetBackBuffer(%u, %#x, %p)...", Index, Type, *ppBackSurface);
	return	m_pOriginalChain->GetBackBuffer(Index, Type, ppBackSurface);
}
