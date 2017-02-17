// ProxyDDraw.cpp : Defines the entry point for the DLL application.
//


#include "stdafx.h"
#include "IGAWin.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				
				IGAWIN::Init();
			}
			break;
		case DLL_PROCESS_DETACH:
			{			
				IGAWIN::UnInit();				
			}
			break;
	}
    return TRUE;
}


#include "ProxyIDtirectDraw.h"

//接管DirectDrawCreate函数
void  _stdcall Hook_IDirectDraw(LPVOID  *lplpDD)
{
	::OutputDebugString("Hook_IDirectDraw");
	*lplpDD                         = new IGA_IDirectDraw((IDirectDraw*)*lplpDD);	
}
VOID  _declspec(naked) IGA2DStartup(GUID FAR *lpGUID,LPDIRECTDRAW FAR *lplpDD,VOID FAR *pUnkOuter )
{
	_asm 
	{
		PUSH  lplpDD		
		CALL  Hook_IDirectDraw	


		POPAD		
		POP   ESI 		
		POP   EBP		

		RETN  12
	}	
}


//接管DirectDrawCreateEx函数
void  _stdcall Hook_IDirectDrawEx(REFIID  iid,LPVOID  *lplpDD)
{
	::OutputDebugString("Hook_IDirectDrawEx");
	if ( ::IsEqualIID(iid, Proxy_IDirectDraw  )) 
	{		 
		*lplpDD =  new IGA_IDirectDraw((IDirectDraw*)*lplpDD);
		::OutputDebugString("IDirectDraw");
	}
	else if ( ::IsEqualIID(iid, Proxy_IDirectDraw2 )) 
	{
		*lplpDD = new IGA_IDirectDraw2((IDirectDraw2*)*lplpDD);		                         
		::OutputDebugString("IDirectDraw2");
	}
	else if ( ::IsEqualIID(iid, Proxy_IDirectDraw4)) 
	{
		*lplpDD = new IGA_IDirectDraw4((IDirectDraw4*)*lplpDD);		
		::OutputDebugString("IDirectDraw4");
	}
	else if ( ::IsEqualIID(iid, Proxy_IDirectDraw7)) 
	{
		*lplpDD = new IGA_IDirectDraw7((IDirectDraw7*)*lplpDD);		
		::OutputDebugString("IDirectDraw7");
	}
}

VOID  _declspec(naked) IGA2DStartupEx( GUID FAR * lpGuid, LPVOID  *lplpDD, REFIID  iid,VOID FAR *pUnkOuter )
{
	_asm 
	{
		PUSH  lplpDD
		PUSH  iid
	    CALL  Hook_IDirectDrawEx	

    
		POPAD
		MOV   EAX , 0
		POP   EDI 
		POP   ESI
		POP   EBX
		POP   EBP		

		RETN  16
	}	
}		
    