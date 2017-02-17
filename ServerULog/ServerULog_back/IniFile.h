#if !defined(Z_FILEINI_H)
#define Z_FILEINI_H

#include <string>
#include <iostream>


BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize);
BOOL WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize);

class CIniFile  
{
	enum{ MAX_PATH_ = 256	};
public:
	CIniFile(LPCTSTR szFileTotalName);
	CIniFile(){};
	~CIniFile();

	VOID Attach(LPCTSTR szFileTotalName);
	bool SetIniFileName(LPCTSTR szFileTotalName);

	BOOL GetString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue,int size,LPCTSTR szDefaultVal = L"" );
	BOOL GetInt   (LPCTSTR szSection,LPCTSTR szKeyName,int& nValue,int DefaultVal = 0 );

	BOOL WriteString(LPCTSTR szSection,LPCTSTR szKeyName,LPTSTR szValue, LPCTSTR szDefaultVal = L"" );
	BOOL WriteInt   (LPCTSTR szSection,LPCTSTR szKeyName,int nValue,int DefaultVal = 0 );


private:
	TCHAR   szFileName[MAX_PATH_];

public:
    static LPCTSTR GetAppPath();
	static LPCTSTR GetAppPath(LPCTSTR szFileName);
};

#endif // !defined Z_FILEINI_H
