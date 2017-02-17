// IE_DDS_ControlPropPage.cpp : Implementation of the CIE_DDS_ControlPropPage property page class.

#include "stdafx.h"
#include "IE_DDS_Control.h"
#include "IE_DDS_ControlPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CIE_DDS_ControlPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CIE_DDS_ControlPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CIE_DDS_ControlPropPage, "IE_DDS_CONTROL.IE_DDS_ControlPropPage.1",
	0x81a6de16, 0x56c, 0x4f7d, 0x93, 0x94, 0xec, 0xa, 0x28, 0xa, 0x5a, 0x59)



// CIE_DDS_ControlPropPage::CIE_DDS_ControlPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CIE_DDS_ControlPropPage

BOOL CIE_DDS_ControlPropPage::CIE_DDS_ControlPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_IE_DDS_CONTROL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CIE_DDS_ControlPropPage::CIE_DDS_ControlPropPage - Constructor

CIE_DDS_ControlPropPage::CIE_DDS_ControlPropPage() :
	COlePropertyPage(IDD, IDS_IE_DDS_CONTROL_PPG_CAPTION)
{
}



// CIE_DDS_ControlPropPage::DoDataExchange - Moves data between page and properties

void CIE_DDS_ControlPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CIE_DDS_ControlPropPage message handlers
