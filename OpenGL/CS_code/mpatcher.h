
#ifndef MPATCHER_H
#define MPATCHER_H

#define WIN32_LEAN_AND_MEAN
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <winnt.h>

class CMemoryPatcher
{
	public:
    	CMemoryPatcher();
        ~CMemoryPatcher();
		bool	Init();
		const char	*	GetErrorText();
		bool	FindProcess (const char * module_name, PROCESSENTRY32 * procentry);
		bool	FindModuleInProcess (const char * module_name, DWORD pid, MODULEENTRY32 * modentry);
		bool	FindModule (const char * module_name, MODULEENTRY32 * modentry);
		bool	FindModule (DWORD pid, DWORD address, MODULEENTRY32 * modentry);
		DWORD	FindPattern (DWORD pid, BYTE * pattern, DWORD pattern_length, DWORD start_addr, DWORD process_len);
		DWORD	FindPatternWithMask (DWORD pid, BYTE * pattern, BYTE * mask, DWORD pattern_length, DWORD start_addr, DWORD process_len);
		bool	FindPeDirectory (MODULEENTRY32 * modentry, DWORD dir, IMAGE_DATA_DIRECTORY * pedir);
		bool	FindImportTable (MODULEENTRY32 * modentry, DWORD funcaddr, DWORD * start, DWORD * end);
		DWORD	ReadProcess (DWORD pid, void * addr, BYTE * buf, DWORD length);
		DWORD	WriteProcess (DWORD pid, void * addr, BYTE * buf, DWORD length);
		bool	WriteProcessByte (DWORD pid, void * addr, BYTE value);
		bool	ReplaceByte (DWORD pid, void * addr, BYTE origvalue, BYTE newvalue);
		bool	Dump (const char * filename, DWORD pid, void * addr, DWORD length);
		void *	AllocProcessMemory (DWORD pid, DWORD size);
	private:
		void		Error (const char * fmt, ...);
		void		FreeProcessHandle(HANDLE hSnapShot);
		bool		ModulesLookup (DWORD pid, const char * name, DWORD addr, MODULEENTRY32 * modentry);
		DWORD		ReadFromProcessToBuf (HANDLE hProcess, void * addr, BYTE * buf, DWORD length);
		bool		ReadPEdirectoryEntry (MODULEENTRY32 * modentry, DWORD entry, IMAGE_DATA_DIRECTORY * dentry);
        bool		CompareMemoryMask (BYTE * buf, BYTE * pattern, BYTE * mask, DWORD pattern_length);
		std::string	szErrorMsg;
        HMODULE		hKernel32;
		HANDLE		(WINAPI *pfnCreateToolhelp32Snapshot) (DWORD, DWORD);
		BOOL		(WINAPI *pfnProcess32First) (HANDLE, LPPROCESSENTRY32);
		BOOL		(WINAPI *pfnProcess32Next) 	(HANDLE, LPPROCESSENTRY32);
		BOOL 		(WINAPI *pfnModule32First)  (HANDLE, LPMODULEENTRY32);
		BOOL 		(WINAPI *pfnModule32Next)   (HANDLE, LPMODULEENTRY32);
		void 		(WINAPI *pfnCloseToolhelp32Snapshot) (HANDLE);
		LPVOID 		(WINAPI *pfnVirtualAllocEx) (HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
};

#endif
