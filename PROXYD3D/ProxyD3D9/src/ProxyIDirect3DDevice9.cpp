#include "StdAfx.h"
#include "ProxyIDirect3DDevice9.h"
#include "ProxyIDirect3DTexture9.h"
#include "ProxyIDirect3DSwapChain9.h"
//#include "ProxyIDirect3DXBuffer9.h"

extern D3D_FILTER   g_d3d_filters;
IGAIDirect3DDevice9::D3D_DEVICE_MAP    IGAIDirect3DDevice9::s_devices;;

IGAIDirect3DDevice9::IGAIDirect3DDevice9(IDirect3DDevice9 *pkOriginal)
{
	m_pkD3DDevice = pkOriginal;
	m_pFilter     = NULL;
	s_devices.insert(D3D_DEVICE_MAP::value_type(pkOriginal,this));
}

IGAIDirect3DDevice9::~IGAIDirect3DDevice9()
{	
    s_devices.erase(m_pkD3DDevice);
}

HRESULT IGAIDirect3DDevice9::QueryInterface (REFIID riid, void** ppvObj)
{
	*ppvObj = NULL;
	HRESULT hRes = m_pkD3DDevice->QueryInterface(riid, ppvObj); 
	if(hRes == NOERROR)
		*ppvObj = this;

	return hRes;
}

ULONG IGAIDirect3DDevice9::AddRef()
{
	return m_pkD3DDevice->AddRef();
}


ULONG IGAIDirect3DDevice9::Release()
{	
	ULONG ulCount = m_pkD3DDevice->Release();
	
	if  ( NULL != m_pFilter)
	{	    
		m_pFilter->OnRelease_After(this ,ulCount);
	}
	

	if(ulCount == 0)
	{
		delete this;
	}

	return ulCount;
}

HRESULT IGAIDirect3DDevice9::TestCooperativeLevel()
{
    return m_pkD3DDevice->TestCooperativeLevel();   
}
UINT    IGAIDirect3DDevice9::GetAvailableTextureMem()
{
    return m_pkD3DDevice->GetAvailableTextureMem();   
}
HRESULT IGAIDirect3DDevice9::EvictManagedResources()
{
	return m_pkD3DDevice->EvictManagedResources();
}
HRESULT IGAIDirect3DDevice9::GetDirect3D(IDirect3D9** ppD3D9)
{
	return m_pkD3DDevice->GetDirect3D( ppD3D9);
}
HRESULT IGAIDirect3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps)
{
	return m_pkD3DDevice->GetDeviceCaps(pCaps);
}
HRESULT IGAIDirect3DDevice9::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode)
{
	return m_pkD3DDevice->GetDisplayMode(iSwapChain, pMode);
}
HRESULT IGAIDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	return m_pkD3DDevice->GetCreationParameters(pParameters);
}
HRESULT IGAIDirect3DDevice9::SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)
{
	return m_pkD3DDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}
void    IGAIDirect3DDevice9::SetCursorPosition(int X,int Y,DWORD Flags)
{
	 m_pkD3DDevice->SetCursorPosition(X, Y, Flags);
}
BOOL    IGAIDirect3DDevice9::ShowCursor(BOOL bShow)
{
	return m_pkD3DDevice->ShowCursor(bShow);
}
HRESULT IGAIDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)
{
	HRESULT res= (m_pkD3DDevice->CreateAdditionalSwapChain(pPresentationParameters,pSwapChain));
	if(SUCCEEDED(res))
	{
		ExIDirect3DSwapChain9	*p	= new ExIDirect3DSwapChain9(*pSwapChain,m_pFilter,this);
		*pSwapChain	= p;
	}
	return res;
}
HRESULT IGAIDirect3DDevice9::GetSwapChain( UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	return m_pkD3DDevice->GetSwapChain(iSwapChain, pSwapChain);
}
UINT    IGAIDirect3DDevice9::GetNumberOfSwapChains()
{
	return m_pkD3DDevice->GetNumberOfSwapChains();
}
HRESULT IGAIDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if  ( NULL != m_pFilter)
		m_pFilter->OnReset_Before(this,pPresentationParameters);
	
	HRESULT hr = m_pkD3DDevice->Reset(pPresentationParameters);
	
	if  ( SUCCEEDED(hr) && NULL != m_pFilter)
		m_pFilter->OnReset_After(this,pPresentationParameters,m_pkD3DDevice);
	
	return hr;	
}
HRESULT IGAIDirect3DDevice9::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	if  ( NULL != m_pFilter)
        m_pFilter->OnPresent_Before(this,pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	
	HRESULT hr = m_pkD3DDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	
	if  ( NULL != m_pFilter)
        m_pFilter->OnPresent_After(this,pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	
	
	return hr;
	
}
HRESULT IGAIDirect3DDevice9::GetBackBuffer( UINT iSwapChain,UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	return m_pkD3DDevice->GetBackBuffer( iSwapChain, BackBuffer, Type, ppBackBuffer);
}
HRESULT IGAIDirect3DDevice9::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)
{
	return m_pkD3DDevice->GetRasterStatus(iSwapChain,pRasterStatus);
}
HRESULT IGAIDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs) 
{
	return m_pkD3DDevice->SetDialogBoxMode(bEnableDialogs);
}
void    IGAIDirect3DDevice9::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	 m_pkD3DDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
}
void    IGAIDirect3DDevice9::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp)
{
	 m_pkD3DDevice->GetGammaRamp(iSwapChain, pRamp);
}
HRESULT IGAIDirect3DDevice9::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) 
{
	static  BOOL  s_bReplace = FALSE;
	if  ( NULL != m_pFilter)
        m_pFilter->OnCreateTexture_Before(this,Width, Height, Levels, Usage, Format, Pool, ppTexture);
	
	HRESULT  hr = m_pkD3DDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture,pSharedHandle);
	if  ( hr == S_OK && !s_bReplace)
	{			
		if  ( g_d3d_filters.pFilterTexture != NULL )
		{
			IGA_IDirect3DTexture9::Init(*ppTexture);
			s_bReplace = TRUE;
		}
        IDirect3DSurface9*   pSurfaces = NULL;
		
		(*ppTexture)->GetSurfaceLevel(0,&pSurfaces);
		if  (pSurfaces != NULL )
			pSurfaces->Release();
		
	}
	
	if  ( NULL != m_pFilter)
        m_pFilter->OnCreateTexture_After(this,Width, Height, Levels, Usage, Format, Pool, ppTexture);
	
	return hr;		
}
HRESULT IGAIDirect3DDevice9::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle) 
{
	return m_pkD3DDevice->CreateVolumeTexture( Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}
HRESULT IGAIDirect3DDevice9::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle) 
{
	return m_pkD3DDevice->CreateCubeTexture( EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}
HRESULT IGAIDirect3DDevice9::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle) 
{
	return m_pkD3DDevice->CreateVertexBuffer( Length, Usage, FVF, Pool, ppVertexBuffer,pSharedHandle);
}
HRESULT IGAIDirect3DDevice9::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)
{
	return m_pkD3DDevice->CreateIndexBuffer( Length, Usage, Format,Pool, ppIndexBuffer, pSharedHandle);
}
HRESULT IGAIDirect3DDevice9::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pkD3DDevice->CreateRenderTarget( Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle) ;
}
HRESULT IGAIDirect3DDevice9::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pkD3DDevice->CreateDepthStencilSurface( Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface,pSharedHandle) ;
}
HRESULT IGAIDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint) 
{
	return m_pkD3DDevice->UpdateSurface(pSourceSurface,  pSourceRect, pDestinationSurface, pDestPoint) ;
}
HRESULT IGAIDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)
{
	return m_pkD3DDevice->UpdateTexture(pSourceTexture,pDestinationTexture);
}
HRESULT IGAIDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface) 
{
	return m_pkD3DDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
}
HRESULT IGAIDirect3DDevice9::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface) 
{
	return m_pkD3DDevice->GetFrontBufferData(iSwapChain,pDestSurface);
}
HRESULT IGAIDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)
{
	return m_pkD3DDevice->StretchRect(pSourceSurface,pSourceRect,pDestSurface, pDestRect,Filter);
}
HRESULT IGAIDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color)
{
	return m_pkD3DDevice->ColorFill( pSurface,  pRect, color);
}
HRESULT IGAIDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)
{
	return m_pkD3DDevice->CreateOffscreenPlainSurface( Width, Height, Format, Pool,ppSurface, pSharedHandle);
}
HRESULT IGAIDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget) 
{
	return m_pkD3DDevice->SetRenderTarget(RenderTargetIndex,pRenderTarget);
}
HRESULT IGAIDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget) 
{
	return m_pkD3DDevice->GetRenderTarget(RenderTargetIndex,ppRenderTarget);
}
HRESULT IGAIDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	return m_pkD3DDevice->SetDepthStencilSurface( pNewZStencil);
}
HRESULT IGAIDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface) 
{
	return m_pkD3DDevice->GetDepthStencilSurface(ppZStencilSurface);
}
HRESULT IGAIDirect3DDevice9::BeginScene() 
{
	return m_pkD3DDevice->BeginScene();
}
HRESULT IGAIDirect3DDevice9::EndScene() 
{
	return m_pkD3DDevice->EndScene();
}
HRESULT IGAIDirect3DDevice9::Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil) 
{
	return m_pkD3DDevice->Clear( Count, pRects, Flags, Color, Z, Stencil) ;
}
HRESULT IGAIDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix) 
{
	if  ( NULL != m_pFilter)
        m_pFilter->OnSetTransform_Before(this,State, pMatrix);
	
	HRESULT hr=  m_pkD3DDevice->SetTransform(State, pMatrix);
	
	if  ( NULL != m_pFilter)
        m_pFilter->OnSetTransform_After(this,State, pMatrix);
	
	return hr;
}
HRESULT IGAIDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix) 
{
	return m_pkD3DDevice->GetTransform( State, pMatrix) ;
}
HRESULT IGAIDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE p1,CONST D3DMATRIX* p2)
{
	return m_pkD3DDevice->MultiplyTransform(p1,p2);
}
HRESULT IGAIDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport) 
{
	return m_pkD3DDevice->SetViewport(pViewport);
}
HRESULT IGAIDirect3DDevice9::GetViewport(D3DVIEWPORT9* pViewport) 
{
	return m_pkD3DDevice->GetViewport(pViewport);
}
HRESULT IGAIDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial) 
{
	return m_pkD3DDevice->SetMaterial(pMaterial);
}
HRESULT IGAIDirect3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial)
{
	return m_pkD3DDevice->GetMaterial(pMaterial);
}
HRESULT IGAIDirect3DDevice9::SetLight(DWORD Index,CONST D3DLIGHT9* p2)
{
	return m_pkD3DDevice->SetLight(Index,p2);
}
HRESULT IGAIDirect3DDevice9::GetLight(DWORD Index,D3DLIGHT9* p2) 
{
	return m_pkD3DDevice->GetLight(Index,p2);
}
HRESULT IGAIDirect3DDevice9::LightEnable(DWORD Index,BOOL Enable) 
{
	return m_pkD3DDevice->LightEnable(Index,Enable);
}
HRESULT IGAIDirect3DDevice9::GetLightEnable(DWORD Index,BOOL* pEnable)
{
	return m_pkD3DDevice->GetLightEnable(Index, pEnable);
}
HRESULT IGAIDirect3DDevice9::SetClipPlane(DWORD Index,CONST float* pPlane) 
{
	return m_pkD3DDevice->SetClipPlane( Index, pPlane);
}
HRESULT IGAIDirect3DDevice9::GetClipPlane(DWORD Index,float* pPlane)
{
	return m_pkD3DDevice->GetClipPlane( Index, pPlane);
}
HRESULT IGAIDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value) 
{
	return m_pkD3DDevice->SetRenderState(State,Value) ;
}
HRESULT IGAIDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue) 
{
	return m_pkD3DDevice->GetRenderState( State, pValue) ;
}
HRESULT IGAIDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)
{
	return m_pkD3DDevice->CreateStateBlock(Type, ppSB);
}
HRESULT IGAIDirect3DDevice9::BeginStateBlock() 
{
	return m_pkD3DDevice->BeginStateBlock();
}
HRESULT IGAIDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	return m_pkD3DDevice->EndStateBlock( ppSB);
}
HRESULT IGAIDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	return m_pkD3DDevice->SetClipStatus(  pClipStatus);
}
HRESULT IGAIDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	return m_pkD3DDevice->GetClipStatus( pClipStatus);
}
HRESULT IGAIDirect3DDevice9::GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture)
{
	return m_pkD3DDevice->GetTexture( Stage, ppTexture);
}
HRESULT IGAIDirect3DDevice9::SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture) 
{
	if  ( NULL != m_pFilter)
        m_pFilter->OnSetTexture_Before(this,Stage, pTexture);
	
	HRESULT hr = m_pkD3DDevice->SetTexture(Stage, pTexture);
	
	if  ( NULL != m_pFilter)
        m_pFilter->OnSetTexture_After(this,Stage, pTexture);
	
	return hr;	
}
HRESULT IGAIDirect3DDevice9::GetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)
{
	return m_pkD3DDevice->GetTextureStageState( Stage, Type, pValue);
}
HRESULT IGAIDirect3DDevice9::SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value) 
{
	return m_pkD3DDevice->SetTextureStageState( Stage, Type, Value) ;
}
HRESULT IGAIDirect3DDevice9::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) 
{
	return m_pkD3DDevice->GetSamplerState( Sampler, Type, pValue) ;
}
HRESULT IGAIDirect3DDevice9::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) 
{
	return m_pkD3DDevice->SetSamplerState( Sampler,Type, Value) ;
}
HRESULT IGAIDirect3DDevice9::ValidateDevice(DWORD* pNumPasses) 
{
	return m_pkD3DDevice->ValidateDevice( pNumPasses) ;
}
HRESULT IGAIDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber,CONST PALETTEENTRY* pEntries) 
{	
	return m_pkD3DDevice->SetPaletteEntries( PaletteNumber,  pEntries) ;
}
HRESULT IGAIDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber,PALETTEENTRY* pEntries) 
{
	return m_pkD3DDevice->GetPaletteEntries( PaletteNumber,pEntries) ;
}
HRESULT IGAIDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber) 
{
	return m_pkD3DDevice->SetCurrentTexturePalette( PaletteNumber) ;
}
HRESULT IGAIDirect3DDevice9::GetCurrentTexturePalette(UINT *PaletteNumber) 
{
	return m_pkD3DDevice->GetCurrentTexturePalette(PaletteNumber) ;
}
HRESULT IGAIDirect3DDevice9::SetScissorRect(CONST RECT* pRect) 
{
	return m_pkD3DDevice->SetScissorRect(  pRect) ;
}
HRESULT IGAIDirect3DDevice9::GetScissorRect(RECT* pRect) 
{
	return m_pkD3DDevice->GetScissorRect( pRect) ;
}
HRESULT IGAIDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware) 
{
	return m_pkD3DDevice->SetSoftwareVertexProcessing( bSoftware) ;
}
BOOL    IGAIDirect3DDevice9::GetSoftwareVertexProcessing() 
{
	return m_pkD3DDevice->GetSoftwareVertexProcessing() ;
}
HRESULT IGAIDirect3DDevice9::SetNPatchMode(float nSegments) 
{
	return m_pkD3DDevice->SetNPatchMode( nSegments) ;
}
float   IGAIDirect3DDevice9::GetNPatchMode() 
{
	return m_pkD3DDevice->GetNPatchMode() ;
}
HRESULT IGAIDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount) 
{
	return m_pkD3DDevice->DrawPrimitive( PrimitiveType, StartVertex, PrimitiveCount) ;
}
HRESULT IGAIDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE p1,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount) 
{
	return m_pkD3DDevice->DrawIndexedPrimitive(p1, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount) ;
}
HRESULT IGAIDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) 
{
	return m_pkD3DDevice->DrawPrimitiveUP( PrimitiveType, PrimitiveCount,  pVertexStreamZeroData, VertexStreamZeroStride) ;
}
HRESULT IGAIDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) 
{
	return m_pkD3DDevice->DrawIndexedPrimitiveUP( PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount,  pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride) ;
}
HRESULT IGAIDirect3DDevice9::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) 
{
	return m_pkD3DDevice->ProcessVertices( SrcStartIndex, DestIndex, VertexCount,pDestBuffer, pVertexDecl, Flags) ;
}
HRESULT IGAIDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) 
{
	return m_pkD3DDevice->CreateVertexDeclaration( pVertexElements, ppDecl) ;
}
HRESULT IGAIDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) 
{
	return m_pkD3DDevice->SetVertexDeclaration( pDecl) ;
}
HRESULT IGAIDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) 
{
	return m_pkD3DDevice->GetVertexDeclaration( ppDecl) ;
}
HRESULT IGAIDirect3DDevice9::SetFVF(DWORD FVF) 
{
	return m_pkD3DDevice->SetFVF( FVF) ;
}
HRESULT IGAIDirect3DDevice9::GetFVF(DWORD* pFVF) 
{
	return m_pkD3DDevice->GetFVF( pFVF) ;
}
HRESULT IGAIDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) 
{
	return m_pkD3DDevice->CreateVertexShader(  pFunction, ppShader) ;
}
HRESULT IGAIDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader) 
{
	return m_pkD3DDevice->SetVertexShader( pShader) ;
}
HRESULT IGAIDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader) 
{
	return m_pkD3DDevice->GetVertexShader( ppShader) ;
}
HRESULT IGAIDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
	return m_pkD3DDevice->SetVertexShaderConstantF(StartRegister,pConstantData, Vector4fCount) ;
}
HRESULT IGAIDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
	return m_pkD3DDevice->GetVertexShaderConstantF( StartRegister, pConstantData, Vector4fCount) ;
}
HRESULT IGAIDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
	return m_pkD3DDevice->SetVertexShaderConstantI( StartRegister,  pConstantData, Vector4iCount) ;
}
HRESULT IGAIDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount) 
{
	return m_pkD3DDevice->GetVertexShaderConstantI( StartRegister, pConstantData, Vector4iCount) ;
}
HRESULT IGAIDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
	return m_pkD3DDevice->SetVertexShaderConstantB( StartRegister, pConstantData,  BoolCount) ;
}
HRESULT IGAIDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount) 
{
	return m_pkD3DDevice->GetVertexShaderConstantB( StartRegister, pConstantData, BoolCount) ;
}
HRESULT IGAIDirect3DDevice9::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) 
{
	return m_pkD3DDevice->SetStreamSource( StreamNumber,pStreamData, OffsetInBytes, Stride) ;
}
HRESULT IGAIDirect3DDevice9::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride) 
{
	return m_pkD3DDevice->GetStreamSource( StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}
HRESULT IGAIDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber,UINT Setting) 
{
	return m_pkD3DDevice->SetStreamSourceFreq( StreamNumber, Setting) ;
}
HRESULT IGAIDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber,UINT* pSetting) 
{
	return m_pkD3DDevice->GetStreamSourceFreq( StreamNumber,pSetting) ;
}
HRESULT IGAIDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData) 
{
	return m_pkD3DDevice->SetIndices( pIndexData) ;
}
HRESULT IGAIDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData) 
{
	return m_pkD3DDevice->GetIndices( ppIndexData);
}
HRESULT IGAIDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) 
{
	return m_pkD3DDevice->CreatePixelShader( pFunction, ppShader);
}
HRESULT IGAIDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader) 
{
	return m_pkD3DDevice->SetPixelShader( pShader);
}
HRESULT IGAIDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader) 
{
	return m_pkD3DDevice->GetPixelShader( ppShader);
}
HRESULT IGAIDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
	return m_pkD3DDevice->SetPixelShaderConstantF( StartRegister,  pConstantData, Vector4fCount);
}
HRESULT IGAIDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
	return m_pkD3DDevice->GetPixelShaderConstantF( StartRegister, pConstantData, Vector4fCount);
}
HRESULT IGAIDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
	return m_pkD3DDevice->SetPixelShaderConstantI(StartRegister,pConstantData, Vector4iCount);
}
HRESULT IGAIDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount) 
{
	return m_pkD3DDevice->GetPixelShaderConstantI( StartRegister, pConstantData,Vector4iCount);
}
HRESULT IGAIDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	return m_pkD3DDevice->SetPixelShaderConstantB( StartRegister,  pConstantData,  BoolCount);
}
HRESULT IGAIDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount) 
{
	return m_pkD3DDevice->GetPixelShaderConstantB( StartRegister, pConstantData, BoolCount);
}
HRESULT IGAIDirect3DDevice9::DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo) 
{
	return m_pkD3DDevice->DrawRectPatch( Handle,  pNumSegs,  pRectPatchInfo) ;
}
HRESULT IGAIDirect3DDevice9::DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo) 
{
	return m_pkD3DDevice->DrawTriPatch( Handle,  pNumSegs,  pTriPatchInfo) ;
}
HRESULT IGAIDirect3DDevice9::DeletePatch(UINT Handle) 
{
	return m_pkD3DDevice->DeletePatch(Handle);
}
HRESULT IGAIDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)
{
	return m_pkD3DDevice->CreateQuery( Type, ppQuery);
}