// CSQLConnection.h: interface for the CSQLConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSQLConnection_H__F4993EC6_76EF_41C3_AE70_88F027A3999C__INCLUDED_)
#define AFX_CSQLConnection_H__F4993EC6_76EF_41C3_AE70_88F027A3999C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
no_namespace rename("EOF", "EndOfFile")




#include <stdio.h>

# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	define	PROTECT(s)		{	static	TCHAR *__FUNC_NAME__	= TEXT(#s);
#	define	UNPROTECT		}
# else
#	define	PROTECT(s)		{			\
	try			\
							{			\
static	TCHAR *__FUNC_NAME__= TEXT(#s);	

#	define	UNPROTECT		}			\
	catch(_com_error e)\
							{			\
							}			\
							catch(...)	\
							{			\
							}			\
						}
# endif


# ifndef SAFE_DELETE
#		define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
# endif    
# ifndef SAFE_DELETE_ARRAY
#		define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
# endif    
# ifndef SAFE_RELEASE
#		define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
# endif


#if	defined (TESTONLY) || defined(TEST)
#		define	DBGONLY	TRACE
#	else
#		define	DBGONLY	
#	endif	

#define CRASHREPORT(func)												\
	static TCHAR * __PROC__		= TEXT(#func);							\
	static TCHAR szError[128]	= { 0 }	;								\
	::_sntprintf(szError, 128, _T("***** %s crashed *****"), __PROC__);	\
::OutputDebugString(szError);				


//#	define _countof(s)				sizeof(s)/sizeof(s[0])
#	define	ERR_CODE				::GetLastError()
class CSQLConnection 
{
public:
	typedef _ConnectionPtr	CONNECTION_PTR;
	typedef _RecordsetPtr	RECORDSET_PTR;
	typedef _CommandPtr		COMMAND_PTR;	
	typedef _Parameter		PARAMETER;
	
	CSQLConnection();
	CSQLConnection(void *p);
	virtual ~CSQLConnection();
	void        Init();
	int			GetFieldValue(LPCSTR lpszFieldName, DWORD& dwValue);
	int 		GetFieldValue(LPCSTR lpszFieldName, long& nValue);
	int 		GetFieldValue(LPCSTR lspzFieldName, LPSTR pszValue, DWORD dwSize);
	int			GetFieldValue(LPCSTR lpszFieldName, BOOL& bValue);
	int			ExecuteSQL(LPCSTR lpszCommandText);
	int			ExecuteSP(LPCSTR lpszName);
	
	int			Close();
	long		SetTimeout(long nTimeout = 3);
	long		GetTimeout() const;
	BOOL		Connect(LPCSTR lpszDatabase, LPCSTR lpszHostName, LPCSTR lpszUserName, LPCSTR lpszPassword);
	BOOL		Reconnect();
	BOOL		IsEOF() const;
	BOOL		IsBOF()	const;
	BOOL		MoveFirst();
	BOOL		MoveNext();
	BOOL		BeginTrans();
	BOOL		CommitTrans();
	BOOL		RollbackTrans();
	BOOL		CreateCommand(LPCSTR lpszSPName);
	BOOL CalculateInstallCount(LPCSTR lpszDateTime);
	BOOL ExecuteSPAdPath(DWORD dwGameId,LPCSTR lpszGameVersion,CHAR* lpszPosition);
	BOOL ExecuteSPAdInfo(DWORD dwGameId,LPCSTR lpszGameVersion,CHAR* lpszPosition);
protected:
	DWORD		CombineDSNConnection(LPSTR pszConnection, DWORD dwSize, LPCSTR lpszDatabase, LPCSTR lpszHostName, LPCSTR lpszUserName, LPCSTR lpszPassword);
public:

	CONNECTION_PTR	*m_pConnection;
	RECORDSET_PTR	*m_pRecordset;
	COMMAND_PTR		*m_pCommand;
	long		m_nTimeout;
	_ParameterPtr	m_param1;
	_ParameterPtr	m_param2;
	_ParameterPtr	m_param3;
	_ParameterPtr	m_param4;
	_ParameterPtr	m_param5;
	_ParameterPtr	m_param6;
	_ParameterPtr	m_param7;

private:
	TCHAR		m_Connection[_MAX_PATH];
	TCHAR		m_UserName[_MAX_PATH];
	TCHAR		m_Password[_MAX_PATH];
	BOOL        m_bConnected;

};



#endif // !defined(AFX_CSQLConnection_H__F4993EC6_76EF_41C3_AE70_88F027A3999C__INCLUDED_)
