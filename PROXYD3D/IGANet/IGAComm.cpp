// IGAComm.cpp: implementation of the IGAComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IGAComm.h"

#include <afxinet.h>

#include <io.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "URLDownload.h"


#	define	CMD_QUERYSDK				"EMMOA_IIS.dll?QuerySDK&Game=%d&Version=%s&SDK=%s&GP=%d&MacID=%s&Chn=%d"
#	define	PARAM_QUERYSDK				"Game Version SDK GP MacID Chn"

#	define	CMD_QUERYCONFIG				"EMMOA_IIS.dll?QueryConfig&Game=%d&Version=%s&Sample=%d&MacID=%s&Chn=%d"
#	define	PARAM_QUERYCONFIG			"Game Version Sample MacID Chn"

#	define	CMD_QUERYADINFO				"EMMOA_IIS.dll?QueryAdInfo&Game=%d&Area=%d&Sample=%d&MacID=%s&Chn=%d&CRC=%u"
#	define	PARAM_QUERYADINFO			"Game Area Sample MacID Chn CRC"
//	支持采样数值
#	define	CMD_QUERYAD					"EMMOA_IIS.dll?QueryAd&Game=%d&Sample=%d&Area=%d&&Ad=%d&Type=%d&FName=%s&MacID=%s&Chn=%d"
#	define	PARAM_QUERYAD				"Game Sample Area Ad Type FName MacID Chn"

#	define	CMD_QUERYSTART				CMD_QUERYSDK
#	define	CMD_QUERYSTART1				CMD_QUERYSDK1

#	define	CMD_QUERYEND				"EMMOA_IIS.dll?CloseGame&Game=%d&PlayTime=%d&MacID=%s&Area=%d&Chn=%d"
#	define	PARAM_QUERYEND				"Game PlayTime MacID Area Chn"

#	define	CMD_UPLOADPLAYINFO			"EMMOA_IIS.dll?UploadPlayInfo&Game=%d&PlayTime=%d&CloseTime=%d&MacID=%s&Area=%d&Chn=%d"
#	define	PARAM_UPLOADPLAYINFO		"Game PlayTime CloseTime MacID Area Chn"

#	define	CMD_UPLOADHARDWAREINFO		"EMMOA_IIS.dll?UploadHardwareInfo&Game=%d&Mac=%s&MS=%d&CPU=%s&Display=%s&OS=%s&GP=%s&Chn=%d&IP=%s"	
#	define	PARAM_UPLOADHARDWAREINFO	"Game Mac MS CPU Display OS GP Chn IP"

#	define	CMD_UPLOADDATA				"EMMOA_IIS.dll?UploadData"
#	define	PARAM_UPLOADDATA			""

#   define  CMD_MAKEBACKUP				"EMMOA_IIS.dll?MakeBackup&Name=%s&Pwd=%s"
#	define  PARAM_MAKEBACKUP			"Name Pwd"

#   define  CMD_STARTSERVER				"EMMOA_IIS.dll?StartServer&Name=%s&Pwd=%s"
#	define  PARAM_STARTSERVER			"Name Pwd"

#   define  URL_SIZE                    1024 

class CFtpFileFindEx : public CFtpFileFind
{
public:
	CFtpFileFindEx(CFtpConnection* pConnection, DWORD_PTR dwContext = 1)
		:CFtpFileFind(pConnection, dwContext)
	{
	}
	
	BOOL SetOption(DWORD dwOption, DWORD dwValue)
	{
		if(m_hContext==NULL)
			return	FALSE;
		return	::InternetSetOption((HINTERNET)m_hContext, dwOption, (void*)&dwValue, sizeof(DWORD));
	}
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
IGAComm::IGAComm(LPCTSTR szURL) :      
	  m_Protocol(0)
	,m_bEnableLog(FALSE)
{
  	if ( szURL == NULL || szURL[0] == 0 ||
		 _tcslen(szURL) >= sizeof(szURL)) 
	{
        m_szURL[0] = 0;		
	}
	else
       _tcsstr(m_szURL,szURL); 

   
}

IGAComm::~IGAComm()
{
}

VOID    IGAComm::SetProtocol(BYTE  bPro)
{
     m_Protocol = bPro;	 
}
BOOL    IGAComm::SetTopURL(LPCTSTR  lpszURL)
{
	if ( lpszURL == NULL || lpszURL[0] == 0) return FALSE;
	if ( _tcslen(lpszURL) >= _MAX_PATH)  return FALSE;

    _tcscpy(m_szURL,lpszURL); 
	return TRUE;
}

//查询sdk升级包
BOOL    IGAComm::QuerySDK(   DWORD   dwGameID, 
							 LPCTSTR lpszGameVer, 
							 LPCTSTR lpszSDK,
							 DWORD   dwType, 
							 LPCTSTR lpszMacID, 
							 DWORD dwChnID,
							 LPCTSTR lpszTmpName,
							 LPVOID  lpReserve )
{
	BOOL   bResult  = FALSE;
    
	TCHAR  szURL[URL_SIZE];
	wsprintf(szURL,_T("%s") CMD_QUERYSDK,m_szURL,dwGameID, lpszGameVer, lpszSDK,dwType, lpszMacID,  dwChnID);
//	CFile file;
//	file.Open("mazhenhui.txt",CFile::modeCreate|CFile::modeReadWrite);
//	file.Write(szURL,strlen(szURL));
//	file.Close();
	bResult = HttpDownLoad::Download(szURL,lpszTmpName,(BOOL)m_Protocol);   

	return bResult;
}


//查询配置文件
BOOL    IGAComm::QueryConfig(   DWORD   dwGameID, 
								LPCTSTR lpszVersion, 
								DWORD dwSample, 
								LPCTSTR lpszMacID , 
								DWORD dwChnID,
								LPCTSTR lpszTmpName,
								LPVOID  lpReserve   )
{
	BOOL   bResult  = FALSE;
    
	TCHAR  szURL[URL_SIZE];
	wsprintf(szURL,_T("%s") CMD_QUERYCONFIG,m_szURL,dwGameID, lpszVersion,  dwSample,  lpszMacID ,  dwChnID);
	bResult = HttpDownLoad::Download(szURL,lpszTmpName,(BOOL)m_Protocol);



	return bResult;
}


//上传硬件信息
BOOL    IGAComm::UploadHardwareInfo(   DWORD dwGameID, 
									   LPCTSTR lpszMacID, 
									   DWORD   dwMemorySize, 
									   LPCTSTR lpszCPU, 
									   LPCTSTR lpszDisplay,
									   LPCTSTR lpszOs,
									   LPCTSTR lpGP, 
									   DWORD    dwChnID,
									   LPCTSTR lpszRemoteIP,
									   LPCTSTR lpszTmpName,
									   LPVOID  lpReserve      )
{
	BOOL   bResult  = FALSE;
    
	TCHAR  szURL[URL_SIZE];
	wsprintf(szURL,_T("%s") CMD_UPLOADHARDWAREINFO,m_szURL,dwGameID,  lpszMacID,  dwMemorySize,  lpszCPU,  lpszDisplay,  lpszOs, lpGP, dwChnID,lpszRemoteIP);
	bResult = HttpDownLoad::Download(szURL,lpszTmpName,(BOOL)m_Protocol);


	return bResult;
}


//查询广告信息文件
BOOL    IGAComm::QueryAdInfo(	DWORD dwGameID, 
								DWORD dwAreaID, 
								DWORD dwSample, 
								LPCTSTR lpszMacID, 
								DWORD dwChnID,
								LPCTSTR lpszTmpName,
								LPVOID  lpReserve 	)
{
	BOOL   bResult  = FALSE;
    
	TCHAR  szURL[URL_SIZE];
	wsprintf(szURL,_T("%s") CMD_QUERYADINFO,m_szURL,dwGameID,  dwAreaID,  dwSample,  lpszMacID,  dwChnID , (DWORD) lpReserve);
	bResult = HttpDownLoad::Download(szURL,lpszTmpName,(BOOL)m_Protocol);


	return bResult;
}	
	
//查询广告
BOOL    IGAComm::QueryAd(   DWORD dwGameID, 
							DWORD dwSampleID, 
							DWORD dwAreaID, 
							DWORD dwAdID, 
							DWORD dwType, 
							LPCTSTR lpszFileName, 
							LPCTSTR lpszMacID, 
							DWORD dwChnID,
							LPCTSTR lpszTmpName,
							LPVOID  lpReserve )
{
	
	BOOL   bResult  = FALSE;
    
	TCHAR  szURL[URL_SIZE];
	wsprintf(szURL,_T("%s") CMD_QUERYAD,m_szURL,dwGameID, dwSampleID,  dwAreaID,  dwAdID,  dwType,  lpszFileName,  lpszMacID,  dwChnID);
	bResult = HttpDownLoad::Download(szURL,lpszTmpName,(BOOL)m_Protocol);


    return bResult;
}		

/*上传玩家信息*/
BOOL    IGAComm::UploadPlayInfo(   DWORD dwGameID, 
								   DWORD dwPlayTime, 
								   DWORD dwCloseTime, 
								   LPCTSTR lpszMacID, 
								   DWORD dwAreaID, 
								   DWORD dwChnID,
								   LPCTSTR lpszTmpName,
								   LPVOID  lpReserve  )
{
	
	BOOL   bResult  = FALSE;
    
	TCHAR  szURL[URL_SIZE];
	wsprintf(szURL,_T("%s") CMD_UPLOADPLAYINFO,m_szURL, dwGameID,  dwPlayTime,  dwCloseTime,  lpszMacID,  dwAreaID,  dwChnID);
	bResult = HttpDownLoad::Download(szURL,lpszTmpName,(BOOL)m_Protocol);
	
	return bResult;
}
/*上传关注度信息*/
BOOL    IGAComm::UploadData(   char*   pData,
							   DWORD   dwLen,
							   LPCTSTR lpszTmpName,
							   LPVOID  lpReserve   )
{
	BOOL   bResult  = FALSE;
    
	IGA_HttpObject    http;

	TCHAR  szURL[URL_SIZE];
	wsprintf(szURL,_T("%s") CMD_UPLOADDATA,m_szURL);
	if  ( http.Open(szURL)) 
	{
		http.SendBuffer(pData,dwLen);
        bResult = TRUE;
		http.Close();
	}
	
	return bResult;
}


BOOL    IGAComm::CloseGame(DWORD dwGameID, DWORD dwPlayTime, LPCTSTR lpszMacID, DWORD dwAreaID, DWORD dwChnID,LPCTSTR lpszTmpName,LPVOID  lpReserve)
{
	BOOL   bResult  = FALSE;
    
	TCHAR  szURL[URL_SIZE];
	wsprintf(szURL,_T("%s") CMD_QUERYEND,m_szURL,dwGameID,  dwPlayTime,  lpszMacID,  dwAreaID,  dwChnID);
	bResult = HttpDownLoad::Download(szURL,lpszTmpName,(BOOL)m_Protocol);



	return bResult;
}



BOOL    IGAComm::MakeBackup( LPCTSTR lpszName, LPCTSTR lpszPwd,LPCTSTR lpszTmpName,LPVOID  lpReserve)
{
	BOOL   bResult  = FALSE;
    
	TCHAR  szURL[URL_SIZE];
	wsprintf(szURL,_T("%s") CMD_MAKEBACKUP,m_szURL,lpszName,  lpszPwd);
	bResult = HttpDownLoad::Download(szURL,lpszTmpName,(BOOL)m_Protocol);


	return bResult;
}
BOOL    IGAComm::StartServer(LPCTSTR lpszName, LPCTSTR lpszPwd,LPCTSTR lpszTmpName,LPVOID  lpReserve)
{
	BOOL   bResult  = FALSE;
    
	TCHAR  szURL[URL_SIZE];
	wsprintf(szURL,_T("%s") CMD_STARTSERVER,m_szURL,lpszName,  lpszPwd);
	bResult = HttpDownLoad::Download(szURL,lpszTmpName,(BOOL)m_Protocol);


	return bResult;
}
//ftp下载文件

BOOL   IGAComm::QueryFileFromFtp( LPCTSTR lpszIP, 
								 LPCTSTR lpszFtpDir,
								 LPCTSTR lpszTmpName,
								 LPCTSTR lpszUserName ,
								 LPCTSTR  lpszPSW , 
								 LPVOID  lpReserve )
{
	//	smallfool
	return	FTPDownload(NULL,		//	无窗口句柄以用来接收网络下载事件消息
		lpszIP,		//	使用配置文件中的或者数据包中的	
		0,			//	从配置中加载
		lpszFtpDir, //	
		lpszTmpName, 
		NULL,		//从配置中加载FTP认证信息
		NULL,		
		FALSE,		//	不需要重新启动后台线程
		FALSE);		//	无后台下载线程，故不需要等待其下载完成
	//	End of smallfool
}

#include "afxinet.h"

#include "z_FileIni.h"


///--- Smallfool

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL IGAComm::FTPDownload(HWND hWnd, LPCTSTR lpszURL, LPCTSTR lpszTargetName, BOOL bBackendThread, BOOL bWaitResult)
{
	CString	strServer, strUserName, strObject, strPassword;
	INTERNET_PORT	nPort	= (INTERNET_PORT)DEFAULT_FTP_PORT;
	DWORD	dwServiceType	= AFX_INET_SERVICE_FTP;
	if(!AfxParseURLEx(lpszURL, dwServiceType, strServer, strObject, nPort, strUserName, strObject))
		return	FALSE;
	return	this->FTPDownload(hWnd, strServer, (UINT)nPort, strObject, lpszTargetName, strUserName, strPassword,bBackendThread, bWaitResult);

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef struct tagDownloadInfoParam
{
	HWND	hWnd;
	TCHAR	szIP[INTERNET_MAX_URL_LENGTH];
	UINT	nPort;
	TCHAR	szObject[INTERNET_MAX_PATH_LENGTH];
	TCHAR	szUserName[INTERNET_MAX_USER_NAME_LENGTH];
	TCHAR	szPassword[INTERNET_MAX_PASSWORD_LENGTH];
	
	TCHAR	szTargetName[_MAX_PATH];
	
	WPARAM	wParam;
	LPARAM	lParam;
}DOWNLOADINFOPARAM, *LPDOWNLOADINFOPARAM;


/************************************************************************/
/*                                                                      */
/************************************************************************/
DWORD WINAPI IGAComm::DownloadThreadProc(void *p)
{
	PROTECT(IGAComm::DownloadThreadProc);
	tagDownloadInfoParam	*pDownloadInfoParam	= (tagDownloadInfoParam*)p;
	ASSERT(pDownloadInfoParam != NULL);
	
	IGAComm*	pThisComm	= (IGAComm*)pDownloadInfoParam->lParam;
	if(pThisComm != NULL)
	{
		pThisComm->FTPDownload(pDownloadInfoParam->hWnd, pDownloadInfoParam->szIP, pDownloadInfoParam->nPort, pDownloadInfoParam->szObject, pDownloadInfoParam->szTargetName, pDownloadInfoParam->szUserName, pDownloadInfoParam->szPassword, FALSE);
	}
	::HeapFree(::GetProcessHeap(), 0, pDownloadInfoParam);

	return	0L;
	UNPROTECT
	return	-1;
}

//	操作的结果状态
#define DS_OK					0x00000001
#define DS_FAILED				0x00000002
#define DS_EXCEPTION			0x00000004
#define DS_OTHERS				0x00000008	//

//	操作指令
#define OPERATION_SESSION		0x00010000
#define OPERATION_CONNECTION	0x00020000
#define OPERATION_SIZE			0x00040000
#define OPERATION_FILE			0x00080000
#define OPERATION_DOWNLOAD		0x00100000
//	销毁对象
#define OPERATION_DESTROY		0x00200000	//

//	操作的运行状态
#define	DOWNLOAD_START			0x00000100
#define	DOWNLOAD_DOWNLOADING	0x00000200
#define DOWNLOAD_END			0x00000400

//	日志类型
#define LOGSTYLE_DBG			0x10000000
#define	LOGSTYLE_RUN			0x20000000
#define	LOGSTYLE_ERR			0x40000000

//	日志输出类型
#define	LOGOUTPUT_TOFILE		0x00000001
#define LOGOUTPUT_TODBG			0x00000002
#define LOGOUTPUT_WINDOW		0x00000004



/************************************************************************/
/*                                                                      */
/************************************************************************/
class CFtpWrapper
{
	CFtpWrapper(const CFtpWrapper&);
	CFtpWrapper operator=(const CFtpWrapper&);
public:
	CFtpWrapper(IGAComm *pComm)
		:m_pComm(pComm)
	{
		m_nTaskID				= ::InterlockedIncrement(&g_nDownloadID);
		
		m_hWnd					= NULL;
		m_bEnableLog			= FALSE;
		m_bPassive				= TRUE;
		m_bUsingMD5				= FALSE;
		m_nPort					= DEFAULT_FTP_PORT;
		m_bSupportResume		= TRUE;
		m_bAutoRetry			= TRUE;
		m_dwMaxRetryTimes		= 5;
		m_dwRetryTimeout		= 1000;
		m_dwMaxTimeout			= 5000;
		m_dwMaxDownloadSpeed	= 0;
		m_bHoldLowDownloadSpeed	= FALSE;
		m_dwDownloadRestClearance	= 0;
		_tcsncpy(m_szServer, DEFAULT_FTP_IP, _MAX_PATH);
		_tcsncpy(m_szUserName, DEFAULT_FTP_ACCOUNT,_MAX_PATH);
		_tcsncpy(m_szPassword, DEFAULT_FTP_PASSWORD, _MAX_PATH);
		*m_szRemoteFile	= 0;
		*m_szLocalFile	= 0;
	}
	BOOL LoadConfig();
	BOOL Download(LPCTSTR lpszIP,UINT nPort, LPCTSTR lpszObject, LPCTSTR lpszTargetName, LPCTSTR lpszUserName, LPCTSTR lpszPassword );
	void SetMessager(HWND hWnd);
private:
	IGAComm	*m_pComm;
	HWND	m_hWnd;
	
	BOOL	m_bEnableLog;
	BOOL	m_bPassive;

	BOOL	m_bUsingMD5;
	TCHAR	m_szLogName[_MAX_PATH];
	
	TCHAR	m_szServer[_MAX_PATH];
	UINT	m_nPort;
	TCHAR	m_szUserName[_MAX_PATH];
	TCHAR	m_szPassword[_MAX_PATH];
	
	TCHAR	m_szRemoteFile[_MAX_PATH];
	TCHAR	m_szLocalFile[_MAX_PATH];
	
	BOOL	m_bSupportResume;

	BOOL	m_bAutoRetry;
	DWORD	m_dwMaxRetryTimes;
	DWORD	m_dwRetryTimeout;
	DWORD	m_dwMaxTimeout;
	
	DWORD	m_dwMaxDownloadSpeed;

	BOOL	m_bHoldLowDownloadSpeed;
	DWORD	m_dwDownloadRestClearance;

	LONG	m_nTaskID;
public:
	static	LONG  g_nDownloadID;
	static	DWORD m_dwErrorLog;
	static	DWORD m_dwRunLog;
	static	DWORD m_dwDebugLog;
private:
	int DownloadDirect(tagDownloadStatus* lpds, CFtpConnection* pConnection, LPCTSTR lpszRemoteFile, LPCTSTR lpszTargetName, DWORD dwRemoteFileSize, DWORD dwLocalFileSize);
	int ResumeDownload(tagDownloadStatus* lpds, CFtpConnection* pConnection, LPCTSTR lpszRemoteFile, LPCTSTR lpszTargetName, DWORD dwRemoteFileSize, DWORD dwLocalFileSize);
	CInternetSession *OpenSession(tagDownloadStatus* lpds, LPCTSTR pstrAgent = NULL,
		DWORD_PTR dwContext = 1,
		DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS,
		LPCTSTR pstrProxyName = NULL,
		LPCTSTR pstrProxyBypass = NULL,
		DWORD dwFlags = 0);
	CFtpConnection *Connect(tagDownloadStatus* lpds, CInternetSession* pSession,LPCTSTR lpszServer, UINT nPort, LPCTSTR lpszUserName, LPCTSTR lpszPassword);
	int GetRemoteFileSize(tagDownloadStatus* lpds, CFtpConnection* pConnection, LPCTSTR lpszRemoteFile);
	int GetLocalFileSize(tagDownloadStatus* lpds, LPCTSTR lpszLocalFile);
	CInternetFile *Open(tagDownloadStatus* lpds, CFtpConnection* pConnection, LPCTSTR lpszFileName, BOOL bAsciiType = FALSE);
	int DownloadImpl(tagDownloadStatus* lpds, CInternetFile* pRemoteFile, FILE* pLocalFile,DWORD dwBytesToRead);
	UINT Read(tagDownloadStatus* lpds, CInternetFile* pFile, void *pBuffer, DWORD dwBytesToRead);
	BOOL WriteToFile(tagDownloadStatus* lpds, FILE* pFile, const char *pBuffer, DWORD dwBytesToWrite);
	void HandleException(tagDownloadStatus* lpds, CInternetException* e, int nRetry = -1);
private:
	BOOL SendMsg(WPARAM wParam, LPARAM lParam);
	void Log(DWORD dwStyle, LPCTSTR lpszFormat, ...);
	void LogTextToFile(LPCTSTR lpszText);
	void LogTextToDbg(LPCTSTR lspzText);
	void LogTextToWindow(LPCTSTR lpszText);
	BOOL MD5Decompress(LPSTR pszTarget, LPCSTR lpszSource)
	{
		_tcsncpy(pszTarget, lpszSource, _MAX_PATH);
		return	TRUE;
	}
};

DWORD CFtpWrapper::m_dwErrorLog		= LOGSTYLE_ERR | LOGOUTPUT_TOFILE | LOGOUTPUT_TODBG;
DWORD CFtpWrapper::m_dwRunLog		= LOGSTYLE_RUN | LOGOUTPUT_TOFILE | LOGOUTPUT_TODBG;
DWORD CFtpWrapper::m_dwDebugLog		= LOGSTYLE_DBG | LOGOUTPUT_TODBG;
LONG  CFtpWrapper::g_nDownloadID	= 0;

BOOL WINAPI FTPURLDownloadToFile(HWND hWnd, LPCTSTR lpszURL, LPCTSTR lpszTargetFile, BOOL bForceToReload)
{
	IGAComm	theComm("");
	if(lpszURL == NULL || !_tcslen(lpszURL) )
		return	theComm.FTPAutoDownload(hWnd , lpszTargetFile);
	return	theComm.FTPDownload(hWnd , lpszURL, lpszTargetFile, TRUE, TRUE);
}


extern HINSTANCE	g_hInst;
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL IGAComm::FTPDownload(HWND hWnd, 
						  LPCTSTR lpszIP,
						  UINT nPort, 
						  LPCTSTR lpszObject, 
						  LPCTSTR lpszTargetName, 
						  LPCTSTR lpszUserName /* = NULL */, 
						  LPCTSTR lpszPassword /* = NULL */, 
						  BOOL bBackendThread /* = FALSE  */, 
						  BOOL bWaitResult /* = FALSE*/)
{
	if(bBackendThread)
	{
		tagDownloadInfoParam	*pDownloadInfoParam	= (tagDownloadInfoParam*)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(tagDownloadInfoParam));
		if(!pDownloadInfoParam)
			return	FALSE;
		pDownloadInfoParam->hWnd	= hWnd;
		pDownloadInfoParam->nPort	= nPort;
		if(lpszIP != NULL)
			::_tcsncpy(pDownloadInfoParam->szIP, lpszIP, INTERNET_MAX_URL_LENGTH);
		ASSERT(lpszObject != NULL);
		::_tcsncpy(pDownloadInfoParam->szObject, lpszObject, INTERNET_MAX_PATH_LENGTH);
		if(lpszUserName != NULL)
			::_tcsncpy(pDownloadInfoParam->szUserName, lpszUserName, INTERNET_MAX_USER_NAME_LENGTH);
		if(lpszPassword != NULL)
			::_tcsncpy(pDownloadInfoParam->szPassword, lpszPassword, INTERNET_MAX_PASSWORD_LENGTH);
		ASSERT( lpszTargetName != NULL);
		::_tcsncpy(pDownloadInfoParam->szTargetName, lpszTargetName, _MAX_PATH);
		pDownloadInfoParam->lParam	= (LPARAM)this;
		
		HANDLE	hDownloadThread	= ::CreateThread(NULL, 0, DownloadThreadProc, (void*)pDownloadInfoParam, 0, NULL);
		if(!hDownloadThread)
		{
			::HeapFree(::GetProcessHeap(), 0, pDownloadInfoParam);
			return	FALSE;
		}
		BOOL	bOk		= TRUE;
		if(bWaitResult)
		{
			WaitForSingleObject(hDownloadThread, INFINITE);
			bOk		= ::_taccess(lpszTargetName, 0) != -1;
		}
		::CloseHandle(hDownloadThread);
		hDownloadThread	= NULL;
		return	bOk;
	}
	else
	{
		CFtpWrapper	ftp(this);
		ftp.LoadConfig( );
		ftp.SetMessager(hWnd);
		return	ftp.Download( lpszIP, nPort, lpszObject, lpszTargetName, lpszUserName, lpszPassword); 
	}
	return	FALSE;
}

bool IGAComm::IsExisted(LPCTSTR lpszFileName)
{
	return ::_taccess(lpszFileName, 00) != -1;
}


BOOL IGAComm::CreatePath(LPCTSTR lpszPathName, bool bRecursive /* = true */)
{
	TCHAR	szPathName[_MAX_PATH]	= { 0 };
	_tcsncpy(szPathName, lpszPathName, _MAX_PATH);
	if(szPathName[_tcslen(szPathName)- 1] != _T('\\'))
		_tcscat(szPathName, _T("\\"));
	
	if( bRecursive )
	{
		TCHAR	szPath[_MAX_PATH]	= { 0 };
		TCHAR	*pszSource	= (TCHAR*)szPathName;
		TCHAR	*p	= NULL;
		
		while( (p = ::_tcschr(pszSource, _T('\\'))) != NULL)
		{
			::CopyMemory(szPath, szPathName, p - szPathName + 1);
			szPath[ p - szPathName + 1 ]	= 0;
			
			if(!IsExisted( szPath ))
			{
				//	Non recursive create the directory;
				if(!CreatePath( szPath, false))
					return	false;
			}
			
			//	Skip some characters such as '\\' '/', ' ';
			while( *p == _T('\\')|| *p == _T(' ')|| *p == _T('/'))
				p++;
			pszSource	= p;
		}
	}
	else if ( 0 != ::CreateDirectory(szPathName, NULL) )
	{
		return	true;
	}

	return	false;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL IGAComm::FTPAutoDownload(HWND hWnd, LPCTSTR lpszPathName)
{
	//	从配置文件中加载待下载任务清单
	TCHAR	szPathName[_MAX_PATH]	= { 0 };
	::GetModuleFileName(g_hInst, szPathName, _MAX_PATH);
	TCHAR	*p	= _tcsrchr(szPathName, _T('\\'));
	if(p == NULL)
		p	= _tcsrchr(szPathName, _T('/'));
	p++;
	*p	= 0;
	
	TCHAR	szIniFile[_MAX_PATH]	= { 0 };
	_sntprintf(szIniFile, _MAX_PATH, _T("%s%s"), szPathName, IDS_INI_FILENAME);

	int		nCount	= ::GetPrivateProfileInt(_T("List"), _T("Count"), 0, szIniFile);
	//
	TCHAR	szBuffer[_MAX_PATH]	= { 0 };
	TCHAR	szPath[_MAX_PATH]	= { 0 };
	//	
	DWORD	dwRet	= 0L;
	if(lpszPathName == NULL || _tcslen(lpszPathName) == 0 )
	{
		dwRet	= ::GetPrivateProfileString(_T("List"), _T("TargetPath"), _T(""), szPath, _MAX_PATH, szIniFile);
	}
	else
	{
		_tcsncpy(szPath, lpszPathName, _MAX_PATH);
		if(szPath[_tcslen(szPath)-1] != _T('\\'))
			_tcscat(szPath, _T("\\"));
	}
	TCHAR	szServer[_MAX_PATH]	= { 0 };
	TCHAR	szUserName[_MAX_PATH]	= { 0 };
	TCHAR	szPassword[_MAX_PATH]	= { 0 };
	
	dwRet	= ::GetPrivateProfileString(_T("FTP"), _T("Server"), DEFAULT_FTP_IP, szServer, sizeof(szServer), szIniFile);
	dwRet	= ::GetPrivateProfileString(_T("FTP"), _T("UserName"), DEFAULT_FTP_ACCOUNT, szUserName, sizeof(szUserName), szIniFile);
	dwRet	= ::GetPrivateProfileString(_T("FTP"), _T("Password"), DEFAULT_FTP_PASSWORD, szPassword, sizeof(szPassword), szIniFile);
	UINT	nPort= GetPrivateProfileInt(_T("FTP"), _T("Port"), DEFAULT_FTP_PORT, szIniFile );
	
	CStringArray	arrSource;
	int nIndex = 0;
	for( nIndex = 0; nIndex < nCount; nIndex++)
	{
		CString	str1;
		str1.Format(_T("File%d"), nIndex+1);
		*szBuffer	= 0;
		dwRet	= ::GetPrivateProfileString(_T("List"), str1, _T(""), szBuffer, _MAX_PATH, szIniFile);
		ASSERT(dwRet < _MAX_PATH);
		if(_tcslen(szBuffer))
		{
			arrSource.Add(szBuffer);
			*szBuffer	= 0;
		}
	}

	if(!IsExisted(lpszPathName))
		CreatePath(lpszPathName, true);
	
	
	for(nIndex =0 ;nIndex < arrSource.GetSize(); nIndex++)
	{
		CString	strSource	= arrSource.GetAt(nIndex);
		const TCHAR *p	= _tcsrchr((LPCTSTR)strSource, _T('/'));
		ASSERT(p != NULL);
		CString	strTargetName;
		strTargetName.Format(_T("%s%s"), lpszPathName, p + 1);
		this->FTPDownload(hWnd, szServer, nPort, arrSource.GetAt(nIndex), strTargetName, szUserName, szPassword, TRUE, FALSE);
	}

	arrSource.RemoveAll();	

	return	FALSE;
}



#define RETRYBEGIN		DWORD	nRetry	= 0;	for(;;)	{
#define RETRYEND		if(m_bAutoRetry && m_dwMaxRetryTimes)	{\
							if(++nRetry >= m_dwMaxRetryTimes)	\
								return	0;						\
							if(m_dwRetryTimeout)				\
								::Sleep(m_dwRetryTimeout);		\
						}else break;	\
					}
#define FTPLOG	Log



/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CFtpWrapper::LoadConfig()
{
	TCHAR	szPathName[_MAX_PATH]	= { 0 };
	::GetModuleFileName(g_hInst, szPathName, _MAX_PATH);
	TCHAR	*p	= _tcsrchr(szPathName, _T('\\'));
	if(p == NULL)
		p	= _tcsrchr(szPathName, _T('/'));
	p++;
	*p	= 0;
	
	TCHAR	szIniFile[_MAX_PATH]	= { 0 };
	_sntprintf(szIniFile, _MAX_PATH, _T("%s%s"), szPathName, IDS_INI_FILENAME);
	
	this->m_nPort= GetPrivateProfileInt(_T("FTP"), _T("Port"), DEFAULT_FTP_PORT, szIniFile );
	ASSERT(this->m_nPort != 0);
	
	this->m_bUsingMD5	= ::GetPrivateProfileInt(_T("FTP"), _T("UsingMD5"), 1, szIniFile) == 1;
		
	TCHAR	szBuffer[_MAX_PATH]	= { 0 };
	DWORD	dwRet	= ::GetPrivateProfileString(_T("FTP"), _T("Server"), DEFAULT_FTP_IP, szBuffer, sizeof(szBuffer), szIniFile);
	ASSERT(dwRet < _MAX_PATH);
	if(this->m_bUsingMD5)
	{
		TCHAR	szBuffer1[_MAX_PATH]	= { 0 };
		this->MD5Decompress(szBuffer1, szBuffer);
		_tcscpy(m_szServer, szBuffer1);
	}
	else
	{
		_tcscpy(m_szServer, szBuffer);
	}
	*szBuffer	= 0;
	
	dwRet	= ::GetPrivateProfileString(_T("FTP"), _T("UserName"), DEFAULT_FTP_ACCOUNT, szBuffer, _MAX_PATH, szIniFile);
	ASSERT(dwRet < _MAX_PATH);
	if(this->m_bUsingMD5)
	{
		TCHAR	szBuffer1[_MAX_PATH]	= { 0 };
		this->MD5Decompress(szBuffer1, szBuffer);
		_tcscpy(m_szUserName, szBuffer1);
	}
	else
	{
		_tcscpy(m_szUserName, szBuffer);
	}
	*szBuffer	= 0;
	
	dwRet	= ::GetPrivateProfileString(_T("FTP"), _T("Password"), DEFAULT_FTP_PASSWORD, szBuffer, _MAX_PATH, szIniFile);
	ASSERT(dwRet < _MAX_PATH);
	if(this->m_bUsingMD5)
	{
		TCHAR	szBuffer1[_MAX_PATH]	= { 0 };
		this->MD5Decompress(szBuffer1, szBuffer);
		_tcscpy(m_szPassword, szBuffer1);
	}
	else
	{
		_tcscpy(m_szPassword, szBuffer);
	}
	*szBuffer	= 0;
	
	m_bEnableLog				= ::GetPrivateProfileInt(_T("FTP"), _T("EnableLog"), 0, szIniFile) == 1;
#ifdef _DEBUG
	m_bEnableLog	= TRUE;
#endif 
	dwRet	= ::GetPrivateProfileString(_T("FTP"), _T("LogFile"), _T("EMMOA_FTPNet.Log"), szBuffer, _MAX_PATH, szIniFile);
	ASSERT(dwRet < _MAX_PATH);
	_sntprintf(m_szLogName, _MAX_PATH, _T("%s%s"), szPathName, szBuffer);
	*szBuffer	= 0;
	
	this->m_bAutoRetry			= ::GetPrivateProfileInt(_T("FTP"), _T("AutoRetry"), 1, szIniFile) == 1;
	if(this->m_bAutoRetry)
		this->m_dwMaxRetryTimes	= ::GetPrivateProfileInt(_T("FTP"), _T("MaxRetryTimes"), 5, szIniFile);
	this->m_dwRetryTimeout		= ::GetPrivateProfileInt(_T("FTP"), _T("RetryTimeout"), 1000, szIniFile);
	this->m_dwMaxTimeout		= ::GetPrivateProfileInt(_T("FTP"), _T("MaxTimeout"), 5000, szIniFile);
	
	this->m_hWnd				= (HWND)::GetPrivateProfileInt(_T("FTP"), _T("NotifyWindow"), 0, szIniFile);
	this->m_bPassive			= ::GetPrivateProfileInt(_T("FTP"), _T("Passive"), 1, szIniFile) == 1;
	this->m_dwDebugLog			= (DWORD)::GetPrivateProfileInt(_T("FTP"), _T("DebugLog"), LOGSTYLE_DBG, szIniFile);
	this->m_dwErrorLog			= (DWORD)::GetPrivateProfileInt(_T("FTP"), _T("ErrorLog"), LOGSTYLE_ERR, szIniFile);
	this->m_dwRunLog			= (DWORD)::GetPrivateProfileInt(_T("FTP"), _T("RunLog"), LOGSTYLE_RUN, szIniFile);
	this->m_dwMaxDownloadSpeed	= (DWORD)::GetPrivateProfileInt(_T("FTP"), _T("MaxDownloadSpeed"), 0, szIniFile);
	this->m_bSupportResume		= ::GetPrivateProfileInt(_T("FTP"), _T("Resume"), 1, szIniFile) == 1;

	return	TRUE;
}

void CFtpWrapper::SetMessager(HWND hWnd)
{
	m_hWnd	= hWnd;
}

template<typename T>
class AutoCloseObject
{	
public:
	AutoCloseObject(T *p)
		:m_p(p)
	{
		ASSERT(m_p != NULL);
	}
	~AutoCloseObject()
	{
		m_p->Close();
		delete	m_p;
		m_p	= NULL;
	}
private:
	T	*m_p;
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CFtpWrapper::Download( LPCTSTR lpszIP,
							 UINT nPort,
							 LPCTSTR lpszObject, 
							 LPCTSTR lpszTargetName, 
							 LPCTSTR lpszUserName, 
							 LPCTSTR lpszPassword)
{
	PROTECT(CFtpWrapper::Download)
	FTPLOG(m_dwDebugLog|m_dwRunLog, _T("Download(%s, %u, %s, %s, %s, %s)..."), lpszIP, nPort, lpszObject, lpszTargetName, lpszUserName, lpszPassword);

	CInternetSession *pSession	= NULL;
	CFtpConnection	*pConnection= NULL;
	DWORD	nRemoteFileSize		= 0;
	DWORD	nLocalFileSize		= 0;
	
	/*
	IP PORT USER PSW 如果指针为空，采用本模块自己读取的
	*/
	if(!lpszIP)
		lpszIP	= m_szServer;
	if(!nPort)
		nPort	= m_nPort;
	if(!lpszUserName)
		lpszUserName	= m_szUserName;
	if(!lpszPassword)
		lpszPassword	= m_szPassword;

	tagDownloadStatus	ds;
	::ZeroMemory(&ds, sizeof(tagDownloadStatus));
	ds.dwTaskID			= this->m_nTaskID;
	
	ds.pszIP			= (TCHAR*)lpszIP;
	ds.nPort			= nPort;
	ds.pszRemoteFile	= (TCHAR*)lpszObject;
	ds.pszLocalFile		= (TCHAR*)lpszTargetName;
	_tcsncpy(m_szRemoteFile, lpszObject, _MAX_PATH);
	_tcsncpy(m_szLocalFile, lpszTargetName, _MAX_PATH);
	
	int	nRet =	 0L;
	DWORD	dwCount		= m_bAutoRetry & m_dwMaxRetryTimes ? m_dwMaxRetryTimes : 1;
	DWORD dwIndex = 0;
	for( dwIndex = 0; dwIndex < dwCount; dwIndex++)
	{

		pSession	= this->OpenSession(&ds, _T("IGAComm_FTP_Engine"));
		if(!pSession)
		{
			ds.dwStatus	= OPERATION_DESTROY;
			SendMsg(0, (LPARAM)&ds);
			continue;
		}
		AutoCloseObject<CInternetSession>	Auto1(pSession);
		
		pConnection	= this->Connect(&ds, pSession, lpszIP, nPort, lpszUserName, lpszPassword);
		if(!pConnection)
		{
			ds.dwStatus	= OPERATION_DESTROY;
			SendMsg(0, (LPARAM)&ds);
			continue;
		}
		AutoCloseObject<CFtpConnection>	Auto2(pConnection);
		
		nRemoteFileSize	= this->GetRemoteFileSize(&ds, pConnection, lpszObject);
		//	nRemoteFileSize	== -1 表示该文件不存在
		//	nRemoteFileSize == -2 表示出现网络异常 
		
		nLocalFileSize	= this->GetLocalFileSize(&ds, lpszTargetName);
		//	nLocalFileSize == -1表示文件不存在
		//	nLocalFileSize == -2读取文件过程中出现异常
		
		FTPLOG(m_dwDebugLog, _T("Download, File sizes(0x%08x, %u, 0x%08x, %u), Index: %d..."), nRemoteFileSize, nRemoteFileSize, nLocalFileSize, nLocalFileSize, dwIndex);
		
		
		//	只有两个文件大小都有效时,才进行网络下载操作.
		if(nRemoteFileSize ==-1 || nRemoteFileSize == -2)
		{
			nRet	= -1;	//	通知上层函数不再需要进行重试操作
		}
		else if(nLocalFileSize != -1 && nLocalFileSize != -2  )	//	如果本地文件不存在或文件访问失败
		{
			if(nLocalFileSize >= nRemoteFileSize)
			{
				//	已经下载成功
				if(nLocalFileSize > nRemoteFileSize)
				{
					HANDLE	hFile	= CreateFile(lpszTargetName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if( hFile != INVALID_HANDLE_VALUE)
					{	
						::SetFilePointer(hFile, nRemoteFileSize, NULL, FILE_BEGIN);
						::SetEndOfFile(hFile);
						::CloseHandle(hFile);
					}
				}
				nRet	= 1;	//	已经下载成功，通知上层函数不再需要进行重试操作
			}
		}
		
		if(!nRet && nRemoteFileSize > 0 )	
		{
			if(!m_bSupportResume)
				nRet	=	DownloadDirect(&ds, pConnection, lpszObject, lpszTargetName, nRemoteFileSize, nLocalFileSize);
			else
				nRet	=	ResumeDownload(&ds, pConnection, lpszObject, lpszTargetName, nRemoteFileSize, nLocalFileSize);
		}
		if( nRet == 1 || nRet == -1)
		{
			ds.dwStatus	= OPERATION_DESTROY;
			SendMsg(0, (LPARAM)&ds);
			break;
		}
		//	重新进行下载操作
		ds.dwStatus	= OPERATION_DESTROY;
		SendMsg(0, (LPARAM)&ds);
	}

	FTPLOG(m_dwDebugLog|m_dwRunLog,
		_T("Download(%s, %u, %s, %s), result: %d, Index: %d!"), 
		lpszIP, nPort, lpszObject, lpszTargetName,  nRet, dwIndex);
	return	nRet == 1;
	UNPROTECT
	return	FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CInternetSession* CFtpWrapper::OpenSession(tagDownloadStatus* lpds, 
										   LPCTSTR pstrAgent /* = NULL */, 
										   DWORD_PTR dwContext /* = 1 */, 
										   DWORD dwAccessType /* = PRE_CONFIG_INTERNET_ACCESS */, 
										   LPCTSTR pstrProxyName /* = NULL */, 
										   LPCTSTR pstrProxyBypass /* = NULL */, 
										   DWORD dwFlags /* = 0 */)
{
	lpds->dwStatus	= OPERATION_SESSION;
	CInternetSession	*pSession	= NULL;
	try
	{
		FTPLOG(m_dwDebugLog|m_dwRunLog, _T("OpenSession..."));
		pSession =	new CInternetSession(pstrAgent, dwContext, dwAccessType, pstrProxyName, pstrProxyBypass, dwFlags);
		ASSERT(pSession != NULL);
		pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, m_dwMaxTimeout);
		pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, m_dwMaxTimeout);
		pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, m_dwMaxTimeout);
		lpds->dwStatus	|= (pSession != NULL) ? DS_OK : DS_FAILED;
		SendMsg(0, (LPARAM)lpds);
		FTPLOG(m_dwDebugLog|m_dwRunLog, _T("OpenSession, result: %p"),pSession);
	}
	catch(CInternetException* e)
	{
		this->HandleException(lpds, e);
		e->Delete();
	}
	return	pSession;
}	
/************************************************************************/
/*                                                                      */
/************************************************************************/
CFtpConnection *CFtpWrapper::Connect(tagDownloadStatus* lpds, 
									 CInternetSession* pSession,
									 LPCTSTR lpszServer, 
									 UINT nPort, 
									 LPCTSTR lpszUserName, 
									 LPCTSTR lpszPassword)
{
	lpds->dwStatus	= OPERATION_CONNECTION;
	
	RETRYBEGIN
	try
	{
		FTPLOG(m_dwDebugLog|m_dwRunLog, _T("Connect %s, %d..., RetryIndex: %d"), lpszServer, nPort, nRetry);
		CFtpConnection* pConnection	= pSession->GetFtpConnection(lpszServer,		//	远程主机
																lpszUserName,		//	用户帐号
																lpszPassword,		//	用户密码
																nPort,				//	指定的端口
																m_bPassive);	// PASV模式，可以在配置中加以调整
		pConnection->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, m_dwMaxTimeout);
		pConnection->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, m_dwMaxTimeout);
		pConnection->SetOption(INTERNET_OPTION_SEND_TIMEOUT, m_dwMaxTimeout);
		lpds->dwStatus	|= (pConnection != NULL) ? DS_OK : DS_FAILED;
		SendMsg(0, (LPARAM)lpds);
		FTPLOG(m_dwDebugLog|m_dwRunLog, _T("%s, %d Connected, RetryIndex: %d, result: %p"),lpszServer, nPort, nRetry, pConnection);
		return	pConnection;
	}
	catch(CInternetException *e)
	{
		// catch errors from Wininet
		this->HandleException(lpds, e, nRetry);
		e->Delete();
	}
	RETRYEND
	return	NULL;
}

/************************************************************************/
/* 函数说明: 返回指定远程文件的大小                                     */
/* 返 回 值:	实际文件大小											*/
/*				-1: 指定文件不存在										*/
/*				-2: 出现网络异常										*/
/*																		*/
/************************************************************************/
int CFtpWrapper::GetRemoteFileSize(tagDownloadStatus* lpds, CFtpConnection* pConnection, LPCTSTR lpszRemoteFile)
{
	ASSERT(lpds != NULL);
	ASSERT(pConnection != NULL);

	lpds->dwStatus	= OPERATION_SIZE;
	RETRYBEGIN
	try
	{
		FTPLOG(m_dwDebugLog|m_dwRunLog, _T("Retrieve %s size..., RetryIndex: %d"),lpszRemoteFile, nRetry);
		CFtpFileFindEx	Found(pConnection);
		Found.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, m_dwMaxTimeout);
		Found.SetOption(INTERNET_OPTION_SEND_TIMEOUT, m_dwMaxTimeout);
		if(Found.FindFile( lpszRemoteFile ) )
		{
			Found.FindNextFile();
			DWORD	dwRet	= Found.GetLength();
			Found.Close();
			lpds->dwStatus	|= (dwRet ? DS_OK : DS_FAILED);
			SendMsg(0, (LPARAM)lpds);
			FTPLOG(m_dwDebugLog|m_dwRunLog, _T("Retrieve %s size, RetryIndex: %d, result: %d"),lpszRemoteFile, nRetry, dwRet);
			return	dwRet;
		}
		DWORD	dwError	= GetLastError();
		if(dwError == ERROR_NO_MORE_FILES)
		{
			Found.Close();
			lpds->dwStatus	|= DS_OTHERS;
			SendMsg(0, (LPARAM)lpds);
			FTPLOG(m_dwErrorLog, _T("Retrieve %s size failed for no such file, RetryIndex: %d!"),lpszRemoteFile, nRetry);
			return	-1;
		}
		FTPLOG(m_dwDebugLog, _T("Retrieve %s size failed for error: %d!"),lpszRemoteFile, dwError);
	}
	catch(CInternetException* e)
	{
		// catch errors from Wininet
		this->HandleException(lpds, e, nRetry);
		e->Delete();
	}
	RETRYEND
	return	-2;
}

/************************************************************************/
/* 函数说明: 返回指定本地临时文件的大小                                 */
/* 返 回 值:	实际文件大小											*/
/*				-1: 指定文件不存在										*/
/*				-2: 文件访问异常										*/
/*																		*/
/************************************************************************/
int CFtpWrapper::GetLocalFileSize(tagDownloadStatus* lpds, LPCTSTR lpszLocalFile)
{
	PROTECT(CFtpWrapper::GetLocalFileSize)
	FTPLOG(m_dwDebugLog, _T("GetLocalFileSize(%s)..."), lpszLocalFile);
	HANDLE	hFile	= ::CreateFile(lpszLocalFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == hFile)
	{
		FTPLOG(m_dwErrorLog, _T("OpenFile(%s) failed for error: %d!"), lpszLocalFile, GetLastError());
		return	-1;
	}
	DWORD	dwRet	=	 ::GetFileSize(hFile, NULL);
	::CloseHandle(hFile);
	FTPLOG(m_dwDebugLog, _T("GetLocalFileSize(%s), result: %d!"), lpszLocalFile, dwRet);
	return	dwRet;
	UNPROTECT
	return	-2;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CFtpWrapper::DownloadDirect(tagDownloadStatus* lpds, CFtpConnection* pConnection, LPCTSTR lpszRemoteFile, LPCTSTR lpszTargetName, DWORD dwRemoteFileSize, DWORD dwLocalFileSize)
{
	return	-1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CFtpWrapper::ResumeDownload(tagDownloadStatus* lpds, CFtpConnection* pConnection, LPCTSTR lpszRemoteFile, LPCTSTR lpszTargetName, DWORD dwRemoteFileSize, DWORD dwLocalFileSize)
{
	PROTECT(CFtpWrapper::ResumeDownload)
	FTPLOG(m_dwDebugLog|m_dwRunLog, _T("ResumeDownload(%s, %s)..."), lpszRemoteFile, lpszTargetName);
	FILE	*pLocalFile				= NULL;
	int	nRet	= -1;
	CInternetFile	*pRemoteFile	= Open(lpds, pConnection, lpszRemoteFile);
	if(pRemoteFile)
	{
		AutoCloseObject<CInternetFile>	Auto1(pRemoteFile);

		//	目前还无法完全支持断点续传,因而首先将本地的未下载完成的文件删除
		if(dwLocalFileSize != -1 && !::DeleteFile(lpszTargetName))
		{
			FTPLOG(m_dwErrorLog, _T("DeleteFile(%s) failed for error: %d!"), lpszTargetName, GetLastError());
			return	-1;
		}
		//	重新开始下载
		dwLocalFileSize			= 0L;
		lpds->dwLocalFileSize	= dwLocalFileSize;
		lpds->dwRemoteFileSize	= dwRemoteFileSize;
		lpds->dwBytesToDownload	= dwRemoteFileSize - dwLocalFileSize;
		lpds->dwBytesDownloaded	= 0;

		pLocalFile	= _tfopen(lpszTargetName, _T("a+b"));
		if(!pLocalFile)
		{
			FTPLOG(m_dwErrorLog, _T("CreateFile(%s) failed for error: %d!"), lpszTargetName, GetLastError());
			return	-1;
		}
		nRet	= DownloadImpl(lpds, pRemoteFile, pLocalFile, dwRemoteFileSize - dwLocalFileSize);
		fclose(pLocalFile);
		pLocalFile	= NULL;
	}
	else
		nRet   =	0;	//	通知上一层将需要将继续循环
	FTPLOG(m_dwDebugLog|m_dwRunLog, _T("ResumeDownload(%s, %s), result: %d!"), lpszRemoteFile, lpszTargetName, nRet);
	return	nRet;
	UNPROTECT
	return	-1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CInternetFile *CFtpWrapper::Open(tagDownloadStatus* lpds, CFtpConnection* pConnection, LPCTSTR lpszFileName, BOOL bAsciiType)
{
	lpds->dwStatus	= OPERATION_FILE;
	RETRYBEGIN
	try
	{
		FTPLOG(m_dwDebugLog|m_dwRunLog, _T("OpenRemoteFile(%s), RetryIndex: %d..."), lpszFileName, nRetry);
		CInternetFile *pFile	= pConnection->OpenFile(lpszFileName, GENERIC_READ, bAsciiType ? FTP_TRANSFER_TYPE_ASCII : FTP_TRANSFER_TYPE_BINARY);
		pFile->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, m_dwMaxTimeout);
		pFile->SetOption(INTERNET_OPTION_SEND_TIMEOUT, m_dwMaxTimeout);
		lpds->dwStatus	|= (pFile != NULL) ? DS_OK : DS_FAILED;
		SendMsg(0, (LPARAM)lpds);
		FTPLOG(m_dwDebugLog|m_dwRunLog, _T("OpenRemoteFile(%s), RetryIndex: %d, result: %p!"),lpszFileName, nRetry, pFile);
		return	pFile;
		
	}
	catch(CInternetException *e)
	{
		// catch errors from Wininet
		this->HandleException(lpds, e, nRetry);
		e->Delete();
	}
	RETRYEND
	return	NULL;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
UINT CFtpWrapper::Read(tagDownloadStatus* lpds, CInternetFile* pFile, void *pBuffer, DWORD dwBytesToRead)
{
	ASSERT(lpds != NULL);
	ASSERT(pFile != NULL);
	ASSERT(!::IsBadWritePtr(pBuffer, dwBytesToRead));	
	lpds->dwStatus	= OPERATION_DOWNLOAD | DOWNLOAD_DOWNLOADING;
	try
	{
		FTPLOG(m_dwDebugLog, _T("InternetReadFile..."));
		UINT	nRet	= 	pFile->Read(pBuffer, dwBytesToRead);
		lpds->dwStatus	|= (nRet != 0) ? DS_OK : DS_FAILED;
		SendMsg(0, (LPARAM)lpds);
		FTPLOG(m_dwDebugLog, _T("InternetReadFile, result: %d"), nRet);
		return	nRet;
	}
	catch(CInternetException *e)
	{
		this->HandleException(lpds, e);
		e->Delete();
	}
	return	-1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CFtpWrapper::WriteToFile(tagDownloadStatus* lpds, FILE* pFile, const CHAR *pBuffer, DWORD dwBytesToWrite)
{
	PROTECT(CFtpWrapper::WriteToFile)
	ASSERT(pFile != NULL);
	UINT	nWritten	= ::fwrite(pBuffer, sizeof(CHAR), dwBytesToWrite, pFile);
	return	nWritten == (UINT)dwBytesToWrite;
	UNPROTECT
	return	FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CFtpWrapper::DownloadImpl(tagDownloadStatus* lpds, CInternetFile* pRemoteFile, FILE* pLocalFile,DWORD dwBytesToRead)
{
	PROTECT(CFtpWrapper::DownloadImpl)
	FTPLOG(m_dwDebugLog|m_dwRunLog, _T("DownloadImpl..."));
	ASSERT(lpds != NULL);
	ASSERT(pRemoteFile != NULL);
	ASSERT(pLocalFile != NULL);
	
	lpds->dwStatus			= OPERATION_DOWNLOAD;

	UINT	nBytesRead		= 0;
	DWORD	dwTimeElapsed	= 0L;
	
	float		ftMinSpeed	= 0.f;
	float		ftMaxSpeed	= 0.f;
	float	ftTimeElapsed	= 0.f;
	float	ftAvrgSpeed		= 0.f;
	float	ftTimeElapsed0	= 0.f;
	float	ftCurSpeed		= 0.f;
	
	//	计算速度
	__int64	liStartTime;
	__int64	liFrequency;
	__int64	liCurTime;
	__int64	liStartTime0;
	
	
	const	UINT MAX_BUFFER_SIZE	= 8096;	//
	CHAR	szBuffer[MAX_BUFFER_SIZE];
	
	DWORD	dwTickStart	= ::GetTickCount();
	::QueryPerformanceFrequency((LARGE_INTEGER*)&liFrequency);
	::QueryPerformanceCounter((LARGE_INTEGER*)&liStartTime);

	lpds->dwStatus	|= DOWNLOAD_START;
	SendMsg(0, (LPARAM)lpds);
	while(dwBytesToRead > nBytesRead)
	{
		//	记录本次开始下载的时间
		::QueryPerformanceCounter((LARGE_INTEGER*)&liStartTime0);
		
		//	采用一块固定内存进行接收，以
		//	控制实际的下载速度和内部进程
		UINT	nRead	= this->Read(lpds, pRemoteFile, szBuffer, sizeof(szBuffer));
		//	下载结束
		if( nRead == 0 || nRead == -1)
		{
			FTPLOG(m_dwDebugLog, _T("DownloadImpl Stopped: %d"), nRead);
			break;
		}
		
		//	计算当前的脉冲速度
		::QueryPerformanceCounter((LARGE_INTEGER*)&liCurTime);
		ftTimeElapsed		= 1.f * ( liCurTime - liStartTime ) / liFrequency * 1000;
		ftTimeElapsed0		= 1.f * ( liCurTime - liStartTime0 ) / liFrequency * 1000;
		ftCurSpeed			= ftTimeElapsed ? (nRead * 1000 / (1024 * ftTimeElapsed0 ) ) : -1;
		dwTimeElapsed		= GetTickCount() - dwTickStart;
		
		FTPLOG(m_dwDebugLog, _T("DownloadImpl, %.2f(ms), %.2f (KB/s)"), ftTimeElapsed, ftCurSpeed);
		
		if(!ftMinSpeed)
			ftMinSpeed	= ftCurSpeed;
		if(!ftMaxSpeed)
			ftMaxSpeed	= ftCurSpeed;
		if(ftCurSpeed > ftMaxSpeed)
			ftMaxSpeed	= ftCurSpeed;
		if(ftMinSpeed > ftCurSpeed)
			ftMinSpeed	= ftCurSpeed;
		
		this->WriteToFile(lpds, pLocalFile, szBuffer, nRead);

		nBytesRead				+= nRead;
		ftAvrgSpeed				= ftTimeElapsed ? nBytesRead * 1000 / (1024 * ftTimeElapsed) : -1;

		
		lpds->dwBytesDownloaded	+= nRead;
		lpds->dwLocalFileSize	+= nRead;
		lpds->ftMinSpeed		= ftMinSpeed;
		lpds->ftMaxSpeed		= ftMaxSpeed;
		lpds->ftAvrgSpeed		= ftAvrgSpeed;
		lpds->ftTimeElapsed		= ftTimeElapsed/1000;
		lpds->ftTimeToDownload	= (dwBytesToRead-nBytesRead)/ftCurSpeed/1000;
		lpds->ftCurSpeed		= ftCurSpeed;

		SendMsg(0, (LPARAM)lpds);
		
		if(nBytesRead >= dwBytesToRead)	//	全部下载完成
		{
			lpds->dwStatus			= OPERATION_DOWNLOAD | DOWNLOAD_END | DS_OK;
			SendMsg(0, (LPARAM)lpds);
			FTPLOG(m_dwDebugLog|m_dwRunLog, _T("DownloadImpl, Status: %s"), _T("OPERATION_DOWNLOAD | DOWNLOAD_END | DS_OK"));
			return	TRUE;
		}
		
		if(this->m_dwMaxDownloadSpeed)
		{
			//	如果当前速度大于最大下载速度
			if(ftCurSpeed > m_dwMaxDownloadSpeed)
			{
				//	需要休息一段时间
				float	ftTimeNeeded	= 1.f * nRead  / m_dwMaxDownloadSpeed;
				float	ftExternalTime	= ftTimeNeeded - ftTimeElapsed;
				DWORD	dwExternalTime	= (DWORD)ftExternalTime;
				::Sleep(dwExternalTime);
			}
		}
		if(m_bHoldLowDownloadSpeed)	//	如果需要保持一定的低下载速度
		{
			::Sleep(this->m_dwDownloadRestClearance);	//稍作休息
		}
	}
	lpds->dwStatus			= OPERATION_DOWNLOAD | DOWNLOAD_END | DS_FAILED;
	SendMsg(0, (LPARAM)lpds);
	FTPLOG(m_dwDebugLog|m_dwRunLog, _T("DownloadImpl, Status: %s"), _T("OPERATION_DOWNLOAD | DOWNLOAD_END | DS_FAILED"));
	return	FALSE;
	UNPROTECT
	return	-1;		
}
const UINT DOWNLOADMSG	= ::RegisterWindowMessage(_T("{686421DD-59D5-4490-89BD-A49D0EB9B32D}"));

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CFtpWrapper::SendMsg(WPARAM wParam, LPARAM lParam)
{
	if(m_hWnd && IsWindow(m_hWnd))
		return	(BOOL)::SendMessage(m_hWnd, DOWNLOADMSG, wParam, lParam);
	return	FALSE;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CFtpWrapper::HandleException(tagDownloadStatus* lpds, CInternetException* e, int nRetry)
{
	// catch errors from Wininet
	lpds->dwStatus	|= DS_EXCEPTION;
	lpds->wParam	= (WPARAM)::GetLastError();
	lpds->lParam	= 0;
	TCHAR szErr[512]	= { 0 };
	if (e->GetErrorMessage(szErr, 512))
	{
		if( nRetry < 0 )
		{
			FTPLOG(m_dwErrorLog, _T("HandleException: %u, Err: %d, %s"), lpds->dwStatus, lpds->wParam, szErr);
		}
		else
		{
			FTPLOG(m_dwErrorLog, _T("HandleException: %u, Err: %d, %s, RetryIndex: %d"), lpds->dwStatus, lpds->wParam, szErr, nRetry);
		}
		lpds->lParam	= (LPARAM)szErr;
	}
	else
	{
		if(nRetry < 0)
		{
			FTPLOG(m_dwErrorLog, _T("HandleException: %u, Err: %d, without msg!"), lpds->dwStatus, lpds->wParam);
		}
		else
		{
			FTPLOG(m_dwErrorLog, _T("HandleException: %u, Err: %d, without msg, nRetryIndex: %d!"), lpds->dwStatus, lpds->wParam, nRetry);
		}
	}
	SendMsg(0, (LPARAM)lpds);
}

void CFtpWrapper::LogTextToDbg(LPCTSTR lpszText)
{
#ifdef NEED_OUTPUT_MSG	
	::OutputDebugString(lpszText);
	if( lpszText[_tcslen(lpszText)] != _T('\n'))
		::OutputDebugString(_T("\r\n"));
#endif
}

void CFtpWrapper::LogTextToWindow(LPCTSTR lpszText)
{
	this->SendMsg((WPARAM)1, (LPARAM)lpszText);
}

void CFtpWrapper::LogTextToFile(LPCTSTR lpszText)
{
	if( !_tcslen(m_szLogName) )
		return	;
	FILE	*pFile	= _tfopen(m_szLogName, _T("a+"));
	if(pFile != NULL)
	{
		static bool bStart = false;
		if(!bStart)
		{
			_ftprintf(pFile, _T("==============Log start=============\r\n"));
			bStart	= true;
		}
		if( lpszText[_tcslen(lpszText)] != _T('\n'))
		{
			::_ftprintf(pFile, _T("%s\r\n"), lpszText);
		}
		else
		{
			::_ftprintf(pFile, _T("%s"), lpszText);
		}
		::fclose(pFile);		
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CFtpWrapper::Log(DWORD dwStyle, LPCTSTR lpszFormat, ...)
{
	if(!this->m_bEnableLog || !this->m_pComm)
		return	;
	TCHAR	szMsg[512]		= { 0 };
	va_list	ap;
	va_start(ap, lpszFormat);
	::_vsntprintf(szMsg, 512, lpszFormat, ap);
	va_end(ap);		
	
	TCHAR	szText[512]	= { 0 };
	_sntprintf(szText, 512, _T("TID: %4d, TaskID: %3d, %s"), ::GetCurrentThreadId(), m_nTaskID, szMsg);
	bool	bToFile		= false;
	bool	bToWindow	= false;
	bool	bToDebug	= false;
	if(dwStyle & LOGSTYLE_DBG)
	{
		if((dwStyle & LOGOUTPUT_TOFILE) && !bToFile)
		{
			this->LogTextToFile(szText);
			bToFile	= true;
		}

		if(!bToDebug)
		{
			this->LogTextToDbg(szText);
			bToDebug	= true;
		}

		if((dwStyle & LOGOUTPUT_WINDOW) && !bToWindow)
		{
			this->LogTextToWindow(szText);
			bToWindow	= true;
		}
	}

	if(dwStyle & LOGSTYLE_ERR)
	{
		if((dwStyle & LOGOUTPUT_TOFILE) && !bToFile)
		{
			this->LogTextToFile(szText);
			bToFile		= true;
		}
		if((dwStyle & LOGOUTPUT_TODBG) && !bToDebug)
		{
			this->LogTextToDbg(szText);
			bToDebug	= true;
		}
		if((dwStyle & LOGOUTPUT_WINDOW) && !bToWindow)
		{
			this->LogTextToWindow(szText);
			bToWindow	= true;
		}
	}

	if(dwStyle & LOGSTYLE_RUN)
	{
		if((dwStyle & LOGOUTPUT_TOFILE) && !bToFile)
		{
			this->LogTextToFile(szText);
			bToFile		= true;
		}
		if((dwStyle & LOGOUTPUT_TODBG) && !bToDebug)
		{
			this->LogTextToDbg(szText);
			bToDebug	= true;
		}
		if((dwStyle & LOGOUTPUT_WINDOW) && !bToWindow)
		{
			this->LogTextToWindow(szText);
			bToWindow	= true;
		}
	}
}

///---	End of smallfool
