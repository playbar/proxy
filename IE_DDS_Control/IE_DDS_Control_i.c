

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_IE_DDS_ControlLib,0x151E31EB,0xEFD8,0x4EF9,0x98,0x74,0x90,0x1A,0x91,0x53,0x56,0xED);


MIDL_DEFINE_GUID(IID, DIID__DIE_DDS_Control,0xA43097A4,0xDC8C,0x4527,0xA9,0x14,0x11,0x0F,0xB6,0x05,0xC7,0x0B);


MIDL_DEFINE_GUID(IID, DIID__DIE_DDS_ControlEvents,0xB650314E,0xC5F7,0x488F,0x91,0xCD,0x1B,0x03,0xFB,0xD4,0x4F,0x8D);


MIDL_DEFINE_GUID(CLSID, CLSID_IE_DDS_Control,0xF49A71F8,0x7732,0x49ED,0xBD,0xBB,0x9D,0x9B,0x8C,0xFB,0xFE,0xE5);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



