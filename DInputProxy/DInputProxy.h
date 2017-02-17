#pragma once
#include "dinput.h"

typedef DWORD	VFT;	// Virtual Function Table Type
typedef VFT		*LPVFT;	// Point to Virtual function table 
typedef void	*LPANYOBJECT;
#define	MAX_VFT_SIZE	1024
/// Common virtual object
typedef	struct tagVObject
{
	LPVFT	pVft;
}VOBJECT, *LPVOBJECT;

#	define	 CPVO2PIVO(pObject)	((VOBJECT*)(LPVOID)(pObject))
extern BOOL WriteMemory(LPCVOID lpAddress, LPCVOID lpData, DWORD dwBytesToWrite);

extern DWORD GetVirtualFunctionNumber(const LPVOBJECT pObject);

extern DWORD CopyVFT(LPVFT pVFT, DWORD dwVFTSize, const LPVOBJECT pVObject);

extern DWORD ReplaceVFT(LPVOBJECT pTargetVObject, const LPVOBJECT pVObject);

extern DWORD ReplaceN_VFT(LPVOBJECT pTargetVObject, const LPVOBJECT pVObject, DWORD dwVFTSize);

#	define	DECLARE_CLASS_BEGIN(classname)												\
struct	IGA_##classname : public classname											\
{																					\
	classname	*m_p##classname;													\
public:																			\
	IGA_##classname():	m_p##classname(NULL){}									\
	IGA_##classname(classname *p##classname);									\
	virtual ~IGA_##classname();			
#	define	DECLARE_CLASS_END()															\
};

#	define	DECLARE_CLASS_BEGIN_EX(classname)											\
struct	IGA_##classname : public classname											\
{																					\
	classname	*m_p##classname;													\
public:																			\
	IGA_##classname():	m_p##classname(NULL){}									\
	IGA_##classname(classname *p##classname)									\
	:m_p##classname(p##classname){}											\
	virtual ~IGA_##classname()	{}	

///////////
#	define	IGA_DECLARE_CLASS_BEGIN(class_name)											\
class	IGA_##class_name : public class_name										\
{																					\
public:																				\
	static IGA_##class_name *GetInstance()											\
	{																				\
		static IGA_##class_name	theObject;											\
		return	&theObject;															\
	}																				\
	static	VFT		s_VirtualFuncTable[MAX_VFT_SIZE];	/*	虚函数指针表*/			\
	static BOOL Init(class_name *pObject);	/*	初始化自定义类*/									

#	define	IGA_DECLARE_CLASS_END()					};	

///	Definitions of member	
#	define	DECLARE_MEMBER_FUNCTION(lRet, Function, Params)								\
	virtual lRet __stdcall Function##Params;								


#	define	DECLARE_MEMBER_FUNCTION_HELPER(lRet, Function, Params)						\
	typedef lRet (__stdcall * LPFN_##Function)Params;									\
	static	LPFN_##Function	m_pfn##Function;

#	define	DECLARE_MEMBER_FUNCTION_EX(lRet, Function, Params1, Params2)				\
	DECLARE_MEMBER_FUNCTION(lRet, Function, Params1);									\
	DECLARE_MEMBER_FUNCTION_HELPER(lRet, Function, Params2);	


#	define	IMPLEMENT_MEMBER_FUNCTION_HELPER0(lRet, classname, Function, Params)		\
	lRet __stdcall IGA_##classname::Function##Params								

#	define	IMPLEMENT_MEMBER_FUNCTION_HELPER1(Function, Params)							\
{	return	m_pfn##Function##Params;	}

#	define	IMPLEMENT_MEMBER_FUNCTION_HELPER2(classname, Function)						\
	IGA_##classname::LPFN_##Function	IGA_##classname::m_pfn##Function	= NULL;	 

#	define	IMPLEMENT_MEMBER_FUNCTION_HELPER_VOID(Function, Params)						\
{	m_pfn##Function##Params;	}

#	define	IMPLEMENT_MEMBER_FUNCTION(lRet, classname, Function, Params, Params1)		\
	IMPLEMENT_MEMBER_FUNCTION_HELPER0(lRet, classname, Function, Params)				\
	IMPLEMENT_MEMBER_FUNCTION_HELPER1(Function, Params1);								\
	IMPLEMENT_MEMBER_FUNCTION_HELPER2(classname, Function);														


#	define	IMPLEMENT_MEMBER_FUNCTION_VOID(classname, Function, Params, Params1)		\
	IMPLEMENT_MEMBER_FUNCTION_HELPER0(void, classname, Function, Params)				\
	IMPLEMENT_MEMBER_FUNCTION_HELPER_VOID(Function, Params1);							\
	IMPLEMENT_MEMBER_FUNCTION_HELPER2(classname, Function);														

#	define	IMPLEMENT_MEMBER_FUNCTION_EX(lRet, classname, Function, Params1, Params2)	\
	lRet __stdcall IGA_##classname::Function##Params1									\
{	return	m_p##classname->Function##Params2;	}


#	define	IGA_IMPLEMENT_CLASS(classname)												\
	VFT		IGA_##classname::s_VirtualFuncTable[MAX_VFT_SIZE]	= { 0 };	

#define		FILL_VIRTUAL_FUNC_TABLE(dwIndex, Function)									\
	ASSERT(dwIndex < MAX_VIRTUAL_FUNC_COUNT);											\
	m_pfn##Function	= (LPFN_##Function)(s_VirtualFuncTable[(DWORD)(dwIndex++)]);


#	define	INIT_VIRTUAL_OBJECT(pObject)												\
	CopyVFT(s_VirtualFuncTable, MAX_VFT_SIZE, CPVO2PIVO(pObject));						\
	ReplaceN_VFT(CPVO2PIVO(pObject), CPVO2PIVO(GetInstance()), MAX_VIRTUAL_FUNC_COUNT);


class IGA_IDirectInput8 :public IDirectInput8
{
public:
	IGA_IDirectInput8(IDirectInput8*  pIDirectInput8);
	~IGA_IDirectInput8();

	virtual HRESULT __stdcall  QueryInterface (REFIID riid, LPVOID FAR * ppvObj) ;
	virtual ULONG   __stdcall  AddRef  () ;
	virtual ULONG   __stdcall  Release () ;

	virtual HRESULT __stdcall CreateDevice(REFGUID, LPDIRECTINPUTDEVICE8A *, LPUNKNOWN);
	virtual HRESULT __stdcall EnumDevices(DWORD,LPDIENUMDEVICESCALLBACKA,LPVOID,DWORD);
	virtual HRESULT __stdcall GetDeviceStatus(REFGUID);
	virtual HRESULT __stdcall RunControlPanel(HWND,DWORD);
	virtual HRESULT __stdcall Initialize( HINSTANCE,DWORD);
	virtual HRESULT __stdcall FindDevice(REFGUID, LPCSTR, LPGUID );
	virtual HRESULT __stdcall EnumDevicesBySemantics(LPCSTR,LPDIACTIONFORMATA,LPDIENUMDEVICESBYSEMANTICSCBA,LPVOID,DWORD);
	virtual HRESULT __stdcall ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK,LPDICONFIGUREDEVICESPARAMSA,DWORD,LPVOID);
private:
	IDirectInput8 * m_pIDirectInput8;

};
//
IGA_DECLARE_CLASS_BEGIN( IDirectInputDevice8 )
	enum
	{
		MAX_VIRTUAL_FUNC_COUNT	= 32,
	};
	/*** IUnknown methods ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, QueryInterface, (REFIID riid, void** ppvObj), (IDirectInputDevice8 *, REFIID riid, void** ppvObj));
	DECLARE_MEMBER_FUNCTION_EX(ULONG,	AddRef, (void), (IDirectInputDevice8 *));
	DECLARE_MEMBER_FUNCTION_EX(ULONG,	Release, (void), (IDirectInputDevice8 *));

	/*** IDirectInputDevice8 methods ***/
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetCapabilities, (LPDIDEVCAPS), (IDirectInputDevice8 *, LPDIDEVCAPS) );
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, EnumObjects, ( LPDIENUMDEVICEOBJECTSCALLBACKA,LPVOID,DWORD), (IDirectInputDevice8 *,  LPDIENUMDEVICEOBJECTSCALLBACKA,LPVOID,DWORD) );
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetProperty, (REFGUID,LPDIPROPHEADER), (IDirectInputDevice8 *, REFGUID,LPDIPROPHEADER));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetProperty, (REFGUID,LPCDIPROPHEADER),(IDirectInputDevice8 *, REFGUID,LPCDIPROPHEADER));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Acquire, (void),(IDirectInputDevice8 *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Unacquire, (void),(IDirectInputDevice8 *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetDeviceState, (DWORD, LPVOID), (IDirectInputDevice8 *, DWORD, LPVOID));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetDeviceData, (DWORD, LPDIDEVICEOBJECTDATA, LPDWORD,DWORD),(IDirectInputDevice8 *, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetDataFormat, (LPCDIDATAFORMAT),(IDirectInputDevice8 *, LPCDIDATAFORMAT));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetEventNotification,(HANDLE), (IDirectInputDevice8 *, HANDLE));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetCooperativeLevel, (HWND, DWORD),(IDirectInputDevice8 *, HWND, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetObjectInfo, (LPDIDEVICEOBJECTINSTANCE, DWORD, DWORD),(IDirectInputDevice8 *, LPDIDEVICEOBJECTINSTANCE, DWORD, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetDeviceInfo, (LPDIDEVICEINSTANCE), (IDirectInputDevice8 *, LPDIDEVICEINSTANCE));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, RunControlPanel, (HWND, DWORD),(IDirectInputDevice8 *, HWND, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Initialize, (HINSTANCE, DWORD, REFGUID ), (IDirectInputDevice8 *, HINSTANCE, DWORD, REFGUID));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, CreateEffect, (REFGUID, LPCDIEFFECT, LPDIRECTINPUTEFFECT *, LPUNKNOWN), (IDirectInputDevice8 *, REFGUID, LPCDIEFFECT, LPDIRECTINPUTEFFECT*, LPUNKNOWN));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, EnumEffects, (LPDIENUMEFFECTSCALLBACK, LPVOID, DWORD),(IDirectInputDevice8 *, LPDIENUMEFFECTSCALLBACK, LPVOID, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetEffectInfo, (LPDIEFFECTINFO, REFGUID), (IDirectInputDevice8 *, LPDIEFFECTINFO, REFGUID));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetForceFeedbackState,(LPDWORD), (IDirectInputDevice8 *, LPDWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SendForceFeedbackCommand, (DWORD), (IDirectInputDevice8 *, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, EnumCreatedEffectObjects, (LPDIENUMCREATEDEFFECTOBJECTSCALLBACK, LPVOID, DWORD), (IDirectInputDevice8 *, LPDIENUMCREATEDEFFECTOBJECTSCALLBACK, LPVOID, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Escape, (LPDIEFFESCAPE), (IDirectInputDevice8 *, LPDIEFFESCAPE ));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, Poll, (void), (IDirectInputDevice8 *));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SendDeviceData, (DWORD, LPCDIDEVICEOBJECTDATA, LPDWORD, DWORD),(IDirectInputDevice8*, DWORD, LPCDIDEVICEOBJECTDATA, LPVOID, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, EnumEffectsInFile, (LPCTSTR, LPDIENUMEFFECTSINFILECALLBACK, LPVOID,DWORD), (IDirectInputDevice8 *, LPCTSTR, LPDIENUMEFFECTSINFILECALLBACK, LPVOID, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, WriteEffectToFile, (LPCSTR, DWORD, LPDIFILEEFFECT, DWORD),(IDirectInputDevice8 *, LPCSTR, DWORD, LPDIFILEEFFECT, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, BuildActionMap, (LPDIACTIONFORMATA, LPCTSTR, DWORD), (IDirectInputDevice8 *, LPDIACTIONFORMATA, LPCSTR, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, SetActionMap, (LPDIACTIONFORMATA, LPCTSTR, DWORD),(IDirectInputDevice8 *, LPDIACTIONFORMATA, LPCSTR, DWORD));
	DECLARE_MEMBER_FUNCTION_EX(HRESULT, GetImageInfo, (LPDIDEVICEIMAGEINFOHEADER), (IDirectInputDevice8 *, LPDIDEVICEIMAGEINFOHEADER));


IGA_DECLARE_CLASS_END();




