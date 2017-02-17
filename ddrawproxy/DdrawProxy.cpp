// gp_d3d9.cpp 

#include "stdafx.h"
#include "resource.h"
//#include "FlashSprite.h"
#include <dinput.h>

// global variables
#pragma data_seg (".ddraw_shared")
HINSTANCE           gl_hOriginalDll  = NULL;
HINSTANCE           gl_hThisInstance = NULL;
IGA_IDirectDraw7*   gl_pIDirectDraw7 = NULL;
IGA_IDirectDraw*	gl_pIDirectDraw  = NULL;        
#pragma data_seg ()


//UCHAR        keyboard_state[256]; // contains keyboard state table
//LPDIRECTINPUT8        lpdi      = NULL;    // dinput object
//LPDIRECTINPUTDEVICE8  lpdikey   = NULL;    // dinput keyboard
//LPDIRECTINPUTDEVICE8  lpdimouse = NULL;    // dinput mouse

//FlashSprite flashSprite;
//const char *g_pcFileName = "car.bmp";
//Surface g_kSurf;

// ---------------------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
// ---------------------------------------------------------------------------------------
{
    OutputDebugString("DDRAWPROXY: DllMain reached.\r\n");
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

// An exported function (faking ddraw.dll's export)
// ---------------------------------------------------------------------------------------
HRESULT WINAPI DirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter)
// ---------------------------------------------------------------------------------------
{
	OutputDebugString("DDRAWPROXY: begin DirectDrawCreate\r\n");
	// This dll does not really intercept calls to DirectDrawCreate. We focus DirectDrawCreateEx!!
		
	if (!gl_hOriginalDll) 
		LoadOriginalDll(); // looking for the "right ddraw.dll"
	
	// Hooking DDRAW interface from Original Library IDirectDraw *pDD; 
	typedef HRESULT (WINAPI* DirectDrawCreate_Type)(GUID FAR *, LPDIRECTDRAW FAR *, IUnknown FAR *);
    DirectDrawCreate_Type DirectDrawCreate_fn = (DirectDrawCreate_Type) GetProcAddress( gl_hOriginalDll, _T("DirectDrawCreate"));
    //Debug
	if (!DirectDrawCreate_fn) 
    {
        OutputDebugString("DDRAWPROXY: Pointer to original DirectDrawCreate function not received ERROR ****\r\n");
        ::ExitProcess(0);
    }

	LPDIRECTDRAW lpDDraw;
	HRESULT hr = DirectDrawCreate_fn(lpGUID, &lpDDraw, pUnkOuter);
	gl_pIDirectDraw = new IGA_IDirectDraw( lpDDraw );
	*lplpDD  = gl_pIDirectDraw;

	
	//HRESULT hr = DirectDrawCreate_fn(lpGUID, lplpDD, pUnkOuter);
	//gl_pIDirectDraw = new IGA_IDirectDraw( lpDDraw );
	//*lplpDD  = gl_pIDirectDraw;
	return hr;

}

// An exported function (faking ddraw.dll's export)
// ---------------------------------------------------------------------------------------
HRESULT WINAPI DirectDrawCreateEx(GUID FAR * lpGuid, LPVOID  *lplpDD, REFIID  iid,IUnknown FAR *pUnkOuter)
// ---------------------------------------------------------------------------------------
{
	OutputDebugString("DDRAWPROXY: Exported function DirectDrawCreateEx reached.\r\n");
	
	// This is set up to work for IID_IDirectDraw7 only !!
	//const IID IID_IDirectDraw7_internal = {0x15e65ec0,0x3b9c,0x11d2,0xb9,0x2f,0x00,0x60,0x97,0x97,0xea,0x5b};

	if (iid != IID_IDirectDraw7)
	{
		OutputDebugString("DDRAWPROXY: IID_IDirectDraw7 not requested. ERROR ****\r\n");
        ::ExitProcess(0);
	}
	
	if ( gl_hOriginalDll == NULL ) 
	{
		LoadOriginalDll(); // looking for the "right ddraw.dll"
	}
	
	// Hooking DDRAW interface from Original Library IDirectDraw *pDD; 
	typedef HRESULT (WINAPI* DirectDrawCreateEx_Type)(GUID FAR *, LPVOID *, REFIID, IUnknown FAR *);
    DirectDrawCreateEx_Type DirectDrawCreateEx_fn = (DirectDrawCreateEx_Type) GetProcAddress( gl_hOriginalDll, _T("DirectDrawCreateEx"));
	if ( DirectDrawCreateEx_fn == NULL ) 
    {
        OutputDebugString("DDRAWPROXY: Pointer to original DirectDrawCreateEx function not received ERROR ****\r\n");
        ::ExitProcess(0);
    }

	LPDIRECTDRAW7 FAR dummy;
	HRESULT h = DirectDrawCreateEx_fn(lpGuid, (LPVOID*) &dummy, iid, pUnkOuter);

	gl_pIDirectDraw7 = new IGA_IDirectDraw7(dummy);
	*lplpDD = (LPVOID)gl_pIDirectDraw7;

	//g_kSurf.Create(gl_pIDirectDraw7, 1500, 280);
	//g_kSurf.LoadBitmap(NULL, g_pcFileName, 0, 0, 1500, 280);

	//flashSprite.SetLoopPlay(true);
	//flashSprite.StartFlash(gl_pIDirectDraw7,"E:\\whoyo\\test.swf", 320,240 );

	OutputDebugString("DDRAWPROXY: Exiting DirectDrawCreateEx.\r\n");
	
	return (h);
}


// An exported function (faking ddraw.dll's export)
// ---------------------------------------------------------------------------------------
HRESULT WINAPI DirectDrawCreateClipper(DWORD dwFlags, LPDIRECTDRAWCLIPPER FAR *lplpDDClipper, IUnknown FAR *pUnkOuter)
// ---------------------------------------------------------------------------------------
{
	OutputDebugString("DDRAWPROXY: Exported function DirectDrawCreateClipper reached.\r\n");
	
	if ( gl_hOriginalDll == NULL ) 
		LoadOriginalDll(); // looking for the "right ddraw.dll"
	
	// Hooking DDRAW interface from Original Library IDirectDraw *pDD; 
	typedef HRESULT (WINAPI* DirectDrawCreateClipper_Type)(DWORD, LPDIRECTDRAWCLIPPER FAR *, IUnknown FAR *);
		
    DirectDrawCreateClipper_Type DirectDrawCreateClipper_fn = (DirectDrawCreateClipper_Type) GetProcAddress( gl_hOriginalDll, _T("DirectDrawCreateClipper"));
    
    //Debug
	if (!DirectDrawCreateClipper_fn) 
    {
        OutputDebugString("DDRAWPROXY: Pointer to original DirectDrawCreateClipper function not received ERROR ****\r\n");
        ::ExitProcess(0);
    }
	
	HRESULT h = DirectDrawCreateClipper_fn(dwFlags, lplpDDClipper, pUnkOuter);
		
	return (h);
}

// An exported function (faking ddraw.dll's export)
// ---------------------------------------------------------------------------------------
HRESULT WINAPI DirectDrawEnumerateW(LPDDENUMCALLBACKW lpCallback, LPVOID lpContext)
// ---------------------------------------------------------------------------------------
{
	OutputDebugString("DDRAWPROXY: Exported function DirectDrawEnumerateW reached.\r\n");
	
	if ( gl_hOriginalDll == NULL ) 
		LoadOriginalDll(); // looking for the "right ddraw.dll"
	
	// Hooking DDRAW interface from Original Library IDirectDraw *pDD; 
	typedef HRESULT (WINAPI* DirectDrawEnumerateW_Type)(LPDDENUMCALLBACKW, LPVOID);
    DirectDrawEnumerateW_Type DirectDrawEnumerateW_fn = (DirectDrawEnumerateW_Type) GetProcAddress( gl_hOriginalDll, _T("DirectDrawEnumerateW"));
    
    //Debug
	if (!DirectDrawEnumerateW_fn) 
    {
        OutputDebugString("DDRAWPROXY: Pointer to original DirectDrawEnumerateW function not received ERROR ****\r\n");
        ::ExitProcess(0);
    }
	HRESULT h = DirectDrawEnumerateW_fn(lpCallback, lpContext);
	return (h); 
}

// An exported function (faking ddraw.dll's export)
// ---------------------------------------------------------------------------------------
HRESULT WINAPI DirectDrawEnumerateA(LPDDENUMCALLBACKA lpCallback, LPVOID lpContext)
// ---------------------------------------------------------------------------------------
{
	OutputDebugString("DDRAWPROXY: Exported function DirectDrawEnumerateA reached.\r\n");
	
	if ( gl_hOriginalDll == NULL ) 
		LoadOriginalDll(); // looking for the "right ddraw.dll"
	
	// Hooking DDRAW interface from Original Library IDirectDraw *pDD; 
	typedef HRESULT (WINAPI* DirectDrawEnumerateA_Type)(LPDDENUMCALLBACKA, LPVOID);
    DirectDrawEnumerateA_Type DirectDrawEnumerateA_fn = (DirectDrawEnumerateA_Type) GetProcAddress( gl_hOriginalDll, _T("DirectDrawEnumerateA"));
    
    //Debug
	if (!DirectDrawEnumerateA_fn) 
    {
        OutputDebugString("DDRAWPROXY: Pointer to original DirectDrawEnumerateA function not received ERROR ****\r\n");
        ::ExitProcess(0);
    }
	
	HRESULT h = DirectDrawEnumerateA_fn(lpCallback, lpContext);

	return (h);
}

// An exported function (faking ddraw.dll's export)
// ---------------------------------------------------------------------------------------
HRESULT WINAPI DirectDrawEnumerateExW(LPDDENUMCALLBACKEXW lpCallback, LPVOID lpContext, DWORD dwFlags)
// ---------------------------------------------------------------------------------------
{
	OutputDebugString("DDRAWPROXY: Exported function DirectDrawEnumerateExW reached.\r\n");
	
	if ( gl_hOriginalDll == NULL ) 
		LoadOriginalDll(); // looking for the "right ddraw.dll"
	
	// Hooking DDRAW interface from Original Library IDirectDraw *pDD; 
	typedef HRESULT (WINAPI* DirectDrawEnumerateExW_Type)(LPDDENUMCALLBACKEXW, LPVOID, DWORD);
		
    DirectDrawEnumerateExW_Type DirectDrawEnumerateExW_fn = (DirectDrawEnumerateExW_Type) GetProcAddress( gl_hOriginalDll, _T("DirectDrawEnumerateExW"));
    
    //Debug
	if (!DirectDrawEnumerateExW_fn) 
    {
        OutputDebugString("DDRAWPROXY: Pointer to original DirectDrawEnumerateExW function not received ERROR ****\r\n");
        ::ExitProcess(0);
    }
	
	HRESULT h = DirectDrawEnumerateExW_fn(lpCallback, lpContext, dwFlags);

	return (h); 
}

// An exported function (faking ddraw.dll's export)
// ---------------------------------------------------------------------------------------
HRESULT WINAPI DirectDrawEnumerateExA(LPDDENUMCALLBACKEXA lpCallback, LPVOID lpContext, DWORD dwFlags)
// ---------------------------------------------------------------------------------------
{
	OutputDebugString("DDRAWPROXY: Exported function DirectDrawEnumerateExA reached.\r\n");
	
	if ( gl_hOriginalDll == NULL ) 
		LoadOriginalDll(); // looking for the "right ddraw.dll"
	
	// Hooking DDRAW interface from Original Library IDirectDraw *pDD; 
	typedef HRESULT (WINAPI* DirectDrawEnumerateExA_Type)(LPDDENUMCALLBACKEXA, LPVOID, DWORD);
		
    DirectDrawEnumerateExA_Type DirectDrawEnumerateExA_fn = (DirectDrawEnumerateExA_Type) GetProcAddress( gl_hOriginalDll, _T("DirectDrawEnumerateExA"));
    
    //Debug
	if (!DirectDrawEnumerateExA_fn) 
    {
        OutputDebugString("DDRAWPROXY: Pointer to original DirectDrawEnumerateExA function not received ERROR ****\r\n");
        ::ExitProcess(0);
    }

	HRESULT h = DirectDrawEnumerateExA_fn(lpCallback, lpContext, dwFlags);

	return (h);
}


// ---------------------------------------------------------------------------------------
void InitInstance(HANDLE hModule) 
// ---------------------------------------------------------------------------------------
{
	OutputDebugString("DDRAWPROXY: InitInstance.");

	// Initialisation
	gl_hOriginalDll        = NULL;
	gl_hThisInstance       = NULL;
	
	// Storing Instance handle into global var
	gl_hThisInstance = (HINSTANCE)  hModule;
}

// ---------------------------------------------------------------------------------------
void LoadOriginalDll(void)
// ---------------------------------------------------------------------------------------
{
    OutputDebugString("DDRAWPROXY: LoadOriginalDll.\r\n");
	
	char buffer[MAX_PATH];
    
    // Getting path to system dir and to d3d9.dll
	::GetSystemDirectory(buffer,MAX_PATH);

	// Append dll name
	strcat(buffer,"\\ddraw.dll");
	
	if ( gl_hOriginalDll == NULL ) 
		gl_hOriginalDll = ::LoadLibrary(buffer);

	// Debug
	if ( gl_hOriginalDll == NULL )
	{
		OutputDebugString("DDRAWPROXY: Original ddraw.dll not loaded ERROR ****\r\n");
		::ExitProcess(0); // exit the hard way
	}
}

// ---------------------------------------------------------------------------------------
void ExitInstance() 
// ---------------------------------------------------------------------------------------
{
	OutputDebugString("DDRAWPROXY: ExitInstance.\r\n");
    
    if (gl_hOriginalDll != NULL )
	{
		::FreeLibrary(gl_hOriginalDll);
	    gl_hOriginalDll = NULL;  
	}
}


