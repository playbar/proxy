#include "StdAfx.h"
#include "ProxyIDtirectDraw.h"


#include "IGAWin.h"
#include "z_FileIni.h"

void  OnShowFlash(HWND  hwnd)
{
	IGAWIN::CreateIGAWin(hwnd,hwnd);
	char TmpBuf[256];
	::GetModuleFileNameA(NULL,TmpBuf,255);
	char* p = strrchr(TmpBuf,'\\');
	if ( p != NULL )  *p =0;		

	char TmpPath[256];
	wsprintf(TmpPath,"%s\\InterAdtive\\EMMOA\\Flash\\flash.ini",TmpBuf);
	ZFileIni   ini(TmpPath);	

	int  x0 =0 ,y0 = 0,x1 =200,y1 =200;
	ini.GetInt("pos","x0",x0,0);
	ini.GetInt("pos","y0",y0,0);
	ini.GetInt("pos","x1",x1,200);
	ini.GetInt("pos","y1",y1,200);

	IGAWIN::ShowFlash(hwnd,x0,y0,x1,y1);

}

/*********************************************************************************************************************************/
/*    IDirectDraw   */
/*********************************************************************************************************************************/

IGA_IDirectDraw::IGA_IDirectDraw(IDirectDraw*  pIDirectDraw) 
             :m_pIDirectDraw(pIDirectDraw)

{
     
}

IGA_IDirectDraw::~IGA_IDirectDraw()
{    
}

HRESULT IGA_IDirectDraw::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
      HRESULT hr = m_pIDirectDraw->QueryInterface (riid,  ppvObj);       
	  if ( ::IsEqualIID(riid, Proxy_IDirectDraw  )) 
	  {		  
		  *ppvObj                         = this;
	  }
	  else if ( ::IsEqualIID(riid, Proxy_IDirectDraw2 )) 
	  {
		  IGA_IDirectDraw2*   pIDirectDraw = new IGA_IDirectDraw2((IDirectDraw2*)*ppvObj);
		  *ppvObj                         = pIDirectDraw;
	  }
	  else if ( ::IsEqualIID(riid, Proxy_IDirectDraw4)) 
	  {
		  IGA_IDirectDraw4*   pIDirectDraw = new IGA_IDirectDraw4((IDirectDraw4*)*ppvObj);
		  *ppvObj                         = pIDirectDraw;
	  }
	  else if ( ::IsEqualIID(riid, Proxy_IDirectDraw7)) 
	  {
		  IGA_IDirectDraw7*   pIDirectDraw = new IGA_IDirectDraw7((IDirectDraw7*)*ppvObj);
		  *ppvObj                         = pIDirectDraw;
	  }

	  return hr;
}
ULONG   IGA_IDirectDraw::AddRef ()  
{
      return  m_pIDirectDraw->AddRef ()  ;
}
ULONG   IGA_IDirectDraw::Release() 
{
	ULONG  ulRet =  m_pIDirectDraw->Release() ;
	if ( ulRet == 0 )
	{
		delete this;
	}
	return ulRet;
}

/*** IDirectDraw methods ***/
HRESULT IGA_IDirectDraw::Compact() 
{
      return m_pIDirectDraw->Compact() ;
}
HRESULT IGA_IDirectDraw::CreateClipper( DWORD param1, LPDIRECTDRAWCLIPPER FAR* param2, IUnknown FAR * param3 ) 
{
      return m_pIDirectDraw->CreateClipper( param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw::CreatePalette( DWORD param0, LPPALETTEENTRY param1, LPDIRECTDRAWPALETTE FAR* param2, IUnknown FAR * param3 ) 
{
      return m_pIDirectDraw->CreatePalette( param0,  param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw::CreateSurface( LPDDSURFACEDESC param1, LPDIRECTDRAWSURFACE FAR * param2, IUnknown FAR * param3) 
{
      return m_pIDirectDraw->CreateSurface(  param1,  param2,  param3) ;
}
HRESULT IGA_IDirectDraw::DuplicateSurface(  LPDIRECTDRAWSURFACE param1, LPDIRECTDRAWSURFACE FAR * param2) 
{
      return m_pIDirectDraw->DuplicateSurface(  param1,  param2) ;
}
HRESULT IGA_IDirectDraw::EnumDisplayModes(  DWORD param1, LPDDSURFACEDESC param2, LPVOID param3, LPDDENUMMODESCALLBACK param4 ) 
{
      return m_pIDirectDraw->EnumDisplayModes(   param1,  param2,  param3,  param4 ) ;
}
HRESULT IGA_IDirectDraw::EnumSurfaces( DWORD param1, LPDDSURFACEDESC param2, LPVOID param3,LPDDENUMSURFACESCALLBACK  param4) 
{
      return m_pIDirectDraw->EnumSurfaces(  param1,  param2,  param3,  param4) ;
}
HRESULT IGA_IDirectDraw::FlipToGDISurface()
{
      return m_pIDirectDraw->FlipToGDISurface();
}
HRESULT IGA_IDirectDraw::GetCaps(  LPDDCAPS param1, LPDDCAPS param2) 
{
      return m_pIDirectDraw->GetCaps(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw::GetDisplayMode(  LPDDSURFACEDESC param1) 
{
      return m_pIDirectDraw->GetDisplayMode(   param1) ;
}
HRESULT IGA_IDirectDraw::GetFourCCCodes(  LPDWORD param1, LPDWORD param2) 
{
      return m_pIDirectDraw->GetFourCCCodes(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw::GetGDISurface( LPDIRECTDRAWSURFACE FAR * param1) 
{
      return m_pIDirectDraw->GetGDISurface(  param1) ;
}
HRESULT IGA_IDirectDraw::GetMonitorFrequency( LPDWORD param1) 
{
     return m_pIDirectDraw->GetMonitorFrequency(  param1) ;
}
HRESULT IGA_IDirectDraw::GetScanLine( LPDWORD param1) 
{
     return m_pIDirectDraw->GetScanLine(  param1) ;
}
HRESULT IGA_IDirectDraw::GetVerticalBlankStatus( LPBOOL param1) 
{
     return m_pIDirectDraw->GetVerticalBlankStatus(  param1) ;
}
HRESULT IGA_IDirectDraw::Initialize( GUID FAR * param1) 
{
     return m_pIDirectDraw->Initialize(  param1) ;
}
HRESULT IGA_IDirectDraw::RestoreDisplayMode() 
{
     return m_pIDirectDraw->RestoreDisplayMode() ;
}
HRESULT IGA_IDirectDraw::SetCooperativeLevel( HWND param1, DWORD param2) 
{
     HRESULT hr = m_pIDirectDraw->SetCooperativeLevel(  param1,  param2) ;
     OnShowFlash(param1);

	 return hr;
}

HRESULT IGA_IDirectDraw::SetDisplayMode   ( DWORD param1, DWORD param2,DWORD param3)
{
    return m_pIDirectDraw->SetDisplayMode(  param1,  param2, param3); 
}
HRESULT IGA_IDirectDraw::WaitForVerticalBlank( DWORD param1, HANDLE param2) 
{
   return m_pIDirectDraw->WaitForVerticalBlank( param1,  param2) ;
}

/*********************************************************************************************************************************/
/*    IDirectDraw2   */
/*********************************************************************************************************************************/
IGA_IDirectDraw2::IGA_IDirectDraw2(IDirectDraw2*  pIDirectDraw) 
:m_pIDirectDraw(pIDirectDraw)

{

}

IGA_IDirectDraw2::~IGA_IDirectDraw2()
{
	
}

HRESULT IGA_IDirectDraw2::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
	HRESULT hr = m_pIDirectDraw->QueryInterface (riid,  ppvObj);

	if ( ::IsEqualIID(riid, Proxy_IDirectDraw  )) 
	{		  
		IGA_IDirectDraw*   pIDirectDraw = new IGA_IDirectDraw((IDirectDraw*)*ppvObj);
		*ppvObj                         = pIDirectDraw;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw2 )) 
	{		
		*ppvObj                         = this;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw4)) 
	{
		IGA_IDirectDraw4*   pIDirectDraw = new IGA_IDirectDraw4((IDirectDraw4*)*ppvObj);
		*ppvObj                         = pIDirectDraw;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw7)) 
	{
		IGA_IDirectDraw7*   pIDirectDraw = new IGA_IDirectDraw7((IDirectDraw7*)*ppvObj);
		*ppvObj                         = pIDirectDraw;
	}

	return hr;
}
ULONG   IGA_IDirectDraw2::AddRef ()  
{
	return  m_pIDirectDraw->AddRef ()  ;
}
ULONG   IGA_IDirectDraw2::Release() 
{
	ULONG  ulRet =  m_pIDirectDraw->Release() ;
	if ( ulRet == 0 )
	{
		delete this;
	}
	return ulRet;
}

/*** IDirectDraw methods ***/
HRESULT IGA_IDirectDraw2::Compact() 
{
	return m_pIDirectDraw->Compact() ;
}
HRESULT IGA_IDirectDraw2::CreateClipper( DWORD param1, LPDIRECTDRAWCLIPPER FAR* param2, IUnknown FAR * param3 ) 
{
	return m_pIDirectDraw->CreateClipper( param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw2::CreatePalette( DWORD param0, LPPALETTEENTRY param1, LPDIRECTDRAWPALETTE FAR* param2, IUnknown FAR * param3 ) 
{
	return m_pIDirectDraw->CreatePalette( param0,  param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw2::CreateSurface( LPDDSURFACEDESC param1, LPDIRECTDRAWSURFACE FAR * param2, IUnknown FAR * param3) 
{
	return m_pIDirectDraw->CreateSurface(  param1,  param2,  param3) ;
}
HRESULT IGA_IDirectDraw2::DuplicateSurface(  LPDIRECTDRAWSURFACE param1, LPDIRECTDRAWSURFACE FAR * param2) 
{
	return m_pIDirectDraw->DuplicateSurface(  param1,  param2) ;
}
HRESULT IGA_IDirectDraw2::EnumDisplayModes(  DWORD param1, LPDDSURFACEDESC param2, LPVOID param3, LPDDENUMMODESCALLBACK param4 ) 
{
	return m_pIDirectDraw->EnumDisplayModes(   param1,  param2,  param3,  param4 ) ;
}
HRESULT IGA_IDirectDraw2::EnumSurfaces( DWORD param1, LPDDSURFACEDESC param2, LPVOID param3,LPDDENUMSURFACESCALLBACK  param4) 
{
	return m_pIDirectDraw->EnumSurfaces(  param1,  param2,  param3,  param4) ;
}
HRESULT IGA_IDirectDraw2::FlipToGDISurface()
{
	return m_pIDirectDraw->FlipToGDISurface();
}
HRESULT IGA_IDirectDraw2::GetCaps(  LPDDCAPS param1, LPDDCAPS param2) 
{
	return m_pIDirectDraw->GetCaps(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw2::GetDisplayMode(  LPDDSURFACEDESC param1) 
{
	return m_pIDirectDraw->GetDisplayMode(   param1) ;
}
HRESULT IGA_IDirectDraw2::GetFourCCCodes(  LPDWORD param1, LPDWORD param2) 
{
	return m_pIDirectDraw->GetFourCCCodes(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw2::GetGDISurface( LPDIRECTDRAWSURFACE FAR * param1) 
{
	return m_pIDirectDraw->GetGDISurface(  param1) ;
}
HRESULT IGA_IDirectDraw2::GetMonitorFrequency( LPDWORD param1) 
{
	return m_pIDirectDraw->GetMonitorFrequency(  param1) ;
}
HRESULT IGA_IDirectDraw2::GetScanLine( LPDWORD param1) 
{
	return m_pIDirectDraw->GetScanLine(  param1) ;
}
HRESULT IGA_IDirectDraw2::GetVerticalBlankStatus( LPBOOL param1) 
{
	return m_pIDirectDraw->GetVerticalBlankStatus(  param1) ;
}
HRESULT IGA_IDirectDraw2::Initialize( GUID FAR * param1) 
{
	return m_pIDirectDraw->Initialize(  param1) ;
}
HRESULT IGA_IDirectDraw2::RestoreDisplayMode() 
{
	return m_pIDirectDraw->RestoreDisplayMode() ;
}
HRESULT IGA_IDirectDraw2::SetCooperativeLevel( HWND param1, DWORD param2) 
{
	HRESULT hr =  m_pIDirectDraw->SetCooperativeLevel(  param1,  param2) ;
	OnShowFlash(param1);
	return hr;
}

HRESULT IGA_IDirectDraw2::SetDisplayMode   ( DWORD param1, DWORD param2,DWORD param3,DWORD param4,DWORD param5)
{
	return m_pIDirectDraw->SetDisplayMode(  param1,  param2, param3,param4,param5); 
}
HRESULT IGA_IDirectDraw2::WaitForVerticalBlank( DWORD param1, HANDLE param2) 
{
	return m_pIDirectDraw->WaitForVerticalBlank( param1,  param2) ;
}

HRESULT IGA_IDirectDraw2::GetAvailableVidMem( LPDDSCAPS param1, LPDWORD param2, LPDWORD param3) 
{
   return m_pIDirectDraw->GetAvailableVidMem(  param1,  param2,  param3) ;
}


/*********************************************************************************************************************************/
/*    IDirectDraw4   */
/*********************************************************************************************************************************/
IGA_IDirectDraw4::IGA_IDirectDraw4(IDirectDraw4*  pIDirectDraw) 
     :m_pIDirectDraw(pIDirectDraw)

{

}

IGA_IDirectDraw4::~IGA_IDirectDraw4()
{
	
}

HRESULT IGA_IDirectDraw4::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
	HRESULT hr = m_pIDirectDraw->QueryInterface (riid,  ppvObj);

	if ( ::IsEqualIID(riid, Proxy_IDirectDraw  )) 
	{		  
		IGA_IDirectDraw*   pIDirectDraw = new IGA_IDirectDraw((IDirectDraw*)*ppvObj);
		*ppvObj                         = pIDirectDraw;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw2 )) 
	{		
		IGA_IDirectDraw2*   pIDirectDraw = new IGA_IDirectDraw2((IDirectDraw2*)*ppvObj);
		*ppvObj                         = pIDirectDraw;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw4)) 
	{
		
		*ppvObj                         = this;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw7)) 
	{
		IGA_IDirectDraw7*   pIDirectDraw = new IGA_IDirectDraw7((IDirectDraw7*)*ppvObj);
		*ppvObj                         = pIDirectDraw;
	}

	return hr;
}
ULONG   IGA_IDirectDraw4::AddRef ()  
{
	return  m_pIDirectDraw->AddRef ()  ;
}
ULONG   IGA_IDirectDraw4::Release() 
{
	ULONG  ulRet =  m_pIDirectDraw->Release() ;
	if ( ulRet == 0 )
	{
		delete this;
	}
	return ulRet;
}


HRESULT IGA_IDirectDraw4::Compact() 
{
	return m_pIDirectDraw->Compact() ;
}
HRESULT IGA_IDirectDraw4::CreateClipper( DWORD param1, LPDIRECTDRAWCLIPPER FAR* param2, IUnknown FAR * param3 ) 
{
	return m_pIDirectDraw->CreateClipper( param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw4::CreatePalette( DWORD param0, LPPALETTEENTRY param1, LPDIRECTDRAWPALETTE FAR* param2, IUnknown FAR * param3 ) 
{
	return m_pIDirectDraw->CreatePalette( param0,  param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw4::CreateSurface( LPDDSURFACEDESC2 param1, LPDIRECTDRAWSURFACE4 FAR * param2, IUnknown FAR * param3) 
{
	return m_pIDirectDraw->CreateSurface(  param1,  param2,  param3) ;
}
HRESULT IGA_IDirectDraw4::DuplicateSurface(  LPDIRECTDRAWSURFACE4 param1, LPDIRECTDRAWSURFACE4 FAR * param2) 
{
	return m_pIDirectDraw->DuplicateSurface(  param1,  param2) ;
}
HRESULT IGA_IDirectDraw4::EnumDisplayModes(  DWORD param1, LPDDSURFACEDESC2 param2, LPVOID param3, LPDDENUMMODESCALLBACK2 param4 ) 
{
	return m_pIDirectDraw->EnumDisplayModes(   param1,  param2,  param3,  param4 ) ;
}
HRESULT IGA_IDirectDraw4::EnumSurfaces( DWORD param1, LPDDSURFACEDESC2 param2, LPVOID param3,LPDDENUMSURFACESCALLBACK2  param4) 
{
	return m_pIDirectDraw->EnumSurfaces(  param1,  param2,  param3,  param4) ;
}
HRESULT IGA_IDirectDraw4::FlipToGDISurface()
{
	return m_pIDirectDraw->FlipToGDISurface();
}
HRESULT IGA_IDirectDraw4::GetCaps(  LPDDCAPS param1, LPDDCAPS param2) 
{
	return m_pIDirectDraw->GetCaps(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw4::GetDisplayMode(  LPDDSURFACEDESC2 param1) 
{
	return m_pIDirectDraw->GetDisplayMode(   param1) ;
}
HRESULT IGA_IDirectDraw4::GetFourCCCodes(  LPDWORD param1, LPDWORD param2) 
{
	return m_pIDirectDraw->GetFourCCCodes(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw4::GetGDISurface( LPDIRECTDRAWSURFACE4 FAR * param1) 
{
	return m_pIDirectDraw->GetGDISurface(  param1) ;
}
HRESULT IGA_IDirectDraw4::GetMonitorFrequency( LPDWORD param1) 
{
	return m_pIDirectDraw->GetMonitorFrequency(  param1) ;
}
HRESULT IGA_IDirectDraw4::GetScanLine( LPDWORD param1) 
{
	return m_pIDirectDraw->GetScanLine(  param1) ;
}
HRESULT IGA_IDirectDraw4::GetVerticalBlankStatus( LPBOOL param1) 
{
	return m_pIDirectDraw->GetVerticalBlankStatus(  param1) ;
}
HRESULT IGA_IDirectDraw4::Initialize( GUID FAR * param1) 
{
	return m_pIDirectDraw->Initialize(  param1) ;
}
HRESULT IGA_IDirectDraw4::RestoreDisplayMode() 
{
	return m_pIDirectDraw->RestoreDisplayMode() ;
}
HRESULT IGA_IDirectDraw4::SetCooperativeLevel( HWND param1, DWORD param2) 
{
	HRESULT hr =  m_pIDirectDraw->SetCooperativeLevel(  param1,  param2) ;
	OnShowFlash(param1);
	return hr;
}

HRESULT IGA_IDirectDraw4::SetDisplayMode   ( DWORD param1, DWORD param2,DWORD param3,DWORD param4,DWORD param5)
{
	return m_pIDirectDraw->SetDisplayMode(  param1,  param2, param3,param4,param5); 
}
HRESULT IGA_IDirectDraw4::WaitForVerticalBlank( DWORD param1, HANDLE param2) 
{
	return m_pIDirectDraw->WaitForVerticalBlank( param1,  param2) ;
}

HRESULT IGA_IDirectDraw4::GetAvailableVidMem( LPDDSCAPS2 param1, LPDWORD param2, LPDWORD param3) 
{
	return m_pIDirectDraw->GetAvailableVidMem(  param1,  param2,  param3) ;
}

HRESULT IGA_IDirectDraw4::GetSurfaceFromDC (HDC param1, LPDIRECTDRAWSURFACE4 * param2) 
{
    return m_pIDirectDraw->GetSurfaceFromDC (param1, param2) ;
}
HRESULT IGA_IDirectDraw4::RestoreAllSurfaces() 
{
    return m_pIDirectDraw->RestoreAllSurfaces() ;
}
HRESULT IGA_IDirectDraw4::TestCooperativeLevel() 
{
    return m_pIDirectDraw->TestCooperativeLevel() ;
}
HRESULT IGA_IDirectDraw4::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER param1, DWORD  param2) 
{
    return m_pIDirectDraw->GetDeviceIdentifier(param1, param2 ) ;
}


/*********************************************************************************************************************************/
/*    IDirectDraw7   */
/*********************************************************************************************************************************/
IGA_IDirectDraw7::IGA_IDirectDraw7(IDirectDraw7*  pIDirectDraw) 
:m_pIDirectDraw(pIDirectDraw)

{

}

IGA_IDirectDraw7::~IGA_IDirectDraw7()
{
	
}

HRESULT IGA_IDirectDraw7::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
	HRESULT hr = m_pIDirectDraw->QueryInterface (riid,  ppvObj);
    //return hr;

	if ( ::IsEqualIID(riid, Proxy_IDirectDraw  )) 
	{		  
		IGA_IDirectDraw*   pIDirectDraw = new IGA_IDirectDraw((IDirectDraw*)*ppvObj);
		*ppvObj                         = pIDirectDraw;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw2 )) 
	{		
		IGA_IDirectDraw2*   pIDirectDraw = new IGA_IDirectDraw2((IDirectDraw2*)*ppvObj);
		*ppvObj                         = pIDirectDraw;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw4)) 
	{
		IGA_IDirectDraw4*   pIDirectDraw = new IGA_IDirectDraw4((IDirectDraw4*)*ppvObj);
		*ppvObj                         = pIDirectDraw;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw7)) 
	{		
		*ppvObj                         = this;
	}

	return hr;
}
ULONG   IGA_IDirectDraw7::AddRef ()  
{
	return  m_pIDirectDraw->AddRef ()  ;
}
ULONG   IGA_IDirectDraw7::Release() 
{
	ULONG  ulRet =  m_pIDirectDraw->Release() ;
	if ( ulRet == 0 )
	{
		delete this;
	}
	return ulRet;
}


HRESULT IGA_IDirectDraw7::Compact() 
{
	return m_pIDirectDraw->Compact() ;
}
HRESULT IGA_IDirectDraw7::CreateClipper( DWORD param1, LPDIRECTDRAWCLIPPER FAR* param2, IUnknown FAR * param3 ) 
{
	return m_pIDirectDraw->CreateClipper( param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw7::CreatePalette( DWORD param0, LPPALETTEENTRY param1, LPDIRECTDRAWPALETTE FAR* param2, IUnknown FAR * param3 ) 
{
	return m_pIDirectDraw->CreatePalette( param0,  param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw7::CreateSurface( LPDDSURFACEDESC2 param1, LPDIRECTDRAWSURFACE7 FAR * param2, IUnknown FAR * param3) 
{
	return m_pIDirectDraw->CreateSurface(  param1,  param2,  param3) ;
}
HRESULT IGA_IDirectDraw7::DuplicateSurface(  LPDIRECTDRAWSURFACE7 param1, LPDIRECTDRAWSURFACE7 FAR * param2) 
{
	return m_pIDirectDraw->DuplicateSurface(  param1,  param2) ;
}
HRESULT IGA_IDirectDraw7::EnumDisplayModes(  DWORD param1, LPDDSURFACEDESC2 param2, LPVOID param3, LPDDENUMMODESCALLBACK2 param4 ) 
{
	return m_pIDirectDraw->EnumDisplayModes(   param1,  param2,  param3,  param4 ) ;
}
HRESULT IGA_IDirectDraw7::EnumSurfaces( DWORD param1, LPDDSURFACEDESC2 param2, LPVOID param3,LPDDENUMSURFACESCALLBACK7  param4) 
{
	return m_pIDirectDraw->EnumSurfaces(  param1,  param2,  param3,  param4) ;
}
HRESULT IGA_IDirectDraw7::FlipToGDISurface()
{
	return m_pIDirectDraw->FlipToGDISurface();
}
HRESULT IGA_IDirectDraw7::GetCaps(  LPDDCAPS param1, LPDDCAPS param2) 
{
	return m_pIDirectDraw->GetCaps(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw7::GetDisplayMode(  LPDDSURFACEDESC2 param1) 
{
	return m_pIDirectDraw->GetDisplayMode(   param1) ;
}
HRESULT IGA_IDirectDraw7::GetFourCCCodes(  LPDWORD param1, LPDWORD param2) 
{
	return m_pIDirectDraw->GetFourCCCodes(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw7::GetGDISurface( LPDIRECTDRAWSURFACE7 FAR * param1) 
{
	return m_pIDirectDraw->GetGDISurface(  param1) ;
}
HRESULT IGA_IDirectDraw7::GetMonitorFrequency( LPDWORD param1) 
{
	return m_pIDirectDraw->GetMonitorFrequency(  param1) ;
}
HRESULT IGA_IDirectDraw7::GetScanLine( LPDWORD param1) 
{
	return m_pIDirectDraw->GetScanLine(  param1) ;
}
HRESULT IGA_IDirectDraw7::GetVerticalBlankStatus( LPBOOL param1) 
{
	return m_pIDirectDraw->GetVerticalBlankStatus(  param1) ;
}
HRESULT IGA_IDirectDraw7::Initialize( GUID FAR * param1) 
{
	return m_pIDirectDraw->Initialize(  param1) ;
}
HRESULT IGA_IDirectDraw7::RestoreDisplayMode() 
{
	return m_pIDirectDraw->RestoreDisplayMode() ;
}
HRESULT IGA_IDirectDraw7::SetCooperativeLevel( HWND param1, DWORD param2) 
{
	
	OnShowFlash(param1);
	HRESULT   hr = m_pIDirectDraw->SetCooperativeLevel(  param1,  param2) ;
    
	return  hr;
}

HRESULT IGA_IDirectDraw7::SetDisplayMode   ( DWORD param1, DWORD param2,DWORD param3,DWORD param4,DWORD param5)
{
	return m_pIDirectDraw->SetDisplayMode(  param1,  param2, param3,param4,param5); 
}
HRESULT IGA_IDirectDraw7::WaitForVerticalBlank( DWORD param1, HANDLE param2) 
{
	return m_pIDirectDraw->WaitForVerticalBlank( param1,  param2) ;
}

HRESULT IGA_IDirectDraw7::GetAvailableVidMem( LPDDSCAPS2 param1, LPDWORD param2, LPDWORD param3) 
{
	return m_pIDirectDraw->GetAvailableVidMem(  param1,  param2,  param3) ;
}

HRESULT IGA_IDirectDraw7::GetSurfaceFromDC (HDC param1, LPDIRECTDRAWSURFACE7 * param2) 
{
	return m_pIDirectDraw->GetSurfaceFromDC (param1, param2) ;
}
HRESULT IGA_IDirectDraw7::RestoreAllSurfaces() 
{
	return m_pIDirectDraw->RestoreAllSurfaces() ;
}
HRESULT IGA_IDirectDraw7::TestCooperativeLevel() 
{
	return m_pIDirectDraw->TestCooperativeLevel() ;
}
HRESULT IGA_IDirectDraw7::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2 param1, DWORD  param2) 
{
	return m_pIDirectDraw->GetDeviceIdentifier(param1, param2 ) ;
}

HRESULT IGA_IDirectDraw7::StartModeTest( LPSIZE param1, DWORD param2, DWORD param3 ) 
{
    return m_pIDirectDraw->StartModeTest(param1, param2,param3 ) ;
}
HRESULT IGA_IDirectDraw7::EvaluateMode ( DWORD param1, DWORD *  param2) 
{
   return m_pIDirectDraw->EvaluateMode(param1, param2 ) ;
}