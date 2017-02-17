// ServerULog.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ServerULog.h"

//--------------------------------------------------------------------------------------
// 包含头文件
//--------------------------------------------------------------------------------------
#include "FtpDownLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CServerULogApp

BEGIN_MESSAGE_MAP(CServerULogApp, CWinApp)
	//{{AFX_MSG_MAP(CServerULogApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerULogApp construction

CServerULogApp::CServerULogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerULogApp object

CServerULogApp theApp;
CFtpDownLog g_FtpLogBase;				//FTP处理对象
CFtpDownLogDecorator g_FtpLog( &g_FtpLogBase);

BOOL CServerULogApp::InitInstance() 
{
	//
	//获得本模块全路径
	//
	::GetModuleFileName(m_hInstance,m_strPath.GetBuffer(MAX_PATH),MAX_PATH);
	m_strPath.ReleaseBuffer();

	m_strPath = m_strPath.Left(m_strPath.ReverseFind('\\')+1);

	if(m_strPath.Right(1)!='\\')
		m_strPath+='\\';

	if(!g_FtpLog.InitData(m_strPath))
		return FALSE;

	return CWinApp::InitInstance();
}

int CServerULogApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_FtpLog.ClearData();
	
	return CWinApp::ExitInstance();
}

//--------------------------------------------------------------------------------------
// 处理服务器事件
//--------------------------------------------------------------------------------------
WORD CALLBACK  HandleEventHook(RFTPEventStr* pEventStruc)
{

	switch(pEventStruc->Event)
	{
	case EVNT_StartDown:	// start of download of file					

		//g_FtpLog.LogText("EVNT_StartDown");

		//
		//忽略被过滤文件
		//
		if (g_FtpLog.FilterFile(pEventStruc))
			break;

		g_FtpLog.StartDownFile(pEventStruc);
		break;

	case EVNT_EndDown:		// successful download of file

		//g_FtpLog.LogText("EVNT_EndDown");

		//
		//忽略被过滤文件
		//
		if (g_FtpLog.FilterFile(pEventStruc))
			break;

		g_FtpLog.EndDownFile(pEventStruc);
			break;

	case EVNT_AbortDown:	// aborted download

		//g_FtpLog.LogText("EVNT_AbortDown");

		//
		//忽略被过滤文件
		//
		if (g_FtpLog.FilterFile(pEventStruc))
			break;

		g_FtpLog.AbortDownd(pEventStruc);
		break;
	}

	return REVNT_None;
}

//--------------------------------------------------------------------------------------
// 处理客户端事件
//--------------------------------------------------------------------------------------
int __cdecl HandleClientEvent(RClientEventStr* pClientEvent)
{
	return 0;
}
