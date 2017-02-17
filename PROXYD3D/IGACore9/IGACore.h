// IGACore.h
#pragma once

// Exported function
DWORD IGAStartup(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,
							  DWORD dwRetValue,
							  DWORD dwRawRetAddress,
							  UINT SDKVersion);
DWORD  Make_IGAIDirect3D9(IDirect3D9 *pIDirect3D9_orig);
void WINAPI IGAShutdown();

// regular functions
void InitInstance(HANDLE hModule);
void ExitInstance(void);
void LoadOriginalDll(void);

class IIGACore
{
public:
	virtual void OnDirect3DCreate(IDirect3D9 *pD3D) = 0;

	virtual void OnInitD3DDevice(IDirect3DDevice9* pDevice) = 0;
	virtual void OnDestroyD3DDevice(IDirect3DDevice9* pDevice) = 0;

	virtual IDirect3DTexture9* OnSetTexture(DWORD Stage,IDirect3DTexture9* pTexture) = 0;

	virtual void OnCreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9* pTexture,HANDLE* pSharedHandle) = 0;
	virtual void OnReleaseTexture(IDirect3DTexture9* pTexture) = 0;

	virtual void OnGetSurfaceLevel(IDirect3DTexture9* pTexture, UINT Level, IDirect3DSurface9* pSurfaceLevel) = 0;

	virtual void OnBeginScene() = 0;
	virtual void OnEndScene() = 0;

	virtual void OnSurfaceLockRect(IDirect3DSurface9* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) = 0;
	virtual void OnSurfaceUnlockRect(IDirect3DSurface9* pSurface) = 0;
};

extern IIGACore* GetIGACore();
