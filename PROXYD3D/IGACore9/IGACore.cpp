// IGACore.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "IGAIDirect3D9.h"
#include "IGAIDirect3DDevice9.h"
#include "IGACore.h"


// global variables
#pragma data_seg (".igacore_shared")
IGAIDirect3DDevice9* gl_pIGAIDirect3DDevice9;
IGAIDirect3D9*       gl_pIGAIDirect3D9;
HINSTANCE            g_hD3D9Dll;
HINSTANCE            gl_hIGACoreInstance;
#pragma data_seg ()

//-------------------------------------------------
//3D模块
FUN_IGA3DINIT          g_pFunIGA3DInit          ;
FUN_IGA3DDestory       g_pFunIGA3DDestoryt         ;
FUN_IGA3DGetMatrixData g_pFunIGA3DGetMatrixData    ;
FUN_IGA3DRender        g_pFunIGA3DRender           ;
FUN_ISetFristDraw      g_pFunSetFristDraw          ;
HINSTANCE              g_hIGA9Core = NULL;
BOOL				   g_bInit3D	= FALSE;
DWORD				   g_RenderPos  = -1;
//--------------------------------------------------


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    //LPVOID lpDummy = lpReserved;
    //lpDummy = NULL;
    
    switch (ul_reason_for_call)
	{
	    case DLL_PROCESS_ATTACH: 
			InitInstance(hModule); 
			break;
	    case DLL_PROCESS_DETACH: 
			ExitInstance(); 
			break;
        
        case DLL_THREAD_ATTACH:  break;
	    case DLL_THREAD_DETACH:  break;
	}
    return TRUE;
}

//---------------------------------------------
//2008-5-23添加
// Exported function
DWORD IGAStartup(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,
							  DWORD dwRetValue,
							  DWORD dwRawRetAddress,
							  UINT SDKVersion)
{

#if OUTPUT_DEBUG
	OutputDebugString("IGAStartup");
#endif

	IDirect3D9 *pIDirect3D9_orig;

	g_hD3D9Dll = GetModuleHandle("d3d9.dll");
	DWORD D3DCreate9_fn = (DWORD) GetProcAddress(g_hD3D9Dll, "Direct3DCreate9");

	//组装调用原始函数代码

	BYTE  Code[]=
	{
		0x8B,0xFF,
		0x55,
		0x8B,0xEC,
		0x81,0xEC,0x08,0x01,0x00,0x00,
		0x90,
		0xE9,0x00,0x00,0x00,0x00,
	};

	*((DWORD*)(Code+0xD)) = D3DCreate9_fn + 0xB - ((DWORD)&Code+0xC+0x5);

	_asm
	{
		push SDKVersion
		lea  eax, Code
		call eax
		mov pIDirect3D9_orig ,eax
	}
	//
	//gl_pIGAIDirect3D8 = new IGAIDirect3D8(pIDirect3D8_orig);
	

	//设置返回值
	dwRetValue = Make_IGAIDirect3D9(pIDirect3D9_orig);

	_asm
	{
		pop edi
		pop esi
		pop ebx
		mov     esp, ebp
		pop     ebp

		popad
		pop eax
		retn 4
	}
	return 0;
}

DWORD  Make_IGAIDirect3D9(IDirect3D9 *pIDirect3D9_orig)
{
#if OUTPUT_DEBUG
	OutputDebugString("Make_IGAIDirect3D9");
#endif

	gl_pIGAIDirect3D9 = new IGAIDirect3D9(pIDirect3D9_orig);
	HINSTANCE hD3D9Dll = GetModuleHandle("d3d9.dll");
	//HideModule(hD3D9Dll);
	return (DWORD)gl_pIGAIDirect3D9;
}

//------------------------------------------



void* WINAPI ExportFun1()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("CheckFullscreen");
	return (void*)GetProcAddress(g_hD3D9Dll,"CheckFullscreen");
}

void* WINAPI ExportFun2()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("Direct3DShaderValidatorCreate9");
	return (void*)GetProcAddress(g_hD3D9Dll,"Direct3DShaderValidatorCreate9");
}

void* WINAPI ExportFun3()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("PSGPError");
	return (void*)GetProcAddress(g_hD3D9Dll,"PSGPError");
}

void* WINAPI ExportFun4()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("PSGPSampleTexture");
	return (void*)GetProcAddress(g_hD3D9Dll,"PSGPSampleTexture");
}

void* WINAPI ExportFun5()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("D3DPERF_BeginEvent");
	return (void*)GetProcAddress(g_hD3D9Dll,"D3DPERF_BeginEvent");
}

void* WINAPI ExportFun6()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("D3DPERF_EndEvent");
	return (void*)GetProcAddress(g_hD3D9Dll,"D3DPERF_EndEvent");
}

void* WINAPI ExportFun7()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("D3DPERF_GetStatus");
	return (void*)GetProcAddress(g_hD3D9Dll,"D3DPERF_GetStatus");
}

void* WINAPI ExportFun8()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("D3DPERF_QueryRepeatFrame");
	return (void*)GetProcAddress(g_hD3D9Dll,"D3DPERF_QueryRepeatFrame");
}

void* WINAPI ExportFun9()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("D3DPERF_SetMarker");
	return (void*)GetProcAddress(g_hD3D9Dll,"D3DPERF_SetMarker");
}


void* WINAPI ExportFun10()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("D3DPERF_SetOptions");
	return (void*)GetProcAddress(g_hD3D9Dll,"D3DPERF_SetOptions");
}

void* WINAPI ExportFun11()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("D3DPERF_SetRegion");
	return (void*)GetProcAddress(g_hD3D9Dll,"D3DPERF_SetRegion");
}

void* WINAPI ExportFun12()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("DebugSetLevel");
	return (void*)GetProcAddress(g_hD3D9Dll,"DebugSetLevel");
}

void* WINAPI ExportFun13()
{
	if (!g_hD3D9Dll)
	{
		LoadOriginalDll();
	}
	if( g_hD3D9Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("DebugSetMute");
	return (void*)GetProcAddress(g_hD3D9Dll,"DebugSetMute");
}


void WINAPI IGAShutdown()
{
	
}

void InitInstance(HANDLE hModule) 
{
#if OUTPUT_DEBUG
	OutputDebugString("InitInstance");
#endif
	g_hD3D9Dll             = NULL;
	gl_hIGACoreInstance     = NULL;
	gl_pIGAIDirect3D9       = NULL;
	gl_pIGAIDirect3DDevice9 = NULL;	
	
	gl_hIGACoreInstance = (HINSTANCE) hModule;

	
	g_hIGA9Core          = LoadLibrary("IGA3D9.dll");
	char szOutputString[256];
	sprintf(szOutputString, "g_hIGA9Core = %d", g_hIGA9Core );
	OutputDebugString(szOutputString);
	if( g_hIGA9Core != NULL )
	{
		OutputDebugString("In LoadLibrary  IGA3D9.dll");
		g_pFunIGA3DInit          =	(FUN_IGA3DINIT) ::GetProcAddress(g_hIGA9Core, _T("IGA3DInit"));
		g_pFunIGA3DDestoryt         =	(FUN_IGA3DDestory ) ::GetProcAddress(g_hIGA9Core, _T("IGA3DDestory"));
		g_pFunIGA3DGetMatrixData    =	(FUN_IGA3DGetMatrixData) ::GetProcAddress(g_hIGA9Core, _T("IGA3DGetMatrixData"));
		g_pFunIGA3DRender           =	(FUN_IGA3DRender) ::GetProcAddress(g_hIGA9Core, _T("IGA3DRender"));
		g_pFunSetFristDraw          =	(FUN_ISetFristDraw ) ::GetProcAddress(g_hIGA9Core, _T("SetFristDraw"));
	
		if( g_pFunIGA3DInit != NULL
		  && g_pFunIGA3DDestoryt != NULL
		  && g_pFunIGA3DGetMatrixData != NULL
		  && g_pFunIGA3DRender != NULL
		  && g_pFunSetFristDraw != NULL )
		{
			g_bInit3D = TRUE;
		}
	}
	//---------------------------------------------------------
	
	//---------------------------------------------------------
	//falsh窗口
	IGAWIN::Init();
	//---------------------------------------------------------


	if (!g_hD3D9Dll)
		LoadOriginalDll();
}

void LoadOriginalDll(void)
{
 //   char buffer[MAX_PATH];

	//::GetSystemDirectory(buffer,MAX_PATH);
	//strcat(buffer,"\\d3d9.wdx");
	//
	//if (!gl_hD3D9Dll) 
	//	gl_hD3D9Dll = ::LoadLibrary(buffer);

	//if (!gl_hD3D9Dll) {
	//	OutputDebugString("IGACore: d3d9.wdx not loaded ERROR ****\r\n");
	//	::ExitProcess(0); // exit the hard way
	//}
}

void ExitInstance() 
{    
    OutputDebugString("IGACore: ExitInstance called.\r\n");
		
	//--------------------------------------
	//3D模块
	if( g_hIGA9Core )
	{
		::FreeLibrary(g_hIGA9Core);
	}
	//--------------------------------------

	//-------------------------------------
	//falsh窗口
	IGAWIN::UnInit();
	//--------------------------------------

	// Release the system's d3d9.wdx
	if ( NULL !=  g_hD3D9Dll) 
	{
	    g_hD3D9Dll = NULL;  
	}
}

