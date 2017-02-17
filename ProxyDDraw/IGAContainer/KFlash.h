//************************************************************************************************************
// class      :  
// purpose    :  显示Flash
// Date       :  2008-7-2  13:37:28
// author     :  3o3afe1
//************************************************************************************************************
#if !defined(AFX_KFLASH_H__B3F0F9C0_0613_4D1E_AF6D_5A17BC3FB8E6__INCLUDED_)
#define AFX_KFLASH_H__B3F0F9C0_0613_4D1E_AF6D_5A17BC3FB8E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Flash9f.tlh"
#include "KWindowContainer.h"


//事件接收器
class KFlashSink : public IGAFlash::_IShockwaveFlashEvents
{	
public:
	KFlashSink();
	virtual ~KFlashSink();
	
	//iunknow
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppv);
	ULONG   STDMETHODCALLTYPE AddRef();
	ULONG   STDMETHODCALLTYPE Release();
	
	// IDispatch method
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo);
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames,UINT cNames, LCID lcid,DISPID* rgDispId);	
	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,WORD wFlags, ::DISPPARAMS __RPC_FAR *pDispParams, VARIANT __RPC_FAR *pVarResult,::EXCEPINFO __RPC_FAR *pExcepInfo, UINT __RPC_FAR *puArgErr);
	
	//flash事件处理函数
    HRESULT OnReadyStateChange  (long newState );
    HRESULT OnProgress          (long percentDone );
    HRESULT FSCommand           (_bstr_t command,_bstr_t args );
    HRESULT FlashCall           (_bstr_t request );
private:
	DWORD   m_dwRefCount;
};

class KFlash : public KContainerCtl
{
public:
	KFlash();
	virtual ~KFlash();


	//创建函数
	virtual BOOL Create(HWND hParent);


	BOOL    Play(LPCTSTR lpszName,BOOL bLoopPaly);
	VOID    Stop();

public:
	CComQIPtr< IGAFlash::IShockwaveFlash>   IFlashPlayer; 	
private:
	KFlashSink     m_sink;
	DWORD          m_dwSinkCooker;
};






#endif // !defined(AFX_KFLASH_H__B3F0F9C0_0613_4D1E_AF6D_5A17BC3FB8E6__INCLUDED_)
