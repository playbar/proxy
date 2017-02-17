#ifndef __IGA_STRUCTS_H__
#define	__IGA_STRUCTS_H__

#include "IGA_Ad_Structs.h"

#define  MAX_MAC 18 
#define	 ID_TIMER1 1
#	pragma message("*** Auto linking ATL.DLL...")
#	include <atlbase.h>
#	define	MAP_BASE	CSimpleMap		
#	define	ARRAY_BASE	CSimpleArray	
#	define	USING_ATL



//#	if	defined(ZLIB) || defined(USING_ZLIB)
//#	pragma message("*** Auto linking ZLIB 0.9...")
//#	include "./include/zlib.h"
//#		if		defined (DEBUG) || defined(_DEBUG) || defined(DBG)
//#			pragma comment(lib, "./lib/zlib.lib")
//#		else
//#			pragma comment(lib, "./lib/zlib.lib")
//#		endif
//#	endif



#define PLAYINFO_EXT	".PlayInfo"
#define FOCUSINFO_EXT   ".FocusInfo"
#define INI_EXT			".ini"
#define LOG_EXT         ".LOG"

#pragma pack(push, 1)

typedef	struct tagClientGameInfo
{
	DWORD	dwGameID;
	TCHAR	szGameVersion[20];
}GameInfo;
typedef	struct tagClientConnectionInfo
{
	DWORD	dwGameID;
	DWORD	dwPlayTime;
	DWORD	dwCloseTime;
	DWORD	dwChnID;
	DWORD	dwAreadID;
	TCHAR	szMacID[20];
	TCHAR	szAddress[20];
}PlayInfo;
typedef	struct tagClientHardInfoInfo
{
	TCHAR	szUpdataTime[20];
	TCHAR	szMacID[40];
	DWORD	dwMemorySize;
	DWORD   dwGameID;
	TCHAR	szOperationSystem[40];
	TCHAR	szAddress[40];
	TCHAR	szCPU[56];
	TCHAR	szDisplay[40];
	TCHAR	szType[40];
	DWORD	dwChnID;
	TCHAR	szLocalAddress[40];
}HardwareInfo;
typedef	struct tagClientUpdateInfo
{
	TCHAR	szUpDateTime[20];
	TCHAR	szGameName[20];
	TCHAR	szGameVersion[20];
	TCHAR	szSDKName[20];
	TCHAR	szAddress[20];
	TCHAR   szMacID[32];
	DWORD	dwType;
	DWORD	dwChnID;
}UpdateInfo;
typedef struct tagFocusInfo 
{
	enum
	{
		MAX_FOCUS	= 4,
	};
	struct {
		union
		{
			DWORD	dwNavigateCount;	//	广告浏览或被播放的次数
			DWORD	dwPlayCount;	
		};
		DWORD	dwFullPlayCount;		//	广告被完整播放的次数
		DWORD	dwPlayTime;				//	广告被播放的时长。
	}	pi[MAX_FOCUS];
	DWORD	dwGameID;
	DWORD	dwAreaID;
	DWORD	dwAddress;
	DWORD	dwAdID;						
	DWORD	dwDownloadCount;			
	DWORD	dwLoadCount;				
	DWORD	dwClickCount;	
	CHAR	szMacID[MAX_MAC];
}FocusInfo;
typedef struct tagRequestHeader 
{
	BYTE	byRequestType;		//	请求头类型
	BYTE	byFlags;			//	子包包头类型
	WORD	wRequestHeader;		//	请求头部长度，包括子包头长度
}REQUESTHEADER, *LPREQUESTHEADER;

	
template<typename HEADINFO>
struct FOCUSREQUEST_T : public HEADINFO
{
	DWORD	dwGameID;
	union
	{
		struct  {
			DWORD	dwReserved[2];
		};
		struct  {
			DWORD	dwAreaID;
			DWORD	dwAddress;
			
		};
	};
	DWORD	dwAdCount;
	CHAR	szMacID[MAX_MAC];
};


typedef struct tagPacketHeader 
{
	DWORD	dwLength;		// General packet length including this packet header
#ifdef SERVER_TIME
	DWORD	dwSrvTime;		//	the tick of the services
#endif
	BYTE	byType;			// Packet type, PACKETTYPE_XXXX
	BYTE	byFlags;		// Packet flags. such as crypt or compressed
	BYTE	byCount;		// Sub packet count
	BYTE	byFrom;			// Reserved;
}PACKETHEADER, *LPPACKETHEADER;

typedef ARRAY_BASE<PlayInfo*>	PlayInfos;
typedef ARRAY_BASE<HardwareInfo*>	HardwareInfos;
typedef ARRAY_BASE<UpdateInfo*>	UpdateInfos;
typedef ARRAY_BASE<GameInfo*> GameInfos;
typedef ARRAY_BASE<FocusInfo*> FocusInfos;
#pragma pack(pop)

#endif	///__IGA_AD_STRUCTS_H__