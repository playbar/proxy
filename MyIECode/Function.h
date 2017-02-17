#if !defined(_FUNCTION_H_)
#define _FUNCTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_CL	8//max content of url, except http://....

//########################################################
char* _FileGetBuffer(CString filename);

//########################################################
CString _GetPlugInSection(CString strFileName, CString strSection);
int		_GetPlugInType(CString strFileName);
CString _GetPlugInIconPath(CString strFileName);
CString _GetPlugInName(CString strFileName);
CString _GetPlugInComments(CString strFileName);
CString _GetPlugInFileName(CString strFileName);

//########################################################
BOOL	_BrowserPath(CWnd* pParentWnd, CString& m_strPath);

//########################################################
BOOL	_FileIsExist(CString strPath);
void	_CreateFullDir(const char *dir);

//########################################################
CString _StringGetUpUrl(CString strUrl);
CString _StringGetRootUrl(CString strUrl);

//########################################################
BOOL	_GroupAddItem(CString strName, CString strUrl, BOOL bState, DWORD dwProperty, CString filename);

//########################################################
DWORD	_RegGetDword(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName);
BOOL	_RegSetDword(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName, DWORD dwValue);
CString _RegGetString(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName);
BOOL	_RegSetString(HKEY hKeyRoot, LPCSTR lpSubKey, LPCSTR lpValueName, LPCSTR lpValue);
void	_RegDeletePath(HKEY hKey, CString strPath);

//########################################################
void	_SetClipString(CString str);
CString _GetClipString();

//########################################################
void _InitLanguageFile(LPCSTR lpFileName);
void _LoadDlgString(CWnd *pDlg, CString strDomain);
void _LoadString(CString &str, UINT id);
void _LoadMenuString(CMenu *pMenu, CString strDomain);
void _LoadMenuStringOwnerDraw(CMenu *pMenu, CString strDomain,CImageList *imgList);
DWORD _GetPrivateProfileString(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName);

//
int MSGBOX(UINT nIDPrompt, UINT nType = MB_OK, UINT nIDHelp = (UINT)-1);
int MSGBOX(LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);
//########################################################
void _LoadMenuString2ComboBox(CMenu *pMenu, CComboBox *pCb);
void _GetMenuItemString(CString &strName, int nID);

//########################################################
BOOL _OpenFtpUrl(CString strUrl);
//########################################################
void _LoadFormUrl(CListCtrl *pCtrl);
CString _GetSameFormUrl(CString strRoot);

//########################################################
void _Ascii2Utf8(CString& strAscii);
void _AsciiFormat(CString& strAscii);

//########################################################


#endif

