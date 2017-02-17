#include "StdAfx.h"
#include "IGA_IDirect3DSurface9.h"

IGA_IDirect3DSurface9::IGA_IDirect3DSurface9(IDirect3DSurface9 * pIDirect3DSurface9, IDirect3DDevice9 *pIDirect3DDevice9 )
{
	m_pIDirect3DSurface9= pIDirect3DSurface9;
	m_pIDirect3DDevice9	= pIDirect3DDevice9;
}

IGA_IDirect3DSurface9::~IGA_IDirect3DSurface9(void)
{
}

HRESULT IGA_IDirect3DSurface9::QueryInterface(REFIID riid, void** ppvObj)
{
	return m_pIDirect3DSurface9->QueryInterface(riid, ppvObj);
}

ULONG IGA_IDirect3DSurface9::AddRef()
{
	return m_pIDirect3DSurface9->AddRef();
}

ULONG IGA_IDirect3DSurface9::Release()
{
	return m_pIDirect3DSurface9->Release();
}

HRESULT IGA_IDirect3DSurface9::GetDevice(IDirect3DDevice9 **ppDevice)
{
	return m_pIDirect3DSurface9->GetDevice(ppDevice);
}

HRESULT IGA_IDirect3DSurface9::SetPrivateData(const GUID &refguid, const void *pData, DWORD SizeOfData, DWORD Flags)
{
	return m_pIDirect3DSurface9->SetPrivateData(refguid, pData, SizeOfData, Flags );
}

HRESULT IGA_IDirect3DSurface9::GetPrivateData(const GUID &refguid, void *pData, DWORD *pSizeOfData)
{
	return m_pIDirect3DSurface9->GetPrivateData(refguid, pData, pSizeOfData );
}

HRESULT IGA_IDirect3DSurface9::FreePrivateData(const GUID &refguid)
{
	return m_pIDirect3DSurface9->FreePrivateData( refguid );
}

DWORD IGA_IDirect3DSurface9::SetPriority(DWORD PriorityNew)
{
	return m_pIDirect3DSurface9->SetPriority( PriorityNew );
}

DWORD IGA_IDirect3DSurface9::GetPriority()
{
	return m_pIDirect3DSurface9->GetPriority();
}

void IGA_IDirect3DSurface9::PreLoad()
{
	return m_pIDirect3DSurface9->PreLoad();
}

D3DRESOURCETYPE IGA_IDirect3DSurface9::GetType()
{
	return m_pIDirect3DSurface9->GetType();
}

HRESULT IGA_IDirect3DSurface9::GetContainer(const IID &riid, void **ppContainer)
{
	return m_pIDirect3DSurface9->GetContainer( riid, ppContainer );
}

HRESULT IGA_IDirect3DSurface9::GetDesc(D3DSURFACE_DESC *pDesc)
{
	return m_pIDirect3DSurface9->GetDesc(pDesc);
}

HRESULT IGA_IDirect3DSurface9::LockRect(D3DLOCKED_RECT *pLockedRect, const RECT *pRect, DWORD Flags)
{
	return m_pIDirect3DSurface9->LockRect( pLockedRect, pRect, Flags );
}

HRESULT IGA_IDirect3DSurface9::UnlockRect()
{
	return m_pIDirect3DSurface9->UnlockRect();
}

HRESULT IGA_IDirect3DSurface9::GetDC(HDC *phdc)
{
	return m_pIDirect3DSurface9->GetDC( phdc );
}

HRESULT IGA_IDirect3DSurface9::ReleaseDC(HDC hdc)
{
	return m_pIDirect3DSurface9->ReleaseDC( hdc );
}




