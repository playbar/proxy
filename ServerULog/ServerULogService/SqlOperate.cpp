
#include "SqlOperate.h"
#include <atlstr.h>

CSqlOperate::CSqlOperate() : m_pConnection(NULL)
{
	::CoInitialize(NULL);
}

CSqlOperate::~CSqlOperate()
{
	if ( NULL != m_pConnection )
	{
		delete m_pConnection;
		m_pConnection = NULL;
	}
	::CoUninitialize();
}


bool CSqlOperate::ConnectSQLDatabase(LPSTR szServerName, LPSTR szDatabase, LPSTR szUseName, LPSTR szPass )
{
	char szSqlConnect[256];
	sprintf(szSqlConnect, "Provider=SQLOLEDB;Data Source=%s;Initial Catalog=%s;User Id=%s;Password=%s",szServerName, szDatabase, szUseName, szPass);
	//OutputDebugString(szSqlConnect );
	::CoInitialize(NULL);
	m_pConnection = new _ConnectionPtr();
	(*m_pConnection).CreateInstance(__uuidof(Connection));

	try
	{
		(*m_pConnection)->Open((LPCSTR)szSqlConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		return false;
	}
	//OutputDebugString("End ConnectSQLDatabase");
	return true;
}

bool CSqlOperate::CloseSqlDatabase()
{
	if((*m_pConnection)->GetState() ==  adStateOpen)   
		(*m_pConnection)->Close();

	return false;
}
bool CSqlOperate::Update(LPCSTR szSqlUpdata)
{
	try
	{
		_CommandPtr	pCommand;
		pCommand.CreateInstance(__uuidof(Command));   
		pCommand->ActiveConnection = (*m_pConnection);
		pCommand->CommandText = szSqlUpdata;
		pCommand->Execute(NULL,NULL,adCmdText);
	}
	catch(_com_error e)
	{
		//char * p = e.Description();
	}

	return false;
}

bool CSqlOperate::Delete(LPCSTR szSqlDele)
{
	try
	{
		_CommandPtr	pCommand;
		pCommand.CreateInstance(__uuidof(Command));   
		pCommand->ActiveConnection = (*m_pConnection);
		pCommand->CommandText = szSqlDele;
		pCommand->Execute(NULL,NULL,adCmdText);
	}
	catch(_com_error e)
	{
		//char * p = e.Description();
		return false;
	}

	return true;
}

bool CSqlOperate::InsertInto(LPCSTR szSqlInsert)
{
	try
	{
		_CommandPtr	pCommand;
		pCommand.CreateInstance(__uuidof(Command));   
		pCommand->ActiveConnection = (*m_pConnection);
		pCommand->CommandText = szSqlInsert;
		pCommand->Execute(NULL,NULL,adCmdText);
	}
	catch(_com_error e)
	{
		//char * p = e.Description();
		return false;
	}
	return true;
}


_RecordsetPtr CSqlOperate::SelectData(LPCSTR szSqlSelect)
{
	_RecordsetPtr pRecordset;
	try
	{
		pRecordset.CreateInstance(__uuidof(Recordset));
		pRecordset->Open(szSqlSelect,(*m_pConnection).GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		
		//CString failedTime,hostName,hostIP,url,manager,mobile,status;
		//_variant_t var; 
		//while(!pRecordset->adoEOF)
		//{
		//	failedTime.Empty();
		//	hostName.Empty();
		//	hostIP.Empty();
		//	url.Empty();
		//	manager.Empty();
		//	mobile.Empty();
		//	status.Empty();
		//	var = pRecordset->GetCollect("failedTime");
		//	if(var.vt != VT_NULL)
		//		failedTime = (LPCSTR)_bstr_t(var);

		//	var = pRecordset->GetCollect("hostName");
		//	if(var.vt != VT_NULL)
		//		hostName = (LPCSTR)_bstr_t(var);

		//	var = pRecordset->GetCollect("hostIP");
		//	if(var.vt != VT_NULL)
		//		hostIP = (LPCSTR)_bstr_t(var);

		//	var = pRecordset->GetCollect("status");
		//	if(var.vt != VT_NULL)
		//		status = (LPCSTR)_bstr_t(var);

		//	var = pRecordset->GetCollect("netAddress");
		//	if(var.vt != VT_NULL)
		//		url = (LPCSTR)_bstr_t(var);

		//	var = pRecordset->GetCollect("manager");
		//	if(var.vt != VT_NULL)
		//		manager = (LPCSTR)_bstr_t(var);

		//	var = pRecordset->GetCollect("mobilePhone");
		//	if(var.vt != VT_NULL)
		//		mobile = (LPCSTR)_bstr_t(var);

		//	//insertList(failedTime,hostName,hostIP,url,status,manager,mobile);

		//	pRecordset->MoveNext();
		//}
		//pRecordset->Close();
	}
	catch(_com_error e)
	{
		//AfxMessageBox("数据库出现问题，不能访问！具体原因："+e.Description());
		return NULL;
	}
	return pRecordset;
}
