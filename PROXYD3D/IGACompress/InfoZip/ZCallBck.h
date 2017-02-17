#ifndef ZCALLBCK_H
#define ZCALLBCK_H


#if _MSC_VER >= 1000
#pragma once
#endif 

#define	BEGIN_ZIP	namespace Zip	{
#define	END_ZIP		}

BEGIN_ZIP

#define MAX_FILES	4096

#pragma pack (push)

struct ZipCallbackData
{
	DWORD m_hwndHandle;
	HWND m_pCaller;
	long int m_liVersion;
	BOOL m_bIsOperationZip;
	long int m_liActionCode;
	long int m_liErrorCode;
	long int m_liFileSize;
	char m_pszFileNameOrMsg[512];
};

typedef struct tagZippedFileHeader
{
	DWORD	dwMarker;
	WORD	wVersion;
	WORD	wGlobalFlags;
	WORD	wCompressedMode;
	WORD	wTimeLastModified;
	WORD	wDateLastModified;
	DWORD	dwCRC32;
	DWORD	dwSizeCompressed;
	DWORD	dwSizeUncompressed;
	WORD	wFileNameLength;
	CHAR	FileName[0];
}ZIPPEDFILEHEADER, *LPZIPPEDFILEHEADER;

//////////////////////////////////////////////////////////////////////////
//	数据描述符
typedef struct tagZippedBuffer
{
	DWORD	dwCRC32;
	DWORD	dwSizeCompressed;
	DWORD	dwSizeUncompressed;
}ZIPPEDBUFFER, *LPZIPPEDBUFFER;

//////////////////////////////////////////////////////////////////////////
//	压缩源文件目录区。
typedef struct tagZippedDirInfo
{
	DWORD	dwMarker;
	WORD	wVersionForCompressed;
	WORD	wVersionForDecompress;

	WORD	wGlobalFlags;
	WORD	wCompressedMode;
	
	WORD	wTimeLastModified;
	WORD	wDateLastModified;
	
	DWORD	dwCRC32;
	DWORD	dwSizeCompressed;
	DWORD	dwSizeUncompressed;
	
	WORD	wFileNameLength;
	WORD	wExtensionLength;
	
	WORD	wCommentLength;
	WORD	wDiskID;
	
	WORD	wInnerFileAttributes;
	WORD	wOuterFileAttributes;
	DWORD	dwLocalHeadOffset;
	CHAR	szFileName[0];
	//szComments[0];
	//szExtensions[0];
}ZIPPEDDIRINFO, *LPZIPPEDDIRINFO;
#pragma pack (pop)

typedef BOOL (__stdcall *ZFunctionPtrType) (ZipCallbackData*);

END_ZIP

#endif ZCALLBCK_H  //   ZCALLBCK_H
