// IGACore.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "IGAIDirect3D8.h"
#include "IGAIDirect3DDevice8.h"
#include "IGA8Core.h"


// global variables
#pragma data_seg (".igacore_shared")
IGAIDirect3DDevice8* gl_pIGAIDirect3DDevice8;
IGAIDirect3D8      * gl_pIGAIDirect3D8;
HINSTANCE            gl_hD3D8Dll;
HINSTANCE            gl_hIGACoreInstance;
#pragma data_seg ()

//---------------------------------------------------------
#pragma data_seg("flag_data")

//导图开关,仅在翻转的时候被使用
DWORD	g_dwOutputTexture = 0;

#pragma data_seg()
#pragma comment(linker,"/SECTION:flag_data,RWS")


//-------------------------------------------------
//3D模块
FUN_IGA3DINIT          g_pFunIGA3DInit          ;
FUN_IGA3DDestory       g_pIGA3DDestoryt         ;
FUN_IGA3DGetMatrixData g_pIGA3DGetMatrixData    ;
FUN_IGA3DRender        g_pIGA3DRender           ;
FUN_ISetFristDraw      g_pSetFristDraw          ;
HINSTANCE              g_h3DCore;
BOOL				   g_bInit3D	= FALSE;
DWORD				   g_RenderPos  = -1;
//--------------------------------------------------


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	gl_hIGACoreInstance = hModule;
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
    return TRUE;
}

//-----------------------------------------------------------
//修改的函数，2008-5-23
// Exported function
DWORD IGAStartup(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,
							  DWORD dwRetValue,
							  DWORD dwRawRetAddress,
							  UINT SDKVersion)
{
	//OutputDebugString("IGAStartup");

	IDirect3D8 *pIDirect3D8_orig;

	gl_hD3D8Dll = GetModuleHandle("d3d8.dll");
	DWORD D3DCreate8_fn = (DWORD) GetProcAddress(gl_hD3D8Dll, "Direct3DCreate8");

	//组装调用原始函数代码

	BYTE  Code[]=
	{
		0x8B,0xFF,
		0x55,
		0x8B,0xEC,
		0x81,0xEC,0x14,0x01,0x00,0x00,
		0x90,
		0xE9,0x00,0x00,0x00,0x00,
	};

	*((DWORD*)(Code+0xD)) = D3DCreate8_fn + 0xB - ((DWORD)&Code+0xC+0x5);

	_asm
	{
		push SDKVersion
		lea  eax, Code
		call eax
		mov pIDirect3D8_orig ,eax
	}
	//
	//gl_pIGAIDirect3D8 = new IGAIDirect3D8(pIDirect3D8_orig);
	

	//设置返回值
	dwRetValue = Make_IGAIDirect3D8(pIDirect3D8_orig);
	_asm
	{
		mov dwRetValue, eax
	}

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
DWORD  Make_IGAIDirect3D8(IDirect3D8 *pIDirect3D8_orig)
{
	gl_pIGAIDirect3D8 = new IGAIDirect3D8(pIDirect3D8_orig);
	return (DWORD)gl_pIGAIDirect3D8;
}

//-----------------------------------------------------------------

HINSTANCE GetD3DhInstance()
{ 
	return gl_hD3D8Dll; 
}

//--------------------------------------------------
void* WINAPI CheckFullscreen_1()
{
	if (!gl_hD3D8Dll)
	{
		LoadOriginalDll();
	}
	if( gl_hD3D8Dll == NULL )
	{
		OutputDebugString("CheckFullscreen: Load d3d8.dll fail! ");
	}
	OutputDebugString("CheckFullscreen");
	return (void*)GetProcAddress(gl_hD3D8Dll,"CheckFullscreen");
}
void* WINAPI ValidatePixelShader_1()
{
	if (!gl_hD3D8Dll)
	{
		LoadOriginalDll();
	}
	if( gl_hD3D8Dll == NULL )
	{
		OutputDebugString("ValidatePixelShader: Load d3d8.dll fail! ");
	}
	OutputDebugString("ValidatePixelShader");
	return (void*)GetProcAddress(gl_hD3D8Dll,"ValidatePixelShader");
}
void* WINAPI ValidateVertexShader_1()
{
	if (!gl_hD3D8Dll)
	{
		LoadOriginalDll();
	}
	if( gl_hD3D8Dll == NULL )
	{
		OutputDebugString("ValidateVertexShader: Load d3d8.dll fail! ");
	}
	OutputDebugString("ValidateVertexShader");
	return (void*)GetProcAddress(gl_hD3D8Dll,"ValidateVertexShader");
}
void* WINAPI DebugSetMute()
{
	if (!gl_hD3D8Dll)
	{
		LoadOriginalDll();
	}
	if( gl_hD3D8Dll == NULL )
	{
		OutputDebugString("DebugSetMute: Load d3d8.dll fail! ");
	}
	OutputDebugString("DebugSetMute");
	return (void*)GetProcAddress(gl_hD3D8Dll,"DebugSetMute");
}

void WINAPI IGAShutdown()
{
	
}

//---------------------------------------------------
void InitInstance( ) 
{
	gl_hD3D8Dll             = NULL;
	gl_hIGACoreInstance     = NULL;
	gl_pIGAIDirect3D8       = NULL;
	gl_pIGAIDirect3DDevice8 = NULL;	
	
	//gl_hIGACoreInstance = (HINSTANCE) hModule;

	//HideModule(gl_hIGACoreInstance);

	//--------------------------------------------------------
	//3D模块
	g_h3DCore                = ::LoadLibrary(_T("IGA3D8.dll"));
	if( g_h3DCore != NULL )
	{
		g_pFunIGA3DInit          =	(FUN_IGA3DINIT) ::GetProcAddress(g_h3DCore, _T("IGA3DInit"));
		g_pIGA3DDestoryt         =	(FUN_IGA3DDestory ) ::GetProcAddress(g_h3DCore, _T("IGA3DDestory"));
		g_pIGA3DGetMatrixData    =	(FUN_IGA3DGetMatrixData) ::GetProcAddress(g_h3DCore, _T("IGA3DGetMatrixData"));
		g_pIGA3DRender           =	(FUN_IGA3DRender) ::GetProcAddress(g_h3DCore, _T("IGA3DRender"));
		g_pSetFristDraw          =	(FUN_ISetFristDraw ) ::GetProcAddress(g_h3DCore, _T("SetFristDraw"));
	
		if( g_pFunIGA3DInit != NULL
		  && g_pIGA3DDestoryt != NULL
		  && g_pIGA3DGetMatrixData != NULL
		  && g_pIGA3DRender != NULL
		  && g_pSetFristDraw != NULL )
		{
			g_bInit3D = TRUE;
		}
	}
	//---------------------------------------------------------
	//falsh窗口
	//IGAWIN::Init();
	//---------------------------------------------------------

	OutputDebugString("IGACore的初始化");
}

void LoadOriginalDll(void)
{
    char buffer[MAX_PATH];

	::GetSystemDirectory(buffer,MAX_PATH);
	strcat(buffer,"\\d3d8.dll");
	
	if (!gl_hD3D8Dll) 
		gl_hD3D8Dll = ::LoadLibrary(buffer);

	if (!gl_hD3D8Dll) {
		OutputDebugString("IGACore: d3d8.dll not loaded ERROR ****\r\n");
		::ExitProcess(0); // exit the hard way
	}
	else
	{
		OutputDebugString("IGACore: d3d8.dll loaded SUCCEEDED****\r\n");
	}
}

void ExitInstance() 
{    
    OutputDebugString("IGACore: ExitInstance called.\r\n");
	
	//--------------------------------------
	//3D模块
	if( g_h3DCore )
	{
		::FreeLibrary(g_h3DCore);
	}
	//--------------------------------------

	//-------------------------------------
	//falsh窗口
	IGAWIN::UnInit();
	//--------------------------------------

	// Release the system's d3d8.dll
    gl_hD3D8Dll = NULL;  

}

void SetOutputTexture()
{
	if( 1 == ::InterlockedExchange((volatile long*)(&g_dwOutputTexture), 1))
	{ 
		return;
	}

       
}
