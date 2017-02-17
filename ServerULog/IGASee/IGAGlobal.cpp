#include "StdAfx.h"
#include ".\igaglobal.h"

//IGAGlobal::IGAGlobal(void)
//{
//}
//
//IGAGlobal::~IGAGlobal(void)
//{
//}


//typedef struct tagPlayInfoSee
//{
//
//}

CString AUTOAPPENDBRK(CString &str)	
{
	int sz = str.GetLength();
	if ((sz < 1) || (str.GetAt(sz-1) != ABRK) || (str.GetAt(sz-1) != BRK))
		str.AppendChar(BRK);
	return str;
}

CString GetConfigFileName()
{
	char szPath[1024] = { 0 };
	::GetModuleFileName(NULL, szPath, sizeof(szPath));

	char * pdest = ::strrchr(szPath, '.');
	*pdest = '\0';
	
	return strcat(szPath, INI_EXT);
}

BOOL CreateConfigFile()
{
	CString strFileName = GetConfigFileName();
	FILE *	pf = NULL;
	if ((pf = fopen(strFileName, "a")) == NULL)
	{
		TRACE("Create Config file is failed!\n");
		return FALSE;
	}

	fclose(pf);
	return TRUE;
}

LPCTSTR GetToken(LPCTSTR psz, int n)
{
  for (int i = 0; i < n; ++i) psz += _tcslen(psz)+1;

  return psz;
}