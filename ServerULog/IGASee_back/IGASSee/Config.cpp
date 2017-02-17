#include "StdAfx.h"
#include "config.h"

CConfig::CConfig(void)
{
	m_sFileName = _T("");
	m_sSection = _T("");
}

CConfig::CConfig(LPCTSTR lpszFileName)
{
	m_sFileName = lpszFileName;
	m_sSection = _T("");
}


CConfig::~CConfig(void)
{
}


void CConfig::SetValue(LPCTSTR lpszKeyName, int nValue)
{
	ASSERT( !m_sFileName.IsEmpty() && !m_sSection.IsEmpty() );
	TCHAR	szTemp[32]	= { 0 };
	::_sntprintf(szTemp, sizeof(szTemp), _T("%d"), nValue);
	::WritePrivateProfileString((LPCTSTR)m_sSection, lpszKeyName, szTemp, (LPCTSTR)m_sFileName);
}


void CConfig::SetValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue)
{
	ASSERT( !m_sFileName.IsEmpty() && !m_sSection.IsEmpty() );
	::WritePrivateProfileString((LPCTSTR)m_sSection, lpszKeyName, lpszValue, (LPCTSTR)m_sFileName);
}


BOOL CConfig::GetValue(LPCTSTR lpszKeyName, int &nValue, int nDefaultValue/* = 0*/)
{
	ASSERT( !m_sFileName.IsEmpty() && !m_sSection.IsEmpty() );
	nValue = ::GetPrivateProfileInt((LPCTSTR)m_sSection, lpszKeyName, nDefaultValue, (LPCTSTR)m_sFileName);

	return TRUE;
}


BOOL CConfig::GetValue(LPCTSTR lpszKeyName, CString & sValue, LPCTSTR lpszDefault/* = _T("")*/)
{
	ASSERT( !m_sFileName.IsEmpty() && !m_sSection.IsEmpty() );
	DWORD dw = ::GetPrivateProfileString((LPCTSTR)m_sSection, lpszKeyName, 
		lpszDefault, sValue.GetBuffer(MAX_PATH), MAX_PATH, (LPCTSTR)m_sFileName);
	sValue.ReleaseBuffer();
	ASSERT( dw < MAX_PATH );
	return TRUE;

}
