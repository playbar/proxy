// KFlash.cpp: implementation of the KFlash class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KFlash.h"



//////////////////////////////////////////////////////////////////////

KFlash::KFlash()
{    
	 
}

KFlash::~KFlash()
{
}

BOOL KFlash::Create(HWND hParent)
{
	BOOL  bResult = FALSE;
	
	if  ( !KWindow::Create(hParent) ) 
		return FALSE;
    
	RECT  rc;
    GetClientRect(m_hWnd, &rc );	
	HWND hwnd    = m_container.Create( m_hWnd, rc, LPCTSTR("ShockwaveFlash.ShockwaveFlash.1"), WS_CHILD|WS_VISIBLE );		    	
	if   ( hwnd == NULL )   return FALSE;	
	
	m_container.QueryControl(&IFlashPlayer);	
	
	return	TRUE;

}


BOOL KFlash::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
	{
		case WM_SIZE:
			{
				if  ( m_container != NULL )
				{				
					RECT  rc;
					GetClientRect(m_hWnd, &rc );
					m_container.MoveWindow(&rc,TRUE);
				}
			}      
			break;
		case WM_TIMER :
			{
				if  ( m_nMinute == 0 )
				{
					m_bStop = TRUE;					
				}
				else
				{
					m_nMinute--;
				}

			}
            break;
		default:
			break;
	}

	return	TRUE;
}
BOOL    KFlash::SetAlpah(BYTE  nAlpah)
{
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	if ( hInst )
	{
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC    fun = NULL; 

        fun=(MYFUNC)GetProcAddress(hInst,_T("SetLayeredWindowAttributes"));	
		if(fun)  
		{
			::SetWindowLong(m_hWnd,GWL_EXSTYLE,::GetWindowLong(m_hWnd,GWL_EXSTYLE)^0x80000); 
			fun(m_hWnd,RGB(255,255,255),nAlpah,2); 		
		}
		FreeLibrary(hInst); 
	}

	return TRUE;
}

//ÏÔÊ¾´°Ìå	    
BOOL    KFlash::CenterWindow(int  nWidht , int nHeight)
{
	
	int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);     
    int nScreenWidth  = ::GetSystemMetrics(SM_CXSCREEN);     
	
	::MoveWindow(m_hWnd,(nScreenWidth- nWidht)/2,(nScreenHeight- nHeight)/2,nWidht,nHeight,TRUE);
	

	TCHAR  szBuf [ 640 ];
	wsprintf(szBuf,"width =%u  nHeight = %u  ",nWidht,nHeight);
	::OutputDebugString(szBuf);

	::ShowWindow(m_hWnd,SW_SHOW);
	
	return FALSE;
}



/*²¥·ÅContainer*/
BOOL  KFlash::Play(LPCTSTR lpszName,int nMinute   )
{
	if ( IFlashPlayer != NULL )
	{	
		_bstr_t aTrans = "Transparent";
		
        
		IFlashPlayer->put_WMode(aTrans);
		
		IFlashPlayer->Movie         = lpszName;
		IFlashPlayer->ScaleMode     = 2;		
		
		IFlashPlayer->Loop          = (VARIANT_BOOL)0xFFFF;
		IFlashPlayer->Menu          = (VARIANT_BOOL)0xFFFF;
		
		
		IFlashPlayer->Play();

		m_bStop = FALSE;
		m_nMinute = nMinute;
		::SetTimer(m_hWnd,1,1000,NULL);
		
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

BOOL KFlash::DestroyWindow()
{

	::DestroyWindow(m_hWnd);
	return TRUE;
}

