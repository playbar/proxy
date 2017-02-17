
#include "stdafx.h"
#include "GlobalFunction.h"
#include "DefineConst.h"


HRESULT AllocateName( LPCSTR Name, LPSTR *pNewName )
{
	UINT cbLength;

	if( Name != NULL )
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy( *pNewName, Name, cbLength * sizeof(CHAR) );
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}


DWORD WINAPI OpenIEThreadProc(void *p)
{
	ShellExecute( GetActiveWindow(), "open", (char*)p, NULL, NULL, SW_SHOW );
	return 0;
}

//
//LRESULT CALLBACK ProxyMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch( uMsg )
//	{
//	case WM_LBUTTONDOWN:
//		OutputDebugString("鼠标左键按下！");
//		break;
//	
//	}
//
//	if( IsWindow(g_hMainWnd) && g_pFunWinProc != 0 )
//	{
//		return g_pFunWinProc( hWnd, uMsg, wParam, lParam );
//	}
//	return 0;
//}




int WINAPI SearchStatCallback(DWORD dwGameID,DWORD dwAdID, WPARAM wParam, LPARAM lParam)
{
	//if( m_p3DFocus != NULL ) //给主模块发送拾取id
	//{
	//	m_p3DFocus(m_PickID, 0);
	//}

	return	0;
}


bool FileExist(const char* path)
{
	return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES || 
		GetLastError() != ERROR_FILE_NOT_FOUND;
}


void GetModulePath( char * szfullPathName )
{
	if ( NULL == szfullPathName )
	{
		return;
	}
	//OutputDebugString(fullPathName);
	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *p;
	p = strrchr(TmpPath,'\\');
	if(!p)
		return ;
	*(p+1) = 0;

	char path[MAX_PATH];	
	sprintf( path,"%s%s", TmpPath, szfullPathName );
	strcpy(szfullPathName,path);

}

//-------------------------------------------------------------------------------------
//Description:
// This function maps a character string to a wide-character (Unicode) string
//
//Parameters:
// lpcszStr: [in] Pointer to the character string to be converted 
// lpwszStr: [out] Pointer to a buffer that receives the translated string. 
// dwSize: [in] Size of the buffer
//
//Return Values:
// TRUE: Succeed
// FALSE: Failed
// 
//Example:
// MByteToWChar(szA,szW,sizeof(szW)/sizeof(szW[0]));
//---------------------------------------------------------------------------------------
BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
	// Get the required size of the buffer that receives the Unicode 
	// string. 
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);

	if(dwSize < dwMinSize)
	{
		return FALSE;
	}


	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);  
	return TRUE;
}

//-------------------------------------------------------------------------------------
//Description:
// This function maps a wide-character string to a new character string
//
//Parameters:
// lpcwszStr: [in] Pointer to the character string to be converted 
// lpszStr: [out] Pointer to a buffer that receives the translated string. 
// dwSize: [in] Size of the buffer
//
//Return Values:
// TRUE: Succeed
// FALSE: Failed
// 
//Example:
// MByteToWChar(szW,szA,sizeof(szA)/sizeof(szA[0]));
//---------------------------------------------------------------------------------------
BOOL WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);
	if(dwSize < dwMinSize)
	{
		return FALSE;
	}
	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,FALSE);
	return TRUE;
}

//异常过滤函数
long ExceptFilter( LPEXCEPTION_POINTERS pep)
{
	char szBuffer[512];
	char *pTemp;
	PEXCEPTION_RECORD pER = pep->ExceptionRecord;
	DWORD dwExceptionCode = pER->ExceptionCode;
	sprintf( szBuffer, "code = %x, Address = %p", dwExceptionCode, pER->ExceptionAddress );

	pTemp = strrchr(szBuffer, '\0');

	switch ( dwExceptionCode )
	{
	case EXCEPTION_ACCESS_VIOLATION:
		sprintf( pTemp, " Attemp to %s data at address %p", pER->ExceptionInformation[0] ? "Write" : "Read", pER->ExceptionInformation[1] );
		break;
	}

	char szExceptLogFil[MAX_PATH];
	strcpy( szExceptLogFil, "ExceptLog.log" );
	GetModulePath(szExceptLogFil);
	ExceptLog(szExceptLogFil, "%s", szBuffer);

	return EXCEPTION_EXECUTE_HANDLER;
}

void ExceptDispose( LPCTSTR pFileName, int iline )
{
	char szExceptLogFil[MAX_PATH];
	strcpy( szExceptLogFil, "ExceptLog.log" );
	GetModulePath(szExceptLogFil);

	SYSTEMTIME st;
	GetLocalTime( &st );
	char szDate[100];
	sprintf( szDate, "%04d-%02d-%02d %02d:%02d:%02d",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	ExceptLog(szExceptLogFil , "%s : %s %d", szDate, pFileName, iline );

}

BOOL ExceptLog(LPCTSTR pchLogFileName, LPCTSTR lpszFormat, ...)
{
	try
	{
		TCHAR	szText[1024*2]		= { 0 };
		va_list	ap;
		va_start(ap, lpszFormat);
		::_vsntprintf(szText, 1024*2, lpszFormat, ap);
		va_end(ap);	

		FILE	*pFile	= NULL;	
		pFile	= ::_tfopen(pchLogFileName,  _T("a+b"));
		if ( pFile != NULL )
		{
			::_ftprintf(pFile, _T("%s\r\n"), szText);
			fclose(pFile);
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}