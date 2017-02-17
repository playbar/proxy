#pragma once

class CConfig
{
public:
	CConfig(void);
	CConfig(LPCTSTR lpszFileName);
	virtual ~CConfig(void);

	void SetValue(LPCTSTR lpszKeyName, int nValue);
	void SetValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue);
	BOOL GetValue(LPCTSTR lpszKeyName, int &nValue, int nDefaultValue = 0);
	BOOL GetValue(LPCTSTR lpszKeyName, CString & sValue, LPCTSTR lpszDefault = _T(""));

	void SetSection(LPCTSTR lpszSection)	{ m_sSection = lpszSection; }
	LPCTSTR GetSection()					{ return m_sSection; }
	void SetFileName(LPCTSTR lpszFileName)	{ m_sFileName = lpszFileName; }
	LPCTSTR GetFileName()					{ return m_sFileName; }

private:
	CString m_sFileName;
	CString m_sSection;
};
