#include "StdAfx.h"
#include "ProxyIDirect3D8.h"
#include "ProxyIDirect3DDevice8.h"


IGA_IDirect3D8::IGA_IDirect3D8(const IDirect3D8 *pkOriginal)
{
	m_pkD3D    = (IDirect3D8*)pkOriginal;
	m_pFilter  = NULL;		
}

IGA_IDirect3D8::~IGA_IDirect3D8()
{
}

HRESULT  STDMETHODCALLTYPE IGA_IDirect3D8::QueryInterface(REFIID riid, void** ppvObj)
{
	*ppvObj = 0;

	HRESULT hRes = m_pkD3D->QueryInterface(riid, ppvObj); 

	if(hRes == NOERROR)
	    *ppvObj = this;

	return hRes;
}

ULONG STDMETHODCALLTYPE IGA_IDirect3D8::AddRef()
{
	return m_pkD3D->AddRef();
}

ULONG STDMETHODCALLTYPE IGA_IDirect3D8::Release()
{
	ULONG ulCount = m_pkD3D->Release();
	
	if ( NULL != m_pFilter)
	{
       m_pFilter->OnRelease_After(ulCount);
	}

	if (ulCount == 0) 
		delete this; 

	return ulCount;
}

HRESULT STDMETHODCALLTYPE IGA_IDirect3D8::RegisterSoftwareDevice(void* pInitializeFunction)
{
	return m_pkD3D->RegisterSoftwareDevice(pInitializeFunction);
}

UINT STDMETHODCALLTYPE IGA_IDirect3D8::GetAdapterCount()
{
	return m_pkD3D->GetAdapterCount();
}

HRESULT STDMETHODCALLTYPE IGA_IDirect3D8::GetAdapterIdentifier(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER8* pIdentifier)
{
	return m_pkD3D->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT STDMETHODCALLTYPE IGA_IDirect3D8::GetAdapterModeCount(UINT Adapter)
{
	return m_pkD3D->GetAdapterModeCount(Adapter);
}

HRESULT STDMETHODCALLTYPE IGA_IDirect3D8::EnumAdapterModes(UINT Adapter,UINT Mode,D3DDISPLAYMODE* pMode)
{
	return m_pkD3D->EnumAdapterModes(Adapter, Mode, pMode);
}

HRESULT STDMETHODCALLTYPE IGA_IDirect3D8::GetAdapterDisplayMode(UINT Adapter,D3DDISPLAYMODE* pMode)
{
	return m_pkD3D->GetAdapterDisplayMode(Adapter,pMode);
}

HRESULT STDMETHODCALLTYPE IGA_IDirect3D8::CheckDeviceType(UINT Adapter,D3DDEVTYPE CheckType,D3DFORMAT DisplayFormat,D3DFORMAT BackBufferFormat,BOOL Windowed)
{
	return m_pkD3D->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
}

HRESULT STDMETHODCALLTYPE IGA_IDirect3D8::CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat)
{
	return m_pkD3D->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT STDMETHODCALLTYPE IGA_IDirect3D8::CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType)
{
	return m_pkD3D->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType);
}

HRESULT STDMETHODCALLTYPE IGA_IDirect3D8::CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat)
{
	return m_pkD3D->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT STDMETHODCALLTYPE IGA_IDirect3D8::GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS8* pCaps)
{
	return m_pkD3D->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR STDMETHODCALLTYPE IGA_IDirect3D8::GetAdapterMonitor(UINT Adapter)
{
	return m_pkD3D->GetAdapterMonitor(Adapter);
}


extern D3D_FILTER   g_d3d_filters;

HRESULT STDMETHODCALLTYPE IGA_IDirect3D8::CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice8** ppReturnedDeviceInterface)
{
	//BehaviorFlags |= D3DCREATE_MULTITHREADED;
	//LOG << "DeviceType"  << (long )DeviceType;
	HRESULT hRes = m_pkD3D->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	IGAIDirect3DDevice8 *pkD3DDevice = NULL;
	if(SUCCEEDED(hRes))
	{
		IDirect3DDevice8* pDeviceOrg = *ppReturnedDeviceInterface;
		pkD3DDevice = new IGAIDirect3DDevice8(*ppReturnedDeviceInterface);
		*ppReturnedDeviceInterface = pkD3DDevice;

		if  ( NULL != m_pFilter)
		{		     
			//设置设备过滤器
			if  ( g_d3d_filters.pFilterDevice != NULL )
			{	     
				 pkD3DDevice->SetFilter(g_d3d_filters.pFilterDevice);			 
			}

			//创建后事件
			m_pFilter->OnCreateDevice_After(pkD3DDevice,hFocusWindow,pDeviceOrg);
			
		}
    }
	return(hRes); 
}
  
