#ifndef _PROXY_IDIRECT3DSURFACE_H
#define _PROXY_IDIRECT3DSURFACE_H
#include <d3d9.h>
class Filter_IDirect3DSurface9;

class IGA_IDirect3DSurface9 : public IDirect3DSurface9
{  
public:
	typedef ULONG (WINAPI *LP_FUN_Release)    (IDirect3DSurface9*);
    typedef HRESULT (WINAPI *LP_FUN_LockRect) (IDirect3DSurface9*,D3DLOCKED_RECT*,CONST RECT*,DWORD);
    typedef HRESULT (WINAPI *LP_FUN_UnLockRect)(IDirect3DSurface9*);

private:
	enum
	{
            SURFACE9_QUERYINTERFACE_INDEX  =  0,		
			SURFACE9_ADDREF_INDEX          =  1,		
			SURFACE9_RELEASE_INDEX         =  2,		
			SURFACE9_GETDEVICE_INDEX       =  3,		
			SURFACE9_SETPRIVATEDATA_INDEX  =  4,		
			SURFACE9_GETPRIVATEDATA_INDEX  =  5,		
			SURFACE9_FREEPRIVATEDATA_INDEX =  6,
			SURFACE9_SETPRIORITY_INDEX     =  7,
			SURFACE9_GETPRIORITY_INDEX     =  8,
			SURFACE9_PRELOAD_INDEX         =  9,
			SURFACE9_GETTYPE_INDEX         = 10,

			SURFACE9_GETCONTAINER_INDEX    = 11,		
			SURFACE9_GETDESC_INDEX         = 12,		
			SURFACE9_LOCKRECT_INDEX        = 13,		
			SURFACE9_UNLOCKRECT_INDEX      = 14,					

			SURFACE9_GETDC_INDEX           = 15,		
			SURFACE9_RELEASEDC_INDEX       = 16,					
	};
public:
	/*** IUnknown methods ***/
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) ;
    ULONG   STDMETHODCALLTYPE AddRef() ;
    ULONG   STDMETHODCALLTYPE Release() ;
	
    /*** IDirect3DSurface9 methods ***/
    HRESULT STDMETHODCALLTYPE GetDevice( IDirect3DDevice9** ppDevice) ;
    HRESULT STDMETHODCALLTYPE SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) ;
    HRESULT STDMETHODCALLTYPE FreePrivateData( REFGUID refguid) ;

	DWORD   STDMETHODCALLTYPE SetPriority (DWORD PriorityNew) ;
	DWORD   STDMETHODCALLTYPE GetPriority();
	void    STDMETHODCALLTYPE PreLoad() ;
	D3DRESOURCETYPE STDMETHODCALLTYPE GetType();


    HRESULT STDMETHODCALLTYPE GetContainer( REFIID riid,void** ppContainer) ;
    HRESULT STDMETHODCALLTYPE GetDesc( D3DSURFACE_DESC *pDesc) ;
    HRESULT STDMETHODCALLTYPE LockRect( D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE UnlockRect() ;

	HRESULT STDMETHODCALLTYPE GetDC    (HDC *phdc) ;
	HRESULT STDMETHODCALLTYPE ReleaseDC(HDC hdc) ;


public:
	static   VOID*    s_pFunRelease   ;
	static   VOID*    s_pFunLockRect  ;
	static   VOID*    s_pFunUnlockRect;

	static  Filter_IDirect3DSurface9*   s_pFilter;

	static   VOID Init(IDirect3DSurface9*  pSurface);
};
#endif