// Compression.cpp: implementation of the KCompression class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Compression.h"

#include "InfoZip.h"
#include <io.h>
//using namespace Zip;


#	include "unzip552/unzip.h"
#	include "unzip552/unzvers.h"
#	include "unzip552/windll/decs.h"
#	include "unzip552/windll/structs.h"




typedef int (WINAPI * _DLL_UNZIP)(int, char **, int, char **,LPDCL, LPUSERFUNCTIONS);
int WINAPI Password(LPSTR p, int n, LPCSTR m, LPCSTR name)
{
	return	1;
}

int WINAPI PrintBuffer(LPSTR p, unsigned long nLength)
{
	TRACE(_T("%s\r\n"), (CHAR *)p);
	return (int)(unsigned int) nLength;
}

int WINAPI GetReplaceDlgRetVal(LPSTR filename)
{

	return 1;
}
void WINAPI ReceiveDllMessage(unsigned long ucsize, unsigned long csiz,unsigned cfactor,
										unsigned mo, unsigned dy, unsigned yr, unsigned hh, unsigned mm,
										char c, LPSTR filename, LPSTR methbuf, unsigned long crc, char fCrypt)
{

}


KUnzip::KUnzip()
{
	m_hInst	= ::LoadLibraryA( "unzip32.dll" );
	if(NULL == m_hInst)
	{	
		TRACE(_T("LoadLibraryA unzip32.dll failed .\r\n"));
		return	;
	}
	
	m_pfnUserFunctions	= new USERFUNCTIONS;
	ZeroMemory(m_pfnUserFunctions, sizeof(USERFUNCTIONS));
	USERFUNCTIONS* pfnUserFunctions =(USERFUNCTIONS*) m_pfnUserFunctions;
	pfnUserFunctions->password	    = Password;
	pfnUserFunctions->print		    = PrintBuffer;
	pfnUserFunctions->sound		    = NULL;	
	pfnUserFunctions->replace		= GetReplaceDlgRetVal;
	pfnUserFunctions->SendApplicationMessage	= ReceiveDllMessage;


	m_pfnWiz_SingleEntryUnzip	= (_DLL_UNZIP)::GetProcAddress(m_hInst, "Wiz_SingleEntryUnzip");
	if(m_pfnWiz_SingleEntryUnzip == NULL)	return	;

	m_pDCL	= new DCL();
	ZeroMemory(m_pDCL, sizeof(DCL));


	m_bInited	= true;
}
KUnzip::~KUnzip()
{
	if(m_bInited)
	{
		if(m_hInst !=NULL)
		{
			::FreeLibrary(this->m_hInst);
			this->m_hInst	= NULL;
		}
		m_pfnWiz_SingleEntryUnzip	= NULL;
		m_bInited	= false;
	}

	if  ( m_pDCL != NULL)
	{
		DCL*  p = (DCL* )m_pDCL;
		delete p;
	}
	m_pDCL  = NULL;

	if  ( m_pfnUserFunctions != NULL)
	{
		USERFUNCTIONS* p = (USERFUNCTIONS*)m_pfnUserFunctions;
		delete p;
	}
	m_pfnUserFunctions = NULL;


}

BOOL KUnzip::Unzip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath)
{
	ASSERT(this->m_pfnWiz_SingleEntryUnzip != NULL);
	ASSERT(this->m_pfnUserFunctions != NULL);
	ASSERT(this->m_pDCL != NULL);
	

	LPDCL pDCL = (LPDCL) m_pDCL;
	pDCL->ncflag = 0; /* Write to stdout if true */
	pDCL->fQuiet = 2;	//0; /* We want all messages.
					  // 1 = fewer messages,
					  // 2 = no messages */
	pDCL->ntflag = 0; /* test zip file if true */
	pDCL->nvflag = 0; /* give a verbose listing if true */
	pDCL->nzflag = 0; /* display a zip file comment if true */
	pDCL->ndflag = 1; /* Recreate directories != 0, skip "../" if < 2 */
	pDCL->naflag = 0; /* Do not convert CR to CRLF */
	pDCL->nfflag = 0; /* Do not freshen existing files only */
	pDCL->noflag = 1; /* Over-write all files if true */
	pDCL->ExtractOnlyNewer = 0;	/* Do not extract only newer */
	pDCL->PromptToOverwrite = 0;	/* "Overwrite all" selected -> no query mode */
	
	pDCL->lpszZipFN		= (LPSTR)lpszSourceZip;	/*argv[1];*/ /* The archive name */

	pDCL->lpszExtractDir	= (LPSTR)lpszTargetPath; /* The directory to extract to. This is set
								  to NULL if you are extracting to the
								  current directory.
								  */
	
	int	nRet	= -1;
	try
	{
		nRet	= (*(_DLL_UNZIP)m_pfnWiz_SingleEntryUnzip)(1, NULL, 0, NULL, pDCL, (LPUSERFUNCTIONS)m_pfnUserFunctions);
		TRACE(_T("Wiz_SingleEntryUnzip : %d\r\n"), nRet);
	}
	catch(...)
	{
		TRACE(_T("Wiz_SingleEntryUnzip failed for unexpected error!\r\n"));
	}	
	return	nRet == 0;
}



// KUnzip::KUnzip()
// {
// 	m_pInfoZip   = new InfoZip;
// 	m_bInited	= ((InfoZip*)m_pInfoZip)->InitializeUnzip();
// }
// KUnzip::KUnzip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath)
// {
// 	m_bInited	= ((InfoZip*)m_pInfoZip)->InitializeUnzip();
// 	if ( m_bInited )
// 		Unzip(lpszSourceZip, lpszTargetPath);
// 
// }
// KUnzip::~KUnzip()
// {
// 	if(m_bInited)
// 	{
// 		if (m_pInfoZip != NULL)
// 		{
// 			InfoZip*  p = ((InfoZip*)m_pInfoZip);
// 			p->FinalizeUnzip();				
// 			delete p;
// 
// 			m_pInfoZip = NULL;
// 		}
// 
// 		m_bInited	= FALSE;
// 	}
// }
// 


// BOOL KUnzip::Unzip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath)
// {
// 	if (!m_bInited || ::_access(lpszSourceZip,0)==-1)
// 	{		
// 		return	FALSE;
// 	}
// 
// 	if ( _access(lpszSourceZip,0)==-1 )
// 	{
//         if (!::CreateDirectory(lpszTargetPath,NULL))
// 			return FALSE;
// 
// 	}
// 	
// 
// 	return	((InfoZip*)m_pInfoZip)->ExtractFiles(lpszSourceZip, lpszTargetPath);
// }

/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
KZip::KZip()
{}

KZip::KZip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath)
{}

KZip::~KZip()
{}

BOOL KZip::Zip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath)
{
	return FALSE;
}
	

