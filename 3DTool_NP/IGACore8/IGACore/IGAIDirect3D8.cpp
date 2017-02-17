// IGAIDirect3D8.cpp

#include "StdAfx.h"
#include "IGAIDirect3D8.h"
#include "IGAIDirect3DDevice8.h"
#include "IGACore.h"


IGAIDirect3D8::IGAIDirect3D8(IDirect3D8 *pOriginal)
{
    m_pIDirect3D8 = pOriginal;
}

IGAIDirect3D8::~IGAIDirect3D8(void)
{
}

HRESULT  __stdcall IGAIDirect3D8::QueryInterface(REFIID riid, void** ppvObj) {
    *ppvObj = NULL;
	HRESULT hRes = m_pIDirect3D8->QueryInterface(riid, ppvObj); 
	if (hRes == NOERROR) {
		*ppvObj = this;
	}
	return hRes;
}

ULONG __stdcall IGAIDirect3D8::AddRef(void) {
    return m_pIDirect3D8->AddRef();
}

ULONG __stdcall IGAIDirect3D8::Release(void) {
    extern IGAIDirect3D8* gl_pIGAIDirect3D8;
	ULONG lCount = m_pIDirect3D8->Release();
	if (lCount == 0) {
		gl_pIGAIDirect3D8 = NULL;
  	    delete this; 
	}
	return lCount;
}

HRESULT __stdcall IGAIDirect3D8::RegisterSoftwareDevice(void* pInitializeFunction)
{
	return m_pIDirect3D8->RegisterSoftwareDevice(pInitializeFunction);
}

UINT __stdcall IGAIDirect3D8::GetAdapterCount() 
{
	return m_pIDirect3D8->GetAdapterCount();
}

HRESULT __stdcall IGAIDirect3D8::GetAdapterIdentifier(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER8* pIdentifier) 
{
	return m_pIDirect3D8->GetAdapterIdentifier( Adapter, Flags, pIdentifier);
}

UINT __stdcall IGAIDirect3D8::GetAdapterModeCount(UINT Adapter) 
{
	return m_pIDirect3D8->GetAdapterModeCount(Adapter);
}

HRESULT __stdcall IGAIDirect3D8::EnumAdapterModes(UINT Adapter,UINT Mode,D3DDISPLAYMODE* pMode) 
{
    return m_pIDirect3D8->EnumAdapterModes( Adapter, Mode, pMode);
}

HRESULT __stdcall IGAIDirect3D8::GetAdapterDisplayMode( UINT Adapter,D3DDISPLAYMODE* pMode) 
{
	return m_pIDirect3D8->GetAdapterDisplayMode(  Adapter, pMode);
}

HRESULT __stdcall IGAIDirect3D8::CheckDeviceType(UINT Adapter,D3DDEVTYPE CheckType,D3DFORMAT DisplayFormat,D3DFORMAT BackBufferFormat,BOOL Windowed) 
{
	return m_pIDirect3D8->CheckDeviceType( Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
}

HRESULT __stdcall IGAIDirect3D8::CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) 
{
	return m_pIDirect3D8->CheckDeviceFormat( Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT __stdcall IGAIDirect3D8::CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType) 
{
	return m_pIDirect3D8->CheckDeviceMultiSampleType( Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType);
}

HRESULT __stdcall IGAIDirect3D8::CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) 
{
	return m_pIDirect3D8->CheckDepthStencilMatch( Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT __stdcall IGAIDirect3D8::GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS8* pCaps) 
{
	return m_pIDirect3D8->GetDeviceCaps( Adapter, DeviceType, pCaps);
}

HMONITOR __stdcall IGAIDirect3D8::GetAdapterMonitor(UINT Adapter) 
{
	return m_pIDirect3D8->GetAdapterMonitor( Adapter);
}

HRESULT __stdcall IGAIDirect3D8::CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice8** ppReturnedDeviceInterface) {
    // global var
	extern IGAIDirect3DDevice8* gl_pIGAIDirect3DDevice8;
	HRESULT hr = m_pIDirect3D8->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	if( SUCCEEDED(hr) )
	{
		OutputDebugString("D3D8设备创建成功！");
		
		DWORD  dwRefCount = (*ppReturnedDeviceInterface)->AddRef();

		gl_pIGAIDirect3DDevice8 = new IGAIDirect3DDevice8(*ppReturnedDeviceInterface);
		GetIGACore()->OnInitD3DDevice(gl_pIGAIDirect3DDevice8);

		//falsh窗口
		IGAWIN::CreateIGAWin( gl_pIGAIDirect3DDevice8, hFocusWindow );
		IGAWIN::ShowFlash(gl_pIGAIDirect3DDevice8,0,0,320,240);
	    //IGAWIN::ShowVideo(gl_pIGAIDirect3DDevice8,0,0,320,240);

		(*ppReturnedDeviceInterface)->AddRef();
        DWORD  dwRefCount1 =  (*ppReturnedDeviceInterface)->Release();
		gl_pIGAIDirect3DDevice8->m_dwRefCount   = dwRefCount1 - dwRefCount ;
		(*ppReturnedDeviceInterface)->Release();

		*ppReturnedDeviceInterface = gl_pIGAIDirect3DDevice8;
	}
	else
	{
		OutputDebugString("D3D8设备创建失败！");
	}

	return hr; 
}
  
