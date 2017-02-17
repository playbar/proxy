// 3D_TOOL.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "3D_TOOL.h"
#include "3D_TOOLDlg.h"
#include ".\3d_tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD_TOOLApp

BEGIN_MESSAGE_MAP(CD_TOOLApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CD_TOOLApp 构造

CD_TOOLApp::CD_TOOLApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CD_TOOLApp 对象

CD_TOOLApp theApp;

/*
	共享的内存
*/
IGA_DATA_MEM g_IGA_Date(_IGA_DATA);

// CD_TOOLApp 初始化

BOOL CD_TOOLApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CD_TOOLDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭
		//对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。
	return FALSE;
}

