#pragma once

DECLARE_DETOUR_CLASS(IDirect3DSurface8)
{
	/*** IUnknown methods ***/

	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, QueryInterface, (REFIID riid, void** ppvObj));
	DECLARE_DETOUR_MEMBER_V_STDCALL(ULONG, AddRef, (void));
	DECLARE_DETOUR_MEMBER_V_STDCALL(ULONG, Release, (void));

    /*** IDirect3DResource8 methods ***/
    DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, GetDevice, (IDirect3DDevice8** ppDevice));
    DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, SetPrivateData, (REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags));
    DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, GetPrivateData, (REFGUID refguid,void* pData,DWORD* pSizeOfData));
    DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, FreePrivateData, (REFGUID refguid));
    DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, GetContainer,(REFIID riid,void** ppContainer));
    DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, GetDesc,(D3DSURFACE_DESC *pDesc));
    DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, LockRect,(D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags));
	DECLARE_DETOUR_MEMBER_V_STDCALL(HRESULT, UnlockRect,(void));
};

void InitIDirect3DSurface8Proxy(IDirect3DSurface8* pSurface);
