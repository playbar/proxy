#include "stdafx.h"
#include <d3d9.h>
#include "D3D9Filter.h"
#include  "ProxyIDirect3D9.h"

/******************************************************************************************************************/
/*                       IDirect3D9                                                                               */
/******************************************************************************************************************/
Filter_IDirect3D9::Filter_IDirect3D9(){}
Filter_IDirect3D9::~Filter_IDirect3D9(){}
VOID Filter_IDirect3D9::OnRelease_After      (ULONG  ulRefCount ){}
VOID Filter_IDirect3D9::OnCreateDevice_After (IDirect3DDevice9*  pDevice,HWND hFocusWindow,IDirect3DDevice9* pDeviceOrg){}

/******************************************************************************************************************/
/*                   IDirect3DDevice9                                                                                             */
/******************************************************************************************************************/	
Filter_IDirect3DDevice9::Filter_IDirect3DDevice9(){}
Filter_IDirect3DDevice9::~Filter_IDirect3DDevice9(){}

VOID  Filter_IDirect3DDevice9::OnRelease_After                 (IDirect3DDevice9* pDevice,ULONG&  ulRefCount ){}
/*** IDirect3DDevice9 methods ***/
VOID  Filter_IDirect3DDevice9::OnReset_Before                  (IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters){}
VOID  Filter_IDirect3DDevice9::OnReset_After                   (IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9* pOrgDevice){}

VOID  Filter_IDirect3DDevice9::OnPresent_Before                (IDirect3DDevice9* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion){}
VOID  Filter_IDirect3DDevice9::OnPresent_After                 (IDirect3DDevice9* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion){}

VOID  Filter_IDirect3DDevice9::OnGetBackBuffer_Before          (IDirect3DDevice9* pDevice, UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer){}
VOID  Filter_IDirect3DDevice9::OnGetBackBuffer_After           (IDirect3DDevice9* pDevice, UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer){}

VOID  Filter_IDirect3DDevice9::OnCreateTexture_Before          (IDirect3DDevice9* pDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture){}
VOID  Filter_IDirect3DDevice9::OnCreateTexture_After           (IDirect3DDevice9* pDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture){}

VOID  Filter_IDirect3DDevice9::OnCreateVertexBuffer_Before     (IDirect3DDevice9* pDevice, UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer){}
VOID  Filter_IDirect3DDevice9::OnCreateVertexBuffer_After      (IDirect3DDevice9* pDevice, UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer){}

VOID  Filter_IDirect3DDevice9::OnCreateIndexBuffer_Before      (IDirect3DDevice9* pDevice, UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer){}
VOID  Filter_IDirect3DDevice9::OnCreateIndexBuffer_After       (IDirect3DDevice9* pDevice, UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer){}

VOID  Filter_IDirect3DDevice9::OnBeginScene_Before             (IDirect3DDevice9* pDevice ){}
VOID  Filter_IDirect3DDevice9::OnBeginScene_After              (IDirect3DDevice9* pDevice ){}

VOID  Filter_IDirect3DDevice9::OnEndScene_Before               (IDirect3DDevice9* pDevice ){}
VOID  Filter_IDirect3DDevice9::OnEndScene_After                (IDirect3DDevice9* pDevice ){}

VOID  Filter_IDirect3DDevice9::OnSetTransform_Before           (IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix){}
VOID  Filter_IDirect3DDevice9::OnSetTransform_After            (IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix){}

VOID  Filter_IDirect3DDevice9::OnSetViewport_Before            (IDirect3DDevice9* pDevice, CONST D3DVIEWPORT9* pViewport){}
VOID  Filter_IDirect3DDevice9::OnSetViewport_After             (IDirect3DDevice9* pDevice, CONST D3DVIEWPORT9* pViewport){}

VOID  Filter_IDirect3DDevice9::OnSetMaterial_Before            (IDirect3DDevice9* pDevice, CONST D3DMATERIAL9* pMaterial){}
VOID  Filter_IDirect3DDevice9::OnSetMaterial_After             (IDirect3DDevice9* pDevice, CONST D3DMATERIAL9* pMaterial){}

VOID  Filter_IDirect3DDevice9::OnSetLight_Before               (IDirect3DDevice9* pDevice, DWORD Index, CONST D3DLIGHT9* pLight){}
VOID  Filter_IDirect3DDevice9::OnSetLight_After                (IDirect3DDevice9* pDevice, DWORD Index, CONST D3DLIGHT9* pLight){}	

VOID  Filter_IDirect3DDevice9::OnSetRenderState_Before         (IDirect3DDevice9* pDevice, D3DRENDERSTATETYPE State, DWORD Value){}
VOID  Filter_IDirect3DDevice9::OnSetRenderState_After          (IDirect3DDevice9* pDevice, D3DRENDERSTATETYPE State, DWORD Value){}

VOID  Filter_IDirect3DDevice9::OnSetTexture_Before             (IDirect3DDevice9* pDevice, DWORD Stage, IDirect3DBaseTexture9* pTexture){}
VOID  Filter_IDirect3DDevice9::OnSetTexture_After              (IDirect3DDevice9* pDevice, DWORD Stage, IDirect3DBaseTexture9* pTexture){}

VOID  Filter_IDirect3DDevice9::OnDrawPrimitive_Before          (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount){}
VOID  Filter_IDirect3DDevice9::OnDrawPrimitive_After           (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount){}

VOID  Filter_IDirect3DDevice9::OnDrawIndexedPrimitive_Before   (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount){}
VOID  Filter_IDirect3DDevice9::OnDrawIndexedPrimitive_After    (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount){}

VOID  Filter_IDirect3DDevice9::OnDrawPrimitiveUP_Before        (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){}
VOID  Filter_IDirect3DDevice9::OnDrawPrimitiveUP_After         (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){}

VOID  Filter_IDirect3DDevice9::OnDrawIndexedPrimitiveUP_Before (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){}
VOID  Filter_IDirect3DDevice9::OnDrawIndexedPrimitiveUP_After  (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){}

VOID  Filter_IDirect3DDevice9::OnSetStreamSource_Before        (IDirect3DDevice9* pDevice, UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT Stride){}
VOID  Filter_IDirect3DDevice9::OnSetStreamSource_After         (IDirect3DDevice9* pDevice, UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT Stride){}

VOID  Filter_IDirect3DDevice9::OnSetIndices_Before             (IDirect3DDevice9* pDevice, IDirect3DIndexBuffer9* pIndexData,UINT BaseVertexIndex){}	
VOID  Filter_IDirect3DDevice9::OnSetIndices_After              (IDirect3DDevice9* pDevice, IDirect3DIndexBuffer9* pIndexData,UINT BaseVertexIndex){}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////纹理////////////////////////////////////////////////////////////////////////////////////
#include "ProxyIDirect3DTexture9.h"
Filter_IDirect3DTexture9::Filter_IDirect3DTexture9() {}
Filter_IDirect3DTexture9::~Filter_IDirect3DTexture9(){}
VOID    Filter_IDirect3DTexture9::OnRelease_After      (IDirect3DDevice9*pDevice,IDirect3DTexture9*  pTexture,ULONG  ulRefCount       ){}
VOID    Filter_IDirect3DTexture9::OnGetSurfaceLevel_Before(IDirect3DTexture9*  pTexture, UINT& Level,IDirect3DSurface9** ppSurfaceLevel) {};
VOID    Filter_IDirect3DTexture9::OnGetSurfaceLevel_After (IDirect3DTexture9*  pTexture, UINT Level,IDirect3DSurface9** ppSurfaceLevel) {};

VOID    Filter_IDirect3DTexture9::OnLockRect_Before (IDirect3DTexture9*  pTexture, UINT& Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) {};
VOID    Filter_IDirect3DTexture9::OnLockRect_After  (IDirect3DTexture9*  pTexture, UINT& Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) {};

VOID    Filter_IDirect3DTexture9::OnUnlockRect_Before(IDirect3DTexture9*  pTexture, UINT Level) {};
VOID    Filter_IDirect3DTexture9::OnUnlockRect_After(IDirect3DTexture9*  pTexture, UINT Level) {};

/*原始函数调用*/
HRESULT Filter_IDirect3DTexture9::LockRectRaw   (IDirect3DTexture9*  pTexture,UINT& Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
	HRESULT hr = (*(IGA_IDirect3DTexture9::LP_FUN_LockRect)IGA_IDirect3DTexture9::s_pFunLockRect) (pTexture,Level,pLockedRect,pRect,Flags);
    return hr;	
}
HRESULT   Filter_IDirect3DTexture9::UnlockRectRaw (IDirect3DTexture9*  pTexture,UINT Level) 
{
	HRESULT hr = (*(IGA_IDirect3DTexture9::LP_FUN_UnLockRect)IGA_IDirect3DTexture9::s_pFunUnlockRect) (pTexture,Level);
    return hr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////表面////////////////////////////////////////////////////////////////////////////////////

#include "ProxyIDirect3DSurface9.h"
Filter_IDirect3DSurface9::Filter_IDirect3DSurface9(){}
Filter_IDirect3DSurface9:: ~Filter_IDirect3DSurface9(){}

VOID    Filter_IDirect3DSurface9::OnRelease_After  (IDirect3DDevice9*pDevice,IDirect3DSurface9* pSurface,ULONG  ulRefCount       ){}
VOID    Filter_IDirect3DSurface9::OnLockRect_Before(IDirect3DSurface9* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) {}
VOID    Filter_IDirect3DSurface9::OnLockRect_After (IDirect3DSurface9* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) {}
VOID    Filter_IDirect3DSurface9::OnUnlockRect_Before(IDirect3DDevice9* pDevice,IDirect3DSurface9* pSurface) {}
VOID    Filter_IDirect3DSurface9::OnUnlockRect_After (IDirect3DSurface9* pSurface) {}

/*原始函数调用*/
HRESULT   Filter_IDirect3DSurface9::LockRectRaw   (IDirect3DSurface9* pSurface,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
	HRESULT hr = (*(IGA_IDirect3DSurface9::LP_FUN_LockRect)IGA_IDirect3DSurface9::s_pFunLockRect)
		          (pSurface,pLockedRect, pRect,Flags);
    return hr;
}
HRESULT   Filter_IDirect3DSurface9::UnlockRectRaw (IDirect3DSurface9* pSurface) 
{
	HRESULT hr = (*(IGA_IDirect3DSurface9::LP_FUN_UnLockRect)IGA_IDirect3DSurface9::s_pFunUnlockRect) (pSurface);
    return hr;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////顶点缓存///////////////////////////////////////////////////////////////////////////////////
Filter_IDirect3DVertexBuffer9::Filter_IDirect3DVertexBuffer9(){}
Filter_IDirect3DVertexBuffer9::~Filter_IDirect3DVertexBuffer9(){}
VOID    Filter_IDirect3DVertexBuffer9::OnRelease_After      (ULONG  ulRefCount       ){}	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////索引缓存////////////////////////////////////////////////////////////////////////////////////
Filter_IDirect3DIndexBuffer9::Filter_IDirect3DIndexBuffer9(){}
Filter_IDirect3DIndexBuffer9::~Filter_IDirect3DIndexBuffer9(){}	
VOID   Filter_IDirect3DIndexBuffer9::OnRelease_After      (ULONG  ulRefCount       ){}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////交换链////////////////////////////////////////////////////////////////////////////////////
Filter_IDirect3DSwapChain9::Filter_IDirect3DSwapChain9(){}
Filter_IDirect3DSwapChain9::~Filter_IDirect3DSwapChain9(){}
VOID    Filter_IDirect3DSwapChain9::OnRelease_After      (ULONG  ulRefCount       ){}
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

D3D_FILTER   g_d3d_filters;
IDirect3D9*  InitD3D9(D3D_FILTER& Filter,const IDirect3D9*   pD3d)
{	 
	IGA_IDirect3D9* pProxyD3D  = new IGA_IDirect3D9(pD3d);
	if ( NULL != pProxyD3D )
	{
	   ::memcpy(&g_d3d_filters,&Filter,sizeof(D3D_FILTER));	
		pProxyD3D->SetFilter(Filter.pFilterD3D);
	}
	
	return pProxyD3D;
}
