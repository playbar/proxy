
#include "DXUTDefine.h"


HRESULT WINAPI DXUTTrace( const CHAR* strFile, DWORD dwLine, HRESULT hr,
						 const WCHAR* strMsg, bool bPopMsgBox )
{
	bool bShowMsgBoxOnError = true;
	if( bPopMsgBox && bShowMsgBoxOnError == false )
		bPopMsgBox = false;

	return DXTrace( strFile, dwLine, hr, strMsg, bPopMsgBox );
}


VOID DXUTOutputDebugStringW( LPCWSTR strMsg, ... )
{
#if defined(DEBUG) || defined(_DEBUG)
	WCHAR strBuffer[512];

	va_list args;
	va_start(args, strMsg);
	StringCchVPrintfW( strBuffer, 512, strMsg, args );
	strBuffer[511] = L'\0';
	va_end(args);

	//OutputDebugString( strBuffer );
#else
	UNREFERENCED_PARAMETER(strMsg);
#endif
}

VOID DXUTOutputDebugStringA( LPCSTR strMsg, ... )
{
#if defined(DEBUG) || defined(_DEBUG)
	CHAR strBuffer[512];

	va_list args;
	va_start(args, strMsg);
	StringCchVPrintfA( strBuffer, 512, strMsg, args );
	strBuffer[511] = '\0';
	va_end(args);

	OutputDebugStringA( strBuffer );
#else
	UNREFERENCED_PARAMETER(strMsg);
#endif
}
