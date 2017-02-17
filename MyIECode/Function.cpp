#include "stdafx.h"
#include "MyIE.h"
#include "resource.h"       // main symbols
#include "wininet.h"		//for INTERNET_MAX_PATH_LENGTH
#include "mainfrm.h"
#include "MenuData.h"
//
#include <sys\stat.h>	//for struct stat
#include <direct.h>		//for mkdir

//##############################################################
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//##############################################################
#define MAX_INI		512
#define _NULL		-1

//##############################################################
//for plugin
CString _GetPlugInSection(CString strFileName, CString strSection)
{
	CString strOut;
	strOut.Empty();
	
	char tmp[INTERNET_MAX_PATH_LENGTH];
	if (::GetPrivateProfileString("General", strSection, NULL, tmp, INTERNET_MAX_PATH_LENGTH, strFileName))
		strOut = tmp;
	return strOut;
}

//0:not 1:script 2:exe
int _GetPlugInType(CString strFileName)
{
	CString strTemp2 = GET_FILE(strFileName);
	strTemp2.MakeLower();
	if (strTemp2.Left(10)=="plugin.ini")
	{
		CString strType = _GetPlugInSection(strFileName,"Type");
		if (strType=="BUTTON" || strType=="M2Plugin_BUTTON")
		{
			CString strSection = _GetPlugInSection(strFileName,"ModuleType");
			if (strSection=="SCRIPT")
				return 1;
			else if (strSection=="EXE")
				return 2;
		}
	}
	return 0;
}

CString _GetPlugInIconPath(CString strFileName)
{
	CString strOut;
	strOut.Empty();

	char tmp[INTERNET_MAX_PATH_LENGTH];
	if (::GetPrivateProfileString("General", "HotIcon", NULL, tmp, INTERNET_MAX_PATH_LENGTH, strFileName)==FALSE)
	{
		if (::GetPrivateProfileString("General", "Icon", NULL, tmp, INTERNET_MAX_PATH_LENGTH, strFileName)==FALSE)
			return strOut;
	}
	strOut = strFileName.Left(strFileName.ReverseFind('\\')+1) + tmp;
	return strOut;
}

CString _GetPlugInName(CString strFileName)
{
	return _GetPlugInSection(strFileName, "Name" );
}

CString _GetPlugInComments(CString strFileName)
{
	return _GetPlugInSection(strFileName, "Comments" );
}

CString _GetPlugInFileName(CString strFileName)
{
	return _GetPlugInSection(strFileName, "FileName" );
}

//##############################################################
//file common function
char* _FileGetBuffer(CString filename)
{
	CFile f;
	if( f.Open(filename, CFile::modeRead|CFile::shareDenyNone) == FALSE)
		return NULL;
	DWORD len = f.GetLength();
	if (len<=0)
		return NULL;
	char* tmp = (char*)malloc(len+1);
	f.Read(tmp, len);
	tmp[len]=0;
	f.Close();
	return tmp;
}

BOOL _BrowserPath(CWnd* pParentWnd, CString& m_strPath)
{
	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc)!= NOERROR)
		return FALSE;
	CString str;
	LOADSTR(str ,IDS_SELECT_PATH);

	BROWSEINFO bInfo;
	LPITEMIDLIST pidl;
	ZeroMemory ( (PVOID) &bInfo,sizeof (BROWSEINFO));

	bInfo.hwndOwner		 = pParentWnd->GetSafeHwnd();
	bInfo.pszDisplayName = m_strPath.GetBuffer (MAX_PATH);
	bInfo.lpszTitle		 = str;//_T("Select Path:");
	bInfo.ulFlags		 = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS;
	bInfo.lpfn			 = NULL;//!BrowseCtrlCallback;  // address of callback function
	bInfo.lParam		 = NULL;//!(LPARAM)this;		// pass address of object to callback function
	
	if ((pidl = ::SHBrowseForFolder(&bInfo)) == NULL)
		return FALSE;
	m_strPath.ReleaseBuffer();
	//!m_iImageIndex = bInfo.iImage;
	
	if (::SHGetPathFromIDList(pidl,m_strPath.GetBuffer(MAX_PATH)) == FALSE)
	{
		pMalloc ->Free(pidl);
		pMalloc ->Release();
		return FALSE;
	}	
	m_strPath.ReleaseBuffer();
	pMalloc ->Free(pidl);
	pMalloc ->Release();
	
	return TRUE;
}

//##############################################################
BOOL _FileIsExist(CString strPath)
{
	//必须这样做,因为 末尾带 \ 的 FindFirstFile stat 中不能用
	int nSlash = strPath.ReverseFind('\\');
	if( nSlash == strPath.GetLength()-1)
		strPath = strPath.Left(nSlash);
	/*
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile( strPath, &fd );
	if ( hFind != INVALID_HANDLE_VALUE )
		FindClose( hFind );
	return hFind != INVALID_HANDLE_VALUE;
	*/
	struct stat stat_buf;
	if (stat(strPath, &stat_buf) >= 0)
		return TRUE;
	else
		return FALSE;
}

void _CreateFullDir(const char *dir)
{
    struct stat stat_buf;
	const char *p;
	char parent[MAX_PATH];
	int length;

	if (_FileIsExist(dir))
		return;
    if ( strlen(dir) == 0) 
		return;
	p = strchr(dir,'\\');
	while(p)
	{
		length = p-dir;
		strncpy(parent,dir,length);
		*(parent+length) = 0;
		//make dir
		if (stat(parent, &stat_buf) < 0 && strlen(parent) > 2)//can not mkdir("c:")
		{
			if ( mkdir(parent)!=0 )
				return;
		}
		//move to next
		if (*(p+1)==0)
			return;
		p = strchr(p+1,'\\');
	}
	//for c:\\aa\bb\cc\\dd,mow create dd
	if (stat(dir, &stat_buf) < 0 && strlen(dir) > 2)//can not mkdir("c:")
	{
		if ( mkdir(dir)!=0 )
			return;
	}
}

//#######################################################################
//国际 URL
#define IS_1URL(strUrl)		  ( strUrl.Right(5)==".com/" ||		\
								strUrl.Right(5)==".net/" ||		\
								strUrl.Right(5)==".org/" ||		\
								strUrl.Right(5)==".edu/" ||		\
								strUrl.Right(4)==".com"  ||		\
								strUrl.Right(4)==".net"  ||		\
								strUrl.Right(4)==".org"  ||		\
								strUrl.Right(4)==".edu" )
//2级 URL
#define IS_2URL(nDot,strUrl)  ( (nDot=strUrl.Find(".com."))>0 ||	\
								(nDot=strUrl.Find(".net."))>0 ||	\
								(nDot=strUrl.Find(".org."))>0 ||	\
								(nDot=strUrl.Find(".edu."))>0 )

//##############################################################
CString _StringGetUpUrl(CString strUrl)
{
	if (!strUrl || strUrl.GetLength()<=0)
		return strUrl;
	if (strUrl=="about:blank")
		return strUrl;
	//if (strUrl.Left(6)=="res://")
	//	return strUrl;
	// must have / and \\ 
	int c,b;
	c = strUrl.Find('/');
	if (c==5 || c==6 || c==4) // file:///   http://   https:// ftp://
	{
		c = '/'; 
		b = 8; // https://
	}
	else if (strUrl.Find('\\')>=0)
	{
		c = '\\';
		if (strUrl.GetAt(1)==':')
			b = 3; // c:\a\b
		else
			b = 2; // \\server1
	}
	else
		return strUrl;
	//
	int len =  strUrl.GetLength();
	int pos1 = strUrl.Find(c,b);	//get first c
	int pos2 = strUrl.ReverseFind(c);

	if ( (pos1<0) || (pos1+1==len && b!=3) )// \\server1   \\server1\  ,must except c:\temp\ 
	{
		if (c=='/')
		{
			// http://mail.qwe.com/ , http://ror.cn/, http://mail.qwe.com.cn/
			if(	(strUrl.Left(7)=="http://" && strUrl.Left(11)!="http://www.") || 
				(strUrl.Left(8)=="https://" && strUrl.Left(12)!="https://www.")
				)
			{
				int nDot=0;
				CString strTemp;
				if ( IS_1URL(strUrl) )
				{
					// http://login.bj.yahoo.com/
					nDot = strUrl.ReverseFind('.');
					strTemp = strUrl.Left(nDot);
					nDot = strTemp.ReverseFind('.');
				}
				else if ( IS_2URL(nDot, strUrl) )
				{
					strTemp = strUrl.Left(nDot);
					nDot = strTemp.ReverseFind('.');
				}
				else if ( strUrl.Find('.') < strUrl.ReverseFind('.')) // http://news.sh.cn/
				{
					int pos1 = strUrl.Find("://");
					pos1 += 3;
					strTemp = strUrl.Mid(pos1);
					if (strTemp.FindOneOf("abcdefghijklmnopqrstuvwxyz")>=0) //http://127.0.0.1/
						nDot = strUrl.Find('.',pos1);
				}
				//replace str as www
				if (nDot>0)
				{
					// http://news.newscn.com/ => http://www.newscn.com/
					int pos1 = strUrl.Find("://");
					pos1 += 3;
					strTemp  = strUrl.Left(pos1);
					strTemp += "www";
					strTemp += strUrl.Mid(nDot);
					strUrl = strTemp;
				}
			}
		}
		return strUrl;
	}
	else if (pos2+1==len)// \\server1\mail\xxx\ 
	{
		strUrl = strUrl.Left(pos2);
		pos2   = strUrl.ReverseFind(c);
	}
	strUrl = strUrl.Left(pos2+1); // \\server1\mail\xxx 
	return strUrl;
}

CString _StringGetRootUrl(CString strUrl)
{
	if (!strUrl || strUrl.GetLength()<1)
		return strUrl;

	if (!IS_URL(strUrl))
		return strUrl;
	int nIndex = 0;
	CString strUrlUp;
	while(nIndex < 100)
	{
		strUrlUp = _StringGetUpUrl(strUrl);
		if (strUrlUp == strUrl)
			break;
		strUrl = strUrlUp;
		//
		nIndex++;
	}
	return strUrl;
}

//##############################################################
//return add ok or not
BOOL _GroupAddItem(CString strName, CString strUrl, BOOL bState, DWORD dwProperty, CString filename)
{
	char state[10]="state",name[9]="name",url[8]="url",download[13]="download"; // x+5
	char num[15];
	int i=0;
	char tmp[INTERNET_MAX_PATH_LENGTH];
	BOOL r = TRUE, bNeedAdd=TRUE;
	DWORD dwPropertyTemp;
	CString strNameTemp,strUrlTemp;
	//get if exist the url
	while(r)
	{
		itoa(i, name+4, 10);
		itoa(i, url+3, 10);
		r = ::GetPrivateProfileString("Group", name, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
		if (r)
		{
			strNameTemp = tmp;
			r = ::GetPrivateProfileString("Group", url, NULL, tmp, INTERNET_MAX_PATH_LENGTH, filename);
			if (r)
			{
				strUrlTemp = tmp;
				itoa(i, download+8, 10);
				dwPropertyTemp = ::GetPrivateProfileInt("Group", download, DEFAULT_PROPERTY, filename);
				if (strName==strNameTemp && strUrl==strUrlTemp && dwProperty==dwPropertyTemp)
				{
					r = FALSE;//no use add the item
					bNeedAdd = FALSE;
				}
			}
		}
		i++;
	}
	if (bNeedAdd)
	{
		if (i>0)
			i--;
		itoa(i, state+5, 10);
		itoa(i, name+4, 10);
		itoa(i, url+3, 10);
		itoa(i, download+8, 10);
		//
		::WritePrivateProfileString("Group", name, strName, filename);
		::WritePrivateProfileString("Group", url, strUrl, filename);
		if (bState==0)
		{
			itoa( 0, num, 10);
			::WritePrivateProfileString("Group", state, num, filename);
		}
		if (dwProperty != DEFAULT_PROPERTY)
		{
			ultoa( dwProperty, num, 10);
			::WritePrivateProfileString("Group", download, num, filename);
		}
	}
	return bNeedAdd;
}

//##############################################################
DWORD _RegGetDword(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName)
{
	DWORD           sz;
	HKEY            hKey;
	DWORD           dwSize = sizeof(sz);

	if(RegOpenKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
		return NULL;
	if (RegQueryValueEx(hKey, _T(lpValueName), NULL, NULL, (LPBYTE)&sz, &dwSize) != ERROR_SUCCESS)
		sz = NULL;
	RegCloseKey(hKey);

	return sz;
}

BOOL _RegSetDword(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName, DWORD dwValue)
{
	BOOL			bRet=FALSE;
	HKEY            hKey;
	DWORD           dwSize = sizeof(dwValue);

	if(RegOpenKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
	{
		if(RegCreateKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
			return bRet;
	}
	if (RegSetValueEx(hKey, _T(lpValueName), NULL, REG_DWORD, (LPBYTE)(&dwValue), dwSize) == ERROR_SUCCESS)
		bRet = TRUE;
	RegCloseKey(hKey);

	return bRet;
}

CString _RegGetString(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName)
{
	CString			strRet;
	HKEY            hKey;
	TCHAR           sz[MAX_PATH];
	DWORD           dwSize = sizeof(sz);

	strRet.Empty();
	memset(sz,0,MAX_PATH);
	if(RegOpenKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
		return strRet;
	if (RegQueryValueEx(hKey, _T(lpValueName), NULL, NULL, (LPBYTE)sz, &dwSize) == ERROR_SUCCESS)
		strRet = sz;
	RegCloseKey(hKey);

	return strRet;
}

BOOL _RegSetString(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName, LPCSTR lpValue)
{
	HKEY            hKey;

	if(RegOpenKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
	{
		if(RegCreateKey(hKeyRoot, _T(lpSubKey), &hKey) != ERROR_SUCCESS)
			return FALSE;
	}
	RegSetValueEx(hKey, _T(lpValueName), 0, REG_SZ, (LPBYTE)lpValue, strlen(lpValue));
	RegCloseKey(hKey);

	return TRUE;
}

void _RegDeletePath(HKEY hKey, CString strPath)
{
	TCHAR   sz[MAX_PATH];
	HKEY	hKeySub;
	DWORD	dwSize = MAX_PATH;
	LONG	lRet;

	if (RegOpenKey(hKey, strPath, &hKeySub) != ERROR_SUCCESS)
		return ;
	//not i++, for that have delete one
	while(RegEnumKey(hKeySub, 0, sz, dwSize) == ERROR_SUCCESS)
	{
		lRet = RegDeleteKey(hKeySub,sz);
		if (lRet != ERROR_SUCCESS)
			_RegDeletePath(hKeySub, sz);
	}
	RegCloseKey(hKeySub);
	//delete the path
	RegDeleteKey(hKey,strPath);
}

//##############################################################
void _SetClipString(CString str)
{
	int len = str.GetLength();

	if ( !::OpenClipboard(NULL) )
		return;
	// Remove the current Clipboard contents
	if( !EmptyClipboard() )
		return;
	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, len+1);
	if (hData == NULL)
		return;
	strcpy((LPSTR)GlobalLock(hData), (LPCSTR)str);
	GlobalUnlock(hData);
	// For the appropriate data formats...
	SetClipboardData( CF_TEXT, hData );
	CloseClipboard();
	//GlobalFree(hData);
}

CString _GetClipString()
{
	CString strUrl;
	strUrl.Empty();

	if ( !::OpenClipboard(NULL) )
		return strUrl;
	// For the appropriate data formats...
	HGLOBAL hData = ::GetClipboardData( CF_TEXT);
	if ( hData == NULL )
	{
		CloseClipboard();
		return strUrl;
	}
	// ...
	DWORD nSize = GlobalSize(hData);
	if (nSize > 0)
	{
		char *str = (char*)GlobalLock(hData);
		strUrl = str;
	}
	GlobalUnlock(hData);
	CloseClipboard();

	return strUrl;
}

//##############################################################
void _InitLanguageFile(LPCSTR lpFileName)
{
	FREENULL(g_strLngBuf);

	if (!theApp.m_bUseLngFile)
		return;
	g_strLngBuf = _FileGetBuffer(lpFileName);
	if (g_strLngBuf == NULL)
	{
		theApp.m_bUseLngFile = FALSE;
	}
}

DWORD _GetPrivateProfileString(
    LPCSTR lpAppName,
    LPCSTR lpKeyName,
    LPCSTR lpDefault,
    LPSTR lpReturnedString,
    DWORD nSize,
    LPCSTR lpFileName
    )
{
	char strTemp[50];
	char *p1,*p2;
	sprintf(strTemp,"[%s]",lpAppName);
	p1 = strstr(g_strLngBuf,strTemp);
	if (p1)
	{
		p1 += strlen(strTemp);
		p2 = strstr(p1,"]\r");
		sprintf(strTemp,"\r\n%s=",lpKeyName);
		p1 = strstr(p1,strTemp);
		if ( p1 && 
			(!p2 || (p2 && p1<p2))
			)
		{
			p1 += strlen(strTemp);
			p2 = strchr(p1,'\r');
			if (p2)
			{
				int len = min( (DWORD)(p2-p1), nSize);
				strncpy(lpReturnedString, p1, len);
				*(lpReturnedString + len) = 0;
				return len;
			}
		}
	}
	if (lpDefault)
	{
		strcpy(lpReturnedString, lpDefault);
		return strlen(lpDefault);
	}
	else
	{
		*lpReturnedString = 0;
		return 0;
	}
}

void _LoadString(CString &str, UINT id)
{
	char num[10];
	char buf[MAX_INI];

	itoa(id,num,10);
	if (_GetPrivateProfileString("StringTable", num, NULL, buf, MAX_INI, theApp.m_strLngFile))
	{
		str = buf;
		if (str.Find("\\r")>=0)
			str.Replace("\\r","\r");
		if (str.Find("\\n")>=0)
			str.Replace("\\n","\n");
	}
	else
		str.LoadString(id);
}

void _LoadDlgString(CWnd *pDlg, CString strDomain)
{
	char num[10];
	char buf[MAX_INI];
	//
	int id;
	CWnd *pWnd=NULL;
	CString strText;
	DWORD r;
	
	if (_GetPrivateProfileString(strDomain, "Caption", NULL, buf, MAX_INI, theApp.m_strLngFile))
		pDlg->SetWindowText(buf);
	//
	pWnd = pDlg->GetNextDlgGroupItem((CWnd *)NULL);
	while(pWnd)
	{
		id = pWnd->GetDlgCtrlID();
		itoa(id,num,10);
		if (id<10)
		{
			if (_GetPrivateProfileString("StringGeneral", num, NULL, buf, MAX_INI, theApp.m_strLngFile))
				pWnd->SetWindowText(buf);
		}
		else
		{
			if (r = _GetPrivateProfileString(strDomain, num, NULL, buf, MAX_INI, theApp.m_strLngFile))
			{
				if (*buf=='\\' && *(buf+1)=='n')//combo box
				{
					char str[MAX_INI];
					char *pos1,*pos2;
					int i=0,len;
					CComboBox *pCb = (CComboBox *)pWnd;
					int nCurSel = pCb->GetCurSel();
					pCb->ResetContent();
					pos1 = buf + 2;
					while (1)
					{
						pos2 = strstr(pos1,"\\n");
						if (!pos2)
							pos2 = buf + r;
						len = pos2 - pos1;
						strncpy(str,pos1,len);
						*(str+len) = 0;
						pCb->InsertString(i,str);
						i++;
						pos1 = pos2+2;
						if (pos1>=buf+r)
							break;
					}
					pCb->SetCurSel(nCurSel);
				}
				else if ( strstr(buf,"\\r") || strstr(buf,"\\n"))
				{
					CString str=buf;
					str.Replace("\\r","\r");
					str.Replace("\\n","\n");
					pWnd->SetWindowText(str);
				}
				else
					pWnd->SetWindowText(buf);
			}
		}
		pWnd = pWnd->GetNextWindow();
	}
}

void _GetMenuItemString(CString &strName, int nID)
{
	if (nID==0)
	{
		LOADSTR(strName,IDS_NONE);
		return ;
	}
	CMenu * pMenu = pmf->GetMenu();
	//
	MENUITEMINFO mii; 
	CMenuData* pmd;
	if(pMenu!=NULL)
	{
		mii.fMask = MIIM_DATA|MIIM_TYPE;    // get the state of the menu item
		mii.dwTypeData = NULL;
		mii.cch = 0;
		mii.dwItemData = NULL;
		mii.cbSize = sizeof(mii);
		pMenu->GetMenuItemInfo(nID, &mii, FALSE); 
		pmd = (CMenuData*)(mii.dwItemData);
		if(mii.fType & MFT_OWNERDRAW && pmd!=NULL)
			strName = pmd->m_szMenuText;
		else
			strName = "";
	}
	strName.Remove('&');
	int pos = strName.Find('\t');
	if (pos>=0)
		strName = strName.Left(pos);
}

void _LoadMenuString2ComboBox(CMenu *pMenu, CComboBox *pCb)
{
	char num[10];
	char buf[MAX_INI];
	//
	CMenu *pSubMenu;
	int value,j,nID;
	UINT i=0,itemCount,state;
	char key[10]="";
	CString strName,strPop;
	static int index=0;
	static char aIndex[5];
	static char nIndex=_NULL;
	static CString strIndex[5];
	BOOL r = TRUE;
	BOOL bNeedReadRes = TRUE;
	//
	if (pMenu == pmf->GetMenu())
	{
		CString strItem;
		LOADSTR(strItem,IDS_NONE);
		pCb->InsertString(0,strItem);
		index=1;
	}
	//
	nIndex++;
	//
	itemCount = pMenu->GetMenuItemCount();
	for(i=0;i<itemCount;i++)
	{
		state=pMenu->GetMenuState(i,MF_BYPOSITION);
		if(state&MF_POPUP)
		{
			bNeedReadRes = TRUE;
			aIndex[nIndex] = i;
			aIndex[nIndex+1] = _NULL;
			if (theApp.m_bUseLngFile)
			{	
				j=0;
				key[0]=0;
				while( (value=aIndex[j++]) != _NULL)
				{
					strcat(key,"X");
					itoa(value, num, 10);
					strcat(key,num);
				}
				r = _GetPrivateProfileString("MenuMain", key, NULL, buf, MAX_INI, theApp.m_strLngFile);
				if (r)
					bNeedReadRes = FALSE;
			}
			//read string from res
			if (bNeedReadRes)
				pMenu->GetMenuString( i, buf, countof(buf)-1, MF_BYPOSITION);//::GetMenuString(hSubMenu, i, name, countof(name)-1, MF_BYPOSITION);
			//
			{
				CString strName;
				strName = buf;
				strName.Replace("\\t","\t");
				strName.Remove('&');
				int pos = strName.Find('\t');
				if (pos>=0)
					strName = strName.Left(pos);
				if (strName.GetLength())
					strIndex[nIndex] = strName;
			}
			//
			pSubMenu = pMenu->GetSubMenu(i);
			if (pSubMenu)
				_LoadMenuString2ComboBox(pSubMenu, pCb);
		}
		else if(state&MF_SEPARATOR)
			;
		else
		{
			nID = pMenu->GetMenuItemID(i);
			//undo close
			if (nID>=B_UNDO && nID<=B_UNDO+MAX_UNDO)
				goto _myexit;
			//add url to this fav
			if (nID>=B_ADDTO_FAV && nID<=E_ADDTO_FAV)
				goto _myexit;
			//add url to this group
			if (nID>=B_ADDTO_GROUP && nID<=E_ADDTO_GROUP)
				goto _myexit;
			//search
			if (nID>=B_SEARCH && nID<=E_SEARCH)
				goto _myexit;
			//
			CString strPop;
			strPop.Empty();
			j=0;
			while(strIndex[j].GetLength())
			{
				strPop += strIndex[j];
				strPop += " \\ ";
				j++;
			}
			_GetMenuItemString(strName,nID);
			strName = strPop + strName;
			if (strName.GetLength())
			{
				pCb->InsertString(index,strName);
				pCb->SetItemData(index,nID);
				index++;
			}
		}
	}
_myexit:
	nIndex--;
	if (nIndex>-1)
	{
		aIndex[nIndex] = _NULL;
		strIndex[nIndex] = "";
	}
}

void _LoadMenuString(CMenu *pMenu, CString strDomain)
{
	char num[10];
	char buf[MAX_INI];
	//
	CMenu *pSubMenu;
	int value,j,nID;
	UINT i=0,itemCount,state;
	static char aIndex[5];
	static char nIndex=_NULL;
	char key[10]="";
	CString strName;
	BOOL r = TRUE;
	
	nIndex++;
	//
	itemCount = pMenu->GetMenuItemCount();
	for(i=0;i<itemCount;i++)
	{
		state=pMenu->GetMenuState(i,MF_BYPOSITION);
		if(state&MF_POPUP)
		{	
			aIndex[nIndex] = i;
			aIndex[nIndex+1] = _NULL;
			j=0;
			key[0]=0;
			while( (value=aIndex[j++]) != _NULL)
			{
				strcat(key,"X");
				itoa(value, num, 10);
				strcat(key,num);
			}
			r = _GetPrivateProfileString(strDomain, key, NULL, buf, MAX_INI, theApp.m_strLngFile);
			if (r)
			{
				strName = buf;
				strName.Replace("\\t","\t");
				pMenu->ModifyMenu( i, MF_BYPOSITION|MF_STRING, 0, strName);
			}

			pSubMenu = pMenu->GetSubMenu(i);
			if (pSubMenu)
				_LoadMenuString(pSubMenu,strDomain);
		}
		else if(state&MF_SEPARATOR)
			;
		else
		{
			nID = pMenu->GetMenuItemID(i);
			itoa(nID,num,10);
			r = _GetPrivateProfileString(strDomain, num, NULL, buf, MAX_INI, theApp.m_strLngFile);
			if (r)
			{
				strName = buf;
				strName.Replace("\\t","\t");
				pMenu->ModifyMenu( i, MF_BYPOSITION|MF_STRING, nID, strName);
			}
		}
	}
	nIndex--;
	if (nIndex>-1)
		aIndex[nIndex] = _NULL;
}

int GetPopMenuIconID(char aIndex[5])
{
	switch (aIndex[0])
	{
	case 0://file
		{
			switch (aIndex[1])
			{
			case 0: return 9;
			case 19:return 65;
			}
		}
		break;
	case 2://view menu
		{
			switch (aIndex[1])
			{
			case 4: return 8;
			case 5:return 14;
			case 9:return 27;
			}
		}
		break;
	case 4://group
		{
			switch (aIndex[1])
			{
			case 2: return 69;
			}
		}
		break;
	case 5://tools
		{
			switch (aIndex[1])
			{
			case 0: return 26;
			case 2: return 37;
			case 3: return 33;
			case 4: return 10;
			case 7: return 38;
			case 8: return 25;
			}
		}
		break;
	}
	return -1;
}

int GetMenuIconID(int nID)
{
	switch (nID)
	{
	case ID_GO_BACK:			return 0;
	case ID_GO_FORWARD:			return 1;
	case ID_RTAB_STOP:
	case ID_VIEW_STOP:			return 2;
	case ID_RTAB_REFRESH:
	case ID_VIEW_REFRESH:		return 3;
	case ID_FILE_NEW_S:			return 4;
	case ID_GO_START_PAGE:		return 4;
	case ID_ALL_PAGE_LINKS:		return 5;
	case ID_FILE_PRINT:			return 7;
	//8FONT-ok
	case ID_FILE_NEW_BLANK:		return 9;
	//case ID_OPTIONS_LOADIMAGE:	return 10;
	case ID_VIEW_FULLSCREEN:	return 11;
	case ID_TOOLS_EMAIL:		return 13;
	//14ENCODING-ok
	case ID_GO_UP_PAGE:			return 15;
	case ID_VIEW_AUTOHIDE:		return 16;
	//17 WEBPROXY
	//18 PROXY
	case ID_FILE_WORKOFFLINE:	return 19;
	case ID_EDIT_EDIT:			return 20;
	case ID_FILE_OPENF:			return 21;
	//22 STOP OPEN
	//25 skin
	case ID_TOOLS_AUTOSCROLL_USEAUTOSCROLLING:		return 26;
	case ID_EDIT_UNDOCLOSE:		return 27;
	//case ID_RTAB_AUTOSAVE:
	case ID_FILE_SAVEAS:		return 28;
	case ID_EDIT_COPY:			return 29;
	case ID_OPTIONS_MyIEOPTIONS:return 30;
	case ID_VIEW_STOP_ALL:		return 31;
	case ID_VIEW_REFRESHALL:	return 32;
	case ID_TOOLS_SETBG:		return 33;
	case ID_ORGGROUP:			return 34;
	case ID_EDIT_FIND:			return 35;
	//36 NEXT
	case ID_SC_FILLFORM:		return 37;
	case ID_TOOLS_ALL:			return 38;
	//38 CLEAN HIS
	case ID_EDIT_PASTE:			return 39;

	//IDB_SYSMENU
	//case ID_VIEW_WIN_MIN:
	case ID_WINDOW_MIN:			return 40;
	//case ID_VIEW_WIN_RESTORE:
	case ID_WINDOW_RESTOR:		return 41;
	//case ID_VIEW_WIN_CLOSE:
	case ID_FILE_CLOSE_1:
	case ID_FILE_CLOSE_2:		return 42;
	//case ID_VIEW_WIN_CLOSEALL:
	case ID_FILE_CLOSEALL:		return 43;
	//case ID_VIEW_WIN_NEXT:
	case ID_TAB_NEXT:		return 44;
	//case ID_VIEW_WIN_PRE:
	case ID_TAB_PRE:		return 45;

	case ID_TOOLS_COLLECT:		return 46;
	case ID_APP_EXIT:			return 47;
	case ID_VIEW_LOCKTOOLBAR:	return 48;
	case ID_EDIT_CUT:			return 49;
	//case ID_FILE_WORKOFFLINE:	return 50;
//51 add here fav
//52 open all fav
	case ID_RTAB_ADDFAV:
	case ID_FAVORITES_ADDTOFAVORITES:	return 53;
	case ID_FAVORITES_ORGNIZEFAVORITES:	return 54;
	case ID_WINDOW_TILE_VERT:			return 55;
	case ID_WINDOW_TILE_HORZ:			return 56;
	case ID_WINDOW_CASCADE:		return 57;
	case ID_HELP_HELP:			return 58;
	case ID_HELP_HOMEPAGE:		return 59;
	case ID_APP_ABOUT:			return 60;
	case ID_TOOLS_SYNC:			return 61;
//62
	case ID_FAVORITES_IMPORTFAVORITES:	return 63;
	case ID_FAVORITES_EXPORTFAVORITES:	return 64;
//65
	case ID_FILE_SENDPAGEBYEMAIL:		return 66;
	case ID_FILE_SENDLINKBYEMAIL:		return 67;
	case ID_TOOLS_ADDR:					return 68;

	//
	case ID_ADDTO_IE:					return 71;

	default:	return -1;
	}
}

void _LoadMenuStringOwnerDraw(CMenu *pMenu, CString strDomain,CImageList *imgList)
{
	char num[10];
	char buf[MAX_INI];
	//
	CMenu *pSubMenu;
	int value,j,nID;
	UINT i=0,itemCount,state;
	static char aIndex[5];
	static char nIndex=_NULL;
	char key[10]="";
	CString strName;
	BOOL r = TRUE;
	BOOL bNeedReadRes = TRUE;
	//
	CMenuData * md = NULL;
	MENUITEMINFO mii;
	
	nIndex++;
	//
	itemCount = pMenu->GetMenuItemCount();
	for(i=0;i<itemCount;i++)
	{
		md= new CMenuData;
		md->m_pImgList = imgList;
		//
		state=pMenu->GetMenuState(i,MF_BYPOSITION);
		if(state&MF_POPUP)
		{
			bNeedReadRes = TRUE;
			aIndex[nIndex] = i;
			aIndex[nIndex+1] = _NULL;
			if (theApp.m_bUseLngFile)
			{	
				j=0;
				key[0]=0;
				while( (value=aIndex[j++]) != _NULL)
				{
					strcat(key,"X");
					itoa(value, num, 10);
					strcat(key,num);
				}
				r = _GetPrivateProfileString(strDomain, key, NULL, buf, MAX_INI, theApp.m_strLngFile);
				if (r)
				{
					strName = buf;
					strName.Replace("\\t","\t");
					pMenu->ModifyMenu( i, MF_BYPOSITION|MF_STRING|MF_POPUP, (UINT)pMenu->GetSubMenu(i)->m_hMenu, strName);
					//
					SetMenuText(strName,md);
					bNeedReadRes = FALSE;
				}
			}
			//read string from res
			if (bNeedReadRes)
			{
				pMenu->GetMenuString( i, buf, countof(buf)-1, MF_BYPOSITION);//::GetMenuString(hSubMenu, i, name, countof(name)-1, MF_BYPOSITION);
				SetMenuText(buf,md);
			}
			md->m_nBitmap = GetPopMenuIconID(aIndex);
			//
			mii.cbSize = sizeof(mii);
			mii.fMask = MIIM_DATA|MIIM_TYPE;
			mii.fType = MFT_STRING|MFT_OWNERDRAW;
			mii.dwItemData = (DWORD)md;
			::SetMenuItemInfo(pMenu->m_hMenu, i, TRUE, &mii);
			//
			pSubMenu = pMenu->GetSubMenu(i);
			if (pSubMenu)
				_LoadMenuStringOwnerDraw(pSubMenu,strDomain,imgList);
		}
		else if(state&MF_SEPARATOR)
		{
			md->m_pImgList = NULL;
			md->m_nBitmap = -1;
			mii.cbSize = sizeof(mii);
			mii.fMask = MIIM_DATA|MIIM_TYPE;
			mii.fType = MF_SEPARATOR|MFT_OWNERDRAW;
			mii.dwItemData = (DWORD)md;
			::SetMenuItemInfo(pMenu->m_hMenu, i, TRUE, &mii);
		}
		else
		{
			bNeedReadRes = TRUE;
			nID = pMenu->GetMenuItemID(i);
			if (theApp.m_bUseLngFile)
			{
				itoa(nID,num,10);
				r = _GetPrivateProfileString(strDomain, num, NULL, buf, MAX_INI, theApp.m_strLngFile);
				if (r)
				{
					strName = buf;
					strName.Replace("\\t","\t");
					pMenu->ModifyMenu( i, MF_BYPOSITION|MF_STRING, nID, strName);
					SetMenuText(strName,md);
					bNeedReadRes = FALSE;
				}
			}
			//
			if (bNeedReadRes)
			{
				pMenu->GetMenuString( i, buf, countof(buf)-1, MF_BYPOSITION);
				SetMenuText(buf,md);
			}
			md->m_nBitmap = GetMenuIconID(nID);
			//
			mii.cbSize = sizeof(mii);
			mii.fMask = MIIM_DATA|MIIM_TYPE;
			mii.fType = MFT_STRING|MFT_OWNERDRAW;
			mii.dwItemData = (DWORD)md;
			::SetMenuItemInfo(pMenu->m_hMenu, i, TRUE, &mii);
		}
	}
	nIndex--;
	if (nIndex>-1)
		aIndex[nIndex] = _NULL;
}

//##############################################################
int MSGBOX(UINT nIDPrompt, UINT nType , UINT nIDHelp )
{
	if (theApp.m_bUseLngFile==FALSE)
		return AfxMessageBox(nIDPrompt,nType,nIDHelp);
	else
	{
		CString str;
		_LoadString(str,nIDPrompt);
		return AfxMessageBox(str,nType,nIDHelp);
	}
}

int MSGBOX(LPCTSTR lpszText, UINT nType , UINT nIDHelp )
{
	return AfxMessageBox(lpszText,nType,nIDHelp);
}

//##############################################################
BOOL _OpenFtpUrl(CString strUrl)
{
	if (pmf->m_nFtpOpenType==0)
		return FALSE;	
	else if (pmf->m_nFtpOpenType==1)
	{
		/*TCHAR	szPath[MAX_PATH];
		ExpandEnvironmentStrings("%windir%\\explorer.exe", szPath, MAX_PATH);
		strApp = szPath;*/
		ShellExecute(NULL, "open", "IEXPLORE.EXE", strUrl, "", SW_SHOWNORMAL );
		return FALSE;
	}
	else if (pmf->m_nFtpOpenType==2 && pmf->m_strFtpExePath.GetLength())
	{
		CString strApp;
		CString strPath;

		strApp = pmf->m_strFtpExePath;
		strPath = strApp;
		int i = strPath.ReverseFind('\\');
		strPath = strPath.Left(i+1);

		//better add ftp:// at head
		if (strUrl.Left(7)=="http://")
			strUrl = strUrl.Right(strUrl.GetLength()-7);
		if (strUrl.Left(6)!="ftp://")
			strUrl = "ftp://" + strUrl;
		if((UINT)ShellExecute(NULL, "open", strApp , strUrl, strPath, SW_SHOW )<=32)
			MSGBOX(IDS_TIP_OPEN_FTP_ERR);
		else
			return FALSE;
	}
	return TRUE;
}

//##############################################################
void _LoadFormUrl(CListCtrl *pCtrl)
{
	char* tmp = _FileGetBuffer(theApp.m_strFormDataPath);
	if (tmp==NULL)
		return;
	//
	char *head,*tail;
	char pUrl[512];
	head = tmp;
	int i=0;
	DWORD len = strlen(tmp);
	while(1)
	{
		if (*head == NULL)
			break;
		else if (*head == '[')
		{
			tail = strstr(head,"]");
			len = tail-(head+1);
			STRNCPY(pUrl,head+1,len );
			pCtrl->InsertItem(i,pUrl);
			i++;
			head = tail+ 3;// ]\r\n
		}
		else
		{
			tail = strstr(head,"\r\n");
			if (!tail)
				break;
			head = tail+ 2;// \r\n
		}
	}
	free(tmp);
}

CString _GetSameFormUrl(CString strRoot)
{
	CString strCur;
	strCur.Empty();
	char* tmp = _FileGetBuffer(theApp.m_strFormDataPath);
	if (tmp==NULL)
		return strCur;
	//
	char *head,*tail;
	char pUrl[512];
	head = tmp;
	int i=0;
	DWORD len = strlen(tmp);
	while(1)
	{
		if (*head == NULL)
			break;
		else if (*head == '[')
		{
			tail = strstr(head,"]");
			len = tail-(head+1);
			STRNCPY(pUrl,head+1,len );
			strCur = pUrl;
			strCur = _StringGetRootUrl(strCur);
			if (strCur.CompareNoCase(strRoot)==0)
			{
				strCur = pUrl;
				break;
			}
			i++;
			head = tail+ 3;// ]\r\n
		}
		else
		{
			tail = strstr(head,"\r\n");
			if (!tail)
				break;
			head = tail+ 2;// \r\n
		}
	}
	free(tmp);
	return strCur;
}

//##############################################################
char NUMS[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

#define IS_09azAZ(c)	( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )

//# => %23
void _CharFormat(char *p)
{
	char c;
	while(*p)
	{	
		if ( IS_09azAZ(*p) )
			p++;//do nothing
		else
		{
			c = *p;
			memmove(p+3,p+1,strlen(p+1)+1);
			*p = '%';
			*(p+1) = NUMS[(c & 0xf0)>>4];
			*(p+2) = NUMS[ c & 0x0f];
			p+=3;
		}
	}
}

void _AsciiFormat(CString& strAscii)
{
	int nLen = strAscii.GetLength();

	int i = 0;
	for ( i=0; i<nLen; i++)
	{
		if ( !IS_09azAZ(strAscii.GetAt(i)) )
			break;
	}
	if (i == nLen)
		return;//no use format

	LPSTR pAsciiBuf = strAscii.GetBuffer(nLen*3+1);
	_CharFormat(pAsciiBuf);
	strAscii.ReleaseBuffer();
}

void _Ascii2Utf8(CString& strAscii)
{
	int nAsciiLen,nTransLen,length;
	LPSTR pAsciiBuf;
	LPSTR pTransBuf;

	nAsciiLen = strAscii.GetLength();
	nTransLen = nAsciiLen*3 + 1;

	pAsciiBuf = strAscii.GetBuffer(nTransLen);
	pTransBuf = (LPSTR)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,nTransLen);

	length = MultiByteToWideChar(CP_ACP,0,pAsciiBuf,nAsciiLen, (LPWSTR)pTransBuf,nTransLen);
	length = WideCharToMultiByte(CP_UTF8,0,(LPWSTR)pTransBuf,length,   pAsciiBuf,nTransLen,NULL,NULL);
	*(pAsciiBuf+length) = 0;
	
	//# => %23
	_CharFormat(pAsciiBuf);
	strAscii.ReleaseBuffer();
	//
	GlobalFree(pTransBuf);
}
