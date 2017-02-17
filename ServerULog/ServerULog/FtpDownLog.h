// FtpDownLog.h: interface for the CFtpDownLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPDOWNLOG_H__C7593685_24FB_4D24_A7AD_4B37535B25CD__INCLUDED_)
#define AFX_FTPDOWNLOG_H__C7593685_24FB_4D24_A7AD_4B37535B25CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ServerU.h"
#include <afxtempl.h>
#include <afxmt.h>

//--------------------------------------------------------------------------------------
// 常量宏定义
//--------------------------------------------------------------------------------------
#define APP_LOG_FILE					"LogFile.txt"
#define SERVER_LOG_INI					"LogFile.ini"
#define INI_SECTION_LOG					"LogFile"
#define INI_KEY_PATH					"FilePath"
#define INI_KEY_FILTER					"FilterFile"

enum DOWNFILE_STATE
{
	DOWNFILE_FAILE,
	DOWNFILE_SUCCESS,
	DOWNFILE_NOEXIT,
	DOWNFILE_CANCEL,
};


typedef struct CDownFileInfo_TYP
{
	SYSTEMTIME		m_st;					//系统当前时间
	char			m_szFileName[MAX_PATH]; //下载文件名
	char			m_szIp[16];				// 客户端IP
	float			m_fDownSpeed;			//下载速度
	DOWNFILE_STATE	m_enuDownFileState;		// 是否成功下载
} CDownFileInfo;


struct DownFileInfoAndID 
{
	CDownFileInfo m_DownFileInfo;
	DWORD  m_SessionID;	// unique ID of the FTP session
};

class CFtpDownLogDecorator;
class CFtpDownLog;

class CFtpDownLogBase  
{
public:
	CFtpDownLogBase(){};
	virtual ~CFtpDownLogBase(){};

public:
	//--------------------------------------------------------------------------------------
	// 接口
	//--------------------------------------------------------------------------------------
	virtual BOOL InitData(CString& strAppPath)=0;						
	virtual void ClearData()=0;										
	virtual void LogText(LPCSTR lpszText)=0;				

public:
	//--------------------------------------------------------------------------------------
	// FTP事件处理相关
	//--------------------------------------------------------------------------------------
	virtual BOOL StartDownFile(RFTPEventStr* pBeforEvent)=0;			
	virtual BOOL EndDownFile(RFTPEventStr* pAfterStruc) =0;			
	virtual BOOL AbortDownd(RFTPEventStr* pCloseStruc) =0;				
	virtual BOOL SaveData(CDownFileInfo &Data) =0;					
	virtual BOOL FilterFile(RFTPEventStr* pStruc) =0;	

};


class CFtpDownLog  : public CFtpDownLogBase
{
public:
	CFtpDownLog();
	~CFtpDownLog();

public:
	//--------------------------------------------------------------------------------------
	// 接口
	//--------------------------------------------------------------------------------------
	BOOL InitData(CString& strAppPath);						//初始化过滤文件数组
	void ClearData();										//清除数据
	void LogText(LPCSTR lpszText);							//写入错误日志文件

public:
	//--------------------------------------------------------------------------------------
	// FTP事件处理相关
	//--------------------------------------------------------------------------------------
	BOOL StartDownFile(RFTPEventStr* pBeforEvent);			// 开始下载事件处理
	BOOL EndDownFile(RFTPEventStr* pAfterStruc);			//成功下载结束事件处理
	BOOL AbortDownd(RFTPEventStr* pCloseStruc);				//放弃下载事件处理
	BOOL SaveData(CDownFileInfo &Data);						//保存记录
	BOOL FilterFile(RFTPEventStr* pStruc);					//过滤文件判断

private:
	CList<DownFileInfoAndID,DownFileInfoAndID&> m_List;		//Session数据链表
	CStringArray		m_FileArray;						//过滤文件数组
	CStringArray		m_strFilePathArray;
	CString				m_strPathLog;						//日志输出全路径
	CString				m_strPathFileIni;					//配置文件全路径
	CCriticalSection	m_Section;							//线程同步
};


class CFtpDownLogDecorator : public CFtpDownLogBase 
{
	friend CFtpDownLog;
public:
	CFtpDownLogDecorator(CFtpDownLogBase * pFtpDownBase):m_pFtpDownBase(pFtpDownBase){};
	virtual ~CFtpDownLogDecorator(){};

public:
	//--------------------------------------------------------------------------------------
	// 接口
	//--------------------------------------------------------------------------------------
	virtual BOOL InitData(CString& strAppPath){ return m_pFtpDownBase->InitData(strAppPath); };					
	virtual void ClearData() {  m_pFtpDownBase->ClearData(); };										
	virtual void LogText(LPCSTR lpszText){  m_pFtpDownBase->LogText( lpszText); };							

public:
	//--------------------------------------------------------------------------------------
	// FTP事件处理相关
	//--------------------------------------------------------------------------------------
	virtual BOOL StartDownFile(RFTPEventStr* pBeforEvent){ return m_pFtpDownBase->StartDownFile(pBeforEvent); };			
	virtual BOOL EndDownFile(RFTPEventStr* pAfterStruc){ return m_pFtpDownBase->EndDownFile(pAfterStruc); };		
	virtual BOOL AbortDownd(RFTPEventStr* pCloseStruc) { return m_pFtpDownBase->AbortDownd(pCloseStruc); };			
	virtual BOOL SaveData(CDownFileInfo &Data){ return m_pFtpDownBase->SaveData(Data); };					
	virtual BOOL FilterFile(RFTPEventStr* pStruc){ return m_pFtpDownBase->FilterFile( pStruc ); };	

protected:
	CFtpDownLogBase * m_pFtpDownBase;
};


#endif // !defined(AFX_FTPDOWNLOG_H__C7593685_24FB_4D24_A7AD_4B37535B25CD__INCLUDED_)
