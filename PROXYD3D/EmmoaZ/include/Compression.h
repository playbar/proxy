//////////////////////////////////////////////////////////////////////
//************************************************************************************************************
// purpose    :  整理成单独dll
// Date       :  2008-6-3  10:56:18
// author     :  3o3afe1 
//************************************************************************************************************
#if !defined(AFX_COMPRESSION_H__67065F80_B2F7_4535_AB04_559E3ED52C6E__INCLUDED_)
#define AFX_COMPRESSION_H__67065F80_B2F7_4535_AB04_559E3ED52C6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef COMPRESS_DLL
    #pragma  message (" export compress dll. 3o3afe1")
    #define _COMPRESS_API   __declspec(dllexport)
#else
    #pragma  message (" import compress dll. 3o3afe1")
    #define _COMPRESS_API   __declspec(dllimport)
    #ifdef _DEBUG
          #pragma comment(lib,"..\\lib\\igacompressd.lib") 
    #else
          #pragma comment(lib,"..\\lib\\igacompress.lib") 
    #endif
#endif

class _COMPRESS_API KUnzip 
{
public:
	KUnzip();	
	~KUnzip();
	
	BOOL Unzip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath);    
	
private:	
	BOOL		m_bInited;
	HINSTANCE	m_hInst;
	VOID*	    m_pfnUserFunctions;
	VOID*		m_pfnWiz_SingleEntryUnzip;
	VOID*		m_pDCL;
};

class _COMPRESS_API KZip 
{
public:
	KZip();
	KZip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath);
	~KZip();
	BOOL Zip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath);    
	
private:
	VOID*		m_pInfoZip;
	BOOL		m_bInited;
};



#endif // !defined(AFX_COMPRESSION_H__67065F80_B2F7_4535_AB04_559E3ED52C6E__INCLUDED_)
