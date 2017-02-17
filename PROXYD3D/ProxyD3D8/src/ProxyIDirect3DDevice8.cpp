#include "StdAfx.h"
#include "ProxyIDirect3DDevice8.h"
#include "ProxyIDirect3DTexture8.h"
#include "ProxyIDirect3DXBuffer8.h"

extern D3D_FILTER   g_d3d_filters;
IGAIDirect3DDevice8::D3D_DEVICE_MAP    IGAIDirect3DDevice8::s_devices;;

IGAIDirect3DDevice8::IGAIDirect3DDevice8(IDirect3DDevice8 *pkOriginal)
{
	m_pkD3DDevice = pkOriginal;
	m_pFilter     = NULL;
	s_devices.insert(D3D_DEVICE_MAP::value_type(pkOriginal,this));
}

IGAIDirect3DDevice8::~IGAIDirect3DDevice8()
{	
    s_devices.erase(m_pkD3DDevice);
}

HRESULT IGAIDirect3DDevice8::QueryInterface (REFIID riid, void** ppvObj)
{
	*ppvObj = NULL;
	HRESULT hRes = m_pkD3DDevice->QueryInterface(riid, ppvObj); 
	if(hRes == NOERROR)
		*ppvObj = this;

	return hRes;
}

ULONG IGAIDirect3DDevice8::AddRef()
{
	return m_pkD3DDevice->AddRef();
}


ULONG IGAIDirect3DDevice8::Release()
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

HRESULT IGAIDirect3DDevice8::TestCooperativeLevel()
{
	
	return m_pkD3DDevice->TestCooperativeLevel();
}

UINT IGAIDirect3DDevice8::GetAvailableTextureMem()
{
	return m_pkD3DDevice->GetAvailableTextureMem();
}

HRESULT IGAIDirect3DDevice8::ResourceManagerDiscardBytes(DWORD Bytes)
{
	return m_pkD3DDevice->ResourceManagerDiscardBytes(Bytes);
}

HRESULT IGAIDirect3DDevice8::GetDirect3D(IDirect3D8** ppD3D8)
{
	return m_pkD3DDevice->GetDirect3D(ppD3D8);
}

HRESULT IGAIDirect3DDevice8::GetDeviceCaps(D3DCAPS8* pCaps)
{
	return m_pkD3DDevice->GetDeviceCaps(pCaps);
}

HRESULT IGAIDirect3DDevice8::GetDisplayMode(D3DDISPLAYMODE* pMode)
{
	return m_pkD3DDevice->GetDisplayMode(pMode);
}

HRESULT IGAIDirect3DDevice8::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	return m_pkD3DDevice->GetCreationParameters(pParameters);
}

HRESULT IGAIDirect3DDevice8::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8* pCursorBitmap)
{
	return m_pkD3DDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void IGAIDirect3DDevice8::SetCursorPosition(int X, int Y, DWORD Flags)
{
	m_pkD3DDevice->SetCursorPosition(X, Y, Flags);
}

BOOL IGAIDirect3DDevice8::ShowCursor(BOOL bShow)
{
	return m_pkD3DDevice->ShowCursor(bShow);
}

HRESULT IGAIDirect3DDevice8::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain8** pSwapChain)  
{
	return m_pkD3DDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}


HRESULT IGAIDirect3DDevice8::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if  ( NULL != m_pFilter)
		m_pFilter->OnReset_Before(this,pPresentationParameters);

	HRESULT hr = m_pkD3DDevice->Reset(pPresentationParameters);
	
	if  ( NULL != m_pFilter)
		m_pFilter->OnReset_After(this,pPresentationParameters,m_pkD3DDevice);

	return hr;
}

HRESULT IGAIDirect3DDevice8::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	if  ( NULL != m_pFilter)
        m_pFilter->OnPresent_Before(this,pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	HRESULT hr = m_pkD3DDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	if  ( NULL != m_pFilter)
        m_pFilter->OnPresent_After(this,pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);


	return hr;
}

HRESULT IGAIDirect3DDevice8::GetBackBuffer(UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface8** ppBackBuffer)
{
	return m_pkD3DDevice->GetBackBuffer(BackBuffer, Type, ppBackBuffer);
}

HRESULT IGAIDirect3DDevice8::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus)
{
	return m_pkD3DDevice->GetRasterStatus(pRasterStatus);
}

void IGAIDirect3DDevice8::SetGammaRamp(DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	m_pkD3DDevice->SetGammaRamp(Flags, pRamp);
}

void IGAIDirect3DDevice8::GetGammaRamp(D3DGAMMARAMP* pRamp)
{
	m_pkD3DDevice->GetGammaRamp(pRamp);
}


HRESULT IGAIDirect3DDevice8::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture8** ppTexture)
{	
	static  BOOL  s_bReplace = FALSE;
	if  ( NULL != m_pFilter)
        m_pFilter->OnCreateTexture_Before(this,Width, Height, Levels, Usage, Format, Pool, ppTexture);

	HRESULT  hr = m_pkD3DDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture);
	if  ( hr == S_OK && !s_bReplace)
	{			
		if  ( g_d3d_filters.pFilterTexture != NULL )
		{
		    IGA_IDirect3DTexture8::Init(*ppTexture);
			s_bReplace = TRUE;
		}
        IDirect3DSurface8*   pSurfaces = NULL;

		(*ppTexture)->GetSurfaceLevel(0,&pSurfaces);
		if  (pSurfaces != NULL )
			pSurfaces->Release();

	}

	if  ( NULL != m_pFilter)
        m_pFilter->OnCreateTexture_After(this,Width, Height, Levels, Usage, Format, Pool, ppTexture);

	return hr;
}

HRESULT IGAIDirect3DDevice8::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture8** ppVolumeTexture)
{
	return m_pkD3DDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture);
}

HRESULT IGAIDirect3DDevice8::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture8** ppCubeTexture)
{
	return m_pkD3DDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture);
}

HRESULT IGAIDirect3DDevice8::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer8** ppVertexBuffer)
{
	static  BOOL  s_bReplace = FALSE;
	HRESULT  hr = m_pkD3DDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer);
	if  ( hr == S_OK && !s_bReplace )
	{
		if  ( g_d3d_filters.pFilterVertexBuffer != NULL )
		{
			IGA_IDirect3DVertexBuffer8::Init(*ppVertexBuffer);
			s_bReplace = TRUE;
		}
	}	
	return hr;
}

HRESULT IGAIDirect3DDevice8::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer8** ppIndexBuffer)
{	
	static  BOOL  s_bReplace = FALSE;
	HRESULT  hr = m_pkD3DDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer);
	if  ( hr == S_OK && !s_bReplace)
	{
		if  ( g_d3d_filters.pFilterIndexBuffer != NULL )
		{
			IGA_IDirect3DIndexBuffer8::Init(*ppIndexBuffer);
			s_bReplace = TRUE;
		}
	}	
	return hr;
}

HRESULT IGAIDirect3DDevice8::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,BOOL Lockable,IDirect3DSurface8** ppSurface)
{
	return m_pkD3DDevice->CreateRenderTarget(Width, Height, Format, MultiSample, Lockable, ppSurface);
}

HRESULT IGAIDirect3DDevice8::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,IDirect3DSurface8** ppSurface)
{
	return m_pkD3DDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, ppSurface);
}
HRESULT IGAIDirect3DDevice8::CreateImageSurface(UINT Width,UINT Height,D3DFORMAT Format,IDirect3DSurface8** ppSurface)
{
	return m_pkD3DDevice->CreateImageSurface(Width, Height, Format, ppSurface);
}

HRESULT IGAIDirect3DDevice8::CopyRects(IDirect3DSurface8* pSourceSurface,CONST RECT* pSourceRectsArray,UINT cRects,IDirect3DSurface8* pDestinationSurface,CONST POINT* pDestPointsArray)
{
	return m_pkD3DDevice->CopyRects(pSourceSurface, pSourceRectsArray, cRects, pDestinationSurface, pDestPointsArray);
}

HRESULT IGAIDirect3DDevice8::UpdateTexture(IDirect3DBaseTexture8* pSourceTexture,IDirect3DBaseTexture8* pDestinationTexture)
{
	return m_pkD3DDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT IGAIDirect3DDevice8::GetFrontBuffer(IDirect3DSurface8* pDestSurface)
{
	return m_pkD3DDevice->GetFrontBuffer(pDestSurface);
}

HRESULT IGAIDirect3DDevice8::SetRenderTarget(IDirect3DSurface8* pRenderTarget,IDirect3DSurface8* pNewZStencil)
{
	return m_pkD3DDevice->SetRenderTarget(pRenderTarget, pNewZStencil);
}

HRESULT IGAIDirect3DDevice8::GetRenderTarget(IDirect3DSurface8** ppRenderTarget)
{
	return m_pkD3DDevice->GetRenderTarget(ppRenderTarget);
}

HRESULT IGAIDirect3DDevice8::GetDepthStencilSurface(IDirect3DSurface8** ppZStencilSurface)
{
	return m_pkD3DDevice->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT IGAIDirect3DDevice8::BeginScene()
{
	return m_pkD3DDevice->BeginScene();
}

HRESULT IGAIDirect3DDevice8::EndScene()
{
	return m_pkD3DDevice->EndScene();
}

HRESULT IGAIDirect3DDevice8::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	return m_pkD3DDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT IGAIDirect3DDevice8::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	if  ( NULL != m_pFilter)
        m_pFilter->OnSetTransform_Before(this,State, pMatrix);

	HRESULT hr=  m_pkD3DDevice->SetTransform(State, pMatrix);

	if  ( NULL != m_pFilter)
        m_pFilter->OnSetTransform_After(this,State, pMatrix);

	return hr;
}

HRESULT IGAIDirect3DDevice8::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
	return m_pkD3DDevice->GetTransform(State, pMatrix);
}

HRESULT IGAIDirect3DDevice8::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return m_pkD3DDevice->MultiplyTransform(State, pMatrix);
}

HRESULT IGAIDirect3DDevice8::SetViewport(CONST D3DVIEWPORT8* pViewport)
{
	return m_pkD3DDevice->SetViewport(pViewport);
}

HRESULT IGAIDirect3DDevice8::GetViewport(D3DVIEWPORT8* pViewport)
{
	return m_pkD3DDevice->GetViewport(pViewport);
}

HRESULT IGAIDirect3DDevice8::SetMaterial(CONST D3DMATERIAL8* pMaterial)
{
	return m_pkD3DDevice->SetMaterial(pMaterial);
}

HRESULT IGAIDirect3DDevice8::GetMaterial(D3DMATERIAL8* pMaterial)
{
	return m_pkD3DDevice->GetMaterial(pMaterial);
}

HRESULT IGAIDirect3DDevice8::SetLight(DWORD Index, CONST D3DLIGHT8* pLight)
{
	return m_pkD3DDevice->SetLight(Index, pLight);
}

HRESULT IGAIDirect3DDevice8::GetLight(DWORD Index, D3DLIGHT8* pLight)
{
	return m_pkD3DDevice->GetLight(Index, pLight);
}

HRESULT IGAIDirect3DDevice8::LightEnable(DWORD Index, BOOL Enable)
{
	return m_pkD3DDevice->LightEnable(Index, Enable);
}

HRESULT IGAIDirect3DDevice8::GetLightEnable(DWORD Index, BOOL* pEnable)
{
	return m_pkD3DDevice->GetLightEnable(Index, pEnable);
}

HRESULT IGAIDirect3DDevice8::SetClipPlane(DWORD Index, CONST float* pPlane)
{
	return m_pkD3DDevice->SetClipPlane(Index, pPlane);
}

HRESULT IGAIDirect3DDevice8::GetClipPlane(DWORD Index, float* pPlane)
{
	return m_pkD3DDevice->GetClipPlane(Index, pPlane);
}

HRESULT IGAIDirect3DDevice8::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	return m_pkD3DDevice->SetRenderState(State, Value);
}

HRESULT IGAIDirect3DDevice8::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
	return m_pkD3DDevice->GetRenderState(State, pValue);
}

HRESULT IGAIDirect3DDevice8::BeginStateBlock()
{
	return m_pkD3DDevice->BeginStateBlock();
}

HRESULT IGAIDirect3DDevice8::EndStateBlock(DWORD* pToken)
{
	return m_pkD3DDevice->EndStateBlock(pToken);
}

HRESULT IGAIDirect3DDevice8::ApplyStateBlock(DWORD Token)
{
	return m_pkD3DDevice->ApplyStateBlock(Token);
}

HRESULT IGAIDirect3DDevice8::CaptureStateBlock(DWORD Token)
{
	return m_pkD3DDevice->CaptureStateBlock(Token);
}

HRESULT IGAIDirect3DDevice8::DeleteStateBlock(DWORD Token)
{
	return m_pkD3DDevice->DeleteStateBlock(Token);
}

HRESULT IGAIDirect3DDevice8::CreateStateBlock(D3DSTATEBLOCKTYPE Type,DWORD* pToken)
{
	return m_pkD3DDevice->CreateStateBlock(Type, pToken);
}

HRESULT IGAIDirect3DDevice8::SetClipStatus(CONST D3DCLIPSTATUS8* pClipStatus)
{
	return m_pkD3DDevice->SetClipStatus(pClipStatus);
}

HRESULT IGAIDirect3DDevice8::GetClipStatus(D3DCLIPSTATUS8* pClipStatus)
{
	return m_pkD3DDevice->GetClipStatus( pClipStatus);
}

HRESULT IGAIDirect3DDevice8::GetTexture(DWORD Stage, IDirect3DBaseTexture8** ppTexture)
{
	return m_pkD3DDevice->GetTexture(Stage, ppTexture);
}

HRESULT IGAIDirect3DDevice8::SetTexture(DWORD Stage, IDirect3DBaseTexture8* pTexture)
{
	if  ( NULL != m_pFilter)
        m_pFilter->OnSetTexture_Before(this,Stage, pTexture);

	HRESULT hr = m_pkD3DDevice->SetTexture(Stage, pTexture);

	if  ( NULL != m_pFilter)
        m_pFilter->OnSetTexture_After(this,Stage, pTexture);

	return hr;

}

HRESULT IGAIDirect3DDevice8::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	return m_pkD3DDevice->GetTextureStageState(Stage, Type, pValue);
}

HRESULT IGAIDirect3DDevice8::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	return m_pkD3DDevice->SetTextureStageState(Stage, Type, Value);
}

HRESULT IGAIDirect3DDevice8::ValidateDevice(DWORD* pNumPasses)
{
	return m_pkD3DDevice->ValidateDevice(pNumPasses);
}

HRESULT IGAIDirect3DDevice8::GetInfo(DWORD DevInfoID,void* pDevInfoStruct,DWORD DevInfoStructSize)
{
	return m_pkD3DDevice->GetInfo(DevInfoID, pDevInfoStruct, DevInfoStructSize);
}

HRESULT IGAIDirect3DDevice8::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	return m_pkD3DDevice->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT IGAIDirect3DDevice8::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	return m_pkD3DDevice->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT IGAIDirect3DDevice8::SetCurrentTexturePalette(UINT PaletteNumber)
{
	return m_pkD3DDevice->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT IGAIDirect3DDevice8::GetCurrentTexturePalette(UINT *PaletteNumber)
{
	return m_pkD3DDevice->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT IGAIDirect3DDevice8::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	return m_pkD3DDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT IGAIDirect3DDevice8::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	return m_pkD3DDevice->DrawIndexedPrimitive(PrimitiveType, minIndex, NumVertices, startIndex, primCount);
}

HRESULT IGAIDirect3DDevice8::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	return m_pkD3DDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT IGAIDirect3DDevice8::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	return m_pkD3DDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT IGAIDirect3DDevice8::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer8* pDestBuffer,DWORD Flags)
{
	return m_pkD3DDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, Flags);
}

HRESULT IGAIDirect3DDevice8::CreateVertexShader(CONST DWORD* pDeclaration,CONST DWORD* pFunction,DWORD* pHandle,DWORD Usage)
{
	return m_pkD3DDevice->CreateVertexShader(pDeclaration, pFunction, pHandle, Usage);
}

HRESULT IGAIDirect3DDevice8::SetVertexShader(DWORD Handle)
{
	return m_pkD3DDevice->SetVertexShader(Handle);
}

HRESULT IGAIDirect3DDevice8::GetVertexShader(DWORD* pHandle)
{
	return m_pkD3DDevice->GetVertexShader(pHandle);
}

HRESULT IGAIDirect3DDevice8::DeleteVertexShader(DWORD Handle)
{
	return m_pkD3DDevice->DeleteVertexShader(Handle);
}

HRESULT IGAIDirect3DDevice8::SetVertexShaderConstant(DWORD Register,CONST void* pConstantData,DWORD ConstantCount)
{
	return m_pkD3DDevice->SetVertexShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT IGAIDirect3DDevice8::GetVertexShaderConstant(DWORD Register,void* pConstantData,DWORD ConstantCount)
{
	return m_pkD3DDevice->GetVertexShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT IGAIDirect3DDevice8::GetVertexShaderDeclaration(DWORD Handle,void* pData,DWORD* pSizeOfData)
{
	return m_pkD3DDevice->GetVertexShaderDeclaration(Handle, pData, pSizeOfData);
}

HRESULT IGAIDirect3DDevice8::GetVertexShaderFunction(DWORD Handle,void* pData,DWORD* pSizeOfData)
{
	return m_pkD3DDevice->GetVertexShaderFunction(Handle, pData, pSizeOfData);
}

HRESULT IGAIDirect3DDevice8::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer8* pStreamData,UINT Stride)
{
	return m_pkD3DDevice->SetStreamSource(StreamNumber, pStreamData, Stride);
}

HRESULT IGAIDirect3DDevice8::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer8** ppStreamData,UINT* pStride)
{
	return m_pkD3DDevice->GetStreamSource(StreamNumber, ppStreamData, pStride);
}

HRESULT IGAIDirect3DDevice8::SetIndices(IDirect3DIndexBuffer8* pIndexData,UINT BaseVertexIndex)
{
	return m_pkD3DDevice->SetIndices(pIndexData, BaseVertexIndex);
}

HRESULT IGAIDirect3DDevice8::GetIndices(IDirect3DIndexBuffer8** ppIndexData,UINT* pBaseVertexIndex)
{
	return m_pkD3DDevice->GetIndices(ppIndexData, pBaseVertexIndex);
}

HRESULT IGAIDirect3DDevice8::CreatePixelShader(CONST DWORD* pFunction,DWORD* pHandle)
{
	return m_pkD3DDevice->CreatePixelShader(pFunction, pHandle);
}

HRESULT IGAIDirect3DDevice8::SetPixelShader(DWORD Handle)
{
	return m_pkD3DDevice->SetPixelShader(Handle);
}

HRESULT IGAIDirect3DDevice8::GetPixelShader(DWORD* pHandle)
{
	return m_pkD3DDevice->GetPixelShader(pHandle);
}

HRESULT IGAIDirect3DDevice8::DeletePixelShader(DWORD Handle)
{
	return m_pkD3DDevice->DeletePixelShader(Handle);
}

HRESULT IGAIDirect3DDevice8::SetPixelShaderConstant(DWORD Register,CONST void* pConstantData,DWORD ConstantCount)
{
	return m_pkD3DDevice->SetPixelShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT IGAIDirect3DDevice8::GetPixelShaderConstant(DWORD Register,void* pConstantData,DWORD ConstantCount)
{
	return m_pkD3DDevice->GetPixelShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT IGAIDirect3DDevice8::GetPixelShaderFunction(DWORD Handle,void* pData,DWORD* pSizeOfData)
{
	return m_pkD3DDevice->GetPixelShaderFunction(Handle, pData, pSizeOfData);
}

HRESULT IGAIDirect3DDevice8::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	return m_pkD3DDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT IGAIDirect3DDevice8::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	return m_pkD3DDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT IGAIDirect3DDevice8::DeletePatch(UINT Handle)
{
	return m_pkD3DDevice->DeletePatch(Handle);
}


IDirect3DDevice8* IGAIDirect3DDevice8::GetOrgDevice()
{
	return m_pkD3DDevice;
}
