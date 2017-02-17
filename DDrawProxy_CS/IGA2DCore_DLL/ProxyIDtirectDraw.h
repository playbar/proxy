#ifndef _PROXY_IDIRECTDRAWE_H
#define _PROXY_IDIRECTDRAWE_H
#include <ddraw.h>

const GUID Proxy_IDirectDraw   = {0x6C14DB80,0xA733,0x11CE,{0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60 }};
const GUID Proxy_IDirectDraw2  = {0xB3A6F3E0,0x2B43,0x11CF,{0xA2,0xDE,0x00,0xAA,0x00,0xB9,0x33,0x56 }};
const GUID Proxy_IDirectDraw4  = {0x9c59509a,0x39bd,0x11d1,{0x8c,0x4a,0x00,0xc0,0x4f,0xd9,0x30,0xc5 }};
const GUID Proxy_IDirectDraw7  = {0x15e65ec0,0x3b9c,0x11d2,{0xb9,0x2f,0x00,0x60,0x97,0x97,0xea,0x5b }};



class IGA_IDirectDraw :public IDirectDraw
{
public:
	IGA_IDirectDraw(IDirectDraw*  pIDirectDraw);
	~IGA_IDirectDraw();

	virtual HRESULT __stdcall   QueryInterface (REFIID riid, LPVOID FAR * ppvObj) ;
	virtual ULONG   __stdcall   AddRef  () ;
	virtual ULONG   __stdcall   Release () ;

	/*** IDirectDraw methods ***/
	virtual HRESULT __stdcall    Compact() ;
	virtual HRESULT __stdcall    CreateClipper( DWORD, LPDIRECTDRAWCLIPPER FAR*, IUnknown FAR * ) ;
	virtual HRESULT __stdcall    CreatePalette( DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE FAR*, IUnknown FAR * ) ;
	virtual HRESULT __stdcall    CreateSurface(  LPDDSURFACEDESC, LPDIRECTDRAWSURFACE FAR *, IUnknown FAR *) ;
	virtual HRESULT __stdcall    DuplicateSurface(  LPDIRECTDRAWSURFACE, LPDIRECTDRAWSURFACE FAR * ) ;
	virtual HRESULT __stdcall    EnumDisplayModes(  DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMMODESCALLBACK ) ;
	virtual HRESULT __stdcall    EnumSurfaces( DWORD, LPDDSURFACEDESC, LPVOID,LPDDENUMSURFACESCALLBACK ) ;
	virtual HRESULT __stdcall    FlipToGDISurface() ;
	virtual HRESULT __stdcall    GetCaps(  LPDDCAPS, LPDDCAPS) ;
	virtual HRESULT __stdcall    GetDisplayMode(  LPDDSURFACEDESC) ;
	virtual HRESULT __stdcall    GetFourCCCodes(  LPDWORD, LPDWORD ) ;
	virtual HRESULT __stdcall    GetGDISurface( LPDIRECTDRAWSURFACE FAR *) ;
	virtual HRESULT __stdcall    GetMonitorFrequency( LPDWORD) ;
	virtual HRESULT __stdcall    GetScanLine( LPDWORD) ;
	virtual HRESULT __stdcall    GetVerticalBlankStatus( LPBOOL ) ;
	virtual HRESULT __stdcall    Initialize( GUID FAR *) ;
	virtual HRESULT __stdcall    RestoreDisplayMode() ;
	virtual HRESULT __stdcall    SetCooperativeLevel( HWND, DWORD) ;
	virtual HRESULT __stdcall    SetDisplayMode   ( DWORD, DWORD,DWORD);
	virtual HRESULT __stdcall    WaitForVerticalBlank( DWORD, HANDLE ) ;
				    
private:		    
    IDirectDraw*   m_pIDirectDraw;
};


class IGA_IDirectDraw2 :public IDirectDraw2
{
public:
	IGA_IDirectDraw2(IDirectDraw2*  pIDirectDraw);
	~IGA_IDirectDraw2();

	virtual HRESULT __stdcall   QueryInterface (REFIID riid, LPVOID FAR * ppvObj) ;
	virtual ULONG   __stdcall   AddRef  () ;
	virtual ULONG   __stdcall   Release () ;

	/*** IDirectDraw methods ***/
	virtual HRESULT __stdcall    Compact() ;
	virtual HRESULT __stdcall    CreateClipper( DWORD, LPDIRECTDRAWCLIPPER FAR*, IUnknown FAR * ) ;
	virtual HRESULT __stdcall    CreatePalette( DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE FAR*, IUnknown FAR * ) ;
	virtual HRESULT __stdcall    CreateSurface(  LPDDSURFACEDESC, LPDIRECTDRAWSURFACE FAR *, IUnknown FAR *) ;
	virtual HRESULT __stdcall    DuplicateSurface(  LPDIRECTDRAWSURFACE, LPDIRECTDRAWSURFACE FAR * ) ;
	virtual HRESULT __stdcall    EnumDisplayModes(  DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMMODESCALLBACK ) ;
	virtual HRESULT __stdcall    EnumSurfaces( DWORD, LPDDSURFACEDESC, LPVOID,LPDDENUMSURFACESCALLBACK ) ;
	virtual HRESULT __stdcall    FlipToGDISurface() ;
	virtual HRESULT __stdcall    GetCaps(  LPDDCAPS, LPDDCAPS) ;
	virtual HRESULT __stdcall    GetDisplayMode(  LPDDSURFACEDESC) ;
	virtual HRESULT __stdcall    GetFourCCCodes(  LPDWORD, LPDWORD ) ;
	virtual HRESULT __stdcall    GetGDISurface( LPDIRECTDRAWSURFACE FAR *) ;
	virtual HRESULT __stdcall    GetMonitorFrequency( LPDWORD) ;
	virtual HRESULT __stdcall    GetScanLine( LPDWORD) ;
	virtual HRESULT __stdcall    GetVerticalBlankStatus( LPBOOL ) ;
	virtual HRESULT __stdcall    Initialize( GUID FAR *) ;
	virtual HRESULT __stdcall    RestoreDisplayMode() ;
	virtual HRESULT __stdcall    SetCooperativeLevel( HWND, DWORD) ;
	virtual HRESULT __stdcall    SetDisplayMode   ( DWORD, DWORD,DWORD,DWORD, DWORD);
	virtual HRESULT __stdcall    WaitForVerticalBlank( DWORD, HANDLE ) ;
				    
	virtual HRESULT __stdcall    GetAvailableVidMem( LPDDSCAPS, LPDWORD, LPDWORD) ;
private:
	IDirectDraw2*   m_pIDirectDraw;
};


class IGA_IDirectDraw4 :public IDirectDraw4
{
public:
	IGA_IDirectDraw4(IDirectDraw4*  pIDirectDraw);
	~IGA_IDirectDraw4();

	virtual HRESULT __stdcall   QueryInterface (REFIID riid, LPVOID FAR * ppvObj) ;
	virtual ULONG   __stdcall   AddRef  () ;
	virtual ULONG   __stdcall   Release () ;

	/*** IDirectDraw methods ***/
	virtual HRESULT __stdcall   Compact() ;
	virtual HRESULT __stdcall   CreateClipper( DWORD, LPDIRECTDRAWCLIPPER FAR*, IUnknown FAR * ) ;
	virtual HRESULT __stdcall   CreatePalette( DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE FAR*, IUnknown FAR * ) ;
	virtual HRESULT __stdcall   CreateSurface(  LPDDSURFACEDESC2, LPDIRECTDRAWSURFACE4 FAR *, IUnknown FAR *) ;
	virtual HRESULT __stdcall   DuplicateSurface(  LPDIRECTDRAWSURFACE4, LPDIRECTDRAWSURFACE4 FAR * ) ;
	virtual HRESULT __stdcall   EnumDisplayModes(  DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2 ) ;
	virtual HRESULT __stdcall   EnumSurfaces( DWORD, LPDDSURFACEDESC2, LPVOID,LPDDENUMSURFACESCALLBACK2 ) ;
	virtual HRESULT __stdcall   FlipToGDISurface() ;
	virtual HRESULT __stdcall   GetCaps(  LPDDCAPS, LPDDCAPS) ;
	virtual HRESULT __stdcall   GetDisplayMode(  LPDDSURFACEDESC2) ;
	virtual HRESULT __stdcall   GetFourCCCodes(  LPDWORD, LPDWORD ) ;
	virtual HRESULT __stdcall   GetGDISurface( LPDIRECTDRAWSURFACE4 FAR *) ;
	virtual HRESULT __stdcall   GetMonitorFrequency( LPDWORD) ;
	virtual HRESULT __stdcall   GetScanLine( LPDWORD) ;
	virtual HRESULT __stdcall   GetVerticalBlankStatus( LPBOOL ) ;
	virtual HRESULT __stdcall   Initialize( GUID FAR *) ;
	virtual HRESULT __stdcall   RestoreDisplayMode() ;
	virtual HRESULT __stdcall   SetCooperativeLevel( HWND, DWORD) ;
	virtual HRESULT __stdcall   SetDisplayMode   ( DWORD, DWORD,DWORD,DWORD, DWORD);
	virtual HRESULT __stdcall   WaitForVerticalBlank( DWORD, HANDLE ) ;
				    
	virtual HRESULT __stdcall   GetAvailableVidMem( LPDDSCAPS2, LPDWORD, LPDWORD) ;
				   
	virtual HRESULT __stdcall   GetSurfaceFromDC ( HDC, LPDIRECTDRAWSURFACE4 *) ;
	virtual HRESULT __stdcall   RestoreAllSurfaces() ;
	virtual HRESULT __stdcall   TestCooperativeLevel() ;
	virtual HRESULT __stdcall   GetDeviceIdentifier(LPDDDEVICEIDENTIFIER, DWORD ) ;
				   
private:		      
	IDirectDraw4*      m_pIDirectDraw;
};



class IGA_IDirectDraw7 :public IDirectDraw7
{
public:
	IGA_IDirectDraw7(IDirectDraw7*  pIDirectDraw);
	~IGA_IDirectDraw7();

	virtual HRESULT __stdcall QueryInterface (REFIID riid, LPVOID FAR * ppvObj) ;
	virtual ULONG   __stdcall AddRef  () ;
	virtual ULONG   __stdcall Release () ;

	/*** IDirectDraw methods ***/
	virtual HRESULT __stdcall  Compact() ;
	virtual HRESULT __stdcall  CreateClipper( DWORD, LPDIRECTDRAWCLIPPER FAR*, IUnknown FAR * ) ;
	virtual HRESULT __stdcall  CreatePalette( DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE FAR*, IUnknown FAR * ) ;
	virtual HRESULT __stdcall  CreateSurface(  LPDDSURFACEDESC2, LPDIRECTDRAWSURFACE7 FAR *, IUnknown FAR *) ;
	virtual HRESULT __stdcall  DuplicateSurface(  LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7 FAR * ) ;
	virtual HRESULT __stdcall  EnumDisplayModes(  DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2 ) ;
	virtual HRESULT __stdcall  EnumSurfaces( DWORD, LPDDSURFACEDESC2, LPVOID,LPDDENUMSURFACESCALLBACK7 ) ;
	virtual HRESULT __stdcall  FlipToGDISurface() ;
	virtual HRESULT __stdcall  GetCaps(  LPDDCAPS, LPDDCAPS) ;
	virtual HRESULT __stdcall  GetDisplayMode(  LPDDSURFACEDESC2) ;
	virtual HRESULT __stdcall  GetFourCCCodes(  LPDWORD, LPDWORD ) ;
	virtual HRESULT __stdcall  GetGDISurface( LPDIRECTDRAWSURFACE7 FAR *) ;
	virtual HRESULT __stdcall  GetMonitorFrequency( LPDWORD) ;
	virtual HRESULT __stdcall  GetScanLine( LPDWORD) ;
	virtual HRESULT __stdcall  GetVerticalBlankStatus( LPBOOL ) ;
	virtual HRESULT __stdcall  Initialize( GUID FAR *) ;
	virtual HRESULT __stdcall  RestoreDisplayMode() ;
	virtual HRESULT __stdcall  SetCooperativeLevel( HWND, DWORD) ;
	virtual HRESULT __stdcall  SetDisplayMode   ( DWORD, DWORD,DWORD,DWORD, DWORD);
	virtual HRESULT __stdcall  WaitForVerticalBlank( DWORD, HANDLE ) ;
				    
	virtual HRESULT __stdcall  GetAvailableVidMem( LPDDSCAPS2, LPDWORD, LPDWORD) ;
	
	virtual HRESULT __stdcall  GetSurfaceFromDC ( HDC, LPDIRECTDRAWSURFACE7 *) ;
	virtual HRESULT __stdcall  RestoreAllSurfaces() ;
	virtual HRESULT __stdcall  TestCooperativeLevel() ;
	virtual HRESULT __stdcall  GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2, DWORD ) ;
	
	virtual HRESULT __stdcall  StartModeTest( LPSIZE, DWORD, DWORD ) ;
	virtual HRESULT __stdcall  EvaluateMode ( DWORD, DWORD * ) ;

private:
	IDirectDraw7*   m_pIDirectDraw;
};

#endif

