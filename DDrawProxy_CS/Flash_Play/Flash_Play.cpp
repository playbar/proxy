// Flash_Play.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "kFlash.h"

CComModule      _Module;


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	
	TCHAR   szFileName [ 256 ]     ;
	int     nReserve            = 0;
	int     nTime               = 60;
    int     nWidth = 200 ,nHeight = 200;
	//::OutputDebugString(lpCmdLine);
	if  ( lpCmdLine == NULL )  
		return 0;
	
	TCHAR*  pCmd  = NULL,pTmp ;
	TCHAR  szCmdLine   [ 1024 ];
	::_tcscpy(szCmdLine,lpCmdLine);
	pCmd = ::_tcschr(szCmdLine,';');
	if  ( pCmd== NULL   )    
		return 0;
	
	*pCmd = 0 ;		pCmd++;
	::_tcscpy(szFileName,szCmdLine);
	nTime = atoi(pCmd);
	pCmd = ::_tcschr(pCmd,';');
	if  ( pCmd== NULL   )     
		return 0;	
	pCmd++;
	nWidth =  atoi(pCmd);

	pCmd = ::_tcschr(pCmd,';');	
	pCmd++;
	nHeight =  atoi(pCmd);

	//::OutputDebugString(szFileName);
	::CoInitialize(NULL);				
    AtlAxWinInit();
	IGAWinInit();

	//pFrame->LoadFrame(128, WS_OVERLAPPED, NULL, NULL);

	KFlash    flash;
	flash.Create(NULL);

	if  ( nTime   > 260  || nTime  < 0 )  nTime  = 260 ;
	if  ( nWidth  > 500 || nWidth < 0 )  nWidth = 500;
	if  ( nHeight > 500 || nHeight< 0 )  nHeight= 500;

	flash.CenterWindow(nWidth,nHeight);

	if  (flash.Play(szFileName,nTime))
	{
		MSG  msg;
		while(  ::GetMessage(&msg,flash.GetHwnd(),0,0) )
		{
			::DispatchMessage(&msg);
			if  ( flash.m_bStop  )
			{
				flash.Close();
				flash.DestroyWindow();
				::DispatchMessage(&msg);
				break;
			}
		}
	}

	::CoUninitialize();
	return 0;
}



