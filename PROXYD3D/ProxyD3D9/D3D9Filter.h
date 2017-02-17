//************************************************************************************************************
// class      :  
// purpose    :  d3d9接口过滤器
// Date       :  2009-7-31  9:49:46
// author     :  3o3afe1
//************************************************************************************************************
#ifndef   _D3DFILTER_H
#define   _D3DFILTER_H
#include <d3d9.h>

#ifdef IGAD3D9_DLL
   #pragma  message (" export ProxyD3D9 dll. 3o3afe1")
   #define IGASDK_API   __declspec(dllexport)
#else
   #pragma  message (" import ProxyD3D9 dll. 3o3afe1")
   #define IGASDK_API   __declspec(dllimport)
    #ifdef _DEBUG
        #pragma comment(lib,"..\\lib\\ProxyD3D9d.lib") 
    #else
        #pragma comment(lib,"..\\lib\\ProxyD3D9.lib") 
    #endif
#endif

/*d3d过滤器*/
class  IGASDK_API Filter_IDirect3D9
{
public:	
	Filter_IDirect3D9();
	virtual   ~Filter_IDirect3D9();

    virtual   VOID    OnRelease_After      (ULONG  ulRefCount       );
	virtual   VOID    OnCreateDevice_After (IDirect3DDevice9*  pDevice,HWND hFocusWindow,IDirect3DDevice9* pDeviceOrg);
};




/*设备过滤器*/
class IGASDK_API Filter_IDirect3DDevice9
{
public:
	Filter_IDirect3DDevice9();
	virtual ~Filter_IDirect3DDevice9();

    virtual VOID OnRelease_After                 (IDirect3DDevice9* pDevice,ULONG&  ulRefCount );	
	
	virtual VOID OnReset_Before                  (IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	virtual VOID OnReset_After                   (IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9* pOrgDevice);

	virtual VOID OnPresent_Before                (IDirect3DDevice9* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	virtual VOID OnPresent_After                 (IDirect3DDevice9* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);

	virtual VOID OnGetBackBuffer_Before          (IDirect3DDevice9* pDevice, UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
    virtual VOID OnGetBackBuffer_After           (IDirect3DDevice9* pDevice, UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);

	virtual VOID OnCreateTexture_Before          (IDirect3DDevice9* pDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture);
	virtual VOID OnCreateTexture_After           (IDirect3DDevice9* pDevice, UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture);

	virtual VOID OnCreateVertexBuffer_Before     (IDirect3DDevice9* pDevice, UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer);
    virtual VOID OnCreateVertexBuffer_After      (IDirect3DDevice9* pDevice, UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer);

	virtual VOID OnCreateIndexBuffer_Before      (IDirect3DDevice9* pDevice, UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer);
	virtual VOID OnCreateIndexBuffer_After       (IDirect3DDevice9* pDevice, UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer);
	
	virtual VOID OnBeginScene_Before             (IDirect3DDevice9* pDevice);
	virtual VOID OnBeginScene_After              (IDirect3DDevice9* pDevice);
	
	virtual VOID OnEndScene_Before               (IDirect3DDevice9* pDevice);
	virtual VOID OnEndScene_After                (IDirect3DDevice9* pDevice);
	
	virtual VOID OnSetTransform_Before           (IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);
    virtual VOID OnSetTransform_After            (IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);

	virtual VOID OnSetViewport_Before            (IDirect3DDevice9* pDevice, CONST D3DVIEWPORT9* pViewport);
	virtual VOID OnSetViewport_After             (IDirect3DDevice9* pDevice, CONST D3DVIEWPORT9* pViewport);

	virtual VOID OnSetMaterial_Before            (IDirect3DDevice9* pDevice, CONST D3DMATERIAL9* pMaterial);
	virtual VOID OnSetMaterial_After             (IDirect3DDevice9* pDevice, CONST D3DMATERIAL9* pMaterial);

	virtual VOID OnSetLight_Before               (IDirect3DDevice9* pDevice, DWORD Index, CONST D3DLIGHT9* pLight);
	virtual VOID OnSetLight_After                (IDirect3DDevice9* pDevice, DWORD Index, CONST D3DLIGHT9* pLight);	

	virtual VOID OnSetRenderState_Before         (IDirect3DDevice9* pDevice, D3DRENDERSTATETYPE State, DWORD Value);
	virtual VOID OnSetRenderState_After          (IDirect3DDevice9* pDevice, D3DRENDERSTATETYPE State, DWORD Value);

	virtual VOID OnSetTexture_Before             (IDirect3DDevice9* pDevice, DWORD Stage, IDirect3DBaseTexture9* pTexture);
	virtual VOID OnSetTexture_After              (IDirect3DDevice9* pDevice, DWORD Stage, IDirect3DBaseTexture9* pTexture);
	
	virtual VOID OnDrawPrimitive_Before          (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
	virtual VOID OnDrawPrimitive_After           (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);

	virtual VOID OnDrawIndexedPrimitive_Before   (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount);
	virtual VOID OnDrawIndexedPrimitive_After    (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount);

	virtual VOID OnDrawPrimitiveUP_Before        (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual VOID OnDrawPrimitiveUP_After         (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);

	virtual VOID OnDrawIndexedPrimitiveUP_Before (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual VOID OnDrawIndexedPrimitiveUP_After  (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);

	virtual VOID OnSetStreamSource_Before        (IDirect3DDevice9* pDevice, UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT Stride);
	virtual VOID OnSetStreamSource_After         (IDirect3DDevice9* pDevice, UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT Stride);

	virtual VOID OnSetIndices_Before             (IDirect3DDevice9* pDevice, IDirect3DIndexBuffer9* pIndexData,UINT BaseVertexIndex);	
	virtual VOID OnSetIndices_After              (IDirect3DDevice9* pDevice, IDirect3DIndexBuffer9* pIndexData,UINT BaseVertexIndex);	
	
};

/*纹理*/
class IGASDK_API Filter_IDirect3DTexture9
{
public:
	Filter_IDirect3DTexture9();
	virtual   ~Filter_IDirect3DTexture9();

    virtual   VOID    OnRelease_After      (IDirect3DDevice9*pDevice,IDirect3DTexture9*  pTexture,ULONG  ulRefCount       );

	virtual   VOID    OnGetSurfaceLevel_Before(IDirect3DTexture9*  pTexture, UINT& Level,IDirect3DSurface9** ppSurfaceLevel) ;
	virtual   VOID    OnGetSurfaceLevel_After (IDirect3DTexture9*  pTexture, UINT Level,IDirect3DSurface9** ppSurfaceLevel) ;

    virtual   VOID    OnLockRect_Before (IDirect3DTexture9*  pTexture, UINT& Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) ;
	virtual   VOID    OnLockRect_After  (IDirect3DTexture9*  pTexture, UINT& Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) ;

    virtual   VOID    OnUnlockRect_Before(IDirect3DTexture9*  pTexture, UINT Level) ;
	virtual   VOID    OnUnlockRect_After(IDirect3DTexture9*  pTexture, UINT Level) ;
    
	HRESULT   LockRectRaw   (IDirect3DTexture9*  pTexture,UINT& Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags);
	HRESULT   UnlockRectRaw (IDirect3DTexture9*  pTexture,UINT Level) ;

};

/*表面过滤*/
class IGASDK_API Filter_IDirect3DSurface9
{
public:
	Filter_IDirect3DSurface9();
	virtual   ~Filter_IDirect3DSurface9();
	
    virtual   VOID    OnRelease_After      (IDirect3DDevice9*pDevice,IDirect3DSurface9* pSurface,ULONG  ulRefCount       );
	
	virtual   VOID    OnLockRect_Before(IDirect3DSurface9* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) ;
	virtual   VOID    OnLockRect_After (IDirect3DSurface9* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) ;
	
	
	
    virtual   VOID    OnUnlockRect_Before(IDirect3DDevice9* pDevice,IDirect3DSurface9* pSurface) ;
	virtual   VOID    OnUnlockRect_After (IDirect3DSurface9* pSurface) ;
	
	
	HRESULT   LockRectRaw   (IDirect3DSurface9* pSurface,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags);
	HRESULT   UnlockRectRaw (IDirect3DSurface9* pSurface) ;
	
};

/*顶点缓冲区*/
class IGASDK_API Filter_IDirect3DVertexBuffer9
{
public:
	Filter_IDirect3DVertexBuffer9();
	virtual   ~Filter_IDirect3DVertexBuffer9();

    virtual   VOID    OnRelease_After      (ULONG  ulRefCount       );

};


/*索引缓存*/
class IGASDK_API Filter_IDirect3DIndexBuffer9
{
public:
	Filter_IDirect3DIndexBuffer9();
	virtual   ~Filter_IDirect3DIndexBuffer9();

    virtual   VOID    OnRelease_After      (ULONG  ulRefCount       );

};


/*交换链*/
class IGASDK_API Filter_IDirect3DSwapChain9
{
public:
	Filter_IDirect3DSwapChain9();
	virtual   ~Filter_IDirect3DSwapChain9();

    virtual   VOID    OnRelease_After      (ULONG  ulRefCount       );

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct  
{
	Filter_IDirect3D9*               pFilterD3D;
	Filter_IDirect3DDevice9*         pFilterDevice;
	Filter_IDirect3DTexture9*        pFilterTexture; 
	Filter_IDirect3DVertexBuffer9*   pFilterVertexBuffer; 
	Filter_IDirect3DIndexBuffer9*    pFilterIndexBuffer; 
	Filter_IDirect3DSurface9*        pFilterSurface; 
	Filter_IDirect3DSwapChain9*      pFilterSwapChain; 
} D3D_FILTER;

extern "C"
{
     IDirect3D9*   InitD3D9(D3D_FILTER& Filter,const IDirect3D9*   pD3d);
};
#endif