#include "stdafx.h"
#include "ProxyIDirect3DSurface8.h"
#include "ProxyIDirect3DDevice8.h"
#include "..\D3D8Filter.h"

VOID*    IGA_IDirect3DSurface8::s_pFunRelease   = NULL ;
VOID*    IGA_IDirect3DSurface8::s_pFunLockRect  = NULL ;
VOID*    IGA_IDirect3DSurface8::s_pFunUnlockRect= NULL ;
Filter_IDirect3DSurface8*   IGA_IDirect3DSurface8::s_pFilter = NULL;
extern D3D_FILTER   g_d3d_filters;

/*** IUnknown methods ***/
HRESULT IGA_IDirect3DSurface8::QueryInterface(REFIID riid, void** ppvObj) { return 0 ;}
ULONG   IGA_IDirect3DSurface8::AddRef() { return 0 ;}
ULONG   IGA_IDirect3DSurface8::Release() 
{
	IDirect3DDevice8*  pDevice = NULL;
	GetDevice(&pDevice);	
	ULONG   ulRefCount =(*((LP_FUN_Release)s_pFunRelease))(this);		
	pDevice->Release();
	
	pDevice     = IGAIDirect3DDevice8::GetDevice(pDevice);
	if  ( g_d3d_filters.pFilterSurface  != NULL && pDevice != NULL)
		g_d3d_filters.pFilterSurface->OnRelease_After(pDevice,this,ulRefCount);
	
	
	return ulRefCount;
}

/*** IDirect3DSurface8 methods ***/
HRESULT IGA_IDirect3DSurface8::GetDevice( IDirect3DDevice8** ppDevice) { return 0 ;}
HRESULT IGA_IDirect3DSurface8::SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) { return 0 ;}
HRESULT IGA_IDirect3DSurface8::GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) { return 0 ;}
HRESULT IGA_IDirect3DSurface8::FreePrivateData( REFGUID refguid) { return 0 ;}
HRESULT IGA_IDirect3DSurface8::GetContainer( REFIID riid,void** ppContainer) { return 0 ;}
HRESULT IGA_IDirect3DSurface8::GetDesc( D3DSURFACE_DESC *pDesc) { return 0 ;}
HRESULT IGA_IDirect3DSurface8::LockRect( D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) 
{
	if  ( g_d3d_filters.pFilterSurface != NULL)
		g_d3d_filters.pFilterSurface->OnLockRect_Before(this,pLockedRect, pRect,Flags);
	
	
	HRESULT  hr =(*((LP_FUN_LockRect)s_pFunLockRect))(this,pLockedRect, pRect,Flags);		
	
	if  ( g_d3d_filters.pFilterSurface != NULL)
		g_d3d_filters.pFilterSurface->OnLockRect_After (this,pLockedRect, pRect,Flags);
	
	return hr;
}
HRESULT IGA_IDirect3DSurface8::UnlockRect() 
{

	IDirect3DDevice8*  pDevice = NULL;
	GetDevice(&pDevice);		
	pDevice->Release();
	pDevice     = IGAIDirect3DDevice8::GetDevice(pDevice);
	if  ( g_d3d_filters.pFilterSurface != NULL)
		g_d3d_filters.pFilterSurface->OnUnlockRect_Before(pDevice,this);	
	

	HRESULT  hr =(*((LP_FUN_UnLockRect)s_pFunUnlockRect))(this);		
	
	
	if  ( g_d3d_filters.pFilterSurface != NULL)
		g_d3d_filters.pFilterSurface->OnUnlockRect_After (this);

	
	
	return hr;

	return 0 ;
}


VOID IGA_IDirect3DSurface8::Init(IDirect3DSurface8*  pSurface)
{
	DWORD  dwNewFunAddr  = 0;
	DWORD  dwOldFunAddr  = 0;
	
	IGA_IDirect3DSurface8     newSurface;
	
	//release
    dwNewFunAddr          = GetOneVirtualFunAddr(SURFACE8_RELEASE_INDEX,&newSurface);
	s_pFunRelease         = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,SURFACE8_RELEASE_INDEX ,pSurface);
	
	
	//LockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(SURFACE8_LOCKRECT_INDEX,&newSurface);
	s_pFunLockRect        = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,SURFACE8_LOCKRECT_INDEX ,pSurface);
	
	//UnlockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(SURFACE8_UNLOCKRECT_INDEX,&newSurface);
	s_pFunUnlockRect      = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,SURFACE8_UNLOCKRECT_INDEX ,pSurface);
}
