// CSQLConnection.cpp: implementation of the CSQLConnection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CSQLConnection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  VarCharSize 50

CSQLConnection::CSQLConnection()
{
//	try
//	{
//		::CoInitialize(NULL);
//		this->m_pConnection	= new CONNECTION();
//		this->m_pRecordset	= new RECORDSET();
//		this->m_pCommand	= new COMMAND();
//		this->m_nTimeout	= 10;
//	}
//
//	catch (_com_error e) 
//	{
//		AfxMessageBox(e.ErrorMessage());
//	}
//	catch (...) 
//	{
//		DWORD RET = GetLastError();
//	}
}
void  CSQLConnection::Init()
{
	try
	{
		::CoInitialize(NULL);
		this->m_pConnection	= new CONNECTION_PTR();
		this->m_pRecordset	= new RECORDSET_PTR();
		this->m_pCommand	= new COMMAND_PTR();
		this->m_nTimeout	= 10;
	}

	catch (_com_error e) 
	{
		AfxMessageBox(e.ErrorMessage());
	}
	catch (...) 
	{
		DWORD RET = GetLastError();
	}
}

CSQLConnection::CSQLConnection(void *p)
{
	::CoInitialize(NULL);
	this->m_pConnection	= new CONNECTION_PTR();
	this->m_pRecordset	= new RECORDSET_PTR();
	this->m_pCommand	= new COMMAND_PTR();
	this->m_nTimeout	= 10;
}

CSQLConnection::~CSQLConnection()
{
	try
	{
		this->Close();
		m_param1 = NULL;
		m_param2 = NULL;
		m_param3 = NULL;
		m_param4 = NULL;
		m_param5 = NULL;
		m_param6 = NULL;
		m_param7 = NULL;
		SAFE_DELETE(m_pCommand);
		SAFE_DELETE(m_pRecordset);
		SAFE_DELETE(m_pConnection);
		//::CoUninitialize();
	}
	
	catch (_com_error e) 
	{
		
	}
	catch (...) 
	{
		DWORD RET = GetLastError();
	}
}


BOOL CSQLConnection::CreateCommand(LPCSTR lpszSPName)
{
	PROTECT(CSQLConnection::CreateCommand)

	ASSERT(m_pConnection != NULL);

	SAFE_DELETE(m_pCommand);

	m_pCommand			= new COMMAND_PTR();
	HRESULT	hr	= (*m_pCommand).CreateInstance(__uuidof(Command));
	if (!SUCCEEDED(hr))
	{
		DBGONLY(_T("%s Failed to CCommand::CreateInstance for error: %d!"), __FUNC_NAME__, ERR_CODE);
		return	FALSE;
	}
	///	初始化命令的参数设置
	(*m_pCommand)->ActiveConnection	= (*m_pConnection);
	(*m_pCommand)->CommandTimeout	= m_nTimeout == 0 ? 5 : m_nTimeout;
	(*m_pCommand)->CommandType		= adCmdStoredProc;
	(*m_pCommand)->CommandText		= lpszSPName;

	return	TRUE;
	UNPROTECT
	return	FALSE;
}

int CSQLConnection::Connect(LPCSTR lpszDatabase, 
						 LPCSTR lpszHostName, 
						 LPCSTR lpszUserName, 
						 LPCSTR lpszPassword)
{
	PROTECT(CSQLConnection::Open)
	ASSERT(m_pConnection != NULL);
	ASSERT(m_pRecordset != NULL);

	//	创建智能指针实例化对象
	HRESULT	hr	= (*m_pConnection).CreateInstance("ADODB.Connection");
	if (!SUCCEEDED(hr))
	{
		DBGONLY(_T("Failed to CConnection::CreateInstance for error: %d!"), ERR_CODE);
		return	-1;
	}
	hr	= (*m_pRecordset).CreateInstance(__uuidof(Recordset));
	if (!SUCCEEDED(hr))
	{
		DBGONLY(_T("Failed to CRecordset::CreateInstance for error: %d!"), ERR_CODE);
		return	-1;
	}

	hr	= (*m_pCommand).CreateInstance(__uuidof(Command));
	if (!SUCCEEDED(hr))
	{
		DBGONLY(_T("Failed to CCommand::CreateInstance for error: %d!"), ERR_CODE);
		return	-1;
	}
	///	初始化命令的参数设置
	(*m_pCommand)->CommandTimeout	= m_nTimeout == 0 ? 5 : m_nTimeout;

	//	组装成连接字符串
	CHAR	szConnection[_MAX_PATH]	= { 0 };
	CombineDSNConnection(szConnection, _MAX_PATH - 1, lpszDatabase, lpszHostName, lpszUserName, lpszPassword);
	this->m_bConnected	= FALSE;
	//	打开数据库
	(*m_pConnection)->ConnectionTimeout	= m_nTimeout;

///	hr	= (*m_pConnection)->Open(szConnection, lpszUserName, lpszPassword, adModeUnknown);
	(*m_pConnection)->ConnectionString	= szConnection;
	(*m_pRecordset)->CursorType = adOpenStatic;
	(*m_pRecordset)->CursorLocation = adUseClient;
	(*m_pConnection)->CursorLocation=   adUseClient; 
	hr	= (*m_pConnection)->Open("","","",adModeUnknown);

	if (SUCCEEDED(hr))
	{
		m_bConnected	= TRUE;
		_snprintf((char*)m_Connection,_MAX_PATH,szConnection);
		_snprintf((char*)m_UserName,_MAX_PATH,lpszUserName);
		_snprintf((char*)m_Password,_MAX_PATH,lpszPassword);
	
		///	设置当前活动连接
		(*m_pCommand)->ActiveConnection	= (*m_pConnection);
		return	1;
	}
	UNPROTECT
	return	0;
}

DWORD CSQLConnection::CombineDSNConnection(LPSTR pszConnection, 
										   DWORD dwSize, 
										   LPCSTR lpszDBName,
										   LPCSTR lpszHostName,
										   LPCSTR lpszUserName,
										   LPCSTR lpszPassword)
{
	//////////////////////////////////////////////////////////////////////////
	//("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s",lpszPassword,lpszUserName,lpszDB,lpszHostName);
	//////////////////////////////////////////////////////////////////////////
	int	nRet	=
		::_snprintf(pszConnection, dwSize, "Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s",
lpszPassword, lpszUserName, lpszDBName,lpszHostName);

	////	::_snprintf(pszConnection, dwSize, "PROVIDER = %s; SERVER = %s; DATABASE = %s", "SQLOLEDB.1",
	//			lpszHostName, lpszDBName);
	return	nRet;
}

int CSQLConnection::Close()
{
	PROTECT(CSQLConnection::Close)
	ASSERT(m_pConnection != NULL);
	if (this->m_bConnected &&(*m_pConnection)->GetState()==adStateOpen)
	{
		(*m_pCommand)->ActiveConnection = NULL;
		(*m_pConnection)->Close();
		this->m_bConnected	= FALSE;
	}
	UNPROTECT
	return	1;
}

int	CSQLConnection::ExecuteSP(LPCSTR lpszName)
{
	PROTECT(CSQLConnection::ExecuteSP)

	ASSERT(m_pCommand != NULL && (*m_pCommand)!= NULL);

	_variant_t		vtResult;
	vtResult.vt		= VT_ERROR;
	vtResult.scode	= DISP_E_PARAMNOTFOUND;

	_variant_t	vtRowsAffected;

	int	nRet	= -1;

	(*m_pCommand)->CommandType	= adCmdStoredProc;
	(*m_pCommand)->CommandText	= lpszName;
	(*m_pConnection)->CursorLocation=   adUseClient;
	(*m_pRecordset)	= (*m_pCommand)->Execute(&vtRowsAffected, &vtResult, adCmdStoredProc);
	nRet	= 1;
	return	nRet;
	UNPROTECT
	return	0;
}


int CSQLConnection::ExecuteSQL(LPCSTR lpszCommandText)
{
	PROTECT(CSQLConnection::ExecuteSQL)
	ASSERT(m_pConnection != NULL);
	ASSERT((*m_pConnection) != NULL);
	int	nRet	= -1;
	ASSERT(m_pRecordset != NULL);
#if _DATABASE_VERSION_ >= 0x0002
	ASSERT(m_pCommand != NULL && (*m_pCommand)!= NULL);
	_variant_t	vtResult;
	vtResult.vt		= VT_ERROR;
	vtResult.scode	= DISP_E_PARAMNOTFOUND;
	_variant_t	vtRowsAffected;
	(*m_pCommand)->CommandType	= adCmdText;
	(*m_pCommand)->CommandText	= lpszCommandText;
	(*m_pRecordset)	= (*m_pCommand)->Execute(&vtRowsAffected, &vtResult, adCmdText);
#else
	(*m_pRecordset)	= (*m_pConnection)->Execute(lpszCommandText, NULL, 0);
#endif
	nRet	= 1;
	return	 nRet;
	UNPROTECT
	return	0;
}


int CSQLConnection::GetFieldValue(LPCSTR lpszFieldName, long& nValue)
{
	ASSERT(this->m_bConnected == TRUE && (*m_pRecordset) != NULL);
	if ( (*m_pRecordset)->BOF || (*m_pRecordset)->EndOfFile )
		return	0;
	_variant_t	vtValue		= (*m_pRecordset)->GetCollect(lpszFieldName);
	if ( vtValue.vt == VT_NULL )
		return	-1;
	nValue	= (long)vtValue;
	return	1;
}

int CSQLConnection::GetFieldValue(LPCSTR lpszFieldName, DWORD& dwValue)
{
	ASSERT(this->m_bConnected == TRUE && (*m_pRecordset) != NULL);
	if ( (*m_pRecordset)->BOF || (*m_pRecordset)->EndOfFile )
		return	0;
	_variant_t	vtValue		= (*m_pRecordset)->GetCollect(lpszFieldName);
	if ( vtValue.vt == VT_NULL )
		return	-1;
	dwValue	= (DWORD)(long)vtValue;
	return	1;
}


int CSQLConnection::GetFieldValue(LPCSTR lpszFieldName, LPSTR pszValue, DWORD dwSize)
{
	ASSERT(this->m_bConnected == TRUE && (*m_pRecordset) != NULL);
	if ( (*m_pRecordset)->BOF || (*m_pRecordset)->EndOfFile )
		return	0;
	_variant_t	vtValue		= (*m_pRecordset)->GetCollect(lpszFieldName);
	if ( vtValue.vt == VT_NULL )
		return	-1;
	lstrcpyA(pszValue, (char *)(_bstr_t)vtValue);
	return	1;
}

int CSQLConnection::GetFieldValue(LPCSTR lpszFieldName, BOOL& bValue)
{
	ASSERT(this->m_bConnected == TRUE && (*m_pRecordset) != NULL);
	if ( (*m_pRecordset)->BOF || (*m_pRecordset)->EndOfFile )
		return	0;
	_variant_t	vtValue		= (*m_pRecordset)->GetCollect(lpszFieldName);
	if ( vtValue.vt == VT_NULL )
		return	-1;
	bValue	= (BOOL)(long)vtValue;
	return	1;
}


BOOL CSQLConnection::IsEOF() const
{
	ASSERT(this->m_bConnected == TRUE && (*m_pRecordset) != NULL);
	return	(*m_pRecordset)->EndOfFile;
}

BOOL CSQLConnection::IsBOF()	const
{
	ASSERT(this->m_bConnected == TRUE && (*m_pRecordset) != NULL);
	return	(*m_pRecordset)->BOF;
}

BOOL CSQLConnection::MoveFirst()
{
	ASSERT(this->m_bConnected == TRUE && (*m_pRecordset) != NULL);
	return	(*m_pRecordset)->MoveFirst();
}

BOOL CSQLConnection::MoveNext()
{
	ASSERT(this->m_bConnected == TRUE && (*m_pRecordset) != NULL);
	return	(*m_pRecordset)->MoveNext();
}



BOOL CSQLConnection::Reconnect()
{
	PROTECT(CSQLConnection::Reconnect)
	this->Close();
	HRESULT	hr	= S_OK;
	///	初始化命令的参数设置
	(*m_pCommand)->CommandTimeout	= m_nTimeout == 0 ? 5 : m_nTimeout;
	//	打开数据库
	(*m_pConnection)->ConnectionTimeout	= m_nTimeout;
	hr	= (*m_pConnection)->Open(this->m_Connection, this->m_UserName, this->m_Password, adModeUnknown);
	if (SUCCEEDED(hr))
	{
		(*m_pCommand)->ActiveConnection	= (*m_pConnection);
		m_bConnected	= TRUE;
		return	TRUE;
	}
	UNPROTECT
	return	FALSE;
}

BOOL CSQLConnection::BeginTrans()
{
	(*m_pConnection)->IsolationLevel	= adXactChaos;
	(*m_pConnection)->Attributes		&=!(adXactCommitRetaining | adXactAbortRetaining);
	return	(*m_pConnection)->BeginTrans();
}

BOOL CSQLConnection::CommitTrans()
{
	return	SUCCEEDED((*m_pConnection)->CommitTrans());
}

BOOL CSQLConnection::RollbackTrans()
{
	return	SUCCEEDED((*m_pConnection)->RollbackTrans());
}



BOOL CSQLConnection::ExecuteSPAdInfo(DWORD dwGameId, LPCSTR lpszGameVersion, CHAR *lpszPosition)
{
	if (*m_pRecordset==NULL||*m_pCommand==NULL||*m_pConnection==NULL)
	{
		return FALSE;
	}
	
	(*m_pCommand)->Parameters->Refresh();
	(*m_pCommand)->ActiveConnection=(*m_pConnection);         //绑定连接      
	(*m_pCommand)->CommandText="H_PRO_GetGameADSource";   //存储过程名称
	(*m_pCommand)->CommandType=adCmdStoredProc; 
	
	
	
	(*m_pConnection)->CursorLocation=   adUseClient; 
	/************************************************************************/
	/* 初始化存储过程的参数                                                 */
	/************************************************************************/

	m_param1=(*m_pCommand)->CreateParameter(_bstr_t ("gameID"),adInteger,adParamInput,sizeof(int));
	m_param2=(*m_pCommand)->CreateParameter(_bstr_t ("GameSourceVersion"),adVarChar,adParamInput,VarCharSize);
	m_param3=(*m_pCommand)->CreateParameter(_bstr_t ("UserPose"),adVarChar,adParamInput,VarCharSize);
	m_param4=(*m_pCommand)->CreateParameter(_bstr_t ("Rows"),adInteger,adParamOutput,sizeof(int));
	
	(*m_pCommand)->Parameters->Append(m_param1);
	(*m_pCommand)->Parameters->Append(m_param2);
	(*m_pCommand)->Parameters->Append(m_param3);
	(*m_pCommand)->Parameters->Append(m_param4);
	
	
	m_param1->Value=_variant_t((long)dwGameId, VT_I4);
	m_param2->Value=lpszGameVersion;
	m_param3->Value=lpszPosition;
	(*m_pRecordset)=(*m_pCommand)->Execute(NULL,NULL,adCmdStoredProc);
	
	return TRUE;
}

BOOL CSQLConnection::ExecuteSPAdPath(DWORD dwGameId, LPCSTR lpszGameVersion, CHAR *lpszPosition)
{
	if (m_pRecordset==NULL||m_pCommand==NULL||m_pConnection==NULL)
	{
		return FALSE;
	}
	
	
	(*m_pCommand)->Parameters->Refresh();
	(*m_pCommand)->ActiveConnection=(*m_pConnection);         //绑定连接      
	(*m_pCommand)->CommandText="H_PRO_GetADSourcePath";   //存储过程名称
	(*m_pCommand)->CommandType=adCmdStoredProc; 
	(*m_pConnection)->CursorLocation =adUseClient;
	/************************************************************************/
	/* 初始化存储过程的参数   (*m_pCommand)                                              */
	/************************************************************************/

	
	m_param4=(*m_pCommand)->CreateParameter(_bstr_t ("GameID"),adInteger,adParamInput,sizeof(int));
	m_param5=(*m_pCommand)->CreateParameter(_bstr_t ("GameSourceVersion"),adVarChar,adParamInput,50);
	m_param6=(*m_pCommand)->CreateParameter(_bstr_t ("AreaCode"),adVarChar,adParamInput,20);
	
	(*m_pCommand)->Parameters->Append(m_param4);
	(*m_pCommand)->Parameters->Append(m_param5);
	(*m_pCommand)->Parameters->Append(m_param6);
	
	m_param4->Value=_variant_t((long)dwGameId, VT_I4);
	m_param5->Value=lpszGameVersion;
	m_param6->Value=lpszPosition;
	(*m_pRecordset)=(*m_pCommand)->Execute(NULL,NULL,adCmdStoredProc);
	
	return TRUE;
}

BOOL CSQLConnection::CalculateInstallCount( LPCSTR lpszDateTime)
{
	(*m_pCommand)->Parameters->Refresh();
	(*m_pCommand)->ActiveConnection=(*m_pConnection);         //绑定连接      
	(*m_pCommand)->CommandText="H_CalculateEveryDayInstallCount";   //存储过程名称
	(*m_pCommand)->CommandType=adCmdStoredProc; 
	
	
	
	(*m_pConnection)->CursorLocation=   adUseClient; 
	/************************************************************************/
	/* 初始化存储过程的参数                                                 */
	/************************************************************************/
	

	m_param1=(*m_pCommand)->CreateParameter(_bstr_t ("cDate"),adVarChar,adParamInput,VarCharSize);
	(*m_pCommand)->Parameters->Append(m_param1);
	m_param1->Value=lpszDateTime;

	(*m_pRecordset)=(*m_pCommand)->Execute(NULL,NULL,adCmdStoredProc);
	return TRUE;		
}
