/************************************************************************/
/* IGA_DirectInputDevice8                                               */
/************************************************************************/
//#include "StdAfx.h"
#include "DInputProxy.h"
#include <stdio.h>

# if !defined MAX_INT
#	define	MAX_INT	0x7FFFFFFF
# endif 

# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	if defined(ASSERT)
#		undef ASSERT
#	endif
#	define	ASSERT	
# else
#	define	ASSERT
# endif 

#	define	appCopyMemory		::CopyMemory


/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL WriteMemory(LPCVOID lpAddress, LPCVOID lpData, DWORD dwBytesToWrite)
{
	ASSERT(!::IsBadReadPtr(lpData, dwBytesToWrite));
	ASSERT(dwBytesToWrite >= 0 && dwBytesToWrite < MAX_INT );
	MEMORY_BASIC_INFORMATION mbi	= { 0 };
	::VirtualQuery(lpAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION));  
	if(!::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect))
		return	FALSE;
	ASSERT(mbi.RegionSize >= dwBytesToWrite );
	appCopyMemory(const_cast<void*>(lpAddress), lpData, dwBytesToWrite);

	DWORD dwOldProtect	= 0L;
	return	::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
DWORD GetVirtualFunctionNumber(LPVOID pObject)
{
	ASSERT(pObject != NULL);
	int *p	= (int *) pObject;
	int *q	= NULL;
	DWORD i = 0;
	q = (int*)(*(int*)p);
	while (1)
	{
		if (NULL == q[i]) 
		{
			return i;
		}
		i++;
	}
	return i;
}

DWORD GetVirtualFunctionNumber(const LPVOBJECT pObject)
{
	ASSERT(pObject != NULL);
	DWORD	dwCount	= 0;

	if(!pObject || !pObject->pVft )
		return	0L;
	LPVFT pVft	= pObject->pVft;
	while(NULL != pVft[dwCount++]);
	return	dwCount;
}


/**/
DWORD CopyVFT(LPVFT pVFT, DWORD dwVFTSize, const LPVOBJECT pVObject)
{
	ASSERT(pVFT != NULL);
	ASSERT(dwVFTSize <= MAX_INT);
	if( pVObject == NULL || pVObject->pVft == NULL )
		return	0L;

	DWORD	dwCount	= GetVirtualFunctionNumber(pVObject);
	dwCount	= dwVFTSize > dwCount ? dwCount : dwVFTSize;
	for(DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
		pVFT[dwIndex]	= pVObject->pVft[dwIndex];
	return	dwCount;
}

DWORD ReplaceVFT(LPVOBJECT pTargetObject, const LPVOBJECT pSourceObject)
{
	if( !pTargetObject || !pTargetObject->pVft )
		return	0L;
	if( !pSourceObject || !pSourceObject->pVft )
		return	0L;

	DWORD	dwCount1	= GetVirtualFunctionNumber( pTargetObject );
	DWORD	dwCount2	= GetVirtualFunctionNumber( pSourceObject );
	dwCount1	= dwCount1 > dwCount2 ? dwCount2 : dwCount1;
	for(DWORD dwIndex = 0; dwIndex < dwCount1; dwIndex++)
	{
		WriteMemory(&pTargetObject->pVft[dwIndex], &pSourceObject->pVft[dwIndex], sizeof(VFT));
	}
	return	dwCount1;
}

DWORD ReplaceN_VFT(LPVOBJECT pTargetObject, const LPVOBJECT pSourceObject, DWORD dwVFTSize)
{
	if( !pTargetObject || !pTargetObject->pVft )
		return	0L;
	if( !pSourceObject || !pSourceObject->pVft )
		return	0L;

	DWORD	dwCount		= GetVirtualFunctionNumber( pTargetObject );
	dwCount		= dwCount > dwVFTSize ? dwVFTSize : dwCount;
	ASSERT(dwCount <= GetVirtualFunctionNumber( pSourceObject ) );

	for(DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
	{
		WriteMemory(&pTargetObject->pVft[dwIndex], &pSourceObject->pVft[dwIndex], sizeof(VFT));
	}
	return	dwCount;
}

IGA_IDirectInput8::IGA_IDirectInput8(IDirectInput8* pIDirectInput8)
:m_pIDirectInput8(pIDirectInput8)
{
	OutputDebugString("IGA_IDirectInput8::IGA_IDirectInput8()");
}

IGA_IDirectInput8::~IGA_IDirectInput8()
{
	OutputDebugString("IGA_IDirectInput8::~IGA_IDirectInput8()");
}

HRESULT IGA_IDirectInput8::QueryInterface(REFIID riid, LPVOID FAR * ppvObj)
{
	OutputDebugString("IGA_IDirectInput8::QueryInterface()");
 	HRESULT hr = m_pIDirectInput8->QueryInterface(riid, ppvObj);
	return hr;
}

ULONG IGA_IDirectInput8::AddRef()
{

	OutputDebugString("IGA_IDirectInput8::AddRef()");
	ULONG ul =  m_pIDirectInput8->AddRef();
	return ul;
}

ULONG IGA_IDirectInput8::Release()
{
	OutputDebugString("IGA_IDirectInput8::Release()");
	return m_pIDirectInput8->Release();
}
HRESULT IGA_IDirectInput8::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8A * lpinput, LPUNKNOWN lpUn)
{
	
	HRESULT hr = m_pIDirectInput8->CreateDevice(rguid, lpinput, lpUn);
	static bool bReplace = false;
	if ( SUCCEEDED(hr) )
	{
		if ( !bReplace )
		{
			OutputDebugString("IGA_IDirectInput8::CreateDevice");
			IGA_IDirectInputDevice8::Init( *lpinput );
			bReplace = true;
		}
	}
	return hr;
}

HRESULT IGA_IDirectInput8::EnumDevices(DWORD dword,LPDIENUMDEVICESCALLBACKA lpnd,LPVOID lpvoid,DWORD dword2)
{
	OutputDebugString("IGA_IDirectInput8::EnumDevices()");
	HRESULT hr = m_pIDirectInput8->EnumDevices(dword, lpnd, lpvoid, dword2);
	return hr;
}

HRESULT IGA_IDirectInput8::GetDeviceStatus(REFGUID  guid)
{
	OutputDebugString("IGA_IDirectInput8::GetDeviceStatus()");
	HRESULT hr = m_pIDirectInput8->GetDeviceStatus(guid);
	return hr;
}

HRESULT IGA_IDirectInput8::RunControlPanel(HWND hwnd, DWORD dword)
{
	OutputDebugString("IGA_IDirectInput8::RunControlPanel()");
	HRESULT hr = m_pIDirectInput8->RunControlPanel(hwnd, dword);
	return hr;
}

HRESULT IGA_IDirectInput8::Initialize(HINSTANCE instance, DWORD dword)
{
	OutputDebugString("IGA_IDirectInput8::Initialize()");
	HRESULT hr = m_pIDirectInput8->Initialize(instance, dword);
	return hr;
}

HRESULT IGA_IDirectInput8::FindDevice(REFGUID guid, LPCSTR lpstr, LPGUID lpguid )
{
	OutputDebugString("IGA_IDirectInput8::FindDevice()");
	HRESULT hr = m_pIDirectInput8->FindDevice(guid, lpstr, lpguid );
	return hr;
}

HRESULT IGA_IDirectInput8::EnumDevicesBySemantics(LPCSTR lpstr, LPDIACTIONFORMATA lpInfo, LPDIENUMDEVICESBYSEMANTICSCBA lpSystem, LPVOID lpvoid, DWORD dword)
{
	OutputDebugString("IGA_IDirectInput8::EnumDevicesBySemantics()");
	HRESULT hr = m_pIDirectInput8->EnumDevicesBySemantics(lpstr, lpInfo, lpSystem, lpvoid, dword);
	return hr;
}


HRESULT IGA_IDirectInput8::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdevice, LPDICONFIGUREDEVICESPARAMSA lparam, DWORD dword, LPVOID lpvoid)
{
	OutputDebugString("IGA_IDirectInput8::ConfigureDevices()");
	HRESULT hr = m_pIDirectInput8->ConfigureDevices(lpdevice, lparam, dword, lpvoid);
	return hr; 
}
 
/************************************************************************/
// IGA_IDirectInputDevice8                                                               
/************************************************************************/
IGA_IMPLEMENT_CLASS( IDirectInputDevice8 )
BOOL IGA_IDirectInputDevice8::Init(IDirectInputDevice8 *pObject)
{
	ASSERT ( NULL != pObject );
	INIT_VIRTUAL_OBJECT(pObject);
	DWORD dwIndex = 0;
	/* IUnknow methods */
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, QueryInterface);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, AddRef);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Release);

	/* IDirectInputDevice8 methods*/
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetCapabilities);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, EnumObjects);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetProperty);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetProperty);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Acquire);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Unacquire);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetDeviceState);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetDeviceData);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetDataFormat);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetEventNotification);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SetCooperativeLevel);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetObjectInfo);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetDeviceInfo);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, RunControlPanel);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Initialize);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, CreateEffect);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, EnumEffects);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetEffectInfo);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetForceFeedbackState);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SendForceFeedbackCommand);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, EnumCreatedEffectObjects);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Escape);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, Poll);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, SendDeviceData);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, EnumEffectsInFile);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, WriteEffectToFile);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, BuildActionMap);
	FILL_VIRTUAL_FUNC_TABLE(dwIndex, GetImageInfo);
	return true;
}

//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectInputDevice8, QueryInterface, (REFIID riid, void ** ppvObj),(this, rrid, ppvObj));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8,QueryInterface)
HRESULT IGA_IDirectInputDevice8::QueryInterface(REFIID riid, void** ppvObj)
{
	OutputDebugString("IGA_IDirectInputDevice8::QueryInterface()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnQueryInterface(this, riid, ppvObj);
	return hr;
}
//IMPLEMENT_MEMBER_FUNCTION(HRESULT, IDirectInputDevice8, AddRef, (void), (this));
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, AddRef)
ULONG IGA_IDirectInputDevice8::AddRef()
{
	OutputDebugString("IGA_IDirectInputDevice8::AddRef()");
	ULONG ul = IGA_IDirectInputDevice8::m_pfnAddRef(this);
	return ul;
}


IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, Release)
ULONG IGA_IDirectInputDevice8::Release()
{
	OutputDebugString("IGA_IDirectInputDevice8::Release()");
	char chOutputString[256];
	ULONG re = IGA_IDirectInputDevice8::m_pfnRelease(this);

	sprintf(chOutputString, "IGA_IDirectInputDevice8::Release, re = %d", re);
	OutputDebugString(chOutputString);
	return re;

}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, GetCapabilities);
HRESULT IGA_IDirectInputDevice8::GetCapabilities(LPDIDEVCAPS param)
{
	OutputDebugString("IGA_IDirectInputDevice8::GetCapabilities()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnGetCapabilities(this, param);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, EnumObjects)
HRESULT IGA_IDirectInputDevice8::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA param1, LPVOID param2, DWORD param3)
{
	OutputDebugString("IGA_IDirectInputDevice8::EnumObjects()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnEnumObjects(this, param1, param2, param3);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, GetProperty)
HRESULT IGA_IDirectInputDevice8::GetProperty( REFGUID riid,LPDIPROPHEADER lpdh)
{
	OutputDebugString("IGA_IDirectInputDevice8::GetProperty()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnGetProperty(this,riid, lpdh);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, SetProperty)
HRESULT IGA_IDirectInputDevice8::SetProperty(REFGUID riid, LPCDIPROPHEADER lpdh)
{
	OutputDebugString("IGA_IDirectInputDevice8::SetProperty()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnSetProperty(this, riid, lpdh);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, Acquire)
HRESULT IGA_IDirectInputDevice8::Acquire()
{
	OutputDebugString("IGA_IDirectInputDevice8::Acquire()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnAcquire(this);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, Unacquire)
HRESULT IGA_IDirectInputDevice8::Unacquire()
{
	OutputDebugString("IGA_IDirectInputDevice8::Unacquire()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnUnacquire(this);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, GetDeviceState)
HRESULT IGA_IDirectInputDevice8::GetDeviceState(DWORD param1, LPVOID param2)
{
	OutputDebugString("IGA_IDirectInputDevice8::GetDeviceState()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnGetDeviceState(this, param1, param2);
	UCHAR *uchTemp = (UCHAR *)param2;
	if (uchTemp[DIK_9])
	{
		OutputDebugString("Press 9");
	}
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, GetDeviceData)
HRESULT IGA_IDirectInputDevice8::GetDeviceData(DWORD param1, LPDIDEVICEOBJECTDATA param2, LPDWORD param3,DWORD param4)
{
	OutputDebugString("IGA_IDirectInputDevice8::GetDeviceData()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnGetDeviceData(this, param1, param2, param3, param4);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, SetDataFormat)
HRESULT IGA_IDirectInputDevice8::SetDataFormat(LPCDIDATAFORMAT param)
{
	OutputDebugString("IGA_IDirectInputDevice8::SetDataFormat()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnSetDataFormat(this, param);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, SetEventNotification)
HRESULT IGA_IDirectInputDevice8::SetEventNotification(HANDLE handle)
{
	OutputDebugString("IGA_IDirectInputDevice8::SetEventNotification()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnSetEventNotification(this, handle);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, SetCooperativeLevel)
HRESULT IGA_IDirectInputDevice8::SetCooperativeLevel(HWND hwnd, DWORD dw)
{
	OutputDebugString("IGA_IDirectInputDevice8::SetCooperativeLevel()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnSetCooperativeLevel(this, hwnd, dw);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, GetObjectInfo)
HRESULT IGA_IDirectInputDevice8::GetObjectInfo(LPDIDEVICEOBJECTINSTANCE lpdoi, DWORD dw1, DWORD dw2)
{
	OutputDebugString("IGA_IDirectInputDevice8::GetObjectInfo()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnGetObjectInfo(this, lpdoi, dw1, dw2 );
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, GetDeviceInfo)
HRESULT IGA_IDirectInputDevice8::GetDeviceInfo(LPDIDEVICEINSTANCE lpdi)
{
	OutputDebugString("IGA_IDirectInputDevice8::GetDeviceInfo()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnGetDeviceInfo(this, lpdi);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, RunControlPanel )
HRESULT IGA_IDirectInputDevice8::RunControlPanel(HWND dw1, DWORD dw2)
{
	OutputDebugString("IGA_IDirectInputDevice8::RunControlPanel()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnRunControlPanel(this, dw1, dw2);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, Initialize)
HRESULT IGA_IDirectInputDevice8::Initialize(HINSTANCE hinstace, DWORD dw, REFGUID riid)
{
	OutputDebugString("IGA_IDirectInputDevice8::Initialize()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnInitialize(this, hinstace, dw, riid);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, CreateEffect)
HRESULT IGA_IDirectInputDevice8::CreateEffect(REFGUID riid, LPCDIEFFECT lpcd, LPDIRECTINPUTEFFECT * lpdie, LPUNKNOWN lpun)
{
	OutputDebugString("IGA_IDirectInputDevice8::CreateEffect()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnCreateEffect(this, riid, lpcd, lpdie, lpun);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, EnumEffects)
HRESULT IGA_IDirectInputDevice8::EnumEffects(LPDIENUMEFFECTSCALLBACK lpdes, LPVOID lpvoid, DWORD dw)
{
	OutputDebugString("IGA_IDirectInputDevice8::EnumEffects()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnEnumEffects(this, lpdes, lpvoid, dw);
	return hr;
}
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, GetEffectInfo)
HRESULT IGA_IDirectInputDevice8::GetEffectInfo(LPDIEFFECTINFO lpef, REFGUID riid)
{
	OutputDebugString("IGA_IDirectInputDevice8::GetEffectInfo()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnGetEffectInfo(this, lpef, riid);
	return hr;
}
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, GetForceFeedbackState)
HRESULT IGA_IDirectInputDevice8::GetForceFeedbackState(LPDWORD lpdw)
{
	OutputDebugString("IGA_IDirectInputDevice8::GetForceFeedbackState()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnGetForceFeedbackState(this, lpdw);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, SendForceFeedbackCommand)
HRESULT IGA_IDirectInputDevice8::SendForceFeedbackCommand(DWORD dw)
{
	OutputDebugString("IGA_IDirectInputDevice8::SendForceFeedbackCommand()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnSendForceFeedbackCommand(this, dw);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, EnumCreatedEffectObjects)
HRESULT IGA_IDirectInputDevice8::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpdob, LPVOID lpvoid, DWORD dw)
{
	OutputDebugString("IGA_IDirectInputDevice8::EnumCreatedEffectObject()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnEnumCreatedEffectObjects(this, lpdob, lpvoid, dw);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, Escape)
HRESULT IGA_IDirectInputDevice8::Escape(LPDIEFFESCAPE lpds)
{
	OutputDebugString("IGA_IDirectInputDevice8::Escape()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnEscape(this, lpds);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, Poll)
HRESULT IGA_IDirectInputDevice8::Poll()
{
	OutputDebugString("IGA_IDirectInputDevice8::Poll()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnPoll(this);
	return hr;
}
 
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, SendDeviceData)
HRESULT IGA_IDirectInputDevice8::SendDeviceData(DWORD dw1, LPCDIDEVICEOBJECTDATA lpcdod, LPDWORD lpdw, DWORD dw2)
{
	OutputDebugString("IGA_IDirectInputDevice8::SendDeviceData()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnSendDeviceData(this, dw1, lpcdod, lpdw, dw2);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, EnumEffectsInFile)
HRESULT IGA_IDirectInputDevice8::EnumEffectsInFile(LPCTSTR lpstr, LPDIENUMEFFECTSINFILECALLBACK lpdei, LPVOID lpvoid,DWORD dw)
{
	OutputDebugString("IGA_IDirectInputDevice8::EnumEffectsInFile()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnEnumEffectsInFile(this, lpstr, lpdei, lpvoid, dw);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, WriteEffectToFile)
HRESULT IGA_IDirectInputDevice8::WriteEffectToFile(LPCSTR lpstr, DWORD dw1, LPDIFILEEFFECT lpde, DWORD dw2)
{
	OutputDebugString("IGA_IDirectInputDevice8::WriteEffectToFile()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnWriteEffectToFile(this, lpstr, dw1, lpde, dw2);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, BuildActionMap)
HRESULT IGA_IDirectInputDevice8::BuildActionMap(LPDIACTIONFORMATA lpdaf, LPCTSTR lpstr, DWORD dw)
{
	OutputDebugString("IGA_IDirectInputDevice8::BuildActionMap()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnBuildActionMap(this, lpdaf, lpstr, dw);
	return hr;
}
IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, SetActionMap)
HRESULT IGA_IDirectInputDevice8::SetActionMap(LPDIACTIONFORMATA lpdaf, LPCTSTR lpstr, DWORD dw)
{
	OutputDebugString("IGA_IDirectInputDevice8::SetActionMap()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnSetActionMap(this, lpdaf, lpstr, dw);
	return hr;
}

IMPLEMENT_MEMBER_FUNCTION_HELPER2(IDirectInputDevice8, GetImageInfo)
HRESULT IGA_IDirectInputDevice8::GetImageInfo(LPDIDEVICEIMAGEINFOHEADER lpdih)
{
	OutputDebugString("IGA_IDirectInputDevice8::GetImageInfo()");
	HRESULT hr = IGA_IDirectInputDevice8::m_pfnGetImageInfo(this, lpdih);
	return hr;
}

