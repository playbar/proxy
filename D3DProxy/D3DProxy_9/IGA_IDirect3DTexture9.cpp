#include "StdAfx.h"
#include "IGA_IDirect3DTexture9.h"

IGA_IDirect3DTexture9::IGA_IDirect3DTexture9(IDirect3DTexture9 * pIDirect3DTexture9)
{
	m_pIDirect3DTexture9 = pIDirect3DTexture9;
}

IGA_IDirect3DTexture9::~IGA_IDirect3DTexture9(void)
{
}

HRESULT IGA_IDirect3DTexture9::QueryInterface(REFIID riid, void** ppvObj)
{
	return m_pIDirect3DTexture9->QueryInterface(riid, ppvObj );
}

ULONG  IGA_IDirect3DTexture9::AddRef()
{
	return m_pIDirect3DTexture9->AddRef();
}

ULONG IGA_IDirect3DTexture9::Release()
{
	return m_pIDirect3DTexture9->Release();
}

HRESULT IGA_IDirect3DTexture9::GetDevice(IDirect3DDevice9 **ppDevice)
{
	return m_pIDirect3DTexture9->GetDevice(ppDevice);
}

HRESULT IGA_IDirect3DTexture9::SetPrivateData(REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags)
{
	return m_pIDirect3DTexture9->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT IGA_IDirect3DTexture9::GetPrivateData(const GUID &refguid, void *pData, DWORD *pSizeOfData)
{
	return m_pIDirect3DTexture9->GetPrivateData(refguid, pData, pSizeOfData );
}

HRESULT IGA_IDirect3DTexture9::FreePrivateData(const GUID &refguid)
{
	return m_pIDirect3DTexture9->FreePrivateData(refguid);
}

DWORD IGA_IDirect3DTexture9::SetPriority( DWORD PriorityNew)
{
	return m_pIDirect3DTexture9->SetPriority( PriorityNew );	
}

DWORD IGA_IDirect3DTexture9::GetPriority()
{
	return m_pIDirect3DTexture9->GetPriority();
}

void IGA_IDirect3DTexture9::PreLoad()
{
	return m_pIDirect3DTexture9->PreLoad();
}

D3DRESOURCETYPE IGA_IDirect3DTexture9::GetType()
{
	return m_pIDirect3DTexture9->GetType();
}

DWORD IGA_IDirect3DTexture9::SetLOD(DWORD LODNew)
{
	return m_pIDirect3DTexture9->SetLOD(LODNew);
}

DWORD IGA_IDirect3DTexture9::GetLOD()
{
	return m_pIDirect3DTexture9->GetLOD();
}

DWORD IGA_IDirect3DTexture9::GetLevelCount()
{
	return m_pIDirect3DTexture9->GetLevelCount();
}

HRESULT IGA_IDirect3DTexture9::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	return m_pIDirect3DTexture9->SetAutoGenFilterType(FilterType);
}

D3DTEXTUREFILTERTYPE IGA_IDirect3DTexture9::GetAutoGenFilterType()
{
	return m_pIDirect3DTexture9->GetAutoGenFilterType();
}

void IGA_IDirect3DTexture9::GenerateMipSubLevels()
{
	return m_pIDirect3DTexture9->GenerateMipSubLevels();
}

HRESULT IGA_IDirect3DTexture9::GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc)
{
	return m_pIDirect3DTexture9->GetLevelDesc( Level, pDesc );
}

HRESULT IGA_IDirect3DTexture9::GetSurfaceLevel(UINT Level, IDirect3DSurface9 **ppSurfaceLevel)
{
	return m_pIDirect3DTexture9->GetSurfaceLevel( Level, ppSurfaceLevel );
}

HRESULT IGA_IDirect3DTexture9::LockRect(UINT Level, D3DLOCKED_RECT *pLockedRect, const RECT *pRect, DWORD Flags)
{
	return LockRect(Level, pLockedRect, pRect, Flags);
}

HRESULT IGA_IDirect3DTexture9::UnlockRect(UINT Level)
{
	return m_pIDirect3DTexture9->UnlockRect( Level );
}

HRESULT IGA_IDirect3DTexture9::AddDirtyRect(const RECT *pDirtyRect)
{
	return m_pIDirect3DTexture9->AddDirtyRect( pDirtyRect );
}






