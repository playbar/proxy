#if !defined(AFX_GARBAGECOLLECTOR1_H__26EC5F0E_DE6D_4327_A177_1BD96D1687BC__INCLUDED_)
#define AFX_GARBAGECOLLECTOR1_H__26EC5F0E_DE6D_4327_A177_1BD96D1687BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
# ifndef SAFE_DELETE
#		define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
# endif    
# ifndef SAFE_DELETE_ARRAY
#		define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
# endif
#	pragma message("*** Auto linking ATL.DLL...")
#	include <atlbase.h>
#	define	MAP_BASE	CSimpleMap		
#	define	ARRAY_BASE	CSimpleArray	
#	define	USING_ATL

typedef ARRAY_BASE<void *> SmartPtr;
typedef ARRAY_BASE<void *> SmartTempBuffer;
class CGarbageCollector  
{
public:
	CGarbageCollector();
	virtual ~CGarbageCollector();
	
	void AddPtr(void* ptr);
	void AddTempBuffer(void* TempBuffer);
public:
	SmartPtr m_autoPtr;
	SmartTempBuffer m_autoTempBuffer; 
	BOOL RemoveAll(void);
};

#endif // !defined(AFX_GARBAGECOLLECTOR1_H__26EC5F0E_DE6D_4327_A177_1BD96D1687BC__INCLUDED_)
