
#include "StdAfx.h"
#include "ProxyIDirect3DTexture8.h"
#include "ProxyIDirect3DSurface8.h"
#include "ProxyIDirect3DDevice8.h"
#include "..\D3D8Filter.h"

extern D3D_FILTER   g_d3d_filters;

VOID*    IGA_IDirect3DTexture8::s_pFunRelease          = NULL ;
VOID*    IGA_IDirect3DTexture8::s_pFunGetSurfaceLevel  = NULL ;
VOID*    IGA_IDirect3DTexture8::s_pFunLockRect         = NULL ;
VOID*    IGA_IDirect3DTexture8::s_pFunUnlockRect       = NULL ;
Filter_IDirect3DTexture8*   IGA_IDirect3DTexture8::s_pFilter = NULL ;

HRESULT IGA_IDirect3DTexture8::QueryInterface(REFIID riid, void** ppvObj)  {  return S_OK; }
ULONG   IGA_IDirect3DTexture8::AddRef()  { return 0;}
ULONG   IGA_IDirect3DTexture8::Release() 
{
	IDirect3DDevice8*  pDevice = NULL;
	GetDevice(&pDevice);	
	ULONG   ulRefCount =(*((LP_FUN_Release)s_pFunRelease))(this);			
	pDevice->Release();

 	pDevice     = IGAIDirect3DDevice8::GetDevice(pDevice);
 	if  ( g_d3d_filters.pFilterTexture != NULL && pDevice != NULL)
 		g_d3d_filters.pFilterTexture->OnRelease_After(pDevice,this,ulRefCount);

	
	return ulRefCount;	
}

HRESULT IGA_IDirect3DTexture8::GetDevice( IDirect3DDevice8** ppDevice){ return S_OK;}
HRESULT IGA_IDirect3DTexture8::SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags){return S_OK;}
HRESULT IGA_IDirect3DTexture8::GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData){return S_OK;}
HRESULT IGA_IDirect3DTexture8::FreePrivateData( REFGUID refguid){return S_OK;}
DWORD   IGA_IDirect3DTexture8::SetPriority( DWORD PriorityNew){return 0;}
DWORD   IGA_IDirect3DTexture8::GetPriority(){return 0;}
void    IGA_IDirect3DTexture8::PreLoad(){}
D3DRESOURCETYPE IGA_IDirect3DTexture8::GetType(){return D3DRTYPE_SURFACE; }
DWORD   IGA_IDirect3DTexture8::SetLOD( DWORD LODNew){ return 0;}
DWORD   IGA_IDirect3DTexture8::GetLOD(){ return 0;}
DWORD   IGA_IDirect3DTexture8::GetLevelCount(){ return 0;}
HRESULT IGA_IDirect3DTexture8::GetLevelDesc( UINT Level,D3DSURFACE_DESC *pDesc){return 0;}

HRESULT IGA_IDirect3DTexture8::GetSurfaceLevel( UINT Level,IDirect3DSurface8** ppSurfaceLevel)
{
	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnGetSurfaceLevel_Before(this,Level, ppSurfaceLevel);	
	
	HRESULT  hr =(*((LP_FUN_GetSurfaceLevel)s_pFunGetSurfaceLevel))(this,Level,ppSurfaceLevel);		
	
	static  BOOL  s_bReplace = FALSE;
	if  ( hr == S_OK )
	{	
		if  ( !s_bReplace )
		{
			IGA_IDirect3DSurface8::Init(*ppSurfaceLevel);
			s_bReplace = TRUE;
		}
	}

	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnGetSurfaceLevel_After (this,Level, ppSurfaceLevel);
	
	return hr;	

}

HRESULT IGA_IDirect3DTexture8::LockRect( UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnLockRect_Before(this,Level,pLockedRect, pRect,Flags);


	HRESULT  hr =(*((LP_FUN_LockRect)s_pFunLockRect))(this,Level,pLockedRect, pRect,Flags);		

	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnLockRect_After (this,Level,pLockedRect, pRect,Flags);

	return hr;
}
HRESULT IGA_IDirect3DTexture8::UnlockRect( UINT Level)
{ 
	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnUnlockRect_Before(this,Level);
	
	
	HRESULT  hr =(*((LP_FUN_UnLockRect)s_pFunUnlockRect))(this,Level);		
	
	if  ( g_d3d_filters.pFilterTexture != NULL)
		g_d3d_filters.pFilterTexture->OnUnlockRect_After (this,Level);
	
	return hr;
}


HRESULT IGA_IDirect3DTexture8::AddDirtyRect( CONST RECT* pDirtyRect){return 0;}

/*初始化虚函数表*/
VOID IGA_IDirect3DTexture8::Init(IDirect3DTexture8*  pTexture)
{
    DWORD  dwNewFunAddr  = 0;
	DWORD  dwOldFunAddr  = 0;

	IGA_IDirect3DTexture8     newTexture;

	//release
    dwNewFunAddr          = GetOneVirtualFunAddr(TEXTURE8_RELEASE_INDEX,&newTexture);
	s_pFunRelease         = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,TEXTURE8_RELEASE_INDEX ,pTexture);

	//GetSurfaceLevel
    dwNewFunAddr          = GetOneVirtualFunAddr(TEXTURE8_GETSURFACELEVEL_INDEX,&newTexture);
	s_pFunGetSurfaceLevel = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,TEXTURE8_GETSURFACELEVEL_INDEX ,pTexture);

	//LockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(TEXTURE8_LOCKRECT_INDEX,&newTexture);
	s_pFunLockRect        = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,TEXTURE8_LOCKRECT_INDEX ,pTexture);

	//UnlockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(TEXTURE8_UNLOCKRECT_INDEX,&newTexture);
	s_pFunUnlockRect      = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,TEXTURE8_UNLOCKRECT_INDEX ,pTexture);
}