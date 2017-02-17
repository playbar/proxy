/*=======================================================
				  MATRIX X v0.2
					Coded by 
	                 Sk||Z
=========================================================*/
#include <windows.h>
#include <tlhelp32.h>

#pragma optimize("", off)

#undef  OLD_HEADERS
//#define OLD_HEADERS

//without the platform sdk you will get 2 errors in this file
//just change the dwords in pdwords ... 

#define MakePtr(Type, Base, Offset) ((Type)(DWORD(Base) + (DWORD)(Offset)))

BOOL InterceptDllCall(HMODULE hLocalModule,const char* c_szDllName,const char* c_szApiName,PVOID pApiNew,PVOID* p_pApiOrg,PVOID pApiToChange)
{
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hLocalModule;
    PIMAGE_NT_HEADERS pNTHeader;
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
    DWORD dwProtect;
	BOOL bSuccess = FALSE; 
    
    DWORD dwAddressToIntercept; 

	if (pApiToChange) {
		dwAddressToIntercept = (DWORD)pApiToChange;
	}
	else {
		dwAddressToIntercept = (DWORD)GetProcAddress(
			GetModuleHandle((char*)c_szDllName), (char*)c_szApiName
		) /*GetProcAddress*/;
	} /*iff*/;

    if (IsBadReadPtr(hLocalModule, sizeof(PIMAGE_NT_HEADERS)))
        return FALSE;
    
    if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
        return FALSE;
    
    pNTHeader = MakePtr(PIMAGE_NT_HEADERS, pDOSHeader, pDOSHeader->e_lfanew);
    if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
        return FALSE;
    
    pImportDesc = MakePtr(
		PIMAGE_IMPORT_DESCRIPTOR, hLocalModule, 
        pNTHeader->OptionalHeader.DataDirectory[
			IMAGE_DIRECTORY_ENTRY_IMPORT
		] /*pNTHeader->OptionalHeader.DataDirectory*/.VirtualAddress
	) /*MakePtr*/;
    
    if (pImportDesc == (PIMAGE_IMPORT_DESCRIPTOR)pNTHeader) return FALSE;
    
	while (pImportDesc->Name) {
		PIMAGE_THUNK_DATA pThunk;
    
		pThunk = MakePtr(
			PIMAGE_THUNK_DATA, hLocalModule, pImportDesc->FirstThunk
		) /*MakePtr*/;
    
		while (pThunk->u1.Function) {
			if (DWORD(pThunk->u1.Function) == dwAddressToIntercept) {	
				if (
					!IsBadWritePtr(
						(LPVOID)(&pThunk->u1.Function), sizeof(DWORD)
					) /*!IsBadWritePtr*/
				){
					if (p_pApiOrg) 
						*p_pApiOrg = PVOID(pThunk->u1.Function);

					bSuccess = TRUE;
				}
				else {
					if (
						VirtualProtect(
							(LPVOID)(&pThunk->u1.Function), sizeof(DWORD),
							PAGE_EXECUTE_READWRITE, &dwProtect
						) /*VirtualProtect*/
					){
						DWORD dwNewProtect;

						if (p_pApiOrg) 
							*p_pApiOrg = PVOID(pThunk->u1.Function);

						bSuccess = TRUE;

						dwNewProtect = dwProtect;
						VirtualProtect(
							(LPVOID)(&pThunk->u1.Function), sizeof(DWORD),
							dwNewProtect, &dwProtect
						) /*VirtualProtect*/;
					} /*if*/
				} /*iff*/
			} /*if*/
			pThunk++;
		} /*while*/
		pImportDesc++;
	} /*while*/

    return bSuccess;
} /*InterceptDllCall(HMODULE, const char*, const char*, PVOID,PVOID*,PVOID)*/

int exportchange(char* base_addr,const char * procname,DWORD newaddr,DWORD* oldaddr)
{
	IMAGE_DOS_HEADER * doshdr = (IMAGE_DOS_HEADER *) base_addr;
	if ( doshdr->e_magic != IMAGE_DOS_SIGNATURE || doshdr->e_lfanew == 0 ) return false;
	IMAGE_NT_HEADERS * pehdr = (IMAGE_NT_HEADERS *) ((char*) base_addr + doshdr->e_lfanew);
	if ( pehdr->Signature != IMAGE_NT_SIGNATURE ) return false;
    if ( !pehdr
   || !pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress 
   || !pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size )
    return false;
	IMAGE_EXPORT_DIRECTORY * exports = (IMAGE_EXPORT_DIRECTORY *) (base_addr + pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	WORD ordinal=0;
	if ( HIWORD (procname) == 0 )
	{
       ordinal = (WORD)(LOWORD (procname) - exports->Base);
	}
    else
	{
       DWORD * names = (DWORD *) (base_addr + exports->AddressOfNames);
	   DWORD i = 0;
       for (  i = 0; i < exports->NumberOfNames; i++ )
	   {
         if ( !strcmp ((const char *) base_addr + names[i], procname) )
		 {
            ordinal = ((WORD*) (base_addr + exports->AddressOfNameOrdinals))[i];
            break;
		 }
	   }
	   if ( i == exports->NumberOfNames ) 
	   {
		   return false;
	   }
    }

	if ( ordinal < exports->NumberOfFunctions )
	{
     DWORD addr = ((DWORD*) (base_addr + exports->AddressOfFunctions))[ordinal];
     if ( addr && oldaddr) *oldaddr = (DWORD)(base_addr + addr);
	 if (newaddr) 
	 {
		 if (!IsBadWritePtr((LPVOID)(&((DWORD*) (base_addr + exports->AddressOfFunctions))[ordinal]), sizeof(DWORD)))
		 {
		 ((DWORD*) (base_addr + exports->AddressOfFunctions))[ordinal] = newaddr;
		 }
		 else
		 {
			 	 DWORD dwProtect,dwNewProtect;
				 if (VirtualProtect((LPVOID)(&((DWORD*) (base_addr + exports->AddressOfFunctions))[ordinal]), sizeof(DWORD),PAGE_EXECUTE_READWRITE, &dwProtect))
				 {
				 ((DWORD*) (base_addr + exports->AddressOfFunctions))[ordinal] = ((char*)newaddr-base_addr);
				 dwNewProtect = dwProtect;
				 VirtualProtect((LPVOID)(&((DWORD*) (base_addr + exports->AddressOfFunctions))[ordinal]), sizeof(DWORD),dwNewProtect, &dwProtect);
				 }
				 else return false;
		 }
	 }
	 return true;
	}
	return false;
}

int importchange(char* base_addr,const char* thedllname,const char* functionname,DWORD newaddr,DWORD* oldaddr)
{
	IMAGE_DOS_HEADER * doshdr = (IMAGE_DOS_HEADER *) base_addr;
	if ( doshdr->e_magic != IMAGE_DOS_SIGNATURE || doshdr->e_lfanew == 0 ) return false;
	IMAGE_NT_HEADERS * pehdr = (IMAGE_NT_HEADERS *) ((char*) base_addr + doshdr->e_lfanew);
	if ( pehdr->Signature != IMAGE_NT_SIGNATURE ) return false;
	IMAGE_IMPORT_DESCRIPTOR * impdesc = (IMAGE_IMPORT_DESCRIPTOR *) (base_addr + pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	if ( !pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress || !pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size ) return false;
	for (; impdesc->Name; impdesc++ )
	{
	if ( !impdesc->FirstThunk ) break;
	const char * dllname = base_addr + impdesc->Name;
	if (!strcmpi(dllname,thedllname))
	{
	IMAGE_THUNK_DATA * imports = (IMAGE_THUNK_DATA *) (base_addr + impdesc->OriginalFirstThunk);
    IMAGE_THUNK_DATA * thunks = (IMAGE_THUNK_DATA *) (base_addr + impdesc->FirstThunk);
	for (;; imports++, thunks++ )
    {
		ULONGLONG test_ordinal;
        const char * test_procname;
		if ( impdesc->OriginalFirstThunk )
		{
         if ( !imports->u1.Ordinal ) break;
         test_ordinal = imports->u1.Ordinal;
         test_procname = (const char *) ((IMAGE_IMPORT_BY_NAME *) (base_addr + (DWORD) imports->u1.AddressOfData))->Name;
		}
         else
		{
          if ( !thunks->u1.Ordinal ) break;
          test_ordinal = thunks->u1.Ordinal;
          test_procname = (const char *) ((IMAGE_IMPORT_BY_NAME *) (base_addr + (DWORD) thunks->u1.AddressOfData))->Name;
		}
        
		 if ( !IMAGE_SNAP_BY_ORDINAL (test_ordinal) )
		 {
			 if (!strcmp(test_procname,functionname))
			 {
			 if (oldaddr) *oldaddr = (DWORD)thunks->u1.Function; 
			 if (newaddr) 
			 {
				 if (!IsBadWritePtr((LPVOID)(&thunks->u1.Function), sizeof(DWORD)))
				 {
					thunks->u1.Function = newaddr;
				 }
				 else
				 {
				 DWORD dwProtect,dwNewProtect;
				 if (VirtualProtect((LPVOID)(&thunks->u1.Function), sizeof(DWORD),PAGE_EXECUTE_READWRITE, &dwProtect))
				 {
				 thunks->u1.Function = newaddr;
				 dwNewProtect = dwProtect;
				 VirtualProtect((LPVOID)(&thunks->u1.Function), sizeof(DWORD),dwNewProtect, &dwProtect);
				 }
				 else return false;
				 }
			 }
			 return true;
			 }		 
			
		 }
	}
	}
	}
	return false;
}

int importoffset(char* base_addr,const char* thedllname,const char* functionname,DWORD* offset)
{
	IMAGE_DOS_HEADER * doshdr = (IMAGE_DOS_HEADER *) base_addr;
	if ( doshdr->e_magic != IMAGE_DOS_SIGNATURE || doshdr->e_lfanew == 0 ) return false;
	IMAGE_NT_HEADERS * pehdr = (IMAGE_NT_HEADERS *) ((char*) base_addr + doshdr->e_lfanew);
	if ( pehdr->Signature != IMAGE_NT_SIGNATURE ) return false;
	IMAGE_IMPORT_DESCRIPTOR * impdesc = (IMAGE_IMPORT_DESCRIPTOR *) (base_addr + pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	if ( !pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress || !pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size ) return false;
	for (; impdesc->Name; impdesc++ )
	{
	if ( !impdesc->FirstThunk ) break;
	const char * dllname = base_addr + impdesc->Name;
	if (!stricmp(dllname,thedllname))
	{
	IMAGE_THUNK_DATA * imports = (IMAGE_THUNK_DATA *) (base_addr + impdesc->OriginalFirstThunk);
    IMAGE_THUNK_DATA * thunks = (IMAGE_THUNK_DATA *) (base_addr + impdesc->FirstThunk);
	for (;; imports++, thunks++ )
    {
		ULONGLONG test_ordinal;
        const char * test_procname;
		if ( impdesc->OriginalFirstThunk )
		{
         if ( !imports->u1.Ordinal ) break;
         test_ordinal = imports->u1.Ordinal;
         test_procname = (const char *) ((IMAGE_IMPORT_BY_NAME *) (base_addr + (DWORD) imports->u1.AddressOfData))->Name;
		}
         else
		{
          if ( !thunks->u1.Ordinal ) break;
          test_ordinal = thunks->u1.Ordinal;
          test_procname = (const char *) ((IMAGE_IMPORT_BY_NAME *) (base_addr + (DWORD) thunks->u1.AddressOfData))->Name;
		}
        
		 if ( !IMAGE_SNAP_BY_ORDINAL (test_ordinal) )
		 {
			 if (!strcmp(test_procname,functionname))
			 {
			 *offset = (DWORD)&thunks->u1.Function; 	
			 return true;
			 }		 
			
		 }
	}
	}
	}
	return false;
}


char* lookup_export_name(char* base_addr, DWORD search_ordinal )
{
	IMAGE_DOS_HEADER * doshdr = (IMAGE_DOS_HEADER *) base_addr;
	if ( doshdr->e_magic != IMAGE_DOS_SIGNATURE || doshdr->e_lfanew == 0 ) return NULL;
	IMAGE_NT_HEADERS * pehdr = (IMAGE_NT_HEADERS *) ((char*) base_addr + doshdr->e_lfanew);
	if ( pehdr->Signature != IMAGE_NT_SIGNATURE ) return NULL;
	IMAGE_EXPORT_DIRECTORY* exports = (IMAGE_EXPORT_DIRECTORY*) (base_addr + pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress );
	if ( !pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress || !pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size ) return NULL;

	WORD ordinal;
	DWORD  index;
	for(index=0;index<exports->NumberOfNames;index++)
	{
		ordinal = ((WORD*) (base_addr + exports->AddressOfNameOrdinals))[index];
		if( ordinal == search_ordinal ) break;
	}
	if(index==exports->NumberOfNames) return NULL;
	char* name =  base_addr + ((DWORD*)(base_addr + exports->AddressOfNames))[index];
	return name;
}