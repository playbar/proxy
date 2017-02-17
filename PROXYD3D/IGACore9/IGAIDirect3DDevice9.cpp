#include "StdAfx.h"
#include "Common.h"
#include "IGAIDirect3DTexture9.h"
#include "IGAIDirect3DDevice9.h"

#include <stdio.h>
#include <tchar.h>

#include "IGACore.h"



//-----------------------------------------------------------------
//3D模块
extern FUN_IGA3DINIT          g_pFunIGA3DInit          ;
extern FUN_IGA3DDestory       g_pFunIGA3DDestoryt         ;
extern FUN_IGA3DGetMatrixData g_pFunIGA3DGetMatrixData    ;
extern FUN_IGA3DRender        g_pFunIGA3DRender           ;
extern FUN_ISetFristDraw      g_pFunSetFristDraw          ;
extern HINSTANCE              g_hIGA9Core;
extern BOOL				      g_bInit3D;	
extern DWORD				  g_RenderPos;
//--------------------------------------------------------------------


IGAIDirect3DDevice9::IGAIDirect3DDevice9(IDirect3DDevice9* pOriginal)
{

#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::IGAIDirect3DDevice9");
#endif
    m_pIDirect3DDevice9 = pOriginal; // store the pointer to original object

	//-----------------------------------------------
	if( g_bInit3D )
	{
		g_RenderPos = g_pFunIGA3DInit( m_pIDirect3DDevice9 );
	}
	//-----------------------------------------------
}

IGAIDirect3DDevice9::~IGAIDirect3DDevice9(void)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::~IGAIDirect3DDevice9");
#endif
	IGAWIN::DestoryIGAWin(this);
}

HRESULT IGAIDirect3DDevice9::QueryInterface (REFIID riid, void** ppvObj) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::QueryInterface");
#endif
	*ppvObj = NULL;
	HRESULT hRes = m_pIDirect3DDevice9->QueryInterface(riid, ppvObj); 
	if (hRes == NOERROR) {
		*ppvObj = this;
	}
	return hRes;
}

ULONG IGAIDirect3DDevice9::AddRef(void)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::AddRef");
#endif
	ULONG lCount = m_pIDirect3DDevice9->AddRef();
    return lCount;
}

ULONG IGAIDirect3DDevice9::Release(void)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::Release");
#endif
	extern IGAIDirect3DDevice9* gl_pIGAIDirect3DDevice9;

	ULONG lCount = m_pIDirect3DDevice9->Release();

	/*if (lCount == m_dwRefCount) 
	{
		GetIGACore()->OnDestroyD3DDevice(gl_pIGAIDirect3DDevice9);
		gl_pIGAIDirect3DDevice9 = NULL;
  	    delete(this);
	}*/
	return lCount;
}

HRESULT IGAIDirect3DDevice9::TestCooperativeLevel(void) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::TestCooperativeLevel");
#endif
    return m_pIDirect3DDevice9->TestCooperativeLevel();
}

UINT IGAIDirect3DDevice9::GetAvailableTextureMem(void) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetAvailableTextureMem");
#endif
    return m_pIDirect3DDevice9->GetAvailableTextureMem();
}

HRESULT IGAIDirect3DDevice9::EvictManagedResources(void) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::EvictManagedResources");
#endif
    return m_pIDirect3DDevice9->EvictManagedResources();
}

HRESULT IGAIDirect3DDevice9::GetDirect3D(IDirect3D9** ppD3D9) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetDirect3D");
#endif
    return m_pIDirect3DDevice9->GetDirect3D(ppD3D9);
}

HRESULT IGAIDirect3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetDeviceCaps");
#endif
    return m_pIDirect3DDevice9->GetDeviceCaps(pCaps);
}

HRESULT IGAIDirect3DDevice9::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetDisplayMode");
#endif
    return m_pIDirect3DDevice9->GetDisplayMode(iSwapChain, pMode);
}

HRESULT IGAIDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetCreationParameters");
#endif
    return(m_pIDirect3DDevice9->GetCreationParameters(pParameters));
}

HRESULT IGAIDirect3DDevice9::SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetCursorProperties");
#endif
    return(m_pIDirect3DDevice9->SetCursorProperties(XHotSpot,YHotSpot,pCursorBitmap));
}

void    IGAIDirect3DDevice9::SetCursorPosition(int X,int Y,DWORD Flags)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetCursorPosition");
#endif
    return(m_pIDirect3DDevice9->SetCursorPosition(X,Y,Flags));
}

BOOL    IGAIDirect3DDevice9::ShowCursor(BOOL bShow)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::ShowCursor");
#endif
    return(m_pIDirect3DDevice9->ShowCursor(bShow));
}

HRESULT IGAIDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)  
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateAdditionalSwapChain");
#endif
    return(m_pIDirect3DDevice9->CreateAdditionalSwapChain(pPresentationParameters,pSwapChain));
}

HRESULT IGAIDirect3DDevice9::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetSwapChain");
#endif
    return(m_pIDirect3DDevice9->GetSwapChain(iSwapChain,pSwapChain));
}

UINT IGAIDirect3DDevice9::GetNumberOfSwapChains(void)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetNumberOfSwapChains");
#endif
    return(m_pIDirect3DDevice9->GetNumberOfSwapChains());
}

HRESULT IGAIDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::Reset");
#endif
	//-----------------------------------------------------
	if( g_bInit3D )
	{
		g_pFunIGA3DDestoryt();
	}

	HRESULT hr = m_pIDirect3DDevice9->Reset( pPresentationParameters);  

	if( SUCCEEDED(hr) && g_bInit3D )
	{
		g_RenderPos = g_pFunIGA3DInit( m_pIDirect3DDevice9 );
	}

	return hr;
	//-------------------------------------------------------
}


HRESULT IGAIDirect3DDevice9::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::Present");
#endif

	if( g_RenderPos == Per_Present && g_bInit3D )
	{
		g_pFunIGA3DRender();
	}

	HRESULT hr = m_pIDirect3DDevice9->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	if( g_bInit3D )
	{
		g_pFunSetFristDraw();
	}

	return hr;
}

HRESULT IGAIDirect3DDevice9::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetBackBuffer");
#endif
    return m_pIDirect3DDevice9->GetBackBuffer(iSwapChain,iBackBuffer,Type,ppBackBuffer);
}

HRESULT IGAIDirect3DDevice9::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetRasterStatus");
#endif
    return m_pIDirect3DDevice9->GetRasterStatus(iSwapChain,pRasterStatus);
}

HRESULT IGAIDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetDialogBoxMode");
#endif
    return m_pIDirect3DDevice9->SetDialogBoxMode(bEnableDialogs);
}

void IGAIDirect3DDevice9::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetGammaRamp");
#endif
    return m_pIDirect3DDevice9->SetGammaRamp(iSwapChain,Flags,pRamp);
}

void IGAIDirect3DDevice9::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetGammaRamp");
#endif
    return m_pIDirect3DDevice9->GetGammaRamp(iSwapChain,pRamp);
}

HRESULT IGAIDirect3DDevice9::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateTexture");
#endif

	static BOOL s_Replaced = FALSE;
	HRESULT hr;
	hr = m_pIDirect3DDevice9->CreateTexture(Width,Height,Levels,Usage,Format,Pool,ppTexture,pSharedHandle);
	if (SUCCEEDED(hr))
	{
		// 初始化 IDirect3DTexture9 Proxy
		if (!s_Replaced) {
			IDirect3DTexture9* pTexture = (*ppTexture);
			InitIDirect3DTexture9Proxy(pTexture);
			s_Replaced = TRUE;
		}
		GetIGACore()->OnCreateTexture(Width,Height,Levels,Usage,Format,Pool,*ppTexture,pSharedHandle);
	}
    return hr;
}

HRESULT IGAIDirect3DDevice9::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateVolumeTexture");
#endif
    return(m_pIDirect3DDevice9->CreateVolumeTexture(Width,Height,Depth,Levels,Usage,Format,Pool,ppVolumeTexture,pSharedHandle));
}

HRESULT IGAIDirect3DDevice9::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateCubeTexture");
#endif
    return(m_pIDirect3DDevice9->CreateCubeTexture(EdgeLength,Levels,Usage,Format,Pool,ppCubeTexture,pSharedHandle));
}

HRESULT IGAIDirect3DDevice9::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateVertexBuffer");
#endif
    return(m_pIDirect3DDevice9->CreateVertexBuffer(Length,Usage,FVF,Pool,ppVertexBuffer,pSharedHandle));
}

HRESULT IGAIDirect3DDevice9::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateIndexBuffer");
#endif
    return(m_pIDirect3DDevice9->CreateIndexBuffer(Length,Usage,Format,Pool,ppIndexBuffer,pSharedHandle));
}

HRESULT IGAIDirect3DDevice9::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateRenderTarget");
#endif
    return(m_pIDirect3DDevice9->CreateRenderTarget(Width,Height,Format,MultiSample,MultisampleQuality,Lockable,ppSurface,pSharedHandle));
}

HRESULT IGAIDirect3DDevice9::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateDepthStencilSurface");
#endif
    return(m_pIDirect3DDevice9->CreateDepthStencilSurface(Width,Height,Format,MultiSample,MultisampleQuality,Discard,ppSurface,pSharedHandle));
}

HRESULT IGAIDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::UpdateSurface");
#endif
    return(m_pIDirect3DDevice9->UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint));
}

HRESULT IGAIDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::UpdateTexture");
#endif
    return(m_pIDirect3DDevice9->UpdateTexture(pSourceTexture,pDestinationTexture));
}

HRESULT IGAIDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetRenderTargetData");
#endif
    return(m_pIDirect3DDevice9->GetRenderTargetData(pRenderTarget,pDestSurface));
}

HRESULT IGAIDirect3DDevice9::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetFrontBufferData");
#endif
    return(m_pIDirect3DDevice9->GetFrontBufferData(iSwapChain,pDestSurface));
}

HRESULT IGAIDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::StretchRect");
#endif
    return(m_pIDirect3DDevice9->StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter));
}

HRESULT IGAIDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::ColorFill");
#endif
    return(m_pIDirect3DDevice9->ColorFill(pSurface,pRect,color));
}

HRESULT IGAIDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateOffscreenPlainSurface");
#endif
    return(m_pIDirect3DDevice9->CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle));
}

HRESULT IGAIDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetRenderTarget");
#endif
    return(m_pIDirect3DDevice9->SetRenderTarget(RenderTargetIndex,pRenderTarget));
}

HRESULT IGAIDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetRenderTarget");
#endif
    return(m_pIDirect3DDevice9->GetRenderTarget(RenderTargetIndex,ppRenderTarget));
}

HRESULT IGAIDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetDepthStencilSurface");
#endif
    return(m_pIDirect3DDevice9->SetDepthStencilSurface(pNewZStencil));
}

HRESULT IGAIDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetDepthStencilSurface");
#endif

    return(m_pIDirect3DDevice9->GetDepthStencilSurface(ppZStencilSurface));
}

HRESULT IGAIDirect3DDevice9::BeginScene(void)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::BeginScene");
#endif

	HRESULT hr = m_pIDirect3DDevice9->BeginScene();
	GetIGACore()->OnBeginScene();
    return hr;
}

HRESULT IGAIDirect3DDevice9::EndScene(void)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::EndScene");
#endif
	HRESULT hr = m_pIDirect3DDevice9->EndScene();
	GetIGACore()->OnEndScene();
    return hr;
}

HRESULT IGAIDirect3DDevice9::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::Clear");
#endif
    return(m_pIDirect3DDevice9->Clear(Count,pRects,Flags,Color,Z,Stencil));
}

HRESULT IGAIDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetTransform");
#endif
	//-----------------------------------------------------
	if( g_bInit3D )
	{
		g_pFunIGA3DGetMatrixData( State, pMatrix );

		if(  g_RenderPos == Per_SetTransform && State == D3DTS_WORLD )
		{
			g_pFunIGA3DRender();
		}
	}
	//--------------------------------------------------

    return(m_pIDirect3DDevice9->SetTransform(State,pMatrix));
}

HRESULT IGAIDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetTransform");
#endif
    return(m_pIDirect3DDevice9->GetTransform(State,pMatrix));
}

HRESULT IGAIDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::MultiplyTransform");
#endif
    return(m_pIDirect3DDevice9->MultiplyTransform(State,pMatrix));
}

HRESULT IGAIDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetViewport");
#endif
    return(m_pIDirect3DDevice9->SetViewport(pViewport));
}

HRESULT IGAIDirect3DDevice9::GetViewport(D3DVIEWPORT9* pViewport)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetViewport");
#endif
    return(m_pIDirect3DDevice9->GetViewport(pViewport));
}

HRESULT IGAIDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetMaterial");
#endif
    return(m_pIDirect3DDevice9->SetMaterial(pMaterial));
}

HRESULT IGAIDirect3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetMaterial");
#endif
    return(m_pIDirect3DDevice9->GetMaterial(pMaterial));
}

HRESULT IGAIDirect3DDevice9::SetLight(DWORD Index,CONST D3DLIGHT9* pLight)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetLight");
#endif
    return(m_pIDirect3DDevice9->SetLight(Index,pLight));
}

HRESULT IGAIDirect3DDevice9::GetLight(DWORD Index,D3DLIGHT9* pLight)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetLight");
#endif
    return(m_pIDirect3DDevice9->GetLight(Index,pLight));
}

HRESULT IGAIDirect3DDevice9::LightEnable(DWORD Index,BOOL Enable)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::LightEnable");
#endif
    return(m_pIDirect3DDevice9->LightEnable(Index,Enable));
}

HRESULT IGAIDirect3DDevice9::GetLightEnable(DWORD Index,BOOL* pEnable) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetLightEnable");
#endif
    return m_pIDirect3DDevice9->GetLightEnable(Index, pEnable);
}

HRESULT IGAIDirect3DDevice9::SetClipPlane(DWORD Index,CONST float* pPlane) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetClipPlane");
#endif
    return m_pIDirect3DDevice9->SetClipPlane(Index, pPlane);
}

HRESULT IGAIDirect3DDevice9::GetClipPlane(DWORD Index,float* pPlane) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetClipPlane");
#endif
	return m_pIDirect3DDevice9->GetClipPlane(Index,pPlane);
}

HRESULT IGAIDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetRenderState");
#endif
	return m_pIDirect3DDevice9->SetRenderState(State, Value);
}

HRESULT IGAIDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetRenderState");
#endif
    return m_pIDirect3DDevice9->GetRenderState(State, pValue);
}

HRESULT IGAIDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateStateBlock");
#endif
    return m_pIDirect3DDevice9->CreateStateBlock(Type,ppSB);
}

HRESULT IGAIDirect3DDevice9::BeginStateBlock(void) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::BeginStateBlock");
#endif
    return(m_pIDirect3DDevice9->BeginStateBlock());
}

HRESULT IGAIDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::EndStateBlock");
#endif
    return m_pIDirect3DDevice9->EndStateBlock(ppSB);
}

HRESULT IGAIDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetClipStatus");
#endif

    return m_pIDirect3DDevice9->SetClipStatus(pClipStatus);
}

HRESULT IGAIDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetClipStatus");
#endif
    return m_pIDirect3DDevice9->GetClipStatus( pClipStatus);
}

HRESULT IGAIDirect3DDevice9::GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetTexture");
#endif
    return m_pIDirect3DDevice9->GetTexture(Stage,ppTexture);
}

HRESULT IGAIDirect3DDevice9::SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetTexture");
#endif
	HRESULT hr;
	if (pTexture != NULL)
	{
		IDirect3DTexture9* tex;
		hr = pTexture->QueryInterface(IID_IDirect3DTexture9, (void **) &tex);
		if (SUCCEEDED(hr) )
		{
			pTexture = GetIGACore()->OnSetTexture(Stage, tex);
			tex->Release();
		}
		else
		{
			// TODO: Failed!!!
		}
	}
	//if( b )
	//{
	//	b =false;
	//	hr = D3DXSaveTextureToFile(
	//			"Replace.dds",
	//			D3DXIFF_DDS,
	//			pTexture,
	//			NULL);
	//	
	//	if (SUCCEEDED(hr))
	//	{
	//		OutputDebugString("导出替换后的图片成功！");
	//	}
	//	else
	//	{
	//		OutputDebugString("导出替换后的图片失败！");
	//	}
	//}
	hr = m_pIDirect3DDevice9->SetTexture(Stage, pTexture);
	return hr;
}

HRESULT IGAIDirect3DDevice9::GetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetTextureStageState");
#endif
    return m_pIDirect3DDevice9->GetTextureStageState(Stage,Type, pValue);
}

HRESULT IGAIDirect3DDevice9::SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetTextureStageState");
#endif
    return m_pIDirect3DDevice9->SetTextureStageState(Stage,Type,Value);
}

HRESULT IGAIDirect3DDevice9::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetSamplerState");
#endif
    return(m_pIDirect3DDevice9->GetSamplerState(Sampler,Type, pValue));
}

HRESULT IGAIDirect3DDevice9::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetSamplerState");
#endif
    return(m_pIDirect3DDevice9->SetSamplerState(Sampler,Type,Value));
}

HRESULT IGAIDirect3DDevice9::ValidateDevice(DWORD* pNumPasses) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::ValidateDevice");
#endif
    return(m_pIDirect3DDevice9->ValidateDevice( pNumPasses));
}

HRESULT IGAIDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber,CONST PALETTEENTRY* pEntries) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetPaletteEntries");
#endif
    return(m_pIDirect3DDevice9->SetPaletteEntries(PaletteNumber, pEntries));
}

HRESULT IGAIDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber,PALETTEENTRY* pEntries) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetPaletteEntries");
#endif
    return(m_pIDirect3DDevice9->GetPaletteEntries(PaletteNumber, pEntries));
}

HRESULT IGAIDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetCurrentTexturePalette");
#endif
    return(m_pIDirect3DDevice9->SetCurrentTexturePalette(PaletteNumber));
}

HRESULT IGAIDirect3DDevice9::GetCurrentTexturePalette(UINT *PaletteNumber) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetCurrentTexturePalette");
#endif
    return(m_pIDirect3DDevice9->GetCurrentTexturePalette(PaletteNumber));
}

HRESULT IGAIDirect3DDevice9::SetScissorRect(CONST RECT* pRect) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetScissorRect");
#endif
    return(m_pIDirect3DDevice9->SetScissorRect( pRect));
}

HRESULT IGAIDirect3DDevice9::GetScissorRect( RECT* pRect) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetScissorRect");
#endif
    return(m_pIDirect3DDevice9->GetScissorRect( pRect));
}

HRESULT IGAIDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetSoftwareVertexProcessing");
#endif
    return(m_pIDirect3DDevice9->SetSoftwareVertexProcessing(bSoftware));
}

BOOL IGAIDirect3DDevice9::GetSoftwareVertexProcessing(void) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetSoftwareVertexProcessing");
#endif
    return(m_pIDirect3DDevice9->GetSoftwareVertexProcessing());
}

HRESULT IGAIDirect3DDevice9::SetNPatchMode(float nSegments) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetNPatchMode");
#endif
    return(m_pIDirect3DDevice9->SetNPatchMode(nSegments));
}

float IGAIDirect3DDevice9::GetNPatchMode(void) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetNPatchMode");
#endif
    return m_pIDirect3DDevice9->GetNPatchMode();
}

HRESULT IGAIDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::DrawPrimitive");
#endif

    return m_pIDirect3DDevice9->DrawPrimitive(PrimitiveType,StartVertex,PrimitiveCount);
}

HRESULT IGAIDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::DrawIndexedPrimitive");
#endif
    return m_pIDirect3DDevice9->DrawIndexedPrimitive(PrimitiveType,BaseVertexIndex,MinVertexIndex,NumVertices,startIndex,primCount);
}

HRESULT IGAIDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::DrawPrimitiveUP");
#endif
    return m_pIDirect3DDevice9->DrawPrimitiveUP(PrimitiveType,PrimitiveCount,pVertexStreamZeroData,VertexStreamZeroStride);
}

HRESULT IGAIDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::DrawIndexedPrimitiveUP");
#endif
    return m_pIDirect3DDevice9->DrawIndexedPrimitiveUP(PrimitiveType,MinVertexIndex,NumVertices,PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData,VertexStreamZeroStride);
}

HRESULT IGAIDirect3DDevice9::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::ProcessVertices");
#endif
    return m_pIDirect3DDevice9->ProcessVertices( SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT IGAIDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateVertexDeclaration");
#endif
    return m_pIDirect3DDevice9->CreateVertexDeclaration( pVertexElements,ppDecl);
}

HRESULT IGAIDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetVertexDeclaration");
#endif
    return m_pIDirect3DDevice9->SetVertexDeclaration(pDecl);
}

HRESULT IGAIDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetVertexDeclaration");
#endif
    return m_pIDirect3DDevice9->GetVertexDeclaration(ppDecl);
}

HRESULT IGAIDirect3DDevice9::SetFVF(DWORD FVF) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetFVF");
#endif
    return m_pIDirect3DDevice9->SetFVF(FVF);
}

HRESULT IGAIDirect3DDevice9::GetFVF(DWORD* pFVF) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetFVF");
#endif
    return m_pIDirect3DDevice9->GetFVF(pFVF);
}

HRESULT IGAIDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateVertexShader");
#endif
    return m_pIDirect3DDevice9->CreateVertexShader(pFunction,ppShader);
}

HRESULT IGAIDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetVertexShader");
#endif
    return m_pIDirect3DDevice9->SetVertexShader(pShader);
}

HRESULT IGAIDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetVertexShader");
#endif
    return m_pIDirect3DDevice9->GetVertexShader(ppShader);
}

HRESULT IGAIDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetVertexShaderConstantF");
#endif
    return m_pIDirect3DDevice9->SetVertexShaderConstantF(StartRegister,pConstantData, Vector4fCount);
}

HRESULT IGAIDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetVertexShaderConstantF");
#endif
    return m_pIDirect3DDevice9->GetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT IGAIDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetVertexShaderConstantI");
#endif
    return m_pIDirect3DDevice9->SetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT IGAIDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetVertexShaderConstantI");
#endif
    return m_pIDirect3DDevice9->GetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT IGAIDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetVertexShaderConstantB");
#endif
    return m_pIDirect3DDevice9->SetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT IGAIDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetVertexShaderConstantB");
#endif
    return m_pIDirect3DDevice9->GetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT IGAIDirect3DDevice9::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetStreamSource");
#endif
    return m_pIDirect3DDevice9->SetStreamSource(StreamNumber,pStreamData,OffsetInBytes,Stride);
}

HRESULT IGAIDirect3DDevice9::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* OffsetInBytes,UINT* pStride) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetStreamSource");
#endif
    return m_pIDirect3DDevice9->GetStreamSource(StreamNumber,ppStreamData,OffsetInBytes,pStride);
}

HRESULT IGAIDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber,UINT Divider) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetStreamSourceFreq");
#endif
    return m_pIDirect3DDevice9->SetStreamSourceFreq(StreamNumber,Divider);
}

HRESULT IGAIDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber,UINT* Divider)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetStreamSourceFreq");
#endif
    return m_pIDirect3DDevice9->GetStreamSourceFreq(StreamNumber,Divider);
}

HRESULT IGAIDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetIndices");
#endif
    return m_pIDirect3DDevice9->SetIndices(pIndexData);
}

HRESULT IGAIDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetIndices");
#endif
    return m_pIDirect3DDevice9->GetIndices(ppIndexData);
}

HRESULT IGAIDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreatePixelShader");
#endif
    return m_pIDirect3DDevice9->CreatePixelShader(pFunction,ppShader);
}

HRESULT IGAIDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetPixelShader");
#endif
    return m_pIDirect3DDevice9->SetPixelShader(pShader);
}

HRESULT IGAIDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetPixelShader");
#endif
    return m_pIDirect3DDevice9->GetPixelShader(ppShader);
}

HRESULT IGAIDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetPixelShaderConstantF");
#endif
    return m_pIDirect3DDevice9->SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT IGAIDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetPixelShaderConstantF");
#endif
    return m_pIDirect3DDevice9->GetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT IGAIDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetPixelShaderConstantI");
#endif
    return m_pIDirect3DDevice9->SetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT IGAIDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetPixelShaderConstantI");
#endif
    return m_pIDirect3DDevice9->GetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT IGAIDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::SetPixelShaderConstantB");
#endif
    return m_pIDirect3DDevice9->SetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT IGAIDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::GetPixelShaderConstantB");
#endif
    return m_pIDirect3DDevice9->GetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT IGAIDirect3DDevice9::DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::DrawRectPatch");
#endif
    return m_pIDirect3DDevice9->DrawRectPatch(Handle,pNumSegs, pRectPatchInfo);
}

HRESULT IGAIDirect3DDevice9::DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::DrawTriPatch");
#endif
    return m_pIDirect3DDevice9->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT IGAIDirect3DDevice9::DeletePatch(UINT Handle) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::DeletePatch");
#endif
    return m_pIDirect3DDevice9->DeletePatch(Handle);
}

HRESULT IGAIDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3DDevice9::CreateQuery");
#endif
    return m_pIDirect3DDevice9->CreateQuery(Type,ppQuery);
}


