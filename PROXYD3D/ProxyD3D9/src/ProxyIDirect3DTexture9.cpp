
#include "StdAfx.h"
#include "ProxyIDirect3DTexture9.h"
#include "ProxyIDirect3DSurface9.h"
#include "ProxyIDirect3DDevice9.h"
#include "..\D3D9Filter.h"

extern D3D_FILTER   g_d3d_filters;

VOID*    IGA_IDirect3DTexture9::s_pFunRelease          = NULL ;
VOID*    IGA_IDirect3DTexture9::s_pFunGetSurfaceLevel  = NULL ;
VOID*    IGA_IDirect3DTexture9::s_pFunLockRect         = NULL ;
VOID*    IGA_IDirect3DTexture9::s_pFunUnlockRect       = NULL ;
Filter_IDirect3DTexture9*   IGA_IDirect3DTexture9::s_pFilter = NULL ;

HRESULT IGA_IDirect3DTexture9::QueryInterface(REFIID riid, void** ppvObj)  {  return S_OK; }
ULONG   IGA_IDirect3DTexture9::AddRef()  { return 0;}
ULONG   IGA_IDirect3DTexture9::Release() 
{
	IDirect3DDevice9*  pDevice = NULL;
	GetDevice(&pDevice);	
	ULONG   ulRefCount =(*((LP_FUN_Release)s_pFunRelease))(this);			
	pDevice->Release();

 	pDevice     = IGAIDirect3DDevice9::GetDevice(pDevice);
 	if  ( g_d3d_filters.pFilterTexture != NULL && pDevice != NULL)
 		g_d3d_filters.pFilterTexture->OnRelease_After(pDevice,this,ulRefCount);

	
	return ulRefCount;	
}

HRESULT IGA_IDirect3DTexture9::GetDevice( IDirect3DDevice9** ppDevice){ return S_OK;}
HRESULT IGA_IDirect3DTexture9::SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags){return S_OK;}
HRESULT IGA_IDirect3DTexture9::GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData){return S_OK;}
HRESULT IGA_IDirect3DTexture9::FreePrivateData( REFGUID refguid){return S_OK;}
DWORD   IGA_IDirect3DTexture9::SetPriority( DWORD PriorityNew){return 0;}
DWORD   IGA_IDirect3DTexture9::GetPriority(){return 0;}
void    IGA_IDirect3DTexture9::PreLoad(){}
D3DRESOURCETYPE IGA_IDirect3DTexture9::GetType(){return D3DRTYPE_SURFACE; }
DWORD   IGA_IDirect3DTexture9::SetLOD( DWORD LODNew){ return 0;}
DWORD   IGA_IDirect3DTexture9::GetLOD(){ return 0;}
DWORD   IGA_IDirect3DTexture9::GetLevelCount(){ return 0;}
HRESULT IGA_IDirect3DTexture9::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType) {return 0;}
D3DTEXTUREFILTERTYPE IGA_IDirect3DTexture9::GetAutoGenFilterType(){return D3DTEXF_NONE;}
void    IGA_IDirect3DTexture9::GenerateMipSubLevels(){return ;}

HRESULT IGA_IDirect3DTexture9::GetLevelDesc( UINT Level,D3DSURFACE_DESC *pDesc){return 0;}

HRESULT IGA_IDirect3DTexture9::GetSurfaceLevel( UINT Level,IDirect3DSurface9** ppSurfaceLevel)
{
	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnGetSurfaceLevel_Before(this,Level, ppSurfaceLevel);	
	
	HRESULT  hr =(*((LP_FUN_GetSurfaceLevel)s_pFunGetSurfaceLevel))(this,Level,ppSurfaceLevel);		
	
	static  BOOL  s_bReplace = FALSE;
	if  ( hr == S_OK )
	{	
		if  ( !s_bReplace )
		{
			IGA_IDirect3DSurface9::Init(*ppSurfaceLevel);
			s_bReplace = TRUE;
		}
	}

	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnGetSurfaceLevel_After (this,Level, ppSurfaceLevel);
	
	return hr;	

}

HRESULT IGA_IDirect3DTexture9::LockRect( UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnLockRect_Before(this,Level,pLockedRect, pRect,Flags);


	HRESULT  hr =(*((LP_FUN_LockRect)s_pFunLockRect))(this,Level,pLockedRect, pRect,Flags);		

	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnLockRect_After (this,Level,pLockedRect, pRect,Flags);

	return hr;
}
HRESULT IGA_IDirect3DTexture9::UnlockRect( UINT Level)
{ 
	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnUnlockRect_Before(this,Level);
	
	
	HRESULT  hr =(*((LP_FUN_UnLockRect)s_pFunUnlockRect))(this,Level);		
	
	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnUnlockRect_After (this,Level);
	
	return hr;
}


HRESULT IGA_IDirect3DTexture9::AddDirtyRect( CONST RECT* pDirtyRect){return 0;}

/*初始化虚函数表*/
VOID IGA_IDirect3DTexture9::Init(IDirect3DTexture9*  pTexture)
{
    DWORD  dwNewFunAddr  = 0;
	DWORD  dwOldFunAddr  = 0;

	IGA_IDirect3DTexture9     newTexture;

	//release
    dwNewFunAddr          = GetOneVirtualFunAddr(TEXTURE9_RELEASE_INDEX,&newTexture);
	s_pFunRelease         = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,TEXTURE9_RELEASE_INDEX ,pTexture);

	//GetSurfaceLevel
    dwNewFunAddr          = GetOneVirtualFunAddr(TEXTURE9_GETSURFACELEVEL_INDEX,&newTexture);
	s_pFunGetSurfaceLevel = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,TEXTURE9_GETSURFACELEVEL_INDEX ,pTexture);

	//LockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(TEXTURE9_LOCKRECT_INDEX,&newTexture);
	s_pFunLockRect        = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,TEXTURE9_LOCKRECT_INDEX ,pTexture);

	//UnlockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(TEXTURE9_UNLOCKRECT_INDEX,&newTexture);
	s_pFunUnlockRect      = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,TEXTURE9_UNLOCKRECT_INDEX ,pTexture);
}