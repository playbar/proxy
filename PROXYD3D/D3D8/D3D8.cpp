// D3D8.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <tchar.h>

BOOL WINAPI InitInstance(HANDLE hModule) ;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH: 
			{	
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




static HINSTANCE    s_hD3d8Dll                     = NULL ;
static VOID*        s_lpFunCheckFullscreen         = NULL ;
static VOID*        s_lpFunValidatePixelShader     = NULL ;
static VOID*        s_lpFunValidateVertexShader    = NULL ;
static VOID*        s_lpFunDebugSetMute            = NULL ;
static VOID*        s_lpFunDirect3DCreate8         = NULL ;


static HINSTANCE    s_hIga8Core                    = NULL ;
static VOID*        s_lpIGAStartup2                = NULL ;

BOOL WINAPI InitInstance() 
{		
	//取系统目录 
	TCHAR   szD3DName[ 256 +1 ];
	::GetSystemDirectory(szD3DName,256);
    ::_tcscat(szD3DName,_T("\\d3d8.dll"));

	::OutputDebugString("开始加载dll...");
    ::OutputDebugString(szD3DName);

	//加载 d3d8.dll
    s_hD3d8Dll = ::LoadLibrary(szD3DName);
	if  ( NULL == s_hD3d8Dll )
	{
		::OutputDebugString("加载d3d失败");
		return FALSE;
	}
	
// 	s_lpFunCheckFullscreen      = GetProcAddress(s_hD3d8Dll,_T("CheckFullscreen"));
// 	if  ( NULL == s_lpFunCheckFullscreen ) 
// 	{
// 		OutputDebugString("取 CheckFullscreen失败");
// 		//return FALSE;   d3d8.  10版本中不存在此函数
// 		return TRUE;
// 	}

	OutputDebugString("取 ValidatePixelShader");
	s_lpFunValidatePixelShader  = GetProcAddress(s_hD3d8Dll,_T("ValidatePixelShader"));
	if  ( NULL == s_lpFunValidatePixelShader ) 
	{
		OutputDebugString("失败");
		return FALSE;
	}

	s_lpFunValidateVertexShader = GetProcAddress(s_hD3d8Dll,_T("ValidateVertexShader"));
	if  ( NULL == s_lpFunValidateVertexShader ) 
	{
		OutputDebugString("取 ValidateVertexShader失败");
		return FALSE;
	}

	s_lpFunDebugSetMute         = GetProcAddress(s_hD3d8Dll,_T("DebugSetMute"));
	if  ( NULL == s_lpFunDebugSetMute ) 
	{
		OutputDebugString("取 DebugSetMute失败");
		return FALSE;
	}

	s_lpFunDirect3DCreate8      = GetProcAddress(s_hD3d8Dll,_T("Direct3DCreate8"));
	if  ( NULL == s_lpFunDirect3DCreate8 ) 
	{
		OutputDebugString("取 Direct3DCreate8失败");
		return FALSE;
	}

	OutputDebugString("成功加载D3D");
	//加载adcore	
	TCHAR   szAdCoreName[ 256 +1 ];	
	::GetModuleFileName(NULL, szAdCoreName, 256);
	TCHAR	*p	= ::_tcsrchr(szAdCoreName, '\\');
	if(p != NULL)		*p	= 0;	
    //::_tcscat(szAdCoreName,_T("\\iga8core.dll"));
	::_tcscat(szAdCoreName,_T("\\Emmoaz.iaf"));

	::OutputDebugString(szAdCoreName);
	s_hIga8Core = ::LoadLibrary(szAdCoreName);
	if  ( NULL == s_hIga8Core )
	{
		::OutputDebugString("加载失败");
		return FALSE;
	}
	s_lpIGAStartup2      = GetProcAddress(s_hIga8Core,_T("IGAStartup2"));

	if  ( NULL == s_lpIGAStartup2 ) 
	{
		return FALSE;
	}




	return TRUE;
}

TCHAR  szCheckFullscreenBuf[] = {"CheckFullscreen"};
void  __declspec(naked) CheckFullscreen(void) 
{
	_asm
	{	
// 	  MOV  EDI , EDI
// 	  PUSH ESI
// 	  PUSH EDI
// 	  PUSH 0x6DD313BC 
// 	  PUSH 1
// 	  PUSH 1
// 	  PUSH 0
// 
// 	  POP  ESI
// 	  POP  ESI
// 	  POP  ESI
// 	  POP  ESI
// 	  POP  EDI
// 	  POP  ESI

//	  PUSH  szCheckFullscreenBuf
// 	  CALL  OutputDebugString	
	  
	  JMP  s_lpFunCheckFullscreen
	}
}

TCHAR  szValidatePixelShaderBuf[] = {"ValidatePixelShader"};
void  __declspec(naked) ValidatePixelShader(void) 
{
//	_asm  MOV  EDI, EDI
//     _asm  PUSH EBP
//     _asm  MOV  EBP, ESP
//     _asm  MOV  EAX, [EBP+0x10]
//     _asm  MOV  ECX, [EBP+0x0C]
//     _asm  MOV  EDX, [EBP+0x08]
//     _asm  PUSH ESI
// 
// 	_asm  POP  ESI
// 	_asm  MOV  ESP, EBP
// 	_asm  POP  EBP
//          
//	_asm  PUSH  szValidatePixelShaderBuf
//	_asm  CALL  OutputDebugString 
	
	_asm   JMP  s_lpFunValidatePixelShader
	
}
TCHAR  szValidateVertexShaderBuf[] = {"ValidateVertexShader"};
void  __declspec(naked) ValidateVertexShader(void) 
{
// 	_asm  MOV   EDI, EDI
//     _asm  PUSH  EBP
// 	_asm  MOV   EBP, ESP
// 	_asm  PUSH  0FFFFFFFFH
// 	
// 
// 	_asm  POP   EAX
// 	_asm  MOV   ESP , EBP 
// 	_asm  POP   EBP



	_asm  JMP  s_lpFunValidateVertexShader	
}

void  __declspec(naked) DebugSetMute(void) 
{
	_asm  JMP   s_lpFunDebugSetMute	
}



void*  _stdcall LoadEmmoaDirect3DCreate8( void* pDirect3D)
{
	typedef  VOID*  (WINAPI *LP_FUN_EMMOAX )(void*);
	if  (s_lpIGAStartup2 != NULL)
	{
        ::OutputDebugString("加载成功");
		return ((LP_FUN_EMMOAX)(s_lpIGAStartup2))(pDirect3D);

	}

    return pDirect3D;

}
void  __declspec(naked)  Direct3DCreate8()
{
	InitInstance();
// 	_asm  MOV  EDI , EDI
// 	_asm  PUSH EBP
// 	_asm  MOV  EBP , ESP
// 	_asm  SUB  ESP , 114h
// 
// 
// 	_asm  MOV  ESP , EBP
// 	_asm  POP  EBP
	

	_asm  PUSH [ESP+4]
	_asm  CALL s_lpFunDirect3DCreate8	

	_asm  PUSH EAX
	_asm  CALL LoadEmmoaDirect3DCreate8    ; 加载我们函数	

	_asm  RET 4
}
