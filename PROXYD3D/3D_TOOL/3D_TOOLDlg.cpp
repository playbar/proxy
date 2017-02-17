// 3D_TOOLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "3D_TOOL.h"
#include "3D_TOOLDlg.h"
#include ".\3d_tooldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//全局的主对话框的指针
CD_TOOLDlg* g_pMainDlg = NULL;

extern IGA_DATA_MEM g_IGA_Date;

/*
	更新客户端状态
*/
void UpdateClient()
{
	g_IGA_Date[0].bIsReload = true;
	g_IGA_Date.Update();
}

void GetAbsPath( char fullPathName[MAX_PATH] )
{
	//OutputDebugString(fullPathName);
	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *p;
	p = strrchr(TmpPath,'\\');
	if(!p)
		return ;
	*(p+1) = 0;

    char path[MAX_PATH];	
	sprintf( path,"%s%s", TmpPath, fullPathName );
	strcpy(fullPathName,path);
	
}

//创建多级目录
BOOL mkdirEx(CString Path)
{
    CString pathname = Path;
    if(pathname.Right(1) != "\\")
	{
		pathname += "\\" ;
	}
	
    int end = pathname.ReverseFind('\\');
    int pt = pathname.Find('\\');
    if ( pt-1 >= 0 && pathname[pt-1] == ':')
	{
		pt = pathname.Find('\\', pt+1);
	}

    CString path;
    while(pt != -1 && pt<=end)
    {
        path = pathname.Left(pt+1);
        if(_access(path, 0) == -1)
		{
			if( -1 == _mkdir(path))
			{
				return false;
			}
		}
        pt = pathname.Find('\\', pt+1);
    }
    return true;
}



int SFileCopy::nCount = 500;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CD_TOOLDlg 对话框



CD_TOOLDlg::CD_TOOLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CD_TOOLDlg::IDD, pParent)
	, m_GameName(_T(""))
	, m_ResourcePath(_T(""))
	, m_nCurPickID(0)
	, m_bShowPickMesh(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CD_TOOLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_GAME_NAME, m_GameName);
	DDX_Text(pDX, IDC_STATIC_RES_PATH, m_ResourcePath);
	DDX_Control(pDX, IDC_LIST_3DINFO, m_3DInfoList);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_ButtonAdd);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_ButtonDelete);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_ButtonClear);
	DDX_Control(pDX, IDC_BUTTON_OUTPUT, m_ButtonOutput);
	DDX_Control(pDX, IDC_BUTTON_SET, m_ButtonSet);
	DDX_Control(pDX, IDC_BUTTON_QUIT, m_ButtonQuit);
	DDX_Text(pDX, IDC_STATIC_TEXT_VLUE, m_nCurPickID);
	DDX_Check(pDX, IDC_CHECK1, m_bShowPickMesh);
}

BEGIN_MESSAGE_MAP(CD_TOOLDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SET, OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, OnBnClickedButtonQuit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_3DINFO, OnNMRclickList3dinfo)
	ON_NOTIFY(NM_CLICK, IDC_LIST_3DINFO, OnNMClickList3dinfo)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnBnClickedButtonOutput)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CD_TOOLDlg 消息处理程序

BOOL CD_TOOLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	bool	_bInit = true;


	UpdateData();
	m_GameName	= "未知的游戏";
	m_ResourcePath = "";
	m_ResourceOutputPath = "";

	CRegKey		reg;
	LPCTSTR lp="Software\\3D_TOOL";
	reg.Create( HKEY_CURRENT_USER, lp,"");

	if( reg.Open(HKEY_CURRENT_USER,lp) == ERROR_SUCCESS )
	{
		//获取值
		DWORD nCount = 1024;
		char*  buf = new char[1024];
		if( reg.QueryValue( buf,"ResourceWorkPath", &nCount ) == ERROR_SUCCESS )
		{
			m_ResourcePath = buf;
		}
		else
		{
			reg.SetValue((LPCSTR)m_ResourcePath,"ResourceWorkPath");
		}

		if( reg.QueryValue( buf,"ResourceOutputPath", &nCount ) == ERROR_SUCCESS )
		{
			m_ResourceOutputPath = buf;
		}
		else
		{
			reg.SetValue((LPCSTR)m_ResourceOutputPath,"ResourceOutputPath");
		}


		delete buf;
	
	}
	else
	{
		AfxMessageBox("注册表打开失败！");
	}
	reg.Close();
	/*
		设置列表项
	*/
	m_3DInfoList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_3DInfoList.InsertColumn(0,_T("对象ID"),LVCFMT_IMAGE|LVCFMT_LEFT);
	m_3DInfoList.InsertColumn(1,_T("模型名称"));
	m_3DInfoList.InsertColumn(2,_T("URL"));
	m_3DInfoList.SetColumnWidth(0 ,50);
	m_3DInfoList.SetColumnWidth(1 ,100);
	m_3DInfoList.SetColumnWidth(2 ,300);

	m_nDataIndex = 0;

	m_CurDataNode = m_DataMap.end();
	m_nSelectIndex = -1;

    /*
		加载本地信息，emmoa.ini、data.ini、Adinfo.xml
	*/
	m_MeshCopyMap.clear();

	/*
		加载emmoa.ini,如果没有找到这个文件这个工具将不起作用
	*/
	char NameINI[MAX_PATH];
	strcpy( NameINI, "emmoa.ini" );
	GetAbsPath(NameINI);
	GetIni()->Attach(NameINI);
	int  game_id = 0;
	GetIni()->GetInt("common","GameID",game_id,0 );
	if( game_id == IGA_SHENQI )
	{
		m_GameName = "神泣";
	}
	else if( game_id == IGA_WUJIEQU )	
	{
		m_GameName = "舞街区（国服）";
	}
	else if( game_id == IGA_WUJIEQU_MLXY )
	{
		m_GameName = "舞街区（马来西亚）";	
	}
	else if( game_id == IGA_WUJIEQU_XJP )
	{
		m_GameName = "舞街区（新加坡）";	
	}
	else if( game_id == IGA_WUJIEQU_FLB )
	{
		m_GameName = "舞街区（菲律宾）";			
	}
	else if( game_id == IGA_WLOL )
	{
		m_GameName = "武林群侠传";			
	}
	else if( game_id == IGA_GV )
	{
		m_GameName = "大航海时代";			
	}
	else if( game_id == IGA_SUN )
	{
		m_GameName = "奇迹世界";			
	}
	else if( game_id == IGA_RELIPAIQIU )
	{
		m_GameName = "热力排球";			
	}
	else if( game_id == IGA_PAOPAOKADINGCHE )
	{
		m_GameName = "跑跑卡丁车";			
	}
	else if( game_id == IGA_TIANZUN )
	{
		m_GameName = "天尊";			
	}
	else if( game_id == IGA_CHAOJIWUZHE )
	{
		m_GameName = "超级舞者";
	}
	else if(game_id == IGA_FENGYICHUANSHUO)
	{
		m_GameName = "封印传说";
	}
	else if ( game_id == IGA_SHUSHAN )
	{
		m_GameName = "蜀山";
	}
	else if ( game_id == IGA_TEST )
	{
		m_GameName = "测试游戏";
	}
	else
	{
		m_ButtonSet.EnableWindow(0);
		m_3DInfoList.EnableWindow(0);
		_bInit = false;
	}


	//...
	if( _bInit && !m_ResourcePath.IsEmpty() && !m_ResourceOutputPath.IsEmpty() )
	{
		SetEnable(true);
		/*
			寻找资源，并填充到添加对话框中
		*/
		int num = m_AddModifictionDlg.FindResource(m_ResourcePath);
	    
		/*
			创建资源输出目录,新路径会被创建
		*/
		BOOL bCreatePath = mkdirEx(m_ResourceOutputPath);
		if( bCreatePath == TRUE )
		{
			UpdateData(FALSE);
			SetEnable(true);
		}
	}
	else
	{
		SetEnable(false);
	}






	UpdateData(FALSE);


	SetTimer(500,500,NULL);

	g_pMainDlg = this;
	
	return FALSE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CD_TOOLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CD_TOOLDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CD_TOOLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CD_TOOLDlg::OnBnClickedButtonSet()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SetDlg.SetPath(m_ResourcePath,m_ResourceOutputPath);
	INT_PTR Key = m_SetDlg.DoModal();
	
	m_ResourcePath = m_SetDlg.GetResourcePath();
	m_ResourceOutputPath = m_SetDlg.GetResourceOutputPath();
	if( m_ResourcePath.IsEmpty() || m_ResourceOutputPath.IsEmpty() )
	{
		AfxMessageBox("工作路径未设置完成！");
		return;
	}

	if( Key == IDOK )
	{
		//添加到注册表
		CRegKey		reg;
		LPCTSTR lp="Software\\3D_TOOL";
		if( reg.Open(HKEY_CURRENT_USER,lp) == ERROR_SUCCESS )
		{
			//获取值
			reg.SetValue((LPCSTR)m_ResourcePath,"ResourceWorkPath");
			reg.SetValue((LPCSTR)m_ResourceOutputPath,"ResourceOutputPath");
		}
		else
		{
			AfxMessageBox("注册表打开失败！");
		}
		reg.Close();

		/*
			寻找资源，并填充到添加对话框中
		*/
		int num = m_AddModifictionDlg.FindResource(m_ResourcePath);
	    
		/*
			创建资源输出目录,新路径会被创建
		*/
		BOOL bCreatePath = mkdirEx(m_ResourceOutputPath);
		if( bCreatePath == TRUE )
		{
			UpdateData(FALSE);
			SetEnable(true);
		}
	}

}

void CD_TOOLDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_AddModifictionDlg.SetCurData(S3DDATA());

	INT_PTR Key = m_AddModifictionDlg.DoModal();
	
	if( Key == IDOK )
	{
		S3DDATA	data = m_AddModifictionDlg.Get3DData();
		//检查是不是已经添加过的数据
		int key = data.obj_id;
		LPNODE p = m_DataMap.find(key);
		if( p == m_DataMap.end() )
		{
			data.obj_id  = ++m_nDataIndex;
			int nCount = m_3DInfoList.GetItemCount();
			CString str(_T(""));
			str.Format( "%d", data.obj_id );
			m_3DInfoList.InsertItem( nCount,"" );
			m_3DInfoList.SetItemText( nCount, 0, str );
			str.Format( "%s", data.mesh_name );
			m_3DInfoList.SetItemText( nCount, 1, str);
			str.Format( "%s", data.url );
			m_3DInfoList.SetItemText( nCount, 2, str);

			/*
				添加到对象列表中		
			*/
			m_DataMap.insert( DATA_NODE(data.obj_id,data) );
			m_nSelectIndex = nCount;

		}
		else
		{
			CString str(_T(""));
			str.Format( "%d", m_CurDataNode->second.obj_id );
			m_3DInfoList.SetItemText( m_nSelectIndex, 0, str );
			str.Format( "%s", m_CurDataNode->second.mesh_name );
			m_3DInfoList.SetItemText( m_nSelectIndex, 1, str);
			str.Format( "%s", m_CurDataNode->second.url );
			m_3DInfoList.SetItemText( m_nSelectIndex, 2, str);
		}

		UpdateData(FALSE);

		/*
			将需要的或新增加的资源文件复制到InterAdtive\EMMOA\3d下
			并重新生成配置文件
		*/
		UpdateDataINI();
		UpdataAdInfoXML();

		//通知客户端更新
      	UpdateClient();	
	}

}

void CD_TOOLDlg::SetEnable(bool bIsEnable )
{
	if( bIsEnable )
	{
		m_ButtonAdd.EnableWindow();
		m_ButtonDelete.EnableWindow();
		m_ButtonClear.EnableWindow();
		m_ButtonOutput.EnableWindow();
	}
	else
	{
		m_ButtonAdd.EnableWindow(0);
		m_ButtonDelete.EnableWindow(0);
		m_ButtonClear.EnableWindow(0);
		m_ButtonOutput.EnableWindow(0);
	}
}

void CD_TOOLDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(500);
	Delete3DRes();
	CDialog::OnOK();
}

void CD_TOOLDlg::OnNMRclickList3dinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	/*
		寻找到被选择的数据节点，并打开 添加\修改对话框进行修改
	*/
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_nSelectIndex = pNMLV->iItem;
	if( m_nSelectIndex > -1 )
	{  
		CString mesh_name = m_3DInfoList.GetItemText( m_nSelectIndex, 0 );
		int _Index = _ttoi(mesh_name);
		m_CurDataNode = m_DataMap.find(_Index);

		m_AddModifictionDlg.SetCurData(m_CurDataNode->second);

		INT_PTR key = m_AddModifictionDlg.DoModal();
		if( key == IDOK )
		{
			m_CurDataNode->second = m_AddModifictionDlg.Get3DData();

			CString str(_T(""));
			str.Format( "%d", m_CurDataNode->second.obj_id );
			m_3DInfoList.SetItemText( m_nSelectIndex, 0, str );
			str.Format( "%s", m_CurDataNode->second.mesh_name );
			m_3DInfoList.SetItemText( m_nSelectIndex, 1, str);
			str.Format( "%s", m_CurDataNode->second.url );
			m_3DInfoList.SetItemText( m_nSelectIndex, 2, str);
		}
	}
	UpdateData(FALSE);

 	/*
		将需要的或新增加的资源文件复制到InterAdtive\EMMOA\3d下
		并重新生成配置文件
	*/
	UpdateDataINI();
	UpdataAdInfoXML();

	//通知客户端更新
    UpdateClient();	

	*pResult = 0;
}

void CD_TOOLDlg::OnNMClickList3dinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	/*
		寻找到被选择的数据节点
	*/
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_nSelectIndex = pNMLV->iItem;
	if( m_nSelectIndex > -1 )
	{  
		CString mesh_name = m_3DInfoList.GetItemText( m_nSelectIndex, 0 );
		int _Index = _ttoi(mesh_name);
		m_CurDataNode = m_DataMap.find(_Index);

	}

	*pResult = 0;
}

void CD_TOOLDlg::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_nSelectIndex == -1 )
	{
		AfxMessageBox("没有数据被选择！");
		return;
	}

	INT_PTR key = AfxMessageBox("确实要移除这条数据吗?", MB_OKCANCEL ); 
	if( key == IDOK )
	{
		m_3DInfoList.DeleteItem(m_nSelectIndex);
		if( m_CurDataNode != m_DataMap.end() )
		{
			m_DataMap.erase(m_CurDataNode);
		}
		m_CurDataNode = m_DataMap.end();
		m_nSelectIndex = -1;
	}

	/*
		将需要的或新增加的资源文件复制到InterAdtive\EMMOA\3d下
		并重新生成配置文件
	*/
	UpdateDataINI();
	UpdataAdInfoXML();

	//通知客户端更新
    UpdateClient();	

}

void CD_TOOLDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_DataMap.empty() )
	{
		AfxMessageBox("列表中没有信息！");
		return;
	}

	INT_PTR key = AfxMessageBox("确实要删除所有信息吗？", MB_OKCANCEL );
	if( key == IDCANCEL )
	{
		m_CurDataNode = m_DataMap.end();
		m_nSelectIndex = -1;
		return;
	}

	m_DataMap.clear();
	m_CurDataNode = m_DataMap.end();

	m_3DInfoList.DeleteAllItems();
	m_nSelectIndex = -1;

	m_AddModifictionDlg.SetCurData(S3DDATA());

	/*
		将需要的或新增加的资源文件复制到InterAdtive\EMMOA\3d下
		并重新生成配置文件
	*/
	UpdateDataINI();
	UpdataAdInfoXML();

	//通知客户端更新
    UpdateClient();	
}

void CD_TOOLDlg::OnBnClickedButtonOutput()
{
	//将使用的资源复制到指定目录下
	for( LPCOPYNODE pNode = m_MeshCopyMap.begin(); pNode != m_MeshCopyMap.end(); pNode++ )
	{
		SFileCopy&  fileCopy = pNode->second;
		/*
			创建输出目录，按模型文件创建，将每个模型的所有资源放到这个文件夹里。
		*/
		CString distPath = m_ResourceOutputPath + CString("\\");
		CString SourceMesnName = pNode->first;
		if( !(SourceMesnName.Right(2) == ".x" || SourceMesnName.Right(2) == ".X"))
		{
			continue;
		}

		char _name[MAX_PATH];
		int num = SourceMesnName.GetLength()-2;
		char* _p = SourceMesnName.GetBuffer();
		memcpy( _name, _p, num );
		_name[num] = '\0';
		distPath += _name;

		BOOL bIsCreatPath = mkdirEx(distPath);
		if( bIsCreatPath == FALSE )
		{
			continue;
		}

		distPath += "\\";
		
		//复制模型文件操作
		CString SoutcePath = m_AddModifictionDlg.GetMeshPath(SourceMesnName);
		CString nameSource = SoutcePath + SourceMesnName;
		CString nameDist = distPath + SourceMesnName;
		BOOL succeed = CopyFile( nameSource, nameDist, true );
		if( succeed == TRUE )
		{
			//复制使用的纹理
			CStringList& TexList = fileCopy.NameList;
			POSITION   pos = TexList.GetHeadPosition();
			while( pos != NULL )
			{
				CString str =  TexList.GetNext(pos);
				CString texSource = SoutcePath + str; 
				CString texDist   = distPath + str;
				CopyFile( texSource, texDist, true );
			}
		}
	}

	//--------------------------------------
	/*
		将设置的信息保存到文本文件	
	*/
	//CFileDialog fileDlg( 0, "*.txt", m_GameName + ".txt", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "*.txt" );

	//INT_PTR key = fileDlg.DoModal();
	//if( key == IDCANCEL )
	//{
	//	return;
	//}

	//CString name = fileDlg.GetPathName();

	//char* pName = name.GetBuffer();

	char text[MAX_PATH];
	strcpy( text, m_ResourceOutputPath.GetBuffer() );
	m_ResourceOutputPath.ReleaseBuffer();
	
	CString pathName = m_ResourceOutputPath + "\\" + m_GameName + ".txt";
	char* pName = pathName.GetBuffer();
	ofstream out;
	out.open(pName);
	if( out.is_open() )
	{
		int num = (int)m_DataMap.size();
		int i = 1;
		out<<"一共添加了"<<num<<"条数据,具体如下："<<endl;
		for( LPNODE p = m_DataMap.begin(); p != m_DataMap.end(); ++p )
		{
			S3DDATA& data = p->second;
			
			out<<"------------ "<<i<<" -----------"<<endl;
			out<<"模型名称："<<data.mesh_name<<endl;
			
			out<<"连接："<<endl;
			out<<"	url: "<<data.url<<endl;

			out<<"位置："<<endl;
			out<<"	x: "<<data.x<<endl;
			out<<"	y: "<<data.y<<endl;
			out<<"	z: "<<data.z<<endl;

			out<<"等比缩放："<<endl;
			out<<"	s_xyz: "<<data.scale_xyz<<endl;

			out<<"旋转："<<endl;
			out<<"	Rot_x: "<<data.rot_x/3.14*180<<endl;
			out<<"	Rot_y: "<<data.rot_y/3.14*180<<endl;
			out<<"	Rot_z: "<<data.rot_z/3.14*180<<endl;
			
			i++;
		}
		out.close();		
	}
	pathName.ReleaseBuffer();
	
	INT_PTR Key = AfxMessageBox("已经将3D信息及使用的资源保存到输出目录。\n\n需要打开输出目录吗？", MB_OKCANCEL );
	if( Key == IDOK )
	{
		ShellExecute( this->GetSafeHwnd(), "open", m_ResourceOutputPath, NULL, NULL, SW_SHOW );
	}
}

void CD_TOOLDlg::UpdataCurList(S3DDATA& data)
{
	//检查是不是已经添加过的数据
	int key = data.obj_id;
	LPNODE p = m_DataMap.find(key);
	if( p == m_DataMap.end() )
	{
		INT_PTR _key = AfxMessageBox("需要添加这条数据吗？", MB_OKCANCEL );
		if( _key == IDCANCEL )
		{
			return;
		}
		else if( _key == IDOK )
		{
			data.obj_id  = ++m_nDataIndex;
			int nCount = m_3DInfoList.GetItemCount();
			CString str(_T(""));
			str.Format( "%d", data.obj_id );
			m_3DInfoList.InsertItem( nCount,"" );
			m_3DInfoList.SetItemText( nCount, 0, str );
			str.Format( "%s", data.mesh_name );
			m_3DInfoList.SetItemText( nCount, 1, str);
			str.Format( "%s", data.url );
			m_3DInfoList.SetItemText( nCount, 2, str);

			/*
				添加到对象列表中		
			*/
			m_DataMap.insert( DATA_NODE(data.obj_id,data) );

			m_CurDataNode = m_DataMap.find(data.obj_id);
			m_nSelectIndex = nCount;
		}
	}
	else	//是要修改的数据
	{
		m_CurDataNode->second = data;
	}
    
	/*
		将需要的或新增加的资源文件复制到InterAdtive\EMMOA\3d下
		这里应该有一个检查机制，减少重复文件的绘制
	*/
	//保存数据到文件
	UpdateDataINI();
	UpdataAdInfoXML();



	/*
		更新列表框
	*/
	if( m_CurDataNode != m_DataMap.end() )
	{
		CString str(_T(""));
		str.Format( "%d", m_CurDataNode->second.obj_id );
		m_3DInfoList.SetItemText( m_nSelectIndex, 0, str );
		str.Format( "%s", m_CurDataNode->second.mesh_name );
		m_3DInfoList.SetItemText( m_nSelectIndex, 1, str);
		str.Format( "%s", m_CurDataNode->second.url );
		m_3DInfoList.SetItemText( m_nSelectIndex, 2, str);
	}

	UpdateData(FALSE);

	//g_IGA_Date[0].bIsReload = true;
	//g_IGA_Date.Update();


	UpdateClient();
	//g_IGA_Date[0].bIsReload = true;

}

void CD_TOOLDlg::UpdateDataINI()
{
	char WorkFullPath[MAX_PATH] = {0};
	GetAbsPath(WorkFullPath);
	CString WorkResPath = WorkFullPath;
	WorkResPath  += "InterAdtive\\EMMOA\\3d\\";

	//创建这个路径，如果没有的话
	BOOL bIsCreate = mkdirEx(WorkResPath);
	if( bIsCreate == FALSE )
	{
		return;
	}

	/*
		将模型文件复制到InterAdtive\EMMOA\3d下，并根据对象ID改名(对于重复使用的模型会复制多次)
	*/
	//将当前列表中使用的资源名称进行统计
	for( LPNODE p = m_DataMap.begin(); p != m_DataMap.end(); p++ )
	{
		CString name = p->second.mesh_name;

		LPCOPYNODE pNode = m_MeshCopyMap.find(name);
		if( pNode == m_MeshCopyMap.end() )
		{
			SFileCopy  fileCopy;
			fileCopy.mesh_id = SFileCopy::nCount;
			p->second.mesh_id = SFileCopy::nCount;
			SFileCopy::nCount++;
			fileCopy.MeshName.Format("m_%d.x",fileCopy.mesh_id); 
			GetMeshResNameList( name, &fileCopy.NameList );
			m_MeshCopyMap.insert(COPY_NODE(name,fileCopy));
		}
		else
		{
			p->second.mesh_id = pNode->second.mesh_id;
		}
	}

	//-------------------------------------------------------
	//将使用的资源复制到指定目录下
	for( LPCOPYNODE pNode = m_MeshCopyMap.begin(); pNode != m_MeshCopyMap.end(); pNode++ )
	{
		SFileCopy&  fileCopy = pNode->second;
		if( !fileCopy.bIsCopy )
		{
			//复制模型文件操作
			CString SourceMesnName = pNode->first;
			CString SoutcePath = m_AddModifictionDlg.GetMeshPath(SourceMesnName);
			CString nameSource = SoutcePath + SourceMesnName;
			CString nameDist = WorkResPath + fileCopy.MeshName;
			BOOL succeed = CopyFile( nameSource, nameDist, true );
			if( succeed == TRUE )
			{
				//复制使用的纹理
				CStringList& TexList = fileCopy.NameList;
				POSITION   pos = TexList.GetHeadPosition();
				while( pos != NULL )
				{
					CString str =  TexList.GetNext(pos);
					CString texSource = SoutcePath + str; 
					CString texDist   = WorkResPath + str;
					CopyFile( texSource, texDist, true );
				}
			}

			//复制过的文件打上标记
			fileCopy.bIsCopy = true;
		}
	}

	//-----------------------------------------------------
	/*
		保存的到data.ini
	*/

	setlocale(LC_ALL,"Chinese-simplified");

	char NameINI[MAX_PATH];
	CString csrtINI = WorkResPath + "data.ini";
	strcpy( NameINI, csrtINI.GetBuffer() );
	csrtINI.ReleaseBuffer();

	ofstream out;
	out.open( NameINI );
	if( out.is_open() )
	{
		out<<"[comm]"<<endl;
		out<<"gameid = "<<4<<"	;"<<m_GameName<<endl;;
		out<<"list = ";

		for( LPNODE p = m_DataMap.begin(); p != m_DataMap.end(); p++ )
		{
			out<<p->second.obj_id<<",";
		}
		out<<endl<<endl;

		for( LPNODE p = m_DataMap.begin(); p != m_DataMap.end(); p++ )
		{
			S3DDATA& data = p->second; 
			out<<"["<<data.obj_id<<"]"<<endl;
			out<<"mesh_id = "<<data.mesh_id<<endl;
			out<<"x = "<<data.x<<endl;
			out<<"y = "<<data.y<<endl;
			out<<"z = "<<data.z<<endl;
			out<<"s_xyz = "<<data.scale_xyz<<endl;
			out<<"r_x = "<<data.rot_x<<endl;
			out<<"r_y = "<<data.rot_y<<endl;
			out<<"r_z = "<<data.rot_z<<endl;
			out<<"url = "<<data.url<<endl<<endl;	
		}

		out.close();	
	}

}

void CD_TOOLDlg::UpdataAdInfoXML()
{



}

void CD_TOOLDlg::GetMeshResNameList(CString name, CStringList* pCStringList)
{
	try
	{
		CString cstrResoutcePath = m_AddModifictionDlg.GetMeshPath(name);
		char path[MAX_PATH];
		strcpy( path, cstrResoutcePath.GetBuffer() );
		cstrResoutcePath.ReleaseBuffer();
		strcat( path, "*.*");
		_finddata_t file;
		long lf;
		if((lf = (long)_findfirst(path,&file))==-1l)//相对文件路径
			/*cout<<"文件没有找到!\n"*/;
		else
		{
			do
			{
			// cout<<file.name;
				if(file.attrib == _A_ARCH)
				{
					/*cout<<"  普通文件  "*/;
					CString _name = file.name;

					if( _name == name )
					{
						continue;
					}
					pCStringList->AddHead(_name);                
				}
			}
			while( _findnext( lf, &file ) == 0 );
		}
		_findclose(lf);
	}
	catch(...)
	{
		//OutputdebugString("查找文件时异常");
	}
}

void CD_TOOLDlg::Delete3DRes( bool bIsAll, CString name )
{
	char WorkFullPath[MAX_PATH] = {0};
	GetAbsPath(WorkFullPath);
	CString WorkResPath = WorkFullPath;
	WorkResPath  += "InterAdtive\\EMMOA\\3d\\";

	if( bIsAll )
	{
		CString dataINI = WorkResPath + "data.ini";
		BOOL bDelDataINI = DeleteFile(dataINI);

		for( LPCOPYNODE pNode = m_MeshCopyMap.begin(); pNode != m_MeshCopyMap.end(); pNode++ )
		{
			SFileCopy&  fileCopy = pNode->second;
		
			CString MeshName = WorkResPath + fileCopy.MeshName;
			BOOL bIsDel = DeleteFile( MeshName );

			CStringList& TexList = fileCopy.NameList;
			POSITION   pos = TexList.GetHeadPosition();
			while( pos != NULL )
			{
				CString str =  TexList.GetNext(pos);
				CString texDist   = WorkResPath + str;
				BOOL bDelTex = DeleteFile( texDist );
			}
		}
	}
	else
	{
		
	}
	m_DataMap.clear();
	m_MeshCopyMap.clear();

	UpdateClient();
}

void CD_TOOLDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData();
	g_IGA_Date[0].bIsShowPickMesh = m_bShowPickMesh;
	m_nCurPickID = g_IGA_Date[0].nPickObjID;
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}

void CD_TOOLDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//AfxMessageBox("qsdqweqw");


	OnBnClickedButtonQuit();
}
