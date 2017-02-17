// IGACore.h
#pragma once

// Exported function

void* WINAPI CheckFullscreen_1();
void* WINAPI ValidatePixelShader_1();
void* WINAPI ValidateVertexShader_1();

void* WINAPI DebugSetMute();

DWORD IGAStartup(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,
							  DWORD,
							  DWORD,
							  UINT SDKVersion);
DWORD  Make_IGAIDirect3D8(IDirect3D8 *pIDirect3D8_orig);
void WINAPI IGAShutdown();



// regular functions
void InitInstance(HANDLE hModule);
void ExitInstance(void);
void LoadOriginalDll(void);

class IIGACore
{
public:
	virtual void OnDirect3DCreate(IDirect3D8 *pD3D) = 0;

	virtual void OnInitD3DDevice(IDirect3DDevice8* pDevice) = 0;
	virtual void OnDestroyD3DDevice(IDirect3DDevice8* pDevice) = 0;

	virtual IDirect3DTexture8* OnSetTexture(DWORD Stage,IDirect3DTexture8* pTexture) = 0;

	virtual void OnCreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture8* pTexture) = 0;
	virtual void OnReleaseTexture(IDirect3DTexture8* pTexture) = 0;

	virtual void OnGetSurfaceLevel(IDirect3DTexture8* pTexture, UINT Level, IDirect3DSurface8* pSurfaceLevel) = 0;

	virtual void OnBeginScene() = 0;
	virtual void OnEndScene() = 0;

	virtual void OnSurfaceLockRect(IDirect3DSurface8* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) = 0;
	virtual void OnSurfaceUnlockRect(IDirect3DSurface8* pSurface) = 0;
};

extern IIGACore* GetIGACore();
