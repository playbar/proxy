// Window.cpp: implementation of the KWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Window.h"
#include "tchar.h"
#define ASSERT  0
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static TCHAR IAG_SZ_REGISTER_CLASSNAME[] = {_T("IAG_WINDOWS_XXXXXX")};


static LRESULT CALLBACK __WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( msg == WM_DESTROY )
	{			
		return	TRUE;
	}
	
	LRESULT   lResult = 1;
	KWindow	*_thisWindow	= (KWindow *)::GetWindowLong(hWnd, GWL_USERDATA);
	if( _thisWindow )
	{
		 lResult = _thisWindow->HandleMessage(msg, wParam, lParam);		
	}

	if ( lResult )
		lResult = ::DefWindowProc(hWnd, msg, wParam, lParam);
	return	lResult;
}


/*³õÊ¼×¢²á´°Ìå*/
BOOL IGAWinInit()
{	
	WNDCLASSEX	wc	= { 0 };
	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.hInstance	= ::GetModuleHandle(NULL);
	wc.lpfnWndProc	= __WindowProc;
	wc.lpszClassName= IAG_SZ_REGISTER_CLASSNAME;  

	
	wc.hbrBackground=  NULL;
	
	
	if(!::RegisterClassEx(&wc))
	{
		return	FALSE;
	}
	
	return TRUE;
}



KWindow::KWindow()
	:m_hWnd(NULL)	
{
}

KWindow::~KWindow()
{
	Close();		
}

BOOL KWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{

	
	return	TRUE;
}

BOOL KWindow::Create(HWND hParent)
{
	ASSERT( this->m_hWnd == NULL );
	TCHAR	szClassName[ 128 ]	= { 0 };
    wsprintf(szClassName, _T("WindowSinker_%p"), this);

	WNDCLASSEX	wc	= { 0 };
	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.hInstance	= ::GetModuleHandle(NULL);
	wc.lpfnWndProc	= __WindowProc;
	wc.lpszClassName= szClassName;
    wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	
	

	if(!::RegisterClassEx(&wc))
	{
		
		m_hWnd	= ::CreateWindowEx( 0, IAG_SZ_REGISTER_CLASSNAME, IAG_SZ_REGISTER_CLASSNAME,
			WS_POPUP, 0, 0, 160, 120, hParent, NULL, wc.hInstance, NULL );
		if( NULL == m_hWnd )
		{
          IGAWinInit();
		  return FALSE;
		}
	}
	else
	{
		//WS_OVERLAPPEDWINDOW
		m_hWnd	= ::CreateWindowEx( 0, szClassName, szClassName,
			WS_POPUP, 0, 0, 160, 120, hParent, NULL, wc.hInstance, NULL );
		
		if( NULL == m_hWnd )
		{
			::UnregisterClass( szClassName, wc.hInstance );
			return	FALSE;
		}

	}

	ASSERT( m_hWnd != NULL );	
	::SetWindowLong(this->m_hWnd, GWL_USERDATA, (LONG)this);

  
	return	TRUE;
}

void KWindow::Close()
{
	if( this->m_hWnd != NULL )
	{
		::SetWindowLong(this->m_hWnd, GWL_USERDATA, (LONG)0);
		
		this->m_hWnd	= NULL;
	}
}


BOOL KWindow::OnMessage(WPARAM wParam, LPARAM lParam)
{
	
	return	TRUE;
}


BOOL KWindow::Send(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT( this->m_hWnd != NULL );
	return	(BOOL)::SendMessage(this->m_hWnd, uMsg, wParam, lParam);
}

BOOL KWindow::Post(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT( this->m_hWnd != NULL );
	return	(BOOL)::PostMessage(this->m_hWnd, uMsg, wParam, lParam);
}
