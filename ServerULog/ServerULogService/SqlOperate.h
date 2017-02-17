/*
*使用ado连接
*/
#pragma once

//#include <afxdao.h>
#pragma warning (disable:4146)
#import "c:\\program files\\common files\\system\\ado\\msado15.dll" named_guids  rename("EOF","adoEOF") , rename("EOF","adoEOF")
#pragma warning(default:4146)
using namespace ADODB;

class CSqlOperate
{
public:
	CSqlOperate(void);
	~CSqlOperate(void);

public:
	bool ConnectSQLDatabase(LPSTR szServerName, LPSTR szDatabase, LPSTR szUseName, LPSTR szPass );
	bool CloseSqlDatabase();
	bool Update(LPCSTR szSqlUpdata);
	bool Delete(LPCSTR szSqlDele);
	bool InsertInto(LPCSTR szSqlInsert);
	_RecordsetPtr SelectData(LPCSTR szSqlSelect);

private:
	_ConnectionPtr * m_pConnection;
	//_RecordsetPtr    m_pRecordset;
	//_CommandPtr      m_pCommand;
	//CString m_strServerName;
	//CString m_strDatabaseName;
	//CString m_strUserName;
	//CString m_strPass;
};
