#pragma once
//#include "iga.h"
// This one is set up to work with IDirectDraw7 only !!


typedef DWORD	VFT;	// Virtual Function Table Type
typedef VFT		*LPVFT;	// Point to Virtual function table 

typedef void	*LPANYOBJECT;

#define	MAX_VFT_SIZE	1024

/// Common virtual object
typedef	struct tagVObject
{
	LPVFT	pVft;
}VOBJECT, *LPVOBJECT;

//	将拥有虚函数的一般C++对象转换成 VOBJECT;
#	define	 CPVO2PIVO(pObject)	((VOBJECT*)(LPVOID)(pObject))


// 写只读的内存
extern BOOL WriteMemory(LPCVOID lpAddress, LPCVOID lpData, DWORD dwBytesToWrite);


// 获得对象的虚函数个数
// extern DWORD GetVirtualFunctionNumber(LPVOID pObject);
extern DWORD GetVirtualFunctionNumber(const LPVOBJECT pObject);

// 替换虚函数表
// 将 pObject 的虚函数表，拷贝到 pVFT 中。
// nVftSize：pVFT 中可以存放的函数指针的个数
extern DWORD CopyVFT(LPVFT pVFT, DWORD dwVFTSize, const LPVOBJECT pVObject);

// 名称： VOID ReplaceVFT(LPVOBJECT pTargetObject, LPVOBJECT pObject);
// 功能： 替换虚函数表
// 描述： 将 pTargetObject 的虚函数表，替换为 pObject 的虚函数表。
extern DWORD ReplaceVFT(LPVOBJECT pTargetVObject, const LPVOBJECT pVObject);

// 名称： VOID ReplaceN_VFT(LPVOBJECT pTargetObject, LPVOBJECT pObject, DWORD dwVFTSize);
// 功能： 替换虚函数表
// 描述： 将 pTargetObject 的虚函数表，替换为 pObject 的虚函数表。
//	       dwVFTSize 替换的虚函数个数
extern DWORD ReplaceN_VFT(LPVOBJECT pTargetVObject, const LPVOBJECT pVObject, DWORD dwVFTSize);

#	define	DECLARE_CLASS_BEGIN(classname)												\
struct	IGA_##classname : public classname											\
{																					\
	classname	*m_p##classname;													\
public:																			\
	IGA_##classname():	m_p##classname(NULL){}									\
	IGA_##classname(classname *p##classname);									\
	virtual ~IGA_##classname();			
#	define	DECLARE_CLASS_END()															\
};

#	define	DECLARE_CLASS_BEGIN_EX(classname)											\
struct	IGA_##classname : public classname											\
{																					\
	classname	*m_p##classname;													\
public:																			\
	IGA_##classname():	m_p##classname(NULL){}									\
	IGA_##classname(classname *p##classname)									\
	:m_p##classname(p##classname){}											\
	virtual ~IGA_##classname()	{}	

///////////
#	define	IGA_DECLARE_CLASS_BEGIN(class_name)											\
struct	IGA_##class_name : public class_name										\
{																					\
	static IGA_##class_name *GetInstance()											\
	{																				\
		static IGA_##class_name	theObject;											\
		return	&theObject;															\
	}																				\
	static	VFT   s_VirtualFuncTable[MAX_VFT_SIZE];	/*	虚函数指针表*/			\
	static  BOOL  Init(class_name *pObject);	/*	初始化自定义类*/									


#	define	IGA_DECLARE_CLASS_END()					};	

///	Definitions of member	
#	define	DECLARE_MEMBER_FUNCTION(lRet, Function, Params)								\
	virtual lRet __stdcall Function##Params;								


#	define	DECLARE_MEMBER_FUNCTION_HELPER(lRet, Function, Params)						\
	typedef lRet (__stdcall * LPFN_##Function)Params;									\
	static	LPFN_##Function	m_pfn##Function;

#	define	DECLARE_MEMBER_FUNCTION_EX(lRet, Function, Params1, Params2)				\
	DECLARE_MEMBER_FUNCTION(lRet, Function, Params1);									\
	DECLARE_MEMBER_FUNCTION_HELPER(lRet, Function, Params2);	


#	define	IMPLEMENT_MEMBER_FUNCTION_HELPER0(lRet, classname, Function, Params)		\
	lRet __stdcall IGA_##classname::Function##Params								

#	define	IMPLEMENT_MEMBER_FUNCTION_HELPER1(Function, Params)							\
	{	return	m_pfn##Function##Params;	}

#	define	IMPLEMENT_MEMBER_FUNCTION_HELPER2(classname, Function)						\
	IGA_##classname::LPFN_##Function	IGA_##classname::m_pfn##Function	= NULL;	 

#	define	IMPLEMENT_MEMBER_FUNCTION_HELPER_VOID(Function, Params)						\
	{	m_pfn##Function##Params;	}

#	define	IMPLEMENT_MEMBER_FUNCTION(lRet, classname, Function, Params, Params1)		\
	IMPLEMENT_MEMBER_FUNCTION_HELPER0(lRet, classname, Function, Params)				\
	IMPLEMENT_MEMBER_FUNCTION_HELPER1(Function, Params1);								\
	IMPLEMENT_MEMBER_FUNCTION_HELPER2(classname, Function);														


#	define	IMPLEMENT_MEMBER_FUNCTION_VOID(classname, Function, Params, Params1)		\
	IMPLEMENT_MEMBER_FUNCTION_HELPER0(void, classname, Function, Params)				\
	IMPLEMENT_MEMBER_FUNCTION_HELPER_VOID(Function, Params1);							\
	IMPLEMENT_MEMBER_FUNCTION_HELPER2(classname, Function);														

#	define	IMPLEMENT_MEMBER_FUNCTION_EX(lRet, classname, Function, Params1, Params2)	\
	lRet __stdcall IGA_##classname::Function##Params1									\
	{	return	m_p##classname->Function##Params2;	}


#	define	IGA_IMPLEMENT_CLASS(classname)												\
	VFT		IGA_##classname::s_VirtualFuncTable[MAX_VFT_SIZE]	= { 0 };	

#define		FILL_VIRTUAL_FUNC_TABLE(dwIndex, Function)									\
	ASSERT(dwIndex < MAX_VIRTUAL_FUNC_COUNT);											\
	m_pfn##Function	= (LPFN_##Function)(s_VirtualFuncTable[(DWORD)(dwIndex++)]);


#	define	INIT_VIRTUAL_OBJECT(pObject)												\
	CopyVFT(s_VirtualFuncTable, MAX_VFT_SIZE, CPVO2PIVO(pObject));					    \
	ReplaceN_VFT(CPVO2PIVO(pObject), CPVO2PIVO(GetInstance()), MAX_VIRTUAL_FUNC_COUNT);

//#	define	INIT_VIRTUAL_OBJECT(pObject)												\
	CopyVFT(s_VirtualFuncTable, MAX_VFT_SIZE, CPVO2PIVO(pObject));						\
	ReplaceN_VFT(CPVO2PIVO(pObject), CPVO2PIVO(GetInstance()), MAX_VIRTUAL_FUNC_COUNT);


const GUID Proxy_IDirectDraw   = {0x6C14DB80,0xA733,0x11CE,{0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60 }};
const GUID Proxy_IDirectDraw2  = {0xB3A6F3E0,0x2B43,0x11CF,{0xA2,0xDE,0x00,0xAA,0x00,0xB9,0x33,0x56 }};
const GUID Proxy_IDirectDraw4  = {0x9c59509a,0x39bd,0x11d1,{0x8c,0x4a,0x00,0xc0,0x4f,0xd9,0x30,0xc5 }};
const GUID Proxy_IDirectDraw7  = {0x15e65ec0,0x3b9c,0x11d2,{0xb9,0x2f,0x00,0x60,0x97,0x97,0xea,0x5b }};

const GUID Proxy_IDirectDrawSurface  = {0x6C14DB81,0xA733,0x11CE,{0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60}};
const GUID Proxy_IDirectDrawSurface2 = {0x57805885,0x6eec,0x11cf,{0x94,0x41,0xa8,0x23,0x03,0xc1,0x0e,0x27}};
const GUID Proxy_IDirectDrawSurface3 = {0xDA044E00,0x69B2,0x11D0,{0xA1,0xD5,0x00,0xAA,0x00,0xB8,0xDF,0xBB}};
const GUID Proxy_IDirectDrawSurface4 = {0x0B2B8630,0xAD35,0x11D0,{0x8E,0xA6,0x00,0x60,0x97,0x97,0xEA,0x5B}};
const GUID Proxy_IDirectDrawSurface7 = {0x06675a80,0x3b9b,0x11d2,{0xb9,0x2f,0x00,0x60,0x97,0x97,0xea,0x5b}};


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
	
	IGA   *m_pIGADraw;

private:		    
	IDirectDraw*   m_pIDirectDraw;
};

/////////////////////////////////////////////////////////////////////////
IGA_DECLARE_CLASS_BEGIN(IDirectDrawSurface)
	enum
	{
		MAX_VIRTUAL_FUNC_COUNT	= 36,
	};

	/*** IUnknown methods ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, QueryInterface, (REFIID riid, void** ppvObj), (IDirectDrawSurface *, REFIID riid, void** ppvObj));
	DECLARE_MEMBER_FUNCTION_EX(ULONG,	AddRef, (void), (IDirectDrawSurface *));
	DECLARE_MEMBER_FUNCTION_EX(ULONG,	Release, (void), (IDirectDrawSurface *));

	/*** IDirectDrawSurface methods ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT,AddAttachedSurface, (LPDIRECTDRAWSURFACE), (IDirectDrawSurface *, LPDIRECTDRAWSURFACE));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, AddOverlayDirtyRect, (LPRECT), (IDirectDrawSurface *, LPRECT));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Blt, (LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDBLTFX), (IDirectDrawSurface *, LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDBLTFX));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, BltBatch, (LPDDBLTBATCH, DWORD, DWORD), (IDirectDrawSurface *, LPDDBLTBATCH, DWORD, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, BltFast, (DWORD,DWORD,LPDIRECTDRAWSURFACE, LPRECT,DWORD), (IDirectDrawSurface *, DWORD,DWORD,LPDIRECTDRAWSURFACE, LPRECT,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, DeleteAttachedSurface, (DWORD,LPDIRECTDRAWSURFACE), (IDirectDrawSurface *, DWORD,LPDIRECTDRAWSURFACE));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, EnumAttachedSurfaces, (LPVOID,LPDDENUMSURFACESCALLBACK), (IDirectDrawSurface *, LPVOID,LPDDENUMSURFACESCALLBACK));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, EnumOverlayZOrders, (DWORD,LPVOID,LPDDENUMSURFACESCALLBACK), (IDirectDrawSurface *, DWORD,LPVOID,LPDDENUMSURFACESCALLBACK));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Flip, (LPDIRECTDRAWSURFACE, DWORD), (IDirectDrawSurface *,LPDIRECTDRAWSURFACE, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetAttachedSurface, (LPDDSCAPS, LPDIRECTDRAWSURFACE FAR *), (IDirectDrawSurface *,LPDDSCAPS, LPDIRECTDRAWSURFACE FAR *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetBltStatus, (DWORD), (IDirectDrawSurface *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetCaps, (LPDDSCAPS), (IDirectDrawSurface *,LPDDSCAPS));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetClipper, (LPDIRECTDRAWCLIPPER FAR*), (IDirectDrawSurface *,LPDIRECTDRAWCLIPPER FAR*));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetColorKey, (DWORD, LPDDCOLORKEY), (IDirectDrawSurface *,DWORD, LPDDCOLORKEY));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetDC, (HDC FAR *), (IDirectDrawSurface *,HDC FAR *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetFlipStatus, (DWORD), (IDirectDrawSurface *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetOverlayPosition, (LPLONG, LPLONG), (IDirectDrawSurface *,LPLONG, LPLONG));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetPalette, (LPDIRECTDRAWPALETTE FAR*), (IDirectDrawSurface *,LPDIRECTDRAWPALETTE FAR*));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetPixelFormat, (LPDDPIXELFORMAT), (IDirectDrawSurface *,LPDDPIXELFORMAT));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetSurfaceDesc, (LPDDSURFACEDESC), (IDirectDrawSurface *,LPDDSURFACEDESC));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Initialize, (LPDIRECTDRAW, LPDDSURFACEDESC), (IDirectDrawSurface *,LPDIRECTDRAW, LPDDSURFACEDESC));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, IsLost, (void), (IDirectDrawSurface *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Lock, (LPRECT,LPDDSURFACEDESC,DWORD,HANDLE), (IDirectDrawSurface *,LPRECT,LPDDSURFACEDESC,DWORD,HANDLE));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, ReleaseDC, (HDC), (IDirectDrawSurface *,HDC));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Restore, (void), (IDirectDrawSurface *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetClipper, (LPDIRECTDRAWCLIPPER), (IDirectDrawSurface *,LPDIRECTDRAWCLIPPER));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetColorKey, (DWORD, LPDDCOLORKEY), (IDirectDrawSurface *,DWORD, LPDDCOLORKEY));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetOverlayPosition, (LONG, LONG), (IDirectDrawSurface *,LONG, LONG));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetPalette, (LPDIRECTDRAWPALETTE), (IDirectDrawSurface *,LPDIRECTDRAWPALETTE));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Unlock, (LPVOID), (IDirectDrawSurface *,LPVOID));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, UpdateOverlay, (LPRECT, LPDIRECTDRAWSURFACE,LPRECT,DWORD, LPDDOVERLAYFX), (IDirectDrawSurface *,LPRECT, LPDIRECTDRAWSURFACE,LPRECT,DWORD, LPDDOVERLAYFX));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, UpdateOverlayDisplay, (DWORD), (IDirectDrawSurface *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, UpdateOverlayZOrder, (DWORD, LPDIRECTDRAWSURFACE), (IDirectDrawSurface *,DWORD, LPDIRECTDRAWSURFACE));


IGA_DECLARE_CLASS_END();

//////////////////////////////////////////////////////////////////////////

IGA_DECLARE_CLASS_BEGIN(IDirectDrawPalette)
	enum
	{
		MAX_VIRTUAL_FUNC_COUNT	= 7,
	};
	/*** IUnknown methods ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, QueryInterface, (REFIID riid, void** ppvObj), (IDirectDrawPalette *, REFIID riid, void** ppvObj));
	DECLARE_MEMBER_FUNCTION_EX(ULONG,	AddRef, (void), (IDirectDrawPalette *));
	DECLARE_MEMBER_FUNCTION_EX(ULONG,	Release, (void), (IDirectDrawPalette *));
	/*** IDirectDrawSurface methods ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetCaps, (LPDWORD lpword),(IDirectDrawPalette *, LPDWORD lpword));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetEntries, (DWORD, DWORD, DWORD, LPPALETTEENTRY),(IDirectDrawPalette *, DWORD, DWORD, DWORD, LPPALETTEENTRY));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Initialize, (LPDIRECTDRAW, DWORD, LPPALETTEENTRY),(IDirectDrawPalette *, LPDIRECTDRAW, DWORD, LPPALETTEENTRY ));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetEntries, (DWORD, DWORD, DWORD, LPPALETTEENTRY),(IDirectDrawPalette *,DWORD, DWORD, DWORD, LPPALETTEENTRY ));
IGA_DECLARE_CLASS_END();



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
	IDirectDraw4*      m_pIDirectDraw4;
};


/////////////////////////////////////////////////////////////////////////
IGA_DECLARE_CLASS_BEGIN(IDirectDrawSurface4)
	enum
	{
		MAX_VIRTUAL_FUNC_COUNT	= 45,
	};
//
//	/*** IUnknown methods ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, QueryInterface, (REFIID riid, LPVOID FAR* ppvObj), (IDirectDrawSurface4 *, REFIID riid, LPVOID FAR* ppvObj));
	DECLARE_MEMBER_FUNCTION_EX(ULONG,	AddRef, (void), (IDirectDrawSurface4 *));
	DECLARE_MEMBER_FUNCTION_EX(ULONG,	Release, (void), (IDirectDrawSurface4 *));
	
	/*** IDirectDrawSurface methods ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT,AddAttachedSurface, (LPDIRECTDRAWSURFACE4), (IDirectDrawSurface4 *, LPDIRECTDRAWSURFACE4));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, AddOverlayDirtyRect, (LPRECT), (IDirectDrawSurface4 *, LPRECT));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Blt, (LPRECT, LPDIRECTDRAWSURFACE4, LPRECT, DWORD, LPDDBLTFX), (IDirectDrawSurface4 *, LPRECT, LPDIRECTDRAWSURFACE4, LPRECT, DWORD, LPDDBLTFX));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, BltBatch, (LPDDBLTBATCH, DWORD, DWORD), (IDirectDrawSurface4 *, LPDDBLTBATCH, DWORD, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, BltFast, (DWORD,DWORD,LPDIRECTDRAWSURFACE4, LPRECT,DWORD), (IDirectDrawSurface4 *, DWORD,DWORD,LPDIRECTDRAWSURFACE4, LPRECT,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, DeleteAttachedSurface, (DWORD,LPDIRECTDRAWSURFACE4), (IDirectDrawSurface4 *, DWORD,LPDIRECTDRAWSURFACE4));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, EnumAttachedSurfaces, (LPVOID,LPDDENUMSURFACESCALLBACK2), (IDirectDrawSurface4 *, LPVOID,LPDDENUMSURFACESCALLBACK2));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, EnumOverlayZOrders, (DWORD,LPVOID,LPDDENUMSURFACESCALLBACK2), (IDirectDrawSurface4 *, DWORD,LPVOID,LPDDENUMSURFACESCALLBACK2));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Flip, (LPDIRECTDRAWSURFACE4, DWORD), (IDirectDrawSurface4 *,LPDIRECTDRAWSURFACE4, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetAttachedSurface, (LPDDSCAPS2, LPDIRECTDRAWSURFACE4 FAR *), (IDirectDrawSurface4 *,LPDDSCAPS2, LPDIRECTDRAWSURFACE4 FAR *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetBltStatus, (DWORD), (IDirectDrawSurface4 *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetCaps, (LPDDSCAPS2), (IDirectDrawSurface4*, LPDDSCAPS2));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetClipper, (LPDIRECTDRAWCLIPPER FAR*), (IDirectDrawSurface4 *,LPDIRECTDRAWCLIPPER FAR*));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetColorKey, (DWORD, LPDDCOLORKEY), (IDirectDrawSurface4 *,DWORD, LPDDCOLORKEY));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetDC, (HDC FAR *), (IDirectDrawSurface4 *,HDC FAR *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetFlipStatus, (DWORD), (IDirectDrawSurface4 *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetOverlayPosition, (LPLONG, LPLONG), (IDirectDrawSurface4 *,LPLONG, LPLONG));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetPalette, (LPDIRECTDRAWPALETTE FAR*), (IDirectDrawSurface4 *,LPDIRECTDRAWPALETTE FAR*));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetPixelFormat, (LPDDPIXELFORMAT), (IDirectDrawSurface4 *,LPDDPIXELFORMAT));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetSurfaceDesc, (LPDDSURFACEDESC2), (IDirectDrawSurface4 *,LPDDSURFACEDESC2));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Initialize, (LPDIRECTDRAW, LPDDSURFACEDESC2), (IDirectDrawSurface4 *,LPDIRECTDRAW, LPDDSURFACEDESC2));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, IsLost, (void), (IDirectDrawSurface4 *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Lock, (LPRECT,LPDDSURFACEDESC2,DWORD,HANDLE), (IDirectDrawSurface4 *,LPRECT,LPDDSURFACEDESC2,DWORD,HANDLE));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, ReleaseDC, (HDC), (IDirectDrawSurface4 *,HDC));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Restore, (void), (IDirectDrawSurface4 *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetClipper, (LPDIRECTDRAWCLIPPER), (IDirectDrawSurface4 *,LPDIRECTDRAWCLIPPER));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetColorKey, (DWORD, LPDDCOLORKEY), (IDirectDrawSurface4 *,DWORD, LPDDCOLORKEY));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetOverlayPosition, (LONG, LONG), (IDirectDrawSurface4 *,LONG, LONG));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetPalette, (LPDIRECTDRAWPALETTE), (IDirectDrawSurface4 *,LPDIRECTDRAWPALETTE));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Unlock, (LPRECT), (IDirectDrawSurface4 *, LPRECT));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, UpdateOverlay, (LPRECT, LPDIRECTDRAWSURFACE4,LPRECT,DWORD, LPDDOVERLAYFX), (IDirectDrawSurface4 *,LPRECT, LPDIRECTDRAWSURFACE4,LPRECT,DWORD, LPDDOVERLAYFX));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, UpdateOverlayDisplay, (DWORD), (IDirectDrawSurface4 *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, UpdateOverlayZOrder, (DWORD, LPDIRECTDRAWSURFACE4), (IDirectDrawSurface4 *,DWORD, LPDIRECTDRAWSURFACE4));

	/**  Added in the v2 interface**/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetDDInterface, (LPVOID FAR *), (IDirectDrawSurface4 *,LPVOID FAR *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, PageLock, (DWORD), (IDirectDrawSurface4 *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, PageUnlock, (DWORD), (IDirectDrawSurface4 *,DWORD));
	/**  Added in the v3 interface ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetSurfaceDesc, (LPDDSURFACEDESC2, DWORD ), (IDirectDrawSurface4 *, LPDDSURFACEDESC2, DWORD));
	/**Added in the v4 interface ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetPrivateData, (REFGUID, LPVOID, DWORD, DWORD), (IDirectDrawSurface4 *,REFGUID, LPVOID, DWORD, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetPrivateData, (REFGUID, LPVOID, LPDWORD), (IDirectDrawSurface4 *,REFGUID, LPVOID, LPDWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, FreePrivateData, (REFGUID), (IDirectDrawSurface4 *,REFGUID));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetUniquenessValue, (LPDWORD), (IDirectDrawSurface4 *,LPDWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, ChangeUniquenessValue, (void), (IDirectDrawSurface4 *));

IGA_DECLARE_CLASS_END();


/*
*继承IDirectDraw7
*/
class IGA_IDirectDraw7 : public IDirectDraw7
{
public:
    IGA_IDirectDraw7(LPDIRECTDRAW7 pOriginal);
    virtual ~IGA_IDirectDraw7(void);

	// The original DDraw function definitions BEGIN
    HRESULT __stdcall QueryInterface (REFIID a, LPVOID FAR * b);
    ULONG   __stdcall AddRef(void);
    ULONG   __stdcall Release(void);
    
    HRESULT  __stdcall Compact(void);
    HRESULT  __stdcall CreateClipper(DWORD a, LPDIRECTDRAWCLIPPER FAR* b, IUnknown FAR* c);
	HRESULT  __stdcall CreatePalette(DWORD a, LPPALETTEENTRY b, LPDIRECTDRAWPALETTE FAR* c, IUnknown FAR* d);
	HRESULT  __stdcall CreateSurface(LPDDSURFACEDESC2 a, LPDIRECTDRAWSURFACE7 FAR* b, IUnknown FAR* c);
	HRESULT  __stdcall DuplicateSurface(LPDIRECTDRAWSURFACE7 a, LPDIRECTDRAWSURFACE7 FAR* b);
	HRESULT  __stdcall EnumDisplayModes(DWORD a, LPDDSURFACEDESC2 b, LPVOID c, LPDDENUMMODESCALLBACK2 d);
	HRESULT  __stdcall EnumSurfaces(DWORD a, LPDDSURFACEDESC2 b, LPVOID c, LPDDENUMSURFACESCALLBACK7 d);
	HRESULT  __stdcall FlipToGDISurface(void);
	HRESULT  __stdcall GetCaps(LPDDCAPS a, LPDDCAPS b);
    HRESULT  __stdcall GetDisplayMode(LPDDSURFACEDESC2 a);
    HRESULT  __stdcall GetFourCCCodes(LPDWORD a, LPDWORD b);
    HRESULT  __stdcall GetGDISurface(LPDIRECTDRAWSURFACE7 FAR * a);
    HRESULT  __stdcall GetMonitorFrequency(LPDWORD a);
    HRESULT  __stdcall GetScanLine(LPDWORD a);
    HRESULT  __stdcall GetVerticalBlankStatus(LPBOOL a);
    HRESULT  __stdcall Initialize(GUID FAR* a);
    HRESULT  __stdcall RestoreDisplayMode(void);
	HRESULT  __stdcall SetCooperativeLevel(HWND a, DWORD b);
	HRESULT  __stdcall SetDisplayMode(DWORD a, DWORD b, DWORD c, DWORD d, DWORD e);
	HRESULT  __stdcall WaitForVerticalBlank(DWORD a, HANDLE b);

	/*** Added in the v2 interface ***/
    HRESULT  __stdcall GetAvailableVidMem(LPDDSCAPS2 a, LPDWORD b, LPDWORD c);

    /*** Added in the V4 Interface ***/
    HRESULT  __stdcall GetSurfaceFromDC(HDC a, LPDIRECTDRAWSURFACE7 *b);
    HRESULT  __stdcall RestoreAllSurfaces(void);
    HRESULT  __stdcall TestCooperativeLevel(void);
    HRESULT  __stdcall GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2 a, DWORD b);
    HRESULT  __stdcall StartModeTest(LPSIZE a, DWORD b, DWORD c);
    HRESULT  __stdcall EvaluateMode(DWORD a, DWORD *b);
	void ShowWeAreHere(void);
    // The original DDraw function definitions END
	IGA   *m_pIGADraw;
	
private:
	
	LPDIRECTDRAW7  m_pIDDraw7;
};



//////////////////////////////////////////////////////////////////////////
/*
*IDirectDrawSurface7
*/
IGA_DECLARE_CLASS_BEGIN(IDirectDrawSurface7)
	enum
	{
		MAX_VIRTUAL_FUNC_COUNT	= 49,
	};

	/*** IUnknown methods ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, QueryInterface, (REFIID riid, void** ppvObj), (IDirectDrawSurface7 *, REFIID riid, void** ppvObj));
	DECLARE_MEMBER_FUNCTION_EX(ULONG,	AddRef, (void), (IDirectDrawSurface7 *));
	DECLARE_MEMBER_FUNCTION_EX(ULONG,	Release, (void), (IDirectDrawSurface7 *));
	
	/*** IDirectDrawSurface methods ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT,AddAttachedSurface, (LPDIRECTDRAWSURFACE7), (IDirectDrawSurface7 *, LPDIRECTDRAWSURFACE7));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, AddOverlayDirtyRect, (LPRECT), (IDirectDrawSurface7 *, LPRECT));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Blt, (LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX), (IDirectDrawSurface7 *, LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, BltBatch, (LPDDBLTBATCH, DWORD, DWORD), (IDirectDrawSurface7 *, LPDDBLTBATCH, DWORD, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, BltFast, (DWORD,DWORD,LPDIRECTDRAWSURFACE7, LPRECT,DWORD), (IDirectDrawSurface7 *, DWORD,DWORD,LPDIRECTDRAWSURFACE7, LPRECT,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, DeleteAttachedSurface, (DWORD,LPDIRECTDRAWSURFACE7), (IDirectDrawSurface7 *, DWORD,LPDIRECTDRAWSURFACE7));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, EnumAttachedSurfaces, (LPVOID,LPDDENUMSURFACESCALLBACK7), (IDirectDrawSurface7 *, LPVOID,LPDDENUMSURFACESCALLBACK7));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, EnumOverlayZOrders, (DWORD,LPVOID,LPDDENUMSURFACESCALLBACK7), (IDirectDrawSurface7 *, DWORD,LPVOID,LPDDENUMSURFACESCALLBACK7));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Flip, (LPDIRECTDRAWSURFACE7, DWORD), (IDirectDrawSurface7 *,LPDIRECTDRAWSURFACE7, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetAttachedSurface, (LPDDSCAPS2, LPDIRECTDRAWSURFACE7 FAR *), (IDirectDrawSurface7 *,LPDDSCAPS2, LPDIRECTDRAWSURFACE7 FAR *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetBltStatus, (DWORD), (IDirectDrawSurface7 *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetCaps, (LPDDSCAPS2), (IDirectDrawSurface7 *,LPDDSCAPS2));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetClipper, (LPDIRECTDRAWCLIPPER FAR*), (IDirectDrawSurface7 *,LPDIRECTDRAWCLIPPER FAR*));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetColorKey, (DWORD, LPDDCOLORKEY), (IDirectDrawSurface7 *,DWORD, LPDDCOLORKEY));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetDC, (HDC FAR *), (IDirectDrawSurface7 *,HDC FAR *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetFlipStatus, (DWORD), (IDirectDrawSurface7 *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetOverlayPosition, (LPLONG, LPLONG), (IDirectDrawSurface7 *,LPLONG, LPLONG));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetPalette, (LPDIRECTDRAWPALETTE FAR*), (IDirectDrawSurface7 *,LPDIRECTDRAWPALETTE FAR*));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetPixelFormat, (LPDDPIXELFORMAT), (IDirectDrawSurface7 *,LPDDPIXELFORMAT));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetSurfaceDesc, (LPDDSURFACEDESC2), (IDirectDrawSurface7 *,LPDDSURFACEDESC2));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Initialize, (LPDIRECTDRAW, LPDDSURFACEDESC2), (IDirectDrawSurface7 *,LPDIRECTDRAW, LPDDSURFACEDESC2));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, IsLost, (void), (IDirectDrawSurface7 *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Lock, (LPRECT,LPDDSURFACEDESC2,DWORD,HANDLE), (IDirectDrawSurface7 *,LPRECT,LPDDSURFACEDESC2,DWORD,HANDLE));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, ReleaseDC, (HDC), (IDirectDrawSurface7 *,HDC));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Restore, (void), (IDirectDrawSurface7 *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetClipper, (LPDIRECTDRAWCLIPPER), (IDirectDrawSurface7 *,LPDIRECTDRAWCLIPPER));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetColorKey, (DWORD, LPDDCOLORKEY), (IDirectDrawSurface7 *,DWORD, LPDDCOLORKEY));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetOverlayPosition, (LONG, LONG), (IDirectDrawSurface7 *,LONG, LONG));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetPalette, (LPDIRECTDRAWPALETTE), (IDirectDrawSurface7 *,LPDIRECTDRAWPALETTE));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Unlock, (LPRECT), (IDirectDrawSurface7 *,LPRECT));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, UpdateOverlay, (LPRECT, LPDIRECTDRAWSURFACE7,LPRECT,DWORD, LPDDOVERLAYFX), (IDirectDrawSurface7 *,LPRECT, LPDIRECTDRAWSURFACE7,LPRECT,DWORD, LPDDOVERLAYFX));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, UpdateOverlayDisplay, (DWORD), (IDirectDrawSurface7 *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, UpdateOverlayZOrder, (DWORD, LPDIRECTDRAWSURFACE7), (IDirectDrawSurface7 *,DWORD, LPDIRECTDRAWSURFACE7));
	
	/*** Added in the v2 interface ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetDDInterface, (LPVOID FAR *), (IDirectDrawSurface7 *,LPVOID FAR *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, PageLock, (DWORD), (IDirectDrawSurface7 *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, PageUnlock, (DWORD), (IDirectDrawSurface7 *,DWORD));

	/*** Added in the v3 interface ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetSurfaceDesc, (LPDDSURFACEDESC2, DWORD), (IDirectDrawSurface7 *,LPDDSURFACEDESC2, DWORD));
	/*** Added in the v4 interface ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetPrivateData, (REFGUID, LPVOID, DWORD, DWORD), (IDirectDrawSurface7 *,REFGUID, LPVOID, DWORD, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetPrivateData, (REFGUID, LPVOID, LPDWORD), (IDirectDrawSurface7 *,REFGUID, LPVOID, LPDWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, FreePrivateData, (REFGUID), (IDirectDrawSurface7 *,REFGUID));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetUniquenessValue, (LPDWORD), (IDirectDrawSurface7 *,LPDWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, ChangeUniquenessValue, (void), (IDirectDrawSurface7 *));
	/*** Moved Texture7 methods here ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetPriority, (DWORD), (IDirectDrawSurface7 *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetPriority, (LPDWORD), (IDirectDrawSurface7 *,LPDWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetLOD, (DWORD), (IDirectDrawSurface7 *,DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetLOD, (LPDWORD), (IDirectDrawSurface7 *,LPDWORD));

IGA_DECLARE_CLASS_END();

