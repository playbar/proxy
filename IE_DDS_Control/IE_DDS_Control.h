#pragma once

// IE_DDS_Control.h : main header file for IE_DDS_Control.DLL

#if !defined( __AFXCTL_H__ )
#error "include 'afxctl.h' before including this file"
#endif

#include "resource.h"       // main symbols


// CIE_DDS_ControlApp : See IE_DDS_Control.cpp for implementation.

class CIE_DDS_ControlApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

