#include "stdafx.h"
#include "assert.h"

#include <iostream>
#include <fstream>
using namespace std;
#include "tchar.h"
#include "z_Memory.h"


PVOID BuildRestrictedSD(PSECURITY_DESCRIPTOR   pSD)  
{       
        DWORD     dwAclLength;       
        PSID       psidEveryone   =   NULL;   
    
        PACL       pDACL       =   NULL;   
        BOOL       bResult   =   FALSE;   
    
        PACCESS_ALLOWED_ACE   pACE   =   NULL;   
    
        SID_IDENTIFIER_AUTHORITY   siaWorld   =   SECURITY_WORLD_SID_AUTHORITY     ;   
          
        SECURITY_INFORMATION   si   =   DACL_SECURITY_INFORMATION;   
          
        __try 
		{       
              //   initialize   the   security   descriptor   
              if   (!InitializeSecurityDescriptor(pSD,SECURITY_DESCRIPTOR_REVISION))   
			  {   
                    printf("InitializeSecurityDescriptor()   failed   with   error   %d\n",   
                                GetLastError());   
                    __leave;   
              }   
    
              //   obtain   a   sid   for   the   Authenticated   Users   Group   
              if   (!AllocateAndInitializeSid(&siaWorld,   1,SECURITY_WORLD_RID,   0,   0,   0,   0,   0,   0,   0, &psidEveryone))
			  {   
                    printf("AllocateAndInitializeSid()   failed   with   error   %d\n",   
                                GetLastError());   
                    __leave;   
              }   
    
              //   NOTE:   
              //     
              //   The   Authenticated   Users   group   includes   all   user   accounts   that   
              //   have   been   successfully   authenticated   by   the   system.   If   access   
              //   must   be   restricted   to   a   specific   user   or   group   other   than     
              //   Authenticated   Users,   the   SID   can   be   constructed   using   the   
              //   LookupAccountSid()   API   based   on   a   user   or   group   name.   
    
              //   calculate   the   DACL   length   
              dwAclLength   =   sizeof(ACL)   
                          //   add   space   for   Authenticated   Users   group   ACE   
                          +   sizeof(ACCESS_ALLOWED_ACE)   -   sizeof(DWORD)   
                          +   GetLengthSid(psidEveryone);   
    
              //   allocate   memory   for   the   DACL   
              pDACL   =   (PACL)   HeapAlloc(GetProcessHeap(),   HEAP_ZERO_MEMORY,   dwAclLength);   
              if   (!pDACL)   
			  {   
                    printf("HeapAlloc()   failed   with   error   %d\n",   GetLastError());   
                    __leave;   
              }   
    
              //   initialize   the   DACL   
              if   (!InitializeAcl(pDACL,   dwAclLength,   ACL_REVISION)) 
			  {   
                    printf("InitializeAcl()   failed   with   error   %d\n",     
                                GetLastError());   
                    __leave;   
              }   
                
              //   add   the   Authenticated   Users   group   ACE   to   the   DACL   with   
              //   GENERIC_READ,   GENERIC_WRITE,   and   GENERIC_EXECUTE   access   
              if   (!AddAccessAllowedAce(pDACL,   ACL_REVISION, GENERIC_ALL,   psidEveryone))  
			  {   
                    printf("AddAccessAllowedAce()   failed   with   error   %d\n",             GetLastError());   
                    __leave;   
              }   
    
              //   set   the   DACL   in   the   security   descriptor   
              if   (!SetSecurityDescriptorDacl(pSD,   TRUE,   pDACL,   FALSE))   
			  {   
                    printf("SetSecurityDescriptorDacl()   failed   with   error   %d\n",     GetLastError());   
                    __leave;   
              }   
    
              bResult   =   TRUE;   
              
        }
		__finally   
		{       
              if   (psidEveryone)   FreeSid(psidEveryone);   
        }   
    
        if   (bResult   ==   FALSE) 
		{   
              if   (pDACL)   HeapFree(GetProcessHeap(),   0,   pDACL);   
              pDACL   =   NULL;   
        }   
    
        return   (PVOID)   pDACL;   
  }   
    
  //   The   following   function   frees   memory   allocated   in   the   
  //   BuildRestrictedSD()   function   
  VOID   FreeRestrictedSD(PVOID   ptr)  
  {   
    
        if   (ptr)   HeapFree(GetProcessHeap(),   0,   ptr);   
    
        return;   
  }   
    
    
  CShareRestrictedSD::CShareRestrictedSD()   
  {   
	  ptr          =   NULL;   
	  sa.nLength   =   sizeof(sa);   
	  sa.lpSecurityDescriptor   =   &sd;   
	  sa.bInheritHandle   =   FALSE;   
	  //   build   a   restricted   security   descriptor   
	  ptr   =   BuildRestrictedSD(&sd);   
	  if   (!ptr)   
	  {   
	    // TRACE("BuildRestrictedSD()   failed\n");   
	  }   
  }   
    
  CShareRestrictedSD::~CShareRestrictedSD()   
  {   
	  if(ptr)   
	    FreeRestrictedSD(ptr);   
  }   
  SECURITY_ATTRIBUTES* CShareRestrictedSD::GetSA()   
  {   
	  if(ptr)
	  {   
	     return   &sa;   
	  }   
	  else   

	  return   NULL;   
 }   

ZShareMemory::ZShareMemory() :
             m_pMap(NULL),
             m_pName(NULL),
			 m_dwSize(0),
			 m_pFirst(NULL)
{

}
ZShareMemory::~ZShareMemory()
{

}

void ZShareMemory::Attach(LPCTSTR szMapname)
{
  //  gLog<<_T("ZShareMemory::attach(LPCTSTR szMapname) no implement \r\n");
}
void ZShareMemory::Detach()
{
 //   gLog<<_T("ZShareMemory::detach() no implement \r\n");
}

void ZShareMemory::_createfilemap()
{
	assert(m_dwSize != 0    );
	assert(m_pMap   == NULL );
	assert(m_pFirst == NULL );
	
	m_pMap   = ::CreateFileMapping(INVALID_HANDLE_VALUE,(LPSECURITY_ATTRIBUTES)m_sd.GetSA() ,PAGE_READWRITE,0,m_dwSize,m_pName);
	if ( m_pMap != NULL )
		m_pFirst = ::MapViewOfFile(m_pMap,FILE_MAP_WRITE|FILE_MAP_READ,0,0,m_dwSize);
	else
//		gLog<<_T("CreateFileMapping <")<<m_pName<< _T(">\r\n");
	

	assert(m_pMap   != NULL );
	assert(m_pFirst != NULL );
}

void ZShareMemory::_deletefilemap()
{
    if ( NULL != m_pMap )
	{
		::UnmapViewOfFile(m_pFirst);
		::CloseHandle(m_pMap);
		m_pMap = NULL;
	}
}

void* ZShareMemory::CreateMem(DWORD dwSize,LPCTSTR szname )
{   
	if ( NULL != m_pMap)   return NULL;

	m_dwSize = dwSize ;
	if (NULL == szname || 0 == szname[0])
       m_pName = NULL;
	else
	{
       m_pName =  new TCHAR [ _tcslen(szname) + 1];
	   assert (m_pName != NULL );
	   _tcscpy(m_pName,szname  );
	}

    _createfilemap();
	return m_pFirst?m_pFirst:NULL;
}
void  ZShareMemory::DeleteMem()
{
    _deletefilemap();

	if ( NULL != m_pName)
	{
		delete [] m_pName;
	    m_pName = NULL;
	}

	m_dwSize   = 0;
	m_pFirst = NULL;
}


BOOL ZShareMemory::UpdateMem(void* p,DWORD dwSize)
{    
    if (m_pFirst == NULL || p < m_pFirst )  return FALSE;
	return ::FlushViewOfFile(p,dwSize);
}












  