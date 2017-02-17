#ifndef IGAIDirect3DDevice8_H
#define IGAIDirect3DDevice8_H

#include <d3d8.h>
#include "..\D3D8Filter.h"

#pragma warning(disable:4786)
#include <map>

class IGAIDirect3DDevice8 : public IDirect3DDevice8
{
public:
    IGAIDirect3DDevice8(IDirect3DDevice8 *pkOriginal);
    virtual ~IGAIDirect3DDevice8();

	/*** IUnknown methods ***/
	HRESULT STDMETHODCALLTYPE QueryInterface (REFIID riid, void** ppvObj);
	ULONG   STDMETHODCALLTYPE AddRef();
	ULONG   STDMETHODCALLTYPE Release();

	/*** IDirect3DDevice8 methods ***/
	HRESULT STDMETHODCALLTYPE TestCooperativeLevel();
	UINT    STDMETHODCALLTYPE GetAvailableTextureMem();
	HRESULT STDMETHODCALLTYPE ResourceManagerDiscardBytes(DWORD Bytes);
	HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D8** ppD3D8);
	HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS8* pCaps);
	HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE* pMode);
	HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters);
	HRESULT STDMETHODCALLTYPE SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface8* pCursorBitmap);
	void    STDMETHODCALLTYPE SetCursorPosition(int X,int Y,DWORD Flags);
	BOOL    STDMETHODCALLTYPE ShowCursor(BOOL bShow);
	HRESULT STDMETHODCALLTYPE CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain8** pSwapChain);
	HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS* pPresentationParameters);
	HRESULT STDMETHODCALLTYPE Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface8** ppBackBuffer);
	HRESULT STDMETHODCALLTYPE GetRasterStatus(D3DRASTER_STATUS* pRasterStatus);
	void    STDMETHODCALLTYPE SetGammaRamp(DWORD Flags,CONST D3DGAMMARAMP* pRamp);
	void    STDMETHODCALLTYPE GetGammaRamp(D3DGAMMARAMP* pRamp);
	HRESULT STDMETHODCALLTYPE CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture8** ppTexture);
	HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture8** ppVolumeTexture);
	HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture8** ppCubeTexture);
	HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer8** ppVertexBuffer);
	HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer8** ppIndexBuffer);
	HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,BOOL Lockable,IDirect3DSurface8** ppSurface);
	HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,IDirect3DSurface8** ppSurface);
	HRESULT STDMETHODCALLTYPE CreateImageSurface(UINT Width,UINT Height,D3DFORMAT Format,IDirect3DSurface8** ppSurface);
	HRESULT STDMETHODCALLTYPE CopyRects(IDirect3DSurface8* pSourceSurface,CONST RECT* pSourceRectsArray,UINT cRects,IDirect3DSurface8* pDestinationSurface,CONST POINT* pDestPointsArray);
	HRESULT STDMETHODCALLTYPE UpdateTexture(IDirect3DBaseTexture8* pSourceTexture,IDirect3DBaseTexture8* pDestinationTexture);
	HRESULT STDMETHODCALLTYPE GetFrontBuffer(IDirect3DSurface8* pDestSurface);
	HRESULT STDMETHODCALLTYPE SetRenderTarget(IDirect3DSurface8* pRenderTarget,IDirect3DSurface8* pNewZStencil);
	HRESULT STDMETHODCALLTYPE GetRenderTarget(IDirect3DSurface8** ppRenderTarget);
	HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface8** ppZStencilSurface);
	HRESULT STDMETHODCALLTYPE BeginScene();
	HRESULT STDMETHODCALLTYPE EndScene();
	HRESULT STDMETHODCALLTYPE Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
	HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);
	HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
	HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);
	HRESULT STDMETHODCALLTYPE SetViewport(CONST D3DVIEWPORT8* pViewport);
	HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT8* pViewport);
	HRESULT STDMETHODCALLTYPE SetMaterial(CONST D3DMATERIAL8* pMaterial);
	HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL8* pMaterial);
	HRESULT STDMETHODCALLTYPE SetLight(DWORD Index, CONST D3DLIGHT8* pLight);
	HRESULT STDMETHODCALLTYPE GetLight(DWORD Index, D3DLIGHT8* pLight);
	HRESULT STDMETHODCALLTYPE LightEnable(DWORD Index, BOOL Enable);
	HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD Index, BOOL* pEnable);
	HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD Index, CONST float* pPlane);
	HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD Index, float* pPlane);
	HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
	HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue);
	HRESULT STDMETHODCALLTYPE BeginStateBlock();
	HRESULT STDMETHODCALLTYPE EndStateBlock(DWORD* pToken);
	HRESULT STDMETHODCALLTYPE ApplyStateBlock(DWORD Token);
	HRESULT STDMETHODCALLTYPE CaptureStateBlock(DWORD Token);
	HRESULT STDMETHODCALLTYPE DeleteStateBlock(DWORD Token);
	HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE Type,DWORD* pToken);
	HRESULT STDMETHODCALLTYPE SetClipStatus(CONST D3DCLIPSTATUS8* pClipStatus);
	HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS8* pClipStatus);
	HRESULT STDMETHODCALLTYPE GetTexture(DWORD Stage, IDirect3DBaseTexture8** ppTexture);
	HRESULT STDMETHODCALLTYPE SetTexture(DWORD Stage, IDirect3DBaseTexture8* pTexture);
	HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
	HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
	HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD* pNumPasses);
	HRESULT STDMETHODCALLTYPE GetInfo(DWORD DevInfoID,void* pDevInfoStruct,DWORD DevInfoStructSize);
	HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries);
	HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries);
	HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT PaletteNumber);
	HRESULT STDMETHODCALLTYPE GetCurrentTexturePalette(UINT *PaletteNumber);
	HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
	HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount);
	HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	HRESULT STDMETHODCALLTYPE ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer8* pDestBuffer,DWORD Flags);
	HRESULT STDMETHODCALLTYPE CreateVertexShader(CONST DWORD* pDeclaration,CONST DWORD* pFunction,DWORD* pHandle,DWORD Usage);
	HRESULT STDMETHODCALLTYPE SetVertexShader(DWORD Handle);
	HRESULT STDMETHODCALLTYPE GetVertexShader(DWORD* pHandle);
	HRESULT STDMETHODCALLTYPE DeleteVertexShader(DWORD Handle);
	HRESULT STDMETHODCALLTYPE SetVertexShaderConstant(DWORD Register,CONST void* pConstantData,DWORD ConstantCount);
	HRESULT STDMETHODCALLTYPE GetVertexShaderConstant(DWORD Register,void* pConstantData,DWORD ConstantCount);
	HRESULT STDMETHODCALLTYPE GetVertexShaderDeclaration(DWORD Handle,void* pData,DWORD* pSizeOfData);
	HRESULT STDMETHODCALLTYPE GetVertexShaderFunction(DWORD Handle,void* pData,DWORD* pSizeOfData);
	HRESULT STDMETHODCALLTYPE SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer8* pStreamData,UINT Stride);
	HRESULT STDMETHODCALLTYPE GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer8** ppStreamData,UINT* pStride);
	HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer8* pIndexData,UINT BaseVertexIndex);
	HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer8** ppIndexData,UINT* pBaseVertexIndex);
	HRESULT STDMETHODCALLTYPE CreatePixelShader(CONST DWORD* pFunction,DWORD* pHandle);
	HRESULT STDMETHODCALLTYPE SetPixelShader(DWORD Handle);
	HRESULT STDMETHODCALLTYPE GetPixelShader(DWORD* pHandle);
	HRESULT STDMETHODCALLTYPE DeletePixelShader(DWORD Handle);
	HRESULT STDMETHODCALLTYPE SetPixelShaderConstant(DWORD Register,CONST void* pConstantData,DWORD ConstantCount);
	HRESULT STDMETHODCALLTYPE GetPixelShaderConstant(DWORD Register,void* pConstantData,DWORD ConstantCount);
	HRESULT STDMETHODCALLTYPE GetPixelShaderFunction(DWORD Handle,void* pData,DWORD* pSizeOfData);
	HRESULT STDMETHODCALLTYPE DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo);
	HRESULT STDMETHODCALLTYPE DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo);
	HRESULT STDMETHODCALLTYPE DeletePatch(UINT Handle);


public:
	inline  VOID SetFilter(Filter_IDirect3DDevice8*  pFilter)	{		 m_pFilter = pFilter;	}

protected:
    IDirect3DDevice8     *m_pkD3DDevice;
	Filter_IDirect3DDevice8*   m_pFilter;

public:	
	IDirect3DDevice8* GetOrgDevice();
	typedef std::map<IDirect3DDevice8*,IGAIDirect3DDevice8*>               D3D_DEVICE_MAP; 
	typedef std::map<IDirect3DDevice8*,IGAIDirect3DDevice8*>::iterator     D3D_DEVICE_MAP_IT;
	static  D3D_DEVICE_MAP    s_devices;

	static  IDirect3DDevice8*   GetDevice(IDirect3DDevice8* pDevice)
	{
            D3D_DEVICE_MAP_IT it = s_devices.find(pDevice);
			if  ( it == s_devices.end())    return NULL;
			return it->second;
	}

};

#endif
