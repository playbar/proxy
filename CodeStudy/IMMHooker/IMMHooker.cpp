// IMMHooker.cpp : Defines the entry point for the DLL application.
//

#include "StdAfx.h"
#include "IMMHooker.h"

typedef DWORD (WINAPI *LPFN_ImeConversionList)(HIMC hIMC,LPCTSTR lpszSrc,DWORD uBufLen,UINT uFlag);
typedef BOOL (WINAPI *LPFN_ImeConfigure)(HKL hKL,HWND hAppWnd,DWORD dwMode,LPVOID  lpData);
// typedef DWORD (WINAPI *LPFN_ImeConversionList)(HIMC hIMC,LPCTSTR lpszSrc,DWORD uBufLen,UINT uFlag);
typedef BOOL (WINAPI *LPFN_ImeDestroy)(UINT uReserved);
typedef UINT (WINAPI *LPFN_ImeEnumRegisterWord)(REGISTERWORDENUMPROC lpfnRegisterWordEnumProc,LPCTSTR lpszReading,DWORD dwStyle,LPCTSTR lpszString,LPVOID lpData);
typedef LRESULT (WINAPI *LPFN_ImeEscape)(HIMC hIMC,UINT uSubFunc,LPVOID lpData);
typedef UINT (WINAPI *LPFN_ImeGetRegisterWordStyle)(UINT nItem,LPSTYLEBUF lpStyleBuf);
typedef BOOL (WINAPI *LPFN_ImeInquire)(IMEINFO* lpImeInfo,LPTSTR lpszWndCls,DWORD lpszOptions);
typedef BOOL (WINAPI *LPFN_ImeProcessKey)(HIMC hIMC,UINT uVirtKey,LPARAM lParam,CONST LPBYTE lpbKeyState);
typedef BOOL (WINAPI *LPFN_ImeRegisterWord)(LPCTSTR lpszReading,DWORD dwStyle,LPCTSTR lpszString);
typedef BOOL (WINAPI *LPFN_ImeSelect)(HIMC hIMC,BOOL fSelect);
typedef BOOL (WINAPI *LPFN_ImeSetActiveContext)(HIMC hIMC,BOOL fOn);
typedef BOOL (WINAPI *LPFN_ImeSetCompositionString)(HIMC hIMC,DWORD dwIndex,LPVOID lpComp,DWORD  dwCompLen,LPVOID lpRead,DWORD  dwReadLen);
typedef UINT (WINAPI *LPFN_ImeToAsciiEx)(UINT uVirtKey,UINT uScanCode,CONST LPBYTE lpbKeyState, DWORD* lpTransBuf,UINT fuState,HIMC hIMC);
typedef BOOL (WINAPI *LPFN_ImeUnregisterWord)(LPCTSTR lpszReading,DWORD dwStyle,LPCTSTR lpszString);
typedef BOOL (WINAPI *LPFN_NotifyIME)(HIMC hIMC,DWORD dwAction,DWORD dwIndex,DWORD dwValue);

HINSTANCE					g_hIMM							= NULL;
LPFN_ImeConversionList		g_pfnImeConversionList			= NULL;
LPFN_ImeConfigure			g_pfnImeConfigure				= NULL;
LPFN_ImeDestroy				g_pfnImeDestroy					= NULL;
LPFN_ImeEnumRegisterWord	g_pfnImeEnumRegisterWord		= NULL;

LPFN_ImeEscape				g_pfnImeEscape					= NULL;
LPFN_ImeGetRegisterWordStyle	g_pfnImeGetRegisterWordStyle= NULL;
LPFN_ImeInquire					g_pfnImeInquire				= NULL;
LPFN_ImeProcessKey				g_pfnImeProcessKey			= NULL;
LPFN_ImeRegisterWord			g_pfnImeRegisterWord		= NULL;
LPFN_ImeSelect					g_pfnImeSelect				= NULL;

LPFN_ImeSetActiveContext		g_pfnImeSetActiveContext	= NULL;
LPFN_ImeSetCompositionString	g_pfnImeSetCompositionString= NULL;
LPFN_ImeToAsciiEx				g_pfnImeToAsciiEx			= NULL;
LPFN_ImeUnregisterWord			g_pfnImeUnregisterWord		= NULL;
LPFN_NotifyIME					g_pfnNotifyIME				= NULL;

#define GETPROCADDRESS(APIName, Temp )			 \
	Temp = ::GetProcAddress( g_hIMM, #APIName ); \
    if( Temp ) *(FARPROC*)&g_pfn##APIName = Temp


HANDLE g_hInst = NULL;


BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			if(g_hIMM == NULL)
			{
				g_hInst	= hModule;
				g_hIMM	= ::LoadLibraryW(L"E:\\EMMOA Projects\\SogouPy.ime");
				FARPROC Temp;
				GETPROCADDRESS(ImeConversionList, Temp);
				GETPROCADDRESS(ImeConfigure, Temp);
				GETPROCADDRESS(ImeDestroy, Temp);
				GETPROCADDRESS(ImeEnumRegisterWord, Temp);

				GETPROCADDRESS(ImeEscape, Temp);
				GETPROCADDRESS(ImeGetRegisterWordStyle, Temp);
				GETPROCADDRESS(ImeInquire, Temp);
				GETPROCADDRESS(ImeProcessKey, Temp);

				GETPROCADDRESS(ImeSelect, Temp);
				GETPROCADDRESS(ImeSetActiveContext, Temp);
				GETPROCADDRESS(ImeSetCompositionString, Temp);
				GETPROCADDRESS(ImeToAsciiEx, Temp);
				
				GETPROCADDRESS(ImeUnregisterWord, Temp);
				GETPROCADDRESS(NotifyIME, Temp);
				GETPROCADDRESS(ImeRegisterWord, Temp);
				::DisableThreadLibraryCalls( (HMODULE) hModule );
			}
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			if(g_hIMM != NULL)
			{
				::FreeLibrary(g_hIMM);
				g_hIMM	= NULL;
			}
			break;
		}
	}
    return TRUE;
}


//// This is an example of an exported variable
//int IMMHOOKER_API nIMMHooker=0;
//
//// This is an example of an exported function.
//int IMMHOOKER_API fnIMMHooker(void)
//{
//	return 42;
//}

//// This is the constructor of a class that has been exported.
//// see IMMHooker.h for the class definition
//CIMMHooker::CIMMHooker()
//{ 
//	return; 
//}

// 		// ImeConfigure
// 		// ImeConversionList
// 		// ImeDestroy
// 		// ImeEnumRegisterWord

// 		// ImeEscape
// 		// ImeGetRegisterWordStyle
// 		// ImeInquire

// 		// ImeProcessKey
// 		// ImeRegisterWord
// 		// ImeSelect

// 		// ImeSetActiveContext
// 		// ImeSetCompositionString
// 		// ImeToAsciiEx
// 		// ImeUnregisterWord
// 		// NotifyIME


//初始化输入法函数
BOOL WINAPI ImeInquire(IMEINFO* lpImeInfo,LPTSTR lpszWndCls,DWORD lpszOptions)
{
	SHOWOUTPUTDEBUG( ImeInquire );
 //	EXTRACE3("ImeInquire(%p, %p, %#x)...", lpImeInfo, lpszWndCls, lpszOptions);
	return	g_pfnImeInquire(lpImeInfo, lpszWndCls, lpszOptions);
	// 	if (!lpImeInfo)
	// 		return (FALSE);
	// 	lpImeInfo->dwPrivateDataSize = sizeof(PRIVCONTEXT);
	// 	lpImeInfo->fdwProperty = IME_PROP_KBD_CHAR_FIRST|
	// 		IME_PROP_CANDLIST_START_FROM_1|
	// 		IME_PROP_IGNORE_UPKEYS;
	// 	lpImeInfo->fdwConversionCaps =      IME_CMODE_NATIVE|IME_CMODE_NATIVE|IME_CMODE_FULLSHAPE|IME_CMODE_CHARCODE|IME_CMODE_SOFTKBD|IME_CMODE_NOCONVERSION;
	// 	lpImeInfo->fdwSentenceCaps = 0;
	// 	lpImeInfo->fdwUICaps = UI_CAP_ROT90|UI_CAP_SOFTKBD;
	// 	lpImeInfo->fdwSCSCaps = SCS_CAP_COMPSTR|SCS_CAP_MAKEREAD;
	// 	lpImeInfo->fdwSelectCaps = (DWORD)0;
	// 	lstrcpy(lpszWndCls, (LPTSTR)szUIClassName);
	return (TRUE);
}      

//自定义汉字编码格式，未作处理。

DWORD WINAPI ImeConversionList(HIMC hIMC,LPCTSTR lpszSrc,DWORD uBufLen,UINT uFlag)
{
	SHOWOUTPUTDEBUG( ImeConversionList) ;
//	ASSERT(g_pfnImeConversionList  != NULL);
//	EXTRACE4("ImeConversionList(%#x, %p, %#x, %#x)...", hIMC, lpszSrc, uBufLen, uFlag);

	return	g_pfnImeConversionList(hIMC, lpszSrc, uBufLen, uFlag);
}

//设置输入法状态函数，如光标跟随等
BOOL WINAPI ImeConfigure(HKL hKL,HWND hAppWnd,DWORD dwMode,LPVOID  lpData)
{
	SHOWOUTPUTDEBUG( ImeConfigure );
//	EXTRACE4("ImeConfigure(%#x, %p, %#x, %p)...", hKL, hAppWnd, dwMode, lpData);
	return	g_pfnImeConfigure(hKL, hAppWnd, dwMode, lpData);
//     switch (dwMode) 
// 	{
// 	case IME_CONFIG_GENERAL:
// 		DialogBox(hInst, MAKEINTRESOURCE(SETIME), (HWND)hAppWnd, (DLGPROC)ImeSetDlgProc);
// 		break;
// 	default:
// 		return (FALSE);
// 		break;
// 	}
	return (TRUE);
}

//退出
BOOL WINAPI ImeDestroy(UINT uReserved)
{
	SHOWOUTPUTDEBUG( ImeDestroy );
//	EXTRACE1("ImeDestroy(%#x)...", uReserved);
	return	g_pfnImeDestroy(uReserved);
} 

//应用接口函数
LRESULT WINAPI ImeEscape(HIMC hIMC,UINT uSubFunc,LPVOID lpData)
{
//	EXTRACE3("ImeEscape(%p, %#x, %p)...", hIMC, uSubFunc, lpData) ;
	return	g_pfnImeEscape(hIMC, uSubFunc, lpData);
 	LRESULT lRet;
// 	
// 	switch (uSubFunc) 
// 	{
// 	case IME_ESC_QUERY_SUPPORT:
// 		if ( lpData == NULL )
// 			return FALSE;
// 		
// 		switch (*(LPUINT)lpData)
// 		{
// 		case IME_ESC_QUERY_SUPPORT:
// 		case IME_ESC_MAX_KEY:
// 		case IME_ESC_IME_NAME:
// 		case IME_ESC_GETHELPFILENAME:
// 			return (TRUE);
// 		case IME_ESC_SEQUENCE_TO_INTERNAL:
// 		case IME_ESC_GET_EUDC_DICTIONARY:
// 		case IME_ESC_SET_EUDC_DICTIONARY:
// 		case IME_INPUTKEYTOSEQUENCE:
// 			return (FALSE);         
// 		default:
// 			return (FALSE);
// 		}
// 		break;
// 		case IME_ESC_SEQUENCE_TO_INTERNAL:
// 		case IME_ESC_GET_EUDC_DICTIONARY:
// 		case IME_ESC_SET_EUDC_DICTIONARY:
// 		case IME_INPUTKEYTOSEQUENCE:
// 			return (FALSE);
// 		case IME_ESC_MAX_KEY:
// 			return ((WORD) 4);
// 		case IME_ESC_GETHELPFILENAME:
// 			{
// 				TCHAR szIMEGUDHlpName[MAXSTRLEN];
// 				if (lpData == NULL )
// 					return FALSE;
// 				
// 				szIMEGUDHlpName[0] = 0;
// 				GetWindowsDirectory((LPTSTR)szIMEGUDHlpName, MAXSTRLEN);
// 				lstrcat((LPTSTR)szIMEGUDHlpName, TEXT("\\HELP\\3JaJa.hlp"));
// 				lstrcpy((char*)lpData, (char*)szIMEGUDHlpName);
// 				return TRUE;
// 			}
// 			
// 		default:
// 			return (FALSE);
//     }
// 	
    return (lRet);
} 

//启动输入法设置功能
BOOL WINAPI ImeSelect(HIMC hIMC,BOOL fSelect)
{
	SHOWOUTPUTDEBUG( ImeSelect );
//	EXTRACE2("ImeSelect(%p, %u)...", hIMC, fSelect);
	return	g_pfnImeSelect(hIMC, fSelect);
	BOOL           fRet;
// 	INPUTCONTEXT	*lpIMC;
// 	
//     if (!hIMC) 
// 		return (FALSE); 
// 	
//     lpIMC = (INPUTCONTEXT*)ImmLockIMC(hIMC);
//     if (!lpIMC) 
// 		return (FALSE); 
// 	
//     fRet = Select(hIMC, lpIMC, fSelect);
//     ImmUnlockIMC(hIMC);
    return (fRet);
} 

//设置输入活动状态
BOOL WINAPI ImeSetActiveContext(HIMC hIMC,BOOL fOn)
{
	SHOWOUTPUTDEBUG( ImeSetActiveContext );
//	EXTRACE2("ImeSetActiveContext(%p, %u)...", hIMC, fOn);
	return	g_pfnImeSetActiveContext(hIMC, fOn);
// 	if (fOn&&hIMC) 
//     {
// 		INPUTCONTEXT* lpIMC;
// 		
// 		lpIMC = (INPUTCONTEXT*)ImmLockIMC(hIMC);
// 		if (!lpIMC) 
// 			return (FALSE);
// 		
//         InitContext(lpIMC);
//         ImmUnlockIMC(hIMC);
// 	}
	return (TRUE);
}

//用户按键处理函数，例如中英文状态，返回 FALSE 为英文状态，按键不作处理。
BOOL WINAPI ImeProcessKey(HIMC hIMC,UINT uVirtKey,LPARAM lParam,CONST LPBYTE lpbKeyState)
{
	SHOWOUTPUTDEBUG(ImeProcessKey);
//	EXTRACE4("ImeProcessKey(%p, %u, %#x, %p)...", hIMC, uVirtKey, lParam, lpbKeyState);
	return	g_pfnImeProcessKey(hIMC, uVirtKey, lParam, lpbKeyState);
}

//输入法编码字符处理。
BOOL WINAPI ImeSetCompositionString(HIMC hIMC,DWORD dwIndex,LPVOID lpComp,	DWORD  dwCompLen,LPVOID lpRead,DWORD  dwReadLen)
{
	SHOWOUTPUTDEBUG(ImeSetCompositionString);
//	EXTRACE6("ImeSetCompositionString(%p, %u, %p, %u, %p, %u)...", hIMC, dwIndex, lpComp, dwCompLen, lpRead, dwReadLen);
	return	g_pfnImeSetCompositionString(hIMC, dwIndex, lpComp, dwCompLen, lpRead, dwReadLen);
}

//将用户按键转换为汉字编码
UINT WINAPI ImeToAsciiEx(UINT uVirtKey,UINT uScanCode,CONST LPBYTE lpbKeyState, DWORD* lpTransBuf,UINT fuState,HIMC hIMC)
{
	SHOWOUTPUTDEBUG(ImeToAsciiEx);
//	EXTRACE6("ImeToAsciiEx(%u, %u, %p, %p, %u, %p)...", uVirtKey, uScanCode, lpbKeyState, lpTransBuf, fuState, hIMC);
	return	g_pfnImeToAsciiEx(uVirtKey, uScanCode, lpbKeyState, lpTransBuf, fuState, hIMC);
}

//处理IME消息函数
BOOL WINAPI NotifyIME(HIMC hIMC,DWORD dwAction,DWORD dwIndex,DWORD dwValue)
{
	SHOWOUTPUTDEBUG( NotifyIME );
//	EXTRACE4("NotifyIME(%p, %#x, %#x, %#x)...", hIMC, dwAction, dwIndex, dwValue);
	return	g_pfnNotifyIME(hIMC, dwAction, dwIndex, dwValue);
}    

//自定义汉字编码格式，未作处理。
BOOL WINAPI ImeRegisterWord(LPCTSTR lpszReading,DWORD dwStyle,LPCTSTR lpszString)
{
	SHOWOUTPUTDEBUG( ImeRegisterWord );
//	EXTRACE3("ImeRegisterWord(%p, %#x, %p)...", lpszReading, dwStyle, lpszString);
	return g_pfnImeRegisterWord(lpszReading, dwStyle, lpszString);
}

//自定义汉字编码格式，未作处理。
BOOL WINAPI ImeUnregisterWord(LPCTSTR lpszReading,DWORD dwStyle,LPCTSTR lpszString)
{
	SHOWOUTPUTDEBUG(ImeUnregisterWord );
//	EXTRACE3("ImeUnregisterWord(%p, %#x, %p)...", lpszReading, dwStyle, lpszString);
   return g_pfnImeUnregisterWord(lpszReading, dwStyle, lpszString);
}

//自定义汉字编码格式，未作处理。
UINT WINAPI ImeGetRegisterWordStyle(UINT nItem,LPSTYLEBUF lpStyleBuf)
{
	SHOWOUTPUTDEBUG( ImeGetRegisterWordStyle );
//	EXTRACE2("ImeGetRegisterWordStyle(%u,  %p)...", nItem, lpStyleBuf);
   return g_pfnImeGetRegisterWordStyle(nItem, lpStyleBuf);
}

//自定义汉字编码格式，未作处理。
UINT WINAPI ImeEnumRegisterWord(REGISTERWORDENUMPROC lpfnRegisterWordEnumProc, LPCTSTR lpszReading,DWORD dwStyle,LPCTSTR lpszString,LPVOID lpData)
{
	SHOWOUTPUTDEBUG( ImeEnumRegisterWord );
//	EXTRACE5("ImeGetRegisterWordStyle(%p,  %p, %#x, %p, %p)...", lpfnRegisterWordEnumProc, lpszReading, dwStyle, lpszString, lpData);
   return g_pfnImeEnumRegisterWord(lpfnRegisterWordEnumProc, lpszReading, dwStyle, lpszString, lpData);
}

//UI窗口过程
LRESULT CALLBACK UIWndProc(HWND hUIWnd,UINT   uMsg,WPARAM wParam,LPARAM lParam)
{
	SHOWOUTPUTDEBUG( UIWndProc );
// 	switch (uMsg) 
// 	{
// 	case WM_CREATE:
// 		CreateUIWindow(hUIWnd);
// 		break;
// 	case WM_DESTROY:
// 		DestroyUIWindow(hUIWnd);
// 		break;
// 	case WM_IME_STARTCOMPOSITION:
// 		break;
// 	}
	return	0L;
}
	
//状态窗口过程
LRESULT CALLBACK StatusWndProc(HWND hStatusWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return	0;
}

//编码窗口过程
LRESULT CALLBACK CompWndProc(HWND hCompWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return	0;
}

//侯选汉字窗口过程
LRESULT CALLBACK CandWndProc(HWND hCandWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return	0;
}

		
