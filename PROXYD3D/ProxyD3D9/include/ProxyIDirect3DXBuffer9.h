#ifndef _PROXY_IDIRECT3DXBUFFER9_H
#define _PROXY_IDIRECT3DXBUFFER9_H
#include <d3d9.h>
//************************************************************************************************************
// class      :  
// purpose    :  ¶¥µã»º³å£¬Ë÷Òý»º³å
// Date       :  2009-7-30  11:27:35
// author     :  3o3afe1
//************************************************************************************************************
// 
// class Filter_IDirect3DVertexBuffer9;
// class IGA_IDirect3DVertexBuffer9 : public IDirect3DVertexBuffer9
// {
// private:
// 	enum
// 	{
// 	    	VERTEXBUFFER9_QUERYINTERFACE_INDEX  =  0,		
// 			VERTEXBUFFER9_ADDREF_INDEX          =  1,		
// 			VERTEXBUFFER9_RELEASE_INDEX         =  2,		
// 			VERTEXBUFFER9_GETDEVICE_INDEX       =  3,		
// 			VERTEXBUFFER9_SETPRIVATEDATA_INDEX  =  4,		
// 			VERTEXBUFFER9_GETPRIVATEDATA_INDEX  =  5,		
// 			VERTEXBUFFER9_FREEPRIVATEDATA_INDEX =  6,		
// 			
//             VERTEXBUFFER9_SETPRIORITY_INDEX     =  7,
//             VERTEXBUFFER9_GETPRIORITY_INDEX     =  8,
//             VERTEXBUFFER9_PRELOAD_INDEX         =  9, 
//             VERTEXBUFFER9_GETTYPE_INDEX         = 10,
//             VERTEXBUFFER9_LOCK_INDEX            = 11,
//             VERTEXBUFFER9_UNLOCK_INDEX          = 12, 
//             VERTEXBUFFER9_GETDESC_INDEX         = 13
// 	};
// public:  
// 	/*** IUnknown methods ***/
//     HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) ;
//     ULONG   STDMETHODCALLTYPE AddRef() ;
//     ULONG   STDMETHODCALLTYPE Release() ;
// 	
//     /*** IDirect3DResource9 methods ***/
//     HRESULT STDMETHODCALLTYPE GetDevice( IDirect3DDevice9** ppDevice) ;
//     HRESULT STDMETHODCALLTYPE SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) ;
//     HRESULT STDMETHODCALLTYPE GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) ;
//     HRESULT STDMETHODCALLTYPE FreePrivateData( REFGUID refguid) ;
//     DWORD   STDMETHODCALLTYPE SetPriority( DWORD PriorityNew) ;
//     DWORD   STDMETHODCALLTYPE GetPriority() ;
//     void    STDMETHODCALLTYPE PreLoad() ;
//     D3DRESOURCETYPE STDMETHODCALLTYPE GetType() ;
//     HRESULT STDMETHODCALLTYPE Lock( UINT OffsetToLock,UINT SizeToLock,BYTE** ppbData,DWORD Flags) ;
//     HRESULT STDMETHODCALLTYPE Unlock() ;
//     HRESULT STDMETHODCALLTYPE GetDesc( D3DVERTEXBUFFER_DESC *pDesc) ;
// 
// public:
// 
// 	static   VOID*    s_pFunRelease   ;
// 	static   VOID*    s_pFunLockRect  ;
// 	static   VOID*    s_pFunUnlockRect;
// 	static  Filter_IDirect3DVertexBuffer9*   s_pFilter;
// 	
// 	static   VOID Init(IDirect3DVertexBuffer9*  pVertexBuffer9);
// 
// };
// 
// 
// 
// class Filter_IDirect3DIndexBuffer9;
// class IGA_IDirect3DIndexBuffer9 : public IDirect3DIndexBuffer9
// {
// 	enum
// 	{
// 	    	INDEXBUFFER9_QUERYINTERFACE_INDEX  =  0,		
// 			INDEXBUFFER9_ADDREF_INDEX          =  1,		
// 			INDEXBUFFER9_RELEASE_INDEX         =  2,		
// 			INDEXBUFFER9_GETDEVICE_INDEX       =  3,		
// 			INDEXBUFFER9_SETPRIVATEDATA_INDEX  =  4,		
// 			INDEXBUFFER9_GETPRIVATEDATA_INDEX  =  5,		
// 			INDEXBUFFER9_FREEPRIVATEDATA_INDEX =  6,		
// 			
//             INDEXBUFFER9_SETPRIORITY_INDEX     =  7,
//             INDEXBUFFER9_GETPRIORITY_INDEX     =  9,
//             INDEXBUFFER9_PRELOAD_INDEX         =  9, 
//             INDEXBUFFER9_GETTYPE_INDEX         = 10,
//             INDEXBUFFER9_LOCK_INDEX            = 11,
//             INDEXBUFFER9_UNLOCK_INDEX          = 12, 
//             INDEXBUFFER9_GETDESC_INDEX         = 13
// 	};
// public:
//     /*** IUnknown methods ***/
//     HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) ;
//     ULONG   STDMETHODCALLTYPE AddRef() ;
//     ULONG   STDMETHODCALLTYPE Release() ;
// 	
//     /*** IDirect3DResource9 methods ***/
//     HRESULT STDMETHODCALLTYPE GetDevice( IDirect3DDevice9** ppDevice) ;
//     HRESULT STDMETHODCALLTYPE SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) ;
//     HRESULT STDMETHODCALLTYPE GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) ;
//     HRESULT STDMETHODCALLTYPE FreePrivateData( REFGUID refguid) ;
//     DWORD   STDMETHODCALLTYPE SetPriority( DWORD PriorityNew) ;
//     DWORD   STDMETHODCALLTYPE GetPriority() ;
//     void    STDMETHODCALLTYPE PreLoad() ;
//     D3DRESOURCETYPE STDMETHODCALLTYPE GetType() ;
//     HRESULT STDMETHODCALLTYPE Lock( UINT OffsetToLock,UINT SizeToLock,BYTE** ppbData,DWORD Flags) ;
//     HRESULT STDMETHODCALLTYPE Unlock() ;
//     HRESULT STDMETHODCALLTYPE GetDesc( D3DINDEXBUFFER_DESC *pDesc) ;
// 	
// public:
// 	static   VOID*    s_pFunRelease   ;
// 	static   VOID*    s_pFunLockRect  ;
// 	static   VOID*    s_pFunUnlockRect;
// 	
// 	static  Filter_IDirect3DIndexBuffer9*   s_pFilter;
// 	
// 	static   VOID Init(IDirect3DIndexBuffer9*  pIndexBuffer9);
// };
class IGA_IDirect3DVertexBuffer9 
{
public:
	IGA_IDirect3DVertexBuffer9(){}
	~IGA_IDirect3DVertexBuffer9(){}
};

class IGA_IDirect3DIndexBuffer9 
{
public:
	IGA_IDirect3DIndexBuffer9(){}
	~IGA_IDirect3DIndexBuffer9(){}	
};

#endif