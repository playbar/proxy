//////////////////////////////////////////////////////////////////////
//************************************************************************************************************
// purpose    :  整理成单独dll
// Date       :  2008-6-3  10:56:18
// author     :  3o3afe1 
//************************************************************************************************************
#if !defined(COMPRESSFILE_H)
#define COMPRESSFILE_H


class __declspec(dllimport) KUnzip 
{
public:
	KUnzip();	
	~KUnzip();
	BOOL Unzip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath);    
	
private:	
	BOOL		m_bInited;
	HINSTANCE	m_hInst;
	VOID*	    m_pfnUserFunctions;
	VOID*		m_pfnWiz_SingleEntryUnzip;
	VOID*		m_pDCL;
};

class __declspec(dllimport) KZip 
{
public:
	KZip();
	KZip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath);
	~KZip();
	BOOL Zip(LPCTSTR lpszSourceZip, LPCTSTR lpszTargetPath);    
	
private:
	VOID*		m_pInfoZip;
	BOOL		m_bInited;
};



#endif // !defined(COMPRESSFILE_H)
