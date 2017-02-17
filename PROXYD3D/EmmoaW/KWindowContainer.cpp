// KContainerCtl.cpp: implementation of the KContainerCtl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "kwindowcontainer.h"
//************************************************************************************************************
// class      :  显示Container弹出窗体
// purpose    :  
// Date       :  2008-6-20  9:25:32
// author     :  3o3afe1
//************************************************************************************************************
KContainerCtl::PARENT_MAP    KContainerCtl::s_ParentContainer; 	

static LRESULT CALLBACK __WindowHookProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{    

	LRESULT  lResult = 1;
	

	BOOL            bExcuted= FALSE;
	HWND            hParent = NULL;
	KContainerCtl*   pContainer  = NULL;

	KContainerCtl::PARENT_MAP_IT   it      = KContainerCtl::s_ParentContainer.find(hWnd);

	if ( it != KContainerCtl::s_ParentContainer.end())
	{	
		lResult  = ::CallWindowProc((*it).second.pFunProc,hWnd,  msg,  wParam,  lParam);				   
		if ( lResult )
           lResult = ::DefWindowProc(hWnd, msg, wParam, lParam);		


		if( msg == WM_MOVE  || msg == WM_SIZE  ) 
		{
			KContainerCtl::CHIELD_VEC   child = (*it).second;

			for( KContainerCtl::Container_VEC_IT  it1 = child.ContainerArr.begin() ; it1 != child.ContainerArr.end() ;it1++)
			{
                (*it1)->ShowWindow();
            }			
		}

	}   
	return	lResult;
}



KContainerCtl::KContainerCtl():
          m_pFunParentCallProc(NULL),
		  m_hParent(NULL)
{
}

KContainerCtl::~KContainerCtl()
{
}

//子类化父窗体
VOID KContainerCtl::HookParent(HWND hParent)
{
	if ( hParent == NULL|| !::IsWindow(hParent))  return;    
		
	PARENT_MAP_IT it = s_ParentContainer.find(hParent);
    if ( it != s_ParentContainer.end())
	{
       m_pFunParentCallProc = (*it).second.pFunProc;
	   (*it).second.ContainerArr.push_back(this);
	}
	else
	{
		m_pFunParentCallProc = (WNDPROC)::SetWindowLong(hParent,GWL_WNDPROC,(LONG)__WindowHookProc);

		CHIELD_VEC     childWin;    
		childWin.ContainerArr.push_back(this);
		childWin.pFunProc = m_pFunParentCallProc;
		s_ParentContainer.insert(PARENT_MAP_VALUE(hParent,childWin));    
	}
	
}
VOID KContainerCtl::UnHookParent (HWND hParent)
{
	if ( hParent == NULL|| !::IsWindow(hParent))  return;    

	PARENT_MAP_IT it0 = s_ParentContainer.find(hParent);
	if( it0 != s_ParentContainer.end())
	{	
		CHIELD_VEC   child = (*it0).second;
		for( Container_VEC_IT  it1 = child.ContainerArr.begin() ; it1 != child.ContainerArr.end() ;it1--)
		{
            if ( *it1  == this )
			{
				child.ContainerArr.erase(it1);
                break;
			}
		}
		
		if (child.ContainerArr.size() == 0 )
		{
			s_ParentContainer.erase(it0);
			::SetWindowLong( hParent , GWL_WNDPROC ,(LONG)m_pFunParentCallProc);  
		}
	}  
}

BOOL KContainerCtl::Create(HWND hParent)
{
	BOOL  bResult = FALSE;

	if  ( !KWindow::Create(hParent) ) return FALSE;
	
    RECT  rc;
    GetClientRect(m_hWnd, &rc );	
    m_hBtnClose  = ::CreateWindow( "BUTTON","X",
		                          WS_CHILD|WS_VISIBLE,
								  rc.right-BTN_CLOSE_WIDTH,
								  0,
								  BTN_CLOSE_WIDTH,
								  BTN_CLOSE_HIGHT,
								  m_hWnd,
								  0,
								  ::GetModuleHandle(NULL),
								  0
								  );	
	if  (m_hBtnClose == NULL )    return FALSE;

	m_hParent = hParent;
	
	HookParent(hParent);
	
	DWORD  dwStyle  =  ::GetWindowLong(m_hWnd,GWL_STYLE);
	::SetWindowLong(m_hWnd,GWL_STYLE,dwStyle|WS_VISIBLE);
	//SetWindowPos(m_hWnd,HWND_TOP ,0,0,10,10,SWP_SHOWWINDOW);

	return	TRUE;
}

BOOL KContainerCtl::OnSize()
{
	if ( m_container!= NULL && m_container.IsWindow() )
	{				
		RECT  rc;
		GetClientRect(m_hWnd, &rc );
		
		::MoveWindow(m_hBtnClose,rc.right-BTN_CLOSE_WIDTH,0,BTN_CLOSE_WIDTH,BTN_CLOSE_HIGHT,TRUE);
		
		
		
		POINT  p0[6];
		p0[0].x =  rc.right - BTN_CLOSE_WIDTH;
		p0[0].y =  rc.top;
		
		p0[1].x =  rc.right ;
		p0[1].y =  rc.top;
		
		p0[2].x =  rc.right ;
		p0[2].y =  rc.bottom;
		
		p0[3].x =  rc.left;
		p0[3].y =  rc.bottom;
		
		p0[4].x =  rc.left;
		p0[4].y =  rc.top + BTN_CLOSE_HIGHT;
		
		p0[5].x =  rc.right - BTN_CLOSE_WIDTH;
		p0[5].y =  rc.top   + BTN_CLOSE_HIGHT;
		
		
		HRGN   rgn  = ::CreatePolygonRgn(p0,6,ALTERNATE);
		::SetWindowRgn(m_hWnd,rgn,TRUE)	;
		
		rc.top += BTN_CLOSE_HIGHT ;
		m_container.MoveWindow(&rc,TRUE);
		
		return TRUE;
	}

	return FALSE;
}
BOOL KContainerCtl::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
	{
	    case WM_CREATE:
			{
	
			}
		     break;
		case WM_PAINT:
			{
			}
			 break;
		case WM_SIZE:
			{
			    OnSize();
			}
			break;
        case WM_COMMAND:
			{			
				DestroyWindow();
			}
			break;
		case WM_ERASEBKGND:
			{   
				
			}
			 break;
        case WM_DESTROY:
			{
				

			}
			break;
		default:
			break;
	}

	return	TRUE;
}
BOOL    KContainerCtl::SetAlpah(BYTE  nAlpah)
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


BOOL  KContainerCtl::ShowWindow()
{
	POINT   p0,p1;
	p0.x   = m_nLeft;
	p0.y   = m_nTop;
	ClientToScreen(m_hParent,&p0);
	
	p1.x   = m_nRight;
	p1.y   = m_nBottom;
	ClientToScreen(m_hParent,&p1);	
	
    return ::MoveWindow(m_hWnd,p0.x,p0.y,m_nRight-m_nLeft,m_nBottom-m_nTop,TRUE);
}

BOOL  KContainerCtl::ShowWindow(int nLeft,int  nTop,int nRight,int nBottom)
{
    m_nLeft = nLeft;
    m_nTop  = nTop;

    m_nRight  = nRight ;
    m_nBottom = nBottom;

    return ShowWindow(); 
}

typedef BOOL     (WINAPI *LP_FUN_CALLBACKPLAYTIMEFOCUS)(DWORD  ,DWORD ,DWORD  ,LPVOID   );
extern LP_FUN_CALLBACKPLAYTIMEFOCUS   s_pFunCallback ;

BOOL  KContainerCtl::DestroyWindow()
{	
 	if  ( NULL != s_pFunCallback )
 	{
 		s_pFunCallback(m_dwWndAddr,2,::GetTickCount(),NULL);
 	}

	::DestroyWindow(m_hWnd);		
    UnHookParent(m_hParent);	
	KContainerCtl::Delete(this);	
	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*动态申请的Container窗机*/
std::map< KContainerCtl* , BOOL  >   KContainerCtl::s_DynamicContainer;


VOID          KContainerCtl::Delete(KContainerCtl*  pWnd)
{
	std::map< KContainerCtl* , BOOL  >::iterator  it = 
                                           s_DynamicContainer.find(pWnd);

	if ( it != s_DynamicContainer.end())
	{
		s_DynamicContainer.erase(it);
		delete pWnd;
	}
}
