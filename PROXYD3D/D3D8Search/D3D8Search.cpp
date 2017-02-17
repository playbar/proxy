//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//
// Empty starting point for new Direct3D applications
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"
#include "resource.h"
#include <WinInet.h>	//Import for INTERNET_MAX_URL_LENGTH
#include <DXUTmisc.h>
#include "D3D8Search.h"
#include "ToolFunction.h"
#include "ShowPic.h"
#include "locale.h"


RECT						g_rectIMEEdit;			//	输入框的大小
RECT						g_rectBtnSearch;		//	搜索按钮的大小
RECT						g_rectBtnShowSearch;	//  显示搜索界面的按钮

//CDXUTIMEEditBox * g_pIMEEdit	= NULL;
CDXUTDialogResourceManager	g_DialogResourceManager; //对话框资源管理器
CDXUTDialog					g_SampleUI;              //对话框

HANDLE	g_hExitEvent	= NULL;

DWORD g_dwGameID = 0;

//ESearchEngine类型
INT	g_nSearchEngine	= SEARCHENGINE_BAIDU;	

#define	IDS_MAIN_DEFAULT_CONFIG	"SEARCH.INI"

#define	IDS_DEFAULT_SEARCHTIPSW	L"请输入搜索内容"
#define	IDS_DEFAULT_SEARCH_TEXT	L"搜索"

#define	IDS_BAIDU_URLA		"http://www.baidu.com/s?wd="
#define	IDS_GOOGLE_URLA		"http://www.google.com//search?hl=zh-CN&q="

LPFN_SEARCHSTATCALLBACK	g_pfnSearchStateCallback	= NULL;
LPWNDPROC				g_pFunWinProc = NULL;


CShowPic *g_pShowPic = NULL;

typedef HRESULT(*pFunInitTexture)(LPDIRECT3DDEVICE9);
typedef LPDIRECT3DTEXTURE9(*pFunGetTexture)(int, bool*);
typedef void(*pFunDestroy)();

HMODULE			g_hD3D9Texture;
pFunInitTexture	g_pFunInitTexture;
pFunGetTexture	g_pFunGetTexture;
pFunDestroy		g_pFunDestoryTexture;
bool			g_bD3D9Texture = false;

bool InitIGATexture9(IDirect3DDevice9* pd3dDevice)
{
	g_hD3D9Texture		= NULL;
	g_pFunInitTexture	= NULL;
	g_pFunGetTexture	= NULL;
	g_pFunDestoryTexture= NULL;

	g_hD3D9Texture = LoadLibrary(TEXT("IGATexture9.dll"));

	if( g_hD3D9Texture != NULL )
	{
		g_pFunInitTexture	= (pFunInitTexture)	GetProcAddress( g_hD3D9Texture, "IGATextureInit");
		g_pFunGetTexture	= (pFunGetTexture)	GetProcAddress( g_hD3D9Texture, "IGAGetTexture");
		g_pFunDestoryTexture= (pFunDestroy)		GetProcAddress( g_hD3D9Texture, "IGATextureDestory");

		if( g_pFunInitTexture != NULL && g_pFunGetTexture != NULL && g_pFunDestoryTexture != NULL )
		{
			if( SUCCEEDED( g_pFunInitTexture( pd3dDevice ) ) )
			{
				g_bD3D9Texture = true;	
			}
		}
	}
	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CALLBACK SearchIEWindow(HWND hwnd, LPARAM lParam )
{

	OutputDebugStringA("SearchIEWindow");
	if(IsWindow(hwnd))
	{
		DWORD	id	= 0;
		GetWindowThreadProcessId(hwnd, &id);
		PROCESS_INFORMATION	*lppi	= (PROCESS_INFORMATION*)lParam;
		DWORD	dwPID	= lppi->dwProcessId;
		if(id == dwPID )
		{
			CHAR	szClassName[128]= { 0 };
			GetClassNameA(hwnd, szClassName, 128);
			if(lstrcmpiA(szClassName, "IEFrame") == 0)	//这好像是IE7.0.可能还需要其他的版本测试
			{
				::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
				return	FALSE;
			}
		}
	}
	return	TRUE;
}


/************************************************************************/
/* 后台启动IE的线程                                                     */
/************************************************************************/
DWORD WINAPI OpenIEThreadProc(void *p)
{
	LPCSTR	lpszURL	= (LPCSTR)p;
	assert(!::IsBadStringPtrA(lpszURL, -1));

	//	h获取IE所在路径
	CHAR szAppName[_MAX_PATH] = "C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE";//IE浏览器程序
	HKEY	hKey	= NULL;
	if(::RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE", &hKey) == ERROR_SUCCESS)
	{
		CHAR	szPathName[_MAX_PATH]	= { 0 };
		LONG	dwSize	= _MAX_PATH;
		if(ERROR_SUCCESS==::RegQueryValueA(hKey, "", szPathName, &dwSize))
		{
			StringCchCopyA(szAppName,_MAX_PATH,szPathName);
		}
		::RegCloseKey(hKey);
	}

	CHAR szCmdLine[_MAX_PATH]	= { 0 };
	StringCchPrintfA(szCmdLine, _MAX_PATH, "%s %s", szAppName, lpszURL);


	PROCESS_INFORMATION	pi	= { 0 };
	STARTUPINFOA	si	= { sizeof(STARTUPINFOA)	};
	si.wShowWindow	= SW_SHOWNORMAL;
	si.dwFlags		= STARTF_USESHOWWINDOW;

	if(!CreateProcessA(NULL, (LPSTR)szCmdLine, NULL, NULL,FALSE, 0, NULL, NULL, &si, &pi))
	{
		DXUT_ERR(L"CreateProcessA failed for error.", GetLastError());
		return	0;
	}


	BOOL	bFlags	= FALSE;

	while(1)
	{
		HANDLE	hEvents[]	= {pi.hProcess, g_hExitEvent};
		DWORD	dwRet =::WaitForMultipleObjects(2, hEvents, FALSE, 0); 
		//::WaitForSingleObject(pi.hProcess, 0);
		if(dwRet == WAIT_OBJECT_0)	//	进程关闭
		{
			bFlags	= FALSE;
			break;
		}
		else if (dwRet == WAIT_OBJECT_0+1)
			return	0;	//	游戏退出

		if(dwRet == WAIT_TIMEOUT)
		{
			bFlags	= TRUE;
			break;	//	进程还没有启动，继续进行
		}
		//	其他情况则继续等待
		::Sleep(100);	//	100毫秒秒钟后继续
	}

	//	是否需要恢复游戏的全屏
	bool bNeedToRestoreFullscreen	= false;
#ifdef _DEBUG
	WCHAR	szTips[256];
#endif
	//	查找IE窗口，找到后直接将其出现在最上层
	if(bFlags)
	{
		//	如果当前游戏窗口处于全屏状态，则发送切换消息
		//	
#ifdef _DEBUG
		StringCchPrintf(szTips, sizeof(szTips), L"DXUTGetHWND = %p, DXUTIsWindowed = %u, DXUTIsActive = %u", DXUTGetHWND(), DXUTIsWindowed(), DXUTIsActive());
		DXTRACE_MSG(szTips);
#else
		if(!DXUTIsWindowed())
		{
			//	
			::SendMessage(DXUTGetHWND(), WM_COMMAND, VK_F2, 0);
			bNeedToRestoreFullscreen	= true;
		}
#endif	
		::EnumWindows(SearchIEWindow, (LPARAM)&pi);


		//	统计有效点击
		if(g_pfnSearchStateCallback)
			(*g_pfnSearchStateCallback)( tagGUISettings::Instance()->dwGameID, tagGUISettings::Instance()->dwAdID, 0, 0);
	}

	//	等待该IE窗口关闭
	if(bFlags && bNeedToRestoreFullscreen)
	{
		while(1)
		{
			HANDLE	hEvents[]	= {pi.hProcess, g_hExitEvent};
			DWORD	dwRet =::WaitForMultipleObjects(2, hEvents, FALSE, 0); 
			if(dwRet == WAIT_OBJECT_0)	//	IE进程关闭
			{
				//	首先检测游戏是否已经被激活
				if(DXUTIsActive())
					break;	//	退出

				if(DXUTIsWindowed())
				{
					//	如果已经被调整为窗口模式，则也不需要
					break;	//	退出
				}

				//	否则恢复游戏为全屏标志
				::SendMessage(DXUTGetHWND(), WM_SYSCOMMAND, SC_RESTORE, 0);
				break;	//	退出
			}	
			else if(dwRet == WAIT_OBJECT_0 + 1)
				return	0;

			::Sleep(10);
		}
	}

	DXTRACE_MSG(L"OpenIEThreadProc DONE!!!!");

	return	0;

	//////////////////////////////////////////////////////////////////////////

	//OutputDebugStringA("OpenIEThreadProc");
	//LPCSTR	lpszURL	= (LPCSTR)p;
	//assert(!::IsBadStringPtrA(lpszURL, -1));
	//
	////	h获取IE所在路径
	//CHAR szAppName[_MAX_PATH] = "C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE";//IE浏览器程序
	//HKEY	hKey	= NULL;
	//if(::RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE", &hKey) == ERROR_SUCCESS)
	//{
	//	//DXTRACE_MSG(L"22222222222222RegQueryValue		 DONE!!!!");
	//	CHAR	szPathName[_MAX_PATH]	= { 0 };
	//	LONG	dwSize	= _MAX_PATH;
	//	if(ERROR_SUCCESS==::RegQueryValueA(hKey, "", szPathName, &dwSize))
	//	{
	//		StringCchCopyA(szAppName,_MAX_PATH,szPathName);
	//		//DXTRACE_MSG(L"RegQueryValue		 DONE!!!!");
	//	}
	//	::RegCloseKey(hKey);
	//}

	//CHAR szCmdLine[_MAX_PATH]	= { 0 };
	//StringCchPrintfA(szCmdLine, _MAX_PATH, "%s %s", szAppName, lpszURL);

	//
	//PROCESS_INFORMATION	pi	= { 0 };
	//STARTUPINFOA	si	= { sizeof(STARTUPINFOA)	};
	//si.wShowWindow	= SW_SHOWNORMAL;
	//si.dwFlags		= STARTF_USESHOWWINDOW;
	//
	//if(!CreateProcessA(NULL, (LPSTR)szCmdLine, NULL, NULL,FALSE, 0, NULL, NULL, &si, &pi))
	//{
	//	DXUT_ERR(L"CreateProcessA failed for error.", GetLastError());
	//	return	0;
	//}
	//
	//BOOL	bFlags	= FALSE;
	//while(1)
	//{
	//	DWORD	dwRet = ::WaitForSingleObject(pi.hProcess, 0);
	//	if(dwRet == WAIT_OBJECT_0)	//	进程关闭
	//	{
	//		bFlags	= FALSE;
	//		break;
	//	}
	//	if(dwRet == WAIT_TIMEOUT)
	//	{
	//		bFlags	= TRUE;
	//		break;	//	进程还没有启动，继续进行
	//	}
	//	::Sleep(1000);	//	1秒钟后继续
	//}
	//

	////	查找IE窗口，找到后直接将其出现在最上层
	//if(bFlags)
	//{
	//	::EnumWindows(SearchIEWindow, (LPARAM)&pi);

	//	//	统计有效点击
	//	if(g_pfnSearchStateCallback)
	//		(*g_pfnSearchStateCallback)( tagGUISettings::Instance()->dwGameID, tagGUISettings::Instance()->dwAdID, 0, 0);
	//}

	//DXTRACE_MSG(L"OpenIEThreadProc DONE!!!!");

	//return	0L;
}

/************************************************************************/
/*  采用后台线程来打开指定的URL                                         */
/************************************************************************/
BOOL CreateIEThread(LPCSTR lpszURL)
{
	if(g_hExitEvent == NULL)
	{
		g_hExitEvent	= ::CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	DWORD	dwID	= 0;
	HANDLE	hThread		= ::CreateThread(NULL, 0, OpenIEThreadProc, (void*)lpszURL, 0, &dwID);
	if(hThread == NULL)
	{
		::OutputDebugStringA("@#####CreateThread Failed!");
		return	FALSE;
	}
	while(::WaitForSingleObject(hThread, 0) != WAIT_TIMEOUT)
		;
	::CloseHandle(hThread);
	return	TRUE;
}

/************************************************************************/
/* 处理GOOGLE搜索                                                       */
/************************************************************************/
BOOL HandleGoogleSearch(LPCWSTR lpszSearchText)
{
	OutputDebugStringA("HandleGoogleSearch");
	if(lpszSearchText == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return	FALSE;
	}

	int len = ::wcslen(lpszSearchText);
	if(len == 0)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return	FALSE;
	}

	CHAR	szText[INTERNET_MAX_URL_LENGTH]	= { 0 };
	int	nRet	= ::WideCharToMultiByte(CP_ACP, 0, lpszSearchText, -1, szText, sizeof(szText), NULL, NULL);
	szText[nRet]	= 0;
	CHAR	szFix[4]	= { 0 };
	CHAR	szBaseCodeText[INTERNET_MAX_URL_LENGTH]	= { 0 };
	for(int nIndex = 0; nIndex < nRet; nIndex++)
	{
		StringCchPrintfA(szFix, 4, "%s%02X", "%", (BYTE)szText[nIndex]);
		StringCchCatA(szBaseCodeText, INTERNET_MAX_URL_LENGTH, szFix);
	}

	StringCchPrintfA(szText, INTERNET_MAX_URL_LENGTH, "%s%s", IDS_GOOGLE_URLA, szBaseCodeText);

	//OutputDebugStringA(szText);

	return	CreateIEThread( szText );
}

/************************************************************************/
/* 处理百度搜索                                                         */
/************************************************************************/
BOOL HandleBaiduSearch(LPCWSTR lpszSearchText)
{
	OutputDebugStringA("HandleBaiduSearch");
	if(lpszSearchText == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return	FALSE;
	}

	int len = ::wcslen(lpszSearchText);
	if(len == 0)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return	FALSE;
	}
	
	CHAR	szText[INTERNET_MAX_URL_LENGTH]	= { 0 };
	int	nRet	= ::WideCharToMultiByte(CP_ACP, 0, lpszSearchText, -1, szText, sizeof(szText), NULL, NULL);
	szText[nRet]	= 0;
	CHAR	szFix[4]	= { 0 };
	CHAR	szBaseCodeText[INTERNET_MAX_URL_LENGTH]	= { 0 };
	for(int nIndex = 0; nIndex < nRet; nIndex++)
	{
		StringCchPrintfA(szFix, 4, "%s%02X", "%", (BYTE)szText[nIndex]);
		StringCchCatA(szBaseCodeText, INTERNET_MAX_URL_LENGTH, szFix);
	}

	StringCchPrintfA(szText, INTERNET_MAX_URL_LENGTH, "%s%s", IDS_BAIDU_URLA, szBaseCodeText);
	
	//OutputDebugStringA(szText);

	return	CreateIEThread( szText );
}

/************************************************************************/
/* 处理搜索按钮的消息                                                   */
/************************************************************************/
BOOL HandleSearchButtonMsg()
{
	OutputDebugStringA("HandleSearchButtonMsg");
	WCHAR text[260]	=	{ 0 };
	HRESULT		hr	= g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->GetTextCopy( text, 260 );
	if(hr != S_OK )
		return	FALSE;
#ifdef _DEBUG
	::OutputDebugStringW(text);
#endif
	switch(g_nSearchEngine)
	{
	default:	//默认为采用百度的搜索
	case SEARCHENGINE_BAIDU:
		HandleBaiduSearch(text);
		break;
	case SEARCHENGINE_GOOGLE:
		HandleGoogleSearch(text);
		break;
	case SEARCHENGINE_SOUGOO:
		break;
	case SEARCHENGINE_OTHERS:
		break;
	}

	return	FALSE;

}

//-----------------------------------------------------------------------------
// Desc: 处理各种控件消息
//-----------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, 
						 void* pUserContext )
{
	//OutputDebugStringA("OnGUIEvent");
	{
		switch( nControlID )
		{
		case IDC_BUTTON_ENABLE_IME:   //启用IME输入法
			CDXUTIMEEditBox::EnableImeSystem( true );
			break;

		case IDC_BUTTON_DISABLE_IME:  //禁用IME输入法
			CDXUTIMEEditBox::EnableImeSystem( false );
			break;
		case IDC_BUTTON_SEARCH:
			HandleSearchButtonMsg();
			break;
		case IDC_BUTTON_CLOSE:
			{
				//g_bShowSearch = !g_bShowSearch;
				g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->SetVisible(true);
				g_SampleUI.GetButton( IDC_BUTTON_SEARCH )->SetVisible(true);
			}
			break;
		case IDC_IME_EDITBOX:
			{
				switch(nEvent)
				case EVENT_EDITBOX_CHANGE:
					{
						//OutputDebugStringA("D3D9EditSearch OnGUIEvent");
					}
					 break;
			}
			break;
		}
	}
}

//--------------------------------------------------------------------------------------
// Rejects any devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	OutputDebugStringA("IsDeviceAcceptable");
	//检查后台缓冲区格式是否支持Alpha混合等操作(post pixel blending operations)
    IDirect3D9* pD3D = DXUTGetD3DObject(); 
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
	OutputDebugStringA("ModifyDeviceSettings");
  //如果不支持硬件顶点处理则使用软件顶点处理
    if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0)
    {
        pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

	//如果使用参考设备,则弹出警告对话框
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning();
    }

    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3DPOOL_MANAGED resources here 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	OutputDebugStringA("OnCreateDevice");
	HRESULT hr;
	V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3DPOOL_DEFAULT resources here 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	OutputDebugStringA("OnResetDevice");
    HRESULT hr;

	V_RETURN( g_DialogResourceManager.OnResetDevice() );

	//调整按钮控件位置
	g_SampleUI.SetLocation( 0, 0 );
    g_SampleUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

	g_pShowPic->InitPic();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	//OutputDebugStringA("OnFrameMove");

}


//--------------------------------------------------------------------------------------
// Render the scene 
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	//OutputDebugStringA("OnFrameRender");
    HRESULT hr = S_OK;

    // Clear the render target and the zbuffer 
    //V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0) );
    // Render the scene
   // if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	//pd3dDevice->EndScene();

	g_pShowPic->InitTranslationMatrix();
	//bool bFinished = false;
	//LPDIRECT3DTEXTURE9  pTextureSearchOver = g_pFunGetTexture(16, &bFinished);
	//g_pShowPic->SetTexture(pTextureSearchOver);
	g_pShowPic->RenderPic();
	
	//pd3dDevice->BeginScene();

	DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" ); 
	if ( g_pShowPic->GetbShowSearch()  )
	{
		V( g_SampleUI.OnRender( fElapsedTime ) );
	}
	DXUT_EndPerfEvent();


     //V( pd3dDevice->EndScene() );
    
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	//OutputDebugStringA("MsgProc");
	//D3DXVECTOR3 vEyePt( 0.0f, 0.0f, -20 );
	//D3DXVECTOR3 vMin( -5.0f, 6.0f, 0.0f);
	//D3DXVECTOR3 vMax( -3.0f, 8.0f, 2.0f);
	//RAY ray;

	g_pShowPic->MsgProc( hWnd, uMsg, wParam, lParam );

	switch(uMsg)
	{
	case WM_KEYUP:
		{
			if(wParam == tagGUISettings::Instance()->wKey)
			{
				if(g_SampleUI.GetVisible())
				{
					g_SampleUI.SetVisible(false);
				}
				else
				{
					g_SampleUI.SetVisible(true);
				}
				*pbNoFurtherProcessing	= false;
				return	0;
			}
		}
		break;
	}

    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

	*pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

	return 0;
}




//--------------------------------------------------------------------------------------
// Release resources created in the OnResetDevice callback here 
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
	OutputDebugStringA("OnLostDevice");
	g_DialogResourceManager.OnLostDevice();
}


//--------------------------------------------------------------------------------------
// Release resources created in the OnCreateDevice callback here
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
	OutputDebugStringA("OnDestroyDevice");
	g_DialogResourceManager.OnDestroyDevice();
}


/************************************************************************/
/* 从配置文件中获取的颜色值解析为具体的颜色数据                         */
/************************************************************************/
D3DCOLOR ColorFromText(LPCSTR lpszText)
{
	OutputDebugStringA("ColorFromText");
	CHAR	szText[32]	= { 0 };
	strncpy(szText, lpszText, 32);
	const	char szSeparators[]	= ",. |;\r\n";
	DWORD	dwValue[4]	= { 0 };
	int		nIndex	= 0;
	CHAR	*p	= strtok(szText,szSeparators);
	while( p != NULL && nIndex < 4)
	{
		if(lstrlenA(p))
		{
			dwValue[nIndex++] = atoi(p);
		}
		p = strtok(NULL, szSeparators);
	}
	return	D3DCOLOR_ARGB(dwValue[0], dwValue[1], dwValue[2], dwValue[3]);
}

/************************************************************************/
/* 从配置文件中加载GUI的配置信息                                        */
/************************************************************************/
BOOL LoadGUIDefaultFromConfig()
{
	OutputDebugStringA("LoadGUIDefaultFromConfig");
	CHAR	szConfigFile[_MAX_PATH]	= { 0 };
	
	CHAR	szBuffer[512]	= { 0 };
	::GetModuleFileNameA(NULL, szBuffer, _MAX_PATH);
	CHAR	*p	= strrchr(szBuffer, '\\');
	if(p == NULL)
		p= strrchr(szBuffer, '/');
	*++p	=0;
	sprintf(szConfigFile, "%s%s", szBuffer, IDS_MAIN_DEFAULT_CONFIG);
	*szBuffer	= 0;

	::GetPrivateProfileStringA("XGUI", "UIFont1","Comic Sans MS", szBuffer, sizeof(szBuffer), szConfigFile);
	if(lstrlenA(szBuffer))
	{
		::MultiByteToWideChar(CP_ACP, 0, szBuffer, -1, tagGUISettings::Instance()->szFontName1, sizeof(tagGUISettings::Instance()->szFontName1));
		*szBuffer	= 0;
	}

	::GetPrivateProfileStringA("XGUI", "UIFont2","Comic Sans MS", szBuffer, sizeof(szBuffer), szConfigFile);
	if(lstrlenA(szBuffer))
	{
		::MultiByteToWideChar(CP_ACP, 0, szBuffer, -1, tagGUISettings::Instance()->szFontName2, sizeof(tagGUISettings::Instance()->szFontName2));
		*szBuffer	= 0;
	}

	tagGUISettings::Instance()->nFontSize1				= ::GetPrivateProfileIntA("XGUI", "UIFontSize1",24, szConfigFile);
	tagGUISettings::Instance()->nFontSize2				= ::GetPrivateProfileIntA("XGUI", "UIFontSize2",16, szConfigFile);
	tagGUISettings::Instance()->rectSearchEdit.left		= (int)::GetPrivateProfileIntA("XGUI", "SearchEdit.left", 20, szConfigFile);
	tagGUISettings::Instance()->rectSearchEdit.top		= (int)::GetPrivateProfileIntA("XGUI", "SearchEdit.top", 100, szConfigFile);
	tagGUISettings::Instance()->rectSearchEdit.right	= tagGUISettings::Instance()->rectSearchEdit.left +(int)::GetPrivateProfileIntA("XGUI", "SearchEdit.Width", 600, szConfigFile);
	tagGUISettings::Instance()->rectSearchEdit.bottom	= tagGUISettings::Instance()->rectSearchEdit.top + (int)::GetPrivateProfileIntA("XGUI", "SearchEdit.Height", 45, szConfigFile);
	
	::GetPrivateProfileStringA("XGUI", "TextureColor","128, 255, 255, 255", szBuffer, sizeof(szBuffer), szConfigFile);
	if(lstrlenA(szBuffer))
	{
		tagGUISettings::Instance()->clrTexture	= ColorFromText(szBuffer);
		*szBuffer	= 0;
	}
	::GetPrivateProfileStringA("XGUI", "TextColor","255, 64, 64, 64", szBuffer, sizeof(szBuffer), szConfigFile);
	if(lstrlenA(szBuffer))
	{
		tagGUISettings::Instance()->clrText	= ColorFromText(szBuffer);
		*szBuffer	= 0;
	}

	::GetPrivateProfileStringA("XGUI", "CaretColor","255, 64, 64, 64", szBuffer, sizeof(szBuffer), szConfigFile);
	if(lstrlenA(szBuffer))
	{
		tagGUISettings::Instance()->clrCaret	= ColorFromText(szBuffer);
		*szBuffer	= 0;
	}
	::GetPrivateProfileStringA("XGUI", "TextColorSelected","255, 0, 255, 255", szBuffer, sizeof(szBuffer), szConfigFile);
	if(lstrlenA(szBuffer))
	{
		tagGUISettings::Instance()->clrTextSelected	= ColorFromText(szBuffer);
		*szBuffer	= 0;
	}

	::GetPrivateProfileStringA("XGUI", "BackColorSelected","255, 40, 72, 72", szBuffer, sizeof(szBuffer), szConfigFile);
	if(lstrlenA(szBuffer))
	{
		tagGUISettings::Instance()->clrBackSelected	= ColorFromText(szBuffer);
		*szBuffer	= 0;
	}

	tagGUISettings::Instance()->rectSearchButton.left		= (int)::GetPrivateProfileIntA("XGUI", "SearchButton.left", 35, szConfigFile);
	tagGUISettings::Instance()->rectSearchButton.top		= (int)::GetPrivateProfileIntA("XGUI", "SearchButton.top", 160, szConfigFile);
	tagGUISettings::Instance()->rectSearchButton.right	= tagGUISettings::Instance()->rectSearchButton.left +(int)::GetPrivateProfileIntA("XGUI", "SearchButton.Width", 60, szConfigFile);
	tagGUISettings::Instance()->rectSearchButton.bottom	= tagGUISettings::Instance()->rectSearchButton.top + (int)::GetPrivateProfileIntA("XGUI", "SearchButton.Height", 35, szConfigFile);

	tagGUISettings::Instance()->rectShowSearchButton.left		= (int)::GetPrivateProfileIntA("XGUI", "ShowSearchButton.left", 35, szConfigFile);
	tagGUISettings::Instance()->rectShowSearchButton.top		= (int)::GetPrivateProfileIntA("XGUI", "ShowSearchButton.top", 160, szConfigFile);
	tagGUISettings::Instance()->rectShowSearchButton.right		= tagGUISettings::Instance()->rectShowSearchButton.left +(int)::GetPrivateProfileIntA("XGUI", "ShowSearchButton.Width", 60, szConfigFile);
	tagGUISettings::Instance()->rectShowSearchButton.bottom		= tagGUISettings::Instance()->rectShowSearchButton.top + (int)::GetPrivateProfileIntA("XGUI", "ShowSearchButton.Height", 35, szConfigFile);


	g_nSearchEngine = (int)::GetPrivateProfileIntA("XGUI", "SearchType", -1, szConfigFile);
	if(g_nSearchEngine >= SEARCHENGINE_OTHERS)
		g_nSearchEngine	= SEARCHENGINE_OTHERS;
	if(g_nSearchEngine < 0)
		g_nSearchEngine	= SEARCHENGINE_BAIDU;


	::GetPrivateProfileStringA("XGUI", "VirtualKey", "F2", szBuffer, sizeof(szBuffer), szConfigFile);
	if(lstrlenA(szBuffer))
	{
		if(lstrcmpiA(szBuffer, "F1") == 0)
		{
			tagGUISettings::Instance()->wKey	= VK_F1;
		}
		else if(lstrcmpiA(szBuffer, "F2") == 0)
		{
			tagGUISettings::Instance()->wKey	= VK_F2;
		}
		else if(lstrcmpiA(szBuffer, "F3") == 0)
		{
			tagGUISettings::Instance()->wKey	= VK_F3;
		}
		else if(lstrcmpiA(szBuffer, "F4") == 0)
		{
			tagGUISettings::Instance()->wKey	= VK_F4;
		}
		else if (lstrcmpiA(szBuffer, "F5") == 0)
		{
			tagGUISettings::Instance()->wKey	= VK_F5;
		}
		else if (lstrcmpiA(szBuffer, "F6") == 0)
		{
			tagGUISettings::Instance()->wKey	= VK_F6;
		}
	}

	return	TRUE;
}

/************************************************************************/
/* 增加新接口，以支持更多功能                                           */
/************************************************************************/
HRESULT WINAPI GUISDK_InitEx(LPDIRECT3DDEVICE9 pDevice, LPARAM lParam)
{

	OutputDebugStringA("GUISDK_InitEx");
	InitIGATexture9( pDevice );
	setlocale(LC_ALL,"Chinese-simplified");
	XGUI_INITIALIZEPARAM	*pInitializeParam	= (XGUI_INITIALIZEPARAM*)lParam;


	assert(pDevice != NULL);
	assert(pInitializeParam != NULL);

	g_pShowPic = new CShowPic();
	g_pShowPic->SetDevice9( pDevice);
	g_pShowPic->SetGameWnd(pInitializeParam->hWndFocus);

	g_dwGameID = pInitializeParam->dwGameID;

	//g_hGameWnd = pInitializeParam->hWndFocus;

	IDirect3D9*	pD3D9	= NULL;
	if(FAILED(pDevice->GetDirect3D(&pD3D9)))
		return	E_FAIL;

	DXUTSetD3D9(pD3D9);

	// Set the callback functions
	DXUTSetCallbackDeviceCreated( OnCreateDevice );
	DXUTSetCallbackDeviceReset( OnResetDevice );
	DXUTSetCallbackDeviceLost( OnLostDevice );
	DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackFrameRender( OnFrameRender );
	DXUTSetCallbackFrameMove( OnFrameMove );

	g_nSearchEngine	= pInitializeParam->nSearchType;
	if(g_nSearchEngine >= SEARCHENGINE_OTHERS)
		g_nSearchEngine	= SEARCHENGINE_OTHERS;
	if(g_nSearchEngine < 0)
		g_nSearchEngine	= SEARCHENGINE_BAIDU;
	
	tagGUISettings::Instance()->dwSearchType	= g_nSearchEngine;
	tagGUISettings::Instance()->dwGameID		= pInitializeParam->dwGameID;
	tagGUISettings::Instance()->dwAdID			= pInitializeParam->dwAdID;
	g_pfnSearchStateCallback					= pInitializeParam->pfnStatCallback;


	// TODO: Perform any application-level initialization here
	g_SampleUI.Init( &g_DialogResourceManager );

	//为g_SampleUI对话框设置消息处理函数
	g_SampleUI.SetCallback( OnGUIEvent );

	//	从主配置文件加载GUI的主要信息
	LoadGUIDefaultFromConfig();

	//为g_SampleUI对话框设置字体
	g_SampleUI.SetFont( 1, tagGUISettings::Instance()->szFontName1, tagGUISettings::Instance()->nFontSize1, FW_NORMAL );
	g_SampleUI.SetFont( 2, tagGUISettings::Instance()->szFontName2, tagGUISettings::Instance()->nFontSize2, FW_NORMAL );
	CopyMemory(&g_rectIMEEdit, &tagGUISettings::Instance()->rectSearchEdit, sizeof(RECT));
	CopyMemory(&g_rectBtnSearch,&tagGUISettings::Instance()->rectSearchButton, sizeof(RECT));
	CopyMemory(&g_rectBtnShowSearch,&tagGUISettings::Instance()->rectShowSearchButton, sizeof(RECT));

	//添加IME编辑框控件
	CDXUTIMEEditBox *pIMEEdit;
	if( SUCCEEDED( g_SampleUI.AddIMEEditBox( IDC_IME_EDITBOX, IDS_DEFAULT_SEARCHTIPSW, g_rectIMEEdit.left, g_rectIMEEdit.top, g_rectIMEEdit.right - g_rectIMEEdit.left, g_rectIMEEdit.bottom - g_rectIMEEdit.top, false, &pIMEEdit ) ) )
	{
		pIMEEdit->GetElement( 0 )->iFont = 1;
		pIMEEdit->GetElement( 1 )->iFont = 1;
		pIMEEdit->GetElement( 9 )->iFont = 1;
		pIMEEdit->GetElement( 0 )->TextureColor.Init( tagGUISettings::Instance()->clrTexture );  // Transparent center
		pIMEEdit->SetBorderWidth( 3 );
		pIMEEdit->SetTextColor( tagGUISettings::Instance()->clrText );
		pIMEEdit->SetCaretColor( tagGUISettings::Instance()->clrCaret );
		pIMEEdit->SetSelectedTextColor( tagGUISettings::Instance()->clrTextSelected );
		pIMEEdit->SetSelectedBackColor( tagGUISettings::Instance()->clrBackSelected );
		pIMEEdit->SetVisible(true);
		//g_pIMEEdit	= pIMEEdit;
	}


	g_SampleUI.AddButton( IDC_BUTTON_SEARCH, IDS_DEFAULT_SEARCH_TEXT, g_rectBtnSearch.left, g_rectBtnSearch.top, g_rectBtnSearch.right - g_rectBtnSearch.left , g_rectBtnSearch.bottom - g_rectBtnSearch.top );
	g_SampleUI.GetButton( IDC_BUTTON_SEARCH )->SetVisible(true);
	
	if( g_nSearchEngine == SEARCHENGINE_BAIDU )
	{
		g_SampleUI.AddButton( IDC_BUTTON_CLOSE,	L"百度...", g_rectBtnShowSearch.left, g_rectBtnShowSearch.top, g_rectBtnShowSearch.right - g_rectBtnShowSearch.left , g_rectBtnShowSearch.bottom - g_rectBtnShowSearch.top );
	}
	else if( g_nSearchEngine == SEARCHENGINE_GOOGLE )
	{
		g_SampleUI.AddButton( IDC_BUTTON_CLOSE,	L"google...", g_rectBtnShowSearch.left, g_rectBtnShowSearch.top, g_rectBtnShowSearch.right - g_rectBtnShowSearch.left , g_rectBtnShowSearch.bottom - g_rectBtnShowSearch.top );
	}

	//g_SampleUI.SetTexture( 10, L"E:\\Whoyo\test.bmp" );
	// Initialize DXUT and create the desired Win32 window and Direct3D device for the application
	DXUTInit( true, true, true );			// Parse the command line, handle the default hotkeys, and show msgboxes

	DXUTSetCursorSettings( true, true );	// Show the cursor and clip it when in full screen
	DXUTSetWindow( pInitializeParam->hWndFocus, pInitializeParam->hWndDeviceFullscreen, pInitializeParam->hWndDeviceWindowed, true );
	return	DXUTSetDevice(pDevice);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
HRESULT WINAPI GUISDK_Init( HWND hWnd , LPWNDPROC Proc, LPDIRECT3DDEVICE9 pd3dDevice )
{
	OutputDebugStringA("GUISDK_Init");
	assert(::IsWindow(hWnd));
	assert(pd3dDevice != NULL);

	g_pShowPic = new CShowPic();
	g_pShowPic->SetDevice9( pd3dDevice);
	g_pShowPic->SetGameWnd(hWnd);

	IDirect3D9*	pD3D9	= NULL;
	if(FAILED(pd3dDevice->GetDirect3D(&pD3D9)))
		return	E_FAIL;
	//DXUTSetD3D9(pD3D9);

	g_pFunWinProc = Proc;

    // Set the callback functions
    DXUTSetCallbackDeviceCreated( OnCreateDevice );
    DXUTSetCallbackDeviceReset( OnResetDevice );
    DXUTSetCallbackDeviceLost( OnLostDevice );
    DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameRender( OnFrameRender );
    DXUTSetCallbackFrameMove( OnFrameMove );
   

	// TODO: Perform any application-level initialization here
	g_SampleUI.Init( &g_DialogResourceManager );
	
	//为g_SampleUI对话框设置消息处理函数
	g_SampleUI.SetCallback( OnGUIEvent );
	
	//	从主配置文件加载GUI的主要信息
	LoadGUIDefaultFromConfig();

	//为g_SampleUI对话框设置字体
    g_SampleUI.SetFont( 1, tagGUISettings::Instance()->szFontName1, tagGUISettings::Instance()->nFontSize1, FW_NORMAL );
	g_SampleUI.SetFont( 2, tagGUISettings::Instance()->szFontName2, tagGUISettings::Instance()->nFontSize2, FW_NORMAL );
	CopyMemory(&g_rectIMEEdit, &tagGUISettings::Instance()->rectSearchEdit, sizeof(RECT));
	CopyMemory(&g_rectBtnSearch,&tagGUISettings::Instance()->rectSearchButton, sizeof(RECT));
	CopyMemory(&g_rectBtnShowSearch,&tagGUISettings::Instance()->rectShowSearchButton, sizeof(RECT));


	//添加IME编辑框控件
	CDXUTIMEEditBox *pIMEEdit;
	if( SUCCEEDED( g_SampleUI.AddIMEEditBox( IDC_IME_EDITBOX, IDS_DEFAULT_SEARCHTIPSW, g_rectIMEEdit.left, g_rectIMEEdit.top, g_rectIMEEdit.right - g_rectIMEEdit.left, g_rectIMEEdit.bottom - g_rectIMEEdit.top, false, &pIMEEdit ) ) )
    {
        pIMEEdit->GetElement( 0 )->iFont = 1;
        pIMEEdit->GetElement( 1 )->iFont = 1;
        pIMEEdit->GetElement( 9 )->iFont = 1;
		pIMEEdit->GetElement( 0 )->TextureColor.Init( tagGUISettings::Instance()->clrTexture );  // Transparent center
        pIMEEdit->SetBorderWidth( 3 );
        pIMEEdit->SetTextColor( tagGUISettings::Instance()->clrText );
        pIMEEdit->SetCaretColor( tagGUISettings::Instance()->clrCaret );
        pIMEEdit->SetSelectedTextColor( tagGUISettings::Instance()->clrTextSelected );
        pIMEEdit->SetSelectedBackColor( tagGUISettings::Instance()->clrBackSelected );
		
		//g_pIMEEdit	= pIMEEdit;
	}

	g_SampleUI.AddButton( IDC_BUTTON_SEARCH, IDS_DEFAULT_SEARCH_TEXT, g_rectBtnSearch.left, g_rectBtnSearch.top, g_rectBtnSearch.right - g_rectBtnSearch.left , g_rectBtnSearch.bottom - g_rectBtnSearch.top );
	g_SampleUI.GetButton( IDC_BUTTON_SEARCH )->SetVisible(false);

	g_SampleUI.AddButton( IDC_BUTTON_CLOSE,	L"百度...", g_rectBtnShowSearch.left, g_rectBtnShowSearch.top, g_rectBtnShowSearch.right - g_rectBtnShowSearch.left , g_rectBtnShowSearch.bottom - g_rectBtnShowSearch.top );
    
	// Initialize DXUT and create the desired Win32 window and Direct3D device for the application
	DXUTInit( true, true, true );			// Parse the command line, handle the default hotkeys, and show msgboxes

	DXUTSetCursorSettings( true, true );	// Show the cursor and clip it when in full screen
    DXUTSetWindow( hWnd, hWnd, hWnd, true );
	return	DXUTSetDevice(pd3dDevice);
	//SetState( true );
	//return 	hr;
}

void WINAPI GUISDK_Render()
{
	//OutputDebugStringA("GUISDK_Render");
	DXUTRender3DEnvironment();
}

void WINAPI GUISDK_Destory()
{
	OutputDebugStringA("GUISDK_Destory");

	g_pFunDestoryTexture();
	if ( g_pShowPic != NULL )
	{
		delete g_pShowPic;
		g_pShowPic = NULL; 
	}
	
	OutputDebugStringA("GUISDK_Destory");

	DXUTShutdown(0);
}
