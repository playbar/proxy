#ifndef _PROXY_IDIRECT3DSURFACE_H
#define _PROXY_IDIRECT3DSURFACE_H
#include <d3d8.h>
class Filter_IDirect3DSurface8;

class IGA_IDirect3DSurface8 : public IDirect3DSurface8
{  
public:
	typedef ULONG (WINAPI *LP_FUN_Release)    (IDirect3DSurface8*);
    typedef HRESULT (WINAPI *LP_FUN_LockRect) (IDirect3DSurface8*,D3DLOCKED_RECT*,CONST RECT*,DWORD);
    typedef HRESULT (WINAPI *LP_FUN_UnLockRect)(IDirect3DSurface8*);

private:
	enum
	{
            SURFACE8_QUERYINTERFACE_INDEX  =  0,		
			SURFACE8_ADDREF_INDEX          =  1,		
			SURFACE8_RELEASE_INDEX         =  2,		
			SURFACE8_GETDEVICE_INDEX       =  3,		
			SURFACE8_SETPRIVATEDATA_INDEX  =  4,		
			SURFACE8_GETPRIVATEDATA_INDEX  =  5,		
			SURFACE8_FREEPRIVATEDATA_INDEX =  6,		
			SURFACE8_GETCONTAINER_INDEX    =  7,		
			SURFACE8_GETDESC_INDEX         =  8,		
			SURFACE8_LOCKRECT_INDEX        =  9,		
			SURFACE8_UNLOCKRECT_INDEX      = 10,					
	};
public:
	/*** IUnknown methods ***/
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) ;
    ULONG   STDMETHODCALLTYPE AddRef() ;
    ULONG   STDMETHODCALLTYPE Release() ;
	
    /*** IDirect3DSurface8 methods ***/
    HRESULT STDMETHODCALLTYPE GetDevice( IDirect3DDevice8** ppDevice) ;
    HRESULT STDMETHODCALLTYPE SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) ;
    HRESULT STDMETHODCALLTYPE FreePrivateData( REFGUID refguid) ;
    HRESULT STDMETHODCALLTYPE GetContainer( REFIID riid,void** ppContainer) ;
    HRESULT STDMETHODCALLTYPE GetDesc( D3DSURFACE_DESC *pDesc) ;
    HRESULT STDMETHODCALLTYPE LockRect( D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE UnlockRect() ;


public:
	static   VOID*    s_pFunRelease   ;
	static   VOID*    s_pFunLockRect  ;
	static   VOID*    s_pFunUnlockRect;

	static  Filter_IDirect3DSurface8*   s_pFilter;

	static   VOID Init(IDirect3DSurface8*  pSurface);
};
#endif