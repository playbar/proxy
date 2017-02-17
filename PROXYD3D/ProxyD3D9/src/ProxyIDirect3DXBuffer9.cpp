#include "StdAfx.h"
//#include "ProxyIDirect3DXBuffer9.h"

/*********************************************************************************************************/
/*                        IDirect3DVertexBuffer9                                                         */
/*********************************************************************************************************/
VOID*    IGA_IDirect3DVertexBuffer9::s_pFunRelease    = NULL ;
VOID*    IGA_IDirect3DVertexBuffer9::s_pFunLockRect   = NULL ;
VOID*    IGA_IDirect3DVertexBuffer9::s_pFunUnlockRect = NULL ;
Filter_IDirect3DVertexBuffer9*   IGA_IDirect3DVertexBuffer9::s_pFilter = NULL ;
	 
HRESULT IGA_IDirect3DVertexBuffer9::QueryInterface(REFIID riid, void** ppvObj) {return 0 ;}
ULONG   IGA_IDirect3DVertexBuffer9::AddRef() {return 0 ;}
ULONG   IGA_IDirect3DVertexBuffer9::Release() {return 0 ;}

/*** IDirect3DResource9 methods ***/
HRESULT IGA_IDirect3DVertexBuffer9::GetDevice( IDirect3DDevice9** ppDevice) {return 0 ;}
HRESULT IGA_IDirect3DVertexBuffer9::SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) {return 0 ;}
HRESULT IGA_IDirect3DVertexBuffer9::GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) {return 0 ;}
HRESULT IGA_IDirect3DVertexBuffer9::FreePrivateData( REFGUID refguid) {return 0 ;}
DWORD   IGA_IDirect3DVertexBuffer9::SetPriority( DWORD PriorityNew) {return 0 ;}
DWORD   IGA_IDirect3DVertexBuffer9:: GetPriority() {return 0 ;}
void    IGA_IDirect3DVertexBuffer9::PreLoad() {return  ;}
D3DRESOURCETYPE IGA_IDirect3DVertexBuffer9::GetType() {return D3DRTYPE_SURFACE ;}
HRESULT IGA_IDirect3DVertexBuffer9::Lock( UINT OffsetToLock,UINT SizeToLock,BYTE** ppbData,DWORD Flags) {return 0 ;}
HRESULT IGA_IDirect3DVertexBuffer9::Unlock() {return 0 ;}
HRESULT IGA_IDirect3DVertexBuffer9::GetDesc( D3DVERTEXBUFFER_DESC *pDesc) {return 0 ;}

VOID IGA_IDirect3DVertexBuffer9::Init(IDirect3DVertexBuffer9*  pVertexBuffer9)
{
    DWORD  dwNewFunAddr  = 0;
	DWORD  dwOldFunAddr  = 0;
	
	IGA_IDirect3DVertexBuffer9     newVertexBuffer9;
	
	//release
    dwNewFunAddr          = GetOneVirtualFunAddr(VERTEXBUFFER9_RELEASE_INDEX,&newVertexBuffer9);
	s_pFunRelease         = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,VERTEXBUFFER9_RELEASE_INDEX ,pVertexBuffer9);	
	
	//LockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(VERTEXBUFFER9_LOCK_INDEX,&newVertexBuffer9);
	s_pFunLockRect        = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,VERTEXBUFFER9_LOCK_INDEX ,pVertexBuffer9);
	
	//UnlockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(VERTEXBUFFER9_UNLOCK_INDEX,&newVertexBuffer9);
	s_pFunUnlockRect      = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,VERTEXBUFFER9_UNLOCK_INDEX ,pVertexBuffer9);
}
/*********************************************************************************************************/
/*            IDirect3DIndexBuffer9                                                                      */
/*********************************************************************************************************/

VOID*    IGA_IDirect3DIndexBuffer9::s_pFunRelease    = NULL ;
VOID*    IGA_IDirect3DIndexBuffer9::s_pFunLockRect   = NULL ;
VOID*    IGA_IDirect3DIndexBuffer9::s_pFunUnlockRect = NULL ;

Filter_IDirect3DIndexBuffer9*   IGA_IDirect3DIndexBuffer9::s_pFilter = NULL ;

	
HRESULT IGA_IDirect3DIndexBuffer9::QueryInterface(REFIID riid, void** ppvObj) 
{
    return 0 ;
}
ULONG   IGA_IDirect3DIndexBuffer9::AddRef() 
{
    return 0 ;
}
ULONG   IGA_IDirect3DIndexBuffer9::Release() 
{
    return 0 ;
}

/*** IDirect3DResource9 methods ***/
HRESULT IGA_IDirect3DIndexBuffer9::GetDevice( IDirect3DDevice9** ppDevice) 
{
    return 0 ;
}
HRESULT IGA_IDirect3DIndexBuffer9::SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) 
{
    return 0 ;
}
HRESULT IGA_IDirect3DIndexBuffer9::GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) 
{
    return 0 ;
}
HRESULT IGA_IDirect3DIndexBuffer9::FreePrivateData( REFGUID refguid) 
{
    return 0 ;
}
DWORD   IGA_IDirect3DIndexBuffer9::SetPriority( DWORD PriorityNew) 
{
    return 0 ;
}
DWORD   IGA_IDirect3DIndexBuffer9::GetPriority() 
{
    return 0 ;
}
void    IGA_IDirect3DIndexBuffer9::PreLoad() 
{
    return  ;
}
D3DRESOURCETYPE IGA_IDirect3DIndexBuffer9::GetType() 
{
    return D3DRTYPE_SURFACE;
}
HRESULT IGA_IDirect3DIndexBuffer9::Lock( UINT OffsetToLock,UINT SizeToLock,BYTE** ppbData,DWORD Flags) 
{
    return 0 ;
}
HRESULT IGA_IDirect3DIndexBuffer9::Unlock() 
{
    return 0 ;
}
HRESULT IGA_IDirect3DIndexBuffer9::GetDesc( D3DINDEXBUFFER_DESC *pDesc) 
{
    return 0 ;
}

VOID IGA_IDirect3DIndexBuffer9::Init(IDirect3DIndexBuffer9*  pIndexBuffer9)
{
    DWORD  dwNewFunAddr  = 0;
	DWORD  dwOldFunAddr  = 0;
	
	IGA_IDirect3DIndexBuffer9     newIndexBuffer9;
	
	//release
    dwNewFunAddr          = GetOneVirtualFunAddr(INDEXBUFFER9_RELEASE_INDEX,&newIndexBuffer9);
	s_pFunRelease         = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,INDEXBUFFER9_RELEASE_INDEX ,pIndexBuffer9);	
	
	//LockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(INDEXBUFFER9_LOCK_INDEX,&newIndexBuffer9);
	s_pFunLockRect        = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,INDEXBUFFER9_LOCK_INDEX ,pIndexBuffer9);
	
	//UnlockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(INDEXBUFFER9_UNLOCK_INDEX,&newIndexBuffer9);
	s_pFunUnlockRect      = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,INDEXBUFFER9_UNLOCK_INDEX ,pIndexBuffer9);
}
