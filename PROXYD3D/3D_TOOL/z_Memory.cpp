/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : z_Memory.cpp
*	共享内存封装类, add by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#include "stdafx.h"
#include "assert.h"
#include "z_Memory.h"


CShareRestrictedSD::CShareRestrictedSD()   
{   
	m_pVoid          =   NULL;   
	m_securityAttributes.nLength   =   sizeof(m_securityAttributes);   
	m_securityAttributes.lpSecurityDescriptor   =   &m_securityDescriptor;   
	m_securityAttributes.bInheritHandle   =   FALSE;   

	m_pVoid   =   BuildRestrictedSD(&m_securityDescriptor);   
	if   (m_pVoid == NULL )   
	{   
		char szOutputDebug[256];
		sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
		OutputDebugString( szOutputDebug );
	}   
}  


CShareRestrictedSD::~CShareRestrictedSD()   
{   
	if(m_pVoid)   
	{
		FreeRestrictedSD(m_pVoid);  
	}
}   
SECURITY_ATTRIBUTES* CShareRestrictedSD::GetSA()   
{   
	if(m_pVoid != NULL )
	{   
		return   &m_securityAttributes;   
	}   
	else  
	{
		return  NULL;   
	}	
}  

PVOID CShareRestrictedSD::BuildRestrictedSD(PSECURITY_DESCRIPTOR   pSD)  
{       
	DWORD     dwAclLength = 0;       
	PSID       psidEveryone   =   NULL;   
	PACL       pDACL       =   NULL;   
	BOOL       bResult   =   FALSE;   
	PACCESS_ALLOWED_ACE   pACE   =   NULL;   
	SID_IDENTIFIER_AUTHORITY   siaWorld   =   SECURITY_WORLD_SID_AUTHORITY     ;   
	SECURITY_INFORMATION   si   =   DACL_SECURITY_INFORMATION;   

	try 
	{ 
		if(!InitializeSecurityDescriptor(pSD,SECURITY_DESCRIPTOR_REVISION))   
		{   
			//printf("InitializeSecurityDescriptor()   failed   with   error   %d\n",   GetLastError());  

			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}   

		if   (!AllocateAndInitializeSid(&siaWorld,   1,SECURITY_WORLD_RID,   0,   0,   0,   0,   0,   0,   0, &psidEveryone))
		{   
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}

		dwAclLength = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD) +  GetLengthSid(psidEveryone);   

		pDACL   =   (PACL)   HeapAlloc(GetProcessHeap(),   HEAP_ZERO_MEMORY,   dwAclLength);   
		if   (pDACL == NULL )   
		{   
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}   
		if   (!InitializeAcl(pDACL,   dwAclLength,   ACL_REVISION)) 
		{   
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}   
		if   (!AddAccessAllowedAce(pDACL,   ACL_REVISION, GENERIC_ALL,   psidEveryone))  
		{   
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}   
		if   (!SetSecurityDescriptorDacl(pSD,   TRUE,   pDACL,   FALSE))   
		{   
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}   
		bResult   =   TRUE;   

	}
	catch(...)   
	{   
		char szOutputDebug[256];
		sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
		OutputDebugString( szOutputDebug );

		if   (psidEveryone)   
			FreeSid(psidEveryone);   
	}   

	if   (bResult   ==   FALSE) 
	{   
		if  ( pDACL != NULL )  
		{
			HeapFree(GetProcessHeap(),   0,   pDACL);  
		}
		pDACL = NULL;   
	}   
	return (PVOID)pDACL;   
}   

VOID   CShareRestrictedSD::FreeRestrictedSD(PVOID   ptr)  
{   
	if (ptr != NULL)  
	{
		HeapFree(GetProcessHeap(), 0, ptr);  
	}
	return;   
}   





CShareMemoryBase::CShareMemoryBase() :
m_hFileMap(NULL),
m_pszName(NULL),
m_dwSize(0),
m_pMapViewOfFile(NULL)
{

}
CShareMemoryBase::~CShareMemoryBase()
{

}

void CShareMemoryBase::Attach(LPCTSTR szMapname)
{
	//  gLog<<_T("ZShareMemory::attach(LPCTSTR szMapname) no implement \r\n");
}
void CShareMemoryBase::Detach()
{
	//   gLog<<_T("ZShareMemory::detach() no implement \r\n");
}

void CShareMemoryBase::CreateFileMap()
{
	assert(m_dwSize != 0    );
	assert(m_hFileMap   == NULL );
	assert(m_pMapViewOfFile == NULL );

	m_hFileMap   = ::CreateFileMapping(INVALID_HANDLE_VALUE,(LPSECURITY_ATTRIBUTES)m_sd.GetSA() ,PAGE_READWRITE,0,m_dwSize,m_pszName);
	if ( m_hFileMap != NULL )
	{
		m_pMapViewOfFile = ::MapViewOfFile(m_hFileMap,FILE_MAP_WRITE|FILE_MAP_READ,0,0,m_dwSize);
	}

	assert(m_hFileMap   != NULL );
	assert(m_pMapViewOfFile != NULL );
}

void CShareMemoryBase::DeleteFileMap()
{
	if ( NULL != m_hFileMap )
	{
		::UnmapViewOfFile(m_pMapViewOfFile);
		::CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
	}
}

void* CShareMemoryBase::CreateMem(DWORD dwSize,LPCTSTR szname )
{   
	if ( NULL != m_hFileMap)   
		return NULL;

	m_dwSize = dwSize ;
	if (NULL == szname || 0 == szname[0])
	{
		m_pszName = NULL;
	}
	else
	{
		m_pszName =  new TCHAR [ _tcslen(szname) + 1];
		assert (m_pszName != NULL );
		_tcscpy(m_pszName,szname  );
	}

	CreateFileMap();
	return m_pMapViewOfFile?m_pMapViewOfFile:NULL;
}
void  CShareMemoryBase::DeleteMem()
{
	DeleteFileMap();

	if ( NULL != m_pszName)
	{
		delete [] m_pszName;
		m_pszName = NULL;
	}

	m_dwSize   = 0;
	m_pMapViewOfFile = NULL;
}


BOOL CShareMemoryBase::UpdateMem(void* p,DWORD dwSize)
{    
	if (m_pMapViewOfFile == NULL || p < m_pMapViewOfFile )  
	{
		return FALSE;
	}
	return ::FlushViewOfFile(p,dwSize);

}

