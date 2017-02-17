#pragma once

class IGA_IDirect3DTexture9 : public IDirect3DTexture9
{
public:
	IGA_IDirect3DTexture9(IDirect3DTexture9 * pIDirect3DTexture9);
	virtual ~IGA_IDirect3DTexture9(void);
	// START: The original DX9 function definitions
	HRESULT __stdcall QueryInterface (REFIID riid, void** ppvObj);
	ULONG   __stdcall AddRef(void);
	ULONG   __stdcall Release(void);

	HRESULT __stdcall GetDevice(IDirect3DDevice9** ppDevice);
	HRESULT __stdcall SetPrivateData(REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags);
	HRESULT __stdcall GetPrivateData(REFGUID refguid,void* pData,DWORD* pSizeOfData);
	HRESULT __stdcall FreePrivateData(REFGUID refguid);
	DWORD   __stdcall SetPriority( DWORD PriorityNew);
	DWORD   __stdcall GetPriority();
	void	__stdcall PreLoad();
	D3DRESOURCETYPE __stdcall GetType();
	DWORD   __stdcall SetLOD(DWORD LODNew);
	DWORD   __stdcall GetLOD();
	DWORD   __stdcall GetLevelCount();
	HRESULT __stdcall SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType);
	D3DTEXTUREFILTERTYPE __stdcall GetAutoGenFilterType();
	void   __stdcall GenerateMipSubLevels();
	HRESULT __stdcall GetLevelDesc( UINT Level,D3DSURFACE_DESC *pDesc );
	HRESULT __stdcall GetSurfaceLevel(UINT Level,IDirect3DSurface9** ppSurfaceLevel);
	HRESULT __stdcall LockRect(UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags);
	HRESULT __stdcall UnlockRect(UINT Level);
	HRESULT __stdcall AddDirtyRect(CONST RECT* pDirtyRect);


private:
	IDirect3DTexture9 * m_pIDirect3DTexture9;
};
