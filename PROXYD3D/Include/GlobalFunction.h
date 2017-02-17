#pragma  once 


#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <shellapi.h>

//////////////////////////////////////////////////////////////////////////

//extern DWORD	g_dwGameID;   //对应游戏ID号
typedef LRESULT(CALLBACK *LPWNDPROC)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef int (WINAPI *LPFN_SEARCHSTATCALLBACK)(DWORD dwGameID, DWORD dwAdID, WPARAM wParam, LPARAM lParam);
typedef VOID(WINAPI *LPCallBack3DFocus)(DWORD,LPVOID);


HRESULT AllocateName( LPCSTR Name, LPSTR *pNewName );
DWORD WINAPI OpenIEThreadProc(void *p);

int WINAPI SearchStatCallback(DWORD dwGameID,DWORD dwAdID, WPARAM wParam, LPARAM lParam);

//判断文件是否存在
bool FileExist(const char* path);			

void GetModulePath( char * szfullPathName );

BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize);
BOOL WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize);


//异常过滤函数
long ExceptFilter( LPEXCEPTION_POINTERS pep );

//异常处理函数
void ExceptDispose( LPCTSTR pFileName, int iline );

//异常日志记录函数
BOOL ExceptLog(LPCTSTR pchLogFileName, LPCTSTR lpszFormat, ...);



