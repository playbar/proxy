#ifndef _IGA_COREIMPL_H_
#define _IGA_COREIMPL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// IGACoreImpl.h : header file
//


// 比较材质
BOOL CompareTexture(DWORD dwLevel, IDirect3DTexture8* pTarget, DWORD dwMatchLevel, IDirect3DTexture8* pMatchTexture);
// 替换材质
BOOL ReplaceTexture(DWORD dwLevel, IDirect3DTexture8* pTarget, DWORD dwAdsLevel, IDirect3DTexture8* pAdsTexture);
// 替换材质（所有层）
BOOL ReplaceTextureAllLevel(IDirect3DTexture8* pTarget, IDirect3DTexture8* pAdsTexture);

class CIGACoreImpl : public IIGACore
{
public:
	CIGACoreImpl();
	virtual ~CIGACoreImpl();
	
	virtual void OnDirect3DCreate(IDirect3D8 *pD3D);
	
	virtual void OnInitD3DDevice(IDirect3DDevice8* pDevice);
	virtual void OnDestroyD3DDevice(IDirect3DDevice8* pDevice);

	virtual void OnCreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture8* pTexture);
	virtual void OnReleaseTexture(IDirect3DTexture8* pTexture);

	virtual IDirect3DTexture8* OnSetTexture(DWORD Stage,IDirect3DTexture8* pTexture);

	virtual void OnGetSurfaceLevel(IDirect3DTexture8* pTexture, UINT Level, IDirect3DSurface8* pSurfaceLevel);

	virtual void OnBeginScene();
	virtual void OnEndScene();

	virtual void OnSurfaceLockRect(IDirect3DSurface8* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags);
	virtual void OnSurfaceUnlockRect(IDirect3DSurface8* pSurface);

protected:
	void SetExportPath(LPCTSTR lpszExportPath);
	void LoadConfigDocument(LPCTSTR lpszFileName);

	void LoadAdsResource();
	void Load2DAdsResource();
	void LoadVideoAdsResource();

	void ReleaseAdsResource();

	void PrintTextureDesc(IDirect3DTexture8* pTexture);

private:
    IDirect3DDevice8* m_pIDirect3DDevice8;

	TCHAR m_chExportPath[MAX_PATH];

	BOOL  m_bEnableExport;
	BOOL  m_bEnableScan;
	BOOL  m_bEnablePlayVideo;

	BOOL  m_bRegistered;
};

#endif // #ifndef _IGA_COREIMPL_H_