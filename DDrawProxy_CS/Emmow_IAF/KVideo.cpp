// KVideo.cpp: implementation of the KVideo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KVideo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KVideo::KVideo()
{
	m_dwWindwoType = K_WINDOW_FLASH;
}

KVideo::~KVideo()
{

}

BOOL KVideo::Create(HWND hParent)
{
	if  ( !KContainerCtl::Create(hParent) ) return FALSE;
    
	RECT  rc;
    GetClientRect(m_hWnd, &rc );	
	HWND hwnd    = m_container.Create( m_hWnd, rc, LPCTSTR("WMPlayer.OCX.7"), WS_CHILD|WS_VISIBLE );		    	
	if   ( hwnd == NULL )   return FALSE;	
	
	m_container.QueryControl(&IMediaPalyer);	 
	return	TRUE;
	
}

BOOL KVideo::Play(LPCTSTR lpszName,BOOL bLoopPaly)
{

	if ( IMediaPalyer != NULL )
	{	
        _bstr_t   url = lpszName;
		IMediaPalyer->PutURL(url);
		IMediaPalyer->PutenableContextMenu(FALSE);
		IMediaPalyer->PutstretchToFit(TRUE);		
		
		_bstr_t  mode("none");
		IMediaPalyer->PutuiMode(mode);
		//IMediaPalyer->PutwindowlessVideo(TRUE);
		
		IMediaPalyer->Getsettings()->PutplayCount(5000);
		IMediaPalyer->Getcontrols()->play();
		
		return TRUE;
	}

	return FALSE;
}

VOID  KVideo::Stop()
{
	if ( IMediaPalyer != NULL )
	{	     
		IMediaPalyer->Getcontrols()->stop();
	}
  
}