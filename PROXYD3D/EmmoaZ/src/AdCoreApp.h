#if !defined(_ADCOREAPP_H)
#define _ADCOREAPP_H

#pragma once

#include "CoreApp.h"
#include "AppHelp.h"
#include "z_Hardware.h"
#include "IGAComm.h"
#include "Compression.h"
#include "Ad_3DFilter.h"
#include "Ad.h"
#include "Encrypt.h"


class KAdCoreApp : public KCoreApp
{
	friend IGA_Filter_IDirect3D;
public:
	KAdCoreApp();	
	virtual ~KAdCoreApp();
	
public:
	/*开始接管*/
	VOID* HookD3D   ( VOID*  pD3D);

	/*读ini配置文件*/
	BOOL  LoadConfig();

	/*读广告配置文件*/
	BOOL  LoadADXml(AD_XML_Info& adInfo);

	/*更新广告配置文件校验值*/
	void UpdateXmlCRC(AD_XML_Info& adInfo);

	/* 加载广告到设备中 */
	DWORD LoadAD2Device( VOID* pDevice);
	VOID  DeleteAdFromDevice( VOID* pDevice);

protected:
	virtual INT     OnInitThread();
	virtual INT     OnExit    ();	
	virtual INT     OnUpdateByXML      ();	
	virtual INT     OnTimeOut ();

private:
	VOID   Init();

    /* 下载，解压zip 指定的广告位*/
    BOOL   DownUpdateAD(AD_XML_RECORD&   ad);

	/*  取sdk版本号 */
	VOID   LoadSDKVersion(TCHAR*  szVersion)   ;

    /* 查询sdk */
	BOOL   QuerySDK()            ;
	
	/* 查询驱动 */
	BOOL   QueryDriver()         ;
    
	/* 查询插件 */
	BOOL   QueryPlugin()         ;

	/* 查询配置 */
	BOOL   QueryConfig()         ;

	/* 查询广告配置 */
	BOOL   QueryAdInfo()         ;

	/* 下载单条广告 */
	BOOL   DownloadOneAdZip    (LPCTSTR szIP,LPCTSTR szDir,LPCTSTR  szLocalName );

	/* 上传硬件信息 */
    BOOL   UploadHardwareInfo()  ;

	/*
	加载新的XML文件数据，并通知相关显示模块更新 
	*/
    BOOL   ApplyNewXmlEffect()  ;

	/*
	清除多余文件
	*/
	void ClearFilesByXML();

	//从xml文件中提取所有文件名
	void GetFilesFromXML(	CStringArray &fileD3DArray,
							CStringArray &fileDDSArray,
							CStringArray &fileFlashArray,
							CStringArray &fileVedioArray);
	//清除指定路径的文件
	void ClearFilesByPath(CString& strPath,CStringArray &fileArray);

	//获得游戏版本号
	void GetGameVersion(int nGameId,char* lpBuf,int nBufferLen);

public:
    /* 上传玩家信息 */
	BOOL   UploadPlayInfo()      ; 

	/* 上传关注度  */
	BOOL   UploadFocus()         ;

private:
	HINSTANCE       m_hInstance;           //此dll句柄

	//hook的3d类型
	DWORD           m_dwHookType;
	TCHAR           m_szHookType[ 20 ];

	//开始时间  f服务器端返回的开始时间
	DWORD           m_dwStartServerTime;
	DWORD           m_dwStarLocalTime;
    DWORD           m_dwRunTime;         //客户端的运行时间


	TCHAR           m_szFTPIP[ 20 ];
	IGAComm         m_net;
    IGAPath         m_paths;
    GameInfo        m_game;
	ZHardwareInfo   m_hardware;
	D3D_FILTER      m_3Dfilter;         //3d过滤器

	IP_AREA         m_IPAREA; 

	//服务器端xml文件内容
	AD_XML_Info     m_ServerxmlAD ;	
public:
	KADManger       m_adManager;

	//需要更新的纹理
	AD_XML_Info     m_NeedUpdateTexture;	

	//解压功能
	KUnzip          m_unZip; 
	//加密
	KBase64Coding   m_encrypt;

};

/*创建对象*/
KAdCoreApp*  CreateADApp();

#endif // !defined(AFX_ADCOREAPP_H__5773C6E5_84BD_4076_9ED4_9C88BC788BEF__INCLUDED_)
