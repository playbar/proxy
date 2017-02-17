#ifndef _PROXY_IDIRECT3DXBUFFER8_H
#define _PROXY_IDIRECT3DXBUFFER8_H
#include <d3d8.h>
//************************************************************************************************************
// class      :  
// purpose    :  ¶¥µã»º³å£¬Ë÷Òý»º³å
// Date       :  2008-7-30  11:27:35
// author     :  3o3afe1
//************************************************************************************************************

class Filter_IDirect3DVertexBuffer8;
class IGA_IDirect3DVertexBuffer8 : public IDirect3DVertexBuffer8
{
private:
	enum
	{
	    	VERTEXBUFFER8_QUERYINTERFACE_INDEX  =  0,		
			VERTEXBUFFER8_ADDREF_INDEX          =  1,		
			VERTEXBUFFER8_RELEASE_INDEX         =  2,		
			VERTEXBUFFER8_GETDEVICE_INDEX       =  3,		
			VERTEXBUFFER8_SETPRIVATEDATA_INDEX  =  4,		
			VERTEXBUFFER8_GETPRIVATEDATA_INDEX  =  5,		
			VERTEXBUFFER8_FREEPRIVATEDATA_INDEX =  6,		
			
            VERTEXBUFFER8_SETPRIORITY_INDEX     =  7,
            VERTEXBUFFER8_GETPRIORITY_INDEX     =  8,
            VERTEXBUFFER8_PRELOAD_INDEX         =  9, 
            VERTEXBUFFER8_GETTYPE_INDEX         = 10,
            VERTEXBUFFER8_LOCK_INDEX            = 11,
            VERTEXBUFFER8_UNLOCK_INDEX          = 12, 
            VERTEXBUFFER8_GETDESC_INDEX         = 13
	};
public:  
	/*** IUnknown methods ***/
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) ;
    ULONG   STDMETHODCALLTYPE AddRef() ;
    ULONG   STDMETHODCALLTYPE Release() ;
	
    /*** IDirect3DResource8 methods ***/
    HRESULT STDMETHODCALLTYPE GetDevice( IDirect3DDevice8** ppDevice) ;
    HRESULT STDMETHODCALLTYPE SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) ;
    HRESULT STDMETHODCALLTYPE FreePrivateData( REFGUID refguid) ;
    DWORD   STDMETHODCALLTYPE SetPriority( DWORD PriorityNew) ;
    DWORD   STDMETHODCALLTYPE GetPriority() ;
    void    STDMETHODCALLTYPE PreLoad() ;
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType() ;
    HRESULT STDMETHODCALLTYPE Lock( UINT OffsetToLock,UINT SizeToLock,BYTE** ppbData,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE Unlock() ;
    HRESULT STDMETHODCALLTYPE GetDesc( D3DVERTEXBUFFER_DESC *pDesc) ;

public:

	static   VOID*    s_pFunRelease   ;
	static   VOID*    s_pFunLockRect  ;
	static   VOID*    s_pFunUnlockRect;
	static  Filter_IDirect3DVertexBuffer8*   s_pFilter;
	
	static   VOID Init(IDirect3DVertexBuffer8*  pVertexBuffer8);

};



class Filter_IDirect3DIndexBuffer8;
class IGA_IDirect3DIndexBuffer8 : public IDirect3DIndexBuffer8
{
	enum
	{
	    	INDEXBUFFER8_QUERYINTERFACE_INDEX  =  0,		
			INDEXBUFFER8_ADDREF_INDEX          =  1,		
			INDEXBUFFER8_RELEASE_INDEX         =  2,		
			INDEXBUFFER8_GETDEVICE_INDEX       =  3,		
			INDEXBUFFER8_SETPRIVATEDATA_INDEX  =  4,		
			INDEXBUFFER8_GETPRIVATEDATA_INDEX  =  5,		
			INDEXBUFFER8_FREEPRIVATEDATA_INDEX =  6,		
			
            INDEXBUFFER8_SETPRIORITY_INDEX     =  7,
            INDEXBUFFER8_GETPRIORITY_INDEX     =  8,
            INDEXBUFFER8_PRELOAD_INDEX         =  9, 
            INDEXBUFFER8_GETTYPE_INDEX         = 10,
            INDEXBUFFER8_LOCK_INDEX            = 11,
            INDEXBUFFER8_UNLOCK_INDEX          = 12, 
            INDEXBUFFER8_GETDESC_INDEX         = 13
	};
public:
    /*** IUnknown methods ***/
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) ;
    ULONG   STDMETHODCALLTYPE AddRef() ;
    ULONG   STDMETHODCALLTYPE Release() ;
	
    /*** IDirect3DResource8 methods ***/
    HRESULT STDMETHODCALLTYPE GetDevice( IDirect3DDevice8** ppDevice) ;
    HRESULT STDMETHODCALLTYPE SetPrivateData( REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE GetPrivateData( REFGUID refguid,void* pData,DWORD* pSizeOfData) ;
    HRESULT STDMETHODCALLTYPE FreePrivateData( REFGUID refguid) ;
    DWORD   STDMETHODCALLTYPE SetPriority( DWORD PriorityNew) ;
    DWORD   STDMETHODCALLTYPE GetPriority() ;
    void    STDMETHODCALLTYPE PreLoad() ;
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType() ;
    HRESULT STDMETHODCALLTYPE Lock( UINT OffsetToLock,UINT SizeToLock,BYTE** ppbData,DWORD Flags) ;
    HRESULT STDMETHODCALLTYPE Unlock() ;
    HRESULT STDMETHODCALLTYPE GetDesc( D3DINDEXBUFFER_DESC *pDesc) ;
	
public:
	static   VOID*    s_pFunRelease   ;
	static   VOID*    s_pFunLockRect  ;
	static   VOID*    s_pFunUnlockRect;
	
	static  Filter_IDirect3DIndexBuffer8*   s_pFilter;
	
	static   VOID Init(IDirect3DIndexBuffer8*  pIndexBuffer8);
};


#endif