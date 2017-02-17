/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : z_Memory.h
*	共享内存封装类, add by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#pragma  once 

#include "stdio.h"

class   CShareRestrictedSD       
{   
public:   
	CShareRestrictedSD();   
	virtual   ~CShareRestrictedSD();   
	SECURITY_ATTRIBUTES* GetSA();  

	PVOID BuildRestrictedSD(PSECURITY_DESCRIPTOR   pSD); 
	VOID  FreeRestrictedSD(PVOID  ptr);  

protected:   
	PVOID     m_pVoid;   
	SECURITY_ATTRIBUTES   m_securityAttributes;   
	SECURITY_DESCRIPTOR   m_securityDescriptor;   
};   

class CShareMemoryBase  
{
public:
	enum{ READONLY,READWRITE};
private:
    CShareMemoryBase(const CShareMemoryBase&) ;
	CShareMemoryBase& operator=(const CShareMemoryBase&) ;

public:
	CShareMemoryBase ();	
	~CShareMemoryBase();

	void  Attach(LPCTSTR szname);
	void  Detach();
    
	void* CreateMem(DWORD dwSize,LPCTSTR szname = NULL);
	void  DeleteMem();

	BOOL  UpdateMem(void* p,DWORD dwSize);
private:
    void CreateFileMap();	
	void DeleteFileMap();
protected:
	HANDLE    m_hFileMap;
	void*     m_pMapViewOfFile;
	DWORD     m_dwSize;
    TCHAR*    m_pszName;

private:
	CShareRestrictedSD       m_sd;

};


/* 共享内存的模板类 
   TType: 存储的数据类型
   size : 存储的当前数据类型数量
*/

template < class TType, size_t size = 1>
class CShareMemory : private CShareMemoryBase
{
public:
	typedef    TType    _type;
private:
    CShareMemory(const CShareMemory&) ;
	CShareMemory& operator=(const CShareMemory&) ;

public:
	CShareMemory ()
	{	
		m_hFileMap = NULL;
		DWORD  dwSize = size * sizeof(TType);
		try
		{	
			CreateMem(dwSize,NULL);
		}
		catch(...)
		{
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}
		return;
	};	

	CShareMemory ( LPCTSTR szname)
	{	
		m_hFileMap = NULL;
		DWORD  dwSize = size * sizeof(TType);
		try
		{	
			CreateMem(dwSize,szname);
		}
		catch(...)
		{
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}
		return;
	}


	~CShareMemory()
	{	
		DeleteMem();
	};

	_type& operator[](int i)
	{

		static _type t;
        if ( m_hFileMap == NULL || i > size)  
		{
			throw("operator[] faile");
			return t;
		}

        return  *((_type*)((char*)m_pMapViewOfFile + i * sizeof(_type)));
	}

	BOOL Update()
	{
		BOOL bUpdate = false;

		if ( m_pMapViewOfFile == NULL )
		{
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
			//return false;
		}
        bUpdate = UpdateMem(m_pMapViewOfFile,m_dwSize);
		return bUpdate;
	}
};
