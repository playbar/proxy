//--------------------------------------------------------------------------------------
// File: D3D9Search.cpp
//

#include <d3d9.h>
#include <commctrl.h> // for InitCommonControls() 
#include <stdio.h>
#include "ShowPic.h"
#include "DXUTState.h"
#include "DXUTgui_My.h"
#include "GlobalFunction.h"
#include "DefineConst.h"
#include "D3D9Search_My.h"

CDXUTDialogResourceManager	g_DialogResourceManager;//对话框资源管理器
CDXUTDialog					g_SampleUI;             //对话框

LPWNDPROC					g_pFunOldMsgProc = NULL;		//存放SetWindowLong消息处理函数地址 
LPCallBack3DFocus			g_pFun3DFocus = NULL;		//点击统计次数的函数指针，通过导出接口获得
DWORD						g_dwAdID = 0;			//广告模型ID，通过导出接口获得


HWND						g_hGameWnd = NULL;		//游戏窗口句柄，通过导出接口获得
DWORD						g_dwGameID = 0;			//游戏ID
D3D9SEARCHSETTINGS			g_D3D9SearchSetting;		 //自定义变量，存放加载变量和通过接口获取变量

char						g_szSearchURLHead[MAX_PATH]; //搜索url链接地址，从search.ini文件读取
TCHAR						g_wszDefaultText[MAX_PATH];
TCHAR						g_wszB_text[50];				 //搜索按钮显示字体，从Search.ini文件读取

RECT						g_rectIMEEdit;			//	输入框的大小
RECT						g_rectBtnSearch;		//	搜索按钮的大小
BOOL						g_bIsWindow = FALSE;


CHookerMsg		g_HookerMsg;
HANDLE			g_hExitEvent	= NULL;

//IDirect3DDevice9 * g_pDirect3DDevice;


void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, 
						 void* pUserContext )
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
		if (  g_pShowPic->GetbShowSearch()  )
		{
			g_pFun3DFocus( g_dwAdID, NULL );
			HandleSearchButtonMsg();
		}
		break;
	case IDC_BUTTON_CLOSE:
		{
			g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->SetVisible(true);
			g_SampleUI.GetButton( IDC_BUTTON_SEARCH )->SetVisible(true);
		}
		break;
	case IDC_IME_EDITBOX:
		{
			switch(nEvent)
			case EVENT_EDITBOX_CHANGE:
				//g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->UpdateRects();
			break;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK MsgProcProxy( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//if ( wParam == 0x5001 )
	//{
	//	return S_OK;
	//	//WM_IME_STARTCOMPOSITION
	//	//wParam = 229;
	//	PostMessage( hWnd, WM_IME_STARTCOMPOSITION, 0, 0 );
	//	SHOWOUTPUTDEBUG(lParam);
	//}

	//if ( uMsg == WM_INPUTLANGCHANGEREQUEST  )
	//{
	//	SHOWOUTPUTDEBUG( MsgProcProxy_WM_INPUTLANGCHANGEREQUEST );
	//}
	//if ( uMsg == WM_INPUTLANGCHANGE)
	//{
	//	SHOWOUTPUTDEBUG( MsgProcProxy_WM_INPUTLANGCHANGE );
	//}
	//if (uMsg == WM_IME_NOTIFY )
	//{
	//	SHOWOUTPUTDEBUG( MsgProcProxy_WM_IME_NOTIFY );
	//}
	//if ( uMsg == WM_IME_COMPOSITION )
	//{
	//	SHOWOUTPUTDEBUG( MsgProcProxy_WM_IME_COMPOSITION );
	//}
	//if ( uMsg == WM_IME_SETCONTEXT )
	//{
	//	SHOWOUTPUTDEBUG( MsgProcProxy_WM_IME_SETCONTEXT );
	//}
	//if ( uMsg == WM_IME_STARTCOMPOSITION )
	//{
	//	SHOWOUTPUTDEBUG( MsgProcProxy_WM_IME_STARTCOMPOSITION );
	//}
	//if ( uMsg == WM_IME_ENDCOMPOSITION )
	//{
	//	SHOWOUTPUTDEBUG( MsgProcProxy_WM_IME_ENDCOMPOSITION );
	//}

	////////////////////////////////////////////////////////////////////////////
	////封印英文输入替换
	//if ( wParam == 'h'  )
	//{
	//	wParam = g_wParam + 32;
	//}
	//if ( wParam == 'H')
	//{
	//	wParam = g_wParam;
	//}

	//////中文输入替换
	//if ( wParam == 'g')
	//{
	//	wParam = g_wParam;
	//}
	//if ( wParam == 'G')
	//{
	//	wParam = g_wParam;
	//}

	if ( uMsg == WM_KEYUP)
	{
		SHOWOUTPUTDEBUG( MsgProcProxy_WM_KEYUP);
	}
	try
	{
		if ( g_pShowPic->MsgProc( hWnd, uMsg, wParam, lParam ) )
		{
			int iyPox =  g_pShowPic->GetiHeight() / 2 - g_D3D9SearchSetting.iSearchBoxHeight / 2;
			g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->SetLocation(g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() + 2, g_pShowPic->GetiyPos() + iyPox );
			g_SampleUI.GetButton( IDC_BUTTON_SEARCH )->SetLocation(g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() + 2 + g_D3D9SearchSetting.iSearchBoxWidth , g_pShowPic->GetiyPos() + iyPox );
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}

		if ( g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam ) )
		{
			return 0;
		}

		if (g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam ))
		{
			return 0;
		}

		if ( ! g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->m_bHasFocus && g_pFunOldMsgProc != NULL )
		{
			return g_pFunOldMsgProc( hWnd, uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}
	}
	catch (...)
	{
		SHOWOUTPUTDEBUG(MsgProcProxy_Catch);
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

}

LRESULT CALLBACK HookGetMsg(int code, WPARAM wParam, LPARAM lParam)
{
	//SHOWOUTPUTDEBUG( HookGetMsg_begin );
	MSG	 *lpMsg	= (MSG*)lParam;

	//if ( lpMsg->message == WM_KEYDOWN )
	//{
	//	SHOWOUTPUTDEBUG( HookGetMsg_WM_KEYDOWN );
	//}

	//if(code >= 0 && g_HookerMsg.GetWndHook() == lpMsg->hwnd )
	{
		if (  g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->m_bHasFocus  )
		{
			if( lpMsg->message == WM_KEYDOWN )
			{
				SHOWOUTPUTDEBUG( HookGetMsg_WM_KEYDOWN );
				lpMsg->message = WM_KEYUP;
				MsgProcProxy( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );
				////屏蔽封印游戏快捷键
				//if (lpMsg->wParam == 'I' || 
				//	lpMsg->wParam == 'F' || 
				//	lpMsg->wParam == 'M' ||
				//	lpMsg->wParam == 'O' ||
				//	lpMsg->wParam == 'Q'  )
				//{
				//	g_bParam = true;
				//	g_wParam = lpMsg->wParam;
				//	lpMsg->wParam = 'H';
				//}
				//else if ( lpMsg->wParam == 81  )
				//{
				//	g_bParam = true;
				//	g_wParam = lpMsg->wParam;
				//	lpMsg->wParam = 'G';
				//}
				//else
				//{
				//	g_wParam = lpMsg->wParam;
				//}
			}
			//////////////////////////////////////////////////////////////////////////


			//if ( s_bReplace )
			//{
			//	//lpMsg->message	= s_uMsg;
			//	//lpMsg->wParam	= s_wParam;
			//	//lpMsg->lParam	= s_lParam;
			//	s_bReplace = false;
			//	goto GOTOMSG;

			//}
			//if ( lpMsg->message == WM_KEYDOWN  && lpMsg->wParam == 229  )
			//{
			//	s_bReplace = true;
			//	s_uMsg = lpMsg->message;
			//	//s_wParam = lpMsg->wParam;
			//	//s_lParam = lpMsg->lParam;

			//	lpMsg->message = WM_KEYUP;
			//	//lpMsg->wParam = 0x5001;
			//	//lpMsg->lParam  = 0x5002;

			//}
		
			/*if ( lpMsg->message == WM_INPUTLANGCHANGEREQUEST  )
			{
				SHOWOUTPUTDEBUG( HookGetMsg_WM_INPUTLANGCHANGEREQUEST );
			}
			if ( lpMsg->message == WM_INPUTLANGCHANGE)
			{
				SHOWOUTPUTDEBUG( HookGetMsg_WM_INPUTLANGCHANGE );
			}
			if (lpMsg->message == WM_IME_NOTIFY )
			{
				SHOWOUTPUTDEBUG( HookGetMsg_WM_IME_NOTIFY );
			}
			if ( lpMsg->message == WM_IME_COMPOSITION )
			{
				SHOWOUTPUTDEBUG( HookGetMsg_WM_IME_COMPOSITION );
			}
			if ( lpMsg->message == WM_IME_SETCONTEXT )
			{
				SHOWOUTPUTDEBUG( HookGetMsg_WM_IME_SETCONTEXT );
			}
			if ( lpMsg->message == WM_IME_STARTCOMPOSITION )
			{
				SHOWOUTPUTDEBUG( HookGetMsg_WM_IME_STARTCOMPOSITION );
			}
			if ( lpMsg->message == WM_IME_ENDCOMPOSITION )
			{
				SHOWOUTPUTDEBUG( HookGetMsg_WM_IME_ENDCOMPOSITION );
			}*/
		}

	/*	MsgProcProxy( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );
		if ( lpMsg->wParam == 229 )
		{
			lpMsg->wParam = 0;
			lpMsg->wParam = 229;
		}*/

		//if (  g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->m_bHasFocus  ) //'i'
		//{
		//	//if( lpMsg->message == WM_KEYDOWN )
		//	if (lpMsg->wParam ==  'I' )
		//	{
		//		g_bParam = true;
		//		g_wParam = lpMsg->wParam;
		//		lpMsg->wParam = 'F';
		//	}
		//}
		//else
		//{
		//	g_bParam = false;
		//}
		

		//return DefWindowProc( lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam );

		////	主要是需要过滤掉WM_KEYDOWN WM_KEYUP这两个消息中的一个
		////	以避免在输入汉字的时候出现重复输入, 这里保留WM_KEYUP，过滤掉WM_KEYDOWN
		//if( lpMsg->message == WM_PAINT
		//	|| lpMsg->message == WM_ACTIVATEAPP
		//	|| lpMsg->message == WM_DESTROY 
		//	|| lpMsg->message == WM_KEYUP 
		//	|| lpMsg->wParam == WM_SYSKEYUP 
		//	|| lpMsg->message == WM_CHAR 
		//	|| lpMsg->message == WM_SYSCHAR 
		//	|| lpMsg->message == WM_DEADCHAR
		//	|| lpMsg->message == WM_SYSCOMMAND
		//	|| lpMsg->message == WM_INPUTLANGCHANGE
		//	||  (lpMsg->message >= WM_MOUSEFIRST && lpMsg->message <= WM_MOUSELAST))
		//{	
		//	//LRESULT	lResult	= DXUTStaticWndProc(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
		//	LRESULT lResult = MsgProcProxy(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
		//	if(lResult)
		//		return	1;
		//}
	}
	//g_wParam = lpMsg->wParam;

	return S_OK;
	//return	CallNextHookEx(g_HookerMsg.GetHook(), code, wParam, lParam);

}



/************************************************************************/
/* 增加新接口，以支持更多功能                                           */
/************************************************************************/
HRESULT WINAPI D3D9SearchInit(LPDIRECT3DDEVICE9 pDevice, LPARAM lParam)
{

	//g_pDirect3DDevice = pDevice;
	
	//if( FAILED( pDevice->TestCooperativeLevel()) )
	//{
	//	SHOWOUTPUTDEBUG( D3D9SearchInit_TestCooperativeLevel);
	//	return E_FAIL;
	//}

	//SHOWOUTPUTDEBUG( D3D9SearchInit_Begin );

	D3DSEARCH_PARAM	*pInitializeParam	= (D3DSEARCH_PARAM*)lParam;

	g_pFun3DFocus	= pInitializeParam->pFun3DFocus;
	g_dwAdID		= pInitializeParam->dwAdID;
	g_hGameWnd		= pInitializeParam->hWndFocus;
	g_dwGameID		= pInitializeParam->dwGameID;

	//long lref1 = g_pDirect3DDevice->AddRef() -1;

	if ( g_pShowPic != NULL )
	{
		delete g_pShowPic;
		g_pShowPic = NULL; 
	}
	g_pShowPic = new CShowPic();
	g_pShowPic->SetDevice9( pDevice);
	g_pShowPic->SetGameWnd( pInitializeParam->hWndFocus );
	g_pShowPic->InitPic( pDevice );

	g_DialogResourceManager.OnCreateDevice( pDevice );

	g_SampleUI.Init( &g_DialogResourceManager );
	g_SampleUI.SetCallback( OnGUIEvent );


	LoadGUIDefaultFromConfig();

	//为g_SampleUI对话框设置字体
	g_SampleUI.SetFont( 1, g_D3D9SearchSetting.szFontName1, g_D3D9SearchSetting.nFontSize1, FW_NORMAL );

	int iyPox =  g_pShowPic->GetiHeight() / 2 - g_D3D9SearchSetting.iSearchBoxHeight / 2;
	g_rectIMEEdit.left	= g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() +2;
	g_rectIMEEdit.top	= g_pShowPic->GetiyPos() + iyPox;
	g_rectIMEEdit.right = g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() + 2 + g_D3D9SearchSetting.iSearchBoxWidth;
	g_rectIMEEdit.bottom= g_pShowPic->GetiyPos() + iyPox + g_D3D9SearchSetting.iSearchBoxHeight; 

	g_rectBtnSearch.left	= g_pShowPic->GetixPos() + g_pShowPic->GetiWidth() + 2 + g_D3D9SearchSetting.iSearchBoxWidth ;
	g_rectBtnSearch.top		= g_pShowPic->GetiyPos();
	g_rectBtnSearch.right	= g_pShowPic->GetixPos() + 2 + g_pShowPic->GetiWidth() + g_D3D9SearchSetting.iSearchBoxWidth + g_D3D9SearchSetting.iSearchBtnWidth;
	g_rectBtnSearch.bottom	= g_pShowPic->GetiyPos() + g_D3D9SearchSetting.iSearchBtnHeight; 

	CDXUTIMEEditBox *pIMEEdit = NULL;
	if( SUCCEEDED( g_SampleUI.AddIMEEditBox( IDC_IME_EDITBOX, g_wszDefaultText, g_rectIMEEdit.left, g_rectIMEEdit.top, g_D3D9SearchSetting.iSearchBoxWidth, g_D3D9SearchSetting.iSearchBoxHeight, false, &pIMEEdit ) ) )
	//if( SUCCEEDED( g_SampleUI.AddIMEEditBox( IDC_IME_EDITBOX, TEXT("Music Search"), 300, 100, 200, 50, false, &pIMEEdit ) ) )
	{
		pIMEEdit->GetElement( 0 )->iFont = 1;
		pIMEEdit->GetElement( 1 )->iFont = 1;
		pIMEEdit->GetElement( 9 )->iFont = 1;
		pIMEEdit->GetElement( 0 )->TextureColor.Init( 0x88888888 );  // Transparent center， 背景颜色
		pIMEEdit->SetBorderWidth( 3 );
		pIMEEdit->SetTextColor( g_D3D9SearchSetting.clrText );
		pIMEEdit->SetCaretColor( g_D3D9SearchSetting.clrCaret );
		pIMEEdit->SetSelectedTextColor( g_D3D9SearchSetting.clrTextSelected );
		pIMEEdit->SetSelectedBackColor( g_D3D9SearchSetting.clrBackSelected );
		pIMEEdit->SetVisible(true);
	}
	g_SampleUI.AddButton( IDC_BUTTON_SEARCH, g_wszB_text, g_rectBtnSearch.left, g_rectBtnSearch.top,g_D3D9SearchSetting.iSearchBtnWidth , g_D3D9SearchSetting.iSearchBtnHeight);
	g_SampleUI.GetButton( IDC_BUTTON_SEARCH )->SetVisible(true);


	//long lref2 = g_pDirect3DDevice->Release();
	//long lrf = lref2 - lref1;

	//if ( IGA_FENGYICHUANSHUO == g_dwGameID )
	//{
		g_HookerMsg.SetWndHook( g_hGameWnd );
		if( g_HookerMsg.GetHook() == NULL)
		{
			HHOOK hHook	= SetWindowsHookExW( WH_GETMESSAGE, HookGetMsg, NULL, ::GetCurrentThreadId());
			if(hHook != NULL)
			{
				g_HookerMsg.SetHook( hHook );
			}
			else
			{
				DXUT_ERR(L"SetWindowsHookEx", ::GetLastError());
			}
		}
	//}

	if ( g_pFunOldMsgProc == NULL )
	{
		if(IsWindowUnicode(g_hGameWnd))
		{
			g_pFunOldMsgProc = (LPWNDPROC)SetWindowLongW( g_hGameWnd, GWL_WNDPROC, (LONG)MsgProcProxy );
		}
		else
		{
			g_pFunOldMsgProc = (LPWNDPROC)SetWindowLongA( g_hGameWnd, GWL_WNDPROC, (LONG)MsgProcProxy );
		}
	}

	//SHOWOUTPUTDEBUG( D3D9SearchInit_End );

	return S_OK;
}



void WINAPI D3D9SearchRender()
{
	try
	{
		if ( g_pShowPic != NULL )
		{
			g_pShowPic->InitTranslationMatrix();
			g_pShowPic->RenderPic();
			if ( g_pShowPic->GetbShowSearch()  )
			{
				g_SampleUI.OnRender( 1 ) ;
			}
		}
	}
	catch (...)
	{
		//SHOWOUTPUTDEBUG(D3D9SearchRender_Catch );
	}

}

void  WINAPI D3D9SearchDestory()
{

	//SHOWOUTPUTDEBUG( D3D9SearchDestory_Begin );


	if ( g_pFunOldMsgProc != NULL)
	{
		if(IsWindowUnicode(g_hGameWnd) )
		{
			SetWindowLongW( g_hGameWnd, GWL_WNDPROC, (LONG)g_pFunOldMsgProc );
		}
		else
		{
			SetWindowLongA( g_hGameWnd, GWL_WNDPROC, (LONG)g_pFunOldMsgProc );
		}
		g_pFunOldMsgProc = NULL;
	}

	//long lref1 = g_pDirect3DDevice->AddRef() -1;

	g_SampleUI.RemoveAllControls();

	g_DialogResourceManager.OnDestroyDevice();

	if ( g_pShowPic != NULL )
	{
		delete g_pShowPic;
		g_pShowPic = NULL; 
	}

	//long lref2 = g_pDirect3DDevice->Release();
	//long lrf = lref2 - lref1;
	return;

}


LRESULT CALLBACK CHookerMsg::HookWindowProc(int code, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT	*lpMsg	= (CWPSTRUCT*)lParam;
	if(m_hWndHook == lpMsg->hwnd )
	{
		;//DXUTStaticWndProc(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
	}

	return	::CallNextHookEx(m_hHook, code, wParam, lParam);
}


/************************************************************************/
/* 从配置文件中获取的颜色值解析为具体的颜色数据                         */
/************************************************************************/
D3DCOLOR ColorFromText(LPCSTR lpszText)
{
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
	CHAR szConfigFile[_MAX_PATH]	= { 0 };
	CHAR szBuffer[512]	= { 0 };

	::GetModuleFileNameA(NULL, szBuffer, _MAX_PATH);
	CHAR * p = strrchr(szBuffer, '\\');
	if(p == NULL)
		p = strrchr(szBuffer, '/');
	*++p = 0;
	sprintf(szConfigFile, "%s%s%s", szBuffer, ADINFO_EMMOA_RES_FLASH, "search.ini");

	*szBuffer	= 0;

	MultiByteToWideChar(CP_ACP, 0, "宋体", -1, g_D3D9SearchSetting.szFontName1, sizeof(g_D3D9SearchSetting.szFontName1));
	g_D3D9SearchSetting.nFontSize1	= 16;			

	GetPrivateProfileStringA("SearchBox", "BackColorARGB","128, 255, 255, 255", szBuffer, sizeof(szBuffer), szConfigFile);
	g_D3D9SearchSetting.clrTexture	= ColorFromText( szBuffer );	
	g_D3D9SearchSetting.clrText	= ColorFromText("255, 64, 64, 64");
	g_D3D9SearchSetting.clrCaret	= ColorFromText("255, 64, 64, 64");
	g_D3D9SearchSetting.clrTextSelected	= ColorFromText("255, 0, 255, 255");
	g_D3D9SearchSetting.clrBackSelected	= ColorFromText("255, 72, 72, 72");

	GetPrivateProfileStringA("url", "SearchURLHead", "http://www.baidu.com/s?wd=", g_szSearchURLHead, sizeof(g_szSearchURLHead), szConfigFile);
	char szBtnText[MAX_PATH];
	memset(g_wszB_text, 0, sizeof(g_wszB_text) );
	GetPrivateProfileStringA("SearchBox", "b_text", "Search", szBtnText, sizeof(szBtnText), szConfigFile);
	MultiByteToWideChar( CP_UTF8,0, szBtnText, strlen(szBtnText)+1, g_wszB_text,	MAX_PATH );

	char szDefaultText[MAX_PATH];
	memset(g_wszDefaultText, 0, sizeof(g_wszDefaultText) );
	GetPrivateProfileStringA("SearchBox", "DefaultText", "歌曲搜索", szDefaultText,sizeof(szDefaultText), szConfigFile);
	MultiByteToWideChar( CP_UTF8,0, szDefaultText, strlen(szDefaultText)+1, g_wszDefaultText,	MAX_PATH );


	g_D3D9SearchSetting.iSearchBoxWidth = GetPrivateProfileIntA("SearchBox", "width",320, szConfigFile);
	g_D3D9SearchSetting.iSearchBoxHeight = GetPrivateProfileIntA("SearchBox", "height",50, szConfigFile);
	g_D3D9SearchSetting.iSearchBtnWidth = GetPrivateProfileIntA("SearchBox", "b_width",60, szConfigFile);
	g_D3D9SearchSetting.iSearchBtnHeight = GetPrivateProfileIntA("SearchBox", "b_height",50, szConfigFile);

	return	TRUE;

}

DWORD WINAPI OpenIEBrowserThreadProc(void *p)
{
	tagThreadParam	*pThreadParam	= (tagThreadParam*)p;

	CHAR	szFileName[_MAX_PATH]	= { 0 };
	GetModuleFileNameA(NULL, szFileName, _MAX_PATH);
	CHAR	*q	= strrchr(szFileName, '\\');
	if(q == NULL)
		q	= strrchr(szFileName, '/');
	*++q	= 0;
	CHAR	szModule[_MAX_PATH]	= { 0 };
	wsprintfA(szModule, "%s%s", szFileName, "EmmoaIE.exe");
	HANDLE	h	= ::CreateFileA(szModule, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h == INVALID_HANDLE_VALUE)
	{
		*szModule	= 0;
		HKEY	hKey	= NULL;
		if(::RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\EMMOA\\IGXExplorer", &hKey) == ERROR_SUCCESS)
		{
			long	dwSize	= _MAX_PATH;
			::RegQueryValueA(hKey, "", szModule, &dwSize);
			::RegCloseKey(hKey);
		}
	}
	else
	{
		::CloseHandle(h);
	}

	if(lstrlenA(szModule))
	{
		CHAR szCmdLine[INTERNET_MAX_URL_LENGTH]	= { 0 };
		wsprintfA(szCmdLine, "%s %s",szModule, pThreadParam->szURL);
		PROCESS_INFORMATION	pi	= { 0 };
		STARTUPINFOA	si	= { sizeof(STARTUPINFOA)	};
		si.wShowWindow	= SW_SHOWNORMAL;
		si.dwFlags		= STARTF_USESHOWWINDOW;

		if(!CreateProcessA(NULL, (LPSTR)szCmdLine, NULL, NULL,FALSE, 0, NULL, NULL, &si, &pi))
		{
			//	DXUT_ERR(L"CreateProcessA failed for error.", GetLastError());
			goto __Exit;
		}
	}

__Exit:
	::HeapFree(::GetProcessHeap(), 0, pThreadParam);
	return	0;
}

/************************************************************************/
/* 处理搜索按钮的消息                                                   */
/************************************************************************/
BOOL HandleSearchButtonMsg()
{
	//OutputDebugStringA("HandleSearchButtonMsg");
	WCHAR lpszSearchText[ INTERNET_MAX_URL_LENGTH ]	=	{ 0 };
	HRESULT		hr	= g_SampleUI.GetIMEEditBox(IDC_IME_EDITBOX)->GetTextCopy( lpszSearchText, INTERNET_MAX_URL_LENGTH );
	if(hr != S_OK )
		return	FALSE;
	//////////////////////////////////////////////////////////////////////////
	if(lpszSearchText == NULL)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return	FALSE;
	}

	int len = ::wcslen(lpszSearchText);
	//if(len == 0)
	//{
	//	::SetLastError(ERROR_INVALID_PARAMETER);
	//	return	FALSE;
	//}
	char	szText[INTERNET_MAX_URL_LENGTH]	= { 0 };
	int	nRet	= ::WideCharToMultiByte(CP_ACP, 0, lpszSearchText, -1, szText, sizeof(szText), NULL, NULL);
	szText[nRet]	= 0;
	CHAR	szFix[4]	= { 0 };
	CHAR	szBaseCodeText[INTERNET_MAX_URL_LENGTH]	= { 0 };
	for(int nIndex = 0; nIndex < nRet; nIndex++)
	{
		StringCchPrintfA(szFix, 4, "%s%02X", "%", (BYTE)szText[nIndex]);
		StringCchCatA(szBaseCodeText, INTERNET_MAX_URL_LENGTH, szFix);
	}
	StringCchPrintfA(szText, INTERNET_MAX_URL_LENGTH, "%s%s", g_szSearchURLHead, szBaseCodeText);
	//////////////////////////////////////////////////////////////////////////
	tagThreadParam	*pThreadParam	= (tagThreadParam*)HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(tagThreadParam));
	pThreadParam->hWndToMonitor	= g_hGameWnd;
	strcpy(pThreadParam->szURL, szText );
	DWORD	dwID	= 0;
	HANDLE	h = ::CreateThread(NULL, 0, OpenIEBrowserThreadProc, (void*)pThreadParam, 0, &dwID);
	::CloseHandle(h);

	return	true;

}


BOOL IsWindows(LPDIRECT3DDEVICE9 pDevice)
{
	bool bd3dpp = false;
	D3DPRESENT_PARAMETERS d3dpp;
	IDirect3DSwapChain9* pSwapChain = NULL;
	if( SUCCEEDED( pDevice->GetSwapChain( 0, &pSwapChain ) ) )
	{
		if( SUCCEEDED( pSwapChain->GetPresentParameters( &d3dpp ) ) )
		{
			bd3dpp = d3dpp.Windowed;
		}
		pSwapChain->Release();
	}
	return bd3dpp;
}


