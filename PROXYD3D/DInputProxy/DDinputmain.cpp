// dllmain.cpp : Defines the entry point for the DLL application.
#include "DInputProxy.h"
//#include "stdafx.h"

#pragma data_seg (".DirectInput_shared")
HINSTANCE           gl_hOriginalDll  = NULL;
HINSTANCE           gl_hThisInstance = NULL;
IGA_IDirectInput8  *g_pIDirectInput8 = NULL;
#pragma  data_seg ()

void LoadOriginalDll(void)
// ---------------------------------------------------------------------------------------
{
	OutputDebugString("DDRAWPROXY: LoadOriginalDll.\r\n");

	char buffer[MAX_PATH];

	// Getting path to system dir and to d3d9.dll
	::GetSystemDirectory(buffer,MAX_PATH);

	// Append dll name
	strcat(buffer,"\\dinput8.dll");

	if ( gl_hOriginalDll == NULL ) 
		gl_hOriginalDll = ::LoadLibrary(buffer);

	// Debug
	if ( gl_hOriginalDll == NULL )
	{
		OutputDebugString("DDRAWPROXY: Original ddraw.dll not loaded ERROR ****\r\n");
		::ExitProcess(0); // exit the hard way
	}
}

void InitInstance(HANDLE hModule) 
{
	OutputDebugString("DInputROXY: InitInstance.");

}

void ExitInstance() 
{
	OutputDebugString("DInputPROXY: ExitInstance.\r\n");
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
	OutputDebugString("DInputPROXY: DllMain reached.\r\n");
	// to avoid compiler lvl4 warnings 
	LPVOID lpDummy = lpReserved;
	lpDummy = NULL;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: 
		InitInstance(hModule); 
		break;

	case DLL_PROCESS_DETACH: 
		ExitInstance(); 
		break;

	case DLL_THREAD_ATTACH:  
		break;

	case DLL_THREAD_DETACH:  
		break;
	}
	return(true);
	
}


HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
{
	OutputDebugString("IGA_DirectInput8Create");
	HRESULT hr = E_FAIL;
	if (gl_hOriginalDll == NULL )
	{
		LoadOriginalDll();
	}
	typedef HRESULT (WINAPI * DirectInput8Cteate_Type)(HINSTANCE, DWORD, REFIID, LPVOID, LPUNKNOWN);
	DirectInput8Cteate_Type DirectInpuptCteate8_fn = (DirectInput8Cteate_Type)GetProcAddress( gl_hOriginalDll, "DirectInput8Create");
	if ( NULL == DirectInpuptCteate8_fn )
	{
		ExitProcess(0);
	}
	
	LPDIRECTINPUT8 FAR dinput;
	hr = DirectInpuptCteate8_fn(hinst, dwVersion, riidltf, &dinput, punkOuter );
	g_pIDirectInput8 = new IGA_IDirectInput8(dinput);
	*ppvOut = (LPVOID)g_pIDirectInput8;

	return hr;
}
