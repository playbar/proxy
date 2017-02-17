#include "StdAfx.h"

#include <d3dx9tex.h>
#include "IGA_IDirect3DSurface9.h"
#include "DrawTriang.h"

extern LPDIRECT3DVERTEXBUFFER9 g_pVB ;    //顶点缓冲区对象

IGA_IDirect3DDevice9::IGA_IDirect3DDevice9(IDirect3DDevice9* pOriginal)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::IGA_IDirect3DDevice9);
    m_pIDirect3DDevice9 = pOriginal; // store the pointer to original object

	if( FAILED( CreateVertexBuffer( 3*sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
	{
		return;
	}

	CUSTOMVERTEX vertices[] =
	{
		{ 100.0f, 200.0f, 0.5f, 1.0f, 0xffff0000, },
		{ 200.0f,  50.0f, 0.5f, 1.0f, 0xff00ff00, }, 
		{ 300.0f, 200.0f, 0.5f, 1.0f, 0xff0000ff, },
	};

	VOID* pVertices;
	if( FAILED( g_pVB->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 ) ) )
		return ;
	memcpy( pVertices, vertices, sizeof(vertices) );
	g_pVB->Unlock();

}

IGA_IDirect3DDevice9::~IGA_IDirect3DDevice9(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::~IGA_IDirect3DDevice9);

	if ( g_pVB != NULL )
	{
		g_pVB->Release();
		g_pVB = NULL;
	}
}


HRESULT IGA_IDirect3DDevice9::QueryInterface (REFIID riid, void** ppvObj)
{
    // check if original dll can provide interface. then send *our* address
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::QueryInterface);
	*ppvObj = NULL;

	HRESULT hRes = m_pIDirect3DDevice9->QueryInterface(riid, ppvObj); 

	//if (hRes == NOERROR)
	//{
	//	*ppvObj = this;
	//}
	
	return hRes;
}

ULONG IGA_IDirect3DDevice9::AddRef(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::AddRef);
    return(m_pIDirect3DDevice9->AddRef());
}

ULONG IGA_IDirect3DDevice9::Release(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::Release);

	if ( g_pVB != NULL )
	{
		g_pVB->Release();
		g_pVB = NULL;
	}

	ULONG lcount = 0;
	//IGA_IDirect3DDevice9 * pTemp = this;
	//if ( pTemp != NULL )
	//{
	//	m_pIDirect3DDevice9 = NULL;
	//	lcount = pTemp->Release();
	//}
	lcount = m_pIDirect3DDevice9->Release();	
    if (lcount==0)
	{
		delete(this);  // destructor will be called automatically
	}
	return lcount;
}

HRESULT IGA_IDirect3DDevice9::TestCooperativeLevel(void)
{
	//SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::TestCooperativeLevel);
    return(m_pIDirect3DDevice9->TestCooperativeLevel());
}

UINT IGA_IDirect3DDevice9::GetAvailableTextureMem(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetAvailableTextureMem);
    return(m_pIDirect3DDevice9->GetAvailableTextureMem());
}

HRESULT IGA_IDirect3DDevice9::EvictManagedResources(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::EvictManagedResources);
    return(m_pIDirect3DDevice9->EvictManagedResources());
}

HRESULT IGA_IDirect3DDevice9::GetDirect3D(IDirect3D9** ppD3D9)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetDirect3D);
    return(m_pIDirect3DDevice9->GetDirect3D(ppD3D9));
}

HRESULT IGA_IDirect3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetDeviceCaps);
    return(m_pIDirect3DDevice9->GetDeviceCaps(pCaps));
}

HRESULT IGA_IDirect3DDevice9::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetDisplayMode);
    return(m_pIDirect3DDevice9->GetDisplayMode(iSwapChain, pMode));
}

HRESULT IGA_IDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetCreationParameters);
    return(m_pIDirect3DDevice9->GetCreationParameters(pParameters));
}

HRESULT IGA_IDirect3DDevice9::SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::SetCursorProperties);
    return(m_pIDirect3DDevice9->SetCursorProperties(XHotSpot,YHotSpot,pCursorBitmap));
}

void    IGA_IDirect3DDevice9::SetCursorPosition(int X,int Y,DWORD Flags)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::SetCursorPosition);
    return(m_pIDirect3DDevice9->SetCursorPosition(X,Y,Flags));
}

BOOL    IGA_IDirect3DDevice9::ShowCursor(BOOL bShow)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::ShowCursor);
    return(m_pIDirect3DDevice9->ShowCursor(bShow));
}

HRESULT IGA_IDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)  
{

	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::CreateAdditionalSwapChain);
	HRESULT hr; 
	extern IGA_IDirect3DSwapChain9*  gl_pmyIDirect3DSwapChain9;
    hr = (m_pIDirect3DDevice9->CreateAdditionalSwapChain(pPresentationParameters,pSwapChain));
	if ( SUCCEEDED(hr) )
	{
		IGA_IDirect3DSwapChain9* pIGASwapChain = new IGA_IDirect3DSwapChain9( *pSwapChain, this);
		*pSwapChain = pIGASwapChain; 
	}
	//gl_pmyIDirect3DSwapChain9 = (IGA_IDirect3DSwapChain9* )*pSwapChain;
	return hr;
}

HRESULT IGA_IDirect3DDevice9::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetSwapChain);

	if (iSwapChain!=0) 
		return(m_pIDirect3DDevice9->GetSwapChain(iSwapChain,pSwapChain));
	
	// we intercept this call and provide our own "fake" SwapChain Object
	IDirect3DSwapChain9* pOriginal = NULL;
	HRESULT hres = m_pIDirect3DDevice9->GetSwapChain(iSwapChain,&pOriginal);

	if ( SUCCEEDED (hres))
	{
		IGA_IDirect3DSwapChain9 *pmyIDirect3DSwapChain9 = new IGA_IDirect3DSwapChain9(pOriginal, m_pIDirect3DDevice9);
		*pSwapChain = pmyIDirect3DSwapChain9;
	}
	
	return(hres); 
}

UINT    IGA_IDirect3DDevice9::GetNumberOfSwapChains(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetNumberOfSwapChains);
    return(m_pIDirect3DDevice9->GetNumberOfSwapChains());
}

HRESULT IGA_IDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::Reset);
    return(m_pIDirect3DDevice9->Reset(pPresentationParameters));
}

HRESULT IGA_IDirect3DDevice9::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::Present);
    ShowWeAreHere();

	HRESULT hres = m_pIDirect3DDevice9->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	return (hres);
}

HRESULT IGA_IDirect3DDevice9::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetBackBuffer);
	HRESULT hr = E_FAIL;
    hr = m_pIDirect3DDevice9->GetBackBuffer(iSwapChain,iBackBuffer,Type,ppBackBuffer);
	//if ( SUCCEEDED( hr ))
	//{
	//	IDirect3DSurface9 * pD3D9Surface = new IGA_IDirect3DSurface9(*ppBackBuffer, this );
	//	*ppBackBuffer = pD3D9Surface;
	//}
	return hr;
}

HRESULT IGA_IDirect3DDevice9::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetRasterStatus);
    return(m_pIDirect3DDevice9->GetRasterStatus(iSwapChain,pRasterStatus));
}

HRESULT IGA_IDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::SetDialogBoxMode);
    return(m_pIDirect3DDevice9->SetDialogBoxMode(bEnableDialogs));
}

void    IGA_IDirect3DDevice9::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::SetGammaRamp);
    return(m_pIDirect3DDevice9->SetGammaRamp(iSwapChain,Flags,pRamp));
}

void    IGA_IDirect3DDevice9::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetGammaRamp);
    return(m_pIDirect3DDevice9->GetGammaRamp(iSwapChain,pRamp));
}

HRESULT IGA_IDirect3DDevice9::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::CreateTexture);
	HRESULT hr = m_pIDirect3DDevice9->CreateTexture(Width,Height,Levels,Usage,Format,Pool,ppTexture,pSharedHandle);

	//if ( SUCCEEDED(hr) )
	//{
	//	IGA_IDirect3DTexture9 * pIDirect3DTexture9 = new IGA_IDirect3DTexture9( *ppTexture, this );
	//	*ppTexture = pIDirect3DTexture9;
	//}
    return hr;
}

HRESULT IGA_IDirect3DDevice9::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::CreateVolumeTexture);
    return(m_pIDirect3DDevice9->CreateVolumeTexture(Width,Height,Depth,Levels,Usage,Format,Pool,ppVolumeTexture,pSharedHandle));
}

HRESULT IGA_IDirect3DDevice9::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::CreateCubeTexture);
    return(m_pIDirect3DDevice9->CreateCubeTexture(EdgeLength,Levels,Usage,Format,Pool,ppCubeTexture,pSharedHandle));
}

HRESULT IGA_IDirect3DDevice9::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::CreateVertexBuffer);
    return(m_pIDirect3DDevice9->CreateVertexBuffer(Length,Usage,FVF,Pool,ppVertexBuffer,pSharedHandle));
}

HRESULT IGA_IDirect3DDevice9::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::CreateIndexBuffer);
    return(m_pIDirect3DDevice9->CreateIndexBuffer(Length,Usage,Format,Pool,ppIndexBuffer,pSharedHandle));
}

HRESULT IGA_IDirect3DDevice9::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::CreateRenderTarget);
    return(m_pIDirect3DDevice9->CreateRenderTarget(Width,Height,Format,MultiSample,MultisampleQuality,Lockable,ppSurface,pSharedHandle));
}

HRESULT IGA_IDirect3DDevice9::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::CreateDepthStencilSurface);
    return(m_pIDirect3DDevice9->CreateDepthStencilSurface(Width,Height,Format,MultiSample,MultisampleQuality,Discard,ppSurface,pSharedHandle));
}

HRESULT IGA_IDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::UpdateSurface);
    return(m_pIDirect3DDevice9->UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint));
}

HRESULT IGA_IDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::UpdateTexture);
    return(m_pIDirect3DDevice9->UpdateTexture(pSourceTexture,pDestinationTexture));
}

HRESULT IGA_IDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetRenderTargetData);
    return(m_pIDirect3DDevice9->GetRenderTargetData(pRenderTarget,pDestSurface));
}

HRESULT IGA_IDirect3DDevice9::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetFrontBufferData);
    return(m_pIDirect3DDevice9->GetFrontBufferData(iSwapChain,pDestSurface));
}

HRESULT IGA_IDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::StretchRect);
    return(m_pIDirect3DDevice9->StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter));
}

HRESULT IGA_IDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::ColorFill);
    return(m_pIDirect3DDevice9->ColorFill(pSurface,pRect,color));
}

HRESULT IGA_IDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::CreateOffscreenPlainSurface);
    return(m_pIDirect3DDevice9->CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle));
}

HRESULT IGA_IDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::SetRenderTarget);
    return(m_pIDirect3DDevice9->SetRenderTarget(RenderTargetIndex,pRenderTarget));
}

HRESULT IGA_IDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetRenderTarget);
	HRESULT hr = E_FAIL;
	hr = m_pIDirect3DDevice9->GetRenderTarget(RenderTargetIndex,ppRenderTarget);
	/*if ( SUCCEEDED(hr) )
	{	
		IGA_IDirect3DSurface9 * pD3D9Surface = new IGA_IDirect3DSurface9(*ppRenderTarget, this );
		*ppRenderTarget = pD3D9Surface;
	}*/
	return hr;
}

HRESULT IGA_IDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::SetDepthStencilSurface);
    return(m_pIDirect3DDevice9->SetDepthStencilSurface(pNewZStencil));
}

HRESULT IGA_IDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetDepthStencilSurface);
    return(m_pIDirect3DDevice9->GetDepthStencilSurface(ppZStencilSurface));
}

HRESULT IGA_IDirect3DDevice9::BeginScene(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::BeginScene);
	HRESULT hr = E_FAIL;
	hr = m_pIDirect3DDevice9->BeginScene();
    return( hr );
}

HRESULT IGA_IDirect3DDevice9::EndScene(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::EndScene);

	HRESULT hr = E_FAIL;

	hr = m_pIDirect3DDevice9->EndScene();
    
	return hr;
}

HRESULT IGA_IDirect3DDevice9::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::Clear);
    return(m_pIDirect3DDevice9->Clear(Count,pRects,Flags,Color,Z,Stencil));
}

HRESULT IGA_IDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	//SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::SetTransform);
    return(m_pIDirect3DDevice9->SetTransform(State,pMatrix));
}

HRESULT IGA_IDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetTransform );
    return(m_pIDirect3DDevice9->GetTransform(State,pMatrix));
}

HRESULT IGA_IDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::MultiplyTransform );
    return(m_pIDirect3DDevice9->MultiplyTransform(State,pMatrix));
}

HRESULT IGA_IDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetViewport );
    return(m_pIDirect3DDevice9->SetViewport(pViewport));
}

HRESULT IGA_IDirect3DDevice9::GetViewport(D3DVIEWPORT9* pViewport)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::GetViewport );
    return(m_pIDirect3DDevice9->GetViewport(pViewport));
}

HRESULT IGA_IDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetMaterial );
    return(m_pIDirect3DDevice9->SetMaterial(pMaterial));
}

HRESULT IGA_IDirect3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetMaterial );
    return(m_pIDirect3DDevice9->GetMaterial(pMaterial));
}

HRESULT IGA_IDirect3DDevice9::SetLight(DWORD Index,CONST D3DLIGHT9* pLight)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetLight );
    return(m_pIDirect3DDevice9->SetLight(Index,pLight));
}

HRESULT IGA_IDirect3DDevice9::GetLight(DWORD Index,D3DLIGHT9* pLight)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetLight );
    return(m_pIDirect3DDevice9->GetLight(Index,pLight));
}

HRESULT IGA_IDirect3DDevice9::LightEnable(DWORD Index,BOOL Enable)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::LightEnable );
    return(m_pIDirect3DDevice9->LightEnable(Index,Enable));
}

HRESULT IGA_IDirect3DDevice9::GetLightEnable(DWORD Index,BOOL* pEnable)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetLightEnable );
    return(m_pIDirect3DDevice9->GetLightEnable(Index, pEnable));
}

HRESULT IGA_IDirect3DDevice9::SetClipPlane(DWORD Index,CONST float* pPlane)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetClipPlane );
    return(m_pIDirect3DDevice9->SetClipPlane(Index, pPlane));
}

HRESULT IGA_IDirect3DDevice9::GetClipPlane(DWORD Index,float* pPlane)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetClipPlane );
    return(m_pIDirect3DDevice9->GetClipPlane(Index,pPlane));
}

HRESULT IGA_IDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	//SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetRenderState );
    return(m_pIDirect3DDevice9->SetRenderState(State, Value));
}

HRESULT IGA_IDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetRenderState );
    return(m_pIDirect3DDevice9->GetRenderState(State, pValue));
}

HRESULT IGA_IDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::CreateStateBlock );
    return(m_pIDirect3DDevice9->CreateStateBlock(Type,ppSB));
}

HRESULT IGA_IDirect3DDevice9::BeginStateBlock(void)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::BeginStateBlock );
    return(m_pIDirect3DDevice9->BeginStateBlock());
}

HRESULT IGA_IDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::EndStateBlock );
    return(m_pIDirect3DDevice9->EndStateBlock(ppSB));
}

HRESULT IGA_IDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetClipStatus );
    return(m_pIDirect3DDevice9->SetClipStatus(pClipStatus));
}

HRESULT IGA_IDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetClipStatus );
    return(m_pIDirect3DDevice9->GetClipStatus( pClipStatus));
}

HRESULT IGA_IDirect3DDevice9::GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetTexture );
    return(m_pIDirect3DDevice9->GetTexture(Stage,ppTexture));
}

HRESULT IGA_IDirect3DDevice9::SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
	//SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetTexture );
    return(m_pIDirect3DDevice9->SetTexture(Stage,pTexture));
}

HRESULT IGA_IDirect3DDevice9::GetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetTextureStageState );
    return(m_pIDirect3DDevice9->GetTextureStageState(Stage,Type, pValue));
}

HRESULT IGA_IDirect3DDevice9::SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
{
	//SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetTextureStageState );
    return(m_pIDirect3DDevice9->SetTextureStageState(Stage,Type,Value));
}

HRESULT IGA_IDirect3DDevice9::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetSamplerState );
    return(m_pIDirect3DDevice9->GetSamplerState(Sampler,Type, pValue));
}

HRESULT IGA_IDirect3DDevice9::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
{
	//SHOWOUTPUTDEBUG(IGA_IDirect3DDevice9::SetSamplerState );
    return(m_pIDirect3DDevice9->SetSamplerState(Sampler,Type,Value));
}

HRESULT IGA_IDirect3DDevice9::ValidateDevice(DWORD* pNumPasses)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::ValidateDevice );
    return(m_pIDirect3DDevice9->ValidateDevice( pNumPasses));
}

HRESULT IGA_IDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber,CONST PALETTEENTRY* pEntries)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetPaletteEntries );
    return(m_pIDirect3DDevice9->SetPaletteEntries(PaletteNumber, pEntries));
}

HRESULT IGA_IDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber,PALETTEENTRY* pEntries)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetPaletteEntries );
    return(m_pIDirect3DDevice9->GetPaletteEntries(PaletteNumber, pEntries));
}

HRESULT IGA_IDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetCurrentTexturePalette );
    return(m_pIDirect3DDevice9->SetCurrentTexturePalette(PaletteNumber));
}

HRESULT IGA_IDirect3DDevice9::GetCurrentTexturePalette(UINT *PaletteNumber)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetCurrentTexturePalette );
    return(m_pIDirect3DDevice9->GetCurrentTexturePalette(PaletteNumber));
}

HRESULT IGA_IDirect3DDevice9::SetScissorRect(CONST RECT* pRect)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetScissorRect );
    return(m_pIDirect3DDevice9->SetScissorRect( pRect));
}

HRESULT IGA_IDirect3DDevice9::GetScissorRect( RECT* pRect)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetScissorRect );
    return(m_pIDirect3DDevice9->GetScissorRect( pRect));
}

HRESULT IGA_IDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetSoftwareVertexProcessing );
    return(m_pIDirect3DDevice9->SetSoftwareVertexProcessing(bSoftware));
}

BOOL    IGA_IDirect3DDevice9::GetSoftwareVertexProcessing(void)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetSoftwareVertexProcessing );
    return(m_pIDirect3DDevice9->GetSoftwareVertexProcessing());
}

HRESULT IGA_IDirect3DDevice9::SetNPatchMode(float nSegments)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetNPatchMode );
    return(m_pIDirect3DDevice9->SetNPatchMode(nSegments));
}

float   IGA_IDirect3DDevice9::GetNPatchMode(void)
{
	SHOWOUTPUTDEBUG(  IGA_IDirect3DDevice9::GetNPatchMode );
    return(m_pIDirect3DDevice9->GetNPatchMode());
}

HRESULT IGA_IDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::DrawPrimitive );
    return(m_pIDirect3DDevice9->DrawPrimitive(PrimitiveType,StartVertex,PrimitiveCount));
}

HRESULT IGA_IDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::DrawIndexedPrimitive );
    return(m_pIDirect3DDevice9->DrawIndexedPrimitive(PrimitiveType,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount));
}

HRESULT IGA_IDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::DrawPrimitiveUP );
    return(m_pIDirect3DDevice9->DrawPrimitiveUP(PrimitiveType,PrimitiveCount,pVertexStreamZeroData,VertexStreamZeroStride));
}

HRESULT IGA_IDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::DrawIndexedPrimitiveUP );
    return(m_pIDirect3DDevice9->DrawIndexedPrimitiveUP(PrimitiveType,MinVertexIndex,NumVertices,PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData,VertexStreamZeroStride));
}

HRESULT IGA_IDirect3DDevice9::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::ProcessVertices );
    return(m_pIDirect3DDevice9->ProcessVertices( SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags));
}

HRESULT IGA_IDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::CreateVertexDeclaration );
    return(m_pIDirect3DDevice9->CreateVertexDeclaration( pVertexElements,ppDecl));
}

HRESULT IGA_IDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetVertexDeclaration );
    return(m_pIDirect3DDevice9->SetVertexDeclaration(pDecl));
}

HRESULT IGA_IDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetVertexDeclaration );
    return(m_pIDirect3DDevice9->GetVertexDeclaration(ppDecl));
}

HRESULT IGA_IDirect3DDevice9::SetFVF(DWORD FVF)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetFVF );
    return(m_pIDirect3DDevice9->SetFVF(FVF));
}

HRESULT IGA_IDirect3DDevice9::GetFVF(DWORD* pFVF)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetFVF );
    return(m_pIDirect3DDevice9->GetFVF(pFVF));
}

HRESULT IGA_IDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::CreateVertexShader );
    return(m_pIDirect3DDevice9->CreateVertexShader(pFunction,ppShader));
}

HRESULT IGA_IDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetVertexShader );
    return(m_pIDirect3DDevice9->SetVertexShader(pShader));
}

HRESULT IGA_IDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetVertexShader );
    return(m_pIDirect3DDevice9->GetVertexShader(ppShader));
}

HRESULT IGA_IDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetVertexShaderConstantF );
    return(m_pIDirect3DDevice9->SetVertexShaderConstantF(StartRegister,pConstantData, Vector4fCount));
}

HRESULT IGA_IDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetVertexShaderConstantF );
    return(m_pIDirect3DDevice9->GetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount));
}

HRESULT IGA_IDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetVertexShaderConstantI );
    return(m_pIDirect3DDevice9->SetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount));
}

HRESULT IGA_IDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetVertexShaderConstantI );
    return(m_pIDirect3DDevice9->GetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount));
}

HRESULT IGA_IDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetVertexShaderConstantB );
    return(m_pIDirect3DDevice9->SetVertexShaderConstantB(StartRegister,pConstantData,BoolCount));
}

HRESULT IGA_IDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetVertexShaderConstantB );
    return(m_pIDirect3DDevice9->GetVertexShaderConstantB(StartRegister,pConstantData,BoolCount));
}

HRESULT IGA_IDirect3DDevice9::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetStreamSource );
    return(m_pIDirect3DDevice9->SetStreamSource(StreamNumber,pStreamData,OffsetInBytes,Stride));
}

HRESULT IGA_IDirect3DDevice9::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* OffsetInBytes,UINT* pStride)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetStreamSource );
    return(m_pIDirect3DDevice9->GetStreamSource(StreamNumber,ppStreamData,OffsetInBytes,pStride));
}

HRESULT IGA_IDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber,UINT Divider)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetStreamSourceFreq );
    return(m_pIDirect3DDevice9->SetStreamSourceFreq(StreamNumber,Divider));
}

HRESULT IGA_IDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber,UINT* Divider)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetStreamSourceFreq );
    return(m_pIDirect3DDevice9->GetStreamSourceFreq(StreamNumber,Divider));
}

HRESULT IGA_IDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetIndices );
    return(m_pIDirect3DDevice9->SetIndices(pIndexData));
}

HRESULT IGA_IDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetIndices );
    return(m_pIDirect3DDevice9->GetIndices(ppIndexData));
}

HRESULT IGA_IDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::CreatePixelShader );
    return(m_pIDirect3DDevice9->CreatePixelShader(pFunction,ppShader));
}

HRESULT IGA_IDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetPixelShader );
    return(m_pIDirect3DDevice9->SetPixelShader(pShader));
}

HRESULT IGA_IDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetPixelShader );
    return(m_pIDirect3DDevice9->GetPixelShader(ppShader));
}

HRESULT IGA_IDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetPixelShaderConstantF );
    return(m_pIDirect3DDevice9->SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount));
}

HRESULT IGA_IDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetPixelShaderConstantF );
    return(m_pIDirect3DDevice9->GetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount));
}

HRESULT IGA_IDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetPixelShaderConstantI );
    return(m_pIDirect3DDevice9->SetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount));
}

HRESULT IGA_IDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetPixelShaderConstantI );
    return(m_pIDirect3DDevice9->GetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount));
}

HRESULT IGA_IDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::SetPixelShaderConstantB );
    return(m_pIDirect3DDevice9->SetPixelShaderConstantB(StartRegister,pConstantData,BoolCount));
}

HRESULT IGA_IDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::GetPixelShaderConstantB );
    return(m_pIDirect3DDevice9->GetPixelShaderConstantB(StartRegister,pConstantData,BoolCount));
}

HRESULT IGA_IDirect3DDevice9::DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::DrawRectPatch );
    return(m_pIDirect3DDevice9->DrawRectPatch(Handle,pNumSegs, pRectPatchInfo));
}

HRESULT IGA_IDirect3DDevice9::DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::DrawTriPatch );
    return(m_pIDirect3DDevice9->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo));
}

HRESULT IGA_IDirect3DDevice9::DeletePatch(UINT Handle)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::DeletePatch );
    return(m_pIDirect3DDevice9->DeletePatch(Handle));
}

HRESULT IGA_IDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)
{
	SHOWOUTPUTDEBUG( IGA_IDirect3DDevice9::CreateQuery );
    return(m_pIDirect3DDevice9->CreateQuery(Type,ppQuery));
}

// This is our test function
void IGA_IDirect3DDevice9::ShowWeAreHere(void)
{
	//DrawRectMy( this, NULL );

	HRESULT hre = E_FAIL;

	DWORD dwFvF = 0;
	hre = BeginScene();
	hre = GetFVF( &dwFvF );

	hre = SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
	hre = SetFVF( D3DFVF_CUSTOMVERTEX );
	hre = DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
	hre = SetFVF( dwFvF );

	hre = EndScene();


	//D3DRECT rec = {1,1,100,100};
	//m_pIDirect3DDevice9->Clear(1, &rec, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,255,0),0 ,0);
	//
	return;

}


