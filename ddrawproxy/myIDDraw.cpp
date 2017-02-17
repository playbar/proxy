#pragma  once 

#include "StdAfx.h"
#include "myIDDraw.h"
#include "drawing3D.h"
#include <map>

#include "DirectXVideoTexture.h"
#include "FlashSprite.h"
#include "t3dlib1.h"
#include "TestPopup.h"

#pragma warning(disable : 4312)

# if !defined MAX_INT
#	define	MAX_INT	0x7FFFFFFF
# endif 

# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	if defined(ASSERT)
#		undef ASSERT
#	endif
#	define	ASSERT	
# else
#	define	ASSERT
# endif 

#	define	appCopyMemory		::CopyMemory

//extern  Surface g_kSurf;

typedef std::map<IDirectDrawSurface7*, BOOL> CExportSurface7;
CExportSurface7 g_mapExportSurface7;

typedef std::map<IDirectDrawSurface*, BOOL> CExportSurface;
CExportSurface g_mapExportSurface;

FlashSprite				* g_pFlashSprite = NULL;
UCHAR					* g_pFlashData =NULL;

CDirectXVideoTexture	*g_pIDirectVideo = NULL; 
UCHAR					*g_pVideoData    = NULL;

PALETTEENTRY     def_palette[256] ={
	0, 0, 0, 0,
	72, 22, 11, 0,
	82, 28, 33, 0,
	94, 38, 44, 0,
	11, 0, 0, 0,
	11, 6, 0, 0,
	19, 0, 0, 0,
	19, 14, 11, 0,
	27, 16, 11, 0,
	35, 0, 0, 0,
	35, 22, 14, 5,
	48, 0, 3, 5,
	40, 30, 19, 5,
	46, 35, 24, 5,
	51, 38, 27, 5,
	70, 0, 0, 5,
	56, 43, 30, 5,
	78, 0, 0, 5,
	56, 54, 0, 5,
	64, 51, 35, 5,
	83, 32, 3, 5,
	70, 62, 11, 5,
	96, 0, 0, 5,
	70, 56, 38, 5,
	78, 64, 43, 5,
	107, 51, 3, 5,
	86, 70, 48, 5,
	99, 67, 22, 5,
	126, 0, 0, 5,
	91, 75, 51, 5,
	102, 86, 0, 5,
	104, 99, 32, 5,
	152, 0, 0, 5,
	131, 83, 6, 5,
	131, 96, 24, 5,
	168, 0, 0, 5,
	168, 11, 0, 5,
	134, 126, 38, 5,
	147, 120, 59, 5,
	168, 83, 80, 5,
	155, 150, 38, 5,
	168, 160, 40, 5,
	0, 3, 0, 5,
	0, 6, 3, 5,
	3, 14, 3, 5,
	3, 14, 11, 5,
	3, 19, 3, 5,
	6, 19, 14, 5,
	6, 24, 6, 5,
	6, 27, 6, 5,
	6, 30, 6, 5,
	8, 24, 19, 5,
	6, 35, 6, 5,
	8, 30, 22, 5,
	11, 38, 14, 5,
	8, 40, 8, 5,
	16, 38, 11, 5,
	8, 43, 8, 5,
	11, 35, 27, 5,
	8, 46, 8, 5,
	19, 43, 11, 5,
	0, 43, 24, 5,
	11, 40, 30, 5,
	8, 51, 8, 5,
	14, 51, 14, 5,
	22, 48, 14, 5,
	14, 48, 27, 5,
	14, 46, 35, 5,
	11, 59, 11, 5,
	24, 56, 14, 5,
	11, 62, 11, 5,
	16, 51, 38, 5,
	19, 48, 43, 5,
	11, 67, 11, 5,
	27, 62, 16, 5,
	0, 70, 0, 5,
	14, 72, 14, 5,
	30, 67, 16, 5,
	19, 59, 46, 5,
	14, 78, 14, 5,
	16, 78, 16, 5,
	32, 62, 43, 5,
	16, 83, 19, 5,
	35, 78, 19, 5,
	43, 67, 46, 5,
	19, 88, 19, 5,
	38, 86, 22, 5,
	19, 94, 19, 5,
	40, 91, 22, 5,
	48, 88, 35, 5,
	43, 96, 22, 5,
	22, 107, 22, 5,
	48, 96, 32, 5,
	54, 91, 40, 5,
	48, 102, 30, 5,
	6, 115, 27, 5,
	48, 110, 27, 5,
	62, 99, 43, 5,
	75, 91, 51, 5,
	48, 118, 24, 5,
	0, 134, 0, 5,
	70, 110, 48, 5,
	56, 126, 30, 5,
	75, 115, 51, 5,
	59, 134, 30, 5,
	83, 126, 56, 5,
	88, 134, 59, 5,
	0, 168, 32, 5,
	94, 139, 62, 5,
	99, 144, 64, 5,
	104, 152, 67, 5,
	110, 158, 70, 5,
	115, 163, 72, 5,
	3, 3, 6, 5,
	0, 0, 8, 5,
	6, 3, 8, 5,
	3, 6, 8, 5,
	8, 6, 11, 5,
	0, 0, 16, 5,
	0, 0, 19, 5,
	6, 11, 16, 5,
	8, 0, 22, 5,
	14, 8, 19, 5,
	0, 0, 27, 5,
	16, 11, 24, 5,
	0, 0, 32, 5,
	19, 14, 30, 5,
	0, 0, 38, 5,
	8, 0, 38, 5,
	24, 16, 35, 5,
	0, 0, 46, 5,
	8, 0, 46, 5,
	24, 19, 40, 5,
	0, 0, 51, 5,
	24, 24, 40, 5,
	0, 6, 54, 5,
	0, 0, 56, 5,
	30, 24, 46, 5,
	0, 0, 62, 5,
	8, 0, 62, 5,
	32, 30, 51, 5,
	0, 0, 70, 5,
	35, 30, 56, 5,
	22, 22, 67, 5,
	0, 0, 75, 5,
	27, 43, 59, 5,
	38, 35, 62, 5,
	0, 0, 83, 5,
	35, 48, 62, 5,
	40, 40, 64, 5,
	43, 46, 70, 5,
	16, 14, 94, 5,
	46, 51, 75, 5,
	0, 0, 102, 5,
	56, 62, 91, 5,
	0, 0, 128, 5,
	43, 43, 128, 5,
	59, 80, 104, 5,
	0, 0, 166, 5,
	14, 14, 168, 5,
	22, 118, 123, 5,
	24, 24, 168, 5,
	91, 99, 136, 5,
	62, 120, 168, 5,
	110, 120, 166, 5,
	104, 115, 155, 5,
	168, 0, 168, 5,
	86, 86, 8, 5,
	96, 80, 54, 5,
	107, 107, 0, 5,
	104, 91, 62, 5,
	112, 99, 67, 5,
	120, 104, 72, 5,
	128, 128, 6, 5,
	123, 107, 91, 5,
	126, 112, 80, 5,
	134, 120, 86, 5,
	150, 150, 3, 5,
	142, 128, 94, 5,
	139, 126, 104, 5,
	155, 131, 88, 5,
	147, 136, 99, 5,
	155, 142, 110, 5,
	168, 168, 0, 5,
	168, 166, 70, 5,
	163, 152, 115, 5,
	168, 160, 123, 5,
	168, 168, 150, 5,
	0, 6, 6, 5,
	3, 6, 6, 5,
	6, 6, 3, 5,
	8, 8, 6, 5,
	6, 11, 11, 5,
	14, 11, 8, 5,
	11, 8, 14, 5,
	14, 14, 14, 5,
	8, 16, 16, 5,
	11, 19, 19, 5,
	19, 14, 19, 5,
	24, 19, 16, 5,
	22, 22, 22, 5,
	14, 27, 27, 5,
	27, 24, 22, 5,
	16, 30, 30, 5,
	32, 27, 19, 5,
	32, 27, 24, 5,
	27, 27, 32, 5,
	16, 35, 32, 5,
	38, 32, 24, 5,
	32, 32, 32, 5,
	38, 32, 30, 5,
	22, 38, 40, 5,
	43, 43, 0, 5,
	38, 35, 35, 5,
	43, 43, 14, 5,
	22, 43, 43, 5,
	43, 38, 35, 5,
	30, 40, 48, 5,
	27, 46, 48, 5,
	48, 43, 35, 5,
	27, 51, 51, 5,
	54, 46, 35, 5,
	46, 46, 46, 5,
	32, 48, 54, 5,
	54, 48, 40, 5,
	43, 43, 56, 5,
	30, 59, 56, 5,
	59, 54, 43, 5,
	54, 54, 54, 5,
	32, 64, 62, 5,
	40, 56, 67, 5,
	64, 56, 46, 5,
	40, 64, 67, 5,
	64, 62, 48, 5,
	70, 62, 51, 5,
	70, 67, 54, 5,
	59, 78, 54, 5,
	46, 72, 78, 5,
	70, 70, 70, 5,
	80, 70, 59, 5,
	56, 72, 83, 5,
	88, 75, 64, 5,
	96, 80, 64, 5,
	86, 86, 86, 5,
	99, 88, 70, 5,
	107, 91, 80, 5,
	94, 94, 94, 0,
	83, 91, 123, 0,
	115, 102, 83, 0,
	107, 107, 112, 0,
	128, 128, 128, 0,
	139, 139, 142, 0,
	128, 128, 168, 0,
	150, 150, 152, 0,
	160, 160, 160, 0,
	170, 170, 170, 0

};

//#pragma  pack(push)  //保存对齐状态  
//#pragma  pack(1)   
//#pragma  pack(pop) 

LPDDSURFACEDESC2 g_pddsd2 = NULL;

BITMAP_FILE      bitmap;                // holds the bitmap
BITMAP_FILE      bitmap_close;
BITMAP_FILE		 bitmap_close_over;
BITMAP_FILE		 bitmap_playwindow;
BITMAP_FILE		 bitmap_playwindow_over;
BITMAP_FILE		 bitmap8;

DDSURFACEDESC	 g_ddsd;
DDSURFACEDESC2        g_ddsd2; 
LPDIRECTDRAWSURFACE4 g_pddbacksurface = NULL;

LPDIRECTDRAWPALETTE   g_lpddpal      = NULL;   // a pointer to the created dd palette

int g_nWidthPos = 0;
int g_nHeightPos = 0;

//#define VK_S	0x53
//
//#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
//#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL WriteMemory(LPCVOID lpAddress, LPCVOID lpData, DWORD dwBytesToWrite)
{
	ASSERT(!::IsBadReadPtr(lpData, dwBytesToWrite));

	ASSERT(dwBytesToWrite >= 0 && dwBytesToWrite < MAX_INT );
	MEMORY_BASIC_INFORMATION mbi	= { 0 };
	::VirtualQuery(lpAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION));  
	if(!::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect))
		return	FALSE;
	ASSERT(mbi.RegionSize >= dwBytesToWrite );
	appCopyMemory(const_cast<void*>(lpAddress), lpData, dwBytesToWrite);

	DWORD dwOldProtect	= 0L;
	BOOL b = ::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect);
	return	b;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
// DWORD GetVirtualFunctionNumber(LPVOID pObject)
// {
// 	ASSERT(pObject != NULL);
//     int *p	= (int *) pObject;
//     int *q	= NULL;
//     DWORD i = 0;
//     q = (int*)(*(int*)p);
// 	while (1)
// 	{
//         if (NULL == q[i]) 
// 		{
// 			return i;
//         }
// 		i++;
//     }
// 	return i;
// }

DWORD GetVirtualFunctionNumber(const LPVOBJECT pObject)
{
	ASSERT(pObject != NULL);
	DWORD	dwCount	= 0;

	if(!pObject || !pObject->pVft )
	{
		return	0L;
	}
	LPVFT pVft	= pObject->pVft;

	while(NULL != pVft[dwCount++])
		;
	return	dwCount;
}


/**/
DWORD CopyVFT(LPVFT pVFT, DWORD dwVFTSize, const LPVOBJECT pVObject)
{
	ASSERT(pVFT != NULL);
	ASSERT(dwVFTSize <= MAX_INT);
	if( pVObject == NULL || pVObject->pVft == NULL )
		return	0L;
	DWORD	dwCount	= GetVirtualFunctionNumber(pVObject);
	dwCount	= dwVFTSize > dwCount ? dwCount : dwVFTSize;
	for(DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
	{
		pVFT[dwIndex]	= pVObject->pVft[dwIndex];
	}
	return	dwCount;
}

DWORD ReplaceVFT(LPVOBJECT pTargetObject, const LPVOBJECT pSourceObject)
{
	if( !pTargetObject || !pTargetObject->pVft )
		return	0L;
	if( !pSourceObject || !pSourceObject->pVft )
		return	0L;

	DWORD	dwCount1	= GetVirtualFunctionNumber( pTargetObject );
	DWORD	dwCount2	= GetVirtualFunctionNumber( pSourceObject );
	dwCount1	= dwCount1 > dwCount2 ? dwCount2 : dwCount1;
	for(DWORD dwIndex = 0; dwIndex < dwCount1; dwIndex++)
	{
		WriteMemory(&pTargetObject->pVft[dwIndex], &pSourceObject->pVft[dwIndex], sizeof(VFT));
	}
	return	dwCount1;
}

DWORD ReplaceN_VFT(LPVOBJECT pTargetObject, const LPVOBJECT pSourceObject, DWORD dwVFTSize)
{
	if( !pTargetObject || !pTargetObject->pVft )
		return	0L;
	if( !pSourceObject || !pSourceObject->pVft )
		return	0L;

	DWORD	dwCount		= GetVirtualFunctionNumber( pTargetObject );
	dwCount		= dwCount > dwVFTSize ? dwVFTSize : dwCount;
	ASSERT(dwCount <= GetVirtualFunctionNumber( pSourceObject ) );

	for(DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
	{
		WriteMemory(&pTargetObject->pVft[dwIndex], &pSourceObject->pVft[dwIndex], sizeof(VFT));
	}
	return	dwCount;
}


IGA_IDirectDraw::IGA_IDirectDraw(IDirectDraw*  pIDirectDraw) 
:m_pIDirectDraw(pIDirectDraw)
{
	OutputDebugString("IGA_IDirectDraw::IGA_IDirectDraw");
	//*
	//if ( NULL == g_pFlashSprite)
	//{
	//	g_pFlashSprite = new FlashSprite();
	//}
	InitDrawing();

	//if (NULL == g_pIDirectVideo )
	//{
	//	g_pIDirectVideo = new CDirectXVideoTexture();
	//}
	//g_pIDirectVideo->CreateFromAVIFile("test.avi");

	//g_pFlashSprite->StartFlash("test.swf",240,180);
	//Load_Bitmap_File(&bitmap,"bitmap24.bmp");
	//Load_Bitmap_File(&bitmap8, "bitmap8.bmp");
	//Load_Bitmap_File(&bitmap_back, "BITMAP_BACKGROUND.bmp");
	//Load_Bitmap_File(&bitmap_over, "BITMAP_MOUSE_OVER.bmp");
	//*/
}

IGA_IDirectDraw::~IGA_IDirectDraw()
{    
	OutputDebugString("IGA_IDirectDraw::~IGA_IDirectDraw");
}


HRESULT IGA_IDirectDraw::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
	OutputDebugString("IGA_IDirectDraw::QueryInterface");
	HRESULT hr = m_pIDirectDraw->QueryInterface (riid,  ppvObj);
	if ( ::IsEqualIID(riid, Proxy_IDirectDraw  )) 
	{		  
		OutputDebugString("Proxy_IDirectDraw");
		*ppvObj  = this;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw4)) 
	{
		OutputDebugString("Proxy_IDirectDraw4");
		IGA_IDirectDraw4*   pIDirectDraw = new IGA_IDirectDraw4((IDirectDraw4*)*ppvObj);
		*ppvObj = pIDirectDraw;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw7)) 
	{
		OutputDebugString("Proxy_IDirectDraw7");
		IGA_IDirectDraw7*   pIDirectDraw = new IGA_IDirectDraw7((IDirectDraw7*)*ppvObj);
		*ppvObj = pIDirectDraw;
	}
	
	/*
	char chOutputString[256];
	sprintf(chOutputString, "IGA_IDirectDraw::QueryInterface() Riid = % d", riid);
	OutputDebugString(chOutputString);

	if (::IsEqualIID(riid, Proxy_IDirectDraw ))
	{
		OutputDebugString("Proxy_IDirectDraw");
	}
	if (::IsEqualIID(riid, Proxy_IDirectDraw2 ))
	{
		OutputDebugString("Proxy_IDirectDraw2");
	}
	if (::IsEqualIID(riid, Proxy_IDirectDraw4 ))
	{
		OutputDebugString("Proxy_IDirectDraw4");
	}
	if (::IsEqualIID(riid, Proxy_IDirectDraw7 ))
	{
		OutputDebugString("Proxy_IDirectDraw7");
	}
	//*/

	return hr;

}
ULONG   IGA_IDirectDraw::AddRef ()  
{
	OutputDebugString("IGA_IDirectDraw::AddRed()");
	return  m_pIDirectDraw->AddRef ()  ;
}
ULONG   IGA_IDirectDraw::Release() 
{
	ULONG  ulRet =  m_pIDirectDraw->Release() ;
	char chOutputString[256];
	sprintf(chOutputString, "IGA_IDirectDraw::Release(),ulRet = %d",ulRet);
	OutputDebugString(chOutputString);
	return ulRet;
}

/*** IDirectDraw methods ***/
HRESULT IGA_IDirectDraw::Compact() 
{
	OutputDebugString("IGA_IDirectDraw::Compact()");
	return m_pIDirectDraw->Compact() ;
}
HRESULT IGA_IDirectDraw::CreateClipper( DWORD param1, LPDIRECTDRAWCLIPPER FAR* param2, IUnknown FAR * param3 ) 
{
	OutputDebugString("IGA_IDirectDraw::CreateClipper");
	return m_pIDirectDraw->CreateClipper( param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw::CreatePalette( DWORD param0, LPPALETTEENTRY param1, LPDIRECTDRAWPALETTE FAR* param2, IUnknown FAR * param3 ) 
{
	OutputDebugString("IGA_IDirectDraw::CreatePalette()");
	param0 = DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE;
	HRESULT hr = m_pIDirectDraw->CreatePalette( param0,  param1,  param2,  param3 ) ;

	static	bool bReplaced	= false;
	if(SUCCEEDED(hr))
	{
		OutputDebugString("success IGA_IDirectDraw::CreateSurface()");
		if(!bReplaced)
		{
			IGA_IDirectDrawPalette::Init( *param2);
			bReplaced = true;
		}
	}
	g_lpddpal = *param2;

	char chOutput[256];
	sprintf(chOutput, "param0 = %d, param1 = %d, param2 = %d, param3 = %d", param0, param1, *param2, param3);
	OutputDebugString(chOutput);
	return hr;
}
HRESULT IGA_IDirectDraw::CreateSurface( LPDDSURFACEDESC param1, LPDIRECTDRAWSURFACE FAR * param2, IUnknown FAR * param3) 
{
	char chOutputString[256];

	HRESULT hr	= m_pIDirectDraw->CreateSurface(param1, param2, param3 );
	sprintf( chOutputString, "param2 = %d", *param2);
	OutputDebugString(chOutputString);

	static	bool bReplaced	= false;
	if(SUCCEEDED(hr))
	{
		OutputDebugString("success IGA_IDirectDraw::CreateSurface()");
		if(!bReplaced)
		{
			IGA_IDirectDrawSurface::Init(*param2);
			bReplaced = true;
		}
	}
	else
	{
		OutputDebugString("failed IGA_IDirectDraw::CreateSurface()");
	}
	return	hr;

	//return m_pIDirectDraw->CreateSurface(  param1,  param2,  param3) ;
}
HRESULT IGA_IDirectDraw::DuplicateSurface(  LPDIRECTDRAWSURFACE param1, LPDIRECTDRAWSURFACE FAR * param2) 
{
	OutputDebugString("IGA_IDirectDraw::DuplicateSurface()");
	return m_pIDirectDraw->DuplicateSurface(  param1,  param2) ;
}
HRESULT IGA_IDirectDraw::EnumDisplayModes(  DWORD param1, LPDDSURFACEDESC param2, LPVOID param3, LPDDENUMMODESCALLBACK param4 ) 
{
	OutputDebugString("IGA_IDirectDraw::EnumDisplayModes()");

	HRESULT hr = m_pIDirectDraw->EnumDisplayModes(   param1,  param2,  param3,  param4 ) ;
	return hr;
}
HRESULT IGA_IDirectDraw::EnumSurfaces( DWORD param1, LPDDSURFACEDESC param2, LPVOID param3,LPDDENUMSURFACESCALLBACK  param4) 
{
	OutputDebugString("IGA_IDirectDraw::EnumSurface()");
	return m_pIDirectDraw->EnumSurfaces(  param1,  param2,  param3,  param4) ;
}
HRESULT IGA_IDirectDraw::FlipToGDISurface()
{
	OutputDebugString("IGA_IDirectDraw::FlipToGDISurface()");
	HRESULT hr = m_pIDirectDraw->FlipToGDISurface();
	return S_OK;
}
HRESULT IGA_IDirectDraw::GetCaps(  LPDDCAPS param1, LPDDCAPS param2) 
{
	OutputDebugString("IGA_IDirectDraw::GetCaps()");
	return m_pIDirectDraw->GetCaps(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw::GetDisplayMode(  LPDDSURFACEDESC param1) 
{
	OutputDebugString("IGA_IDirectDraw::GetDisplayMode()");
	HRESULT hr = m_pIDirectDraw->GetDisplayMode(   param1) ;
	return hr;
}
HRESULT IGA_IDirectDraw::GetFourCCCodes(  LPDWORD param1, LPDWORD param2) 
{
	OutputDebugString("IGA_IDirectDraw::GetFourCCCodes()");
	return m_pIDirectDraw->GetFourCCCodes(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw::GetGDISurface( LPDIRECTDRAWSURFACE FAR * param1) 
{
	OutputDebugString("IGA_IDirectDraw::GetGDISurface()");
	return m_pIDirectDraw->GetGDISurface(  param1) ;
}
HRESULT IGA_IDirectDraw::GetMonitorFrequency( LPDWORD param1) 
{
	OutputDebugString("IGA_IDirectDraw::GetMonitorFrequency()");
	return m_pIDirectDraw->GetMonitorFrequency(  param1) ;
}
HRESULT IGA_IDirectDraw::GetScanLine( LPDWORD param1) 
{
	OutputDebugString("IGA_IDirectDraw::GetScanLine()");
	return m_pIDirectDraw->GetScanLine(  param1) ;
}
HRESULT IGA_IDirectDraw::GetVerticalBlankStatus( LPBOOL param1) 
{
	OutputDebugString("IGA_IDirectDraw::GetVerticalBlankStatus()");
	return m_pIDirectDraw->GetVerticalBlankStatus(  param1) ;
}
HRESULT IGA_IDirectDraw::Initialize( GUID FAR * param1) 
{
	OutputDebugString("IGA_IDirectDraw::Initialize()");
	return m_pIDirectDraw->Initialize(  param1) ;
}
HRESULT IGA_IDirectDraw::RestoreDisplayMode() 
{
	OutputDebugString("IGA_IDirectDraw::RestoreDisplayMode()");
	return m_pIDirectDraw->RestoreDisplayMode() ;
}
HRESULT IGA_IDirectDraw::SetCooperativeLevel( HWND param1, DWORD param2) 
{
	OutputDebugString("IGA_IDirectDraw::SetCooperativeLevel");
	HRESULT hr = m_pIDirectDraw->SetCooperativeLevel(  param1,  param2) ;
	return hr;
}


HRESULT IGA_IDirectDraw::SetDisplayMode( DWORD param1, DWORD param2,DWORD param3)
{
	OutputDebugString("IGA_IDirectDraw::SetDisplayMode");
	char chOutputString[256];
	HRESULT hr = m_pIDirectDraw->SetDisplayMode(  param1,  param2, param3); 
	
	sprintf(chOutputString, "IGA_DirectDraw:SetDisplayMode(),width = %d, height = %d, depth = %d, hr = %d",param1, param2, param3, hr);
	OutputDebugString(chOutputString);
	return hr;
}
HRESULT IGA_IDirectDraw::WaitForVerticalBlank( DWORD param1, HANDLE param2) 
{
	OutputDebugString("IGA_IDirectDraw::WaitForVerticalBlank()");
	return m_pIDirectDraw->WaitForVerticalBlank( param1,  param2) ;
}


//////////////////////////////////////////////////////////////////////////////////////////////
IGA_IMPLEMENT_CLASS(IDirectDrawSurface)
BOOL IGA_IDirectDrawSurface::Init(IDirectDrawSurface *pObject)
{
	OutputDebugString("IGA_IDirectDrawSurface::Init");
	ASSERT(pObject != NULL);
	INIT_VIRTUAL_OBJECT(pObject);
	DWORD	dwIndex	= 0;
	/*** IUnknown methods ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, QueryInterface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddRef);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Release);
	/*** IDirectDrawSurface methods ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddAttachedSurface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddOverlayDirtyRect);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Blt);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, BltBatch);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, BltFast);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, DeleteAttachedSurface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, EnumAttachedSurfaces);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, EnumOverlayZOrders);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Flip);	
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetAttachedSurface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetBltStatus);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetCaps);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetClipper);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetColorKey);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetDC);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetFlipStatus);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetOverlayPosition);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetPalette);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetPixelFormat);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetSurfaceDesc);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Initialize);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, IsLost);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Lock);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, ReleaseDC);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Restore);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetClipper);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetColorKey);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetOverlayPosition);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetPalette);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Unlock);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, UpdateOverlay);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, UpdateOverlayDisplay);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, UpdateOverlayZOrder);

	return	TRUE;
}
/*** IUnknown methods ***/
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, QueryInterface, (REFIID riid, void** ppvObj), (this, riid, ppvObj));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, QueryInterface)
HRESULT IGA_IDirectDrawSurface::QueryInterface(REFIID riid, void** ppvObj)
{
	OutputDebugString("IGA_IDirectDrawSurface::QueryInterface()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnQueryInterface(this, riid, ppvObj );
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(ULONG, IDirectDrawSurface, AddRef, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, AddRef)
ULONG IGA_IDirectDrawSurface::AddRef()
{
	OutputDebugString("IGA_IDirectDrawSurface::AddRef()");
	ULONG re = IGA_IDirectDrawSurface::m_pfnAddRef(this );	
	return re;
}

//IMPLEMENT_MEMBER_FUNCTION(ULONG, IDirectDrawSurface, Release, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, Release)
ULONG IGA_IDirectDrawSurface::Release()
{
	OutputDebugString("IGA_IDirectDrawSurface::Release()");
	char chOutputString[256];
	ULONG re = IGA_IDirectDrawSurface::m_pfnRelease(this);
	sprintf(chOutputString, "IGA_IDirectDrawSurface::Release(),re = %d",re);
	OutputDebugString(chOutputString);
	return re;
}

/*** IDirectDrawSurface methods ***/
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, AddAttachedSurface);
HRESULT IGA_IDirectDrawSurface::AddAttachedSurface(LPDIRECTDRAWSURFACE a)
{
	OutputDebugString("IGA_IDirectDrawSurface::AddAttachedSurface()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnAddAttachedSurface(this, a);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, AddOverlayDirtyRect, (LPRECT a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, AddOverlayDirtyRect);
HRESULT IGA_IDirectDrawSurface::AddOverlayDirtyRect(LPRECT rect)
{
	OutputDebugString("IGA_IDirectDrawSurface::AddOverlayDirtyRect()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnAddOverlayDirtyRect(this, rect);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, Blt, (LPRECT a, LPDIRECTDRAWSURFACE b, LPRECT c, DWORD d, LPDDBLTFX e), (this, a, b, c, d, e));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, Blt);
HRESULT IGA_IDirectDrawSurface::Blt(LPRECT a, LPDIRECTDRAWSURFACE b, LPRECT c, DWORD d, LPDDBLTFX e)
{
	//OutputDebugString("IGA_IDirectDrawSurface::Blt()");
	/*/
	CHAR chOutputString[255];
	USHORT * back_buffer = (USHORT *)g_ddsd.lpSurface;
	int lpitch = (int)(g_ddsd.lPitch >> 1  );
	//////////////////////////////////////////////////////////////////////////
	int dwWidth = g_ddsd.dwWidth;
	int dwHeight = g_ddsd.dwHeight;
	//sprintf(chOutputString, "In Unlock, lpitch : %d, Width:%d, Height:%d ", lpitch, dwWidth, dwHeight );
	//OutputDebugString(chOutputString);
	//////////////////////////////////////////////////////////////////////////
	g_pIDirectVideo->CheckForLoop();
	g_pVideoData = (UCHAR*)g_pIDirectVideo->m_pVideoBuffer;
	UCHAR * lpBuffer = g_pVideoData;
	int nVideoHeight = g_pIDirectVideo->m_VideoHeight;
	int nVideoWidth  = g_pIDirectVideo->m_VideoWidth;
	//sprintf(chOutputString, "IGA_IDirectDrawSurface::UnLock(),g_pVideoData = %p,nHeight = %d, nWidth = %d",g_pVideoData, nVideoHeight, nVideoWidth);
	//OutputDebugString(chOutputString);
	//if (FAILED(g_lpddpal->SetEntries(0,0,MAX_COLORS_PALETTE,def_palette)))
	//	return false;
	if (lpitch == 800 || lpitch == 1024 )
	{
		for (int yh = 0 ; yh < nVideoHeight; yh ++) 
		{
			for (int xw = 0 ; xw < nVideoWidth; xw ++)
			{
				UCHAR blue  = lpBuffer[ yh*nVideoWidth*3 + xw*3 + 0 ] >> 3;
				UCHAR green = lpBuffer[ yh*nVideoWidth*3 + xw*3 + 1 ] >> 2;
				UCHAR red   = lpBuffer[ yh*nVideoWidth*3 + xw*3 + 2 ] >> 3;
				USHORT pixel = _RGB16BIT565(red, green, blue );
				back_buffer[xw   + (nVideoHeight  - yh - 1)* lpitch  ] = pixel;
			}
		}
	}

	//*/
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnBlt(this, a, b, c, d, e);
	return hr;
}
//
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, BltBatch, (LPDDBLTBATCH a, DWORD b, DWORD c), (this, a, b, c));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, BltBatch);
HRESULT IGA_IDirectDrawSurface::BltBatch(LPDDBLTBATCH lpblt, DWORD word1, DWORD word2)
{
	OutputDebugString("IGA_IDirectDrawSurface::BltBatch()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnBltBatch(this, lpblt, word1, word2 );
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, BltFast, (DWORD a, DWORD b, LPDIRECTDRAWSURFACE7 c, LPRECT d, DWORD e), (this, a, b, c, d, e));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, BltFast);
HRESULT IGA_IDirectDrawSurface::BltFast(DWORD a, DWORD b, LPDIRECTDRAWSURFACE c, LPRECT d, DWORD e)
{
	OutputDebugString("IGA_IDirectDrawSurface::BltFast()");
	HRESULT hr;
	hr = IGA_IDirectDrawSurface::m_pfnBltFast(this, a, b, c, d, e);
	/*
	CHAR chFileName[MAX_PATH];
	CHAR chOutputString[255];

	DDSURFACEDESC2 desc;
	DDCOLORKEY clrKey;
	DDPIXELFORMAT pixelfmt;

	if (gl_pIDirectDraw == NULL || gl_pIDirectDraw->m_pIGADraw == NULL)
		return hr;

	IDirectDrawSurface* pSurface = (IDirectDrawSurface*)c;
	if (pSurface != NULL)
	{
		memset(&pixelfmt, 0, sizeof(DDPIXELFORMAT));
		pixelfmt.dwSize = sizeof(DDPIXELFORMAT);
		hr = IGA_IDirectDrawSurface::m_pfnGetPixelFormat(pSurface, &pixelfmt);

		if (SUCCEEDED(hr))
		{
			//if (pixelfmt.dwRGBBitCount == 16)
			//{
			//	CExportSurface::iterator p = g_mapExportSurface.find(pSurface);
			//	if (p == g_mapExportSurface.end())

			sprintf(chFileName, "Z:\\BltFast_SURFACE_%p.bmp", pSurface);
			gl_pIDirectDraw->m_pIGADraw->SaveToBitmapFile(pSurface, chFileName);
			OutputDebugString(chFileName);
			g_mapExportSurface[pSurface] = TRUE;

			//   锁定表面   
			/*	DDSURFACEDESC2 ddsd;   
			ddsd.dwSize = sizeof(ddsd);
			hr = IGA_IDirectDrawSurface::m_pfnGetSurfaceDesc(pSurface, &ddsd);
			if (SUCCEEDED(hr))
			{
			if (ddsd.dwWidth == 800 && ddsd.dwHeight == 872)
			{
			sprintf(chOutputString, "ReplaceSurface called!");
			OutputDebugString(chOutputString);
			gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface, gl_pIDDraw->m_pIGADraw->m_hBitmap);
			}


			if (ddsd.dwWidth == 800 && ddsd.dwHeight == 1800)
			{
			sprintf(chOutputString, "ReplaceSurface called!");
			OutputDebugString(chOutputString);
			gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface, gl_pIDDraw->m_pIGADraw->m_hBitmapB1);
			}

			if (ddsd.dwWidth == 800 && ddsd.dwHeight == 1302)
			{
			sprintf(chOutputString, "ReplaceSurface called!");
			OutputDebugString(chOutputString);
			gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface, gl_pIDDraw->m_pIGADraw->m_hBitmapC1);
			}

			if (ddsd.dwWidth == 288 && ddsd.dwHeight == 600)
			{
			sprintf(chOutputString, "ReplaceSurface called!");
			OutputDebugString(chOutputString);
			gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface, gl_pIDDraw->m_pIGADraw->m_hBitmapD1);
			}

			if (ddsd.dwWidth == 800 && ddsd.dwHeight == 926)
			{
			sprintf(chOutputString, "ReplaceSurface called!");
			OutputDebugString(chOutputString);
			gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface, gl_pIDDraw->m_pIGADraw->m_hBitmapE1);
			}

			//////////////////////////////////////////////////////////////////////////
			//if (ddsd.dwWidth == 244 && ddsd.dwHeight == 123)
			//{
			//	sprintf(chOutputString, "ReplaceSurface called!");
			//	OutputDebugString(chOutputString);
			//	gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface);
			//}
			//}
			//}
		}
	}
	*/
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, DeleteAttachedSurface, (DWORD a,LPDIRECTDRAWSURFACE b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, DeleteAttachedSurface);
HRESULT IGA_IDirectDrawSurface::DeleteAttachedSurface(DWORD word,LPDIRECTDRAWSURFACE lpddsur)
{
	OutputDebugString("IGA_IDirectDrawSurface::DeleteAttachedSurface()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnDeleteAttachedSurface(this, word, lpddsur);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, EnumAttachedSurfaces, (LPVOID a, LPDDENUMSURFACESCALLBACK b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, EnumAttachedSurfaces);
HRESULT IGA_IDirectDrawSurface::EnumAttachedSurfaces(LPVOID a, LPDDENUMSURFACESCALLBACK b)
{
	OutputDebugString("IGA_IDirectDrawSurface::EnumAttachedSurface()");
	LRESULT hr = IGA_IDirectDrawSurface::m_pfnEnumAttachedSurfaces(this, a,b);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, EnumOverlayZOrders, (DWORD a, LPVOID b, LPDDENUMSURFACESCALLBACK c), (this, a, b, c));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, EnumOverlayZOrders);
HRESULT IGA_IDirectDrawSurface::EnumOverlayZOrders(DWORD a,LPVOID b,LPDDENUMSURFACESCALLBACK c)
{
	OutputDebugString("IGA_IDirectDrawSurface::EnumOverlayZOrder()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnEnumOverlayZOrders(this, a,b,c);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, Flip, (LPDIRECTDRAWSURFACE a, DWORD b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, Flip);
HRESULT IGA_IDirectDrawSurface::Flip(LPDIRECTDRAWSURFACE a, DWORD b)
{
	OutputDebugString("IGA_IDirectDrawSurface::Flip()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnFlip(this, a, b );
	/*/
	CHAR chOutputString[255];
	USHORT * back_buffer = (USHORT *)g_ddsd.lpSurface;
	int lpitch = (int)(g_ddsd.lPitch >> 1  );


	//////////////////////////////////////////////////////////////////////////
	int dwWidth = g_ddsd.dwWidth;
	int dwHeight = g_ddsd.dwHeight;
	//sprintf(chOutputString, "In Unlock, lpitch : %d, Width:%d, Height:%d ", lpitch, dwWidth, dwHeight );
	//OutputDebugString(chOutputString);

	//////////////////////////////////////////////////////////////////////////
	g_pIDirectVideo->CheckForLoop();
	g_pVideoData = (UCHAR*)g_pIDirectVideo->m_pVideoBuffer;
	UCHAR * lpBuffer = g_pVideoData;
	int nVideoHeight = g_pIDirectVideo->m_VideoHeight;
	int nVideoWidth  = g_pIDirectVideo->m_VideoWidth;
	//sprintf(chOutputString, "IGA_IDirectDrawSurface::UnLock(),g_pVideoData = %p,nHeight = %d, nWidth = %d",g_pVideoData, nVideoHeight, nVideoWidth);
	//OutputDebugString(chOutputString);

	//if (FAILED(g_lpddpal->SetEntries(0,0,MAX_COLORS_PALETTE,def_palette)))
	//	return false;


	if (lpitch == 800 )
	{
		for (int yh = 0 ; yh < nVideoHeight; yh ++) 
		{
			for (int xw = 0 ; xw < nVideoWidth; xw ++)
			{
				UCHAR blue  = lpBuffer[ yh*nVideoWidth*3 + xw*3 + 0 ] >> 3;
				UCHAR green = lpBuffer[ yh*nVideoWidth*3 + xw*3 + 1 ] >> 2;
				UCHAR red   = lpBuffer[ yh*nVideoWidth*3 + xw*3 + 2 ] >> 3;
				USHORT pixel = _RGB16BIT565(red, green, blue );
				back_buffer[xw   + (nVideoHeight  - yh - 1)* lpitch  ] = pixel;
			}
		}
	}

	//*/
	/*
	static int i = 0;
	HRESULT hr = E_FAIL;
	CHAR chFileName[MAX_PATH];
	CHAR chOutputString[255];
	DDPIXELFORMAT pixelfmt;

	if (gl_pIDirectDraw == NULL || gl_pIDirectDraw->m_pIGADraw == NULL)
		return hr;

	IDirectDrawSurface* pSurface = (IDirectDrawSurface*)a;
	if (pSurface != NULL)
	{
		memset(&pixelfmt, 0, sizeof(DDPIXELFORMAT));
		pixelfmt.dwSize = sizeof(DDPIXELFORMAT);
		hr = IGA_IDirectDrawSurface::m_pfnGetPixelFormat(pSurface, &pixelfmt);

		if (SUCCEEDED(hr))
		{
			//if ( pixelfmt.dwRGBBitCount == 16)  //为了能够导出图片，去掉16位判断
			//{
			CExportSurface::iterator p = g_mapExportSurface.find(pSurface);
			if (p == g_mapExportSurface.end())
			{
				sprintf(chFileName, "Z:\\Flip_Surface_%d.bmp",i );
				gl_pIDirectDraw->m_pIGADraw->SaveToBitmapFile(pSurface, chFileName);
				OutputDebugString(chFileName);
				i++;
				g_mapExportSurface[pSurface] = TRUE;
			}
			//}
		}
	}
	*/
	return hr;

}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetAttachedSurface, (LPDDSCAPS a, LPDIRECTDRAWSURFACE FAR * b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetAttachedSurface);
HRESULT IGA_IDirectDrawSurface::GetAttachedSurface(LPDDSCAPS a, LPDIRECTDRAWSURFACE FAR * b)
{
	OutputDebugString("IGA_IDirectDrawSurface::GetAttachedSurface()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnGetAttachedSurface(this, a, b);
	return hr;
}


//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetBltStatus, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetBltStatus);
HRESULT IGA_IDirectDrawSurface::GetBltStatus(DWORD a)
{
	//OutputDebugString("IGA_IDirectDrawSurface::GetBltStatus()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnGetBltStatus(this, a);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetCaps, (LPDDSCAPS a), (this , a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetCaps);
HRESULT IGA_IDirectDrawSurface::GetCaps(LPDDSCAPS lpdd)
{
	OutputDebugString("IGA_IDirectDrawSurface::GetCaps()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnGetCaps(this, lpdd);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetClipper, (LPDIRECTDRAWCLIPPER FAR* a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetClipper)
HRESULT IGA_IDirectDrawSurface::GetClipper(LPDIRECTDRAWCLIPPER FAR* lpddclip)
{
	OutputDebugString("IGA_IDirectDrawSurface::GetClipper()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnGetClipper(this, lpddclip);
	return hr;
}

//只调用这个函数时，只能导出界面图来
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetColorKey, (DWORD a, LPDDCOLORKEY b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetColorKey);
HRESULT IGA_IDirectDrawSurface::GetColorKey(DWORD word, LPDDCOLORKEY lpcolor)
{
	OutputDebugString("IGA_IDirectDrawSurface::GetColorKey()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnGetColorKey(this, word, lpcolor);
	return hr;
}


//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetDC, (HDC FAR * a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetDC);
HRESULT IGA_IDirectDrawSurface::GetDC(HDC FAR * hDC)
{
	OutputDebugString("IGA_IDirectDrawSurface::GetDC()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnGetDC(this, hDC);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetFlipStatus, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetFlipStatus);
HRESULT IGA_IDirectDrawSurface::GetFlipStatus(DWORD word)
{
	OutputDebugString("IGA_IDirectDrawSurface::GetFlipStatus()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnGetFlipStatus(this, word);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetOverlayPosition, (LPLONG a, LPLONG b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetOverlayPosition);
HRESULT IGA_IDirectDrawSurface::GetOverlayPosition(LPLONG lpl, LPLONG lpl2)
{
	OutputDebugString("IGA_IDirectDrawSurface::GetOverlayPosition()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnGetOverlayPosition(this, lpl,lpl2);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetPalette, (LPDIRECTDRAWPALETTE FAR* a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetPalette);
HRESULT IGA_IDirectDrawSurface::GetPalette(LPDIRECTDRAWPALETTE FAR* lpddpal)
{
	OutputDebugString("IGA_IDirectDrawSurface::GetPalette()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnGetPalette(this, lpddpal);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetPixelFormat, (LPDDPIXELFORMAT a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetPixelFormat);
HRESULT IGA_IDirectDrawSurface::GetPixelFormat(LPDDPIXELFORMAT lpddformat)
{
	OutputDebugString("IGA_IDirectDrawSurface::GetPixelFormat()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnGetPixelFormat(this, lpddformat);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, GetSurfaceDesc, (LPDDSURFACEDESC a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, GetSurfaceDesc);
HRESULT IGA_IDirectDrawSurface::GetSurfaceDesc(LPDDSURFACEDESC a)
{
	OutputDebugString("IGA_IDirectDrawSurface::GetSurfaceDesc()");
	LRESULT hr=IGA_IDirectDrawSurface::m_pfnGetSurfaceDesc(this, a);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, Initialize, (LPDIRECTDRAW a, LPDDSURFACEDESC b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, Initialize);
HRESULT IGA_IDirectDrawSurface::Initialize(LPDIRECTDRAW lpddraw, LPDDSURFACEDESC lpddsd)
{
	OutputDebugString("IGA_IDirectDrawSurface::Initialize()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnInitialize(this, lpddraw, lpddsd);
	return hr;
}
//能导出两张图片，但不是我们想要的
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, IsLost, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, IsLost);
HRESULT IGA_IDirectDrawSurface::IsLost()
{
	//OutputDebugString("IGA_IDirectDrawSurface::IsLost()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnIsLost(this);
	return hr;
}

//调用这个函数是终止了程序
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, Lock, (LPRECT a,LPDDSURFACEDESC b,DWORD c,HANDLE d), (this, a, b, c, d));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, Lock);
//错误分析 LPDDSURFACEDESC ddsd 在源程序可能是临时变量
HRESULT IGA_IDirectDrawSurface::Lock(LPRECT rect,LPDDSURFACEDESC ddsd,DWORD word ,HANDLE h)
{
	//OutputDebugString("IGA_IDirectDrawSurface::Lock");
	HRESULT hr = E_FAIL;
	hr = IGA_IDirectDrawSurface::m_pfnLock(this, NULL, ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, h);
	g_ddsd =  *ddsd;

	//char szOutput[256];
	//sprintf_s(szOutput, "left = %d, right = %d, top = %d, bottom = %d", rect->left, rect->right, rect->top, rect->bottom );
	//OutputDebugString(szOutput );

	//int lpitch16 = (int)(ddsd->lPitch >>1);
	//USHORT * video_buffer = (USHORT *)ddsd->lpSurface;
	//char szOutput[256];
	//sprintf(szOutput, "lpitch16 = %d", lpitch16 );
	//OutputDebugString(szOutput);
	/*
	if (lpitch16 == 800)
	{
		for (int index = 0; index < 1000; index++)
		{
			int red = rand()%256;
			int green = rand()%256;
			int blue = rand()%256;
			int x = rand()/320;
			int y = rand()/240;
			USHORT pixel = (blue & 31)+((green & 31)<<5)+((red & 31)<< 10);
			video_buffer[x + y * lpitch16 ] = pixel;
		}
	}
	
	//*/

	return hr;
}
//到此为之
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, ReleaseDC, (HDC a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, ReleaseDC);
HRESULT IGA_IDirectDrawSurface::ReleaseDC(HDC hDC)
{
	OutputDebugString("IGA_IDirectDrawSurface::ReleaseDC()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnReleaseDC(this, hDC);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, Restore, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, Restore);
HRESULT IGA_IDirectDrawSurface::Restore()
{
	OutputDebugString("IGA_IDirectDrawSurface::Restore()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnRestore(this );
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, SetClipper, (LPDIRECTDRAWCLIPPER a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, SetClipper);
HRESULT IGA_IDirectDrawSurface::SetClipper(LPDIRECTDRAWCLIPPER lpddclip)
{
	OutputDebugString("IGA_IDirectDrawSurface::SetClipper()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnSetClipper(this, lpddclip);
	return hr;
}

//只能导出一部分界面图
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, SetColorKey, (DWORD a, LPDDCOLORKEY b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, SetColorKey);
HRESULT IGA_IDirectDrawSurface::SetColorKey(DWORD word, LPDDCOLORKEY lpddcolor)
{
	OutputDebugString("IGA_IDirectDrawSurface::SetColorKey()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnSetColorKey(this, word, lpddcolor);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, SetOverlayPosition, (LONG a, LONG b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, SetOverlayPosition);
HRESULT IGA_IDirectDrawSurface::SetOverlayPosition(LONG a, LONG b)
{
	OutputDebugString("IGA_IDirectDrawSurface::SetOverlayPosition()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnSetOverlayPosition(this, a, b);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, SetPalette, (LPDIRECTDRAWPALETTE a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, SetPalette);
HRESULT IGA_IDirectDrawSurface::SetPalette(LPDIRECTDRAWPALETTE lpddpal)
{
	OutputDebugString("IGA_IDirectDrawSurface::SetPalette()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnSetPalette(this, lpddpal);
	char chOutPut[256];
	sprintf(chOutPut, " hr = %d, lpddpal = %d ", hr, lpddpal);
	OutputDebugString(chOutPut);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, Unlock, (LPVOID a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, Unlock);
HRESULT IGA_IDirectDrawSurface::Unlock(LPVOID lpRect)
{	
	//OutputDebugString("IGA_IDirectDrawSurface::Unlock");
	
	HRESULT hr = E_FAIL;
	CHAR chOutputString[255];

	Draw_Video();
/*/
	USHORT * back_buffer = (USHORT *)g_ddsd.lpSurface;
	int lpitch = (int)(g_ddsd.lPitch >> 1  );
	//////////////////////////////////////////////////////////////////////////
	int dwWidth = g_ddsd.dwWidth;
	int dwHeight = g_ddsd.dwHeight;
	//sprintf(chOutputString, "In Unlock, lpitch : %d, Width:%d, Height:%d ", lpitch, dwWidth, dwHeight );
	//OutputDebugString(chOutputString);

	//////////////////////////////////////////////////////////////////////////
	g_pIDirectVideo->CheckForLoop();
	g_pVideoData = (UCHAR*)g_pIDirectVideo->m_pVideoBuffer;
	UCHAR * lpBuffer = g_pVideoData;
	int nVideoHeight = g_pIDirectVideo->m_VideoHeight;
	int nVideoWidth  = g_pIDirectVideo->m_VideoWidth;
	//sprintf(chOutputString, "IGA_IDirectDrawSurface::UnLock(),g_pVideoData = %p,nHeight = %d, nWidth = %d",g_pVideoData, nVideoHeight, nVideoWidth);
	//OutputDebugString(chOutputString);

	//if (FAILED(g_lpddpal->SetEntries(0,0,MAX_COLORS_PALETTE,def_palette)))
	//	return false;


	if (lpitch == 800 )
	{
		for (int yh = 0 ; yh < nVideoHeight; yh ++) 
		{
			for (int xw = 0 ; xw < nVideoWidth; xw ++)
			{
				UCHAR blue  = lpBuffer[ yh*nVideoWidth*3 + xw*3 + 0 ] >> 3;
				UCHAR green = lpBuffer[ yh*nVideoWidth*3 + xw*3 + 1 ] >> 2;
				UCHAR red   = lpBuffer[ yh*nVideoWidth*3 + xw*3 + 2 ] >> 3;
				USHORT pixel = _RGB16BIT565(red, green, blue );
				back_buffer[xw   + (nVideoHeight  - yh - 1)* lpitch  ] = pixel;
			}
		}
	}
	
//*/

/*	
	
	for (int yh = 0 ; yh < bitmap8.bitmapinfoheader.biHeight; yh ++) 
	{
		for (int xw = 0 ; xw < bitmap8.bitmapinfoheader.biWidth; xw ++)
		{
			back_buffer[ xw + yh * lpitch >>1 ] = bitmap8.buffer[yh * bitmap8.bitmapinfoheader.biWidth + xw]; 
		}
	}
//*/

/*
	int       Colorcount[4096];   
	short     Colorindex[4096]; 
	memset(Colorindex,0,4096*sizeof(short));   
	memset(Colorcount,0,4096*sizeof(int));   
	int   i,j;   
	int   blue,green,red,clrindex,colornum=0;   
	struct   panel
	{   
		int   blue;   
		int   green;   
		int   red;   
	}pan[256];

	for(i=0;i<nVideoHeight * nVideoWidth * 3;)   
	{   
		blue  = lpBuffer[i++]&0xf0;   
		green = lpBuffer[i++]&0xf0;   
		red   = lpBuffer[i++]&0xf0;   
		clrindex=(blue<<4)+green+(red>>4);   
		Colorcount[clrindex]++;   
	}  

	for(i=0;i<4096;i++)   
	{   
		if(Colorcount[i]!=0)   
		{   
			Colorcount[colornum]=Colorcount[i];   
			Colorindex[colornum]=short(i);   
			colornum++;   
		}   
	}  

	for(i=0,j=0;i<256;i++)   
	{   
		Colorcount[colornum-i-1]=i;   
		pan[i].blue=(Colorindex[colornum-i-1]&0xf00)>>4;   
		pan[i].green=(Colorindex[colornum-i-1]&0x0f0);   
		pan[i].red=(Colorindex[colornum-i-1]&0x00f)<<4;   
	} 

	if(colornum>256)   
	{   
		int   Colorerror,Colorerr;   
		for(i=0;i<colornum-256;i++)   
		{   
			Colorerr=1000000;   
			blue=(Colorindex[i]&0xf00)>>4;   
			green=(Colorindex[i]&0x0f0);   
			red=(Colorindex[i]&0x00f)<<4;   
			clrindex=0;   
			for(j=0;j<256;j++)   
			{   
				Colorerror=(blue-pan[j].blue)*(blue-pan[j].blue)+   
					(green-pan[j].green)*(green-pan[j].green)+   
					(red-pan[j].red)*(red-pan[j].red);   
				if(Colorerror<Colorerr)   
				{   
					Colorerr=Colorerror;   
					clrindex=j;   
				}   
			}   
			Colorcount[i]=clrindex;   
		}   
	}   

	for(i=0 ; i<nVideoHeight * nVideoWidth * 3;)   
	{   
		blue  = lpBuffer[i++]&0xf0;   
		green = lpBuffer[i++]&0xf0;   
		red   = lpBuffer[i++]&0xf0;   
		clrindex=(blue<<4)+green+(red>>4);   
		for(j=colornum-1;j>=0;j--) 
		{
			if(clrindex==Colorindex[j])   
			{   
				video_buffer[ nVideoWidth * (nVideoHeight-(i- 3)/3/nVideoWidth -1) + (i-3)/3%nVideoWidth] = (unsigned char)(Colorcount[j]);
				break;   
			}   
		}
	}  
//*/

	//////////////////////////////////////////////////////////////////////////

	//DWORD RGB565Mask[4] = {0xF800, 0x07E0, 0x001F, 0};
	////DWORD RGB32Mask[] = {0xFF000000, 0x00ff0000, 0x0000ff00, 0x00000000 };
	//char chFileName[256];
	//sprintf(chFileName, "Z:\\temp_%d.bmp",i); 
	//FILE*   fp;   
	//if(  (fp=fopen(chFileName,   "wb"))   !=   NULL   )   
	//{   
	//	//   保存文件头   
	//	BITMAPFILEHEADER   FileHeader;   
	//	FileHeader.bfType   =   'MB';
	//	FileHeader.bfSize   =   nWidth   *   nHeight   *   2   +   0x36;   
	//	FileHeader.bfReserved1   =   0;   
	//	FileHeader.bfReserved2   =   0;   
	//	FileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGB565Mask);   
	//	fwrite(&FileHeader,   sizeof(BITMAPFILEHEADER),   1,   fp);   
	//	//   保存文件信息   
	//	BITMAPINFOHEADER   Header;   
	//	Header.biSize   =   sizeof(BITMAPINFOHEADER);     //   结构的大小   
	//	Header.biWidth   =   nWidth;                     //   宽   
	//	Header.biHeight   =   -nHeight;                     //   高   
	//	Header.biPlanes   =   1;                         //   固定   
	//	Header.biBitCount   =   16;                         //   颜色数   
	//	Header.biCompression   =   BI_BITFIELDS;                 //   是否压缩   
	//	Header.biSizeImage   =   nWidth   *   nHeight   *   2;     //   图片的大小   
	//	Header.biXPelsPerMeter   =   0;   
	//	Header.biYPelsPerMeter   =   0;   
	//	Header.biClrUsed   =   0;   
	//	Header.biClrImportant   =   0;   
	//	fwrite(&Header,   Header.biSize,   1,   fp);   
	//	fwrite(&RGB565Mask, sizeof(RGB565Mask),   1,   fp); 
	//	fseek(fp,   FileHeader.bfOffBits,   SEEK_SET); 
	//	for(int i=0; i < nHeight;   i++)   
	//	{   
	//		fwrite(lpBuffer,  nWidth * 4, 1, fp);
	//		lpBuffer += nWidth <<1 ;
	//	}
	//}
	//fclose(fp);

	//sprintf(chOutputString, "pixel = %d", pixel);
	//OutputDebugString(chOutputString);
//////////////////////////////////////////////////////////////////////////


	//for (int index_y = 0; index_y < 240; index_y++)
	//{
	//	for (int index_x = 0; index_x < 320; index_x++)
	//	{
	//		// get BGR values
	//		UCHAR blue  = (bitmap.buffer[index_y*320*3 + index_x*3 + 0]) >> 3,
	//			  green = (bitmap.buffer[index_y*320*3 + index_x*3 + 1]) >> 2,
	//			  red   = (bitmap.buffer[index_y*320*3 + index_x*3 + 2]) >> 3;
	//		USHORT pixel = _RGB16BIT565(red, green, blue);

	//		video_buffer[index_x + (index_y*lpitch)] = pixel;
	//	} // end for index_x

	//} // end for index_y
	//////////////////////////////////////////////////////////////////////////

	//*/
	hr = IGA_IDirectDrawSurface::m_pfnUnlock(this, lpRect);
	//OutputDebugString("IGA_IDirectDrawSurface::Unlock()");

	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, UpdateOverlay, (LPRECT a, LPDIRECTDRAWSURFACE b,LPRECT c, DWORD d, LPDDOVERLAYFX e), (this, a, b, c, d, e));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, UpdateOverlay)
HRESULT IGA_IDirectDrawSurface::UpdateOverlay(LPRECT rect, LPDIRECTDRAWSURFACE lpddsur,LPRECT rect2,DWORD word, LPDDOVERLAYFX lpddol)
{
	OutputDebugString("IGA_IDirectDrawSurface::UpdateOverlay()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnUpdateOverlay(this, rect, lpddsur, rect2,word, lpddol );
	return hr;
}

//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, UpdateOverlayDisplay, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, UpdateOverlayDisplay);
HRESULT IGA_IDirectDrawSurface::UpdateOverlayDisplay(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface::UpdateOverlayDisplay()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnUpdateOverlayDisplay(this, a);
	return hr;
}

//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface, UpdateOverlayZOrder, (DWORD a, LPDIRECTDRAWSURFACE b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface, UpdateOverlayZOrder)
HRESULT IGA_IDirectDrawSurface::UpdateOverlayZOrder(DWORD word, LPDIRECTDRAWSURFACE lpddsur)
{
	OutputDebugString("IGA_IDirectDrawSurface::UpdateOverlayZOrder()");
	HRESULT hr = IGA_IDirectDrawSurface::m_pfnUpdateOverlayZOrder(this, word, lpddsur);
	return hr;
}

//////////////////////////////////////////////////////////////////////////
/* IDirectDrawPalette */
//////////////////////////////////////////////////////////////////////////

IGA_IMPLEMENT_CLASS(IDirectDrawPalette)
BOOL IGA_IDirectDrawPalette::Init(IDirectDrawPalette *pObject)
{
	ASSERT(pObject != NULL);
	INIT_VIRTUAL_OBJECT(pObject);
	DWORD	dwIndex	= 0;
	/*** IUnknown methods ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, QueryInterface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddRef);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Release);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetCaps);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetEntries);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Initialize);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetEntries);
	return true;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawPalette, QueryInterface)
HRESULT IGA_IDirectDrawPalette::QueryInterface(REFIID riid, void** ppvObj)
{
	OutputDebugString("IGA_IDirectDrawPalette::QueryInterface()");
	HRESULT hr = IGA_IDirectDrawPalette::m_pfnQueryInterface(this, riid, ppvObj );
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawPalette, AddRef)
ULONG IGA_IDirectDrawPalette::AddRef()
{
	OutputDebugString("IGA_IDirectDrawPalette::AddRef()");
	ULONG re = IGA_IDirectDrawPalette::m_pfnAddRef(this );	
	return re;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawPalette, Release)
ULONG IGA_IDirectDrawPalette::Release()
{
	OutputDebugString("IGA_IDirectDrawPalette::Release()");
	char chOutputString[256];
	ULONG re = IGA_IDirectDrawPalette::m_pfnRelease(this);
	sprintf(chOutputString, "IGA_IDirectDrawPalette::Release(),re = %d",re);
	OutputDebugString(chOutputString);
	return re;
}


IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawPalette, GetCaps)
HRESULT IGA_IDirectDrawPalette::GetCaps(LPDWORD lpword)
{
	OutputDebugString("IGA_IDirectDrawPalette::GetCaps()");
	HRESULT re = IGA_IDirectDrawPalette::m_pfnGetCaps(this ,lpword );	
	return re;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawPalette, GetEntries)
HRESULT IGA_IDirectDrawPalette::GetEntries(DWORD param1, DWORD param2, DWORD param3, LPPALETTEENTRY param4)
{
	OutputDebugString("IGA_IDirectDrawPalette::GetEntries()");
	HRESULT re = IGA_IDirectDrawPalette::m_pfnGetEntries(this, param1, param2, param3, param4 );	
	return re;
}
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawPalette, Initialize)
HRESULT IGA_IDirectDrawPalette::Initialize(LPDIRECTDRAW param1, DWORD param2, LPPALETTEENTRY param3)
{
	OutputDebugString("IGA_IDirectDrawPalette::Initialize()");
	HRESULT re = IGA_IDirectDrawPalette::m_pfnInitialize(this, param1, param2, param3 );	
	return re;
}
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawPalette, SetEntries)
HRESULT IGA_IDirectDrawPalette::SetEntries(DWORD param1, DWORD param2, DWORD param3, LPPALETTEENTRY param4)
{

	OutputDebugString("IGA_IDirectDrawPalette::SetEntries()");
	HRESULT re = IGA_IDirectDrawPalette::m_pfnSetEntries(this, param1, param2, param3, param4 );

	/*
	char szOutput[256];
	sprintf_s(szOutput, "param1 = %d, param2 = %d, param3 = %d", param1, param2, param3 );
	OutputDebugString(szOutput );

	static int iname = 1;
	char szFileName[256];
	sprintf(szFileName,"Z:\\temp_%d.txt", iname );
	iname++;

	FILE * fp;
	fp = fopen(szFileName, "w+");
	BYTE red, green, blue, flag;
	for (int i = 0; i< 256; i++)
	{
		red =   param4[i].peRed;
		green = param4[i].peGreen;
		blue =  param4[i].peBlue;
		flag =  param4[i].peFlags;
		//sprintf(szOutput, "red = %d, green = %d, blue = %d, flag = %d\n", red, green, blue, flag );
		sprintf(szOutput, "%d, %d, %d, %d,\n", red, green, blue, flag );
		fwrite(szOutput, strlen(szOutput), 1, fp );
	}
	fclose(fp);
	//*/

	return re;
}


/*********************************************************************************************************************************/
/*    IDirectDraw4   */
/*********************************************************************************************************************************/
IGA_IDirectDraw4::IGA_IDirectDraw4(IDirectDraw4*  pIDirectDraw) 
:m_pIDirectDraw4(pIDirectDraw)
{
	/*if ( NULL == g_pFlashSprite)
	{
		g_pFlashSprite = new FlashSprite();
	}
	if (NULL == g_pIDirectVideo )
	{
		g_pIDirectVideo = new CDirectXVideoTexture();
	}
	g_pFlashSprite->StartFlash("test.swf",240,180);
	g_pIDirectVideo->CreateFromAVIFile("test.avi");
	Load_Bitmap_File(&bitmap,"bitmap24.bmp");
	Load_Bitmap_File(&bitmap_back, "BITMAP_BACKGROUND.bmp");
	Load_Bitmap_File(&bitmap_over, "BITMAP_MOUSE_OVER.bmp");
	InstallHotKey();*/

	//InitDrawing();
	
	OutputDebugString("IGA_IDirectDraw4::IGA_IDirectDraw4()");
}

IGA_IDirectDraw4::~IGA_IDirectDraw4()
{
	/*Unload_Bitmap_File(&bitmap);
	Unload_Bitmap_File(&bitmap_back);
	Unload_Bitmap_File(&bitmap_over);*/
	UnInitDrawing();
	OutputDebugString("IGA_IDirectDraw4::~IGA_IDirectDraw4()");
}

HRESULT IGA_IDirectDraw4::QueryInterface (REFIID riid, LPVOID FAR * ppvObj) 
{
	OutputDebugString("IGA_IDirectDraw4::QueryInterface");
	HRESULT hr = m_pIDirectDraw4->QueryInterface (riid,  ppvObj);
	if ( ::IsEqualIID(riid, Proxy_IDirectDraw  )) 
	{		  
		IGA_IDirectDraw*   pIDirectDraw = new IGA_IDirectDraw((IDirectDraw*)*ppvObj);
		*ppvObj  = pIDirectDraw;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw4)) 
	{

		*ppvObj = this;
	}
	else if ( ::IsEqualIID(riid, Proxy_IDirectDraw7)) 
	{
		IGA_IDirectDraw7*   pIDirectDraw = new IGA_IDirectDraw7((IDirectDraw7*)*ppvObj);
		*ppvObj = pIDirectDraw;
	}

	return hr;
}
ULONG   IGA_IDirectDraw4::AddRef ()  
{
	OutputDebugString("IGA_IDirectDraw4::AddRef()");
	return  m_pIDirectDraw4->AddRef ()  ;
}
ULONG   IGA_IDirectDraw4::Release() 
{
	OutputDebugString("IGA_IDirectDraw4::Release()");
	ULONG  ulRet =  m_pIDirectDraw4->Release() ;
	
	return ulRet;
}


HRESULT IGA_IDirectDraw4::Compact() 
{
	OutputDebugString("IGA_IDirectDraw4::Compact()");
	return m_pIDirectDraw4->Compact() ;
}
HRESULT IGA_IDirectDraw4::CreateClipper( DWORD param1, LPDIRECTDRAWCLIPPER FAR* param2, IUnknown FAR * param3 ) 
{
	OutputDebugString("IGA_IDirectDraw4::CreateClipper()");
	return m_pIDirectDraw4->CreateClipper( param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw4::CreatePalette( DWORD param0, LPPALETTEENTRY param1, LPDIRECTDRAWPALETTE FAR* param2, IUnknown FAR * param3 ) 
{
	OutputDebugString("IGA_IDirectDraw4::CreatePalette()");
	return m_pIDirectDraw4->CreatePalette( param0,  param1,  param2,  param3 ) ;
}
HRESULT IGA_IDirectDraw4::CreateSurface( LPDDSURFACEDESC2 param1, LPDIRECTDRAWSURFACE4 FAR * param2, IUnknown FAR * param3) 
{
	OutputDebugString("IGA_IDirectDraw4::CreateSurface()");
	HRESULT hr = m_pIDirectDraw4->CreateSurface(  param1,  param2,  param3) ; 

	static	bool bReplaced	= false;
	if(SUCCEEDED(hr))
	{
		OutputDebugString("success IGA_IDirectDraw4::CreateSurface()");
		if(!bReplaced)
		{
			IGA_IDirectDrawSurface4::Init(*param2);
			bReplaced = true;
		}
	}
	else
	{
		OutputDebugString("Failed IGA_IDirectDraw4::CreateSurface()");
	}
	return hr;
}
HRESULT IGA_IDirectDraw4::DuplicateSurface(  LPDIRECTDRAWSURFACE4 param1, LPDIRECTDRAWSURFACE4 FAR * param2) 
{
	OutputDebugString("IGA_IDirectDraw4::DuplicateSurface()");
	return m_pIDirectDraw4->DuplicateSurface(  param1,  param2) ;
}
HRESULT IGA_IDirectDraw4::EnumDisplayModes(  DWORD param1, LPDDSURFACEDESC2 param2, LPVOID param3, LPDDENUMMODESCALLBACK2 param4 ) 
{
	OutputDebugString("IGA_IDirectDraw4::EnumDisplayModes()");
	return m_pIDirectDraw4->EnumDisplayModes(   param1,  param2,  param3,  param4 ) ;
}
HRESULT IGA_IDirectDraw4::EnumSurfaces( DWORD param1, LPDDSURFACEDESC2 param2, LPVOID param3,LPDDENUMSURFACESCALLBACK2  param4) 
{
	OutputDebugString("IGA_IDirectDraw4::EnumSurface()");
	return m_pIDirectDraw4->EnumSurfaces(  param1,  param2,  param3,  param4) ;
}
HRESULT IGA_IDirectDraw4::FlipToGDISurface()
{
	OutputDebugString("IGA_IDirectDraw4::FlipToGDISurface()");
	return m_pIDirectDraw4->FlipToGDISurface();
}
HRESULT IGA_IDirectDraw4::GetCaps(  LPDDCAPS param1, LPDDCAPS param2) 
{
	OutputDebugString("IGA_IDirectDraw4::GetCaps()");
	return m_pIDirectDraw4->GetCaps(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw4::GetDisplayMode(  LPDDSURFACEDESC2 param1) 
{
	OutputDebugString("IGA_IDirectDraw4::GetDisplayMode()");
	return m_pIDirectDraw4->GetDisplayMode(   param1) ;
}
HRESULT IGA_IDirectDraw4::GetFourCCCodes(  LPDWORD param1, LPDWORD param2) 
{
	OutputDebugString("IGA_IDirectDraw4::GetFourCCCodes()");
	return m_pIDirectDraw4->GetFourCCCodes(   param1,  param2) ;
}
HRESULT IGA_IDirectDraw4::GetGDISurface( LPDIRECTDRAWSURFACE4 FAR * param1) 
{
	OutputDebugString("IGA_IDirectDraw4::GetGDISurface()");
	return m_pIDirectDraw4->GetGDISurface(  param1) ;
}
HRESULT IGA_IDirectDraw4::GetMonitorFrequency( LPDWORD param1) 
{
	OutputDebugString("IGA_IDirectDraw4::GetMonitorFrequency()");
	return m_pIDirectDraw4->GetMonitorFrequency(  param1) ;
}
HRESULT IGA_IDirectDraw4::GetScanLine( LPDWORD param1) 
{
	OutputDebugString("IGA_IDirectDraw4::GetScanLine()");
	return m_pIDirectDraw4->GetScanLine(  param1) ;
}
HRESULT IGA_IDirectDraw4::GetVerticalBlankStatus( LPBOOL param1) 
{
	OutputDebugString("IGA_IDirectDraw4::GetVerticalBlankStatus()");
	return m_pIDirectDraw4->GetVerticalBlankStatus(  param1) ;
}
HRESULT IGA_IDirectDraw4::Initialize( GUID FAR * param1) 
{
	OutputDebugString("IGA_IDirectDraw4::Initialize()");
	return m_pIDirectDraw4->Initialize(  param1) ;
}
HRESULT IGA_IDirectDraw4::RestoreDisplayMode() 
{
	OutputDebugString("IGA_IDirectDraw4::RestoreDisplayMode()");
	return m_pIDirectDraw4->RestoreDisplayMode() ;
}
HRESULT IGA_IDirectDraw4::SetCooperativeLevel( HWND param1, DWORD param2) 
{
	OutputDebugString("IGA_IDirectDraw4::SetCooperativeLevel()");
	HRESULT hr =  m_pIDirectDraw4->SetCooperativeLevel(  param1,  param2) ;
	return hr;
}

HRESULT IGA_IDirectDraw4::SetDisplayMode( DWORD param1, DWORD param2,DWORD param3,DWORD param4,DWORD param5)
{
	OutputDebugString("IGA_IDirectDraw4::SetDisplayMode()");
	return m_pIDirectDraw4->SetDisplayMode(  param1,  param2, param3,param4,param5); 
}
HRESULT IGA_IDirectDraw4::WaitForVerticalBlank( DWORD param1, HANDLE param2) 
{
	OutputDebugString("IGA_IDirectDraw4::WaitForVerticalBlank()");
	return m_pIDirectDraw4->WaitForVerticalBlank( param1,  param2) ;
}

HRESULT IGA_IDirectDraw4::GetAvailableVidMem( LPDDSCAPS2 param1, LPDWORD param2, LPDWORD param3) 
{
	OutputDebugString("IGA_IDirectDraw4::GetAvailableVidMem()");
	return m_pIDirectDraw4->GetAvailableVidMem(  param1,  param2,  param3) ;
}

HRESULT IGA_IDirectDraw4::GetSurfaceFromDC (HDC param1, LPDIRECTDRAWSURFACE4 * param2) 
{
	OutputDebugString("IGA_IDirectDraw4::GetSurfaceFromDC()");
	return m_pIDirectDraw4->GetSurfaceFromDC (param1, param2) ;
}
HRESULT IGA_IDirectDraw4::RestoreAllSurfaces() 
{
	OutputDebugString("IGA_IDirectDraw4::RestoreAllSurfaces()");
	return m_pIDirectDraw4->RestoreAllSurfaces() ;
}
HRESULT IGA_IDirectDraw4::TestCooperativeLevel() 
{
	OutputDebugString("IGA_IDirectDraw4::TestCooperativeLevel()");
	return m_pIDirectDraw4->TestCooperativeLevel() ;
}
HRESULT IGA_IDirectDraw4::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER param1, DWORD  param2) 
{
	OutputDebugString("IGA_IDirectDraw4::GetDeviceIdentifier()");
	return m_pIDirectDraw4->GetDeviceIdentifier(param1, param2 ) ;
}

//
////////////////////////////////////////////////////////////////////////////
////IGA_IDirectDrawSurface4
IGA_IMPLEMENT_CLASS(IDirectDrawSurface4)
BOOL IGA_IDirectDrawSurface4::Init(IDirectDrawSurface4 *pObject)
{
	ASSERT(pObject != NULL);
	INIT_VIRTUAL_OBJECT(pObject);
	DWORD	dwIndex	= 0;
	/*** IUnknown methods ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, QueryInterface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddRef);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Release);
	/*** IDirectDrawSurface methods ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddAttachedSurface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddOverlayDirtyRect);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Blt);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, BltBatch);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, BltFast);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, DeleteAttachedSurface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, EnumAttachedSurfaces);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, EnumOverlayZOrders);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Flip);	
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetAttachedSurface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetBltStatus);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetCaps);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetClipper);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetColorKey);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetDC);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetFlipStatus);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetOverlayPosition);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetPalette);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetPixelFormat);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetSurfaceDesc);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Initialize);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, IsLost);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Lock);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, ReleaseDC);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Restore);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetClipper);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetColorKey);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetOverlayPosition);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetPalette);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Unlock);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, UpdateOverlay);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, UpdateOverlayDisplay);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, UpdateOverlayZOrder);
	/*** Added in the v2 interface ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetDDInterface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, PageLock);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, PageUnlock);
	/*** Added in the v3 interface ***/

	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetSurfaceDesc);

	/*** Added in the v4 interface ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetPrivateData);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetPrivateData);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, FreePrivateData);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetUniquenessValue);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, ChangeUniquenessValue);
	return	TRUE;
}
//
///*** IUnknown methods ***/
IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, QueryInterface, (REFIID riid, void** ppvObj), (this, riid, ppvObj));
IMPLEMENT_MEMBER_FUNCTION(ULONG, IDirectDrawSurface4, AddRef, (void), (this));
IMPLEMENT_MEMBER_FUNCTION(ULONG, IDirectDrawSurface4, Release, (void), (this));

/*** IDirectDrawSurface methods ***/
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, AddAttachedSurface,(LPDIRECTDRAWSURFACE4 a),(this,a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, AddAttachedSurface);
HRESULT IGA_IDirectDrawSurface4::AddAttachedSurface(LPDIRECTDRAWSURFACE4 a)
{
	OutputDebugString("IGA_IDirectDrawSurface4::AddAttachedSurface()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnAddAttachedSurface(this, a);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, AddOverlayDirtyRect, (LPRECT a), (this, a));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, AddOverlayDirtyRect);
HRESULT IGA_IDirectDrawSurface4::AddOverlayDirtyRect(LPRECT rect)
{
	OutputDebugString("IGA_IDirectDrawSurface4::AddOverlayDirtyRect()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnAddOverlayDirtyRect(this, rect);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, Blt, (LPRECT a, LPDIRECTDRAWSURFACE4 b, LPRECT c, DWORD d, LPDDBLTFX e), (this, a, b, c, d, e));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, Blt);
HRESULT IGA_IDirectDrawSurface4::Blt(LPRECT a, LPDIRECTDRAWSURFACE4 b, LPRECT c, DWORD d, LPDDBLTFX e)
{
	//UCHAR * buffer = (UCHAR *)g_ddsd2.lpSurface;
	//memset(buffer, 0, 1000 * 100);
	OutputDebugString("IGA_IDirectDrawSurface4::Blt()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnBlt(this, a, b, c, d, e);
	return hr;
}
//
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, BltBatch, (LPDDBLTBATCH a, DWORD b, DWORD c), (this, a, b, c));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, BltBatch);
HRESULT IGA_IDirectDrawSurface4::BltBatch(LPDDBLTBATCH lpblt, DWORD word1, DWORD word2)
{
	OutputDebugString("IGA_IDirectDrawSurface4::BltBatch()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnBltBatch(this, lpblt, word1, word2 );
	return hr;
}


//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, BltFast, (DWORD a, DWORD b, LPDIRECTDRAWSURFACE4 c, LPRECT d, DWORD e), (this, a, b, c, d, e));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, BltFast);
HRESULT IGA_IDirectDrawSurface4::BltFast(DWORD a, DWORD b, LPDIRECTDRAWSURFACE4 c, LPRECT d, DWORD e)
{
	//UCHAR * buffer = (UCHAR *)g_ddsd2.lpSurface;
	//memset(buffer, 0, 1000 * 100);
	//char chOutputString[256];
	//sprintf(chOutputString, "Rect address : %p", d);
	//OutputDebugString(chOutputString);

	//char chOutputString[256];
	//USHORT * video_buffer = (USHORT *)g_ddsd2.lpSurface;
	////memset(video_buffer, 0 ,100*100);
	//int lpitch = (int)(g_ddsd.lPitch  >> 1 );

	//int nHeight = g_pFlashSprite->m_nHeight;
	//int nWidth  = g_pFlashSprite->m_nWidth;

	//sprintf(chOutputString, "video_buffer address is : %p, nHeight = %d, nWidth = %d",video_buffer, nHeight, nWidth);
	//OutputDebugString(chOutputString);


	//g_pFlashSprite->m_pFlashPlayer->Update();
	//g_pFlashSprite->m_pFlashPlayer->Render();
	//g_pFlashData = (UCHAR *)g_pFlashSprite->m_pFlashPlayer->GetFlashFrameBuffer();
	//UCHAR*  lpBuffer = g_pFlashData;

	//for (int y = 0 ; y < nHeight; y ++) 
	//{
	//	//memset(video_buffer+ (y*lpitch) , 0, nWidth );
	//	/*for (int x = 0 ; x < nWidth; x ++)
	//	{
	//		UCHAR blue  = lpBuffer[ y*nWidth*3 + x*3 + 0 ] >>3;
	//		UCHAR green = lpBuffer[ y*nWidth*3 + x*3 + 1 ] >>2;
	//		UCHAR red   = lpBuffer[ y*nWidth*3 + x*3 + 2 ] >>3;
	//		USHORT pixel = _RGB16BIT565(red, green, blue );
	//		video_buffer[x  + (nHeight- y -1)*lpitch ] = pixel;

	//	}*/
	//}

	OutputDebugString("IGA_IDirectDrawSurface4::BltFast()");
	HRESULT hr = S_OK;
	hr = IGA_IDirectDrawSurface4::m_pfnBltFast(this, a,b,c,d,e);

	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, DeleteAttachedSurface, (DWORD a,LPDIRECTDRAWSURFACE4 b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, DeleteAttachedSurface);
HRESULT IGA_IDirectDrawSurface4::DeleteAttachedSurface(DWORD word,LPDIRECTDRAWSURFACE4 lpddsur)
{
	OutputDebugString("IGA_IDirectDrawSurface4::DeleteAttachedSurface()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnDeleteAttachedSurface(this, word, lpddsur);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, EnumAttachedSurfaces, (LPVOID a, LPDDENUMSURFACESCALLBACK4 b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, EnumAttachedSurfaces);
HRESULT IGA_IDirectDrawSurface4::EnumAttachedSurfaces(LPVOID a, LPDDENUMSURFACESCALLBACK2 b)
{
	OutputDebugString("IGA_IDirectDrawSurface4::EnumAttachedSurface()");
	LRESULT hr = IGA_IDirectDrawSurface4::m_pfnEnumAttachedSurfaces(this, a,b);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, EnumOverlayZOrders, (DWORD a, LPVOID b, LPDDENUMSURFACESCALLBACK4 c), (this, a, b, c));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, EnumOverlayZOrders);
HRESULT IGA_IDirectDrawSurface4::EnumOverlayZOrders(DWORD a,LPVOID b,LPDDENUMSURFACESCALLBACK2 c)
{
	OutputDebugString("IGA_IDirectDrawSurface::EnumOverlayZOrder()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnEnumOverlayZOrders(this, a,b,c);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, Flip, (LPDIRECTDRAWSURFACE4 a, DWORD b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, Flip);
HRESULT IGA_IDirectDrawSurface4::Flip(LPDIRECTDRAWSURFACE4 a, DWORD b)
{
	HRESULT hr = E_FAIL;
	/*
	char chOutputString[256];
	///*
	//*视频播放部分
	
	memset(&g_ddsd2, 0, sizeof(g_ddsd2));
	g_ddsd2.dwSize = sizeof(g_ddsd2);
	RECT rect;
	g_pddbacksurface->Lock(NULL, &g_ddsd2, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL );
	USHORT * back_buffer = (USHORT *)g_ddsd2.lpSurface;
	int back_lpitch = (int)(g_ddsd2.lPitch  >> 1 );
	//Draw_Video();
	Draw_Flash();
	g_pddbacksurface->Unlock(NULL);

	//*/
	OutputDebugString("IGA_IDirectDrawSurface::Flip()");
	hr = IGA_IDirectDrawSurface4::m_pfnFlip(this, a, b);
	
	return hr;

}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetAttachedSurface, (LPDDSCAPS2 a, LPDIRECTDRAWSURFACE4 FAR * b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetAttachedSurface);
HRESULT IGA_IDirectDrawSurface4::GetAttachedSurface(LPDDSCAPS2 a, LPDIRECTDRAWSURFACE4 FAR * b)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetAttachedSurface()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetAttachedSurface(this, a, b);
	g_pddbacksurface = *b;
	
	return hr;
}


//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetBltStatus, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetBltStatus);
HRESULT IGA_IDirectDrawSurface4::GetBltStatus(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetBltStatus()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetBltStatus(this, a);
	
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetCaps, (LPDDSCAPS2 a), (this , a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetCaps);
HRESULT IGA_IDirectDrawSurface4::GetCaps(LPDDSCAPS2 lpdd)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetCaps()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetCaps(this, lpdd);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetClipper, (LPDIRECTDRAWCLIPPER FAR* a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetClipper)
HRESULT IGA_IDirectDrawSurface4::GetClipper(LPDIRECTDRAWCLIPPER FAR* lpddclip)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetClipper()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetClipper(this, lpddclip);
	return hr;
}

//只调用这个函数时，只能导出界面图来
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetColorKey, (DWORD a, LPDDCOLORKEY b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetColorKey);
HRESULT IGA_IDirectDrawSurface4::GetColorKey(DWORD word, LPDDCOLORKEY lpcolor)
{
	OutputDebugString("IGA_IDirectDrawSurface4()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetColorKey(this, word, lpcolor);
	return hr;
}


//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetDC, (HDC FAR * a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetDC);
HRESULT IGA_IDirectDrawSurface4::GetDC(HDC FAR * hDC)
{
	//UCHAR * buffer = (UCHAR *)g_ddsd2.lpSurface;
	//memset(buffer, 0, 1000 * 100);
	OutputDebugString("IGA_IDirectDrawSurface4::GetDC()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetDC(this, hDC);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetFlipStatus, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetFlipStatus);
HRESULT IGA_IDirectDrawSurface4::GetFlipStatus(DWORD word)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetFlipStatus()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetFlipStatus(this, word);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetOverlayPosition, (LPLONG a, LPLONG b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetOverlayPosition);
HRESULT IGA_IDirectDrawSurface4::GetOverlayPosition(LPLONG lpl, LPLONG lpl2)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetOverlayPosition()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetOverlayPosition(this, lpl,lpl2);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetPalette, (LPDIRECTDRAWPALETTE FAR* a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetPalette);
HRESULT IGA_IDirectDrawSurface4::GetPalette(LPDIRECTDRAWPALETTE FAR* lpddpal)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetPalette()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetPalette(this, lpddpal);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetPixelFormat, (LPDDPIXELFORMAT a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetPixelFormat);
HRESULT IGA_IDirectDrawSurface4::GetPixelFormat(LPDDPIXELFORMAT lpddformat)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetPixelFormat()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetPixelFormat(this, lpddformat);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetSurfaceDesc, (LPDDSURFACEDESC2 a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetSurfaceDesc);
HRESULT IGA_IDirectDrawSurface4::GetSurfaceDesc(LPDDSURFACEDESC2 a)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetSurfaceDesc()");
	LRESULT hr=IGA_IDirectDrawSurface4::m_pfnGetSurfaceDesc(this, a);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, Initialize, (LPDIRECTDRAW a, LPDDSURFACEDESC2 b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, Initialize);
HRESULT IGA_IDirectDrawSurface4::Initialize(LPDIRECTDRAW lpddraw, LPDDSURFACEDESC2 lpddsd)
{
	OutputDebugString("IGA_IDirectDrawSurface4::Initialize()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnInitialize(this, lpddraw, lpddsd);
	return hr;
}
//能导出两张图片，但不是我们想要的
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, IsLost, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, IsLost);
HRESULT IGA_IDirectDrawSurface4::IsLost()
{
	OutputDebugString("IGA_IDirectDrawSurface4::IsLost()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnIsLost(this);
	return hr;
}

//调用这个函数是终止了程序
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, Lock, (LPRECT a,LPDDSURFACEDESC2 b,DWORD c,HANDLE d), (this, a, b, c, d));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, Lock);
HRESULT IGA_IDirectDrawSurface4::Lock(LPRECT rect,LPDDSURFACEDESC2 ddsd,DWORD word ,HANDLE h)
{
	OutputDebugString("IGA_IDirectDrawSurface4::Lock()");
	HRESULT hr = E_FAIL;
	hr = IGA_IDirectDrawSurface4::m_pfnLock(this, rect, ddsd, word, h);
	g_ddsd2 = *ddsd;
	/*if ( NULL == g_pddsd2 )
	{
		g_pddsd2 = ddsd;
	}*/

	/*int lpitch16 = (int)(ddsd->lPitch >>1);
	USHORT * video_buffer = (USHORT *)ddsd->lpSurface;
	for (int index = 0; index < 1000; index++)
	{
		int red = rand()%256;
		int green = rand()%256;
		int blue = rand()%256;
		int x = rand()/640;
		int y = rand()/480;
		USHORT pixel = (blue & 31)+((green & 31)<<5)+((red & 31)<< 10);
		video_buffer[x + y * lpitch16] = pixel;
	}*/
	//pSurface->Unlock(NULL);

	//CHAR chFileName[MAX_PATH];
	//static int i = 0;
	//DDPIXELFORMAT pixelfmt;

	//if (gl_pIDirectDraw4 == NULL || gl_pIDirectDraw4->m_pIGADraw == NULL)
	//	return hr;

	//IDirectDrawSurface4* pSurface = (IDirectDrawSurface4*)this;
	//if (pSurface != NULL)
	//{
	//	memset(&pixelfmt, 0, sizeof(DDPIXELFORMAT));
	//	pixelfmt.dwSize = sizeof(DDPIXELFORMAT);
	//	hr = IGA_IDirectDrawSurface4::m_pfnGetPixelFormat(this, &pixelfmt);
	//	//g_kSurf.Draw(pSurface, 245, 140, 0, 0, 150, 140);

	//	//DDBLTFX ddbltfx; // this contains the DDBLTFX structure
	//	//memset(&ddbltfx, 0, sizeof(ddbltfx));
	//	//ddbltfx.dwSize = sizeof(ddbltfx);
	//	//ddbltfx.dwFillColor = 0x00ff00ff; 

	//	////flashSprite.Update();
	//	//RECT rect;
	//	//rect.left = 0;
	//	//rect.top = 0;
	//	//rect.right = 100;
	//	//rect.bottom = 100;
	//	//pSurface->Blt(&rect,     // ptr to dest rectangle
	//	//	NULL,       // ptr to source surface, NA            
	//	//	NULL,       // ptr to source rectangle, NA
	//	//	DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
	//	//	&ddbltfx);  // ptr to DDBLTFX structure
	//	////pSurface->Flip(NULL, DDFLIP_WAIT );

	//	if (SUCCEEDED(hr))
	//	{
	//		CExportSurface4::iterator p = g_mapExportSurface4.find(pSurface);
	//		if (p == g_mapExportSurface4.end())
	//		{
	//			sprintf(chFileName, "Z:\\Lock_SURFACE_%p.bmp",pSurface );
	//			gl_pIDirectDraw4->m_pIGADraw->SaveToBitmapFile(pSurface, chFileName);
	//			OutputDebugString(chFileName);
	//			i++;
	//			g_mapExportSurface4[pSurface] = TRUE;
	//		}
	//	}
	//}

	return hr;
}
//到此为之
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, ReleaseDC, (HDC a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, ReleaseDC);
HRESULT IGA_IDirectDrawSurface4::ReleaseDC(HDC hDC)
{
	//SetTextColor(hDC, 0x00ff00);
	//SetBkColor(hDC, 0xffffff);
	//TextOut(hDC,100,120,"test",4);
	OutputDebugString("IGA_IDirectDrawSurface4::ReleaseDC()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnReleaseDC(this, hDC);
	
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, Restore, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, Restore);
HRESULT IGA_IDirectDrawSurface4::Restore()
{
	OutputDebugString("IGA_IDirectDrawSurface4::Restore()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnRestore(this );
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, SetClipper, (LPDIRECTDRAWCLIPPER a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, SetClipper);
HRESULT IGA_IDirectDrawSurface4::SetClipper(LPDIRECTDRAWCLIPPER lpddclip)
{
	OutputDebugString("IGA_IDirectDrawSurface4::SetClipper()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnSetClipper(this, lpddclip);
	return hr;
}

//只能导出一部分界面图
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, SetColorKey, (DWORD a, LPDDCOLORKEY b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, SetColorKey);
HRESULT IGA_IDirectDrawSurface4::SetColorKey(DWORD word, LPDDCOLORKEY lpddcolor)
{
	OutputDebugString("IGA_IDirectDrawSurface4::SetColorKey()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnSetColorKey(this, word, lpddcolor);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, SetOverlayPosition, (LONG a, LONG b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, SetOverlayPosition);
HRESULT IGA_IDirectDrawSurface4::SetOverlayPosition(LONG a, LONG b)
{
	OutputDebugString("IGA_IDirectDrawSurface4::SetOverlayPosition()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnSetOverlayPosition(this, a, b);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, SetPalette, (LPDIRECTDRAWPALETTE a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, SetPalette);
HRESULT IGA_IDirectDrawSurface4::SetPalette(LPDIRECTDRAWPALETTE lpddpal)
{
	OutputDebugString("IGA_IDirectDrawSurface4::SetPalette()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnSetPalette(this, lpddpal);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, Unlock, (LPRECT a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, Unlock);
HRESULT IGA_IDirectDrawSurface4::Unlock(LPRECT lpRect)
{	
	OutputDebugString("IGA_IDirectDrawSurface4::Unlock()");
	HRESULT hr = E_FAIL;
	hr = IGA_IDirectDrawSurface4::m_pfnUnlock(this, lpRect);
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, UpdateOverlay, (LPRECT a, LPDIRECTDRAWSURFACE4 b,LPRECT c, DWORD d, LPDDOVERLAYFX e), (this, a, b, c, d, e));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, UpdateOverlay)
HRESULT IGA_IDirectDrawSurface4::UpdateOverlay(LPRECT rect, LPDIRECTDRAWSURFACE4 lpddsur,LPRECT rect2,DWORD word, LPDDOVERLAYFX lpddol)
{
	OutputDebugString("IGA_IDirectDrawSurface4::UpdateOverlay()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnUpdateOverlay(this, rect, lpddsur, rect2,word, lpddol );
	return hr;
}

//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, UpdateOverlayDisplay, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, UpdateOverlayDisplay);
HRESULT IGA_IDirectDrawSurface4::UpdateOverlayDisplay(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface4::UpdateOverlayDisplay()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnUpdateOverlayDisplay(this, a);
	return hr;
}

//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, UpdateOverlayZOrder, (DWORD a, LPDIRECTDRAWSURFACE4 b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, UpdateOverlayZOrder)
HRESULT IGA_IDirectDrawSurface4::UpdateOverlayZOrder(DWORD word, LPDIRECTDRAWSURFACE4 lpddsur)
{
	OutputDebugString("IGA_IDirectDrawSurface4::UpdateOverlayZOrder()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnUpdateOverlayZOrder(this, word, lpddsur);
	return hr;
}

/*** Added in the v2 interface ***/
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetDDInterface, (LPVOID FAR * a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetDDInterface)
HRESULT IGA_IDirectDrawSurface4::GetDDInterface(LPVOID FAR * a)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetDDInterface()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetDDInterface(this, a);
	return hr;
}

//没用调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, PageLock, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4,  PageLock)
HRESULT IGA_IDirectDrawSurface4::PageLock(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface4::PageLock()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnPageLock(this, a);
	return hr;
}
//环境变黑了，好像是关闭了灯光
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, PageUnlock, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, PageUnlock)
HRESULT IGA_IDirectDrawSurface4::PageUnlock(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface4::PageUnlock()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnPageUnlock(this, a);
	return hr;
}

/*** Added in the v3 interface ***/
//导不出图来
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, SetSurfaceDesc, (LPDDSURFACEDESC2 a, DWORD b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, SetSurfaceDesc);
HRESULT IGA_IDirectDrawSurface4::SetSurfaceDesc(LPDDSURFACEDESC2 a, DWORD b)
{
	OutputDebugString("IGA_IDirectDrawSurface4::SetSurfaceDesc()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnSetSurfaceDesc(this, a, b);
	//CHAR chFileName[MAX_PATH];
	//CHAR chOutputString[255];

	//DDSURFACEDESC2 desc;
	//DDCOLORKEY clrKey;
	//DDPIXELFORMAT pixelfmt;	
	///*if (gl_pIDirectDraw4 == NULL || gl_pIDirectDraw4->m_pIGADraw == NULL)
	//	return hr;*/

	//IDirectDrawSurface4* pSurface = (IDirectDrawSurface4*)this;
	//if (pSurface != NULL)
	//{
	//	memset(&pixelfmt, 0, sizeof(DDPIXELFORMAT));
	//	pixelfmt.dwSize = sizeof(DDPIXELFORMAT);
	//	hr = IGA_IDirectDrawSurface4::m_pfnGetPixelFormat(pSurface, &pixelfmt);

	//	if (SUCCEEDED(hr))
	//	{
	//		if ( pixelfmt.dwRGBBitCount == 16)
	//		{
	//			//	CExportSurface::iterator p = g_mapExportSurface.find(pSurface);
	//			//	if (p == g_mapExportSurface.end())
	//			/*{
	//				sprintf(chFileName, "Z:\\BltFast_SURFACE_%p.bmp", pSurface);
	//				gl_pIDirectDraw4->m_pIGADraw->SaveToBitmapFile(pSurface, chFileName);
	//				OutputDebugString(chFileName);
	//				g_mapExportSurface4[pSurface] = TRUE;
	//			}*/
	//		}
	//	}
	//}
	return hr;
}
/*** Added in the v4 interface ***/
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, SetPrivateData, (REFGUID a, LPVOID b, DWORD c, DWORD d), (this, a, b, c, d));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, SetPrivateData)
HRESULT IGA_IDirectDrawSurface4::SetPrivateData(REFGUID a, LPVOID b, DWORD c, DWORD d)
{
	OutputDebugString("IGA_IDirectDrawSurface4::SetPrivateData()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnSetPrivateData(this, a, b, c, d);
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetPrivateData, (REFGUID a, LPVOID b, LPDWORD c), (this, a, b, c));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetPrivateData)
HRESULT IGA_IDirectDrawSurface4::GetPrivateData(REFGUID a, LPVOID b, LPDWORD c)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetPrivateData()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetPrivateData(this, a, b, c);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, FreePrivateData, (REFGUID a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, FreePrivateData)
HRESULT IGA_IDirectDrawSurface4::FreePrivateData(REFGUID a)
{
	OutputDebugString("IGA_IDirectDrawSurface4::FreePrivateData()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnFreePrivateData(this, a);
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, GetUniquenessValue, (LPDWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, GetUniquenessValue)
HRESULT IGA_IDirectDrawSurface4::GetUniquenessValue(LPDWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface4::GetUniquenessValue()");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnGetUniquenessValue(this, a);
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface4, ChangeUniquenessValue, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface4, ChangeUniquenessValue);
HRESULT IGA_IDirectDrawSurface4::ChangeUniquenessValue()
{
	OutputDebugString("IGA_IDirectDrawSurface4::ChangeUniquenessValue");
	HRESULT hr = IGA_IDirectDrawSurface4::m_pfnChangeUniquenessValue(this);
	return hr;
}



// This one is set up to work with IDirectDraw7 only !!

// ---------------------------------------------------------------------------------------
IGA_IDirectDraw7::IGA_IDirectDraw7(LPDIRECTDRAW7 pOriginal)
{
	OutputDebugString("IGA_IDirectDraw7::IGA_IDirectDraw7");
	m_pIDDraw7 = pOriginal;
	m_pIGADraw = new IGA( pOriginal);//初始化IGA指针

	if (m_pIGADraw != NULL) {
		m_pIGADraw->LoadImageResource();
	}
	
}

// ---------------------------------------------------------------------------------------
IGA_IDirectDraw7::~IGA_IDirectDraw7(void)
{
	OutputDebugString("IGA_IDirectDraw7::~IGA_IDirectDraw7");
	if ( NULL != m_pIGADraw )
	{
		delete m_pIGADraw;
		m_pIGADraw = NULL;
	}
}

// ---------------------------------------------------------------------------------------
//*************************  这三个函数是IUnKnown所包括的三个函数   ***********************************


HRESULT __stdcall IGA_IDirectDraw7::QueryInterface (REFIID a, LPVOID FAR * b)
{
	OutputDebugString("IGA_IDirectDraw7::QueryInterface");
	*b = NULL;
	HRESULT hRes = m_pIDDraw7->QueryInterface(a, b); 

	if (hRes == NOERROR) // if OK, send our "fake" address
	{
		*b = this;
	}
	return hRes;
}

// ---------------------------------------------------------------------------------------
ULONG   __stdcall IGA_IDirectDraw7::AddRef(void)
{
	OutputDebugString("IGA_IDirectDraw7::AddRef");
	return(m_pIDDraw7->AddRef());
}

// ---------------------------------------------------------------------------------------
ULONG   __stdcall IGA_IDirectDraw7::Release(void)
{
	OutputDebugString("IGA_IDirectDraw7::Release");
	ULONG count = m_pIDDraw7->Release();
	return(count);
}


//********************************************************************************************************

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::Compact(void)
{
	OutputDebugString("IGA_IDirectDraw7::Compact");
	return(m_pIDDraw7->Compact());
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::CreateClipper(DWORD a, LPDIRECTDRAWCLIPPER FAR* b, IUnknown FAR* c)
{
	OutputDebugString("IGA_IDirectDraw7::CreateClipper");
	return(m_pIDDraw7->CreateClipper(a, b, c));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::CreatePalette(DWORD a, LPPALETTEENTRY b, LPDIRECTDRAWPALETTE FAR* c, IUnknown FAR* d)
{
	OutputDebugString("IGA_IDirectDraw7::CreatePalette");
	return(m_pIDDraw7->CreatePalette(a, b, c, d));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::CreateSurface(LPDDSURFACEDESC2 a, LPDIRECTDRAWSURFACE7 FAR* b, IUnknown FAR* c)
{	
	OutputDebugString("IGA_IDirectDraw7::CreateSurface");
	CHAR chOutputString[255];		
	HRESULT	hr	= m_pIDDraw7->CreateSurface(a, b, c);
	static	bool bReplaced	= false;
	if(SUCCEEDED(hr))
	{
		if(!bReplaced)
		{
			sprintf(chOutputString, "IDirectDrawSurface7 Replaced!!!");
			OutputDebugString(chOutputString);
			IGA_IDirectDrawSurface7::Init(*b);
			bReplaced = true;
		}
	}
	return	hr;
//	return(m_pIDDraw->CreateSurface(a, b, c));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::DuplicateSurface(LPDIRECTDRAWSURFACE7 a, LPDIRECTDRAWSURFACE7 FAR* b)
{
	OutputDebugString("IGA_IDirectDraw7::DuplicateSurface");
	return(m_pIDDraw7->DuplicateSurface(a, b));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::EnumDisplayModes(DWORD a, LPDDSURFACEDESC2 b, LPVOID c, LPDDENUMMODESCALLBACK2 d)
{
	OutputDebugString("IGA_IDirectDraw7::EnumDisplayModes");
	return(m_pIDDraw7->EnumDisplayModes(a, b, c, d));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::EnumSurfaces(DWORD a, LPDDSURFACEDESC2 b, LPVOID c, LPDDENUMSURFACESCALLBACK7 d)
{
	OutputDebugString("IGA_IDirectDraw7::EnumSurfaces");
	return(m_pIDDraw7->EnumSurfaces(a, b, c, d));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::FlipToGDISurface(void)
{
	OutputDebugString("IGA_IDirectDraw7::FlipToGDISurface");
	return(m_pIDDraw7->FlipToGDISurface());
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::GetCaps(LPDDCAPS a, LPDDCAPS b)
{
	OutputDebugString("IGA_IDirectDraw7::GetCaps");
	return(m_pIDDraw7->GetCaps(a, b));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::GetDisplayMode(LPDDSURFACEDESC2 a)
{
	OutputDebugString("IGA_IDirectDraw7::GetDisplayMode");
	return(m_pIDDraw7->GetDisplayMode(a));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::GetFourCCCodes(LPDWORD a, LPDWORD b)
{
	OutputDebugString("IGA_IDirectDraw7::GetFourCCCodes");
	return(m_pIDDraw7->GetFourCCCodes(a, b));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::GetGDISurface(LPDIRECTDRAWSURFACE7 FAR * a)
{
	OutputDebugString("IGA_IDirectDraw7::GetGDISurface");
	return(m_pIDDraw7->GetGDISurface(a));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::GetMonitorFrequency(LPDWORD a)
{
	OutputDebugString("IGA_IDirectDraw7::GetMonitorFrequency");
	return(m_pIDDraw7->GetMonitorFrequency(a));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::GetScanLine(LPDWORD a)
{
	OutputDebugString("IGA_IDirectDraw7::GetScanLine");
	return(m_pIDDraw7->GetScanLine(a));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::GetVerticalBlankStatus(LPBOOL a)
{
	OutputDebugString("IGA_IDirectDraw7::GetVerticalBlankStatus");
	return(m_pIDDraw7->GetVerticalBlankStatus(a));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::Initialize(GUID FAR* a)
{
	OutputDebugString("IGA_IDirectDraw7::Initialize");
	return(m_pIDDraw7->Initialize(a));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::RestoreDisplayMode(void)
{
	OutputDebugString("IGA_IDirectDraw7::RestoreDisplayMode");
	return(m_pIDDraw7->RestoreDisplayMode());
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::SetCooperativeLevel(HWND a, DWORD b)
{
	OutputDebugString("IGA_IDirectDraw7::SetCooperativeLevel");
	HRESULT h = m_pIDDraw7->SetCooperativeLevel(a, b ); 
	return(h);
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::SetDisplayMode(DWORD a, DWORD b, DWORD c, DWORD d, DWORD e)
{
	OutputDebugString("IGA_IDirectDraw7::SetDisplayMode");
	char dummy[300];
	sprintf(dummy,"Parameters: %i, %i, %i, %i, %i \r\n",a,b,c,d,e);
	OutputDebugString(dummy);
	HRESULT h = m_pIDDraw7->SetDisplayMode(a, b, c, d, e);	
	return(h); 
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::WaitForVerticalBlank(DWORD a, HANDLE b)
{
	OutputDebugString("IGA_IDirectDraw7::WaitForVerticalBlank");
	return(m_pIDDraw7->WaitForVerticalBlank(a, b));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::GetAvailableVidMem(LPDDSCAPS2 a, LPDWORD b, LPDWORD c)
{
	OutputDebugString("IGA_IDirectDraw7::GetAvailableVidMem");
	return(m_pIDDraw7->GetAvailableVidMem(a, b, c));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::GetSurfaceFromDC(HDC a, LPDIRECTDRAWSURFACE7 *b)
{
	OutputDebugString("IGA_IDirectDraw7::GetSurfaceFromDC");
	return(m_pIDDraw7->GetSurfaceFromDC(a, b));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::RestoreAllSurfaces(void)
{
	OutputDebugString("IGA_IDirectDraw7::RestoreAllSurfaces");
	return(m_pIDDraw7->RestoreAllSurfaces());
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::TestCooperativeLevel(void)
{
	OutputDebugString("IGA_IDirectDraw7::TestCooperativeLevel");
	return(m_pIDDraw7->TestCooperativeLevel());
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2 a, DWORD b)
{
	OutputDebugString("IGA_IDirectDraw7::GetDeviceIdentifier");
	return(m_pIDDraw7->GetDeviceIdentifier(a, b));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::StartModeTest(LPSIZE a, DWORD b, DWORD c)
{
	OutputDebugString("IGA_IDirectDraw7::StartModeTest");
	return(m_pIDDraw7->StartModeTest(a, b, c));
}

// ---------------------------------------------------------------------------------------
HRESULT  __stdcall IGA_IDirectDraw7::EvaluateMode(DWORD a, DWORD *b)
{
	OutputDebugString("IGA_IDirectDraw7::EvaluateMode");
	return(m_pIDDraw7->EvaluateMode(a, b));
}



//////////////////////////////////////////////////////////////////////////////////////////////
IGA_IMPLEMENT_CLASS(IDirectDrawSurface7)

BOOL IGA_IDirectDrawSurface7::Init(IDirectDrawSurface7 *pObject)
{
	OutputDebugString("IGA_IDirectDrawSurface7::Init");
	ASSERT(pObject != NULL);
	INIT_VIRTUAL_OBJECT(pObject);
	DWORD	dwIndex	= 0;
	/*** IUnknown methods ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, QueryInterface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddRef);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Release);
	/*** IDirectDrawSurface methods ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddAttachedSurface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddOverlayDirtyRect);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Blt);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, BltBatch);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, BltFast);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, DeleteAttachedSurface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, EnumAttachedSurfaces);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, EnumOverlayZOrders);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Flip);	
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetAttachedSurface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetBltStatus);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetCaps);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetClipper);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetColorKey);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetDC);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetFlipStatus);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetOverlayPosition);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetPalette);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetPixelFormat);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetSurfaceDesc);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Initialize);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, IsLost);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Lock);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, ReleaseDC);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Restore);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetClipper);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetColorKey);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetOverlayPosition);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetPalette);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Unlock);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, UpdateOverlay);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, UpdateOverlayDisplay);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, UpdateOverlayZOrder);
	/*** Added in the v2 interface ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetDDInterface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, PageLock);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, PageUnlock);
	/*** Added in the v3 interface ***/

	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetSurfaceDesc);

	/*** Added in the v4 interface ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetPrivateData);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetPrivateData);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, FreePrivateData);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetUniquenessValue);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, ChangeUniquenessValue);
	/*** Moved Texture7 methods here ***/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetPriority);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetPriority);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetLOD);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetLOD);
	
	return	TRUE;
}
/*** IUnknown methods ***/
IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, QueryInterface, (REFIID riid, void** ppvObj), (this, riid, ppvObj));
IMPLEMENT_MEMBER_FUNCTION(ULONG, IDirectDrawSurface7, AddRef, (void), (this));
IMPLEMENT_MEMBER_FUNCTION(ULONG, IDirectDrawSurface7, Release, (void), (this));

/*** IDirectDrawSurface methods ***/
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, AddAttachedSurface,(LPDIRECTDRAWSURFACE7 a),(this,a));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, AddAttachedSurface);
HRESULT IGA_IDirectDrawSurface7::AddAttachedSurface(LPDIRECTDRAWSURFACE7 a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::AddAttachedSurface()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnAddAttachedSurface(this, a);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, AddOverlayDirtyRect, (LPRECT a), (this, a));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, AddOverlayDirtyRect);
HRESULT IGA_IDirectDrawSurface7::AddOverlayDirtyRect(LPRECT rect)
{
	OutputDebugString("IGA_IDirectDrawSurface7::AddOverlayDirtyRect()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnAddOverlayDirtyRect(this, rect);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, Blt, (LPRECT a, LPDIRECTDRAWSURFACE7 b, LPRECT c, DWORD d, LPDDBLTFX e), (this, a, b, c, d, e));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, Blt);
HRESULT IGA_IDirectDrawSurface7::Blt(LPRECT a, LPDIRECTDRAWSURFACE7 b, LPRECT c, DWORD d, LPDDBLTFX e)
{
	OutputDebugString("IGA_IDirectDrawSurface7::Blt()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnBlt(this, a, b, c, d, e);
	//static int i = 0;
	//CHAR chFileName[MAX_PATH];
	//CHAR chOutputString[255];
	//DDPIXELFORMAT pixelfmt;
	//if (gl_pIDirectDraw7 == NULL || gl_pIDirectDraw7->m_pIGADraw == NULL)
	//{
	//	return hr;
	//}
	//IDirectDrawSurface7* pSurface = (IDirectDrawSurface7*)b;
	//sprintf(chOutputString, "Blt: Surface address is %p", pSurface);
	//OutputDebugString(chOutputString);

	//if (pSurface != NULL)
	//{
	//	memset(&pixelfmt, 0, sizeof(DDPIXELFORMAT));
	//	pixelfmt.dwSize = sizeof(DDPIXELFORMAT);
	//	hr = IGA_IDirectDrawSurface7::m_pfnGetPixelFormat(pSurface, &pixelfmt);

	//	DDSURFACEDESC2       ddsd;   
	//	memset(&ddsd, 0, sizeof(ddsd));
	//	ddsd.dwSize = sizeof(ddsd);

	//	/*pSurface->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	//	int lpitch16 = (int)(ddsd.lPitch >>1);
	//	USHORT * video_buffer = (USHORT *)ddsd.lpSurface;
	//	for (int index = 0; index < 1000; index++)
	//	{
	//		int red = rand()%256;
	//		int green = rand()%256;
	//		int blue = rand()%256;
	//		int x = rand()/640;
	//		int y = rand()/480;
	//		USHORT pixel = (blue & 31)+((green & 31)<<5)+((red & 31)<< 10);
	//		video_buffer[x + y * lpitch16] = pixel;
	//	}

	//	pSurface->Unlock(NULL);*/

	//	//DDBLTFX ddbltfx; // this contains the DDBLTFX structure
	//	//memset(&ddbltfx, 0, sizeof(ddbltfx));
	//	//ddbltfx.dwSize = sizeof(ddbltfx);
	//	//ddbltfx.dwFillColor = 0x00ff00ff; 

	//	////flashSprite.Update();
	//	//RECT rect;
	//	//rect.left = 0;
	//	//rect.top = 0;
	//	//rect.right = 100;
	//	//rect.bottom = 100;
	//	//pSurface->Blt(&rect,     // ptr to dest rectangle
	//	//	NULL,       // ptr to source surface, NA            
	//	//	NULL,       // ptr to source rectangle, NA
	//	//	DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
	//	//	&ddbltfx);  // ptr to DDBLTFX structure
	//	////pSurface->Flip(NULL, DDFLIP_WAIT );

	//	if (SUCCEEDED(hr))
	//	{
	//		//if ( pixelfmt.dwRGBBitCount == 16)  //为了能够导出图片，去掉16位判断
	//		//{
	//			CExportSurface7::iterator p = g_mapExportSurface7.find(pSurface);
	//			if (p == g_mapExportSurface7.end())
	//			{
	//				sprintf(chFileName, "Z:\\Blt_SURFACE_%p.bmp", pSurface);
	//				gl_pIDirectDraw7->m_pIGADraw->SaveToBitmapFile(pSurface, chFileName);
	//				OutputDebugString(chFileName);
	//				i++;
	//				g_mapExportSurface7[pSurface] = TRUE;
	//			}
	//		//}
	//	}
	//}

	return hr;
}
//
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, BltBatch, (LPDDBLTBATCH a, DWORD b, DWORD c), (this, a, b, c));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, BltBatch);
HRESULT IGA_IDirectDrawSurface7::BltBatch(LPDDBLTBATCH lpblt, DWORD word1, DWORD word2)
{
	OutputDebugString("IGA_IDirectDrawSurface7::BltBatch()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnBltBatch(this, lpblt, word1, word2 );
	return hr;
}


//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, BltFast, (DWORD a, DWORD b, LPDIRECTDRAWSURFACE7 c, LPRECT d, DWORD e), (this, a, b, c, d, e));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, BltFast);
HRESULT IGA_IDirectDrawSurface7::BltFast(DWORD a, DWORD b, LPDIRECTDRAWSURFACE7 c, LPRECT d, DWORD e)
{
	OutputDebugString("IGA_IDirectDrawSurface7::BltFast()");
	HRESULT hr;
	hr = IGA_IDirectDrawSurface7::m_pfnBltFast(this, a, b, c, d, e);
	CHAR chFileName[MAX_PATH];
	CHAR chOutputString[255];

	DDSURFACEDESC2 desc;
	DDCOLORKEY clrKey;
	DDPIXELFORMAT pixelfmt;

	if (gl_pIDirectDraw7 == NULL || gl_pIDirectDraw7->m_pIGADraw == NULL)
		return hr;

	IDirectDrawSurface7* pSurface = (IDirectDrawSurface7*)c;
	if (pSurface != NULL)
	{
		memset(&pixelfmt, 0, sizeof(DDPIXELFORMAT));
		pixelfmt.dwSize = sizeof(DDPIXELFORMAT);
		hr = IGA_IDirectDrawSurface7::m_pfnGetPixelFormat(pSurface, &pixelfmt);

		if (SUCCEEDED(hr))
		{
			//if (pixelfmt.dwRGBBitCount == 16)
			//{
				//	CExportSurface::iterator p = g_mapExportSurface.find(pSurface);
				//	if (p == g_mapExportSurface.end())
				
				sprintf(chFileName, "Z:\\BltFast_SURFACE_%p.bmp", pSurface);
				gl_pIDirectDraw7->m_pIGADraw->SaveToBitmapFile(pSurface, chFileName);
				OutputDebugString(chFileName);
				g_mapExportSurface7[pSurface] = TRUE;
				
				//   锁定表面   
			/*	DDSURFACEDESC2 ddsd;   
				ddsd.dwSize = sizeof(ddsd);
				hr = IGA_IDirectDrawSurface7::m_pfnGetSurfaceDesc(pSurface, &ddsd);
				if (SUCCEEDED(hr))
				{
					if (ddsd.dwWidth == 800 && ddsd.dwHeight == 872)
					{
						sprintf(chOutputString, "ReplaceSurface called!");
						OutputDebugString(chOutputString);
						gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface, gl_pIDDraw->m_pIGADraw->m_hBitmap);
					}


					if (ddsd.dwWidth == 800 && ddsd.dwHeight == 1800)
					{
						sprintf(chOutputString, "ReplaceSurface called!");
						OutputDebugString(chOutputString);
						gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface, gl_pIDDraw->m_pIGADraw->m_hBitmapB1);
					}

					if (ddsd.dwWidth == 800 && ddsd.dwHeight == 1302)
					{
						sprintf(chOutputString, "ReplaceSurface called!");
						OutputDebugString(chOutputString);
						gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface, gl_pIDDraw->m_pIGADraw->m_hBitmapC1);
					}

					if (ddsd.dwWidth == 288 && ddsd.dwHeight == 600)
					{
						sprintf(chOutputString, "ReplaceSurface called!");
						OutputDebugString(chOutputString);
						gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface, gl_pIDDraw->m_pIGADraw->m_hBitmapD1);
					}

					if (ddsd.dwWidth == 800 && ddsd.dwHeight == 926)
					{
						sprintf(chOutputString, "ReplaceSurface called!");
						OutputDebugString(chOutputString);
						gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface, gl_pIDDraw->m_pIGADraw->m_hBitmapE1);
					}*/

					//////////////////////////////////////////////////////////////////////////
					//if (ddsd.dwWidth == 244 && ddsd.dwHeight == 123)
					//{
					//	sprintf(chOutputString, "ReplaceSurface called!");
					//	OutputDebugString(chOutputString);
					//	gl_pIDDraw->m_pIGADraw->ReplaceSurface(pSurface);
					//}
				//}
			//}
		}
	}
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, DeleteAttachedSurface, (DWORD a,LPDIRECTDRAWSURFACE7 b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, DeleteAttachedSurface);
HRESULT IGA_IDirectDrawSurface7::DeleteAttachedSurface(DWORD word,LPDIRECTDRAWSURFACE7 lpddsur)
{
	OutputDebugString("IGA_IDirectDrawSurface7::DeleteAttachedSurface()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnDeleteAttachedSurface(this, word, lpddsur);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, EnumAttachedSurfaces, (LPVOID a, LPDDENUMSURFACESCALLBACK7 b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, EnumAttachedSurfaces);
HRESULT IGA_IDirectDrawSurface7::EnumAttachedSurfaces(LPVOID a, LPDDENUMSURFACESCALLBACK7 b)
{
	OutputDebugString("IGA_IDirectDrawSurface7::EnumAttachedSurface()");
	LRESULT hr = IGA_IDirectDrawSurface7::m_pfnEnumAttachedSurfaces(this, a,b);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, EnumOverlayZOrders, (DWORD a, LPVOID b, LPDDENUMSURFACESCALLBACK7 c), (this, a, b, c));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, EnumOverlayZOrders);
HRESULT IGA_IDirectDrawSurface7::EnumOverlayZOrders(DWORD a,LPVOID b,LPDDENUMSURFACESCALLBACK7 c)
{
	OutputDebugString("IGA_IDirectDrawSurface::EnumOverlayZOrder()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnEnumOverlayZOrders(this, a,b,c);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, Flip, (LPDIRECTDRAWSURFACE7 a, DWORD b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, Flip);
HRESULT IGA_IDirectDrawSurface7::Flip(LPDIRECTDRAWSURFACE7 a, DWORD b)
{
	OutputDebugString("IGA_IDirectDrawSurface7::Flip()");
	static int i = 0;
	HRESULT hr = E_FAIL;
	CHAR chFileName[MAX_PATH];
	CHAR chOutputString[255];

	DDPIXELFORMAT pixelfmt;

	if (gl_pIDirectDraw7 == NULL || gl_pIDirectDraw7->m_pIGADraw == NULL)
		return hr;

	IDirectDrawSurface7* pSurface = (IDirectDrawSurface7*)a;
	if (pSurface != NULL)
	{
		memset(&pixelfmt, 0, sizeof(DDPIXELFORMAT));
		pixelfmt.dwSize = sizeof(DDPIXELFORMAT);
		hr = IGA_IDirectDrawSurface7::m_pfnGetPixelFormat(pSurface, &pixelfmt);

		if (SUCCEEDED(hr))
		{
			//if ( pixelfmt.dwRGBBitCount == 16)  //为了能够导出图片，去掉16位判断
			//{
				CExportSurface7::iterator p = g_mapExportSurface7.find(pSurface);
				if (p == g_mapExportSurface7.end())
				{
					sprintf(chFileName, "Z:\\Flip_Surface_%d.bmp",i );
					gl_pIDirectDraw7->m_pIGADraw->SaveToBitmapFile(pSurface, chFileName);
					OutputDebugString(chFileName);
					i++;
					g_mapExportSurface7[pSurface] = TRUE;
				}
			//}
		}
	}
	return hr;

}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetAttachedSurface, (LPDDSCAPS2 a, LPDIRECTDRAWSURFACE7 FAR * b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetAttachedSurface);
HRESULT IGA_IDirectDrawSurface7::GetAttachedSurface(LPDDSCAPS2 a, LPDIRECTDRAWSURFACE7 FAR * b)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetAttachedSurface()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetAttachedSurface(this, a, b);
	return hr;
}


//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetBltStatus, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetBltStatus);
HRESULT IGA_IDirectDrawSurface7::GetBltStatus(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetBltStatus()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetBltStatus(this, a);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetCaps, (LPDDSCAPS2 a), (this , a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetCaps);
HRESULT IGA_IDirectDrawSurface7::GetCaps(LPDDSCAPS2 lpdd)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetCaps()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetCaps(this, lpdd);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetClipper, (LPDIRECTDRAWCLIPPER FAR* a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetClipper)
HRESULT IGA_IDirectDrawSurface7::GetClipper(LPDIRECTDRAWCLIPPER FAR* lpddclip)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetClipper()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetClipper(this, lpddclip);
	return hr;
}

//只调用这个函数时，只能导出界面图来
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetColorKey, (DWORD a, LPDDCOLORKEY b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetColorKey);
HRESULT IGA_IDirectDrawSurface7::GetColorKey(DWORD word, LPDDCOLORKEY lpcolor)
{
	OutputDebugString("IGA_IDirectDrawSurface7()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetColorKey(this, word, lpcolor);
	return hr;
}


//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetDC, (HDC FAR * a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetDC);
HRESULT IGA_IDirectDrawSurface7::GetDC(HDC FAR * hDC)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetDC()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetDC(this, hDC);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetFlipStatus, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetFlipStatus);
HRESULT IGA_IDirectDrawSurface7::GetFlipStatus(DWORD word)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetFlipStatus()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetFlipStatus(this, word);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetOverlayPosition, (LPLONG a, LPLONG b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetOverlayPosition);
HRESULT IGA_IDirectDrawSurface7::GetOverlayPosition(LPLONG lpl, LPLONG lpl2)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetOverlayPosition()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetOverlayPosition(this, lpl,lpl2);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetPalette, (LPDIRECTDRAWPALETTE FAR* a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetPalette);
HRESULT IGA_IDirectDrawSurface7::GetPalette(LPDIRECTDRAWPALETTE FAR* lpddpal)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetPalette()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetPalette(this, lpddpal);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetPixelFormat, (LPDDPIXELFORMAT a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetPixelFormat);
HRESULT IGA_IDirectDrawSurface7::GetPixelFormat(LPDDPIXELFORMAT lpddformat)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetPixelFormat()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetPixelFormat(this, lpddformat);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetSurfaceDesc, (LPDDSURFACEDESC2 a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetSurfaceDesc);
HRESULT IGA_IDirectDrawSurface7::GetSurfaceDesc(LPDDSURFACEDESC2 a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetSurfaceDesc()");
	LRESULT hr=IGA_IDirectDrawSurface7::m_pfnGetSurfaceDesc(this, a);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, Initialize, (LPDIRECTDRAW a, LPDDSURFACEDESC2 b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, Initialize);
HRESULT IGA_IDirectDrawSurface7::Initialize(LPDIRECTDRAW lpddraw, LPDDSURFACEDESC2 lpddsd)
{
	OutputDebugString("IGA_IDirectDrawSurface7::Initialize()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnInitialize(this, lpddraw, lpddsd);
	return hr;
}
//能导出两张图片，但不是我们想要的
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, IsLost, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, IsLost);
HRESULT IGA_IDirectDrawSurface7::IsLost()
{
	OutputDebugString("IGA_IDirectDrawSurface7::IsLost()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnIsLost(this);
	return hr;
}

//调用这个函数是终止了程序
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, Lock, (LPRECT a,LPDDSURFACEDESC2 b,DWORD c,HANDLE d), (this, a, b, c, d));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, Lock);
HRESULT IGA_IDirectDrawSurface7::Lock(LPRECT rect,LPDDSURFACEDESC2 ddsd,DWORD word ,HANDLE h)
{
	OutputDebugString("IGA_IDirectDrawSurface7::Lock()");
	HRESULT hr = E_FAIL;
	hr = IGA_IDirectDrawSurface7::m_pfnLock(this, rect, ddsd, word, h);
	if ( NULL == g_pddsd2 )
	{
		g_pddsd2 = ddsd;
	}
	/*int lpitch16 = (int)(ddsd->lPitch >>1);
	USHORT * video_buffer = (USHORT *)ddsd->lpSurface;
	for (int index = 0; index < 1000; index++)
	{
		int red = rand()%256;
		int green = rand()%256;
		int blue = rand()%256;
		int x = rand()/640;
		int y = rand()/480;
		USHORT pixel = (blue & 31)+((green & 31)<<5)+((red & 31)<< 10);
		video_buffer[x + y * lpitch16] = pixel;
	}*/
	//pSurface->Unlock(NULL);

	//CHAR chFileName[MAX_PATH];
	//static int i = 0;
	//DDPIXELFORMAT pixelfmt;

	//if (gl_pIDirectDraw7 == NULL || gl_pIDirectDraw7->m_pIGADraw == NULL)
	//	return hr;

	//IDirectDrawSurface7* pSurface = (IDirectDrawSurface7*)this;
	//if (pSurface != NULL)
	//{
	//	memset(&pixelfmt, 0, sizeof(DDPIXELFORMAT));
	//	pixelfmt.dwSize = sizeof(DDPIXELFORMAT);
	//	hr = IGA_IDirectDrawSurface7::m_pfnGetPixelFormat(this, &pixelfmt);
	//	//g_kSurf.Draw(pSurface, 245, 170, 0, 0, 150, 140);

	//	//DDBLTFX ddbltfx; // this contains the DDBLTFX structure
	//	//memset(&ddbltfx, 0, sizeof(ddbltfx));
	//	//ddbltfx.dwSize = sizeof(ddbltfx);
	//	//ddbltfx.dwFillColor = 0x00ff00ff; 

	//	////flashSprite.Update();
	//	//RECT rect;
	//	//rect.left = 0;
	//	//rect.top = 0;
	//	//rect.right = 100;
	//	//rect.bottom = 100;
	//	//pSurface->Blt(&rect,     // ptr to dest rectangle
	//	//	NULL,       // ptr to source surface, NA            
	//	//	NULL,       // ptr to source rectangle, NA
	//	//	DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
	//	//	&ddbltfx);  // ptr to DDBLTFX structure
	//	////pSurface->Flip(NULL, DDFLIP_WAIT );

	//	if (SUCCEEDED(hr))
	//	{
	//		CExportSurface7::iterator p = g_mapExportSurface7.find(pSurface);
	//		if (p == g_mapExportSurface7.end())
	//		{
	//			sprintf(chFileName, "Z:\\Lock_SURFACE_%p.bmp",pSurface );
	//			gl_pIDirectDraw7->m_pIGADraw->SaveToBitmapFile(pSurface, chFileName);
	//			OutputDebugString(chFileName);
	//			i++;
	//			g_mapExportSurface7[pSurface] = TRUE;
	//		}
	//	}
	//}
	
	return hr;
}
//到此为之
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, ReleaseDC, (HDC a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, ReleaseDC);
HRESULT IGA_IDirectDrawSurface7::ReleaseDC(HDC hDC)
{
	OutputDebugString("IGA_IDirectDrawSurface7::ReleaseDC()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnReleaseDC(this, hDC);
	return hr;
}
//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, Restore, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, Restore);
HRESULT IGA_IDirectDrawSurface7::Restore()
{
	OutputDebugString("IGA_IDirectDrawSurface7::Restore()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnRestore(this );
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, SetClipper, (LPDIRECTDRAWCLIPPER a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, SetClipper);
HRESULT IGA_IDirectDrawSurface7::SetClipper(LPDIRECTDRAWCLIPPER lpddclip)
{
	OutputDebugString("IGA_IDirectDrawSurface7::SetClipper()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnSetClipper(this, lpddclip);
	return hr;
}

//只能导出一部分界面图
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, SetColorKey, (DWORD a, LPDDCOLORKEY b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, SetColorKey);
HRESULT IGA_IDirectDrawSurface7::SetColorKey(DWORD word, LPDDCOLORKEY lpddcolor)
{
	OutputDebugString("IGA_IDirectDrawSurface7::SetColorKey()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnSetColorKey(this, word, lpddcolor);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, SetOverlayPosition, (LONG a, LONG b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, SetOverlayPosition);
HRESULT IGA_IDirectDrawSurface7::SetOverlayPosition(LONG a, LONG b)
{
	OutputDebugString("IGA_IDirectDrawSurface7::SetOverlayPosition()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnSetOverlayPosition(this, a, b);
	return hr;
}

//这个函数导不出图来，没有作用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, SetPalette, (LPDIRECTDRAWPALETTE a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, SetPalette);
HRESULT IGA_IDirectDrawSurface7::SetPalette(LPDIRECTDRAWPALETTE lpddpal)
{
	OutputDebugString("IGA_IDirectDrawSurface7::SetPalette()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnSetPalette(this, lpddpal);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, Unlock, (LPRECT a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, Unlock);
HRESULT IGA_IDirectDrawSurface7::Unlock(LPRECT lpRect)
{	
	OutputDebugString("IGA_IDirectDrawSurface7::Unlock()");
	HRESULT hr = E_FAIL;
	CHAR chOutputString[255];
	//IDirectDrawSurface7* pSurface = (IDirectDrawSurface7*)this;
	//sprintf(chOutputString, "Unlock: Surface address is  %p", pSurface);
	//OutputDebugString(chOutputString);
	hr = IGA_IDirectDrawSurface7::m_pfnUnlock(this, lpRect);
	USHORT * video_buffer = (USHORT *)g_pddsd2->lpSurface;
	int lpitch16 = (int)(g_pddsd2->lPitch >>1);
	int dwWidth = g_pddsd2->dwWidth;
	int dwHeight = g_pddsd2->dwHeight;
	sprintf(chOutputString, "the addres g_ddsd: %p, Whidth: %d, Height :%d",g_pddsd2, dwWidth, dwHeight );
	OutputDebugString(chOutputString);

	//memset(video_buffer, 0xffffff,dwWidth/2*dwHeight/2);
	/*for (int index = 0; index < 100; index++)
	{
		int red = 0xff;
		int green = 0xff;
		int blue = 0xff;
		int x = rand()/400;
		int y = rand()/300;
		USHORT pixel = (blue & 31)+((green & 31)<<5)+((red & 31)<< 10);
		video_buffer[x + y * lpitch16] = pixel;
	}*/


	//
	//static int i = 0;
	//CHAR chFileName[MAX_PATH];
	//

	//DDSURFACEDESC2 desc;
	//DDCOLORKEY clrKey;
	//DDPIXELFORMAT pixelfmt;

	////hr = IGA_IDirectDrawSurface7::m_pfnUnlock(this, lpRect);
	//if (gl_pIDirectDraw7 == NULL || gl_pIDirectDraw7->m_pIGADraw == NULL)
	//	return hr;


	//if (pSurface != NULL)
	//{
	//	memset(&pixelfmt, 0, sizeof(DDPIXELFORMAT));
	//	pixelfmt.dwSize = sizeof(DDPIXELFORMAT);
	//	hr = IGA_IDirectDrawSurface7::m_pfnGetPixelFormat(this, &pixelfmt);

	//	DDBLTFX ddbltfx; // this contains the DDBLTFX structure
	//	memset(&ddbltfx, 0, sizeof(ddbltfx));
	//	ddbltfx.dwSize = sizeof(ddbltfx);
	//	ddbltfx.dwFillColor = 0x00ff00ff; 

	//	RECT rect;
	//	rect.left = 0;
	//	rect.top = 0;
	//	rect.right = 100;
	//	rect.bottom = 100;
	//	pSurface->Blt(&rect,     // ptr to dest rectangle
	//		NULL,       // ptr to source surface, NA            
	//		NULL,       // ptr to source rectangle, NA
	//		DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
	//		&ddbltfx);  // ptr to DDBLTFX structure
	//	////pSurface->Flip(NULL, DDFLIP_WAIT );

	//	if (SUCCEEDED(hr))
	//	{
	//		//if ( pixelfmt.dwRGBBitCount == 16)
	//		//{
	//			CExportSurface7::iterator p = g_mapExportSurface7.find(pSurface);
	//			if (p == g_mapExportSurface7.end())
	//			{
	//				sprintf(chFileName, "Z:\\Unlock_Surface_%p.bmp",pSurface);
	//				gl_pIDirectDraw7->m_pIGADraw->SaveToBitmapFile(pSurface, chFileName);
	//				OutputDebugString(chFileName);
	//				i++;
	//				g_mapExportSurface7[pSurface] = TRUE;
	//			}
	//		//}
	//	}
	//}
	
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, UpdateOverlay, (LPRECT a, LPDIRECTDRAWSURFACE7 b,LPRECT c, DWORD d, LPDDOVERLAYFX e), (this, a, b, c, d, e));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, UpdateOverlay)
HRESULT IGA_IDirectDrawSurface7::UpdateOverlay(LPRECT rect, LPDIRECTDRAWSURFACE7 lpddsur,LPRECT rect2,DWORD word, LPDDOVERLAYFX lpddol)
{
	OutputDebugString("IGA_IDirectDrawSurface7::UpdateOverlay()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnUpdateOverlay(this, rect, lpddsur, rect2,word, lpddol );
	return hr;
}

//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, UpdateOverlayDisplay, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, UpdateOverlayDisplay);
HRESULT IGA_IDirectDrawSurface7::UpdateOverlayDisplay(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::UpdateOverlayDisplay()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnUpdateOverlayDisplay(this, a);
	return hr;
}

//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, UpdateOverlayZOrder, (DWORD a, LPDIRECTDRAWSURFACE7 b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, UpdateOverlayZOrder)
HRESULT IGA_IDirectDrawSurface7::UpdateOverlayZOrder(DWORD word, LPDIRECTDRAWSURFACE7 lpddsur)
{
	OutputDebugString("IGA_IDirectDrawSurface7::UpdateOverlayZOrder()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnUpdateOverlayZOrder(this, word, lpddsur);
	return hr;
}

/*** Added in the v2 interface ***/
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetDDInterface, (LPVOID FAR * a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetDDInterface)
HRESULT IGA_IDirectDrawSurface7::GetDDInterface(LPVOID FAR * a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetDDInterface()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetDDInterface(this, a);
	return hr;
}

//没用调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, PageLock, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7,  PageLock)
HRESULT IGA_IDirectDrawSurface7::PageLock(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::PageLock()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnPageLock(this, a);
	return hr;
}
//环境变黑了，好像是关闭了灯光
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, PageUnlock, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, PageUnlock)
HRESULT IGA_IDirectDrawSurface7::PageUnlock(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::PageUnlock()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnPageUnlock(this, a);
	return hr;
}

/*** Added in the v3 interface ***/
//导不出图来
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, SetSurfaceDesc, (LPDDSURFACEDESC2 a, DWORD b), (this, a, b));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, SetSurfaceDesc);
HRESULT IGA_IDirectDrawSurface7::SetSurfaceDesc(LPDDSURFACEDESC2 a, DWORD b)
{
	OutputDebugString("IGA_IDirectDrawSurface7::SetSurfaceDesc()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnSetSurfaceDesc(this, a, b);
	CHAR chFileName[MAX_PATH];
	CHAR chOutputString[255];

	DDSURFACEDESC2 desc;
	DDCOLORKEY clrKey;
	DDPIXELFORMAT pixelfmt;	
	if (gl_pIDirectDraw7 == NULL || gl_pIDirectDraw7->m_pIGADraw == NULL)
		return hr;

	IDirectDrawSurface7* pSurface = (IDirectDrawSurface7*)this;
	if (pSurface != NULL)
	{
		memset(&pixelfmt, 0, sizeof(DDPIXELFORMAT));
		pixelfmt.dwSize = sizeof(DDPIXELFORMAT);
		hr = IGA_IDirectDrawSurface7::m_pfnGetPixelFormat(pSurface, &pixelfmt);

		if (SUCCEEDED(hr))
		{
			if ( pixelfmt.dwRGBBitCount == 16)
			{
				//	CExportSurface::iterator p = g_mapExportSurface.find(pSurface);
				//	if (p == g_mapExportSurface.end())
				{
					sprintf(chFileName, "Z:\\BltFast_SURFACE_%p.bmp", pSurface);
					gl_pIDirectDraw7->m_pIGADraw->SaveToBitmapFile(pSurface, chFileName);
					OutputDebugString(chFileName);
					g_mapExportSurface7[pSurface] = TRUE;
				}
			}
		}
	}
	return hr;
}
/*** Added in the v4 interface ***/
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, SetPrivateData, (REFGUID a, LPVOID b, DWORD c, DWORD d), (this, a, b, c, d));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, SetPrivateData)
HRESULT IGA_IDirectDrawSurface7::SetPrivateData(REFGUID a, LPVOID b, DWORD c, DWORD d)
{
	OutputDebugString("IGA_IDirectDrawSurface7::SetPrivateData()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnSetPrivateData(this, a, b, c, d);
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetPrivateData, (REFGUID a, LPVOID b, LPDWORD c), (this, a, b, c));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetPrivateData)
HRESULT IGA_IDirectDrawSurface7::GetPrivateData(REFGUID a, LPVOID b, LPDWORD c)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetPrivateData()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetPrivateData(this, a, b, c);
	return hr;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, FreePrivateData, (REFGUID a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, FreePrivateData)
HRESULT IGA_IDirectDrawSurface7::FreePrivateData(REFGUID a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::FreePrivateData()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnFreePrivateData(this, a);
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetUniquenessValue, (LPDWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetUniquenessValue)
HRESULT IGA_IDirectDrawSurface7::GetUniquenessValue(LPDWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetUniquenessValue()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetUniquenessValue(this, a);
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, ChangeUniquenessValue, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, ChangeUniquenessValue);
HRESULT IGA_IDirectDrawSurface7::ChangeUniquenessValue()
{
	OutputDebugString("IGA_IDirectDrawSurface7::ChangeUniquenessValue");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnChangeUniquenessValue(this);
	return hr;
}
/*** Moved Texture7 methods here ***/
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, SetPriority, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, SetPriority)
HRESULT IGA_IDirectDrawSurface7::SetPriority(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::SetPriority()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnSetPriority(this, a);
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetPriority, (LPDWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetPriority)
HRESULT IGA_IDirectDrawSurface7::GetPriority(LPDWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::GetPriority()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetPriority(this, a);
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, SetLOD, (DWORD a), (this, a));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, SetLOD)
HRESULT IGA_IDirectDrawSurface7::SetLOD(DWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::SetLOD()");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnSetLOD(this, a);
	return hr;
}
//没有调用
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectDrawSurface7, GetLOD, (LPDWORD a), (this, a));

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectDrawSurface7, GetLOD)
HRESULT IGA_IDirectDrawSurface7::GetLOD(LPDWORD a)
{
	OutputDebugString("IGA_IDirectDrawSurface7::SetLOD");
	HRESULT hr = IGA_IDirectDrawSurface7::m_pfnGetLOD(this, a );
	return hr;
}



