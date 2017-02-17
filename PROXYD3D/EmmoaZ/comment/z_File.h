#if !defined(_Z_FILE_H)
#define _Z_FILE_H

#pragma once

class ZFile 
{
public:
	enum SeekPosition 
	{ 
		begin	= 0x0, 
		current = 0x1, 
		end		= 0x2 
	};
	ZFile();
	ZFile(LPCTSTR lpszFileName, BOOL bReadOnly = TRUE);
	virtual ~ZFile();
	
	INT     Open()                                  ;
	INT     Open(LPCTSTR, UINT = OPEN_EXISTING)                 ;					
	void    Close()					                ;				
	BOOL    Seek(LONG nOffset, DWORD dwSeekMode= 0) ;
	
	
	INT     Write(const void *pData, DWORD dwLength);
	INT     Read(void *pData, DWORD dwLength)		;
	BOOL    SetEnd()								;

	VOID            SetFileName(LPCTSTR lpszFileName)       ;		
	DWORD           GetLength()	const                       ;

public:
	inline BOOL    IsOpened()	const						{	return	(m_hFile != INVALID_HANDLE_VALUE);	}
	inline LPCTSTR GetFileName()							{	return	(LPCTSTR)m_FileName;	}

protected:
	virtual void     OnLoaded()	{ }
protected:
	TCHAR	    m_FileName[_MAX_PATH];
	HANDLE      m_hFile;
	OVERLAPPED  m_ol;
	BOOL        m_bReadOnly;
public:
	static  BOOL   IsExisted      (LPCTSTR lpszFileName);
	
	
	static	BOOL   DeletePath     (LPCTSTR lpszPathName, BOOL bIncludeSubFolder = TRUE);
	static  BOOL   CopyPath       (LPCTSTR lpszSourcePath, LPCTSTR lpszTargetPath, BOOL bIncludeSubFolder = TRUE);	
    static  BOOL   CreatePath     (LPCTSTR lpszPathName);

	static	BOOL   DeleteFile     (LPCTSTR lpszFileName)									;
	static  BOOL   CopyFile       (LPCTSTR lpszSource, LPCTSTR lpszTarget, BOOL bFailIfExists);
    static  BOOL   CreateFile     (LPCTSTR lpszFileName, bool bRecursive = true);
	
	static  BOOL   IsDirectory    (LPCTSTR lpszFileName);
	
	
 	static  DWORD  GetFileVersion (LPCTSTR lpszFileName, LPTSTR pszVersion, DWORD &dwSize);
 	static  DWORD  GetFileVersion (LPCTSTR lpszFileName, DWORD* pdwFileVersion = NULL, DWORD *pdwFileHighVersion = NULL, DWORD *pdwProductVersion = NULL, DWORD *pdwProductHighVersion = NULL);
	
	static  void   MakeReadWriteable(LPCTSTR lspzFileName);
	static	DWORD  GetFileCRC32   (LPCTSTR lspzFileName, int nType = 0);
};



#endif // !defined(_Z_FILE_H)
