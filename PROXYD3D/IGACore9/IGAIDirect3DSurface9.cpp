#include "stdafx.h"
#include "Common.h"
#include "IGACommon.h"

#include "IGAIDirect3DSurface9.h"
#include "IGACore.h"

#include <process.h>

#pragma warning(disable: 4409)
#pragma warning(disable: 4410)
#pragma warning(disable: 4312)


#define _ORGTHIS       IDirect3DSurface9* pThis

IMPLEMENT_DETOUR_CLASS(IDirect3DSurface9)

/*** IUnknown methods ***/

DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, QueryInterface, (_ORGTHIS, REFIID riid, void** ppvObj), 0)
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, ULONG, AddRef, (_ORGTHIS), 1)
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, ULONG, Release, (_ORGTHIS), 2)

/*** IDirect3DResource9 methods ***/
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, GetDevice, (_ORGTHIS, IDirect3DDevice9** ppDevice), 3)
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, SetPrivateData, (_ORGTHIS, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags), 4)
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, GetPrivateData, (_ORGTHIS, REFGUID refguid,void* pData,DWORD* pSizeOfData), 5)
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, FreePrivateData, (_ORGTHIS, REFGUID refguid), 6)

DECLARE_SOURCE_MEMBER(IDirect3DSurface9, DWORD, SetPriority, (_ORGTHIS, DWORD PriorityNew), 7)
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, DWORD, GetPriority, (_ORGTHIS), 8)

DECLARE_SOURCE_MEMBER(IDirect3DSurface9, void, PreLoad, (_ORGTHIS), 9)
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, D3DRESOURCETYPE, GetType, (_ORGTHIS), 10)

DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, GetContainer,(_ORGTHIS, REFIID riid,void** ppContainer), 11)
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, GetDesc,(_ORGTHIS, D3DSURFACE_DESC *pDesc), 12)

DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, LockRect,(_ORGTHIS, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags), 13)
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, UnlockRect,(_ORGTHIS), 14)

DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, GetDC,(_ORGTHIS, HDC *phdc), 15)
DECLARE_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, ReleaseDC,(_ORGTHIS, HDC hdc), 16)

void InitIDirect3DSurface9Proxy(IDirect3DSurface9* pSurface)
{
	// ³õÊ¼»¯ VFT
	CopyVFT(GetIDirect3DSurface9SourceVFT(), DEFAULT_IGAVFT_SIZE, CPVO2PIVO(pSurface));
//	ReplaceVFT(CPVO2PIVO(pSurface), CPVO2PIVO(GetIDirect3DSurface9Proxy()));
	ReplaceN_VFT(CPVO2PIVO(pSurface), CPVO2PIVO(GetIDirect3DSurface9Proxy()), 17);

	/*** IUnknown methods ***/

	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, QueryInterface, (_ORGTHIS, REFIID riid, void** ppvObj), 0)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, ULONG, AddRef, (_ORGTHIS), 1)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, ULONG, Release, (_ORGTHIS), 2)

	/*** IDirect3DResource9 methods ***/
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, GetDevice, (_ORGTHIS, IDirect3DDevice9** ppDevice), 3)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, SetPrivateData, (_ORGTHIS, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags), 4)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, GetPrivateData, (_ORGTHIS, REFGUID refguid,void* pData,DWORD* pSizeOfData), 5)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, FreePrivateData, (_ORGTHIS, REFGUID refguid), 6)

	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, DWORD, SetPriority, (_ORGTHIS, DWORD PriorityNew), 7)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, DWORD, GetPriority, (_ORGTHIS), 8)

	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, void, PreLoad, (_ORGTHIS), 9)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, D3DRESOURCETYPE, GetType, (_ORGTHIS), 10)

	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, GetContainer,(_ORGTHIS, REFIID riid,void** ppContainer), 11)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, GetDesc,(_ORGTHIS, D3DSURFACE_DESC *pDesc), 12)

	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, LockRect,(_ORGTHIS, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags), 13)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, UnlockRect,(_ORGTHIS), 14)

	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, GetDC,(_ORGTHIS, HDC *phdc), 15)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface9, HRESULT, ReleaseDC,(_ORGTHIS, HDC hdc), 16)
}

// handler
void __stdcall OnUnlockRect(IDirect3DSurface9* pTexture);

/*** IUnknown methods ***/

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, QueryInterface, (REFIID riid, void** ppvObj)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::QueryInterface");
#endif
	return lpvfnQueryInterface((IDirect3DSurface9*)this, riid, ppvObj);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, ULONG, AddRef, (void))
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::AddRef");
#endif
	return lpvfnAddRef((IDirect3DSurface9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, ULONG, Release, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::Release");
#endif
	return lpvfnRelease((IDirect3DSurface9*)this);
}

/*** IDirect3DResource9 methods ***/

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, GetDevice, (IDirect3DDevice9** ppDevice))
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::GetDevice");
#endif
	return lpvfnGetDevice((IDirect3DSurface9*)this, ppDevice);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, SetPrivateData, (REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::SetPrivateData");
#endif
	return lpvfnSetPrivateData((IDirect3DSurface9*)this, refguid, pData, SizeOfData, Flags);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, GetPrivateData, (REFGUID refguid,void* pData,DWORD* pSizeOfData)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::GetPrivateData");
#endif
	return lpvfnGetPrivateData((IDirect3DSurface9*)this, refguid, pData, pSizeOfData);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, FreePrivateData, (REFGUID refguid)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::FreePrivateData");
#endif
	return lpvfnFreePrivateData((IDirect3DSurface9*)this, refguid);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, DWORD, SetPriority, (DWORD PriorityNew)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::SetPriority");
#endif
	return lpvfnSetPriority((IDirect3DSurface9*)this, PriorityNew);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, DWORD, GetPriority, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::GetPriority");
#endif
	return lpvfnGetPriority((IDirect3DSurface9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, void, PreLoad, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::PreLoad");
#endif
	return lpvfnPreLoad((IDirect3DSurface9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, D3DRESOURCETYPE, GetType, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::GetType");
#endif
	return lpvfnGetType((IDirect3DSurface9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, GetContainer,(REFIID riid,void** ppContainer)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::GetContainer");
#endif
	return lpvfnGetContainer((IDirect3DSurface9*)this, riid, ppContainer);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, GetDesc,(D3DSURFACE_DESC *pDesc)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::GetDesc");
#endif
	return lpvfnGetDesc((IDirect3DSurface9*)this, pDesc);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, LockRect,(D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags))
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::LockRect");
#endif
	HRESULT hr = lpvfnLockRect((IDirect3DSurface9*)this, pLockedRect, pRect, Flags);
	if (SUCCEEDED(hr)) {
		GetIGACore()->OnSurfaceLockRect((IDirect3DSurface9*)this, pLockedRect, pRect, Flags);
	}
	return hr;
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, UnlockRect,(void))
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::UnlockRect");
#endif
	HRESULT hr = lpvfnUnlockRect((IDirect3DSurface9*)this);
	if (SUCCEEDED(hr)) {
		GetIGACore()->OnSurfaceUnlockRect((IDirect3DSurface9*)this);
	}
	return hr;
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, GetDC,(HDC *phdc)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::GetDC");
#endif
	return lpvfnGetDC((IDirect3DSurface9*)this, phdc);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface9, HRESULT, ReleaseDC,(HDC hdc)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DSurface9::ReleaseDC");
#endif
	return lpvfnReleaseDC((IDirect3DSurface9*)this, hdc);
}
