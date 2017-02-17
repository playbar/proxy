// URLDownload.cpp: implementation of the URLDownload class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "URLDownload.h"
#include "Protocol.h"

#include "wininet.h" 
#pragma comment(lib,"wininet.lib" )

#include <io.h>


//zhuzhenfei  清临时文件
#pragma comment(lib,"wininet.lib")
#include <wininet.h>
BOOL   HttpDownLoad::DeleteHttpTmpFile()
{
	BOOL bResult = FALSE;
	BOOL bDone   = FALSE;
	LPINTERNET_CACHE_ENTRY_INFO    lpCacheEntry = NULL;
	DWORD   dwTrySize;
	DWORD   dwEntrySize = 4096;
	HANDLE  hCacheDir   = NULL;
	DWORD   dwError     = ERROR_INSUFFICIENT_BUFFER;
	
	do
	{
		switch(dwError)
		{
		case ERROR_INSUFFICIENT_BUFFER:
			delete [] lpCacheEntry;
			lpCacheEntry =(LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
			lpCacheEntry->dwStructSize = dwEntrySize;
			dwTrySize =dwEntrySize;
			BOOL bSuccess;
			if ( NULL == hCacheDir)
				bSuccess = (hCacheDir = FindFirstUrlCacheEntry(NULL,lpCacheEntry,&dwTrySize))!= NULL;
			else
				bSuccess = FindNextUrlCacheEntry(hCacheDir,lpCacheEntry,&dwTrySize);
			
			if ( bSuccess)
				dwError =ERROR_SUCCESS;
			else
			{
				dwError = GetLastError();
				dwEntrySize = dwTrySize;
			}
			break;
		case ERROR_NO_MORE_ITEMS:
			bDone =TRUE;
			bResult = TRUE;
			break;
		case ERROR_SUCCESS:
			if (!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
			{
				if ( ::_tcsstr(lpCacheEntry->lpszSourceUrlName,_T("EMMOA_IIS")) != NULL)
					DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
			}
			dwTrySize =dwEntrySize;
			if (FindNextUrlCacheEntry(hCacheDir,lpCacheEntry,&dwTrySize))
				dwError =ERROR_SUCCESS;
			else
			{
				dwError = GetLastError();
				dwEntrySize = dwTrySize;
			}
			break;
		default:
			bDone =TRUE;
			break;
			
		}
		
		if ( bDone )
		{
			
			delete [] lpCacheEntry;
			if ( hCacheDir )
				FindCloseUrlCache(hCacheDir);
		}
	}while(!bDone);
	
	return bResult;
}

BOOL  DeleteNetTmpFile(LPCTSTR  lpszURL)
{
	TCHAR  szCanonicalizedURL[INTERNET_MAX_URL_LENGTH];
	DWORD  dwNeededLength       = INTERNET_MAX_URL_LENGTH;
	if ( !::InternetCanonicalizeUrl(lpszURL, szCanonicalizedURL,&dwNeededLength, ICU_BROWSER_MODE) )
		return FALSE;

	return ::DeleteUrlCacheEntry(szCanonicalizedURL);

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL HttpDownLoad::Download(LPCTSTR lpszURL,LPCTSTR  lpszTmpFile,BOOL  bProtocol)
{//内部调用不进行判断

	BOOL  bResult = FALSE;

#ifdef NEED_OUTPUT_MSG	
	::OutputDebugString(lpszURL);
	::OutputDebugString("\r\n");	
#endif

	const HRESULT hr = ::URLDownloadToFile(NULL,lpszURL,lpszTmpFile,0,0);	
	if ( ! DeleteNetTmpFile(lpszURL))
	   DeleteHttpTmpFile();

	if(FAILED(hr))
	{		
		return	bResult;
	}

    if ( ::_taccess(lpszTmpFile,0) != -1) 
		bResult = TRUE;
	

	return bResult;

	
}

BOOL HttpDownLoad::SendBuf (LPCTSTR lpszURL,LPCTSTR  lpszTmpFile,char* pData , DWORD  dwLen)
{
   BOOL  bResult = FALSE;


   return bResult ;
}



IGA_HttpObject::IGA_HttpObject()
{
	m_hSession     = NULL;
	m_hConnection  = NULL;
	m_hRequest     = NULL;
}

IGA_HttpObject::~IGA_HttpObject()
{
}

DWORD IGA_HttpObject::SendBuffer(const CHAR *pBuffer, DWORD dwLength)
{
	ASSERT(this->m_hRequest != NULL);
	INTERNET_BUFFERS	BufferIn;
	DWORD				dwRet	= 0L;
	BufferIn.dwStructSize		= sizeof( INTERNET_BUFFERS ); // Must be set or error will occur
	BufferIn.Next				= NULL; 
	BufferIn.lpcszHeader		= NULL;
	BufferIn.dwHeadersLength	= 0;
	BufferIn.dwHeadersTotal		= 0;
	BufferIn.lpvBuffer			= NULL;                
	BufferIn.dwBufferLength		= 0;
	BufferIn.dwBufferTotal		= dwLength; // This is the only member used other than dwStructSize
	BufferIn.dwOffsetLow		= 0;
	BufferIn.dwOffsetHigh		= 0;

	if(!::HttpSendRequestEx( this->m_hRequest, &BufferIn, NULL, 0, 0))
		return 0L;

	BOOL bRet	= ::InternetWriteFile(this->m_hRequest, pBuffer, dwLength, &dwRet);
	
	if(bRet)
	{
		bRet	= ::HttpEndRequest(this->m_hRequest, NULL, 0, 0);
	}
	return dwRet;
}


BOOL IGA_HttpObject::Read(CHAR *pBuffer, DWORD dwBytesToRead, DWORD*pdwBytesRead /* = NULL */)
{
	ASSERT(this->m_hRequest != NULL);
	DWORD	dwRet	= 0L;
	if(!::InternetReadFile(this->m_hRequest, pBuffer, dwBytesToRead, &dwRet))
		return	FALSE;

	if(pdwBytesRead)
		*pdwBytesRead	= dwRet;
	return	TRUE;
}

void IGA_HttpObject::Close()
{
	if(m_hRequest != NULL)
	{
		::InternetCloseHandle(this->m_hRequest);
		this->m_hRequest	= NULL;
	}
	if( this->m_hConnection != NULL)
	{
		::InternetCloseHandle(this->m_hConnection);
		this->m_hConnection	= NULL;
	}
	if( this->m_hSession != NULL)
	{
		::InternetCloseHandle(this->m_hSession);
		this->m_hSession	= NULL;
	}
}

BOOL   IGA_HttpObject::TranslateURL(LPCTSTR lpszURL, LPTSTR pszServer,LPTSTR  pszObject,LPTSTR lpszUserName,LPTSTR lpszPassword)
{
	BOOL   bRetVal = FALSE;
	TCHAR  szCanonicalizedURL[INTERNET_MAX_URL_LENGTH];
	DWORD  dwNeededLength       = INTERNET_MAX_URL_LENGTH;
	 if ( !::InternetCanonicalizeUrl(lpszURL, szCanonicalizedURL,&dwNeededLength, ICU_BROWSER_MODE) )
		 return FALSE;
	

	
 	TCHAR   szServer   [ 100 ];
 	TCHAR   szObject   [ 100 ];
 	TCHAR   szUserName [ 100 ];
 	TCHAR   szPassword [ 100 ];

	URL_COMPONENTS urlComponents	= { 0};
	ZeroMemory(&urlComponents, sizeof(URL_COMPONENTS));
	urlComponents.dwStructSize = sizeof(URL_COMPONENTS);	
	
	urlComponents.dwHostNameLength	= INTERNET_MAX_URL_LENGTH;
	urlComponents.lpszHostName		= szServer;
	urlComponents.dwUrlPathLength	= INTERNET_MAX_URL_LENGTH;
	urlComponents.lpszUrlPath		= szObject;
	urlComponents.lpszUserName		= szUserName;
	urlComponents.dwUserNameLength	= INTERNET_MAX_URL_LENGTH;
	urlComponents.dwPasswordLength	= INTERNET_MAX_URL_LENGTH;
	urlComponents.lpszPassword		= szPassword;
	bRetVal = ::InternetCrackUrl(szCanonicalizedURL, 0,0, &urlComponents);
    if  ( bRetVal )
	{
		::_tcscpy(pszServer,szServer);
		::_tcscpy(pszObject,szObject);
		::_tcscpy(lpszUserName,szUserName);
		::_tcscpy(lpszPassword,szPassword);
	}
	return bRetVal;

}
BOOL IGA_HttpObject::Open(LPCTSTR lpszURL)
{
	TCHAR   szServer[100];
	TCHAR   szObject[100];
	TCHAR   szUserName[100];
	TCHAR   szPassword[100];
	
	ASSERT(m_hSession == NULL);		
	m_hSession	= ::InternetOpen( _T("EMMOA_Core"), PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, 0);
	if( m_hSession == NULL )		return	FALSE;   
	
	TranslateURL(lpszURL,szServer,szObject,szUserName,szPassword);
	m_hConnection	= ::InternetConnect(m_hSession, 
		                                szServer,
		                                INTERNET_DEFAULT_HTTP_PORT,
										szUserName,
										szPassword,
										INTERNET_SERVICE_HTTP,
										0,
										NULL);

	
	if(m_hConnection == NULL )		return	FALSE;

	m_hRequest	= ::HttpOpenRequest(m_hConnection, "POST", szObject, NULL,
		                            NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);

	return	(m_hRequest != NULL);
}