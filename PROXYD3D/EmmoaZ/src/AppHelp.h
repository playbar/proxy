#ifndef _APPHELP1_H
#define _APPHELP1_H

#pragma once

/*游戏相关信息*/
struct GameInfo
{
	DWORD					m_dwAreaID;                     //  区域
	TCHAR				    m_GameName    [ 128 ];				//	游戏名称
	TCHAR				    m_GameVersion [ 128 ];				//	游戏版本
	DWORD					m_dwGameID;						//	游戏ID号
	DWORD					m_dwGameVer;					//	游戏版本信息	
	DWORD					m_dwChannelID;					//	渠道或网吧编号
};

/* 路径相关*/
class IGAPath
{
public:
	IGAPath ();
	~IGAPath();

public:
	//	取工作路径
	static BOOL	GetWorkingPath( LPTSTR lpszPath,DWORD   dwLen ,HANDLE hModel = NULL);

public:
	BOOL GetPluginsPath(LPTSTR pszPathName,DWORD   dwLen);
	BOOL GetCachePath(LPTSTR pszPathName,DWORD   dwLen);
	BOOL GetUnzippedPath(LPTSTR pszPathName,DWORD   dwLen);
	BOOL GetDownloadPath(LPTSTR pszPathName,DWORD   dwLen);
	BOOL GetDefaultTargetPath(LPTSTR pszPathName,DWORD   dwLen);
	BOOL GetDefaultTempPath(LPTSTR pszPathName,DWORD   dwLen);
	
    /*取系统配置文件名称*/
	VOID GetSysCfgName(LPTSTR pszName,DWORD   dwLen);

	/*取广告配置文件*/
	VOID GetAdCfgName(LPTSTR pszName,DWORD   dwLen);

	/* 取匹配文件路径*/
	VOID GetMatchFilePath(LPTSTR pszName,DWORD   dwLen);

	/* 取广告文件路径*/
	VOID GetADFilePath(LPTSTR pszName,DWORD   dwLen);

	/* 取flash文件路径*/
	VOID GetFlashPath(LPTSTR pszName,DWORD   dwLen);
	VOID GetFlashCfgName(LPTSTR pszName,DWORD   dwLen);

	/* 取video文件路径*/
	VOID GetVideoPath(LPTSTR pszName,DWORD   dwLen);
	VOID GetVideoCfgName(LPTSTR pszName,DWORD   dwLen);

	/* 取active flash文件路径*/
	VOID GetActiveFlashPath(LPTSTR pszName,DWORD   dwLen);
	VOID GetActiveFlashCfgName(LPTSTR pszName,DWORD   dwLen);

	/* 取3d文件路径*/
	VOID Get3DPath(LPTSTR pszName,DWORD   dwLen);
	VOID Get3DCfgName(LPTSTR pszName,DWORD   dwLen);

	//取安全配置文件路径
	VOID GetSecurityFilePath(LPTSTR pszName,DWORD   dwLen);

	/*取SDK文件路径*/
	VOID GetSDKZIPFileName(LPTSTR pszName,DWORD   dwLen);
private:	
	TCHAR	m_WorkingPath[_MAX_PATH +1 ];					//	DLL的工作路径，目录为游戏可执行文件所在的目录
};

class  IP_AREA
{
public:
	IP_AREA();
	~IP_AREA();

    DWORD   GetAreaID(LPCTSTR  lpszIP);

private:
    HINSTANCE  m_hAreaDll ;
};

#endif
