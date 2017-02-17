#include "stdafx.h"
#include "D3D8Filter.h"
#include "d3d8.h"
#include  "ProxyIDirect3D8.h"

/******************************************************************************************************************/
/*                       IDirect3D8                                                                               */
/******************************************************************************************************************/
Filter_IDirect3D8::Filter_IDirect3D8(){}
Filter_IDirect3D8::~Filter_IDirect3D8(){}
VOID Filter_IDirect3D8::OnRelease_After      (ULONG  ulRefCount ){}
VOID Filter_IDirect3D8::OnCreateDevice_After (IDirect3DDevice8*  pDevice,HWND hFocusWindow,IDirect3DDevice8*  pOrgDevice){}

/******************************************************************************************************************/
/*                   IDirect3DDevice8                                                                                             */
/******************************************************************************************************************/	
Filter_IDirect3DDevice8::Filter_IDirect3DDevice8(){}
Filter_IDirect3DDevice8::~Filter_IDirect3DDevice8(){}

VOID  Filter_IDirect3DDevice8::OnRelease_After                 (IDirect3DDevice8* pDevice,ULONG&  ulRefCount ){}
/*** IDirect3DDevice8 methods ***/
VOID  Filter_IDirect3DDevice8::OnReset_Before                  (IDirect3DDevice8* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters){}
VOID  Filter_IDirect3DDevice8::OnReset_After                   (IDirect3DDevice8* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice8* pOrgDevice){}

VOID  Filter_IDirect3DDevice8::OnPresent_Before                (IDirect3DDevice8* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion){}
VOID  Filter_IDirect3DDevice8::OnPresent_After                 (IDirect3DDevice8* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion){}

VOID  Filter_IDirect3DDevice8::OnGetBackBuffer_Before          (IDirect3DDevice8* pDevice, UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface8** ppBackBuffer){}
VOID  Filter_IDirect3DDevice8::OnGetBackBuffer_After           (IDirect3DDevice8* pDevice, UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface8** ppBackBuffer){}

VOID  Filter_IDirect3DDevice8::OnCreateTexture_Before          (IDirect3DDevice8* pDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture8** ppTexture){}
VOID  Filter_IDirect3DDevice8::OnCreateTexture_After           (IDirect3DDevice8* pDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture8** ppTexture){}

VOID  Filter_IDirect3DDevice8::OnCreateVertexBuffer_Before     (IDirect3DDevice8* pDevice, UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer8** ppVertexBuffer){}
VOID  Filter_IDirect3DDevice8::OnCreateVertexBuffer_After      (IDirect3DDevice8* pDevice, UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer8** ppVertexBuffer){}

VOID  Filter_IDirect3DDevice8::OnCreateIndexBuffer_Before      (IDirect3DDevice8* pDevice, UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer8** ppIndexBuffer){}
VOID  Filter_IDirect3DDevice8::OnCreateIndexBuffer_After       (IDirect3DDevice8* pDevice, UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer8** ppIndexBuffer){}

VOID  Filter_IDirect3DDevice8::OnBeginScene_Before             (IDirect3DDevice8* pDevice ){}
VOID  Filter_IDirect3DDevice8::OnBeginScene_After              (IDirect3DDevice8* pDevice ){}

VOID  Filter_IDirect3DDevice8::OnEndScene_Before               (IDirect3DDevice8* pDevice ){}
VOID  Filter_IDirect3DDevice8::OnEndScene_After                (IDirect3DDevice8* pDevice ){}

VOID  Filter_IDirect3DDevice8::OnSetTransform_Before           (IDirect3DDevice8* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix){}
VOID  Filter_IDirect3DDevice8::OnSetTransform_After            (IDirect3DDevice8* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix){}

VOID  Filter_IDirect3DDevice8::OnSetViewport_Before            (IDirect3DDevice8* pDevice, CONST D3DVIEWPORT8* pViewport){}
VOID  Filter_IDirect3DDevice8::OnSetViewport_After             (IDirect3DDevice8* pDevice, CONST D3DVIEWPORT8* pViewport){}

VOID  Filter_IDirect3DDevice8::OnSetMaterial_Before            (IDirect3DDevice8* pDevice, CONST D3DMATERIAL8* pMaterial){}
VOID  Filter_IDirect3DDevice8::OnSetMaterial_After             (IDirect3DDevice8* pDevice, CONST D3DMATERIAL8* pMaterial){}

VOID  Filter_IDirect3DDevice8::OnSetLight_Before               (IDirect3DDevice8* pDevice, DWORD Index, CONST D3DLIGHT8* pLight){}
VOID  Filter_IDirect3DDevice8::OnSetLight_After                (IDirect3DDevice8* pDevice, DWORD Index, CONST D3DLIGHT8* pLight){}	

VOID  Filter_IDirect3DDevice8::OnSetRenderState_Before         (IDirect3DDevice8* pDevice, D3DRENDERSTATETYPE State, DWORD Value){}
VOID  Filter_IDirect3DDevice8::OnSetRenderState_After          (IDirect3DDevice8* pDevice, D3DRENDERSTATETYPE State, DWORD Value){}

VOID  Filter_IDirect3DDevice8::OnSetTexture_Before             (IDirect3DDevice8* pDevice, DWORD Stage, IDirect3DBaseTexture8* pTexture){}
VOID  Filter_IDirect3DDevice8::OnSetTexture_After              (IDirect3DDevice8* pDevice, DWORD Stage, IDirect3DBaseTexture8* pTexture){}

VOID  Filter_IDirect3DDevice8::OnDrawPrimitive_Before          (IDirect3DDevice8* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount){}
VOID  Filter_IDirect3DDevice8::OnDrawPrimitive_After           (IDirect3DDevice8* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount){}

VOID  Filter_IDirect3DDevice8::OnDrawIndexedPrimitive_Before   (IDirect3DDevice8* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount){}
VOID  Filter_IDirect3DDevice8::OnDrawIndexedPrimitive_After    (IDirect3DDevice8* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount){}

VOID  Filter_IDirect3DDevice8::OnDrawPrimitiveUP_Before        (IDirect3DDevice8* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){}
VOID  Filter_IDirect3DDevice8::OnDrawPrimitiveUP_After         (IDirect3DDevice8* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){}

VOID  Filter_IDirect3DDevice8::OnDrawIndexedPrimitiveUP_Before (IDirect3DDevice8* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){}
VOID  Filter_IDirect3DDevice8::OnDrawIndexedPrimitiveUP_After  (IDirect3DDevice8* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride){}

VOID  Filter_IDirect3DDevice8::OnSetStreamSource_Before        (IDirect3DDevice8* pDevice, UINT StreamNumber,IDirect3DVertexBuffer8* pStreamData,UINT Stride){}
VOID  Filter_IDirect3DDevice8::OnSetStreamSource_After         (IDirect3DDevice8* pDevice, UINT StreamNumber,IDirect3DVertexBuffer8* pStreamData,UINT Stride){}

VOID  Filter_IDirect3DDevice8::OnSetIndices_Before             (IDirect3DDevice8* pDevice, IDirect3DIndexBuffer8* pIndexData,UINT BaseVertexIndex){}	
VOID  Filter_IDirect3DDevice8::OnSetIndices_After              (IDirect3DDevice8* pDevice, IDirect3DIndexBuffer8* pIndexData,UINT BaseVertexIndex){}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////纹理////////////////////////////////////////////////////////////////////////////////////
#include "ProxyIDirect3DTexture8.h"
Filter_IDirect3DTexture8::Filter_IDirect3DTexture8() {}
Filter_IDirect3DTexture8::~Filter_IDirect3DTexture8(){}
VOID    Filter_IDirect3DTexture8::OnRelease_After      (IDirect3DDevice8*pDevice,IDirect3DTexture8*  pTexture,ULONG  ulRefCount       ){}
VOID    Filter_IDirect3DTexture8::OnGetSurfaceLevel_Before(IDirect3DTexture8*  pTexture, UINT& Level,IDirect3DSurface8** ppSurfaceLevel) {};
VOID    Filter_IDirect3DTexture8::OnGetSurfaceLevel_After (IDirect3DTexture8*  pTexture, UINT Level,IDirect3DSurface8** ppSurfaceLevel) {};

VOID    Filter_IDirect3DTexture8::OnLockRect_Before (IDirect3DTexture8*  pTexture, UINT& Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) {};
VOID    Filter_IDirect3DTexture8::OnLockRect_After  (IDirect3DTexture8*  pTexture, UINT& Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) {};

VOID    Filter_IDirect3DTexture8::OnUnlockRect_Before(IDirect3DTexture8*  pTexture, UINT Level) {};
VOID    Filter_IDirect3DTexture8::OnUnlockRect_After(IDirect3DTexture8*  pTexture, UINT Level) {};

/*原始函数调用*/
HRESULT Filter_IDirect3DTexture8::LockRectRaw   (IDirect3DTexture8*  pTexture,UINT& Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
	HRESULT hr = (*(IGA_IDirect3DTexture8::LP_FUN_LockRect)IGA_IDirect3DTexture8::s_pFunLockRect) (pTexture,Level,pLockedRect,pRect,Flags);
    return hr;	
}
HRESULT   Filter_IDirect3DTexture8::UnlockRectRaw (IDirect3DTexture8*  pTexture,UINT Level) 
{
	HRESULT hr = (*(IGA_IDirect3DTexture8::LP_FUN_UnLockRect)IGA_IDirect3DTexture8::s_pFunUnlockRect) (pTexture,Level);
    return hr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////表面////////////////////////////////////////////////////////////////////////////////////

#include "ProxyIDirect3DSurface8.h"
Filter_IDirect3DSurface8::Filter_IDirect3DSurface8(){}
Filter_IDirect3DSurface8:: ~Filter_IDirect3DSurface8(){}

VOID    Filter_IDirect3DSurface8::OnRelease_After  (IDirect3DDevice8*pDevice,IDirect3DSurface8* pSurface,ULONG  ulRefCount       ){}
VOID    Filter_IDirect3DSurface8::OnLockRect_Before(IDirect3DSurface8* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) {}
VOID    Filter_IDirect3DSurface8::OnLockRect_After (IDirect3DSurface8* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) {}
VOID    Filter_IDirect3DSurface8::OnUnlockRect_Before(IDirect3DDevice8* pDevice,IDirect3DSurface8* pSurface) {}
VOID    Filter_IDirect3DSurface8::OnUnlockRect_After (IDirect3DSurface8* pSurface) {}

/*原始函数调用*/
HRESULT   Filter_IDirect3DSurface8::LockRectRaw   (IDirect3DSurface8* pSurface,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
	HRESULT hr = (*(IGA_IDirect3DSurface8::LP_FUN_LockRect)IGA_IDirect3DSurface8::s_pFunLockRect)
		          (pSurface,pLockedRect, pRect,Flags);
    return hr;
}
HRESULT   Filter_IDirect3DSurface8::UnlockRectRaw (IDirect3DSurface8* pSurface) 
{
	HRESULT hr = (*(IGA_IDirect3DSurface8::LP_FUN_UnLockRect)IGA_IDirect3DSurface8::s_pFunUnlockRect) (pSurface);
    return hr;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////顶点缓存///////////////////////////////////////////////////////////////////////////////////
Filter_IDirect3DVertexBuffer8::Filter_IDirect3DVertexBuffer8(){}
Filter_IDirect3DVertexBuffer8::~Filter_IDirect3DVertexBuffer8(){}
VOID    Filter_IDirect3DVertexBuffer8::OnRelease_After      (ULONG  ulRefCount       ){}	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////索引缓存////////////////////////////////////////////////////////////////////////////////////
Filter_IDirect3DIndexBuffer8::Filter_IDirect3DIndexBuffer8(){}
Filter_IDirect3DIndexBuffer8::~Filter_IDirect3DIndexBuffer8(){}	
VOID   Filter_IDirect3DIndexBuffer8::OnRelease_After      (ULONG  ulRefCount       ){}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////交换链////////////////////////////////////////////////////////////////////////////////////
Filter_IDirect3DSwapChain8::Filter_IDirect3DSwapChain8(){}
Filter_IDirect3DSwapChain8::~Filter_IDirect3DSwapChain8(){}
VOID    Filter_IDirect3DSwapChain8::OnRelease_After      (ULONG  ulRefCount       ){}
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

D3D_FILTER   g_d3d_filters;
IDirect3D8*  InitD3D8(D3D_FILTER& Filter,const IDirect3D8*   pD3d)
{	 
	IGA_IDirect3D8* pProxyD3D  = new IGA_IDirect3D8(pD3d);
	if ( NULL != pProxyD3D )
	{
	   ::memcpy(&g_d3d_filters,&Filter,sizeof(D3D_FILTER));	
		pProxyD3D->SetFilter(Filter.pFilterD3D);
	}
	
	return pProxyD3D;
}
