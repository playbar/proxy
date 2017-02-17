// SetFormDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyIE.h"
#include "SetFormDataDlg.h"
#include "InputDlg.h"
#include "MainFrm.h"
#include "InputPassword.h"
#include "Crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetFormDataDlg dialog

CSetFormDataDlg::CSetFormDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetFormDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetFormDataDlg)
	m_bEncryptStringItem = FALSE;
	//}}AFX_DATA_INIT
}


void CSetFormDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetFormDataDlg)
	DDX_Control(pDX, IDC_URL, m_url);
	DDX_Control(pDX, IDC_AUTO_SUBMIT, m_AutoSubmit);
	DDX_Control(pDX, IDC_FORM, m_form);
	DDX_Check(pDX, IDC_ENCRYPT_STRING_ITEM, m_bEncryptStringItem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetFormDataDlg, CDialog)
	//{{AFX_MSG_MAP(CSetFormDataDlg)
	ON_BN_CLICKED(IDC_EDIT_ITEM, OnEditItem)
	ON_BN_CLICKED(IDC_DEL_URL, OnDelUrl)
	ON_NOTIFY(NM_DBLCLK, IDC_FORM, OnDblclkForm)
	ON_BN_CLICKED(IDC_DEL_URL_ALL, OnDelUrlAll)
	ON_BN_CLICKED(IDC_SET_MAIN_PWD, OnSetMainPwd)
	ON_BN_CLICKED(IDC_SET_FILL_PWD, OnSetFillPwd)
	ON_BN_CLICKED(IDC_AUTO_SUBMIT, OnAutoSubmit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_URL, OnItemchangedUrl)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(NM_DBLCLK, IDC_URL, OnDblclkUrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetFormDataDlg message handlers

BOOL CSetFormDataDlg::OnInitDialog() 
{
	LOADDLG("DialogFormData");
	CDialog::OnInitDialog();

	try{
	// TODO: Add extra initialization here
	m_form.SetExtendedStyle(m_form.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	//
	CRect rect;
	CString str;
	LOADSTR(str ,IDS_NAME);
	m_form.InsertColumn(0,str);
	m_form.GetWindowRect(rect);
	m_form.SetColumnWidth(0, 100);
	LOADSTR(str ,IDS_DATA);
	m_form.InsertColumn(1,str);
	m_form.SetColumnWidth(1, rect.Width()-130);
	//
	m_url.SetExtendedStyle(m_url.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	LOADSTR(str ,IDS_URL);
	m_url.InsertColumn(0,str);
	m_url.InsertColumn(1,"");//must do this for sort.
	m_url.SetColumnWidth(1, 0);
	m_url.GetWindowRect(rect);
	m_url.SetColumnWidth(0, rect.Width()-25);
	//
	_LoadFormUrl(&m_url);
	m_url.AddSortIndexData();

	}catch(...){}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetFormDataDlg::FillFormItem(int index)
{
	try{
	m_form.DeleteAllItems();
	//
	CString strUrl;
	strUrl = m_url.GetItemText(index, 0);
	char* tmp = _FileGetBuffer(theApp.m_strFormDataPath);
	if (tmp==NULL)
		return ;
	DWORD len = strlen(tmp);
	//
	char *head,*tail;
	char *pName;
	char *pData;
	int i=0;
	head = strstr(tmp, strUrl);
	if (!head)
		goto _MyExit;
	head = strstr(head,"\r\n");
	head += 2;
	while(1)
	{
		if (*head == NULL)
			break;
		else if (*head == '[')
			break;
		else
		{
			tail = strstr(head,"=");
			len = tail-head;
			pName = new char[len+1];
			STRNCPY(pName, head, len);
			//
			head = tail+1;
			tail = strstr(head,"\r\n");
			if (!tail)
			{
				delete[] pName;
				break;
			}
			len = tail-head;
			pData = new char[len+1];
			STRNCPY(pData, head, len);
			//
			if ( strcmp(pName, "MyIEFormFillPwd")!=0 && strcmp(pName, "MyIEAutoSubmit")!=0)
			{
				m_form.InsertItem(i,pName);
				m_form.SetItemText(i,1,pData);
			}
			delete[] pName;
			delete[] pData;
			//
			i++;
			head = tail+ 2;// \r\n
		}
	}
	//auto submit
	::GetPrivateProfileString(strUrl, "MyIEAutoSubmit", _T(""), tmp, 5, theApp.m_strFormDataPath);
	if (*tmp == '1')
		m_AutoSubmit.SetCheck(1);
	else
		m_AutoSubmit.SetCheck(0);

	m_form.AddSortIndexData();

_MyExit:
	free(tmp);
	}catch(...){}
}

void CSetFormDataDlg::OnDelUrl() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_url.GetSelectionMark();
	if (nCurSel<0)
		return;

	CString strUrl = m_url.GetItemText(nCurSel, 0);
	CString filename = theApp.m_strFormDataPath;
	WritePrivateProfileSection(strUrl, NULL, filename);
	//
	m_url.DeleteItem(nCurSel);
	int nCount = m_url.GetItemCount();
	if (nCount<=0)
		nCurSel = 0;
	else if (nCurSel>=nCount)
		nCurSel = nCount-1;
	LIST_FOCUS_POS(m_url,nCurSel);
}

void CSetFormDataDlg::OnEditItem() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_form.GetSelectionMark();
	if (nCurSel<0)
		return;

	CInputDlg dlg;
	CString strMsg;
	LOADSTR(strMsg ,IDS_INPUT_ITEM_DATA);
	dlg.m_strMsg = strMsg;
	//
	CString strData = m_form.GetItemText(nCurSel,1);
	dlg.m_strOutput = strData;
	
	if(dlg.DoModal() == IDOK)
	{
		CString filename = theApp.m_strFormDataPath;
		CString strUrl  = m_url.GetItemText(m_url.GetSelectionMark(), 0);
		CString strName = m_form.GetItemText(nCurSel, 0);
		::WritePrivateProfileString(strUrl, strName, dlg.m_strOutput, filename);
		//
		m_form.SetItemText(nCurSel,1,dlg.m_strOutput);
	}
}

void CSetFormDataDlg::OnSetFillPwd() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_url.GetSelectionMark();
	if (nCurSel<0)
		return;

	CInputPassword dlg;
	dlg.m_bRemPsw = false;
	if (dlg.DoModal()==IDOK)
	{
		CString strEncode;
		Encoding2(dlg.m_strPassword, strEncode);

		CString filename = theApp.m_strFormDataPath;
		CString strUrl = m_url.GetItemText(nCurSel,0);
		::WritePrivateProfileString(strUrl, "MyIEFormFillPwd", strEncode, filename);
	}
}

void CSetFormDataDlg::OnDblclkForm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEditItem();
	*pResult = 0;
}

void CSetFormDataDlg::OnDelUrlAll() 
{
	// TODO: Add your control notification handler code here
	if(MSGBOX(IDS_CONFIRM_DELALL_FORMDATA, MB_YESNO|MB_ICONQUESTION)==IDNO)
		return;
	pmf->PostMessage(WM_COMMAND, ID_TOOL_CLEANMyIEFORM);
	m_url.DeleteAllItems();
	m_form.DeleteAllItems();
}

void CSetFormDataDlg::OnSetMainPwd() 
{
	// TODO: Add your control notification handler code here
	CInputPassword dlg;
	dlg.m_bRemPsw = false;
	if (dlg.DoModal()==IDOK)
	{
		CString strEncode;
		Encoding2(dlg.m_strPassword, strEncode);
		if (pmf->m_strFormMainPwd != strEncode)
		{
			CWinApp* app = AfxGetApp();
			app->WriteProfileString("Settings", "FormMainPwd", strEncode);
			pmf->m_strFormMainPwd = strEncode;
		}
	}
}

void CSetFormDataDlg::OnAutoSubmit() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_url.GetSelectionMark();
	if (nCurSel<0)
		return;

	CString filename = theApp.m_strFormDataPath;
	CString strUrl = m_url.GetItemText(nCurSel,0);
	if (m_AutoSubmit.GetCheck())
		::WritePrivateProfileString(strUrl, "MyIEAutoSubmit", "1", filename);
	else
		::WritePrivateProfileString(strUrl, "MyIEAutoSubmit", "0", filename);
}

void CSetFormDataDlg::OnItemchangedUrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int i=-1, nIndex = pNMListView->iItem;
	POSITION pos = m_url.GetFirstSelectedItemPosition();
	if(pos!=NULL)
		i = m_url.GetNextSelectedItem(pos);
	if(nIndex==i)
	{
		FillFormItem(nIndex);
	}

	*pResult = 0;
}

void CSetFormDataDlg::OnDblclkUrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_url.GetSelectionMark();
	if (nCurSel<0)
		return;

	CInputDlg dlg;
	LOADSTR(dlg.m_strMsg  ,IDS_URL);
	dlg.m_strOutput = m_url.GetItemText(nCurSel,0);
	if (dlg.DoModal()==IDOK)
		pmf->NewChildWindow(1,2,dlg.m_strOutput);

	*pResult = 0;
}
