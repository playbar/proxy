
#ifndef APIHOOK_H
#define APIHOOK_H

BOOL InterceptDllCall(HMODULE hLocalModule,const char* c_szDllName,const char* c_szApiName,PVOID pApiNew,PVOID* p_pApiOrg,PVOID pApiToChange);
int importchange(char* base_addr,const char* thedllname,const char* functionname,DWORD newaddr,DWORD* oldaddr);
int importoffset(char* base_addr,const char* thedllname,const char* functionname,DWORD* offset);
int exportchange(char* base_addr,const char * procname,DWORD newaddr,DWORD* oldaddr);

#endif