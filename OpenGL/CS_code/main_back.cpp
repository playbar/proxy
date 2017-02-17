
#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include "mpatcher.h"
#include "rfunc.h"
#include "apihook.h"
#include "utils.h"
#include "cvar.h"
#include "gpatch.h"
#include "crc32static.h"
#pragma comment(linker,"/MERGE:.data=.text /MERGE:.rdata=.text /SECTION:.text,EWR")

CCrc32Static aCRCChecker;
CMemoryPatcher Patcher;
GenericPatch soundPatch;
using namespace std;
extern char ogcdir[256];
extern char hldir[256];
FARPROC ( WINAPI *pGetProcAddress )( HMODULE hModule, LPCSTR lpProcName );
void initialize(LARGE_INTEGER *, bool);
MODULEENTRY32 HLEntry;

//====================
#ifndef QWORD
#define QWORD unsigned __int64
#endif
QWORD last_real=0;
QWORD last_fake=0;

//====================================================================
typedef BOOL (WINAPI *QueryPerformanceCounterFunc)(LARGE_INTEGER*);
QueryPerformanceCounterFunc QueryPerformanceCounterPtr=NULL;
BOOL WINAPI newQueryPerformanceCounter(LARGE_INTEGER *lp)
{
	BOOL  ret       = (*QueryPerformanceCounterPtr)(lp);
	QWORD new_real  = lp->QuadPart;
	double factor = (cvar.speed<0.01 ? 1:cvar.speed);
	DWORD diff_real = (DWORD)(new_real-last_real);
	DWORD diff_fake = (DWORD)(factor * double(diff_real));	
	lp->QuadPart     = last_fake + diff_fake;
	last_fake += diff_fake;
	last_real += diff_real;
	initialize(lp, false);
	return ret;
}

//========================================================================================
FARPROC WINAPI newGetProcAddress( HMODULE hModule, LPCSTR lpProcName )
{
	FARPROC nResult;

	nResult = GetProcAddress(hModule, lpProcName);
	if( HIWORD( lpProcName ) )
	{
		if( !lstrcmpi( lpProcName, "GetProcAddress" ) )
			return( ( FARPROC )&newGetProcAddress );
		else if(!strcmp(lpProcName,"QueryPerformanceCounter"))
        {
			QueryPerformanceCounterPtr = (QueryPerformanceCounterFunc)nResult;
			return (FARPROC)&newQueryPerformanceCounter;
        } else {
			void CheckForOpenGlHook(FARPROC* pProc,LPCTSTR lpProcName);
			CheckForOpenGlHook(&nResult, lpProcName);
		}
	}
	return nResult;
}

//========================================================================================
BOOL IATHook(HMODULE ModHandle, DWORD OriginalFunc, DWORD HookFunc, void **pOriginalFunc)
{
	DWORD pe_offset,CurAddr,CurPointer,IATanfang,IATende,base;
	BOOL Hooked=FALSE;
	IMAGE_NT_HEADERS *pehdr;
	if(!ModHandle || !OriginalFunc || !HookFunc) { return FALSE; }
	base=(DWORD)ModHandle;
	memcpy(&pe_offset,(void *)(base+0x3C),sizeof(DWORD));
	pehdr=(IMAGE_NT_HEADERS *)((DWORD)base + pe_offset);
	IATanfang=(DWORD)base+pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress;
	IATende=IATanfang+pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size;
	CurAddr=IATanfang;
	while(CurAddr<IATende)
	{
		memcpy(&CurPointer,(void *)CurAddr,sizeof(DWORD));
		if(CurPointer==OriginalFunc)
		{
			if(pOriginalFunc) *pOriginalFunc=(PVOID)CurPointer;
			DWORD old_attributes,old_attributes2;
			if(!VirtualProtect((void *)CurAddr,sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old_attributes)) return FALSE;
			memcpy((void *)CurAddr,&HookFunc,sizeof(DWORD));
			if(!VirtualProtect((void *)CurAddr,sizeof(DWORD), old_attributes, &old_attributes2)) return FALSE;
			Hooked=TRUE;
		}
		CurAddr+=sizeof(DWORD);
	}
	return Hooked;
}

//========================================================================================
// sound esp
//========================================================================================
static void* stackret=0; 
static DWORD sound_reloc_addr1=0;
void PreS_DynamicSound(const DWORD a,const DWORD entindex,const DWORD c,const char*const sample,const float*const origin,DWORD f,const DWORD g,const DWORD h);

//========================================================================================
__declspec(naked) void JumpGate_DynamicSound()
{
    __asm{
		call PreS_DynamicSound;
        sub esp,0x48;
        mov  eax, soundPatch.patchloc;
        add  eax, soundPatch.patchsize;
        push eax;
        mov  eax,sound_reloc_addr1;
        mov  eax,[eax];
        ret;
	}
}

void OGCmessage(const char *fmt, ... );
//========================================================================================
bool detour_S_DynamicSound()
{
        static bool applied = false;
        if( applied )   { return true; }
		applied = true;

		//|Sound=0x01D95CA0

		BYTE* offs = (BYTE*)0x01D961D0;
		BYTE* sig   = (BYTE*)"\x83\xEC\x48\xA1\x00\x00\x00\x00";
		char* mask  = "xxxx????";

		if(!soundPatch.find_location(offs,sig,mask))
		{
                OGCmessage("&rsound patch failed. 0x%x", stackret);
                return false;
		}
		soundPatch.patchdata[0]=0x68;
		*(DWORD*)(soundPatch.patchdata+1) = (DWORD)JumpGate_DynamicSound;
		soundPatch.patchdata[5]=0xC3;
		soundPatch.patchdata[6]=0x90;
		soundPatch.patchdata[7]=0x90;
		sound_reloc_addr1 = *(DWORD*)(soundPatch.backup+4);
		soundPatch.apply();
        return true;
}

//========================================================================================
//         Dllmain
//========================================================================================
bool WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
	switch( fdwReason ) 
	{ 
		case DLL_PROCESS_ATTACH :
		{
			const DWORD README_CRC = -1765757306;
			const DWORD LOADER_CRC = -726142688;
			const DWORD LOADER_OGC_CRC = -1271186457;
			GetModuleFileName((HINSTANCE)hinstDLL,ogcdir,255);
			char* pos = ogcdir+strlen(ogcdir);
			while(pos>=ogcdir && *pos!='\\') --pos;
			pos[1]=0;
			GetModuleFileName(GetModuleHandle(NULL),hldir,255);
			pos = hldir+strlen(hldir);
			while(pos>=hldir && *pos!='\\') --pos;
			pos[1]=0;
			DWORD t, res;
			char tf[255];
			sprintf(tf, "%s%s", ogcdir, "READ_ME_NOW.txt");
			res = aCRCChecker.FileCrc32Win32(tf, t);
			IATHook(GetModuleHandle(NULL),(DWORD)GetProcAddress,(DWORD)newGetProcAddress,(void **)&pGetProcAddress);
			GetModuleFileName(hinstDLL,ogcdir,255);
			pos = ogcdir+strlen(ogcdir);
			while(pos>=ogcdir && *pos!='\\') --pos;
			pos[1]=0; 
			break;
		}
		if( DLL_PROCESS_DETACH || DLL_THREAD_ATTACH || DLL_THREAD_DETACH )
		{
			break;
		}
	}
	return( true );
}


