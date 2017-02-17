//************************************************************************************************************
// class      :  
// purpose    :  IGA 通讯类
// Date       :  2008-6-5  13:22:36
// author     :  3o3afe1
//************************************************************************************************************
#if !defined(AFX_IGACOMM_H__CF866030_5B0C_497E_A810_F52695D97ECF__INCLUDED_)
#define AFX_IGACOMM_H__CF866030_5B0C_497E_A810_F52695D97ECF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef IGANET_DLL
    #pragma  message (" export iganet dll. 3o3afe1")
    #define IGASDK_API   __declspec(dllexport)
#else
    #pragma  message (" import iganet dll. 3o3afe1")
    #define IGASDK_API   __declspec(dllimport)
#endif



/*具体通讯类*/
class  IGASDK_API IGAComm  
{
public:
	//	smallfool
	BOOL FTPDownload(HWND hWnd, LPCTSTR lpszURL, LPCTSTR lpszTargetName, BOOL bBackendThread = FALSE, BOOL bWaitResult = FALSE);
	BOOL FTPDownload(HWND hWnd, LPCTSTR lpszIP,UINT nPort,LPCTSTR lpszObject, LPCTSTR lpszTargetName, LPCTSTR lpszUserName = NULL, LPCTSTR lpszPassword = NULL, BOOL bBackendThread = FALSE, BOOL bWaitResult = FALSE );
	BOOL FTPAutoDownload(HWND hWnd, LPCTSTR lpszTargetPath);
	//	End of smallfool

	IGAComm(LPCTSTR szURL) ;
	~IGAComm();	
	
	
	BOOL    SetTopURL(LPCTSTR  lpszURL);	
    VOID    SetProtocol(BYTE  bPro);

	//查询sdk升级包
	BOOL    QuerySDK(   
		                DWORD   dwGameID, 
						LPCTSTR lpszGameVer, 
						LPCTSTR lpszSDK,
						DWORD dwType, 
						LPCTSTR lpszMacID, 
						DWORD dwChnID,
						LPCTSTR lpszTmpName,
						LPVOID  lpReserve = NULL
					);

	//查询配置文件
	BOOL    QueryConfig(
		                DWORD   dwGameID, 
						LPCTSTR lpszVersion, 
						DWORD dwSample, 
						LPCTSTR lpszMacID , 
						DWORD dwChnID,
						LPCTSTR lpszTmpName,
						LPVOID  lpReserve = NULL
					   );

	//上传硬件信息
	BOOL    UploadHardwareInfo(
		                DWORD dwGameID, 
						LPCTSTR lpszMacID, 
						DWORD dwMemorySize, 
						LPCTSTR lpszCPU, 
						LPCTSTR lpszDisplay,
						LPCTSTR dwOs,
						LPCTSTR dwGP, 
						DWORD dwChnID,
						LPCTSTR lpszRemoteIP,
						LPCTSTR lpszTmpName,
						LPVOID  lpReserve = NULL
						     );

	//查询广告信息文件
	BOOL    QueryAdInfo(
		                DWORD dwGameID, 
						DWORD dwAreaID, 
						DWORD dwSample, 
						LPCTSTR lpszMacID, 
						DWORD dwChnID,
						LPCTSTR lpszTmpName,
						LPVOID  lpReserve = NULL
						);

	//从ftp服务器下载xml文件
	BOOL    QueryAdInfoFromFtp(
						DWORD dwGameID, 
						DWORD dwAreaID, 
						DWORD dwSample, 
						LPCTSTR lpszMacID, 
						DWORD dwChnID,
						LPCTSTR lpszTmpName,
						LPVOID  lpReserve = NULL
						);

	//查询广告
	BOOL    QueryAd(
		                DWORD dwGameID, 
						DWORD dwSampleID, 
						DWORD dwAreaID, 
						DWORD dwAdID, 
						DWORD dwType, 
						LPCTSTR lpszFileName, 
						LPCTSTR lpszMacID, 
						DWORD dwChnID,
						LPCTSTR lpszTmpName,
						LPVOID  lpReserve = NULL
					);
	

	/*上传玩家信息*/
	BOOL    UploadPlayInfo(
		                DWORD dwGameID, 
						DWORD dwPlayTime, 
						DWORD dwCloseTime, 
						LPCTSTR lpszMacID, 
						DWORD dwAreaID, 
						DWORD dwChnID,
						LPCTSTR lpszTmpName,
						LPVOID  lpReserve = NULL
						  );

	/*上传关注度信息*/
	BOOL    UploadData(
		               char*   pData,
					   DWORD   dwLen,
					   LPCTSTR lpszTmpName,
					   LPVOID  lpReserve = NULL
					  );


	BOOL    CloseGame(DWORD dwGameID, DWORD dwPlayTime, LPCTSTR lpszMacID, DWORD dwAreaID, DWORD dwChnID,LPCTSTR lpszTmpName,LPVOID  lpReserve = NULL);
	BOOL    MakeBackup( LPCTSTR lpszName, LPCTSTR lpszPwd,LPCTSTR lpszTmpName,LPVOID  lpReserve = NULL);
	BOOL    StartServer(LPCTSTR lpszName, LPCTSTR lpszPwd,LPCTSTR lpszTmpName,LPVOID  lpReserve = NULL);

	//ftp下载文件
	BOOL    QueryFileFromFtp(
		LPCTSTR lpszIP,
		LPCTSTR lpszFtpDir,
		LPCTSTR lpszTmpName,
		LPCTSTR lpszUserName = NULL ,
		LPCTSTR lpszPSW   = NULL,
		LPVOID  lpReserve = NULL
		
		);

	//	rizhi
	//	smallfool
private:
	static DWORD WINAPI DownloadThreadProc(void *p);
public:
	static bool IsExisted(LPCTSTR lpszFileName);
	static BOOL CreatePath(LPCTSTR lpszPathName, bool bRecursive = true);
	//	End of smallfool
private:	
	/*URL*/
	TCHAR             m_szURL[_MAX_PATH];	
	BYTE              m_Protocol;
	BOOL			m_bEnableLog;

};



#endif // !defined(AFX_IGACOMM_H__CF866030_5B0C_497E_A810_F52695D97ECF__INCLUDED_)
