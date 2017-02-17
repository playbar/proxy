#include "stdafx.h"
#include "Common.h"
#include "IGACommon.h"

#include "IGAIDirect3DSurface8.h"
#include "IGA8Core.h"

#include <process.h>

#pragma warning(disable: 4409)
#pragma warning(disable: 4410)
#pragma warning(disable: 4312)


#define _ORGTHIS       IDirect3DSurface8* pThis

IMPLEMENT_DETOUR_CLASS(IDirect3DSurface8)

/*** IUnknown methods ***/

DECLARE_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, QueryInterface, (_ORGTHIS, REFIID riid, void** ppvObj), 0)
DECLARE_SOURCE_MEMBER(IDirect3DSurface8, ULONG, AddRef, (_ORGTHIS), 1)
DECLARE_SOURCE_MEMBER(IDirect3DSurface8, ULONG, Release, (_ORGTHIS), 2)

/*** IDirect3DResource8 methods ***/
DECLARE_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, GetDevice, (_ORGTHIS, IDirect3DDevice8** ppDevice), 3)
DECLARE_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, SetPrivateData, (_ORGTHIS, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags), 4)
DECLARE_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, GetPrivateData, (_ORGTHIS, REFGUID refguid,void* pData,DWORD* pSizeOfData), 5)
DECLARE_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, FreePrivateData, (_ORGTHIS, REFGUID refguid), 6)

DECLARE_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, GetContainer,(_ORGTHIS, REFIID riid,void** ppContainer), 7)
DECLARE_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, GetDesc,(_ORGTHIS, D3DSURFACE_DESC *pDesc), 8)

DECLARE_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, LockRect,(_ORGTHIS, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags), 9)
DECLARE_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, UnlockRect,(_ORGTHIS), 10)

void InitIDirect3DSurface8Proxy(IDirect3DSurface8* pSurface)
{
	// ³õÊ¼»¯ VFT
	CopyVFT(GetIDirect3DSurface8SourceVFT(), DEFAULT_IGAVFT_SIZE, CPVO2PIVO(pSurface));
	ReplaceN_VFT(CPVO2PIVO(pSurface), CPVO2PIVO(GetIDirect3DSurface8Proxy()), 11);

	/*** IUnknown methods ***/

	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, QueryInterface, (_ORGTHIS, REFIID riid, void** ppvObj), 0)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, ULONG, AddRef, (_ORGTHIS), 1)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, ULONG, Release, (_ORGTHIS), 2)

	/*** IDirect3DResource8 methods ***/
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, GetDevice, (_ORGTHIS, IDirect3DDevice8** ppDevice), 3)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, SetPrivateData, (_ORGTHIS, REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags), 4)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, GetPrivateData, (_ORGTHIS, REFGUID refguid,void* pData,DWORD* pSizeOfData), 5)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, FreePrivateData, (_ORGTHIS, REFGUID refguid), 6)

	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, GetContainer,(_ORGTHIS, REFIID riid,void** ppContainer), 7)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, GetDesc,(_ORGTHIS, D3DSURFACE_DESC *pDesc), 8)

	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, LockRect,(_ORGTHIS, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags), 9)
	IMPLEMENT_SOURCE_MEMBER(IDirect3DSurface8, HRESULT, UnlockRect,(_ORGTHIS), 10)
}

/*** IUnknown methods ***/

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, HRESULT, QueryInterface, (REFIID riid, void** ppvObj)) {
	return lpvfnQueryInterface((IDirect3DSurface8*)this, riid, ppvObj);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, ULONG, AddRef, (void)) {
	return lpvfnAddRef((IDirect3DSurface8*)this);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, ULONG, Release, (void)) {
	return lpvfnRelease((IDirect3DSurface8*)this);
}

/*** IDirect3DResource8 methods ***/

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, HRESULT, GetDevice, (IDirect3DDevice8** ppDevice)) {
	return lpvfnGetDevice((IDirect3DSurface8*)this, ppDevice);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, HRESULT, SetPrivateData, (REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags)) {
	return lpvfnSetPrivateData((IDirect3DSurface8*)this, refguid, pData, SizeOfData, Flags);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, HRESULT, GetPrivateData, (REFGUID refguid,void* pData,DWORD* pSizeOfData)) {
	return lpvfnGetPrivateData((IDirect3DSurface8*)this, refguid, pData, pSizeOfData);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, HRESULT, FreePrivateData, (REFGUID refguid)) {
	return lpvfnFreePrivateData((IDirect3DSurface8*)this, refguid);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, HRESULT, GetContainer,(REFIID riid,void** ppContainer)) {
	return lpvfnGetContainer((IDirect3DSurface8*)this, riid, ppContainer);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, HRESULT, GetDesc,(D3DSURFACE_DESC *pDesc)) {
	return lpvfnGetDesc((IDirect3DSurface8*)this, pDesc);
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, HRESULT, LockRect,(D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags))
{
	HRESULT hr = lpvfnLockRect((IDirect3DSurface8*)this, pLockedRect, pRect, Flags);
	if (SUCCEEDED(hr)) {
		GetIGACore()->OnSurfaceLockRect((IDirect3DSurface8*)this, pLockedRect, pRect, Flags);
	}
	return hr;
}

IMPLEMENT_DETOUR_MEMBER(IDirect3DSurface8, HRESULT, UnlockRect,(void))
{
	HRESULT hr = lpvfnUnlockRect((IDirect3DSurface8*)this);
	if (SUCCEEDED(hr)) {
		GetIGACore()->OnSurfaceUnlockRect((IDirect3DSurface8*)this);
	}
	return hr;
}
