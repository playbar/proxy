#include "StdAfx.h"
#include "Common.h"
#include "IGACommon.h"
#include "IGAIDirect3DSurface8.h"
#include "IGAIDirect3DTexture8.h"
#include "IGACore.h"

#pragma warning(disable: 4409)
#pragma warning(disable: 4410)
#pragma warning(disable: 4312)

#define _ORGTHIS       IDirect3DTexture8* pThis

IMPLEMENT_DETOUR_CLASS(IDirect3DTexture8)

/*** IUnknown methods ***/

DECLARE_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, QueryInterface, (_ORGTHIS, REFIID riid, void** ppvObj), 0);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, ULONG, AddRef, (_ORGTHIS), 1);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, ULONG, Release, (_ORGTHIS), 2);

/*** IDirect3DBaseTexture8 methods ***/

DECLARE_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, GetDevice, (_ORGTHIS, IDirect3DDevice8** ppDevice), 3);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, SetPrivateData, (_ORGTHIS, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags), 4);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, GetPrivateData, (_ORGTHIS, REFGUID refguid,void* pData,DWORD* pSizeOfData), 5);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, FreePrivateData, (_ORGTHIS, REFGUID refguid), 6);

DECLARE_SOURCE_MEMBER(IDirect3DTexture8, DWORD, SetPriority, (_ORGTHIS, DWORD PriorityNew), 7);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, DWORD, GetPriority, (_ORGTHIS), 8);

DECLARE_SOURCE_MEMBER(IDirect3DTexture8, void, PreLoad, (_ORGTHIS), 9);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, D3DRESOURCETYPE, GetType, (_ORGTHIS), 10);

DECLARE_SOURCE_MEMBER(IDirect3DTexture8, DWORD, SetLOD, (_ORGTHIS, DWORD LODNew), 11);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, DWORD, GetLOD, (_ORGTHIS), 12);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, DWORD, GetLevelCount, (_ORGTHIS), 13);

DECLARE_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, GetLevelDesc, (_ORGTHIS, UINT Level,D3DSURFACE_DESC *pDesc), 14);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, GetSurfaceLevel, (_ORGTHIS, UINT Level,IDirect3DSurface8** ppSurfaceLevel), 15);

DECLARE_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, LockRect, (_ORGTHIS, UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags), 16);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, UnlockRect, (_ORGTHIS, UINT Level), 17);
DECLARE_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, AddDirtyRect, (_ORGTHIS, CONST RECT* pDirtyRect), 18);

void InitIDirect3DTexture8Proxy(IDirect3DTexture8* pTexture)
{
	// ³õÊ¼»¯ VFT
	CopyVFT(GetIDirect3DTexture8SourceVFT(), DEFAULT_IGAVFT_SIZE, CPVO2PIVO(pTexture));
	ReplaceN_VFT(CPVO2PIVO(pTexture), CPVO2PIVO(GetIDirect3DTexture8Proxy()), 19);

	/*** IUnknown methods ***/

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, QueryInterface, (_ORGTHIS, REFIID riid, void** ppvObj), 0);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, ULONG, AddRef, (_ORGTHIS), 1);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, ULONG, Release, (_ORGTHIS), 2);

	/*** IDirect3DBaseTexture8 methods ***/

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, GetDevice, (_ORGTHIS, IDirect3DDevice8** ppDevice), 3);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, SetPrivateData, (_ORGTHIS, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags), 4);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, GetPrivateData, (_ORGTHIS, REFGUID refguid,void* pData,DWORD* pSizeOfData), 5);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, FreePrivateData, (_ORGTHIS, REFGUID refguid), 6);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, DWORD, SetPriority, (_ORGTHIS, DWORD PriorityNew), 7);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, DWORD, GetPriority, (_ORGTHIS), 8);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, void, PreLoad, (_ORGTHIS), 9);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, D3DRESOURCETYPE, GetType, (_ORGTHIS), 10);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, DWORD, SetLOD, (_ORGTHIS, DWORD LODNew), 11);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, DWORD, GetLOD, (_ORGTHIS), 12);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, DWORD, GetLevelCount, (_ORGTHIS), 13);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, GetLevelDesc, (_ORGTHIS, UINT Level,D3DSURFACE_DESC *pDesc), 14);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, GetSurfaceLevel, (_ORGTHIS, UINT Level,IDirect3DSurface8** ppSurfaceLevel), 15);

	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, LockRect, (_ORGTHIS, UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags), 16);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, UnlockRect, (_ORGTHIS, UINT Level), 17);
	IMPLEMENT_SOURCE_MEMBER(IDirect3DTexture8, HRESULT, AddDirtyRect, (_ORGTHIS, CONST RECT* pDirtyRect), 18);
}

/*** IUnknown methods ***/

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, HRESULT, QueryInterface, (REFIID riid, void** ppvObj)) {
	return lpvfnQueryInterface((IDirect3DTexture8*)this, riid, ppvObj);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, ULONG, AddRef, (void)) {
	return lpvfnAddRef((IDirect3DTexture8*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, ULONG, Release, (void)) {

	ULONG lCount = lpvfnRelease((IDirect3DTexture8*)this);
	if (lCount == 0) {
		GetIGACore()->OnReleaseTexture((IDirect3DTexture8*)this);
	}
	return lCount;
}

/*** IDirect3DBaseTexture8 methods ***/

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, HRESULT, GetDevice, (IDirect3DDevice8** ppDevice)) {
	return lpvfnGetDevice((IDirect3DTexture8*)this, ppDevice);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, HRESULT, SetPrivateData, (REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags)) {
	return lpvfnSetPrivateData((IDirect3DTexture8*)this, refguid, pData, SizeOfData, Flags);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, HRESULT, GetPrivateData, (REFGUID refguid,void* pData,DWORD* pSizeOfData)) {
	return lpvfnGetPrivateData((IDirect3DTexture8*)this, refguid, pData, pSizeOfData);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, HRESULT, FreePrivateData, (REFGUID refguid)) {
	return lpvfnFreePrivateData((IDirect3DTexture8*)this, refguid);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, DWORD, SetPriority, (DWORD PriorityNew)) {
	return lpvfnSetPriority((IDirect3DTexture8*)this, PriorityNew);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, DWORD, GetPriority, (void)) {
	return lpvfnGetPriority((IDirect3DTexture8*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, void, PreLoad, (void)) {
	return lpvfnPreLoad((IDirect3DTexture8*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, D3DRESOURCETYPE, GetType, (void)) {
	return lpvfnGetType((IDirect3DTexture8*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, DWORD, SetLOD, (DWORD LODNew)) {
	return lpvfnSetLOD((IDirect3DTexture8*)this, LODNew);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, DWORD, GetLOD, (void)) {
	return lpvfnGetLOD((IDirect3DTexture8*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, DWORD, GetLevelCount, (void)) {
	return lpvfnGetLevelCount((IDirect3DTexture8*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, HRESULT, GetLevelDesc, (UINT Level,D3DSURFACE_DESC *pDesc)) {
	return lpvfnGetLevelDesc((IDirect3DTexture8*)this, Level, pDesc);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, HRESULT, GetSurfaceLevel, (UINT Level,IDirect3DSurface8** ppSurfaceLevel))
{
	static int g_nCallCount;

	HRESULT hr = lpvfnGetSurfaceLevel((IDirect3DTexture8*)this, Level, ppSurfaceLevel);
	if (SUCCEEDED(hr))
	{
		static BOOL s_Replaced = FALSE;
		if (!s_Replaced)
		{
			IDirect3DSurface8* pSurface = (*ppSurfaceLevel);
			InitIDirect3DSurface8Proxy(pSurface);
			s_Replaced = TRUE;
		}
		GetIGACore()->OnGetSurfaceLevel((IDirect3DTexture8*)this, Level, *ppSurfaceLevel);
	}
	return hr;
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, HRESULT, LockRect, (UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)) {
	return lpvfnLockRect((IDirect3DTexture8*)this, Level, pLockedRect, pRect, Flags);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, HRESULT, UnlockRect, (UINT Level)) {
	return lpvfnUnlockRect((IDirect3DTexture8*)this, Level);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DTexture8, HRESULT, AddDirtyRect, (CONST RECT* pDirtyRect)) {
	return lpvfnAddDirtyRect((IDirect3DTexture8*)this, pDirtyRect);
}