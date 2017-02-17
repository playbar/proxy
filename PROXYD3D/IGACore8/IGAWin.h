#pragma once

//ZZhenFei 2008.06.20 16:01 ´´½¨

#ifndef  _IGAWIN_H
#define  _IGAWIN_H

namespace IGAWIN
{
	BOOL   Init();
	VOID   UnInit();

	//flash
	BOOL  CreateIGAWin (VOID* pDev,HWND  hwnd);
	BOOL  DestoryIGAWin(VOID* pDev);
	BOOL  ShowFlash   (VOID* ,int x0,int y0,int x1,int y1,DWORD  dwWaitTime = 0);
	//video
	BOOL  ShowVideo   (VOID* ,int x0,int y0,int x1,int y1,DWORD  dwWaitTime = 0);

    //ie
	BOOL  ShowIE     (VOID* ,int x0,int y0,int x1,int y1,DWORD  dwWaitTime = 0);

}

#endif