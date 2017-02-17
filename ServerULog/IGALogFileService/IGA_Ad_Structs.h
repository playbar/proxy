#ifndef __IGA_AD_STRUCTS_H__
#define	__IGA_AD_STRUCTS_H__


#include "consts.h"

#pragma pack(push, 1)

/************************************************************************/
/* 媒介信息                                                             */
/************************************************************************/
typedef struct tagMediaID
{
	union
	{
		DWORD	dwMediaID;
		struct 
		{
			WORD	w[2];
		};
		struct 
		{
			BYTE	b[4];
		};
		struct 
		{
			WORD	wReserved;
			BYTE	byScene;
			BYTE	bySample;
		};
	};
}MEDIAID, *LPMEDIAID;

/************************************************************************/
/* 广告表                                                               */
/************************************************************************/

typedef struct tagAdId
{
	DWORD	dwID;									// PK, INT, NOT NULL
	DWORD	dwVersion;
	CHAR	szAdPosition[MAX_ADPOSITION];			// NVARCHAR
	CHAR	szAdMedium[MAX_ADMEDIUM];
	CHAR	szAdIntroduction[MAX_ADINTRODUCTION];
	double	dbPrice;								// Decimal(18,2)
	INT		nCurrencyType;							//	
	INT		nAdPayMode;								
	INT		bDeleted;
}ADID,*LPADID;


/************************************************************************/
/* 通过网络传输给客户端的广告位信息                                     */
/************************************************************************/
typedef struct tagAdInfoInComm
{
	DWORD	dwID;							//	广告ID号
	BYTE	byEnabled;						//	广告是否关闭还是打开
	BYTE	byType;							//	媒介类型	MEDIUMTYPE_DDS
	union
	{
		struct  {
			BYTE	byOrigFileNameLength;			//	随后而来的文件内容类型
			BYTE	byAdFileNameLength;				//	随后而来的文件名称长度
			DWORD	dwOriginalLevel;				//	源文件的层次
			DWORD	dwAdLevel;						//	广告文件的层次
			DWORD	dwOriginalFileVersion;			//	源文件的版本
			DWORD	dwAdFileVersion;				//	广告文件的版本号。
		};
		struct  {
			BYTE	byFileNameLength0;
			BYTE	byFileNameLength1;
			DWORD	dwLevel0;
			DWORD	dwLevel1;
			DWORD	dwVersion0;
			DWORD	dwVersion1;
		};
	};
	//	接下来的是文件名称，
	//CHAR	szOriginalFileName[byOrigFileNameLength];
	//CHAR	szAdFileName[byAdFileNameLength];
	
}ADINFOINCOMM, *LPADINFOINCOMM;

/************************************************************************/
/* 客户端保留在本地的广告位信息结构                                     */
/************************************************************************/
typedef struct tagClientAdInfo
{
	DWORD	dwID;							//	广告ID号
	DWORD	dwType;							//	广告类型或媒介类型等
	union
	{
		struct  {
			DWORD	dwOriginalLevel;				//	源文件的层次
			DWORD	dwAdLevel;						//	广告文件的层次
			DWORD	dwOriginalFileVersion;			//	源文件的版本
			DWORD	dwAdFileVersion;				//	广告版本号
			
			CHAR	szOriginalFileName[_MAX_PATH];	//	源文件名称
			CHAR	szAdFileName[_MAX_PATH];		//	广告资源文件名称
		};
		
		struct  {
			DWORD	dwLevel0;						//	源文件的层次
			DWORD	dwLevel1;						//	广告文件的层次
			DWORD	dwVersion0;						//	源文件的版本
			DWORD	dwVersion1;						//	广告版本号
			CHAR	szFileName0[_MAX_PATH];			//	源文件名称
			CHAR	szFileName1[_MAX_PATH];			//	广告资源文件名称
		};
	};

	DWORD	dwReserved[4];

}CLIENTADINFO, *LPCLIENTADINFO;


/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef struct tagGameID
{
	DWORD	dwGameID;
	CHAR	szGameName[MAX_GAMENAME];
	DWORD	dwVersion;
	CHAR	szVersion[MAX_GAMEVER];
	CHAR	szPublisher[MAX_PUBLISHER];
	CHAR	szDeveloper[MAX_DEVELOPER];
	DWORD	dwGraphicsPlatform;				//GP_DX9;	GP_GX8

}GAMEID, *LPGAMEID;

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef struct tagHardwareID
{
	CHAR	szMac[MAX_MAC];
	CHAR	szCpu[MAX_CPU];
	CHAR	szDisplay[MAX_DISPLAY];
	DWORD	dwOperatingSystem;
	DWORD	dwMemorySize;
	DWORD	dwGraphicsPlatform;				//GP_DX9;	GP_GX8
}HARDWAREID, *LPHARDWAREID;



/************************************************************************/
/* 广告关注度数据                                                       */
/************************************************************************/
typedef struct tagFocus
{
	enum
	{
		MAX_FOCUS	= 4,
	};
	DWORD	dwAdID;						//	广告ID号
	DWORD	dwDownloadCount;			//	下载次数
	DWORD	dwLoadCount;				//	广告资源被加载的次数
	DWORD	dwClickCount;				//	广告被点击的次数
	struct {
		union
		{
			DWORD	dwNavigateCount;	//	广告浏览或被播放的次数
			DWORD	dwPlayCount;	
		};
		DWORD	dwFullPlayCount;		//	广告被完整播放的次数
		DWORD	dwPlayTime;				//	广告被播放的时长。
	}	pi[MAX_FOCUS];
	
}FOCUS, *LPFOCUS;

/************************************************************************/
/* 客户端连接记录信息                                                   */
/************************************************************************/
typedef struct tagClientPlayInfo
{
	DWORD	dwPlayerID;
	DWORD	dwGameID;
	DWORD	dwLocationID;
	DWORD	dwHighVersion;
	DWORD	dwLowVersion;
	DWORD	dwTime;
	WORD	wConnectTimes;
	WORD	wConnectedTimes;
	DWORD	dwPlayCount;
	struct  {
		DWORD	dwPlayTick;
		DWORD	dwPlayTime;
	}	pi[0];
}CLIENTPLAYINFO, *LPCLIENTPLAYINFO;

typedef	struct tagPlayInfo
{
	DWORD	dwPlayTime;
	DWORD	dwCloseTime;
}PLAYINFO, *LPPLAYINFO;

#pragma pack(pop)



#endif	///__IGA_AD_STRUCTS_H__