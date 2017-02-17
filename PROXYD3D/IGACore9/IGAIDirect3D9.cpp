// IGAIDirect3D9.cpp

#include "StdAfx.h"
#include "IGAIDirect3D9.h"
#include "IGAIDirect3DDevice9.h"
#include "IGACore.h"


IGAIDirect3D9::IGAIDirect3D9(IDirect3D9 *pOriginal)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::IGAIDirect3D9");
#endif

    m_pIDirect3D9 = pOriginal;
}

IGAIDirect3D9::~IGAIDirect3D9(void)
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::~IGAIDirect3D9");
#endif
}

HRESULT  __stdcall IGAIDirect3D9::QueryInterface(REFIID riid, void** ppvObj) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::QueryInterface");
#endif

    *ppvObj = NULL;
	HRESULT hRes = m_pIDirect3D9->QueryInterface(riid, ppvObj); 
	if (hRes == NOERROR) {
		*ppvObj = this;
	}
	return hRes;
}

ULONG __stdcall IGAIDirect3D9::AddRef(void) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::AddRef");
#endif

    return m_pIDirect3D9->AddRef();
}

ULONG __stdcall IGAIDirect3D9::Release(void) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::Release");
#endif

    extern IGAIDirect3D9* gl_pIGAIDirect3D9;
	ULONG lCount = m_pIDirect3D9->Release();
	//if (lCount == 0) {
	//	gl_pIGAIDirect3D9 = NULL;
 // 	    delete this; 
	//}
	return lCount;
}

HRESULT __stdcall IGAIDirect3D9::RegisterSoftwareDevice(void* pInitializeFunction) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::RegisterSoftwareDevice");
#endif
    return m_pIDirect3D9->RegisterSoftwareDevice(pInitializeFunction);
}

UINT __stdcall IGAIDirect3D9::GetAdapterCount(void) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::GetAdapterCount");
#endif
    return m_pIDirect3D9->GetAdapterCount();
}

HRESULT __stdcall IGAIDirect3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::GetAdapterIdentifier");
#endif
    return m_pIDirect3D9->GetAdapterIdentifier(Adapter,Flags,pIdentifier);
}

UINT __stdcall IGAIDirect3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::GetAdapterModeCount");
#endif
    return m_pIDirect3D9->GetAdapterModeCount(Adapter, Format);
}

HRESULT __stdcall IGAIDirect3D9::EnumAdapterModes(UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::EnumAdapterModes");
#endif
    return m_pIDirect3D9->EnumAdapterModes(Adapter,Format,Mode,pMode);
}

HRESULT __stdcall IGAIDirect3D9::GetAdapterDisplayMode( UINT Adapter,D3DDISPLAYMODE* pMode) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::GetAdapterDisplayMode");
#endif
    return m_pIDirect3D9->GetAdapterDisplayMode(Adapter,pMode);
}

HRESULT __stdcall IGAIDirect3D9::CheckDeviceType(UINT iAdapter,D3DDEVTYPE DevType,D3DFORMAT DisplayFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::CheckDeviceType");
#endif
    return m_pIDirect3D9->CheckDeviceType(iAdapter,DevType,DisplayFormat,BackBufferFormat,bWindowed);
}

HRESULT __stdcall IGAIDirect3D9::CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::CheckDeviceFormat");
#endif
    return m_pIDirect3D9->CheckDeviceFormat(Adapter,DeviceType,AdapterFormat,Usage,RType,CheckFormat);
}

HRESULT __stdcall IGAIDirect3D9::CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::CheckDeviceMultiSampleType");
#endif
    return m_pIDirect3D9->CheckDeviceMultiSampleType(Adapter,DeviceType,SurfaceFormat,Windowed,MultiSampleType,pQualityLevels);
}

HRESULT __stdcall IGAIDirect3D9::CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::CheckDepthStencilMatch");
#endif
    return m_pIDirect3D9->CheckDepthStencilMatch(Adapter,DeviceType,AdapterFormat,RenderTargetFormat,DepthStencilFormat);
}

HRESULT __stdcall IGAIDirect3D9::CheckDeviceFormatConversion(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::CheckDeviceFormatConversion");
#endif
    return m_pIDirect3D9->CheckDeviceFormatConversion(Adapter,DeviceType,SourceFormat,TargetFormat);
}

HRESULT __stdcall IGAIDirect3D9::GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::GetDeviceCaps");
#endif
    return m_pIDirect3D9->GetDeviceCaps(Adapter,DeviceType,pCaps);
}

HMONITOR __stdcall IGAIDirect3D9::GetAdapterMonitor(UINT Adapter) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::GetAdapterMonitor");
#endif
    return m_pIDirect3D9->GetAdapterMonitor(Adapter);
}

HRESULT __stdcall IGAIDirect3D9::CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface) 
{
#if OUTPUT_DEBUG
	OutputDebugString("IGAIDirect3D9::CreateDevice");
#endif
    // global var
	extern IGAIDirect3DDevice9* gl_pIGAIDirect3DDevice9;
	HRESULT hr = m_pIDirect3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

//	TCHAR chDebugString[100];
//	_stprintf(chDebugString, _T("PROXYDLL: IGAIDirect3D9::CreateDevice\r\n"));
//	OutputDebugString(chDebugString);
	if( SUCCEEDED(hr) )
	{   
		DWORD  dwRefCount = (*ppReturnedDeviceInterface)->AddRef();
		gl_pIGAIDirect3DDevice9 = new IGAIDirect3DDevice9(*ppReturnedDeviceInterface);
		GetIGACore()->OnInitD3DDevice(gl_pIGAIDirect3DDevice9);
		//falsh窗口
		//IGAWIN::CreateIGAWin( gl_pIGAIDirect3DDevice9, hFocusWindow );
		//IGAWIN::ShowFlash(gl_pIGAIDirect3DDevice9,0,0,320,240);
	    //IGAWIN::ShowVideo(gl_pIGAIDirect3DDevice8,0,0,320,240);
		

		(*ppReturnedDeviceInterface)->AddRef();
        DWORD  dwRefCount1 =  (*ppReturnedDeviceInterface)->Release();
		gl_pIGAIDirect3DDevice9->m_dwRefCount   = dwRefCount1 - dwRefCount ;
		(*ppReturnedDeviceInterface)->Release();
		
		
		*ppReturnedDeviceInterface = gl_pIGAIDirect3DDevice9;

	}
	else
	{
		OutputDebugString("D3D9设备创建失败！");
	}
	return hr; 
}
  
