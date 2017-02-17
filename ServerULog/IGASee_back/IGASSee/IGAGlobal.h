#pragma once

//class IGAGlobal
//{
//public:
//	IGAGlobal(void);
//	~IGAGlobal(void);
//};

#	if	defined(ZLIB) || defined(USING_ZLIB)
#	pragma message("*** Auto linking ZLIB 0.9...")
#	include "./../lib/include/zlib.h"
#		if		defined (DEBUG) || defined(_DEBUG) || defined(DBG)
#			pragma comment(lib, "./../lib/Debug/zlib.lib")
#		else
#			pragma comment(lib, "./../lib/release/zlib.lib")
#		endif
#	endif


#define PLAYINFO_EXT	_T(".PlayInfo")
#define FOCUSINFO_EXT   _T(".FocusInfo")
#define INI_EXT			_T(".ini")
#define LOG_EXT         _T(".LOG")

#define	IDS_SECTION_SDK				_T("SDK")
#define	IDS_SECTION_HARDWAREINFO	_T("HARDWAREINFO")

#define MAX_MAC			18
#define MAX_PATH_KEY    5


#define ABRK	_T('/')
#define BRK		_T('\\')

// 使用紧凑字节
#pragma pack(push, 1)

typedef	struct tagClientPlayInfo
{
	DWORD	dwGameID;
	DWORD	dwPlayTime;
	DWORD	dwCloseTime;
	DWORD	dwChnID;
	DWORD	dwAreadID;
	TCHAR	szMacID[20];
	TCHAR	szAddress[20];
}CLIENTPLAYINFO;

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

#pragma pack(pop)


typedef struct tagListData
{
	WPARAM wParam;
	LPARAM lParam;
}LISTDATA, *LPLISTDATA;


static	const CHAR *g_szGP[]	=
{
	_T("NONE"),
		_T("DirectX 9.0"),
		_T("DirectX 8.0"),
		_T("DirectDraw"),
		_T("OpenGL"),
		_T("GDI"),
		_T("Unknown"),
};


// 公共函数
CString AUTOAPPENDBRK(CString &str);

CString GetConfigFileName();
BOOL CreateConfigFile();

LPCTSTR GetToken(LPCTSTR psz, int n);
