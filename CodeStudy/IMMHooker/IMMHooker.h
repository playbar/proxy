
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the IMMHOOKER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// IMMHOOKER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#include "Imm.h"
#include "stdio.h"



#ifdef IMMHOOKER_EXPORTS
#define IMMHOOKER_API __declspec(dllexport)
#else
#define IMMHOOKER_API __declspec(dllimport)
#endif
// This class is exported from the IMMHooker.dll
//class IMMHOOKER_API CIMMHooker 
//{
//public:
//	CIMMHooker(void);
//	// TODO: add your methods here.
//};

// Put up an assertion failure message box.
inline void InlineShowOutPutDebug(LPCSTR file, int line,  PCSTR expr) 
{
	char szOutputString[ sizeof(char) *MAX_PATH];
	memset(szOutputString, 0, sizeof(char)  * MAX_PATH );
	SYSTEMTIME st;
	GetLocalTime( &st );
	char szDate[100];
	sprintf( szDate, "%04d-%02d-%02d %02d:%02d:%02d",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	sprintf(szOutputString, "%s,  File %s, line %d ,Date:%s ",expr, file, line, szDate );
	OutputDebugStringA(szOutputString);
}


//在debug下显示输出信息宏,使用前要先定义存放字符串的数组
# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	define	SHOWOUTPUTDEBUG(szOutputString) InlineShowOutPutDebug(__FILE__, __LINE__, #szOutputString)
# else
#	define	SHOWOUTPUTDEBUG(szOutputString)
# endif

//extern IMMHOOKER_API int nIMMHooker;

//IMMHOOKER_API int fnIMMHooker(void);

extern "C" IMMHOOKER_API BOOL WINAPI ImeProcessKey(HIMC hIMC,UINT uVirtKey,LPARAM lParam,CONST LPBYTE lpbKeyState);
extern "C" IMMHOOKER_API BOOL WINAPI ImeConfigure(HKL hKL,HWND hAppWnd,DWORD dwMode,LPVOID  lpData);
extern "C" IMMHOOKER_API DWORD WINAPI ImeConversionList(HIMC hIMC,LPCTSTR lpszSrc,DWORD uBufLen,UINT uFlag);
extern "C" IMMHOOKER_API BOOL WINAPI ImeDestroy(UINT uReserved);
extern "C" IMMHOOKER_API UINT WINAPI ImeEnumRegisterWord(REGISTERWORDENUMPROC lpfnRegisterWordEnumProc,LPCTSTR lpszReading,DWORD dwStyle,LPCTSTR lpszString,LPVOID lpData);
extern "C" IMMHOOKER_API LRESULT WINAPI ImeEscape(HIMC hIMC,UINT uSubFunc,LPVOID lpData);
extern "C" IMMHOOKER_API UINT WINAPI ImeGetRegisterWordStyle(UINT nItem,LPSTYLEBUF lpStyleBuf);
extern "C" IMMHOOKER_API BOOL WINAPI ImeInquire(IMEINFO* lpImeInfo,LPTSTR lpszWndCls,DWORD lpszOptions);
extern "C" IMMHOOKER_API BOOL WINAPI ImeRegisterWord(LPCTSTR lpszReading,DWORD dwStyle,LPCTSTR lpszString);
extern "C" IMMHOOKER_API BOOL WINAPI ImeSelect(HIMC hIMC,BOOL fSelect);
extern "C" IMMHOOKER_API BOOL WINAPI ImeSetActiveContext(HIMC hIMC,BOOL fOn);
extern "C" IMMHOOKER_API BOOL WINAPI ImeSetCompositionString(HIMC hIMC,DWORD dwIndex,LPVOID lpComp,DWORD  dwCompLen,LPVOID lpRead,DWORD  dwReadLen);
extern "C" IMMHOOKER_API UINT WINAPI ImeToAsciiEx(UINT uVirtKey,UINT uScanCode,CONST LPBYTE lpbKeyState, DWORD* lpTransBuf,UINT fuState,HIMC hIMC);
extern "C" IMMHOOKER_API BOOL WINAPI ImeUnregisterWord(LPCTSTR lpszReading,DWORD dwStyle,LPCTSTR lpszString);
extern "C" IMMHOOKER_API BOOL WINAPI NotifyIME(HIMC hIMC,DWORD dwAction,DWORD dwIndex,DWORD dwValue);




