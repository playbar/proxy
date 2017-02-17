

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Feb 25 10:50:52 2009
 */
/* Compiler settings for .\MyIE.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __MyIE_i_h__
#define __MyIE_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMyIENSHandle_FWD_DEFINED__
#define __IMyIENSHandle_FWD_DEFINED__
typedef interface IMyIENSHandle IMyIENSHandle;
#endif 	/* __IMyIENSHandle_FWD_DEFINED__ */


#ifndef __MyIENSHandle_FWD_DEFINED__
#define __MyIENSHandle_FWD_DEFINED__

#ifdef __cplusplus
typedef class MyIENSHandle MyIENSHandle;
#else
typedef struct MyIENSHandle MyIENSHandle;
#endif /* __cplusplus */

#endif 	/* __MyIENSHandle_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IMyIENSHandle_INTERFACE_DEFINED__
#define __IMyIENSHandle_INTERFACE_DEFINED__

/* interface IMyIENSHandle */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMyIENSHandle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("25E81C73-A6C1-46B2-A83B-C05565643062")
    IMyIENSHandle : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IMyIENSHandleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMyIENSHandle * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMyIENSHandle * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMyIENSHandle * This);
        
        END_INTERFACE
    } IMyIENSHandleVtbl;

    interface IMyIENSHandle
    {
        CONST_VTBL struct IMyIENSHandleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMyIENSHandle_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMyIENSHandle_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMyIENSHandle_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMyIENSHandle_INTERFACE_DEFINED__ */



#ifndef __MyIELib_LIBRARY_DEFINED__
#define __MyIELib_LIBRARY_DEFINED__

/* library MyIELib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MyIELib;

EXTERN_C const CLSID CLSID_MyIENSHandle;

#ifdef __cplusplus

class DECLSPEC_UUID("B96C53BD-FADC-45C7-A72D-EB5230433A46")
MyIENSHandle;
#endif
#endif /* __MyIELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


