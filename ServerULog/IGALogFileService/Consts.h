#ifndef __CONSTS_H__
#define	__CONSTS_H__


#	define	IGA_SDK_VERSION				0x00000001
#	define	FULL_COMMPANY_NAME			_T("Game Media Kingdom Co., Ltd,")
#	define	DEFAULT_COMMPANY_NAME		_T("InterAdtive")
#	define	COMMPANY_NICKNAME			_T("IA")

#	define	AUTHOR						_T("Smallfool")
#	define	FIRSTAUTHOR					_T("")
#	define	SECONDAUTHOR				_T("")
#	define	THIRDAUTHOR					_T("")


#	define	PROJECTNAME					_T("In-game Advertising")
#	define	PROJECT						_T("EMMOA")
#	define	PROJECT0					"EMMOA"

#	define	DRIVERNAME					_T("EMMOA")

#	define	CLIENTS						_T("Clients")
#	define	SHELL						_T("Shell")
#	define	CORE						_T("Core")
#	define	NETWORK						_T("Network")
#	define	COMMON						_T("Common")
#	define	SERVER						_T("Server")
#	define	BACKEND						_T("BackEnd")
#	define	TOOLS						_T("Tools")

#	define	AD							_T("Ad")
#	define	ADVERTISER					_T("Advertiser")
#	define	GAME						_T("Game")
#	define	PUBLISHER					_T("Publisher")

#	define	HITS						_T("Hits")
#	define	PAGES						_T("Pages")
#	define	VISITORS					_T("Visitors")
#	define	USER						_T("User")

#	define	ATTENTION					_T("Attention")
#	define	INTEREST					_T("Interest")
#	define	DESIRE						_T("Desire")
#	define	ACTION						_T("Action")

#	define	BACKUPFILEEXTNAME			_T(".wbk")
#	define	WY_COMPRESSED_FILE_EXT		_T(".wyx")
#	define	ZIPPED_FILE_EXT				_T(".zip")
#	define	EXEEXTNAME					_T(".exe")
#	define	DLLEXTNAME					_T(".dll")
#	define	OCXEXTNAME					_T(".OCX")
#	define	SYSEXTNAME					_T(".SYS")
#	define	LOGEXTNAME					_T(".LOG")
#	define	INIEXTNAME					_T(".INI")
#	define	XMLEXTNAME					_T(".XML")
#	define	DDSEXTNAME					_T(".DDS")
#	define	IGAD3DDLLEXT				_T(".WDX")
#	define	DATAFILEEXT					_T(".DAT")

#	define	IGA_SHELL_NAME				PROJECT0 "_Shell9"
#	define	IGA_SHELL8_NAME				PROJECT0 "_Shell8"
#	define	IGA_CORE_NAME				PROJECT0 "_Core"

#	define	IGA_SHELL_DLL_NAME			IGA_SHELL_NAME ".dll"
#	define	IGA_SHELL8_DLL_NAME			IGA_SHELL8_NAME ".dll"
#	define	IGA_CORE_DLL_NAME			IGA_CORE_NAME ".dll"
#	define	IGA_CORE_EXPORT_FUNC(x)		IGA_CORE_NAME "_" #x

/************************************************************************/
/* 发布是使用驱动进行DLL加载时必须进行更名处理                          */
/*	IAF = InterAdtive File												*/
/************************************************************************/

#	if defined IGA_RELEASE_USING_DRIVER
#		define	IAFILEEXT					".IAF"
#		define	IGA_SHELL_RETAIL_NAME		"EMMOAR.IAF"
#		define	IGA_SHELL8_RETAIL_NAME		"EMMOAS.IAF"
#		define	IGA_CORE_RETAIL_NAME		"EMMOAZ.IAF"
#	endif	//IGA_RELEASE 


#	define	IGA_UNIMPLEMENT(s)			#s

#	define	COMMPANY_HOMEPAGE			"http://www.gmk.com.cn/"
#	define	PROJECT_HOMEPAGE0			"http://www.emmoa.com/"
#	define	PROJECT_HOMEPAGE1			"http://www.emmoa.net/"
#	define	PROJECT_HOMEPAGE2			"http://www.emmoa.cn/"
#	define	PROJECT_HOMEPAGE3			"http://www.emmoa.com.cn/"

#	define	CMD_QUERYSDK				"EMMOA_IIS.dll?QuerySDK&Game=%s&Version=%s&SDK=%s&GP=%d&MacID=%s&Chn=%d"
#	define	PARAM_QUERYSDK				"Game Version SDK GP MacID Chn"
#	define	CMD_QUERYCONFIG				"EMMOA_IIS.dll?QueryConfig&Game=%s&Version=%s&Sample=%d&MacID=%s&Chn=%d"
#	define	PARAM_QUERYCONFIG			"Game Version Sample MacID Chn"
#	define	CMD_QUERYADINFO				"EMMOA_IIS.dll?QueryAdInfo&Game=%d&Area=%d&Sample=%d&MacID=%s&Chn=%d"
#	define	PARAM_QUERYADINFO			"Game Area Sample MacID Chn"
//	支持采样数值
#	define	CMD_QUERYAD					"EMMOA_IIS.dll?QueryAd&Game=%d&Sample=%d&Area=%d&&Ad=%d&Type=%d&FName=%s&MacID=%s&Chn=%d"
#	define	PARAM_QUERYAD				"Game Sample Area Ad Type FName MacID Chn"
#	define	CMD_QUERYSTART				CMD_QUERYSDK
#	define	CMD_QUERYSTART1				CMD_QUERYSDK1
#	define	CMD_QUERYEND				"EMMOA_IIS.dll?CloseGame&Game=%d&PlayTime=%d&MacID=%s&Area=%d&Chn=%d"
#	define	PARAM_QUERYEND				"Game PlayTime MacID Area Chn"
#	define	CMD_UPLOADPLAYINFO			"EMMOA_IIS.dll?UploadPlayInfo&Game=%d&PlayTime=%d&CloseTime=%d&MacID=%s&Area=%d&Chn=%d"
#	define	PARAM_UPLOADPLAYINFO		"Game PlayTime CloseTime MacID Area Chn"
#	define	CMD_UPLOADHARDWAREINFO		"EMMOA_IIS.dll?UploadHardwareInfo&Game=%d&Mac=%s&MS=%d&CPU=%s&Display=%s&OS=%d&GP=%d&Chn=%d&IP=%s"	
#	define	PARAM_UPLOADHARDWAREINFO	"Game Mac MS CPU Display OS GP Chn IP"
#	define	CMD_UPLOADDATA				"EMMOA_IIS.dll?UploadData"
#	define	PARAM_UPLOADDATA			""
#   define  CMD_MAKEBACKUP				"EMMOA_IIS.dll?MakeBackup&Name=%s&Pwd=%s"
#	define  PARAM_MAKEBACKUP			"Name Pwd"
#   define  CMD_STARTSERVER				"EMMOA_IIS.dll?StartServer&Name=%s&Pwd=%s"
#	define  PARAM_STARTSERVER			"Name Pwd"

#	define	IDS_MAP						_T("Map")
#	define	IDS_SCENE					_T("Scene")
#	define	IDS_SAMPLE					_T("Sample")


#ifdef _DEBUG
#	define	DEFSAMPLEFILENAME			"Sample.zip"
#	define	PARAM_SAMPLEFILENAME		"Sample_%03d.zip"
#	define	FOLDER_SMAPLES				"Samples"
#	define	PARAM_FOLDER_SAMPLES		"Samples_%03d"
#	define	FOLDER_ADRES				"AdFiles"
#	define	FOLDER_ORIGINALFILE			"OriginalFiles"
#	define	XML_ADINFO					"\\" DEFAULT_COMMPANY_NAME "\\AdInfo.XML"
#	define	FOLDER_SDK					"SDKUpdate"
#else
#	define	DEFSAMPLEFILENAME			"3A8843A7-3604-46c5-BDFF-25E8DFC083FC.PKG"
#	define	PARAM_SAMPLEFILENAME		"3A8843A7-3604-46c5-BDFF-25E8DFC083FC_%03d.PKG"
#	define	FOLDER_SMAPLES				"9A7CE7F9-514F-41ad-9DFB-185F2597BFFB"
#	define	PARAM_FOLDER_SAMPLES		"9A7CE7F9-514F-41ad-9DFB-185F2597BFFB_%03d"
#	define	FOLDER_ADRES				"C1713DA9-0B9A-43ee-959C-77C9C0CE3653"
#	define	FOLDER_ORIGINALFILE			"93A67786-06BF-4af3-9796-45FF2DBD0373"
#	define	XML_ADINFO					"\\" DEFAULT_COMMPANY_NAME "\\{7A53B594-673B-40f0-B3B8-765DBB7EEF24}.XML"
#	define	FOLDER_SDK					"6C84A125-CA92-4bc8-B1CF-CE35EE52AF8D"
#endif

#	define	FOLDER_PLUGINS				_T("Plugins")


#	define	SECOND						1000
#	define	MINUTE						(60 * SECOND)
#	define	HOUR						(60 * MINUTE)

//#define MINCHAR     0x80        
//#define MAXCHAR     0x7f        
//#define MINSHORT    0x8000      
//#define MAXSHORT    0x7fff      
//#define MINLONG     0x80000000  
//#define MAXLONG     0x7fffffff  
//#define MAXBYTE     0xff        
//#define MAXWORD     0xffff      
//#define MAXDWORD    0xffffffff  
/************************************************************************/
/* 日志风格                                                             */
/************************************************************************/
#define	LOGSTYLE_NONE		0	//	无需进行日志
#define	LOGSTYLE_DBG		1	//	只输出调试的日志信息
#define	LOGSTYLE_ERR		2	//	只输出错误信息
#define	LOGSTYLE_WARNING	3	//	只输出警告信息
#define	LOGSTYLE_RUNTIME	4	//	只输出运行的日志信息

#define	LOGSTYLE_NETWORK	0x1000	//	网络层的日志
#define	LOGSTYLE_RENDER		0x2000	//	渲染层的日志
//#define	LOGSTYLE_PLUGINS	0x4000	//	来自插件的日志

#define	LOGSTYLE_ALL		7		//	输出所有的日志信息


/************************************************************************/
/* Attentions,请保持 AdCore的导出函数符号下列的顺序名称                 */
/************************************************************************/
#	if defined	(USING_D3D9)	|| defined(USING_D3D9DLL)
#	define	IGA_AdCore_Export_Direct3DCreate9					1
#	define	IGA_AdCore_Export_DebugSetMute						2
#	define	IGA_AdCore_Export_DebugSetLevel						3
#	define	IGA_AdCore_Export_D3DPERF_BeginEvent				4
#	define	IGA_AdCore_Export_D3DPERF_EndEvent					5
#	define	IGA_AdCore_Export_D3DPERF_SetMarker					6
#	define	IGA_AdCore_Export_D3DPERF_SetRegion					7
#	define	IGA_AdCore_Export_D3DPERF_QueryRepeatFrame			8
#	define	IGA_AdCore_Export_D3DPERF_SetOptions				9
#	define	IGA_AdCore_Export_D3DPERF_GetStatus					10
#	define	IGA_AdCore_Export_Direct3DShaderValidatorCreate9	11
#	define	IGA_AdCore_Export_CheckFullscreen					12
#	define	IGA_AdCore_Export_PSGPError							13
#	define	IGA_AdCore_Export_PSGPSampleTexture					14
#	endif

#	if defined	(USING_D3D8)	|| defined(USING_D3D8DLL)
#	define	IGA_AdCore_Export_Direct3DCreate8					1
#	define	IGA_AdCore_Export_DebugSetMute						2
#	define	IGA_AdCore_Export_ValidatePixelShader				3
#	define	IGA_AdCore_Export_ValidateVertexShader				4
#	define	IGA_AdCore_Export_CheckFullscreen					5
#	endif


#	define	UNZIPDLLNAME			PROJECT0"_Unzip.DLL"
#	define	ZIPDLLNAME				PROJECT0"_Zip.dll"
#	if defined IGA_RELEASE_USING_DRIVER
#		define	UNZIP32DLLNAME			PROJECT0"U"IAFILEEXT
#	else
#		define	UNZIP32DLLNAME			PROJECT0"_Unzip32.DLL"
#endif	



#	define	STRNONE					_T("NONE")
#	define	STREMPTY				_T("Empty")

#	define	ERROR_TIPS				_T("***Error***")
#	define	FATAL_TIPS				_T("!!!Fatal!!!")
#	define	NORMAL_TIPS				_T("===Tips===")

#	define	ERR_TIPS(s)				_T(#s) _T(" Failed for error: ") 
#	define	FUNC_BEGIN(s)			_T(#s) _T(" Entering ...")
#	define	FUNC_END(s)				_T(#s) _T(" Left!")
#	define	FUNC_ENDBY(s)			_T(#s) _T(" Left with Result: ")
#	define	BRK						_T('\\')	//	斜杠字符
#	define	ABRK					_T('/')		//	反斜杠字符
#	define	ZERO					_T('\0')	//	'0'字符
#	define	DOT						_T('.')		//	点字符
#	define	EMPTY					_T(' ')		//	空格字符
#	define	ENDL					_T("\n")
#	define	ALLFILES				_T("*.*")
#	define	UNDERLINE				_T("_")

#	define	DIREND					_T("\\")
#	define	DIREND2					_T("/")

#	define	IGA_LINE				inline
#	define	ZIPHEAD					"PK"


# if !defined MAX_INT
#	define	MAX_INT	0x7FFFFFFF
# endif 

#if !defined (INVALID_FILE_ATTRIBUTES) 
#	define INVALID_FILE_ATTRIBUTES ((DWORD)-1) 
#endif

#if !defined (BIF_NEWDIALOGSTYLE)
#define	BIF_NEWDIALOGSTYLE 0x0040
#endif

#if !defined (BIF_USENEWUI)
#define	BIF_USENEWUI	0x0050
#endif



/************************************************************************/
/*                                                                      */
/************************************************************************/
#define	MAX_GAMENAME		64
#define	MAX_ADNAME			64
#define	MAX_ADTYPE			64
#define	MAX_ADPOSITION		64
#define	MAX_ADMEDIUM		64
#define	MAX_ADINTRODUCTION	100
#define	MAX_ADRESOURCE		64

#define MAX_IP              16
#define	MAX_MAC				18
#define	MAX_CPU				64
#define	MAX_DISPLAY			64
#define	MAX_OS				128

#define	MAX_PUBLISHER		128
#define	MAX_DEVELOPER		128
#define	MAX_GAMEVER			64

#define	MAX_AREANAME		32

/************************************************************************/
/*                                                                      */
/************************************************************************/
#	define	SDKUPDATING_MUTEXNAME		DEFAULT_COMMPANY_NAME _T("_") PROJECT _T("_SDK_Updating_MutexName")	
#	define	INSTALLDIRECTORY			_T("Install Directory")

#ifdef _DEBUG
#	define	BACKUP_DIRECTORY			_T("Backup")
#	define	DOWNLOADED_DIRECTORY		_T("Downloaded")
#	define	UNZIPPED_DIRECTORY			_T("Unzipped")
#	define	DOWNLOAD_TEMP_DIRECTORY		_T("Downloading")
#	define	CACHE_DIRECTORY				_T("Cache")
#	define	IDS_SDK_UPDATE_FILE_NAME	_T("SDKUpdate")
#	define	DEFAULT_TEMPFILENAME		_T("DownloadTempFile.ema")
#else
#	define	BACKUP_DIRECTORY			_T("0CD07CEC-7FE4-4a40-A3F9-7C31244BE8BA")
#	define	DOWNLOADED_DIRECTORY		_T("C32F4521-B19C-4254-8744-C44B2D588312")
#	define	UNZIPPED_DIRECTORY			_T("CA929C71-403A-4f8e-80D8-35979FD00C07")
#	define	DOWNLOAD_TEMP_DIRECTORY		_T("C5428205-37AF-4432-BA99-BEC590872A1A")
#	define	CACHE_DIRECTORY				_T("741F952E-FB4A-46d8-B9D1-7F7A6EB59F8C")
#	define	IDS_SDK_UPDATE_FILE_NAME	_T("7F7A6EB59F8C")
#	define	DEFAULT_TEMPFILENAME		_T("085DB981-15F9-4a3a-95C6-EE12852EC97F.ema")
#endif
#	define	IDS_DOWNLOADING_FILE_EXT	_T(".!dt")

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef enum eTableName
{
	TABLE_ADINFO,
	TABLE_GAMEINFO,
}TABLENAME;

typedef enum eAdFocusUpdateMode
{
	AdFocusUpdateMode_DownloadCount = -3,
	AdFocusUpdateMode_ClickCount,
	AdFocusUpdateMode_LoadCount,
	AdFocusUpdateMode_Clearest,	//	最清晰的曝光状态
	AdFocusUpdateMode_Clearer,	//	比较清晰的曝光状态
	AdFocusUpdateMode_Clear,	//	清晰的曝光状态
	AdFocusUpdateMode_Spur,		//	不清晰的曝光状态
}EADFOCUSUPDATEMODE;

typedef enum  eResTypeOfAdInfo
{
	RESTYPEOFADINFO_ZIPFILE	= 0,			//	Zip 文件，包含原始图片文件以及广告文件
	RESTYPEOFADINFO_COMPRESSFILE,			//	自定义的压缩文件，包含原始图片以及广告文件
	RESTYPEOFADINFO_ADRESFILE,				//	仅包含广告资源文件图片
	RESTYPEOFADINFO_ORIGINALIMGFILE,		//	仅包含原始图片文件
	RESTYPEOFADINFO_NONE,					//	无任何文件
}ERESTYPEOFADINFO;

/************************************************************************/
/* 客户端使用的通信方式                                                 */
/************************************************************************/
typedef enum eCommType
{
	COMMTYPE_HTTP	= 0,					//	使用HTTP请求进行通信
	COMMTYPE_TCP,							//	使用自定义的TCP通信方式进行通信
	COMMTYPE_P2P,							//	使用P2P进行通信
}ECOMMTYPE;

/************************************************************************/
/* 服务器发送给客户端的AdInfo的格式                                     */
/************************************************************************/
typedef enum eAdInfoFormat
{
	ADINFOFMT_XML,							//	服务器直接将XML文件发送给客户端
	ADINFOFMT_PACKED,						//	服务器发送的是压缩的通信流
}EADINFOFORMAT;

/************************************************************************/
/* 客户端上传关注度数据的方式                                           */
/************************************************************************/
typedef enum eUploadFocusType
{
	UPLOADFOCUSTYPE_GAMESTART,				//	每次游戏启动上传
	UPLOADFOCUSTYPE_GAMESTOP,				//	每次游戏结束上传
	UPLOADFOCUSTYPE_DAIRY,					//	定时每天上传
	UPLOADFOCUSTYPE_HOUR,					//	定时每小时上传
	UPLOADFOCUSTYPE_WEEKLY,					//	定时每周上传
	UPLOADFOCUSTYPE_REALTIME,				//	实时上传
}EUPLOADFOCUSTYPE;

/************************************************************************/
/* 文件类型                                                             */
/************************************************************************/
typedef enum eFileType
{
	FILETYPE_NONE	= 0,
	FILETYPE_INI,	//	广告文件下载升级
	FILETYPE_XML,	//	XML文件，主要用于存放AdInfo信息
	FILETYPE_DDS,	//	DirectDrawSurface文件，主要存放图片类型的DDS文件
	FILETYPE_ZIP,	//	ZIP压缩文件
	FILETYPE_WYX,	//	EMMOA自定义的压缩文件
	FILETYPE_DLL,
	FILETYPE_JPG,
	FILETYPE_PNG,
	FILETYPE_SWF,
	FILETYPE_WBK,
	FILETYPE_EXE,
	FILETYPE_OCX,
	FILETYPE_WDX,
	FILETYPE_SYS,
	FILETYPE_LOG,
	FILETYPE_BMP,
	FILETYPE_TGA,
	FILETYPE_GIF,
	FILETYPE_RAR,	

	FILETYPE_MDL,
	FILETYPE_PLUGINS,
	FILETYPE_RES,
	FILETYPE_PATCHES,
	FILETYPE_DAT,

	
	/************************************************************************/
	/* 每个游戏相关的自定义到文件类型                                       */
	/************************************************************************/
	FILETYPE_FILE0,
	FILETYPE_PKG	= FILETYPE_FILE0,

	FILETYPE_FILE1,
	FILETYPE_FILE2,
	FILETYPE_FILE3,
	FILETYPE_FILE4,
	FILETYPE_FILE5,
	FILETYPE_FILE6,
	FILETYPE_FILE7,
	FILETYPE_FILE8,
	FILETYPE_FILE9,
	FILETYPE_FILEA,
	FILETYPE_FILEB,
	FILETYPE_FILEC,
	FILETYPE_FILED,
	FILETYPE_FILEE,
	FILETYPE_FILEF,

	FILETYPE_MAX,

}FILETYPE;


/************************************************************************/
/* 文件ID号                                                             */
/************************************************************************/
typedef enum eFileID
{
	FILEID_UNKNOWN	= -128,	//	未指定的文件ID号
	/************************************************************************/
	/* TODO:请在这里添加，不要改变接下来的这些ID数值，否则
	*	会引起系统以前版本的不兼容。
	*/
	/************************************************************************/
	FILEID_PLUGINSF	= -19,	//	插件ID号
	FILEID_PLUGINSE	= -18,	//	插件ID号
	FILEID_PLUGINSD	= -17,	//	插件ID号
	FILEID_PLUGINSC	= -16,	//	插件ID号
	FILEID_PLUGINSB	= -15,	//	插件ID号
	FILEID_PLUGINSA	= -14,	//	插件ID号
	FILEID_PLUGINS9	= -13,	//	插件ID号
	FILEID_PLUGINS8	= -12,	//	插件ID号
	FILEID_PLUGINS7	= -11,	//	插件ID号
	FILEID_PLUGINS6	= -10,	//	插件ID号
	FILEID_PLUGINS5	= -9,	//	插件ID号
	FILEID_PLUGINS4	= -8,	//	插件ID号
	FILEID_PLUGINS3	= -7,	//	插件ID号
	FILEID_PLUGINS2	= -6,	//	插件ID号
	FILEID_PLUGINS1	= -5,	//	插件ID号
	FILEID_PLUGINS0	= -4,	//	插件ID号

	FILEID_SDK		= -3,	//	SDK文件ID号
	FILEID_ADINFO	= -2,	//	广告信息文件ID好
	FILEID_CONFIG	= -1,	//	网络配置文件
	FILEID_AD		= 0,	//	广告资源
}FILEID;

/************************************************************************/
/* 操作系统类型                                                         */
/************************************************************************/
typedef enum  eOSTYPE
{
	OS_VISTA,		//	VISTA或更高的系统	
	OS_SVR2003,		//	Windows 2003
	OS_XP,			//	XP操作
	OS_2K,
	OS_WINNT,
	OS_98,
	OS_95,
	OS_ME,
	OS_W32S,
	OS_UNKNOWN,
}OSTYPE;

/************************************************************************/
/* 游戏类型                                                             */
/************************************************************************/
typedef enum eGameType
{
	GAMETYPE_UNKNOWN	=0,		//	未知类型的游戏

	GAMETYPE_FPS,	//	第一人称射击
	GAMETYPE_RPG,	//	角色扮演
	GAMETYPE_SLC,	//	策略
	GAMETYPE_AVG,	//	冒险
	GAMETYPE_SIM,	//	模拟
	GAMETYPE_STG,	//	射击
	GAMETYPE_FTG,	//	格斗
	GAMETYPE_PUZ,	//	益智
	GAMETYPE_RAC,	//	赛车
	GAMETYPE_SPT,	//	运动
	GAMETYPE_ACT,	//	动作

	GAMETYPE_3D		= 0x10000000,
	GAMETYPE_2D		= 0x20000000,
	
	GAMETYPE_OTHERS,
}GAMETYPE;

/************************************************************************/
/* 广告类型                                                              */
/************************************************************************/
typedef enum eAdType
{
	ADTYPE_UNKNOWN	= 0,
	ADTYPE_IMG,		//	图片型广告
	ADTYPE_FLASH,	//	FLASH广告
	ADTYPE_VIDEO,	//	视频广告
	ADTYPE_NPC,		//	NPC广告
	ADTYPE_SOUND,	//	声音广告
	//	TODO，请在这里继续添加，不要改变上述分类的数值
	

	ADTYPE_OTHERS,	//	
}ADTYPE;


/************************************************************************/
/*  广告资源类型                                                        */
/************************************************************************/
typedef enum eMediumType
{
	MEDIUMTYPE_DDS,	//
	MEDIUMTYPE_BMP,	//	
}EMEDIUMTYPE;

/************************************************************************/
/* DX或OpenGL平台                                                       */
/************************************************************************/
typedef enum eGraphicPlatform
{
	GP_NONE,
	GP_DX9,
	GP_DX8,
	GP_DD,
	GP_OPGL,
	GP_GDI,
	GP_OTHERS,
}GRAPHICSPLATFORM;

/************************************************************************/
/* 通信过程中的错误号                                                   */
/************************************************************************/
typedef enum eCommError
{
	ERR_COMM_NONE	= 0,			//	无错误信息

	ERR_COMM_INVALID_GAMEID,		//	无效的游戏ID号
	ERR_COMM_INVALID_AD_MEDIA,		//	无效的广告ID号

	ERR_COMM_CANT_OPEN_PLAYINFO,	
	ERR_COMM_BACKUP_PLAYINFO,	
	ERR_COMM_SAVE_PLAYINFO,			//	保存PlayInfo时写失败
	
	ERR_COMM_INVALID_PACKET_LENGTH,	//	通信报长度不正确
	ERR_COMM_INVALID_PACKET_FROM,	//	通信报来源不明确
	ERR_COMM_INVALID_PACKET_TYPE,	//	通信报格式不正确
	ERR_COMM_INVALID_AREAID,		//	无效的区域ID号
	ERR_COMM_INVALID_CHNID,			//	无效的渠道ID号或网吧号
	ERR_COMM_INVALID_SDKVER,		//	无效的SDK版本好
	ERR_COMM_INVALID_REQUESTTYPE,	//	无效的或尚未支持的请求号

	ERR_COMM_SAVE_FOCUSINFO,
	ERR_COMM_INVALID_DATA_FOCUSINFO,
	ERR_COMM_CANT_OPEN_FOCUSINFO,

}COMMERROR;


#endif