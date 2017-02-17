// 3D_TOOL.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "resource.h"		// 主符号


// CD_TOOLApp:
// 有关此类的实现，请参阅 3D_TOOL.cpp
//

class CD_TOOLApp : public CWinApp
{
public:
	CD_TOOLApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CD_TOOLApp theApp;
