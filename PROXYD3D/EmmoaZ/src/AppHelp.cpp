#include "StdAfx.h"
#include "apphelp.h"
#include "z_File.h"
#include "AdCoreDef.h"



#define	    DEFAULT_COMMPANY_NAME		_T("InterAdtive")


#define     AD_RES_FOLDER               _T("res")        //资源路径
#define     AD_RES_FOLDER_TEXTURE       _T("texture")    //纹理

#define     AD_RES_FOLDER_MESH          _T("mesh")       //模型
#define     AD_MESH_CFG                 _T("data.ini")

#define     AD_RES_FOLDER_FALSH         _T("flash")      //flash
#define     AD_FALSH_CFG                _T("flash.ini")

#define     AD_RES_FOLDER_VIDEO         _T("video")      //video
#define     AD_VIDEO_CFG                _T("video.ini")

#define     AD_RES_FOLDER_ACTIVE_FALSH  _T("activeflash")//活动flash
//#define     AD_ACTIVEFALSH_CFG          _T("activeFlash.ini")
#define     AD_ACTIVEFALSH_CFG          _T("Flash.ini")



#define	    FOLDER_PLUGINS				_T("Plugins")
#define	    FOLDER_DOWNLOADED   		_T("Downloaded")
#define	    FOLDER_UNZIPPED  			_T("Unzipped")
#define	    FOLDER_DOWNLOAD_TEMP		FOLDER_DOWNLOADED
#define	    FOLDER_CACHE				_T("Cache")

#define     IGA_SYS_INI_CFG0            _T("Emmoa.ini")         //配置文件名称
#define     IGA_AD_XML_CFG0             _T("AdInfo.xml")        //广告配置文件名称

IGAPath::IGAPath ()
{
	m_WorkingPath[0] = 0;
	GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
    
	TCHAR   szTmpPath [ 256 ] ;
	wsprintf(szTmpPath,_T("%s%s"),m_WorkingPath,DEFAULT_COMMPANY_NAME);
	::CreateDirectory(szTmpPath,NULL);

	wsprintf(szTmpPath,_T("%s%s//%s"),m_WorkingPath,DEFAULT_COMMPANY_NAME,PROJECT);
	::CreateDirectory(szTmpPath,NULL);

	wsprintf(szTmpPath,_T("%s%s//%s//%s"),m_WorkingPath,DEFAULT_COMMPANY_NAME,PROJECT,AD_RES_FOLDER);
	::CreateDirectory(szTmpPath,NULL);
	
}

IGAPath::~IGAPath()
{

}

BOOL	IGAPath::GetWorkingPath( LPTSTR lpszPath,DWORD   dwLen ,HANDLE hModel )
{	
	if ( lpszPath == NULL ) return FALSE;

	BOOL    bResult = FALSE;
	TCHAR	szPath[_MAX_PATH]	= { 0 };
	::GetModuleFileName((HINSTANCE)hModel, szPath, _MAX_PATH);
	TCHAR	*p	= appStrrChr(szPath, BRK);
	if(p == NULL)
		p	= appStrrChr(szPath, ABRK);
	ASSERT( p != NULL );
	p++;
	*p	= 0;

	if ( _tcslen(p) < dwLen )
	{
		_tcscpy(lpszPath,szPath);
		bResult  = TRUE;
	}

	return	bResult;
}

/************************************************************************/
/* 取插件的路径                                                         */
/************************************************************************/
BOOL IGAPath::GetPluginsPath(LPTSTR pszPathName,DWORD   dwLen)
{
	
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}


	
	wsprintf(pszPathName,  _T("%s%s%s%s%s%s%s"),
							m_WorkingPath,
							DEFAULT_COMMPANY_NAME,
							DIREND,
							PROJECT,
							DIREND,
							FOLDER_PLUGINS,
							DIREND);

	if( !ZFile::IsExisted(pszPathName))
		ZFile::CreatePath(pszPathName);

	return	TRUE;	

}

BOOL IGAPath::GetCachePath(LPTSTR pszPath,DWORD   dwLen)
{
#ifdef NEED_OUTPUT_MSG
	OutputDebugString("IGAPath::GetCachePath");
#endif	
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}

	
	wsprintf(pszPath,	_T("%s%s%s%s%s%s%s"), m_WorkingPath,
						DEFAULT_COMMPANY_NAME,
						DIREND,
						PROJECT,
						DIREND,
						FOLDER_CACHE,
						DIREND);	
	if( !ZFile::IsExisted(pszPath))
		ZFile::CreatePath(pszPath);

	

    return	TRUE;
}


BOOL IGAPath::GetUnzippedPath(LPTSTR pszPath,DWORD   dwLen)
{
#ifdef NEED_OUTPUT_MSG
	OutputDebugString("IGAPath::GetUnzippedPath");
#endif	
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}

	wsprintf(pszPath,	_T("%s%s%s%s%s%s%s"), m_WorkingPath,
						DEFAULT_COMMPANY_NAME,
						DIREND,
						PROJECT,
						DIREND,
						FOLDER_UNZIPPED,
						DIREND);		

	if( !ZFile::IsExisted(pszPath))
		ZFile::CreatePath(pszPath);


	
	return	TRUE;
}

BOOL IGAPath::GetDownloadPath(LPTSTR pszPath,DWORD   dwLen)
{
   
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}

	wsprintf(pszPath,	_T("%s%s%s%s%s%s%s"), m_WorkingPath,
						DEFAULT_COMMPANY_NAME,
						DIREND,
						PROJECT,
						DIREND,
						FOLDER_DOWNLOADED,
						DIREND);

	if( !ZFile::IsExisted(pszPath))
		ZFile::CreatePath(pszPath);
	
	
	return	TRUE;
}

BOOL IGAPath::GetDefaultTargetPath(LPTSTR pszPath,DWORD   dwLen)
{
	return	GetDownloadPath(pszPath,dwLen);
}

BOOL IGAPath::GetDefaultTempPath(LPTSTR pszPath,DWORD   dwLen)
{
	return GetDownloadPath(pszPath,dwLen);
}

/*取配置文件名称*/
VOID IGAPath::GetSysCfgName(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s%s"), m_WorkingPath,IGA_SYS_INI_CFG0);		
}
/*取广告配置文件*/
VOID IGAPath::GetAdCfgName(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s%s\\%s"), m_WorkingPath,DEFAULT_COMMPANY_NAME,IGA_AD_XML_CFG0);	
}

/* 取匹配文件路径*/
VOID IGAPath::GetMatchFilePath(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s%s\\%s\\%s\\%s\\"), 
		                m_WorkingPath,
						DEFAULT_COMMPANY_NAME,
						PROJECT,
						AD_RES_FOLDER,
						AD_RES_FOLDER_TEXTURE);
	if( !ZFile::IsExisted(pszName))
		ZFile::CreatePath(pszName);

}

/* 取广告文件路径*/
VOID IGAPath::GetADFilePath(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s%s\\%s\\%s\\%s\\"),
		                m_WorkingPath,
		                DEFAULT_COMMPANY_NAME,
						PROJECT,
						AD_RES_FOLDER,
						AD_RES_FOLDER_TEXTURE);

	
	if( !ZFile::IsExisted(pszName))
		ZFile::CreatePath(pszName);

}


/* 取flash文件路径*/
VOID IGAPath::GetFlashPath(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s%s\\%s\\%s\\%s\\"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME,
		PROJECT,
		AD_RES_FOLDER,
		AD_RES_FOLDER_FALSH);
	if( !ZFile::IsExisted(pszName))
		ZFile::CreatePath(pszName);

}
VOID IGAPath::GetFlashCfgName(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s%s\\%s\\%s\\%s\\%s"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME,
		PROJECT,
		AD_RES_FOLDER,
		AD_RES_FOLDER_FALSH,
		_T("flash.ini"));
}

/* 取video文件路径*/
VOID IGAPath::GetVideoPath(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s%s\\%s\\%s\\%s\\"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME,
		PROJECT,
		AD_RES_FOLDER,
		AD_RES_FOLDER_VIDEO);
	if( !ZFile::IsExisted(pszName))
		ZFile::CreatePath(pszName);

}
VOID IGAPath::GetVideoCfgName(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s%s\\%s\\%s\\%s\\%s"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME,
		PROJECT,
		AD_RES_FOLDER,
		AD_RES_FOLDER_VIDEO,
		AD_VIDEO_CFG);
}

/* 取active flash文件路径*/
VOID IGAPath::GetActiveFlashPath(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s%s\\%s\\%s\\%s\\"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME,
		PROJECT,
		AD_RES_FOLDER,
		AD_RES_FOLDER_ACTIVE_FALSH);
	if( !ZFile::IsExisted(pszName))
		ZFile::CreatePath(pszName);

}
VOID IGAPath::GetActiveFlashCfgName(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s%s\\%s\\%s\\%s\\%s"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME,
		PROJECT,
		AD_RES_FOLDER,
		AD_RES_FOLDER_ACTIVE_FALSH,
		AD_ACTIVEFALSH_CFG);
}

/* 取3d文件路径*/
VOID IGAPath::Get3DPath(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
// 	wsprintf(pszName,	_T("%s%s\\%s\\%s\\%s\\"),
// 		m_WorkingPath,
// 		DEFAULT_COMMPANY_NAME,
// 		PROJECT,
// 		AD_RES_FOLDER,
// 		AD_RES_FOLDER_MESH);
	wsprintf(pszName,	_T("%s%s\\%s\\3d\\"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME,
		PROJECT
		);
	if( !ZFile::IsExisted(pszName))
		ZFile::CreatePath(pszName);

}
VOID IGAPath::Get3DCfgName(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
// 	wsprintf(pszName,	_T("%s%s\\%s\\%s\\%s\\%s"),
// 		m_WorkingPath,
// 		DEFAULT_COMMPANY_NAME,
// 		PROJECT,
// 		AD_RES_FOLDER,
// 		AD_RES_FOLDER_MESH,
// 		AD_MESH_CFG);
	wsprintf(pszName,	_T("%s%s\\%s\\3d\\data.ini"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME,
		PROJECT
		);

}

//取安全配置文件路径
VOID IGAPath::GetSecurityFilePath(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s\\%s\\EmmoaSecurity.ini"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME		
		);
}

/*取SDK文件路径*/
VOID IGAPath::GetSDKZIPFileName(LPTSTR pszName,DWORD   dwLen)
{
	if (m_WorkingPath[0] == 0 )
	{
		IGAPath::GetWorkingPath(m_WorkingPath,_MAX_PATH,NULL);
	}	
	wsprintf(pszName,	_T("%s\\%s\\EMMOA\\SDK\\Emmoa.sdk"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME		
		);

	TCHAR   szSDKPath [ _MAX_PATH +1 ];
	wsprintf(szSDKPath,	_T("%s\\%s\\EMMOA\\SDK\\"),
		m_WorkingPath,
		DEFAULT_COMMPANY_NAME		
		);

	if( !ZFile::IsExisted(szSDKPath))
		ZFile::CreatePath(szSDKPath);
}

IP_AREA::IP_AREA():
       m_hAreaDll(NULL)
{
     
}
IP_AREA::~IP_AREA()
{

	if  ( NULL != m_hAreaDll )
	{
		FreeLibrary(m_hAreaDll);
		m_hAreaDll = NULL;
	}
}

DWORD   IP_AREA::GetAreaID(LPCTSTR  lpszIP)
{
	if  ( NULL == m_hAreaDll )
	    m_hAreaDll = LoadLibrary( "Emmoap_e.iaf");

	typedef  DWORD (WINAPI *LP_EMMOAP_E)(LPCTSTR );
	static LP_EMMOAP_E    lp_funEmmoap_e = NULL;
    if  ( NULL == lp_funEmmoap_e )
	{
        lp_funEmmoap_e = (LP_EMMOAP_E)GetProcAddress(m_hAreaDll,"EMMOAP_E") ;
		
	}
     
	if  ( lp_funEmmoap_e != NULL)
	{

		return lp_funEmmoap_e(lpszIP);
	}

	return -1;
}