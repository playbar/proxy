#include "StdAfx.h"

//extern IGA_IDirect3DDevice9* gl_pmyIDirect3DDevice9;

IGA_IDirect3D9::IGA_IDirect3D9(IDirect3D9 *pOriginal)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::IGA_IDirect3D9);
    m_pIDirect3D9 = pOriginal;
}

IGA_IDirect3D9::~IGA_IDirect3D9(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::~IGA_IDirect3D9);
}

HRESULT  __stdcall IGA_IDirect3D9::QueryInterface(REFIID riid, void** ppvObj)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::QueryInterface);
    *ppvObj = NULL;
	HRESULT hRes = m_pIDirect3D9->QueryInterface(riid, ppvObj); 

	//if (hRes == NOERROR) // if OK, send our "fake" address
	//{
	//	*ppvObj = this;
	//}
	return hRes;
}

ULONG    __stdcall IGA_IDirect3D9::AddRef(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::AddRef);
    return(m_pIDirect3D9->AddRef());
}

ULONG    __stdcall IGA_IDirect3D9::Release(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::Release);
	ULONG count = m_pIDirect3D9->Release();
	if (count == 0) 
	{
  	    delete(this); 
	}

	return(count);
}

HRESULT __stdcall IGA_IDirect3D9::RegisterSoftwareDevice(void* pInitializeFunction)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::RegisterSoftwareDevice);
    return(m_pIDirect3D9->RegisterSoftwareDevice(pInitializeFunction));
}

UINT __stdcall IGA_IDirect3D9::GetAdapterCount(void)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::GetAdapterCount);
    return(m_pIDirect3D9->GetAdapterCount());
}

HRESULT __stdcall IGA_IDirect3D9::GetAdapterIdentifier(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::GetAdapterIdentifier);
    return(m_pIDirect3D9->GetAdapterIdentifier(Adapter,Flags,pIdentifier));
}

UINT __stdcall IGA_IDirect3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::GetAdapterModeCount);
    return(m_pIDirect3D9->GetAdapterModeCount(Adapter, Format));
}

HRESULT __stdcall IGA_IDirect3D9::EnumAdapterModes(UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::EnumAdapterModes);
    return(m_pIDirect3D9->EnumAdapterModes(Adapter,Format,Mode,pMode));
}

HRESULT __stdcall IGA_IDirect3D9::GetAdapterDisplayMode( UINT Adapter,D3DDISPLAYMODE* pMode)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::GetAdapterDisplayMode);
    return(m_pIDirect3D9->GetAdapterDisplayMode(Adapter,pMode));
}

HRESULT __stdcall IGA_IDirect3D9::CheckDeviceType(UINT iAdapter,D3DDEVTYPE DevType,D3DFORMAT DisplayFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::CheckDeviceType);
    return(m_pIDirect3D9->CheckDeviceType(iAdapter,DevType,DisplayFormat,BackBufferFormat,bWindowed));
}

HRESULT __stdcall IGA_IDirect3D9::CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::CheckDeviceFormat);
    return(m_pIDirect3D9->CheckDeviceFormat(Adapter,DeviceType,AdapterFormat,Usage,RType,CheckFormat));
}

HRESULT __stdcall IGA_IDirect3D9::CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::CheckDeviceMultiSampleType);
    return(m_pIDirect3D9->CheckDeviceMultiSampleType(Adapter,DeviceType,SurfaceFormat,Windowed,MultiSampleType,pQualityLevels));
}

HRESULT __stdcall IGA_IDirect3D9::CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::CheckDepthStencilMatch);
    return(m_pIDirect3D9->CheckDepthStencilMatch(Adapter,DeviceType,AdapterFormat,RenderTargetFormat,DepthStencilFormat));
}

HRESULT __stdcall IGA_IDirect3D9::CheckDeviceFormatConversion(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::CheckDeviceFormatConversion);
    return(m_pIDirect3D9->CheckDeviceFormatConversion(Adapter,DeviceType,SourceFormat,TargetFormat));
}

HRESULT __stdcall IGA_IDirect3D9::GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::GetDeviceCaps);
    return(m_pIDirect3D9->GetDeviceCaps(Adapter,DeviceType,pCaps));
}

HMONITOR __stdcall IGA_IDirect3D9::GetAdapterMonitor(UINT Adapter)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::GetAdapterMonitor);
    return(m_pIDirect3D9->GetAdapterMonitor(Adapter));
}

HRESULT __stdcall IGA_IDirect3D9::CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface)
{
	SHOWOUTPUTDEBUG(IGA_IDirect3D9::CreateDevice);
    HRESULT hres = m_pIDirect3D9->CreateDevice( Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	if ( SUCCEEDED(hres) )
	{
		IGA_IDirect3DDevice9 *pmyIDirect3DDevice9 = new IGA_IDirect3DDevice9(*ppReturnedDeviceInterface);
		*ppReturnedDeviceInterface = pmyIDirect3DDevice9;
	}
	return(hres); 
}
  
