#include "stdafx.h"

namespace IGAWIN
{	
    //  IAG弹出窗体dll
	typedef BOOL    (WINAPI *PFUNCLOSEIGAWINDOW)(DWORD  dwWnd);
	typedef BOOL    (WINAPI *PFUNSHOWIGAWINDOW)(HWND  hParent, int nLeft,int nTop,int nRight,int nBottom,LPCTSTR  szName , DWORD dwType );

	static PFUNCLOSEIGAWINDOW   s_pFunclose;
	static PFUNSHOWIGAWINDOW    s_pFunShow;
	static HMODULE              s_hPopWin;


	const DWORD   IGA_WM_CREATE_FLASH = ::RegisterWindowMessage(_T("IGA_CRETE_FLASH_HAHAHA_ZZF"));
	const DWORD   IGA_WM_CREATE_VIDEO = ::RegisterWindowMessage(_T("IGA_CRETE_VIDEO_HAHAHA_ZZF"));
	const DWORD   IGA_WM_CREATE_IE    = ::RegisterWindowMessage(_T("IGA_CRETE_IE_HAHAHA_ZZF"));	
	

    #define D3D_FLASH_INDEX   0
	#define D3D_VIDEO_INDEX   1
	#define D3D_IE_INDEX      2

	//保存设备相关的窗体信息
	struct   D3DPOS
	{
		DWORD          m_dwCooker;

		//子窗体位置
		int            x0;
		int            y0;
		int            x1;
		int            y1;        

		//文件路径
		TCHAR          szPath[256];
	};
	struct   D3DeviceHwnd
	{
		HWND           m_hWnd;
		void*          m_pDev;
		DWORD          m_pFunWndProc;	
		DWORD          m_dwRefCount;
		
		//窗体位置
        D3DPOS         m_pos[3];   

		D3DeviceHwnd()
		{	
			memset(this,0,sizeof(D3DeviceHwnd));
		}

	} ;

	//允许hook的窗体的最大数量
    #define  MAX_IGA_HOOK_WIN   4
	static D3DeviceHwnd   s_IGAWND_content[MAX_IGA_HOOK_WIN];

	//窗体函数
	LRESULT CALLBACK   IGAWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		LRESULT lResult = 1;  

		for( int i = 0; i <MAX_IGA_HOOK_WIN ;i++)
		{
			if( s_IGAWND_content[i].m_hWnd == hWnd)
			{			
				lResult = ::CallWindowProc((WNDPROC)(s_IGAWND_content[i].m_pFunWndProc),hWnd, msg, wParam, lParam);	
				
				if ( IGA_WM_CREATE_FLASH == msg) 
				{
					if ( NULL != s_pFunShow )
					{							
						OutputDebugString("********Flash播放**********");
						OutputDebugString(s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].szPath);
						s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].m_dwCooker =s_pFunShow(hWnd,
																				s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].x0,
																				s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].y0,
																				s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].x1,
																				s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].y1,																				
								                                                s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].szPath,
																				D3D_FLASH_INDEX);					
					}					
				}
				else if (IGA_WM_CREATE_VIDEO == msg)
				{
					if ( NULL != s_pFunShow )
					{							
						OutputDebugString("********视频播放**********");
						OutputDebugString(s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].szPath);;
						s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].m_dwCooker =s_pFunShow(hWnd,
							s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].x0,
							s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].y0,
							s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].x1,
							s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].y1,							
							s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].szPath,
							D3D_VIDEO_INDEX);					
					}
				}
				else if ( IGA_WM_CREATE_IE == msg)
				{}
				else if (  WM_SETCURSOR  == msg)
				{
						return 0;
				}
				else;;			
			}		
		}

		if ( lResult )
			lResult = ::DefWindowProc(hWnd, msg, wParam, lParam);		

		return lResult;
	}
   //内部使用函数 HOOK窗体
	BOOL   _Hook   ( VOID*  pD3Device,HWND   hwnd )
	{
		if (!::IsWindow(hwnd) )  return FALSE;
		
		//只考虑一台设备对应一个窗体句柄的
		for( int i = 0; i <MAX_IGA_HOOK_WIN ;i++)
		{
           if (s_IGAWND_content[i].m_hWnd == hwnd || s_IGAWND_content[i].m_pDev == pD3Device )   
			   return FALSE;
		}


		for( int i = 0; i <MAX_IGA_HOOK_WIN ;i++)
		{
			if (s_IGAWND_content[i].m_hWnd == NULL )
			{
				s_IGAWND_content[i].m_hWnd           = hwnd;
				s_IGAWND_content[i].m_pDev           = pD3Device;    
				s_IGAWND_content[i].m_pFunWndProc    = (DWORD)::GetWindowLong(hwnd,GWL_WNDPROC);	
				
				::SetWindowLong(hwnd,GWL_WNDPROC,(LONG)(IGAWndProc));
				::OutputDebugString("%*************WINHOOK success****************");
				return TRUE;
			}				
		}	   
		
		return FALSE;
	}

	BOOL   _UnHook ( VOID*  pD3Device )
	{
		
		for( int i = 0; i <MAX_IGA_HOOK_WIN ;i++)
		{
			if (s_IGAWND_content[i].m_pDev == pD3Device )   
			{
               ::SetWindowLong(s_IGAWND_content[i].m_hWnd,GWL_WNDPROC,(LONG)s_IGAWND_content[i].m_pFunWndProc)	;
			   return TRUE;
			}
			
		}		
		return FALSE;
	}


	//加载显示窗体 dll
	BOOL   Init()
	{
		TCHAR   TmpBuf[256];
		TCHAR   szTmpPath[256];
		::GetModuleFileName(NULL,TmpBuf,256);
		TCHAR* p = _tcsrchr(TmpBuf,'\\');
		*p = 0;

		wsprintf(szTmpPath,"%s\\EmmoaW.IAF",TmpBuf);
		s_hPopWin = ::LoadLibrary(szTmpPath);

		s_pFunShow    = (PFUNSHOWIGAWINDOW)::GetProcAddress(s_hPopWin,"ShowIGAWindow");
		s_pFunclose   = (PFUNCLOSEIGAWINDOW)::GetProcAddress(s_hPopWin,"CloseIGAWindow");

		return (s_hPopWin!=NULL);

	}

	VOID   UnInit()
	{

		s_pFunShow  = NULL;
		s_pFunclose = NULL;

		if( s_hPopWin )		
			::FreeLibrary(s_hPopWin);
		s_hPopWin      = NULL;
	}   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//创建flash
	BOOL CreateIGAWin(VOID* pDev,HWND  hwnd)
	{ 		
		TCHAR  szBuf[256];		
		wsprintf(szBuf,"*******3o3afe1*************************CreateIGAWin*******pDev=%p***hwnd=0x%8X*********",pDev,hwnd);
		::OutputDebugString(szBuf);
		

		::GetModuleFileName(NULL,szBuf,255);
		TCHAR* p = _tcsrchr(szBuf,'\\');
		if ( p != NULL )  *p =0;	
		/* 没有指定的模块，暂时先注销
		HMODULE   hPlugIn = ::LoadLibrary(_T("EmmoaWP.iaf"));
		if ( hPlugIn != NULL )
		{
		   ::OutputDebugString("窗口插件工作");
           typedef HWND(* LPFUN_WP1)(HWND);
		   LPFUN_WP1   pFun_WP1 = (LPFUN_WP1)::GetProcAddress(hPlugIn,_T("_WP1"));
		   hwnd = pFun_WP1(hwnd);
		   ::FreeLibrary(hPlugIn);
		}		
		*/

		_Hook(pDev,hwnd);			
		return TRUE;
	};

	BOOL DestoryIGAWin(VOID* pDev)
	{	
		_UnHook(pDev);
		for( int i = 0; i <MAX_IGA_HOOK_WIN ;i++)
		{
			if (s_IGAWND_content[i].m_pDev == pDev && s_pFunclose!= NULL )   
			{
				if ( s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].m_dwCooker != 0 )
				{
					(*s_pFunclose)(s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].m_dwCooker);
					s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].m_dwCooker = 0;
				}

				if ( s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].m_dwCooker != 0 )
				{
					(*s_pFunclose)(s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].m_dwCooker);
					s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].m_dwCooker = 0;
				}

				if ( s_IGAWND_content[i].m_pos[D3D_IE_INDEX].m_dwCooker != 0)
				{
					(*s_pFunclose)(s_IGAWND_content[i].m_pos[D3D_IE_INDEX].m_dwCooker);
					s_IGAWND_content[i].m_pos[D3D_IE_INDEX].m_dwCooker = 0;
				}
			}
		}

		return TRUE;
	}


	static HANDLE   s_hSendThread = NULL;
	DWORD WINAPI SendThread(LPVOID lp);

	//显示flash
	BOOL ShowFlash(VOID* pDev,int x0,int y0,int x1,int y1,DWORD dwWaitTime)
	{
		::OutputDebugString("*******3o3afe1************ShowFlash**start**************************");
		int    i       = 0;		
		if ( dwWaitTime == 0)   dwWaitTime = 3000;

		for(  i = 0; i <MAX_IGA_HOOK_WIN ;i++)
		{
			if (s_IGAWND_content[i].m_pDev == pDev )   
			{   
				TCHAR TmpBuf[256];
				::GetModuleFileName(NULL,TmpBuf,255);
				TCHAR* p = _tcsrchr(TmpBuf,'\\');
				if ( p != NULL )  *p =0;		
                
				TCHAR TmpPath[256];
				//wsprintf(TmpPath,"%s\\InterAdtive\\EMMOA\\Flash\\*.swf",TmpBuf);
				wsprintf(TmpPath,"%s\\InterAdtive\\EMMOA\\Flash\\adv.swf",TmpBuf);
				

				//KFileFind	Find;
				//if(!Find.FindFile(TmpPath))		return	FALSE;				
				BOOL		bFound	= FALSE;	
				BOOL        bValid  = FALSE;
				do 
				{
					//bFound	= Find.FindNextFile();
					//if( Find.IsDots() )				continue;
					//if( Find.IsDirectory() )        continue;
					//wsprintf(s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].szPath,_T("%s"),Find.GetFilePath().Get());					
					wsprintf(s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].szPath,_T("%s"),TmpPath);
					bValid = TRUE;
					break;
				}while(bFound);			
                if ( !bValid)  return FALSE;

				OutputDebugString(s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].szPath);
				s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].m_dwCooker = 0;
				s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].x0 =x0;
				s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].y0 =y0;
				s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].x1 =x1;
				s_IGAWND_content[i].m_pos[D3D_FLASH_INDEX].y1 =y1;

				break;
			}
		}

		if ( i < MAX_IGA_HOOK_WIN && s_hSendThread == NULL)
		{
			::OutputDebugString("*******3o3afe1************ShowFlash****************************");
		   s_hSendThread = ::CreateThread(NULL,0,SendThread,(LPVOID)D3D_FLASH_INDEX,0,0);
		   return TRUE;
		}
		return FALSE;		
	}
   
	//显示视频
	BOOL  ShowVideo   (VOID* pDev ,int x0,int y0,int x1,int y1,DWORD  dwWaitTime )
	{
		::OutputDebugString("*******3o3afe1************ShowVideo**start**************************");
		int    i       = 0;		

		if ( dwWaitTime == 0)   dwWaitTime = 3000;

		for(  i = 0; i <MAX_IGA_HOOK_WIN ;i++)
		{
			if (s_IGAWND_content[i].m_pDev == pDev )   
			{   
				TCHAR TmpBuf[256];
				::GetModuleFileName(NULL,TmpBuf,255);
				TCHAR* p = _tcsrchr(TmpBuf,'\\');
				if ( p != NULL )  *p =0;		

				TCHAR TmpPath[256];
				//wsprintf(TmpPath,"%s\\InterAdtive\\EMMOA\\video\\*.wmv",TmpBuf);
				wsprintf(TmpPath,"%s\\InterAdtive\\EMMOA\\video\\adv.wmv",TmpBuf);

				//KFileFind	Find;
				//if(!Find.FindFile(TmpPath))		return	FALSE;				
				BOOL		bFound	= FALSE;
				BOOL        bValid  = FALSE;
				do 
				{
					//bFound	= Find.FindNextFile();
					//if( Find.IsDots() )				continue;
					//if( Find.IsDirectory() )        continue;
					//wsprintf(s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].szPath,_T("%s"),Find.GetFilePath().Get());
					wsprintf(s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].szPath,_T("%s"),TmpPath);
					bValid = TRUE;
					break;
				}while(bFound);			
				if ( !bValid)  return FALSE;

				s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].m_dwCooker = 0;
				s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].x0 =x0;
				s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].y0 =y0;
				s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].x1 =x1;
				s_IGAWND_content[i].m_pos[D3D_VIDEO_INDEX].y1 =y1;

				break;
			}
		}

		if ( i < MAX_IGA_HOOK_WIN && s_hSendThread == NULL)
		{
			::OutputDebugString("*******3o3afe1************ShowVideo****************************");
			s_hSendThread = ::CreateThread(NULL,0,SendThread,(LPVOID)D3D_VIDEO_INDEX,0,0);
			return TRUE;
		}
		return FALSE;		
	}
     
	//显示ie
	BOOL  ShowIE     (VOID* ,int x0,int y0,int x1,int y1,DWORD  dwWaitTime )
	{
		return TRUE;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD WINAPI SendThread(LPVOID lp)
	{

		BOOL  bRun = TRUE ;
        HWND  hWnd = NULL ;
        
		
		DWORD  dwType = (DWORD)lp; 
		if ( dwType > D3D_VIDEO_INDEX)
			dwType = D3D_VIDEO_INDEX;

		while(bRun)
		{	
			int  j = 0;
			for( int i = 0; i <MAX_IGA_HOOK_WIN ;i++)
			{
				hWnd = s_IGAWND_content[i].m_hWnd;
				

				if ( s_IGAWND_content[i].m_pos[dwType].m_dwCooker  == 0 && hWnd != NULL )   
				{
					if ( !(::GetWindowLong(hWnd,GWL_STYLE ) & WS_VISIBLE ))  continue;					
					if ( dwType == D3D_FLASH_INDEX)
					{
					   ::SendMessage(hWnd,IGA_WM_CREATE_FLASH,0,0);					
					}
					else if ( dwType == D3D_VIDEO_INDEX)
					{
						::SendMessage(hWnd,IGA_WM_CREATE_VIDEO,0,0);						
					}
					else
						::OutputDebugString("*******3o3afe1******thread4****");;
				}
				else
				{
                     j++;
				}
				::Sleep(2000);
			}

			if ( j == MAX_IGA_HOOK_WIN)
			{
				::CloseHandle(s_hSendThread);
				return 100;
			}
		}
		return 0;
	}
}
