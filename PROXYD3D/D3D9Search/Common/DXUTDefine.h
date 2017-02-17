
#pragma  once 
#include "DXUTmisc.h"

#define RELEASE_COM(ixPoint)	{ if(NULL != (ixPoint)) { (ixPoint)->Release(); (ixPoint) = NULL; } }

#ifndef SAFE_DELETE
#define SAFE_DELETE( T ) {if( T ) { delete T; T = NULL; }}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY( T ) {if( T ) { delete[] T; T = NULL; }}
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( T ) if( T ) T->Release()
#endif



#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = x; if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = x; }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
#endif
#endif

//////////////////////////////////////////////////////////////////////////

//在debug下显示输出信息宏,使用前要先定义存放字符串的数组
inline void InlineShowOutPutDebug(LPCSTR file, int line,  PCSTR expr) 
{
	TCHAR sz[2*MAX_PATH];
	wsprintf(sz, TEXT("File %s, line %d : %s"), file, line, expr );
	OutputDebugString(sz);
}

# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	define	SHOWOUTPUTDEBUG(sz) InlineShowOutPutDebug(__FILE__, __LINE__, #sz)
# else
#	define	SHOWOUTPUTDEBUG(sz)
# endif
//////////////////////////////////////////////////////////////////////////

