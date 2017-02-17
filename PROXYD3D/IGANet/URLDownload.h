
#if !defined(AFX_URLDOWNLOAD_H__698291DC_C019_43EF_94B1_93009EEEB0DE__INCLUDED_)
#define AFX_URLDOWNLOAD_H__698291DC_C019_43EF_94B1_93009EEEB0DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class HttpDownLoad
{
public:
	HttpDownLoad();
	~HttpDownLoad();
public:
	static  BOOL    DeleteHttpTmpFile();
	static	BOOL	Download(LPCTSTR lpszURL,LPCTSTR  lpszTmpFile,BOOL  bProtocol = FALSE);
	static  BOOL    SendBuf (LPCTSTR lpszURL,LPCTSTR  lpszTmpFile,char* pData , DWORD  dwLen);

};


class FtpDownLoad
{
public:
	FtpDownLoad();
	~FtpDownLoad();
public:
	static	BOOL	Download(LPCTSTR lpszURL,LPCTSTR  lpszTmpFile);

};	

#include <WinInet.h>
class IGA_HttpObject
{
public:
	IGA_HttpObject();
	~IGA_HttpObject();
	
	void   Close();
	BOOL   Open(LPCTSTR lpszURL);
	BOOL   Read(CHAR *pBuffer, DWORD dwBytesToRead, DWORD*pdwBytesRead = NULL);
	DWORD  SendBuffer(const CHAR *pBuffer, DWORD dwLength);

    BOOL   TranslateURL(LPCTSTR lpszURL, LPTSTR pszServer,LPTSTR  pszObject,LPTSTR lpszUserName,LPTSTR lpszPassword);

private:
	HINTERNET	m_hSession;
	HINTERNET	m_hConnection;
	HINTERNET	m_hRequest;
};


#endif // !defined(AFX_URLDOWNLOAD_H__698291DC_C019_43EF_94B1_93009EEEB0DE__INCLUDED_)
