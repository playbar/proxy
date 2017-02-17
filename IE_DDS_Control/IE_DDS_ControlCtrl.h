#pragma once

// IE_DDS_ControlCtrl.h : Declaration of the CIE_DDS_ControlCtrl ActiveX Control class.


// CIE_DDS_ControlCtrl : See IE_DDS_ControlCtrl.cpp for implementation.

#include "CddsAnalyse.h"
#include  <atlbase.h>
#include <atlctl.h>

class CIE_DDS_ControlCtrl : public COleControl
{
	DECLARE_DYNCREATE(CIE_DDS_ControlCtrl)

	STDMETHOD(get_PenColor)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_PenColor)(/*[in]*/ BSTR newVal);

// Constructor
public:
	CIE_DDS_ControlCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

	STDMETHODIMP GetQueryResultToArray(LONG* nSum, VARIANT* vQuery);
	int add(int a, int b) ;

// Implementation
protected:
	~CIE_DDS_ControlCtrl();

	DECLARE_OLECREATE_EX(CIE_DDS_ControlCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CIE_DDS_ControlCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CIE_DDS_ControlCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CIE_DDS_ControlCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidDDSFileName = 113,
		dispidGetFileName = 112L,
		dispidID = 110L,    
		dispidNAME = 111L, 
	};

public:
	CddsAnalyse m_cddsAnalyse;

	ULONG m_nQueryNum;

	CString m_strQuery;
	COLORREF m_PenColor;
protected:
	void GetDDSFileName(LPCTSTR bstrFileName);
	void OnDDSFileNameChanged(void);
	CString m_DDSFileName;
};

