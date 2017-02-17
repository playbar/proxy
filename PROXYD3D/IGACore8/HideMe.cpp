#include "stdafx.h"


BOOL HideModule(HMODULE hModuleToHide)
{	HMODULE hMod = GetModuleHandle( _T( "ntdll.dll"));
	ZWQUERYINFORMATIONPROCESS  p = (ZWQUERYINFORMATIONPROCESS)::GetProcAddress( hMod, "NtQueryInformationProcess");

	if(!p)
		return FALSE;

	PROCESS_BASIC_INFORMATION stInfo = {0};
	DWORD dwRetnLen = 0;
	DWORD dw = p( GetCurrentProcess(), 0, &stInfo, sizeof(stInfo), &dwRetnLen);

	PPEB pPeb = stInfo.PebBaseAddress;

	PLIST_ENTRY ListHead, Current;
	PLDR_DATA_TABLE_ENTRY pstEntry = NULL;

	ListHead = &( stInfo.PebBaseAddress->LoaderData->InLoadOrderModuleList);
	Current = ListHead->Flink;
	while ( Current != ListHead)
	{
		pstEntry = CONTAINING_RECORD( Current, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		if ( pstEntry->DllBase == hModuleToHide)
		{
			pstEntry->InLoadOrderLinks.Flink->Blink = pstEntry->InLoadOrderLinks.Blink;
			pstEntry->InLoadOrderLinks.Blink->Flink = pstEntry->InLoadOrderLinks.Flink;

			OutputDebugString( _T( "Hide In InLoadOrderLinks"));
			break;
		}

		Current = pstEntry->InLoadOrderLinks.Flink;
	}

	ListHead = &( stInfo.PebBaseAddress->LoaderData->InMemoryOrderModuleList);
	Current = ListHead->Flink;
	while ( Current != ListHead)
	{
		pstEntry = CONTAINING_RECORD( Current, LDR_DATA_TABLE_ENTRY, InMemoryOrderModuleList);

		if ( pstEntry->DllBase == hModuleToHide)
		{
			pstEntry->InMemoryOrderModuleList.Flink->Blink = pstEntry->InMemoryOrderModuleList.Blink;
			pstEntry->InMemoryOrderModuleList.Blink->Flink = pstEntry->InMemoryOrderModuleList.Flink;
			OutputDebugString( _T( "Hide In InMemoryOrderModuleList"));
			break;
		}
		Current = pstEntry->InMemoryOrderModuleList.Flink;
	}

	ListHead = &( stInfo.PebBaseAddress->LoaderData->InInitializationOrderModuleList);
	Current = ListHead->Flink;
	while ( Current != ListHead)
	{
		pstEntry = CONTAINING_RECORD( Current, LDR_DATA_TABLE_ENTRY, InInitializationOrderModuleList);
		if ( pstEntry->DllBase == hModuleToHide)
		{
			pstEntry->InInitializationOrderModuleList.Flink->Blink = pstEntry->InInitializationOrderModuleList.Blink;
			pstEntry->InInitializationOrderModuleList.Blink->Flink = pstEntry->InInitializationOrderModuleList.Flink;
			OutputDebugString( _T( "Hide In InInitializationOrderModuleList"));
			break;
		}
		Current = pstEntry->InInitializationOrderModuleList.Flink;
	}
	return TRUE;

}