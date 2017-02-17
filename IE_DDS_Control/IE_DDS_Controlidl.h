

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Sat Oct 11 13:06:36 2008
 */
/* Compiler settings for .\IE_DDS_Control.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __IE_DDS_Controlidl_h__
#define __IE_DDS_Controlidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DIE_DDS_Control_FWD_DEFINED__
#define ___DIE_DDS_Control_FWD_DEFINED__
typedef interface _DIE_DDS_Control _DIE_DDS_Control;
#endif 	/* ___DIE_DDS_Control_FWD_DEFINED__ */


#ifndef ___DIE_DDS_ControlEvents_FWD_DEFINED__
#define ___DIE_DDS_ControlEvents_FWD_DEFINED__
typedef interface _DIE_DDS_ControlEvents _DIE_DDS_ControlEvents;
#endif 	/* ___DIE_DDS_ControlEvents_FWD_DEFINED__ */


#ifndef __IE_DDS_Control_FWD_DEFINED__
#define __IE_DDS_Control_FWD_DEFINED__

#ifdef __cplusplus
typedef class IE_DDS_Control IE_DDS_Control;
#else
typedef struct IE_DDS_Control IE_DDS_Control;
#endif /* __cplusplus */

#endif 	/* __IE_DDS_Control_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __IE_DDS_ControlLib_LIBRARY_DEFINED__
#define __IE_DDS_ControlLib_LIBRARY_DEFINED__

/* library IE_DDS_ControlLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_IE_DDS_ControlLib;

#ifndef ___DIE_DDS_Control_DISPINTERFACE_DEFINED__
#define ___DIE_DDS_Control_DISPINTERFACE_DEFINED__

/* dispinterface _DIE_DDS_Control */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DIE_DDS_Control;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A43097A4-DC8C-4527-A914-110FB605C70B")
    _DIE_DDS_Control : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DIE_DDS_ControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DIE_DDS_Control * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DIE_DDS_Control * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DIE_DDS_Control * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DIE_DDS_Control * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DIE_DDS_Control * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DIE_DDS_Control * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DIE_DDS_Control * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DIE_DDS_ControlVtbl;

    interface _DIE_DDS_Control
    {
        CONST_VTBL struct _DIE_DDS_ControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DIE_DDS_Control_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DIE_DDS_Control_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DIE_DDS_Control_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DIE_DDS_Control_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DIE_DDS_Control_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DIE_DDS_Control_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DIE_DDS_Control_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DIE_DDS_Control_DISPINTERFACE_DEFINED__ */


#ifndef ___DIE_DDS_ControlEvents_DISPINTERFACE_DEFINED__
#define ___DIE_DDS_ControlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DIE_DDS_ControlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DIE_DDS_ControlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B650314E-C5F7-488F-91CD-1B03FBD44F8D")
    _DIE_DDS_ControlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DIE_DDS_ControlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DIE_DDS_ControlEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DIE_DDS_ControlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DIE_DDS_ControlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DIE_DDS_ControlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DIE_DDS_ControlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DIE_DDS_ControlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DIE_DDS_ControlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DIE_DDS_ControlEventsVtbl;

    interface _DIE_DDS_ControlEvents
    {
        CONST_VTBL struct _DIE_DDS_ControlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DIE_DDS_ControlEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DIE_DDS_ControlEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DIE_DDS_ControlEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DIE_DDS_ControlEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DIE_DDS_ControlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DIE_DDS_ControlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DIE_DDS_ControlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DIE_DDS_ControlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_IE_DDS_Control;

#ifdef __cplusplus

class DECLSPEC_UUID("F49A71F8-7732-49ED-BDBB-9D9B8CFBFEE5")
IE_DDS_Control;
#endif
#endif /* __IE_DDS_ControlLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


