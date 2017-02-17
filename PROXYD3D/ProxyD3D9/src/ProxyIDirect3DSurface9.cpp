#include "stdafx.h"
#include "ProxyIDirect3DSurface9.h"
#include "ProxyIDirect3DDevice9.h"
#include "..\D3D9Filter.h"

VOID*    IGA_IDirect3DSurface9::s_pFunRelease   = NULL ;
VOID*    IGA_IDirect3DSurface9::s_pFunLockRect  = NULL ;
VOID*    IGA_IDirect3DSurface9::s_pFunUnlockRect= NULL ;
Filter_IDirect3DSurface9*   IGA_IDirect3DSurface9::s_pFilter = NULL;
extern D3D_FILTER   g_d3d_filters;

/*** IUnknown methods ***/
HRESULT IGA_IDirect3DSurface9::QueryInterface(REFIID riid, void** ppvObj) { return 0 ;}
ULONG   IGA_IDirect3DSurface9::AddRef() { return 0 ;}
ULONG   IGA_IDirect3DSurface9::Release() 
{
	IDirect3DDevice9*  pDevice = NULL;
	GetDevice(&pDevice);	
	ULONG   ulRefCount =(*((LP_FUN_Release)s_pFunRelease))(this);		
	pDevice->Release();
	
	pDevice     = IGAIDirect3DDevice9::GetDevice(pDevice);
	if  ( g_d3d_filters.pFilterSurface  != NULL && pDevice != NULL)
		g_d3d_filters.pFilterSurface->OnRelease_After(pDevice,this,ulRefCount);
	
	
	return ulRefCount;
}

/*** IDirect3DSurface9 methods ***/
HRESULT IGA_IDirect3DSurface9::GetDevice( IDirect3DDevice9** ppDevice) { return 0 ;}
HRESULT IGA_IDirect3DSurface9::SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) { return 0 ;}
HRESULT IGA_IDirect3DSurface9::GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) { return 0 ;}
HRESULT IGA_IDirect3DSurface9::FreePrivateData( REFGUID refguid) { return 0 ;}
HRESULT IGA_IDirect3DSurface9::GetContainer( REFIID riid,void** ppContainer) { return 0 ;}
HRESULT IGA_IDirect3DSurface9::GetDesc( D3DSURFACE_DESC *pDesc) { return 0 ;}


DWORD   IGA_IDirect3DSurface9::SetPriority (DWORD PriorityNew) {return 0;}
DWORD   IGA_IDirect3DSurface9::GetPriority(){return 0;}
void    IGA_IDirect3DSurface9:: PreLoad(){ }
D3DRESOURCETYPE IGA_IDirect3DSurface9::GetType(){return D3DRTYPE_SURFACE;}
HRESULT IGA_IDirect3DSurface9::GetDC    (HDC *phdc){return 0;}
HRESULT IGA_IDirect3DSurface9::ReleaseDC(HDC hdc) {return 0;}



HRESULT IGA_IDirect3DSurface9::LockRect( D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) 
{
	if  ( g_d3d_filters.pFilterSurface != NULL)
		g_d3d_filters.pFilterSurface->OnLockRect_Before(this,pLockedRect, pRect,Flags);
	
	
	HRESULT  hr =(*((LP_FUN_LockRect)s_pFunLockRect))(this,pLockedRect, pRect,Flags);		
	
	if  ( g_d3d_filters.pFilterSurface != NULL)
		g_d3d_filters.pFilterSurface->OnLockRect_After (this,pLockedRect, pRect,Flags);
	
	return hr;
}
HRESULT IGA_IDirect3DSurface9::UnlockRect() 
{

	IDirect3DDevice9*  pDevice = NULL;
	GetDevice(&pDevice);		
	pDevice->Release();
	pDevice     = IGAIDirect3DDevice9::GetDevice(pDevice);
	if  ( g_d3d_filters.pFilterSurface != NULL)
		g_d3d_filters.pFilterSurface->OnUnlockRect_Before(pDevice,this);	
	

	HRESULT  hr =(*((LP_FUN_UnLockRect)s_pFunUnlockRect))(this);		
	
	
	if  ( g_d3d_filters.pFilterSurface != NULL)
		g_d3d_filters.pFilterSurface->OnUnlockRect_After (this);

	
	
	return hr;

	return 0 ;
}


VOID IGA_IDirect3DSurface9::Init(IDirect3DSurface9*  pSurface)
{
	DWORD  dwNewFunAddr  = 0;
	DWORD  dwOldFunAddr  = 0;
	
	IGA_IDirect3DSurface9     newSurface;
	
	//release
    dwNewFunAddr          = GetOneVirtualFunAddr(SURFACE9_RELEASE_INDEX,&newSurface);
	s_pFunRelease         = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,SURFACE9_RELEASE_INDEX ,pSurface);
	
	
	//LockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(SURFACE9_LOCKRECT_INDEX,&newSurface);
	s_pFunLockRect        = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,SURFACE9_LOCKRECT_INDEX ,pSurface);
	
	//UnlockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(SURFACE9_UNLOCKRECT_INDEX,&newSurface);
	s_pFunUnlockRect      = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,SURFACE9_UNLOCKRECT_INDEX ,pSurface);
}
