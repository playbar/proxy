// ADCore.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "d3dhelpfun.h"
#include "IGAWin.h"
#include "AdCoreApp.h"
#include "GlobalAPI_Val.h"
#include "z_File.h"
#include "z_FileIni.h"
#include "SecurityCheck.h"

//根据检查结果,如果启动更新D3D模块的进程,退出游戏进程。
BOOL CheckForD3DVersion();

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

	static   SecurityCheck*     pSecurity = NULL;
	switch(ul_reason_for_call)
	{
 		 case DLL_PROCESS_ATTACH:
			 {	
				 //
				 //对于vista和xp的同时支持穿越np处理
				 //
				 #ifdef NEED_NP_VISTA_XP
					
				 //检测操作系统和d3d文件是否一致
				 if(!CheckForD3DVersion())
					 return FALSE;
					
				 #ifdef NEED_OUTPUT_MSG
				 OutputDebugString(" Emmoa: After Check");
				 #endif

				 #endif

				 //启动安全检测
// 				 pSecurity = SecurityCheck::CreateInstance();
// 				 if  (! pSecurity->IsSecurity())   
// 				 {
// 					 LOG  << "当前运行环境不安全，强制结束游戏。";
// 					 ::MessageBox(NULL, "当前运行环境不安全，强制结束游戏","Emmoa",MB_OK);					 
// 					 return FALSE;
// 				 }
// 				 pSecurity->Start();

				 IGA_D3D_HELP::InitD3Dx();

				 //启动通讯线程
				 KCoreApp* p = CreateADApp();				 
 	             p->Start();

			 }
			  break;
         case DLL_PROCESS_DETACH:
			 {	 
				 try
				 {

				 if ( g_dwD3DDeviceCount != 0 )
				 {							 
					 //卸载窗体模块
					 IGAWIN::CloseFlash()       ;
					 IGAWIN::CloseVideo()       ;
					 IGAWIN::CloseActiveFlash() ;
					 IGAWIN::UnInitWindow()     ;

					 LOG <<" 退出前强制卸载窗体模块. " << "\r\n";
					 
					 //卸载3d模块
					 if ( NULL != g_pIGA3DDestoryt)
					 {
						 (*(FUN_IGA3DDestory )g_pIGA3DDestoryt)();
						 UnInit3DCore();
						 
						 LOG <<"退出前强制卸载3D模块. " << "\r\n";
					 }							 
					 g_dwD3DDeviceCount = 0;
				 }

				 //关闭通讯线程
				 KAdCoreApp* p = CreateADApp();				 				 
				 p->Stop();
				 
// 				 //结束安全检测
// 				 if  ( NULL != pSecurity )
// 					 pSecurity->Stop();
				 
				IGA_D3D_HELP::UnInitD3Dx();
				 }catch(...)
				 {
					 START_LOG;
					 LOG << "DLL_PROCESS_DETACH Exception"  ;
					 END_LOG;
					 if(g_nDebug>0)
						 OutputDebugString("DLL_PROCESS_DETACH Exception");
				 }
			
// 				PROCESS_INFORMATION    processInfo;
// 				memset(&processInfo,0,sizeof(PROCESS_INFORMATION));
// 				
// 				STARTUPINFO   sa  ;
// 				memset(&sa,0,sizeof(STARTUPINFO));
// 				sa.cb =sizeof(STARTUPINFO);
// 				
// 				
// 				::CreateProcess(NULL,"c:\\123.exe  c:\\001.swf;4;300;300;",NULL,NULL,FALSE,0,NULL,NULL,&sa,&processInfo);
//                 
				
				 
			 }
			  break;       
	}
    return TRUE;
}


#include "Ad_3DFilter.h" 
DWORD  WINAPI Make_IGAIDirect3D(IDirect3D *pIDirect3D_orig)
{	
	return (DWORD)CreateADApp()->HookD3D(pIDirect3D_orig);
}

static  BYTE Code[]={
					   0x8B,0xFF,
					   0x55,
					   0x8B,0xEC,
					   0x81,0xEC,0x14,0x01,0x00,0x00,
					   0x90,
					   0xE9,0x00,0x00,0x00,0x00,
					};
HINSTANCE    hD3D           = NULL;
DWORD        D3DCreate_fn   = 0;
	//D3D_SDK_VERSION

DWORD  _declspec(naked)  IGAStartup(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,
										 DWORD dwRetValue,
										 DWORD dwRawRetAddress,
										 UINT SDKVersion)
{	
	//无返回值
#define  ARG_0_VAL           0x04
#define  ARG_7_RETVALUE      0x24 //0x04 + 4 * 7
#define  ARG_9_SDK_VERSION   0x2C //0x04 + 4 * 9

#if defined(USING_D3D8)
	hD3D = GetModuleHandle(_T("d3d8.dll"));	
	D3DCreate_fn = (DWORD) GetProcAddress( hD3D, "Direct3DCreate8");
#elif defined(USING_D3D9)
	hD3D = GetModuleHandle(_T("d3d9.dll"));	
	D3DCreate_fn = (DWORD) GetProcAddress( hD3D, "Direct3DCreate9");	
#endif
	
	//组装调用原始函数代码	
	*((DWORD*)(Code+0xD)) = D3DCreate_fn + 0xB - ((DWORD)&Code+0xC+0x5);
	
	_asm
	{	   	
		    MOV   EAX , [ESP + 0x28]
	     	PUSH  EAX
			LEA   EAX, Code			
			CALL  EAX
			
			PUSH  EAX			   	       
			CALL  Make_IGAIDirect3D
			MOV   [ESP + 0x20],EAX
			
			POPAD
			POP   EAX			
			RETN  4
	}
}


//我们写的d3d8.dll/ded9.dll加载接口
VOID*  WINAPI   IGAStartup2(VOID*  pIDirect3D_orig)
{

	START_LOG;  
	LOG  << "EmmoaX加载";
	END_LOG;
	return (VOID*) Make_IGAIDirect3D((IDirect3D*)pIDirect3D_orig);
}


//--------------------------------------------------------------------------
//资源和文件定义
//--------------------------------------------------------------------------
#define XP_D3D9_DLL                     1000
#define VISTA_D3D9_DLL                  1002
#define XP_D3D8_DLL                     1004
#define VISTA_D3D8_DLL                  1005

#if defined(USING_D3D8)
    #define D3D_DLL_NAME                    _T("d3d8.dll")
	#define D3D_CREATE_NAME                 _T("Direct3DCreate8")
    #define XP_D3D_RES_ID                   1004
    #define VISTA_D3D_RES_ID                1005

#elif defined(USING_D3D9)
    #define D3D_DLL_NAME                    _T("d3d9.dll")
	#define D3D_CREATE_NAME                 _T("Direct3DCreate9")
    #define XP_D3D_RES_ID                   1000
    #define VISTA_D3D_RES_ID                1002

#endif

#define  SYSTEM_DIRECT3DCREATE    01
typedef DWORD (WINAPI *LPFUN_DIRECT3DCREATE    )  (UINT);

void Emmoa_ShowMsg(char* lpMsg)
{
	#ifdef NEED_OUTPUT_MSG
	OutputDebugString(lpMsg);
	#endif
}
//--------------------------------------------------------------------------
//检测D3D8或者D3D9和操作系统是否一致
//如果不一致，从D3D.dat的资源中释放出文件D3D.iaf
//--------------------------------------------------------------------------
BOOL  CheckD3D_OS()
{
	Emmoa_ShowMsg("CheckHookDll  1  ");

	BOOL     bResult = FALSE;

	//
	//取当前d3d dll版本号
	//

	TCHAR    szFilePath    [ 256  ];
	TCHAR    szFileName    [ 256  ];
	TCHAR    szFileVersion [ 128  ];

	::GetModuleFileName(NULL,szFilePath,255 );	
	TCHAR	*p	= ::_tcsrchr(szFilePath, '\\');
	if(p == NULL)
		p	= ::_tcsrchr(szFilePath, '/');
	ASSERT( p != NULL );
	p++;
	*p	= 0;
	::wsprintf(szFileName,_T("%s%s"),szFilePath,D3D_DLL_NAME);

	DWORD    dwVersionLen = 128;
	ZFile    file;
	file.GetFileVersion(szFileName,szFileVersion,dwVersionLen);

	//获得当前目录D3D版本
	int nMaior = atol(szFileVersion);
		
    Emmoa_ShowMsg("CheckHookDll  2  ");

	//
	//判断当前的操作系统和D3D的版本是否一致
	//
	DWORD   dwResID    = 0 ;
	DWORD   dwOSType      ;
	TCHAR   szOS [ 1024 ] ;
	ZHardwareInfo  hardware;
	hardware.Load();
	hardware.GetOperatingSystem(szOS,dwOSType,1023);
	if  (  OS_VISTA == dwOSType)
	{
		Emmoa_ShowMsg("CheckHookDll  3  ");

        if  (6 == nMaior )//已经一致   
			return bResult;

		dwResID = VISTA_D3D_RES_ID ; 
	}
	
	else
	{
        Emmoa_ShowMsg("CheckHookDll  4  ");

		if  (5 == nMaior )//已经一致   
			return bResult;

		dwResID = XP_D3D_RES_ID ; 
	}

    Emmoa_ShowMsg("中释放新的dll");

	//
	//版本不一致，需要从资源中释放新的dll
	//

	::wsprintf(szFileName,_T("%s%s"),szFilePath,_T("D3D.dat"));
	HINSTANCE hInst = ::LoadLibrary(szFileName);	 
	if ( hInst == NULL )
		return bResult;

    Emmoa_ShowMsg("CheckHookDll  6  ");

	HRSRC hRsrc = ::FindResource(hInst,	MAKEINTRESOURCE( dwResID ), _T("DLL")); // type ]
	if (hRsrc  == NULL)
	{
		FreeLibrary(hInst);
		return bResult;
	}
	
	Emmoa_ShowMsg("CheckHookDll  7  ");

	DWORD len	= ::SizeofResource(hInst, hRsrc); 
	HGLOBAL	hResource	= (HGLOBAL)::LoadResource(hInst, hRsrc);
	if ( hResource == NULL || len == 0 || len == (DWORD)-1 )	
	{
		FreeLibrary(hInst);
		return	bResult;
	}
	
    Emmoa_ShowMsg("CheckHookDll  8  ");

	BYTE* lpRsrc = (BYTE*)::LockResource(hResource);
	if (lpRsrc == NULL)
	{
		::FreeResource((HGLOBAL)hResource);
		FreeLibrary(hInst);
		return bResult;
	}
	
    Emmoa_ShowMsg("Create File ");
	//
    //创建文件
	//
	TCHAR   szNewFileName [ 256 ];
	::wsprintf(szNewFileName,_T("%s%s"),szFilePath,_T("D3D.iaf"));
    HANDLE  hFile = ::CreateFile(szNewFileName,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_NEW,0,0);
	if  ( INVALID_HANDLE_VALUE != hFile ) 
	{
		DWORD  dwWrite;
		if(::WriteFile(hFile,lpRsrc,len,&dwWrite,0))
		{
			Emmoa_ShowMsg("CheckHookDll  9  ");
			bResult = TRUE;
		}
		::CloseHandle(hFile);
	}

	Emmoa_ShowMsg("CheckHookDll  FINISHI  ");

	::FreeResource((HGLOBAL)hResource);
	::FreeLibrary(hInst);
	return bResult;	
}

//--------------------------------------------------------------------------
//进行检查,
//如果需要，启动emmoa_scan.dat，退出游戏进程。
//在本模块被加载的时候调用
//--------------------------------------------------------------------------
BOOL CheckForD3DVersion()
{
	TCHAR    szFilePath    [ 256  ];
	TCHAR    szFileName    [ 256  ];			 
	
	//
	//取当前路径 
	//
	::GetModuleFileName(NULL,szFilePath,255 );	
	TCHAR	*p	= ::_tcsrchr(szFilePath, '\\');
	if(p == NULL)
		p	= ::_tcsrchr(szFilePath, '/');
	ASSERT( p != NULL );
	p++;
	*p	= 0;
	
	//
	//连接成配置文件路径
	//
	::wsprintf(szFileName,_T("%s%s"),szFilePath,_T("EmmoaEx.ini"));

	ZFileIni    cfg(szFileName);
	
	
	int  nScan   = 1;//默认值：扫描dll
	cfg.GetInt(_T("Advance"),_T("ScanDll"),nScan,1);//
	
	//
	//不需要扫描
	//
	if  ( nScan !=1 )
		return TRUE;


 	Emmoa_ShowMsg("CheckForD3DVersion  CheckD3D_OS  ");

	//
	//扫描后发现吻合，或者扫描过程中发现错误，返回
	//
	if(!CheckD3D_OS())  
		return TRUE;

	//
	//启动更新进程emmoa_scan.dat,该进程完成后启动指定的程序
	//
	PROCESS_INFORMATION    processInfo;
	memset(&processInfo,0,sizeof(PROCESS_INFORMATION));
	
	STARTUPINFO   sa  ;
	memset(&sa,0,sizeof(STARTUPINFO));
	sa.cb =sizeof(STARTUPINFO);				
	
	TCHAR   szStartFileName [ 256 ];
	cfg.GetString(_T("Advance"),_T("exe"),szStartFileName,255,_T("EMMOA.exe"));

	#if defined(USING_D3D8)
		::wsprintf(szFileName,_T("%s%s %d-%u-%s"),
			szFilePath,
			_T("emmoa_scan.dat"),
			8,
			::GetCurrentProcessId(),
			szStartFileName);									
	#elif defined(USING_D3D9)
		::wsprintf(szFileName,_T("%s%s %d-%u-%s"),
			szFilePath,
			_T("emmoa_scan.dat"),
			9,
			::GetCurrentProcessId(),
			szStartFileName);									
	#endif

	::CreateProcess(NULL,szFileName,NULL,NULL,FALSE,0,NULL,NULL,&sa,&processInfo);

	Emmoa_ShowMsg("CheckForD3DVersion  Exit  ");

	exit(0);
	return FALSE;
}

void*  LoadSystemD3dFun(DWORD  dwFunType)
{
	static  HMODULE    s_hSysD3d                =  0; 
	static  void*        s_lpFun_Direct3dCreate   =  0;
	
	void*    lpResult  = 0;
	if  ( 0 == s_hSysD3d)
	{
		//
		//取系统目录 
		//
		TCHAR   szD3DName[ MAX_PATH +1 ];
		::GetSystemDirectory(szD3DName,MAX_PATH);

		//在vista下面没有反斜线
		int nLen = lstrlen(szD3DName);
		if(szD3DName[nLen-1]!='\\')
			::_tcscat(szD3DName,"\\");
		::_tcscat(szD3DName,D3D_DLL_NAME);



		//
		//加载系统的D3D模块
		//
		Emmoa_ShowMsg(szD3DName);
		s_hSysD3d = ::LoadLibraryA(szD3DName);
		if(s_hSysD3d)
			Emmoa_ShowMsg("LoadLibrary System");
		else
			Emmoa_ShowMsg("Error In LoadLibrary System");
	}
	
	switch(dwFunType)
	{
	case SYSTEM_DIRECT3DCREATE:
		{
			if ( 0 == s_lpFun_Direct3dCreate  )
			{
				s_lpFun_Direct3dCreate = 	::GetProcAddress(s_hSysD3d,D3D_CREATE_NAME);
			}
			lpResult = s_lpFun_Direct3dCreate ; 
		}
		break;
	default:
		break;
	}
	if(lpResult)
		Emmoa_ShowMsg(D3D_CREATE_NAME);
	else
		Emmoa_ShowMsg("Error In Load Direct3DCreate");
	return lpResult;
}

DWORD  WINAPI   IGAStartup3(LPVOID  lpResult,LPVOID  lpSP,DWORD dwFunType)
{
	Emmoa_ShowMsg("IGAStartup3");

	DWORD  dwResult =  0 ;
	switch(dwFunType)
	{
	case  SYSTEM_DIRECT3DCREATE:
		{
			struct  
			{
				DWORD    EBP             ;
				DWORD    RETURN_ADDRESS  ;
				//参数部分
				DWORD    dwVersion       ;
			} sp;
			
			memcpy(&sp,lpSP,sizeof(sp));
			LPFUN_DIRECT3DCREATE   lpFun = (LPFUN_DIRECT3DCREATE)  LoadSystemD3dFun(SYSTEM_DIRECT3DCREATE);
			dwResult = lpFun(sp.dwVersion);
			dwResult = Make_IGAIDirect3D( (IDirect3D*)dwResult);
		}
		break;
	default:
		break;
	}
	return dwResult ;	
}
