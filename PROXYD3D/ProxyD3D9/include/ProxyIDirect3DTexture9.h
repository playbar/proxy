//************************************************************************************************************
// class      :  IGA_IDirect3DTexture9
// purpose    :  接管此类部分虚拟表
// Date       :  2009-7-30  10:26:29
// author     :  3o3afe1
//************************************************************************************************************

#ifndef _PROXY_IDIRECT3DTEXTURE9_H
#define _PROXY_IDIRECT3DTEXTURE9_H
#include <d3d9.h>

class Filter_IDirect3DTexture9;
class IGA_IDirect3DTexture9 : public IDirect3DTexture9
{
public:
	typedef ULONG   (WINAPI *LP_FUN_Release)(IDirect3DTexture9*);
	typedef HRESULT (WINAPI *LP_FUN_GetSurfaceLevel)(IDirect3DTexture9*,UINT,IDirect3DSurface9**);
	typedef HRESULT (WINAPI *LP_FUN_LockRect)(IDirect3DTexture9*,UINT,D3DLOCKED_RECT*,CONST RECT*,DWORD);
    typedef HRESULT (WINAPI *LP_FUN_UnLockRect)(IDirect3DTexture9*,UINT);	

private:
	enum
	{
        TEXTURE9_QUERYINTERFACE_INDEX  =  0,		
		TEXTURE9_ADDREF_INDEX          =  1,		
		TEXTURE9_RELEASE_INDEX         =  2,		
		TEXTURE9_GETDEVICE_INDEX       =  3,		
		TEXTURE9_SETPRIVATEDATA_INDEX  =  4,		
		TEXTURE9_GETPRIVATEDATA_INDEX  =  5,		
		TEXTURE9_FREEPRIVATEDATA_INDEX =  6,		
		TEXTURE9_SETPRIORITY_INDEX     =  7,		
		TEXTURE9_GETPRIORITY_INDEX     =  8,		
		TEXTURE9_PRELOAD_INDEX         =  9,		
		TEXTURE9_GETTYPE_INDEX         = 10,		
		TEXTURE9_SETLOD_INDEX          = 11,		
		TEXTURE9_GETLOD_INDEX          = 12,		
		TEXTURE9_GETLEVELCOUNT_INDEX   = 13,
		
		TEXTURE9_SETAUTOGENFILTERTYPE_INDEX   = 14,
		TEXTURE9_GETAUTOGENFILTERTYPE_INDEX   = 15,
	    TEXTURE9_GENERATEMIPSUBLEVELS_INDEX   = 16,



		TEXTURE9_GETLEVELDESC_INDEX    = 17,		
		TEXTURE9_GETSURFACELEVEL_INDEX = 18,		
		TEXTURE9_LOCKRECT_INDEX        = 19,		
		TEXTURE9_UNLOCKRECT_INDEX      = 20,		
		TEXTURE9_ADDDIRTYRECT_INDEX    = 21 
	};
public:
    /*** IUnknown methods ***/
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) ;
    ULONG   STDMETHODCALLTYPE AddRef() ;
    ULONG   STDMETHODCALLTYPE Release() ;
	
    /*** IDirect3DBaseTexture9 methods ***/
    HRESULT STDMETHODCALLTYPE GetDevice( IDirect3DDevice9** ppDevice) ;
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
	
	HRESULT STDMETHODCALLTYPE SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType) ;
	D3DTEXTUREFILTERTYPE STDMETHODCALLTYPE GetAutoGenFilterType();
	void    STDMETHODCALLTYPE GenerateMipSubLevels();


    HRESULT STDMETHODCALLTYPE GetLevelDesc( UINT Level,D3DSURFACE_DESC *pDesc) ;
    HRESULT STDMETHODCALLTYPE GetSurfaceLevel( UINT Level,IDirect3DSurface9** ppSurfaceLevel) ;
    HRESULT STDMETHODCALLTYPE LockRect( UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE UnlockRect( UINT Level) ;
    HRESULT STDMETHODCALLTYPE AddDirtyRect( CONST RECT* pDirtyRect) ;


public:
	static   VOID*    s_pFunRelease         ;
	static   VOID*    s_pFunGetSurfaceLevel ;
	static   VOID*    s_pFunLockRect        ;
	static   VOID*    s_pFunUnlockRect      ;

	static   Filter_IDirect3DTexture9*   s_pFilter;


	static   VOID Init(IDirect3DTexture9*  pTexture);

};

#endif

