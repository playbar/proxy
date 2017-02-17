//************************************************************************************************************
// class      :  
// purpose    :  d3d8π˝¬À¿‡
// Date       :  2008-7-30  13:33:55
// author     :  3o3afe1
//************************************************************************************************************
#ifndef IGA_IDirect3D8_H
#define IGA_IDirect3D8_H

#define NOMINMAX
#include <d3d8.h>
#include "..\D3D8Filter.h"

class IGA_IDirect3D8 : public IDirect3D8
{
public:
    IGA_IDirect3D8(const IDirect3D8 *pkOriginal);
    virtual ~IGA_IDirect3D8();

	/*** IUnknown methods ***/
    HRESULT  STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj);
    ULONG    STDMETHODCALLTYPE AddRef();
    ULONG    STDMETHODCALLTYPE Release();

	/*** IDirect3D8 methods ***/
    HRESULT  STDMETHODCALLTYPE RegisterSoftwareDevice(void* pInitializeFunction);
    UINT     STDMETHODCALLTYPE GetAdapterCount();
    HRESULT  STDMETHODCALLTYPE GetAdapterIdentifier(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER8* pIdentifier);
    UINT     STDMETHODCALLTYPE GetAdapterModeCount(UINT Adapter);
    HRESULT  STDMETHODCALLTYPE EnumAdapterModes(UINT Adapter,UINT Mode,D3DDISPLAYMODE* pMode);
    HRESULT  STDMETHODCALLTYPE GetAdapterDisplayMode(UINT Adapter,D3DDISPLAYMODE* pMode);
    HRESULT  STDMETHODCALLTYPE CheckDeviceType(UINT Adapter,D3DDEVTYPE CheckType,D3DFORMAT DisplayFormat,D3DFORMAT BackBufferFormat,BOOL Windowed);
    HRESULT  STDMETHODCALLTYPE CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat);
    HRESULT  STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType);
    HRESULT  STDMETHODCALLTYPE CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat);
    HRESULT  STDMETHODCALLTYPE GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS8* pCaps);
    HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT Adapter);
    HRESULT  STDMETHODCALLTYPE CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice8** ppReturnedDeviceInterface);

public:   
	inline  VOID SetFilter(Filter_IDirect3D8*  pFilter)	{		 m_pFilter = pFilter;	}
private:
    IDirect3D8*           m_pkD3D;
	Filter_IDirect3D8*    m_pFilter;
};

#endif
