#pragma once

// IE_DDS_ControlPropPage.h : Declaration of the CIE_DDS_ControlPropPage property page class.


// CIE_DDS_ControlPropPage : See IE_DDS_ControlPropPage.cpp for implementation.

class CIE_DDS_ControlPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CIE_DDS_ControlPropPage)
	DECLARE_OLECREATE_EX(CIE_DDS_ControlPropPage)

// Constructor
public:
	CIE_DDS_ControlPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_IE_DDS_CONTROL };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

