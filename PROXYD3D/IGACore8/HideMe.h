#ifndef		_HIDE_ME_H
#define		_HIDE_ME_H\


typedef struct _UNICODE_STRING { 
        USHORT Length;
        USHORT MaximumLength;
        PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT TlsIndex;
    union
    {
        LIST_ENTRY HashLinks;
        PVOID SectionPointer;
    };
    ULONG CheckSum;
    union
    {
        ULONG TimeDateStamp;
        PVOID LoadedImports;
    };
    PVOID EntryPointActivationContext;
    PVOID PatchInformation;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef LONG NTSTATUS;

typedef
 NTSTATUS 
(_stdcall *ZWQUERYINFORMATIONPROCESS)(
IN     HANDLE           ProcessHandle,
IN     ULONG			ProcessInformationClass,
OUT    PVOID            ProcessInformation,
IN     ULONG            ProcessInformationLength,
OUT    PULONG           ReturnLength  OPTIONAL);

typedef struct _RTL_DRIVE_LETTER_CURDIR { 
        USHORT Flags;
        USHORT Length;
        ULONG TimeStamp;
        UNICODE_STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;


typedef struct _PEB_LDR_DATA 
{ 
        ULONG Length;
        BOOLEAN Initialized;
        PVOID SsHandle;
        LIST_ENTRY InLoadOrderModuleList;
        LIST_ENTRY InMemoryOrderModuleList;
        LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LDR_MODULE { 
        LIST_ENTRY InLoadOrderModuleList;
        LIST_ENTRY InMemoryOrderModuleList;
        LIST_ENTRY InInitializationOrderModuleList;
        PVOID BaseAddress;
        PVOID EntryPoint;
        ULONG SizeOfImage;
        UNICODE_STRING FullDllName;
        UNICODE_STRING BaseDllName;
        ULONG Flags;
        SHORT LoadCount;
        SHORT TlsIndex;
        LIST_ENTRY HashTableEntry;
        ULONG TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;

typedef struct _RTL_USER_PROCESS_PARAMETERS { 
        ULONG MaximumLength;
        ULONG Length;
        ULONG Flags;
        ULONG DebugFlags;
        PVOID ConsoleHandle;
        ULONG ConsoleFlags;
        HANDLE StdInputHandle;
        HANDLE StdOutputHandle;
        HANDLE StdErrorHandle;
        UNICODE_STRING CurrentDirectoryPath;
        HANDLE CurrentDirectoryHandle;
        UNICODE_STRING DllPath;
        UNICODE_STRING ImagePathName;
        UNICODE_STRING CommandLine;
        PVOID Environment;
        ULONG StartingPositionLeft;
        ULONG StartingPositionTop;
        ULONG Width;
        ULONG Height;
        ULONG CharWidth;
        ULONG CharHeight;
        ULONG ConsoleTextAttributes;
        ULONG WindowFlags;
        ULONG ShowWindowFlags;
        UNICODE_STRING WindowTitle;
        UNICODE_STRING DesktopName;
        UNICODE_STRING ShellInfo;
        UNICODE_STRING RuntimeData;
        RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB_FREE_BLOCK { 
        struct _PEB_FREE_BLOCK *Next;
        ULONG Size;
} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;
typedef struct _PEB { 
        BOOLEAN InheritedAddressSpace;
        BOOLEAN ReadImageFileExecOptions;
        BOOLEAN BeingDebugged;
        BOOLEAN Spare;
        HANDLE Mutant;
        PVOID ImageBaseAddress;
        PPEB_LDR_DATA LoaderData;                                                        
        PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
        PVOID SubSystemData;
        PVOID ProcessHeap;
        PVOID FastPebLock;
        PVOID FastPebLockRoutine;
        PVOID FastPebUnlockRoutine;
        ULONG EnvironmentUpdateCount;
        PVOID *KernelCallbackTable;
        PVOID EventLogSection;
        PVOID EventLog;
        PPEB_FREE_BLOCK FreeList;
        ULONG TlsExpansionCounter;
        PVOID TlsBitmap;
        ULONG TlsBitmapBits[0x2];
        PVOID ReadOnlySharedMemoryBase;
        PVOID ReadOnlySharedMemoryHeap;
        PVOID *ReadOnlyStaticServerData;
        PVOID AnsiCodePageData;
        PVOID OemCodePageData;
        PVOID UnicodeCaseTableData;
        ULONG NumberOfProcessors;
        ULONG NtGlobalFlag;
        BYTE Spare2[0x4];
        LARGE_INTEGER CriticalSectionTimeout;
        ULONG HeapSegmentReserve;
        ULONG HeapSegmentCommit;
        ULONG HeapDeCommitTotalFreeThreshold;
        ULONG HeapDeCommitFreeBlockThreshold;
        ULONG NumberOfHeaps;
        ULONG MaximumNumberOfHeaps;
        PVOID **ProcessHeaps;
        PVOID GdiSharedHandleTable;
        PVOID ProcessStarterHelper;
        PVOID GdiDCAttributeList;
        PVOID LoaderLock;
        ULONG OSMajorVersion;
        ULONG OSMinorVersion;
        ULONG OSBuildNumber;
        ULONG OSPlatformId;
        ULONG ImageSubSystem;
        ULONG ImageSubSystemMajorVersion;
        ULONG ImageSubSystemMinorVersion;
        ULONG GdiHandleBuffer[0x22];
        ULONG PostProcessInitRoutine;
        ULONG TlsExpansionBitmap;
        BYTE TlsExpansionBitmapBits[0x80];
        ULONG SessionId;
} PEB, *PPEB;
typedef struct _PROCESS_BASIC_INFORMATION {
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG AffinityMask;
    ULONG BasePriority;
    ULONG UniqueProcessId;
    ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;

typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;	

BOOL HideModule(HMODULE hModule);
#endif