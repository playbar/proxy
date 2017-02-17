// IE_DDS_ControlCtrl.cpp : Implementation of the CIE_DDS_ControlCtrl ActiveX Control class.

#include "stdafx.h"
#include "IE_DDS_Control.h"
#include "IE_DDS_ControlCtrl.h"
#include "IE_DDS_ControlPropPage.h"
#include   <comdef.h>   

#include <vector>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CIE_DDS_ControlCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CIE_DDS_ControlCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


BEGIN_PROP_MAP(CIE_DDS_ControlCtrl)
	PROP_ENTRY("PenColor", 1, CLSID_NULL)
END_PROP_MAP()

// Dispatch map

BEGIN_DISPATCH_MAP(CIE_DDS_ControlCtrl, COleControl)
	DISP_FUNCTION_ID(CIE_DDS_ControlCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_DEFVALUE(CIE_DDS_ControlCtrl, "ID")    
	DISP_DEFVALUE(CIE_DDS_ControlCtrl, "NAME")    
	DISP_STOCKPROP_READYSTATE()    
	DISP_FUNCTION_ID(CIE_DDS_ControlCtrl, "GetFileName", dispidGetFileName, GetDDSFileName, VT_EMPTY, VTS_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CIE_DDS_ControlCtrl, "DDSFileName", dispidDDSFileName, m_DDSFileName, OnDDSFileNameChanged, VT_BSTR)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CIE_DDS_ControlCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CIE_DDS_ControlCtrl, 1)
	PROPPAGEID(CIE_DDS_ControlPropPage::guid)
END_PROPPAGEIDS(CIE_DDS_ControlCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CIE_DDS_ControlCtrl, "IE_DDS_CONTROL.IE_DDS_ControlCtrl.1",
	0xf49a71f8, 0x7732, 0x49ed, 0xbd, 0xbb, 0x9d, 0x9b, 0x8c, 0xfb, 0xfe, 0xe5)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CIE_DDS_ControlCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DIE_DDS_Control =
		{ 0xA43097A4, 0xDC8C, 0x4527, { 0xA9, 0x14, 0x11, 0xF, 0xB6, 0x5, 0xC7, 0xB } };
const IID BASED_CODE IID_DIE_DDS_ControlEvents =
		{ 0xB650314E, 0xC5F7, 0x488F, { 0x91, 0xCD, 0x1B, 0x3, 0xFB, 0xD4, 0x4F, 0x8D } };



// Control type information

static const DWORD BASED_CODE _dwIE_DDS_ControlOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CIE_DDS_ControlCtrl, IDS_IE_DDS_CONTROL, _dwIE_DDS_ControlOleMisc)



// CIE_DDS_ControlCtrl::CIE_DDS_ControlCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CIE_DDS_ControlCtrl

BOOL CIE_DDS_ControlCtrl::CIE_DDS_ControlCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_IE_DDS_CONTROL,
			IDB_IE_DDS_CONTROL,
			afxRegApartmentThreading,
			_dwIE_DDS_ControlOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CIE_DDS_ControlCtrl::CIE_DDS_ControlCtrl - Constructor

CIE_DDS_ControlCtrl::CIE_DDS_ControlCtrl()
:m_nQueryNum(0)
{
	InitializeIIDs(&IID_DIE_DDS_Control, &IID_DIE_DDS_ControlEvents);
	// TODO: Initialize your control's instance data here.
}



// CIE_DDS_ControlCtrl::~CIE_DDS_ControlCtrl - Destructor

CIE_DDS_ControlCtrl::~CIE_DDS_ControlCtrl()
{
	// TODO: Cleanup your control's instance data here.
	m_cddsAnalyse.ReleaseFile();
}



// CIE_DDS_ControlCtrl::OnDraw - Drawing function

void CIE_DDS_ControlCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	char szOutput[256];
	sprintf(szOutput, "m_PenColor = %d",m_PenColor);
	OutputDebugString(szOutput);

	m_strQuery ="c:\\temp.dds";

	/*pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	CRect rect;
	GetClientRect(&rect);
	pdc->DrawText(szOutput, 20, &rect, 0);*/

//*
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));	
	if  ( !m_strQuery.IsEmpty())
	{
		m_cddsAnalyse.LoadDDSFromFile((LPTSTR)m_strQuery.GetBuffer(0),(HWND)pdc->m_hDC);

		CRect rect;
		GetClientRect(&rect);

		CBitmap   *pOldBitmap;   
		CDC   ppdc;   
		CClientDC   dc(this);   
		ppdc.CreateCompatibleDC(pdc);    
		pOldBitmap = (CBitmap *)ppdc.SelectObject( m_cddsAnalyse.m_hBitMap);   
		int   cxIcon   =   GetSystemMetrics(SM_CXICON);   
		int   cyIcon   =   GetSystemMetrics(SM_CYICON);   
		GetClientRect(&rect);   
		int   x   =   (rect.Width()   -   cxIcon   +   1)   /   2;   
		int   y   =   (rect.Height()   -   cyIcon   +   1)   /   2;   

		dc.BitBlt(0,0,rect.Width(),rect.Height(),&ppdc,0,0,SRCCOPY);   
		//ppdc.SelectObject(pOldBitmap);  
	}	
//*/

}



// CIE_DDS_ControlCtrl::DoPropExchange - Persistence support

void CIE_DDS_ControlCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
	

	PX_ULong(pPX, _T("ID"), m_nQueryNum);    
	PX_String(pPX, _T("NAME"), m_strQuery);//还有其他PX_Short之类，详参msdn    

	char chOutput[256];
	sprintf(chOutput, "m_strQuery = %s",m_strQuery);
	OutputDebugString(chOutput);

}



// CIE_DDS_ControlCtrl::OnResetState - Reset control to default state

void CIE_DDS_ControlCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CIE_DDS_ControlCtrl::AboutBox - Display an "About" box to the user

void CIE_DDS_ControlCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_IE_DDS_CONTROL);
	dlgAbout.DoModal();
}
// CIE_DDS_ControlCtrl message handlers

int CIE_DDS_ControlCtrl::add(int a, int b) 
{ 
	return 0; 
} 


STDMETHODIMP CIE_DDS_ControlCtrl::GetQueryResultToArray(LONG* nSum, VARIANT* vQuery)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*nSum=m_nQueryNum;
	CString str=m_strQuery;
	VariantClear(vQuery);
	SAFEARRAY * pSA;
	// Create the safearray.
	SAFEARRAYBOUND sabRSarray[1];  // A one-dimensional array.
	sabRSarray[0].cElements=*nSum;       // Two elements.
	sabRSarray[0].lLbound=0;         //Lower bound of array.
	pSA=SafeArrayCreate(VT_VARIANT,1, sabRSarray);
	_variant_t var1;
	long ndex = 0;
	CString strTmp;
	int iStart = 0;
	for(int i = 0; i< *nSum; i++)
	{
		iStart = str.Find(";");
		strTmp = str.Left(iStart);
		str = str.Mid(iStart + 1);
		var1.vt = VT_BSTR;
		var1.bstrVal = strTmp.AllocSysString();
		ndex = i;
		SafeArrayPutElement(pSA, &ndex, &var1);
	}
	vQuery->vt = VT_ARRAY|VT_VARIANT;
	vQuery->parray = pSA;
	return S_OK;
}



void CIE_DDS_ControlCtrl::GetDDSFileName(LPCTSTR bstrFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	m_strQuery = bstrFileName;
	InvalidateRgn( NULL, true );
	//OutputDebugString(m_strQuery);
	return ;
}



STDMETHODIMP CIE_DDS_ControlCtrl::get_PenColor(BSTR *pVal)
{
	return S_OK;
}

STDMETHODIMP CIE_DDS_ControlCtrl::put_PenColor(BSTR newVal)
{
	_bstr_t b(newVal, true);

#ifdef _UNICODE
	std::wstring s, s16(L"0123456789abcdefABCDEF");
	s = b;
#else
	std::string s, s16("0123456789abcdefABCDEF");
	s = (LPCSTR)b;
#endif
	if (s.length()) 
	{	// check string's format if like "#ff00aa" 
#ifdef _UNICODE
		if (s.at(0) == L'#') {
#else
		if (s.at(0) == '#') 
		{
#endif
			s = s.substr(1, s.length() - 1);
			if (s.length() == 6) {
				int st = s.find_first_not_of(s16);
				if (st == -1) {
#ifdef _UNICODE
					swscanf((const unsigned short*)s.c_str(), (const unsigned short*)L"%x", &m_PenColor);
#else
					sscanf(s.c_str(), "%x", &m_PenColor);
#endif
					m_PenColor = ((m_PenColor>>0x0) & 0xff) <<0x10 |
						((m_PenColor>>0x8) & 0xff) <<0x8  |
						((m_PenColor>>0x10) & 0xff) <<0x0;
					return S_OK;

				}
			}
		}
	}
	m_PenColor = 0;
	return S_OK;
}
void CIE_DDS_ControlCtrl::OnDDSFileNameChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here

	SetModifiedFlag();
}
