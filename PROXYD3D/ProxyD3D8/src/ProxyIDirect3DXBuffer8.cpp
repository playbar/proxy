#include "StdAfx.h"
#include "ProxyIDirect3DXBuffer8.h"

/*********************************************************************************************************/
/*                        IDirect3DVertexBuffer8                                                         */
/*********************************************************************************************************/
VOID*    IGA_IDirect3DVertexBuffer8::s_pFunRelease    = NULL ;
VOID*    IGA_IDirect3DVertexBuffer8::s_pFunLockRect   = NULL ;
VOID*    IGA_IDirect3DVertexBuffer8::s_pFunUnlockRect = NULL ;
Filter_IDirect3DVertexBuffer8*   IGA_IDirect3DVertexBuffer8::s_pFilter = NULL ;
	 
HRESULT IGA_IDirect3DVertexBuffer8::QueryInterface(REFIID riid, void** ppvObj) {return 0 ;}
ULONG   IGA_IDirect3DVertexBuffer8::AddRef() {return 0 ;}
ULONG   IGA_IDirect3DVertexBuffer8::Release() {return 0 ;}

/*** IDirect3DResource8 methods ***/
HRESULT IGA_IDirect3DVertexBuffer8::GetDevice( IDirect3DDevice8** ppDevice) {return 0 ;}
HRESULT IGA_IDirect3DVertexBuffer8::SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) {return 0 ;}
HRESULT IGA_IDirect3DVertexBuffer8::GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) {return 0 ;}
HRESULT IGA_IDirect3DVertexBuffer8::FreePrivateData( REFGUID refguid) {return 0 ;}
DWORD   IGA_IDirect3DVertexBuffer8::SetPriority( DWORD PriorityNew) {return 0 ;}
DWORD   IGA_IDirect3DVertexBuffer8:: GetPriority() {return 0 ;}
void    IGA_IDirect3DVertexBuffer8::PreLoad() {return  ;}
D3DRESOURCETYPE IGA_IDirect3DVertexBuffer8::GetType() {return D3DRTYPE_SURFACE ;}
HRESULT IGA_IDirect3DVertexBuffer8::Lock( UINT OffsetToLock,UINT SizeToLock,BYTE** ppbData,DWORD Flags) {return 0 ;}
HRESULT IGA_IDirect3DVertexBuffer8::Unlock() {return 0 ;}
HRESULT IGA_IDirect3DVertexBuffer8::GetDesc( D3DVERTEXBUFFER_DESC *pDesc) {return 0 ;}

VOID IGA_IDirect3DVertexBuffer8::Init(IDirect3DVertexBuffer8*  pVertexBuffer8)
{
    DWORD  dwNewFunAddr  = 0;
	DWORD  dwOldFunAddr  = 0;
	
	IGA_IDirect3DVertexBuffer8     newVertexBuffer8;
	
	//release
    dwNewFunAddr          = GetOneVirtualFunAddr(VERTEXBUFFER8_RELEASE_INDEX,&newVertexBuffer8);
	s_pFunRelease         = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,VERTEXBUFFER8_RELEASE_INDEX ,pVertexBuffer8);	
	
	//LockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(VERTEXBUFFER8_LOCK_INDEX,&newVertexBuffer8);
	s_pFunLockRect        = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,VERTEXBUFFER8_LOCK_INDEX ,pVertexBuffer8);
	
	//UnlockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(VERTEXBUFFER8_UNLOCK_INDEX,&newVertexBuffer8);
	s_pFunUnlockRect      = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,VERTEXBUFFER8_UNLOCK_INDEX ,pVertexBuffer8);
}
/*********************************************************************************************************/
/*            IDirect3DIndexBuffer8                                                                      */
/*********************************************************************************************************/

VOID*    IGA_IDirect3DIndexBuffer8::s_pFunRelease    = NULL ;
VOID*    IGA_IDirect3DIndexBuffer8::s_pFunLockRect   = NULL ;
VOID*    IGA_IDirect3DIndexBuffer8::s_pFunUnlockRect = NULL ;

Filter_IDirect3DIndexBuffer8*   IGA_IDirect3DIndexBuffer8::s_pFilter = NULL ;

	
HRESULT IGA_IDirect3DIndexBuffer8::QueryInterface(REFIID riid, void** ppvObj) 
{
    return 0 ;
}
ULONG   IGA_IDirect3DIndexBuffer8::AddRef() 
{
    return 0 ;
}
ULONG   IGA_IDirect3DIndexBuffer8::Release() 
{
    return 0 ;
}

/*** IDirect3DResource8 methods ***/
HRESULT IGA_IDirect3DIndexBuffer8::GetDevice( IDirect3DDevice8** ppDevice) 
{
    return 0 ;
}
HRESULT IGA_IDirect3DIndexBuffer8::SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) 
{
    return 0 ;
}
HRESULT IGA_IDirect3DIndexBuffer8::GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) 
{
    return 0 ;
}
HRESULT IGA_IDirect3DIndexBuffer8::FreePrivateData( REFGUID refguid) 
{
    return 0 ;
}
DWORD   IGA_IDirect3DIndexBuffer8::SetPriority( DWORD PriorityNew) 
{
    return 0 ;
}
DWORD   IGA_IDirect3DIndexBuffer8::GetPriority() 
{
    return 0 ;
}
void    IGA_IDirect3DIndexBuffer8::PreLoad() 
{
    return  ;
}
D3DRESOURCETYPE IGA_IDirect3DIndexBuffer8::GetType() 
{
    return D3DRTYPE_SURFACE;
}
HRESULT IGA_IDirect3DIndexBuffer8::Lock( UINT OffsetToLock,UINT SizeToLock,BYTE** ppbData,DWORD Flags) 
{
    return 0 ;
}
HRESULT IGA_IDirect3DIndexBuffer8::Unlock() 
{
    return 0 ;
}
HRESULT IGA_IDirect3DIndexBuffer8::GetDesc( D3DINDEXBUFFER_DESC *pDesc) 
{
    return 0 ;
}

VOID IGA_IDirect3DIndexBuffer8::Init(IDirect3DIndexBuffer8*  pIndexBuffer8)
{
    DWORD  dwNewFunAddr  = 0;
	DWORD  dwOldFunAddr  = 0;
	
	IGA_IDirect3DIndexBuffer8     newIndexBuffer8;
	
	//release
    dwNewFunAddr          = GetOneVirtualFunAddr(INDEXBUFFER8_RELEASE_INDEX,&newIndexBuffer8);
	s_pFunRelease         = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,INDEXBUFFER8_RELEASE_INDEX ,pIndexBuffer8);	
	
	//LockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(INDEXBUFFER8_LOCK_INDEX,&newIndexBuffer8);
	s_pFunLockRect        = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,INDEXBUFFER8_LOCK_INDEX ,pIndexBuffer8);
	
	//UnlockRect
    dwNewFunAddr          = GetOneVirtualFunAddr(INDEXBUFFER8_UNLOCK_INDEX,&newIndexBuffer8);
	s_pFunUnlockRect      = (VOID*)SetOneVirtualFunAddr(dwNewFunAddr,INDEXBUFFER8_UNLOCK_INDEX ,pIndexBuffer8);
}
