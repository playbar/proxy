#include "StdAfx.h"
#include "DrawTriang.h"


IGA_IDirect3DSwapChain9::IGA_IDirect3DSwapChain9(IDirect3DSwapChain9* pOriginal, IDirect3DDevice9* pDevice)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::IGA_IDirect3DSwapChain9 ); 
	m_pIDirect3DSwapChain9 = pOriginal; // store the pointer to original object
	m_pIDirect3DDevice9 = pDevice; 
}

IGA_IDirect3DSwapChain9::~IGA_IDirect3DSwapChain9(void)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::~IGA_IDirect3DSwapChain9 );
	m_pIDirect3DSwapChain9 = NULL;
}

HRESULT IGA_IDirect3DSwapChain9::QueryInterface (REFIID riid, void** ppvObj)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::QueryInterface );
	// check if original dll can provide interface. then send *our* address
	*ppvObj = NULL;
	HRESULT hRes = m_pIDirect3DSwapChain9->QueryInterface(riid, ppvObj); 

	//if (hRes == NOERROR)
	//{
	//	*ppvObj = this;
	//}
	
	return hRes;
}

ULONG IGA_IDirect3DSwapChain9::AddRef(void)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::AddRef );
	return(m_pIDirect3DSwapChain9->AddRef());
}

ULONG IGA_IDirect3DSwapChain9::Release(void)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::Release );
	// Calling original function now
	ULONG count = m_pIDirect3DSwapChain9->Release();
		
    if (count==0)
	{
		// now, the Original Object has deleted itself, so do we here
		//gl_pmyIDirect3DSwapChain9 = NULL;
		delete(this);  // destructor will be called automatically
	}

	return (count);
}

HRESULT IGA_IDirect3DSwapChain9::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::Present );
	// Some games use this one for presenting. They ignore the Device's present routine 
    // we may want to draw own things here before flipping surfaces
    // ... draw own stuff ...
	//this->ShowWeAreHere();

	HRESULT hres =S_OK;

	DrawRectMy( m_pIDirect3DDevice9, NULL );

	hres = m_pIDirect3DSwapChain9->Present(pSourceRect,pSourceRect,hDestWindowOverride,pDirtyRegion,dwFlags);

	//hres = m_pIDirect3DDevice9->Present(pSourceRect, pSourceRect, hDestWindowOverride, pDirtyRegion );
	return (hres);
}

HRESULT IGA_IDirect3DSwapChain9::GetFrontBufferData(IDirect3DSurface9* pDestSurface)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::GetFrontBufferData );
	return(m_pIDirect3DSwapChain9->GetFrontBufferData(pDestSurface));
}

HRESULT IGA_IDirect3DSwapChain9::GetBackBuffer(UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::GetBackBuffer );
	HRESULT re;
	re = m_pIDirect3DSwapChain9->GetBackBuffer(iBackBuffer,Type,ppBackBuffer);
	/*D3DLOCKED_RECT lockRect;
	if ( SUCCEEDED( (*ppBackBuffer)->LockRect( &lockRect, NULL, 0 ) ))
	{
		CopyMemory(lockRect.pBits, 0, 2000);
		(*ppBackBuffer)->UnlockRect();
	}*/
	return re; 
}

HRESULT IGA_IDirect3DSwapChain9::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::GetRasterStatus );
	return(m_pIDirect3DSwapChain9->GetRasterStatus(pRasterStatus));
}

HRESULT IGA_IDirect3DSwapChain9::GetDisplayMode(D3DDISPLAYMODE* pMode)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::GetDisplayMode );
	return(m_pIDirect3DSwapChain9->GetDisplayMode(pMode));
}

HRESULT IGA_IDirect3DSwapChain9::GetDevice(IDirect3DDevice9** ppDevice)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::GetDevice );
	return(m_pIDirect3DSwapChain9->GetDevice(ppDevice));
}

HRESULT IGA_IDirect3DSwapChain9::GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::GetPresentParameters );
	return(m_pIDirect3DSwapChain9->GetPresentParameters(pPresentationParameters));
}

// This is our test function
void IGA_IDirect3DSwapChain9::ShowWeAreHere(void)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DSwapChain9::ShowWeAreHere );
	D3DRECT rec = {100,1,150,50};
	m_pIDirect3DDevice9->Clear(1, &rec, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,0,0),0 ,0);
}

