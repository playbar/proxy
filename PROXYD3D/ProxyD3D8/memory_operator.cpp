#include "StdAfx.h"

typedef DWORD	VFT;	// Virtual Function Table Type
typedef VFT		*LPVFT;	// Point to Virtual function table 

typedef void	*LPANYOBJECT;


#define	MAX_VFT_SIZE	1024
#define MAX_INT         MAX_VFT_SIZE

/// Common virtual object
typedef	struct tagVObject
{
	LPVFT	pVft;
}VOBJECT, *LPVOBJECT;

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL WriteMemory(LPCVOID lpAddress, LPCVOID lpData, DWORD dwBytesToWrite)
{
//	ASSERT(!::IsBadReadPtr(lpData, dwBytesToWrite));
//	ASSERT(dwBytesToWrite >= 0 && dwBytesToWrite < MAX_INT );
	MEMORY_BASIC_INFORMATION mbi	= { 0 };
	::VirtualQuery(lpAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION));  
	if(!::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect))		return	FALSE;

//	ASSERT(mbi.RegionSize >= dwBytesToWrite );
	::CopyMemory(const_cast<void*>(lpAddress), lpData, dwBytesToWrite);

	DWORD dwOldProtect	= 0L;
	return	::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect);
}

/*取虚函数个数*/
DWORD GetVirtualFunctionNumber(const LPVOBJECT pObject)
{
//	ASSERT(pObject != NULL);
	DWORD	dwCount	= 0;
	if(!pObject || !pObject->pVft )		return	0L;

	LPVFT pVft	= pObject->pVft;
	while(NULL != pVft[dwCount++]);
	return	dwCount;
}




/* 替换一个虚函数 */
DWORD SetOneVirtualFunAddr(DWORD dwNewFun,DWORD  dwIndex ,const void* pObject)
{
	DWORD   dwRet = 0;
	if  ( dwNewFun == 0  )  return dwRet;

	LPVOBJECT   pTargetObject = (LPVOBJECT) pObject;
	DWORD	dwCount		= GetVirtualFunctionNumber( pTargetObject );
    if  ( dwIndex >= dwCount)     return dwRet;

	dwRet = pTargetObject->pVft[dwIndex];
    WriteMemory(&pTargetObject->pVft[dwIndex],&dwNewFun,sizeof(DWORD));

    return dwRet;    
}

/* 取一个虚函数的地址 */
DWORD  GetOneVirtualFunAddr(DWORD dwIndex,const void* pObject)
{
	DWORD   dwRet = 0;
	LPVOBJECT   pTargetObject = (LPVOBJECT) pObject;
	DWORD	dwCount		= GetVirtualFunctionNumber( pTargetObject );
	if  ( dwIndex < dwCount)     
        dwRet = pTargetObject->pVft[dwIndex];

	return dwRet;
}