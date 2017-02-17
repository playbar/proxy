
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TESTPOPUP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTPOPUP_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this 

#define TESTPOPUP_API __declspec(dllexport)__stdcall


BOOL TESTPOPUP_API InstallHotKey();
BOOL TESTPOPUP_API UninstHotKey();

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
