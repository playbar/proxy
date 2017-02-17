// IE_DDS_Control.cpp : Implementation of CIE_DDS_ControlApp and DLL registration.

#include "stdafx.h"
#include "IE_DDS_Control.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CIE_DDS_ControlApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x151E31EB, 0xEFD8, 0x4EF9, { 0x98, 0x74, 0x90, 0x1A, 0x91, 0x53, 0x56, 0xED } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CIE_DDS_ControlApp::InitInstance - DLL initialization

BOOL CIE_DDS_ControlApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CIE_DDS_ControlApp::ExitInstance - DLL termination

int CIE_DDS_ControlApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
