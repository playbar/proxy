/***********************************************************************/
/* Ω”π‹directdrawsurface¿‡                                                               
/************************************************************************/

#pragma once

#include <ddraw.h>

const GUID Proxy_IDirectDrawSurface  = {0x6C14DB81,0xA733,0x11CE,{0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60}};
const GUID Proxy_IDirectDrawSurface2 = {0x57805885,0x6eec,0x11cf,{0x94,0x41,0xa8,0x23,0x03,0xc1,0x0e,0x27}};
const GUID Proxy_IDirectDrawSurface3 = {0xDA044E00,0x69B2,0x11D0,{0xA1,0xD5,0x00,0xAA,0x00,0xB8,0xDF,0xBB}};
const GUID Proxy_IDirectDrawSurface4 = {0x0B2B8630,0xAD35,0x11D0,{0x8E,0xA6,0x00,0x60,0x97,0x97,0xEA,0x5B}};
const GUID Proxy_IDirectDrawSurface7 = {0x06675a80,0x3b9b,0x11d2,{0xb9,0x2f,0x00,0x60,0x97,0x97,0xea,0x5b}};


class IGA_IDirectDrawSurface : public IDirectDrawSurface
{
public:
	IGA_IDirectDrawSurface( IDirectDrawSurface*   pIDirectDrawSurface);
	~IGA_IDirectDrawSurface(void);

	virtual HRESULT __stdcall   QueryInterface (REFIID riid, LPVOID FAR * ppvObj) ;
	virtual ULONG   __stdcall   AddRef  () ;
	virtual ULONG   __stdcall   Release () ;

	virtual HRESULT __stdcall  AddAttachedSurface( LPDIRECTDRAWSURFACE );
	virtual HRESULT __stdcall  AddOverlayDirtyRect( LPRECT );  
	virtual HRESULT __stdcall  Blt(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDBLTFX );
	virtual HRESULT __stdcall  BltBatch( LPDDBLTBATCH, DWORD, DWORD );
	virtual HRESULT __stdcall  BltFast(DWORD,DWORD,LPDIRECTDRAWSURFACE, LPRECT,DWORD);
	virtual HRESULT __stdcall  DeleteAttachedSurface( DWORD, LPDIRECTDRAWSURFACE );
	virtual HRESULT __stdcall  EnumAttachedSurfaces( LPVOID, LPDDENUMSURFACESCALLBACK );
	virtual HRESULT __stdcall  EnumOverlayZOrders( DWORD, LPVOID, LPDDENUMSURFACESCALLBACK );
	virtual HRESULT __stdcall  Flip(LPDIRECTDRAWSURFACE, DWORD );
	virtual HRESULT __stdcall  GetAttachedSurface( LPDDSCAPS, LPDIRECTDRAWSURFACE FAR *);
	virtual HRESULT __stdcall  GetBltStatus( DWORD );
	virtual HRESULT __stdcall  GetCaps( LPDDSCAPS );
	virtual HRESULT __stdcall  GetClipper( LPDIRECTDRAWCLIPPER FAR *);
	virtual HRESULT __stdcall  GetColorKey( DWORD, LPDDCOLORKEY );
	virtual HRESULT __stdcall  GetDC( HDC FAR *);
	virtual HRESULT __stdcall  GetFlipStatus( DWORD );
	virtual HRESULT __stdcall  GetOverlayPosition( LPLONG, LPLONG );
	virtual HRESULT __stdcall  GetPalette( LPDIRECTDRAWPALETTE FAR*);
	virtual HRESULT __stdcall  GetPixelFormat( LPDDPIXELFORMAT );
	virtual HRESULT __stdcall  GetSurfaceDesc( LPDDSURFACEDESC );
	virtual HRESULT __stdcall  Initialize( LPDIRECTDRAW, LPDDSURFACEDESC );
	virtual HRESULT __stdcall  IsLost();
	virtual HRESULT __stdcall  Lock( LPRECT, LPDDSURFACEDESC, DWORD, HANDLE );
	virtual HRESULT __stdcall  ReleaseDC( HDC );
	virtual HRESULT __stdcall  Restore();
	virtual HRESULT __stdcall  SetClipper( LPDIRECTDRAWCLIPPER );
	virtual HRESULT __stdcall  SetColorKey( DWORD, LPDDCOLORKEY );
	virtual HRESULT __stdcall  SetOverlayPosition( LONG, LONG );
	virtual HRESULT __stdcall  SetPalette( LPDIRECTDRAWPALETTE );
	virtual HRESULT __stdcall  Unlock( LPVOID );
	virtual HRESULT __stdcall  UpdateOverlay( LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDOVERLAYFX );
	virtual HRESULT __stdcall  UpdateOverlayDisplay( DWORD );
	virtual HRESULT __stdcall  UpdateOverlayZOrder( DWORD, LPDIRECTDRAWSURFACE );


private:
	IDirectDrawSurface*   m_pIDirectDrawSurface;

};


class IGA_IDirectDrawSurface2: public IDirectDrawSurface2
{
public:
	IGA_IDirectDrawSurface2(IDirectDrawSurface2*   pIDirectDrawSurface2);
	~IGA_IDirectDrawSurface2(void);

	virtual HRESULT __stdcall   QueryInterface (REFIID riid, LPVOID FAR * ppvObj) ;
	virtual ULONG   __stdcall   AddRef  () ;
	virtual ULONG   __stdcall   Release () ;

	virtual HRESULT __stdcall  AddAttachedSurface( LPDIRECTDRAWSURFACE2 );
	virtual HRESULT __stdcall  AddOverlayDirtyRect( LPRECT );  
	virtual HRESULT __stdcall  Blt(LPRECT, LPDIRECTDRAWSURFACE2, LPRECT, DWORD, LPDDBLTFX );
	virtual HRESULT __stdcall  BltBatch( LPDDBLTBATCH, DWORD, DWORD );
	virtual HRESULT __stdcall  BltFast( DWORD, DWORD, LPDIRECTDRAWSURFACE2, LPRECT, DWORD );
	virtual HRESULT __stdcall  DeleteAttachedSurface( DWORD, LPDIRECTDRAWSURFACE2 );
	virtual HRESULT __stdcall  EnumAttachedSurfaces( LPVOID, LPDDENUMSURFACESCALLBACK );
	virtual HRESULT __stdcall  EnumOverlayZOrders( DWORD, LPVOID, LPDDENUMSURFACESCALLBACK );
	virtual HRESULT __stdcall  Flip(LPDIRECTDRAWSURFACE2, DWORD );
	virtual HRESULT __stdcall  GetAttachedSurface( LPDDSCAPS, LPDIRECTDRAWSURFACE2 FAR *);
	virtual HRESULT __stdcall  GetBltStatus( DWORD );
	virtual HRESULT __stdcall  GetCaps( LPDDSCAPS );
	virtual HRESULT __stdcall  GetClipper( LPDIRECTDRAWCLIPPER FAR *);
	virtual HRESULT __stdcall  GetColorKey( DWORD, LPDDCOLORKEY );
	virtual HRESULT __stdcall  GetDC( HDC FAR *);
	virtual HRESULT __stdcall  GetFlipStatus( DWORD );
	virtual HRESULT __stdcall  GetOverlayPosition( LPLONG, LPLONG );
	virtual HRESULT __stdcall  GetPalette( LPDIRECTDRAWPALETTE FAR*);
	virtual HRESULT __stdcall  GetPixelFormat( LPDDPIXELFORMAT );
	virtual HRESULT __stdcall  GetSurfaceDesc( LPDDSURFACEDESC );
	virtual HRESULT __stdcall  Initialize( LPDIRECTDRAW, LPDDSURFACEDESC );
	virtual HRESULT __stdcall  IsLost();
	virtual HRESULT __stdcall  Lock( LPRECT, LPDDSURFACEDESC, DWORD, HANDLE );
	virtual HRESULT __stdcall  ReleaseDC( HDC );
	virtual HRESULT __stdcall  Restore();
	virtual HRESULT __stdcall  SetClipper( LPDIRECTDRAWCLIPPER );
	virtual HRESULT __stdcall  SetColorKey( DWORD, LPDDCOLORKEY );
	virtual HRESULT __stdcall  SetOverlayPosition( LONG, LONG );
	virtual HRESULT __stdcall  SetPalette( LPDIRECTDRAWPALETTE );
	virtual HRESULT __stdcall  Unlock( LPVOID );
	virtual HRESULT __stdcall  UpdateOverlay( LPRECT, LPDIRECTDRAWSURFACE2, LPRECT, DWORD, LPDDOVERLAYFX );
	virtual HRESULT __stdcall  UpdateOverlayDisplay( DWORD );
	virtual HRESULT __stdcall  UpdateOverlayZOrder( DWORD, LPDIRECTDRAWSURFACE2 );

	/*** Added in the v2 interface ****/
	virtual HRESULT __stdcall  GetDDInterface( LPVOID FAR *);
	virtual HRESULT __stdcall  PageLock( DWORD );
	virtual HRESULT __stdcall  PageUnlock( DWORD );

private:
	IDirectDrawSurface2*   m_pIDirectDrawSurface2;

};


class IGA_IDirectDrawSurface3 : public IDirectDrawSurface3
{
public:
	IGA_IDirectDrawSurface3(IDirectDrawSurface3*   pIDirectDrawSurface3);
	~IGA_IDirectDrawSurface3(void);

	virtual HRESULT __stdcall   QueryInterface (REFIID riid, LPVOID FAR * ppvObj) ;
	virtual ULONG   __stdcall   AddRef  () ;
	virtual ULONG   __stdcall   Release () ;

	virtual HRESULT __stdcall  AddAttachedSurface( LPDIRECTDRAWSURFACE3 );
	virtual HRESULT __stdcall  AddOverlayDirtyRect( LPRECT );  
	virtual HRESULT __stdcall  Blt(LPRECT, LPDIRECTDRAWSURFACE3, LPRECT, DWORD, LPDDBLTFX );
	virtual HRESULT __stdcall  BltBatch( LPDDBLTBATCH, DWORD, DWORD );
	virtual HRESULT __stdcall  BltFast( DWORD, DWORD, LPDIRECTDRAWSURFACE3, LPRECT, DWORD );
	virtual HRESULT __stdcall  DeleteAttachedSurface( DWORD, LPDIRECTDRAWSURFACE3 );
	virtual HRESULT __stdcall  EnumAttachedSurfaces( LPVOID, LPDDENUMSURFACESCALLBACK );
	virtual HRESULT __stdcall  EnumOverlayZOrders( DWORD, LPVOID, LPDDENUMSURFACESCALLBACK );
	virtual HRESULT __stdcall  Flip(LPDIRECTDRAWSURFACE3, DWORD );
	virtual HRESULT __stdcall  GetAttachedSurface( LPDDSCAPS, LPDIRECTDRAWSURFACE3 FAR *);
	virtual HRESULT __stdcall  GetBltStatus( DWORD );
	virtual HRESULT __stdcall  GetCaps( LPDDSCAPS );
	virtual HRESULT __stdcall  GetClipper( LPDIRECTDRAWCLIPPER FAR *);
	virtual HRESULT __stdcall  GetColorKey( DWORD, LPDDCOLORKEY );
	virtual HRESULT __stdcall  GetDC( HDC FAR *);
	virtual HRESULT __stdcall  GetFlipStatus( DWORD );
	virtual HRESULT __stdcall  GetOverlayPosition( LPLONG, LPLONG );
	virtual HRESULT __stdcall  GetPalette( LPDIRECTDRAWPALETTE FAR*);
	virtual HRESULT __stdcall  GetPixelFormat( LPDDPIXELFORMAT );
	virtual HRESULT __stdcall  GetSurfaceDesc( LPDDSURFACEDESC );
	virtual HRESULT __stdcall  Initialize( LPDIRECTDRAW, LPDDSURFACEDESC );
	virtual HRESULT __stdcall  IsLost();
	virtual HRESULT __stdcall  Lock( LPRECT, LPDDSURFACEDESC, DWORD, HANDLE );
	virtual HRESULT __stdcall  ReleaseDC( HDC );
	virtual HRESULT __stdcall  Restore();
	virtual HRESULT __stdcall  SetClipper( LPDIRECTDRAWCLIPPER );
	virtual HRESULT __stdcall  SetColorKey( DWORD, LPDDCOLORKEY );
	virtual HRESULT __stdcall  SetOverlayPosition( LONG, LONG );
	virtual HRESULT __stdcall  SetPalette( LPDIRECTDRAWPALETTE );
	virtual HRESULT __stdcall  Unlock( LPVOID );
	virtual HRESULT __stdcall  UpdateOverlay( LPRECT, LPDIRECTDRAWSURFACE3, LPRECT, DWORD, LPDDOVERLAYFX );
	virtual HRESULT __stdcall  UpdateOverlayDisplay( DWORD );
	virtual HRESULT __stdcall  UpdateOverlayZOrder( DWORD, LPDIRECTDRAWSURFACE3 );

	/*** Added in the v2 interface ****/
	virtual HRESULT __stdcall GetDDInterface(LPVOID FAR *);
	virtual HRESULT __stdcall PageLock( DWORD );
	virtual HRESULT __stdcall PageUnlock( DWORD );

	/*** Added in the v3 interface **/
	virtual HRESULT __stdcall SetSurfaceDesc(LPDDSURFACEDESC, DWORD );
private:
	IDirectDrawSurface3*   m_pIDirectDrawSurface3;

};

class IGA_IDirectDrawSurface4 : public IDirectDrawSurface4
{
public:
	IGA_IDirectDrawSurface4(IDirectDrawSurface4*   pIDirectDrawSurface4);
	~IGA_IDirectDrawSurface4(void);

	virtual HRESULT __stdcall   QueryInterface (REFIID riid, LPVOID FAR * ppvObj) ;
	virtual ULONG   __stdcall   AddRef  () ;
	virtual ULONG   __stdcall   Release () ;

	virtual HRESULT __stdcall  AddAttachedSurface( LPDIRECTDRAWSURFACE4 );
	virtual HRESULT __stdcall  AddOverlayDirtyRect( LPRECT );  
	virtual HRESULT __stdcall  Blt(LPRECT, LPDIRECTDRAWSURFACE4, LPRECT, DWORD, LPDDBLTFX );
	virtual HRESULT __stdcall  BltBatch( LPDDBLTBATCH, DWORD, DWORD );
	virtual HRESULT __stdcall  BltFast( DWORD, DWORD, LPDIRECTDRAWSURFACE4, LPRECT, DWORD );
	virtual HRESULT __stdcall  DeleteAttachedSurface( DWORD, LPDIRECTDRAWSURFACE4 );
	virtual HRESULT __stdcall  EnumAttachedSurfaces( LPVOID, LPDDENUMSURFACESCALLBACK2 );
	virtual HRESULT __stdcall  EnumOverlayZOrders( DWORD, LPVOID, LPDDENUMSURFACESCALLBACK2 );
	virtual HRESULT __stdcall  Flip(LPDIRECTDRAWSURFACE4, DWORD );
	virtual HRESULT __stdcall  GetAttachedSurface( LPDDSCAPS2, LPDIRECTDRAWSURFACE4 FAR *);
	virtual HRESULT __stdcall  GetBltStatus( DWORD );
	virtual HRESULT __stdcall  GetCaps( LPDDSCAPS2 );
	virtual HRESULT __stdcall  GetClipper( LPDIRECTDRAWCLIPPER FAR *);
	virtual HRESULT __stdcall  GetColorKey( DWORD, LPDDCOLORKEY );
	virtual HRESULT __stdcall  GetDC( HDC FAR *);
	virtual HRESULT __stdcall  GetFlipStatus( DWORD );
	virtual HRESULT __stdcall  GetOverlayPosition( LPLONG, LPLONG );
	virtual HRESULT __stdcall  GetPalette( LPDIRECTDRAWPALETTE FAR*);
	virtual HRESULT __stdcall  GetPixelFormat( LPDDPIXELFORMAT );
	virtual HRESULT __stdcall  GetSurfaceDesc( LPDDSURFACEDESC2 );
	virtual HRESULT __stdcall  Initialize( LPDIRECTDRAW, LPDDSURFACEDESC2 );
	virtual HRESULT __stdcall  IsLost();
	virtual HRESULT __stdcall  Lock( LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE );
	virtual HRESULT __stdcall  ReleaseDC( HDC );
	virtual HRESULT __stdcall  Restore();
	virtual HRESULT __stdcall  SetClipper( LPDIRECTDRAWCLIPPER );
	virtual HRESULT __stdcall  SetColorKey( DWORD, LPDDCOLORKEY );
	virtual HRESULT __stdcall  SetOverlayPosition( LONG, LONG );
	virtual HRESULT __stdcall  SetPalette( LPDIRECTDRAWPALETTE );
	virtual HRESULT __stdcall  Unlock( LPRECT );
	virtual HRESULT __stdcall  UpdateOverlay( LPRECT, LPDIRECTDRAWSURFACE4, LPRECT, DWORD, LPDDOVERLAYFX );
	virtual HRESULT __stdcall  UpdateOverlayDisplay( DWORD );
	virtual HRESULT __stdcall  UpdateOverlayZOrder( DWORD, LPDIRECTDRAWSURFACE4 );

	/** Addid in the v2 interface***/
	virtual HRESULT __stdcall  GetDDInterface( LPVOID FAR *);
	virtual HRESULT __stdcall  PageLock( DWORD );
	virtual HRESULT __stdcall  PageUnlock( DWORD );

	/*** Added in the v3 interface ****/
	virtual HRESULT __stdcall  SetSurfaceDesc( LPDDSURFACEDESC2, DWORD );
	/**Added in the v4 interface ***/
	virtual HRESULT __stdcall  SetPrivateData( REFGUID, LPVOID, DWORD, DWORD );
	virtual HRESULT __stdcall  GetPrivateData( REFGUID, LPVOID, LPDWORD );
	virtual HRESULT __stdcall  FreePrivateData( REFGUID );
	virtual HRESULT __stdcall  GetUniquenessValue( LPDWORD );
	virtual HRESULT __stdcall  ChangeUniquenessValue( );

private:
	IDirectDrawSurface4*   m_pIDirectDrawSurface4;

};

//////////////////////////////////////////////////////////////////////////

class IGA_IDirectDrawSurface7 : public IDirectDrawSurface7
{
public:
	IGA_IDirectDrawSurface7(IDirectDrawSurface7*   pIDirectDrawSurface7);
	~IGA_IDirectDrawSurface7(void);

	virtual HRESULT __stdcall   QueryInterface (REFIID riid, LPVOID FAR * ppvObj) ;
	virtual ULONG   __stdcall   AddRef  () ;
	virtual ULONG   __stdcall   Release () ;

	virtual HRESULT __stdcall  AddAttachedSurface( LPDIRECTDRAWSURFACE7 );
	virtual HRESULT __stdcall  AddOverlayDirtyRect( LPRECT );  
	virtual HRESULT __stdcall  Blt(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX );
	virtual HRESULT __stdcall  BltBatch( LPDDBLTBATCH, DWORD, DWORD );
	virtual HRESULT __stdcall  BltFast( DWORD, DWORD, LPDIRECTDRAWSURFACE7, LPRECT, DWORD );
	virtual HRESULT __stdcall  DeleteAttachedSurface( DWORD, LPDIRECTDRAWSURFACE7 );
	virtual HRESULT __stdcall  EnumAttachedSurfaces( LPVOID, LPDDENUMSURFACESCALLBACK7 );
	virtual HRESULT __stdcall  EnumOverlayZOrders( DWORD, LPVOID, LPDDENUMSURFACESCALLBACK7 );
	virtual HRESULT __stdcall  Flip(LPDIRECTDRAWSURFACE7, DWORD );
	virtual HRESULT __stdcall  GetAttachedSurface( LPDDSCAPS2, LPDIRECTDRAWSURFACE7 FAR *);
	virtual HRESULT __stdcall  GetBltStatus( DWORD );
	virtual HRESULT __stdcall  GetCaps( LPDDSCAPS2 );
	virtual HRESULT __stdcall  GetClipper( LPDIRECTDRAWCLIPPER FAR *);
	virtual HRESULT __stdcall  GetColorKey( DWORD, LPDDCOLORKEY );
	virtual HRESULT __stdcall  GetDC( HDC FAR *);
	virtual HRESULT __stdcall  GetFlipStatus( DWORD );
	virtual HRESULT __stdcall  GetOverlayPosition( LPLONG, LPLONG );
	virtual HRESULT __stdcall  GetPalette( LPDIRECTDRAWPALETTE FAR*);
	virtual HRESULT __stdcall  GetPixelFormat( LPDDPIXELFORMAT );
	virtual HRESULT __stdcall  GetSurfaceDesc( LPDDSURFACEDESC2 );
	virtual HRESULT __stdcall  Initialize( LPDIRECTDRAW, LPDDSURFACEDESC2 );
	virtual HRESULT __stdcall  IsLost();
	virtual HRESULT __stdcall  Lock( LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE );
	virtual HRESULT __stdcall  ReleaseDC( HDC );
	virtual HRESULT __stdcall  Restore();
	virtual HRESULT __stdcall  SetClipper( LPDIRECTDRAWCLIPPER );
	virtual HRESULT __stdcall  SetColorKey( DWORD, LPDDCOLORKEY );
	virtual HRESULT __stdcall  SetOverlayPosition( LONG, LONG );
	virtual HRESULT __stdcall  SetPalette( LPDIRECTDRAWPALETTE );
	virtual HRESULT __stdcall  Unlock( LPRECT );
	virtual HRESULT __stdcall  UpdateOverlay( LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDOVERLAYFX );
	virtual HRESULT __stdcall  UpdateOverlayDisplay( DWORD );
	virtual HRESULT __stdcall  UpdateOverlayZOrder( DWORD, LPDIRECTDRAWSURFACE7 );

	/** Addid in the v2 interface***/
	virtual HRESULT __stdcall  GetDDInterface( LPVOID FAR *);
	virtual HRESULT __stdcall  PageLock( DWORD );
	virtual HRESULT __stdcall  PageUnlock( DWORD );

	/*** Added in the v3 interface ****/
	virtual HRESULT __stdcall  SetSurfaceDesc( LPDDSURFACEDESC2, DWORD );
	/**Added in the v4 interface ***/
	virtual HRESULT __stdcall  SetPrivateData( REFGUID, LPVOID, DWORD, DWORD );
	virtual HRESULT __stdcall  GetPrivateData( REFGUID, LPVOID, LPDWORD );
	virtual HRESULT __stdcall  FreePrivateData( REFGUID );
	virtual HRESULT __stdcall  GetUniquenessValue( LPDWORD );
	virtual HRESULT __stdcall  ChangeUniquenessValue( );
	/** Moved Texture7 methods here****/
	virtual HRESULT __stdcall  SetPriority( DWORD );
	virtual HRESULT __stdcall  GetPriority( LPDWORD );
	virtual HRESULT __stdcall  SetLOD( DWORD );
	virtual HRESULT __stdcall  GetLOD( LPDWORD );
private:
	IDirectDrawSurface7*   m_pIDirectDrawSurface7;

};