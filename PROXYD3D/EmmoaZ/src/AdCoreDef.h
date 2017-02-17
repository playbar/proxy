#ifndef _ADCOREDEF_H
#define _ADCOREDEF_H


/*配置文件定义宏*/
#define	IDS_SECTION_COMMON			_T("Common")
#define	IDS_SECTION_HTTP			_T("HTTP")
#define	IDS_SECTION_TCP				_T("Tcp")
#define	IDS_SECTION_SAMPLE			_T("Sample")
#define	IDS_KEY_DOWNLOAD_URL		_T("DownloadURL")
#define	IDS_KEY_ENABLE_LOGGING		_T("EnableLogging")
#define	IDS_KEY_USING_LOCAL_CONFIG	_T("UsingLocalConfig")
#define	IDS_KEY_SAVE_LOCAL_FILE_USING_ENCRYPT	_T("SaveLocalFileUsingEncrypt")

#define	IDS_KEY_IAMTOOL				_T("Tools")
#define	IDS_KEY_ENABLE_SDK_UPDATE	_T("EnableSDKUpdate")
#define	IDS_KEY_COMMTYPE			_T("CommType")
#define	IDS_KEY_DEFAULT_URL			_T("DefaultURL")
#define	IDS_KEY_ADINFO_URL			_T("AdInfoURL")
#define	IDS_KEY_ADINFO_URL_EX		_T("AdInfoURL1")
#define	IDS_KEY_ADINFOXML			_T("AdInfoXML")
#define	IDS_KEY_CONFIG_URL			_T("ConfigURL")
#define	IDS_KEY_ADSETTINGS			_T("AdURL")
#define	IDS_KEY_DELETEEXPIREDAD		_T("DeleteAdFileWhenExpired")
#define	IDS_KEY_GAMEID				_T("GameID")
#define	IDS_KEY_DEBUG				_T("DEBUG")
#define	IDS_KEY_ADCOUNT				_T("AdCount")
#define	IDS_KEY_CHN_ID				_T("Channel")
#define	IDS_KEY_UPLOADFOCUS			_T("UploadFocus")
#define	IDS_KEY_MINSLEEPTIME		_T("MinSleepTime")
#define	IDS_KEY_MAXSLEEPTIME		_T("MaxSleepTime")
#define	IDS_SECTION_EXPORT			_T("Export")
#define	IDS_KEY_EXPORTPATH			_T("Path")
#define	IDS_KEY_ENABLEEXPORT		_T("Enable")
#define	IDS_KEY_EXPORTPARAM			_T("Param")
#define	IDS_KEY_STARTSN				_T("StartSN")
#define	IDS_KEY_DOWNLOAD_CONFIG_DAILY		_T("DownloadConfigDaily")	//	每天下载一次网络配置文件
#define	IDS_KEY_DOWNLOAD_ADINFO_DAILY		_T("DownloadAdInfoDaily")

#define	IDS_KEY_BUILDIN_WINDOW		_T("BuildinWindow")
#define	IDS_KEY_SHORTCUT			_T("Shortcut")
#define	IDS_KEY_ROUTERLIST			_T("RouterList")
#define	IDS_KEY_ROUTERIP			_T("RouterIP")
#define	IDS_KEY_ROUTERPORT			_T("RouterPort")
#define	IDS_KEY_SAMPLECOUNT			_T("SampleCount")

/**/
#	define	PROJECT_HOMEPAGE0			"http://www.emmoa.com/"
#	define	PROJECT_HOMEPAGE1			"http://www.emmoa.net/"
#	define	PROJECT_HOMEPAGE2			"http://www.emmoa.cn/"
#	define	PROJECT_HOMEPAGE3			"http://www.emmoa.com.cn/"


/*函数*/

#	define	appZeroMemory		::ZeroMemory
#	define	appCopyMemory		::CopyMemory
#	define	appStrStr			::_tcsstr
#	define	appStrChr			::_tcschr
#	define	appStrrChr			::_tcsrchr
#	define	appStrlen			::_tcslen
#	define	appStrCmp			::_tcscmp
#	define	appStrICmp			::_tcsicmp
#	define	appStrNCmp			::_tcsncmp
#	define	appStrNICmp			::_tcsnicmp
#	define	appStrCat			::_tcscat


#	define	appStrCopy			::_tcscpy
#	define	appPrintF			::_stprintf
#	define	appPrintFN			::_sntprintf
#	define	appFullPath			::_tfullpath
#	define	appSplitPath		::_tsplitpath
#	define	appAccess			::_taccess
#	define	appMakeDir			::_tmkdir

#	define	appMoveMemory		::memmove
#	define	appCompMemory		::memcmp
#	define	appCmpIMemory		::memicmp
#	define	appSetMemory		::memset

/*常用符号*/
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

/*工程名称*/
#	define	PROJECT						_T("EMMOA")
#endif