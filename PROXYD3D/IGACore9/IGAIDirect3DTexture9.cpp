#include "StdAfx.h"
#include "Common.h"
#include "IGACommon.h"
#include "IGAIDirect3DSurface9.h"
#include "IGAIDirect3DTexture9.h"
#include "IGACore.h"

#pragma warning(disable: 4409)
#pragma warning(disable: 4410)
#pragma warning(disable: 4312)

#define _ORGTHIS       IDirect3DTexture9* pThis

IMPLEMENT_DETOUR_CLASS(IDirect3DTexture9)

/*** IUnknown methods ***/

DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, QueryInterface, (_ORGTHIS, REFIID riid, void** ppvObj), 0);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, ULONG, AddRef, (_ORGTHIS), 1);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, ULONG, Release, (_ORGTHIS), 2);

/*** IDirect3DBaseTexture9 methods ***/

DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, GetDevice, (_ORGTHIS, IDirect3DDevice9** ppDevice), 3);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, SetPrivateData, (_ORGTHIS, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags), 4);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, GetPrivateData, (_ORGTHIS, REFGUID refguid,void* pData,DWORD* pSizeOfData), 5);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, FreePrivateData, (_ORGTHIS, REFGUID refguid), 6);

DECLARE_SOURCE_MEMBER(IDirect3DTexture9, DWORD, SetPriority, (_ORGTHIS, DWORD PriorityNew), 7);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, DWORD, GetPriority, (_ORGTHIS), 8);

DECLARE_SOURCE_MEMBER(IDirect3DTexture9, void, PreLoad, (_ORGTHIS), 9);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, D3DRESOURCETYPE, GetType, (_ORGTHIS), 10);

DECLARE_SOURCE_MEMBER(IDirect3DTexture9, DWORD, SetLOD, (_ORGTHIS, DWORD LODNew), 11);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, DWORD, GetLOD, (_ORGTHIS), 12);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, DWORD, GetLevelCount, (_ORGTHIS), 13);

DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, SetAutoGenFilterType, (_ORGTHIS, D3DTEXTUREFILTERTYPE FilterType), 14);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, D3DTEXTUREFILTERTYPE, GetAutoGenFilterType, (_ORGTHIS), 15);

DECLARE_SOURCE_MEMBER(IDirect3DTexture9, void, GenerateMipSubLevels, (_ORGTHIS), 16);

DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, GetLevelDesc, (_ORGTHIS, UINT Level,D3DSURFACE_DESC *pDesc), 17);

DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, GetSurfaceLevel, (_ORGTHIS, UINT Level,IDirect3DSurface9** ppSurfaceLevel), 18);

DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, LockRect, (_ORGTHIS, UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags), 19);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, UnlockRect, (_ORGTHIS, UINT Level), 20);
DECLARE_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, AddDirtyRect, (_ORGTHIS, CONST RECT* pDirtyRect), 21);

void InitIDirect3DTexture9Proxy(IDirect3DTexture9* pTexture)
{
	// ³õÊ¼»¯ VFT
	CopyVFT(GetIDirect3DTexture9SourceVFT(), DEFAULT_IGAVFT_SIZE, CPVO2PIVO(pTexture));
	ReplaceN_VFT(CPVO2PIVO(pTexture), CPVO2PIVO(GetIDirect3DTexture9Proxy()), 22);

	/*** IUnknown methods ***/

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, QueryInterface, (_ORGTHIS, REFIID riid, void** ppvObj), 0);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, ULONG, AddRef, (_ORGTHIS), 1);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, ULONG, Release, (_ORGTHIS), 2);

	/*** IDirect3DBaseTexture9 methods ***/

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, GetDevice, (_ORGTHIS, IDirect3DDevice9** ppDevice), 3);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, SetPrivateData, (_ORGTHIS, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags), 4);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, GetPrivateData, (_ORGTHIS, REFGUID refguid,void* pData,DWORD* pSizeOfData), 5);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, FreePrivateData, (_ORGTHIS, REFGUID refguid), 6);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, DWORD, SetPriority, (_ORGTHIS, DWORD PriorityNew), 7);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, DWORD, GetPriority, (_ORGTHIS), 8);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, void, PreLoad, (_ORGTHIS), 9);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, D3DRESOURCETYPE, GetType, (_ORGTHIS), 10);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, DWORD, SetLOD, (_ORGTHIS, DWORD LODNew), 11);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, DWORD, GetLOD, (_ORGTHIS), 12);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, DWORD, GetLevelCount, (_ORGTHIS), 13);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, SetAutoGenFilterType, (_ORGTHIS, D3DTEXTUREFILTERTYPE FilterType), 14);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, D3DTEXTUREFILTERTYPE, GetAutoGenFilterType, (_ORGTHIS), 15);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, void, GenerateMipSubLevels, (_ORGTHIS), 16);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, GetLevelDesc, (_ORGTHIS, UINT Level,D3DSURFACE_DESC *pDesc), 17);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, GetSurfaceLevel, (_ORGTHIS, UINT Level,IDirect3DSurface9** ppSurfaceLevel), 18);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, LockRect, (_ORGTHIS, UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags), 19);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, UnlockRect, (_ORGTHIS, UINT Level), 20);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture9, HRESULT, AddDirtyRect, (_ORGTHIS, CONST RECT* pDirtyRect), 21);
}

/*** IUnknown methods ***/

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, QueryInterface, (REFIID riid, void** ppvObj)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::QueryInterface");
#endif
	return lpvfnQueryInterface((IDirect3DTexture9*)this, riid, ppvObj);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, ULONG, AddRef, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::AddRef");
#endif
	return lpvfnAddRef((IDirect3DTexture9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, ULONG, Release, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::Release");
#endif

	ULONG lCount = lpvfnRelease((IDirect3DTexture9*)this);
	if (lCount == 0) {
		GetIGACore()->OnReleaseTexture((IDirect3DTexture9*)this);
	}
	return lCount;
}

/*** IDirect3DBaseTexture9 methods ***/

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, GetDevice, (IDirect3DDevice9** ppDevice)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::GetDevice");
#endif
	return lpvfnGetDevice((IDirect3DTexture9*)this, ppDevice);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, SetPrivateData, (REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::SetPrivateData");
#endif
	return lpvfnSetPrivateData((IDirect3DTexture9*)this, refguid, pData, SizeOfData, Flags);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, GetPrivateData, (REFGUID refguid,void* pData,DWORD* pSizeOfData)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::GetPrivateData");
#endif
	return lpvfnGetPrivateData((IDirect3DTexture9*)this, refguid, pData, pSizeOfData);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, FreePrivateData, (REFGUID refguid)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::FreePrivateData");
#endif
	return lpvfnFreePrivateData((IDirect3DTexture9*)this, refguid);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, DWORD, SetPriority, (DWORD PriorityNew)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::SetPriority");
#endif
	return lpvfnSetPriority((IDirect3DTexture9*)this, PriorityNew);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, DWORD, GetPriority, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::GetPriority");
#endif
	return lpvfnGetPriority((IDirect3DTexture9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, void, PreLoad, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::Preload");
#endif
	return lpvfnPreLoad((IDirect3DTexture9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, D3DRESOURCETYPE, GetType, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::GetType");
#endif
	return lpvfnGetType((IDirect3DTexture9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, DWORD, SetLOD, (DWORD LODNew)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::SetLOD");
#endif
	return lpvfnSetLOD((IDirect3DTexture9*)this, LODNew);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, DWORD, GetLOD, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::GetLOD");
#endif
	return lpvfnGetLOD((IDirect3DTexture9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, DWORD, GetLevelCount, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::GetLevelCount");
#endif

	return lpvfnGetLevelCount((IDirect3DTexture9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, SetAutoGenFilterType, (D3DTEXTUREFILTERTYPE FilterType)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::SetAutoGenFilterType");
#endif
	return lpvfnSetAutoGenFilterType((IDirect3DTexture9*)this, FilterType);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, D3DTEXTUREFILTERTYPE, GetAutoGenFilterType, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::GetAutoGenFilterType");
#endif
	return lpvfnGetAutoGenFilterType((IDirect3DTexture9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, void, GenerateMipSubLevels, (void)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::GenerateMipSubLevels");
#endif
	return lpvfnGenerateMipSubLevels((IDirect3DTexture9*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, GetLevelDesc, (UINT Level,D3DSURFACE_DESC *pDesc)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::GetLevelDesc");
#endif
	return lpvfnGetLevelDesc((IDirect3DTexture9*)this, Level, pDesc);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, GetSurfaceLevel, (UINT Level,IDirect3DSurface9** ppSurfaceLevel))
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::GetSurfaceLevel");
#endif
	static int g_nCallCount;

	HRESULT hr = lpvfnGetSurfaceLevel((IDirect3DTexture9*)this, Level, ppSurfaceLevel);
	if (SUCCEEDED(hr))
	{
		static BOOL s_Replaced = FALSE;
		if (!s_Replaced)
		{
			IDirect3DSurface9* pSurface = (*ppSurfaceLevel);
			InitIDirect3DSurface9Proxy(pSurface);
			s_Replaced = TRUE;
		}
		GetIGACore()->OnGetSurfaceLevel((IDirect3DTexture9*)this, Level, *ppSurfaceLevel);
	}
	return hr;
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, LockRect, (UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::LockRect");
#endif
	return lpvfnLockRect((IDirect3DTexture9*)this, Level, pLockedRect, pRect, Flags);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, UnlockRect, (UINT Level)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::UnLockRect");
#endif
	return lpvfnUnlockRect((IDirect3DTexture9*)this, Level);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture9, HRESULT, AddDirtyRect, (CONST RECT* pDirtyRect)) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IDirect3DTexture9::AddDirtyRect");
#endif
	return lpvfnAddDirtyRect((IDirect3DTexture9*)this, pDirtyRect);
}