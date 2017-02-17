// KFlash.cpp: implementation of the KFlash class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KFlash.h"



KFlashSink::KFlashSink()
{
       m_dwRefCount = 0;
}
KFlashSink::~KFlashSink()
{

}

HRESULT  KFlashSink::QueryInterface(REFIID riid, LPVOID* ppv)
{
	*ppv = NULL;
	if (riid == IID_IUnknown)
	{
		*ppv = (LPUNKNOWN)this;
		AddRef();
		return S_OK;
	}
	else if (riid == IID_IDispatch)
	{
		*ppv = (IDispatch*)this;
		AddRef();
		return S_OK;
	}
	else if (riid == __uuidof(IGAFlash::_IShockwaveFlashEvents))
	{
		*ppv = (IGAFlash::_IShockwaveFlashEvents*) this;
		AddRef();
		return S_OK;
	}
	else
	{   
		return E_NOTIMPL;
	}
}

ULONG KFlashSink::AddRef()
{  
	return ++m_dwRefCount;
}

ULONG KFlashSink::Release()
{  
	return --m_dwRefCount;	
}

// IDispatch method
HRESULT KFlashSink::GetTypeInfoCount(UINT* pctinfo){	return E_NOTIMPL; }
HRESULT KFlashSink::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo){	return E_NOTIMPL; }
HRESULT KFlashSink::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames,	UINT cNames, LCID lcid,DISPID* rgDispId){	return E_NOTIMPL; }

HRESULT KFlashSink::Invoke(DISPID dispIdMember,
						   REFIID riid, 
						   LCID lcid,
	                       WORD wFlags,
						   ::DISPPARAMS __RPC_FAR *pDispParams, 
						   VARIANT __RPC_FAR *pVarResult,
	                       ::EXCEPINFO __RPC_FAR *pExcepInfo,
						   UINT __RPC_FAR *puArgErr)
{

	TCHAR  szBuff[255]; 
	wsprintf(szBuff,"dispIdMember=%d",dispIdMember); 
	OutputDebugString(szBuff); 

	switch(dispIdMember)
	{
		case 0x7a6:			
			break;
		case 0x96:			
			if ((pDispParams->cArgs == 2) &&
				(pDispParams->rgvarg[0].vt == VT_BSTR) &&
				(pDispParams->rgvarg[1].vt == VT_BSTR))
				{
					FSCommand(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
					
					wsprintf(szBuff,"    ",(char*)pDispParams->rgvarg[0].bstrVal); 
					OutputDebugString(szBuff); 

				}
			break;
		case DISPID_READYSTATECHANGE:					
			break;
		default:
			{			
			
			}
			return DISP_E_MEMBERNOTFOUND;
	} 

	return NOERROR;
}

HRESULT KFlashSink::OnReadyStateChange (long newState)
{	
	::OutputDebugString("OnReadyStateChange");
	return S_OK;
}

HRESULT KFlashSink::OnProgress(long percentDone )
{	
	::OutputDebugString("OnProgress");
	return S_OK;
}

HRESULT KFlashSink::FSCommand (_bstr_t command, _bstr_t args)
{
	char*  p  = (char*)command;
	p         = (char*)args;
	::OutputDebugString("FSCommand");
	return S_OK;
}	

HRESULT KFlashSink::FlashCall           (_bstr_t request )
{
	::OutputDebugString("FlashCall");
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KFlash::KFlash()
{
     m_dwWindwoType = K_WINDOW_FLASH;
}

KFlash::~KFlash()
{
// 	if ( 0 != m_dwSinkCooker)
// 	   IFlashPlayer.Unadvise(m_dwSinkCooker);
}

BOOL KFlash::Create(HWND hParent)
{
	if  ( !KContainerCtl::Create(hParent) ) return FALSE;
    
	RECT  rc;
    GetClientRect(m_hWnd, &rc );	
	HWND hwnd    = m_container.Create( m_hWnd, rc, LPCTSTR("ShockwaveFlash.ShockwaveFlash.1"), WS_CHILD|WS_VISIBLE );		    	
	if   ( hwnd == NULL )   return FALSE;	
	
	m_container.QueryControl(&IFlashPlayer);

	
	IFlashPlayer.Advise((IUnknown*)&m_sink,__uuidof(IGAFlash::_IShockwaveFlashEvents),&m_dwSinkCooker);	
	return	TRUE;

}
/*²¥·ÅContainer*/
BOOL  KFlash::Play(LPCTSTR lpszName,BOOL bLoopPaly)
{
	if ( IFlashPlayer != NULL )
	{	
		_bstr_t aTrans = "Transparent";
		
	//	_bstr_t aTrans =   "Opaque";
        
		IFlashPlayer->put_WMode(aTrans);

		IFlashPlayer->Movie         = lpszName;
		IFlashPlayer->ScaleMode     = 2;		
		
		IFlashPlayer->Loop          = (VARIANT_BOOL)0xFFFF;
		IFlashPlayer->Menu          = (VARIANT_BOOL)0xFFFF;
		
		IFlashPlayer->Play();


// 		HWND   hwnd = ::FindWindowEx(m_container.m_hWnd,NULL,NULL,NULL);
// 		HINSTANCE hInst = LoadLibrary("User32.DLL");
// 		if ( hInst )
// 		{
// 			typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
// 			MYFUNC    fun = NULL; 
// 			
// 			fun=(MYFUNC)GetProcAddress(hInst,_T("SetLayeredWindowAttributes"));	
// 			if(fun)  
// 			{
// 				::SetWindowLong(hwnd,GWL_EXSTYLE,::GetWindowLong(m_hWnd,GWL_EXSTYLE)^0x80000); 
// 				fun(hwnd,RGB(255,255,255),50,1); 		
// 			}
// 			FreeLibrary(hInst); 
// 		}


		return TRUE;
	}

	return FALSE;
}

VOID  KFlash::Stop()
{
	if ( IFlashPlayer != NULL )
	{	
		IFlashPlayer->Stop();
	}

}


