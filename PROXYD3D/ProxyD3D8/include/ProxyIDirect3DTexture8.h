//************************************************************************************************************
// class      :  IGA_IDirect3DTexture8
// purpose    :  接管此类部分虚拟表
// Date       :  2008-7-30  10:26:29
// author     :  3o3afe1
//************************************************************************************************************

#ifndef _PROXY_IDIRECT3DTEXTURE8_H
#define _PROXY_IDIRECT3DTEXTURE8_H
#include <d3d8.h>

class Filter_IDirect3DTexture8;
class IGA_IDirect3DTexture8 : public IDirect3DTexture8
{
public:
	typedef ULONG   (WINAPI *LP_FUN_Release)(IDirect3DTexture8*);
	typedef HRESULT (WINAPI *LP_FUN_GetSurfaceLevel)(IDirect3DTexture8*,UINT,IDirect3DSurface8**);
	typedef HRESULT (WINAPI *LP_FUN_LockRect)(IDirect3DTexture8*,UINT,D3DLOCKED_RECT*,CONST RECT*,DWORD);
    typedef HRESULT (WINAPI *LP_FUN_UnLockRect)(IDirect3DTexture8*,UINT);	

private:
	enum
	{
        TEXTURE8_QUERYINTERFACE_INDEX  =  0,		
		TEXTURE8_ADDREF_INDEX          =  1,		
		TEXTURE8_RELEASE_INDEX         =  2,		
		TEXTURE8_GETDEVICE_INDEX       =  3,		
		TEXTURE8_SETPRIVATEDATA_INDEX  =  4,		
		TEXTURE8_GETPRIVATEDATA_INDEX  =  5,		
		TEXTURE8_FREEPRIVATEDATA_INDEX =  6,		
		TEXTURE8_SETPRIORITY_INDEX     =  7,		
		TEXTURE8_GETPRIORITY_INDEX     =  8,		
		TEXTURE8_PRELOAD_INDEX         =  9,		
		TEXTURE8_GETTYPE_INDEX         = 10,		
		TEXTURE8_SETLOD_INDEX          = 11,		
		TEXTURE8_GETLOD_INDEX          = 12,		
		TEXTURE8_GETLEVELCOUNT_INDEX   = 13,		
		TEXTURE8_GETLEVELDESC_INDEX    = 14,		
		TEXTURE8_GETSURFACELEVEL_INDEX = 15,		
		TEXTURE8_LOCKRECT_INDEX        = 16,		
		TEXTURE8_UNLOCKRECT_INDEX      = 17,		
		TEXTURE8_ADDDIRTYRECT_INDEX    = 18 
	};
public:
    /*** IUnknown methods ***/
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) ;
    ULONG   STDMETHODCALLTYPE AddRef() ;
    ULONG   STDMETHODCALLTYPE Release() ;
	
    /*** IDirect3DBaseTexture8 methods ***/
    HRESULT STDMETHODCALLTYPE GetDevice( IDirect3DDevice8** ppDevice) ;
    HRESULT STDMETHODCALLTYPE SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) ;
    HRESULT STDMETHODCALLTYPE FreePrivateData( REFGUID refguid) ;
    DWORD   STDMETHODCALLTYPE SetPriority( DWORD PriorityNew) ;
    DWORD   STDMETHODCALLTYPE GetPriority() ;
    void    STDMETHODCALLTYPE PreLoad() ;
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType() ;
    DWORD   STDMETHODCALLTYPE SetLOD( DWORD LODNew) ;
    DWORD   STDMETHODCALLTYPE GetLOD() ;
    DWORD   STDMETHODCALLTYPE GetLevelCount() ;
    HRESULT STDMETHODCALLTYPE GetLevelDesc( UINT Level,D3DSURFACE_DESC *pDesc) ;
    HRESULT STDMETHODCALLTYPE GetSurfaceLevel( UINT Level,IDirect3DSurface8** ppSurfaceLevel) ;
    HRESULT STDMETHODCALLTYPE LockRect( UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE UnlockRect( UINT Level) ;
    HRESULT STDMETHODCALLTYPE AddDirtyRect( CONST RECT* pDirtyRect) ;


public:
	static   VOID*    s_pFunRelease         ;
	static   VOID*    s_pFunGetSurfaceLevel ;
	static   VOID*    s_pFunLockRect        ;
	static   VOID*    s_pFunUnlockRect      ;

	static   Filter_IDirect3DTexture8*   s_pFilter;


	static   VOID Init(IDirect3DTexture8*  pTexture);

};

#endif.