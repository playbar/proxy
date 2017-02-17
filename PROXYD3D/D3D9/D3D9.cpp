// D3D9.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <tchar.h>

BOOL InitInstance(HANDLE hModule) ;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: 
		{
			InitInstance(hModule);
			break;
		}		
	case DLL_PROCESS_DETACH:
		{				
			break;
		}
		
	case DLL_THREAD_ATTACH:  break;
	case DLL_THREAD_DETACH:  break;
	}
	return TRUE;
}



//////////////////////////////////////////////////////////////////////////
static  HINSTANCE  s_hD3d9Dll               = NULL;
void* lpfnCheckFullscreen					= NULL;
void* lpfnDirect3DShaderValidatorCreate9	= NULL;
void* lpfnPSGPError							= NULL;
void* lpfnPSGPSampleTexture					= NULL;
void* lpfnD3DPERF_BeginEvent				= NULL;
void* lpfnD3DPERF_EndEvent					= NULL;
void* lpfnD3DPERF_GetStatus					= NULL;
void* lpfnD3DPERF_QueryRepeatFrame			= NULL;
void* lpfnD3DPERF_SetMarker					= NULL;
void* lpfnD3DPERF_SetOptions				= NULL;
void* lpfnD3DPERF_SetRegion					= NULL;
void* lpfnDebugSetLevel						= NULL;
void* lpfnDebugSetMute						= NULL;
void* lpfnDirect3DCreate9					= NULL;										
//////////////////////////////////////////////////////////////////////////

static HINSTANCE    s_hIga9Core                    = NULL ;
static VOID*        s_lpIGAStartup2                = NULL ;

BOOL IsVista()
{
	OSVERSIONINFOEX osvi	= { 0 };
	
	memset(&osvi,0, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
	BOOL bOsVersionInfoEx	= FALSE;
	if( !(bOsVersionInfoEx = ::GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! ::GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}
	
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
			return TRUE;
	}
	return FALSE;
}
BOOL Emmoa_LoadSytem()
{
	//取系统目录 
	TCHAR   szD3DName[ 256 +1 ];
	::GetSystemDirectory(szD3DName,256);
    ::_tcscat(szD3DName,_T("\\d3d9.dll"));
	//OutputDebugString(szD3DName);
	//加载 d3d9.dll
    
	s_hD3d9Dll = ::LoadLibrary(szD3DName);
	if  ( NULL == s_hD3d9Dll )
	{
		int nErrorCode = GetLastError();
		char szTmp[200];
		wsprintf(szTmp,"Load system dll failed! ErrorCode: %d",
			nErrorCode);
		OutputDebugString(szTmp);
		ExitProcess(0);
		return FALSE;
	}	
	
	if(!IsVista())
	{
		lpfnCheckFullscreen					= (void*)GetProcAddress(s_hD3d9Dll, "CheckFullscreen");
		if ( lpfnCheckFullscreen == NULL )   return FALSE;
	}
	
	
	lpfnDirect3DShaderValidatorCreate9	= (void*)GetProcAddress(s_hD3d9Dll, "Direct3DShaderValidatorCreate9");
	if ( lpfnDirect3DShaderValidatorCreate9 == NULL )  return FALSE;
	
	lpfnPSGPError						= (void*)GetProcAddress(s_hD3d9Dll, "PSGPError");
	if ( lpfnPSGPError == NULL )    return FALSE;
	
	
	lpfnPSGPSampleTexture				= (void*)GetProcAddress(s_hD3d9Dll, "PSGPSampleTexture");
	if ( lpfnPSGPSampleTexture == NULL)  return FALSE;
	
	lpfnD3DPERF_BeginEvent				= (void*)GetProcAddress(s_hD3d9Dll, "D3DPERF_BeginEvent");	
	if ( lpfnD3DPERF_BeginEvent == NULL )  return FALSE;
	
	
	lpfnD3DPERF_EndEvent				= (void*)GetProcAddress(s_hD3d9Dll, "D3DPERF_EndEvent");
	if ( lpfnD3DPERF_EndEvent == NULL )  return FALSE;
	
	
	lpfnD3DPERF_GetStatus				= (void*)GetProcAddress(s_hD3d9Dll, "D3DPERF_GetStatus");
	if ( lpfnD3DPERF_GetStatus == NULL)  return FALSE;
	
	lpfnD3DPERF_QueryRepeatFrame		= (void*)GetProcAddress(s_hD3d9Dll, "D3DPERF_QueryRepeatFrame");
	if( lpfnD3DPERF_QueryRepeatFrame == NULL)  return FALSE;
	
	
	lpfnD3DPERF_SetMarker				= (void*)GetProcAddress(s_hD3d9Dll, "D3DPERF_SetMarker");
	if ( lpfnD3DPERF_SetMarker == NULL )  return FALSE;
	
	
	lpfnD3DPERF_SetOptions				= (void*)GetProcAddress(s_hD3d9Dll, "D3DPERF_SetOptions");
	if ( lpfnD3DPERF_SetOptions == NULL )  return FALSE;
	
	
	lpfnD3DPERF_SetRegion				= (void*)GetProcAddress(s_hD3d9Dll, "D3DPERF_SetRegion");
	if ( lpfnD3DPERF_SetRegion == NULL ) return FALSE;
	
	
	lpfnDebugSetLevel					= (void*)GetProcAddress(s_hD3d9Dll, "DebugSetLevel");
	if ( lpfnDebugSetLevel == NULL )  return FALSE;
	
	lpfnDebugSetMute					= (void*)GetProcAddress(s_hD3d9Dll, "DebugSetMute");
	if ( lpfnDebugSetMute == NULL)   return FALSE;
	
	lpfnDirect3DCreate9					= (void*)GetProcAddress(s_hD3d9Dll, "Direct3DCreate9" );
	if ( lpfnDirect3DCreate9 == NULL )  return FALSE;
	
	
	//加载Emmoa.dll
	
	TCHAR   szAdCoreName[ 256 +1 ];	
	::GetModuleFileName(NULL, szAdCoreName, 256);
	TCHAR	*p	= ::_tcsrchr(szAdCoreName, '\\');
	if(p != NULL)		*p	= 0;	
    
	::_tcscat(szAdCoreName,_T("\\Emmoa.dll"));
	s_hIga9Core = ::LoadLibrary(szAdCoreName);
	if  ( NULL == s_hIga9Core )
	{
		return FALSE;
	}
	s_lpIGAStartup2      = GetProcAddress(s_hIga9Core,_T("IGAStartup2"));
	
	if  ( NULL == s_lpIGAStartup2 ) 
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL InitInstance(HANDLE hModule) 
{
	//OutputDebugString("InitInstance");
	return TRUE;
}

void __declspec(naked) CheckFullscreen(void)
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    ESI
// 		PUSH    EDI
// 		PUSH    4B64193CH
// 		PUSH    1             
// 		PUSH    1             
// 		PUSH    0             
// 
// 		POP     ESI
// 		POP     ESI
// 		POP     ESI
// 		POP     ESI
// 		POP     EDI
// 		POP     ESI		

		JMP lpfnCheckFullscreen;

		RET
	}
}
void __declspec(naked) Direct3DShaderValidatorCreate9(void) 
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		PUSH    0FFFFFFFFH
// 
// 		POP     EAX
// 		MOV     ESP, EBP
// 		POP     EBP

		JMP lpfnDirect3DShaderValidatorCreate9

		RET
	}
}
void __declspec(naked) PSGPError(void) 
{

	_asm
	{
// 	   MOV     EDI, EDI
// 	   PUSH    EBP
// 	   MOV     EBP, ESP
// 	   PUSH    ECX
// 
// 	   POP     ECX
// 	   MOV     ESP , EBP
// 	   POP     EBP

	   JMP    lpfnPSGPError
	   RET
	}
}
void __declspec(naked) PSGPSampleTexture(void) 
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 10H
// 		PUSH    EBX
// 
// 		POP     EBX
// 		MOV     ESP, EBP
// 		POP     EBP
// 
// 		PUSH    [EBP+08H] 
// 		PUSH    [EBP+0CH] 
// 		PUSH    [EBP+10H] 
// 		PUSH    [EBP+14H] 
		JMP    lpfnPSGPSampleTexture

		//RET     10h
	}
}
void __declspec(naked) D3DPERF_BeginEvent(void) 
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 18H
// 
// 		MOV     ESP, EBP
// 		POP     EBP

		JMP lpfnD3DPERF_BeginEvent

		RET
	}
}
void __declspec(naked) D3DPERF_EndEvent(void) 
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 18H
// 			
// 		MOV     ESP, EBP
// 		POP     EBP
			
		JMP lpfnD3DPERF_EndEvent
			
		RET
	}
	
}
void __declspec(naked) D3DPERF_GetStatus(void) 
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 18H
// 			
// 		MOV     ESP, EBP
// 		POP     EBP
			
		JMP lpfnD3DPERF_GetStatus
			
		RET
	}	
}
void __declspec(naked) D3DPERF_QueryRepeatFrame(void)
 {
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 1CH
// 			
// 		MOV     ESP, EBP
// 		POP     EBP
			
		JMP lpfnD3DPERF_QueryRepeatFrame
			
		RET
	}		
}
void __declspec(naked) D3DPERF_SetMarker(void) 
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 18H
// 			
// 		MOV     ESP, EBP
// 		POP     EBP
			
		JMP lpfnD3DPERF_SetMarker
			
		RET
	}	
	
}
void __declspec(naked) D3DPERF_SetOptions(void) 
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 18H
// 			
// 		MOV     ESP, EBP
// 		POP     EBP
			
		JMP lpfnD3DPERF_SetOptions
			
		//RET
	}		
}
void __declspec(naked) D3DPERF_SetRegion(void) 
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 18H
// 			
// 		MOV     ESP, EBP
// 		POP     EBP
			
		JMP lpfnD3DPERF_SetRegion
			
		//RET
	}
}
void __declspec(naked) DebugSetLevel(void) 
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 18H
// 			
// 		MOV     ESP, EBP
// 		POP     EBP
			
		JMP lpfnDebugSetLevel
			
		//RET
	}
	
}
void __declspec(naked) DebugSetMute(void)
{
	_asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 1CH
// 			
// 		MOV     ESP, EBP
// 		POP     EBP
			
		JMP lpfnDebugSetMute
			
		//RET
	}	
	
}


void*  _stdcall LoadEmmoaDirect3DCreate9( void* pDirect3D)
{
	//::OutputDebugString("LoadEmmoaDirect3DCreate9");
	typedef  VOID*  (WINAPI *LP_FUN_EMMOAX )(void*);
	if  (s_lpIGAStartup2 != NULL)
	{
        //::OutputDebugString("core加载成功");
		return ((LP_FUN_EMMOAX)(s_lpIGAStartup2))(pDirect3D);		
	}

	
    return pDirect3D;
	
}
void __declspec(naked) Direct3DCreate9()
{
    _asm
	{
// 		MOV     EDI, EDI
// 		PUSH    EBP
// 		MOV     EBP, ESP
// 		SUB     ESP, 108H
// 
// 		MOV     ESP, EBP
// 		POP     EBP 


		//MOV     EAX,m_bIsLoadLibraryEx
		//CMP		EAX,0
		//JZ		LOAD_OK

		CALL	Emmoa_LoadSytem

		PUSH    [ESP+4]
		CALL    lpfnDirect3DCreate9

		PUSH    EAX
		CALL    LoadEmmoaDirect3DCreate9    //; 加载我们函数	

		RET     4
	}
}
