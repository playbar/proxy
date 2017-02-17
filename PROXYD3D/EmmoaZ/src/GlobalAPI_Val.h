//************************************************************************************************************
// class      :  
// purpose    :  动态加载的api函数指针,和一些全局变量
// Date       :  2008-8-5  10:38:25
// author     :  3o3afe1
//************************************************************************************************************
#ifndef _GLOBALAPI_VAL_H
#define _GLOBALAPI_VAL_H


/*  IGA3D8.dll   IGA3D9.dll 函数指针  */

#if defined(USING_D3D9)
	#include <d3d9.h>

	#define     _3D_CORE_NAME               _T("IGA3D9.dll")
	#define     _3D_CORE_IGA3DINIT	        _T("IGA3DInit")
	#define     _3D_CORE_IGA3DDESTORY	    _T("IGA3DDestory")
	#define     _3D_CORE_IGA3DGETMATRIXDATA	_T("IGA3DGetMatrixData")
	#define     _3D_CORE_IGA3DRENDE	        _T("IGA3DRender")
	#define     _3D_CORE_SETFRISTDRAW	    _T("SetFristDraw")
    #define     _3D_CORE_SETUPDATE  	    _T("SetUpdate")

	typedef DWORD (*FUN_IGA3DINIT)(HWND, LPDIRECT3DDEVICE9 ,LPDIRECT3DDEVICE9,VOID*,VOID* );
	typedef void  (*FUN_IGA3DGetMatrixData)( D3DTRANSFORMSTATETYPE ,CONST D3DMATRIX*  );
	typedef void  (*FUN_IGA3DRender)(   );
	typedef void  (*FUN_IGA3DDestory)(   );
	typedef void  (*FUN_ISetFristDraw)(   );    
	typedef void  (*FUN_ISetFristDraw)(   );   
	typedef void  (*FUN_SetUpdate)();
	
#elif defined(USING_D3D8)
	#include <d3d8.h>

	#define     _3D_CORE_NAME               _T("IGA3D8.dll")
	#define     _3D_CORE_IGA3DINIT	        _T("IGA3DInit")
	#define     _3D_CORE_IGA3DDESTORY	    _T("IGA3DDestory")
	#define     _3D_CORE_IGA3DGETMATRIXDATA	_T("IGA3DGetMatrixData")
	#define     _3D_CORE_IGA3DRENDE	        _T("IGA3DRender")
	#define     _3D_CORE_SETFRISTDRAW	    _T("SetFristDraw")
	#define     _3D_CORE_SETUPDATE  	    _T("SetUpdate")


	typedef DWORD (*FUN_IGA3DINIT)( HWND,LPDIRECT3DDEVICE8  ,LPDIRECT3DDEVICE8,VOID*,VOID*);
	typedef void  (*FUN_IGA3DGetMatrixData)( D3DTRANSFORMSTATETYPE ,CONST D3DMATRIX*  );
	typedef void  (*FUN_IGA3DRender)(   );
	typedef void  (*FUN_IGA3DDestory)(   );
	typedef void  (*FUN_ISetFristDraw)(   );
    typedef void  (*FUN_SetUpdate)();
	

    


#elif defined (USING_DDRAW)

#elif defined(USING_OPENGL)

#else
   #pragma error("未定义接管类型宏")
#endif



extern VOID*       g_pFunIGA3DInit          ;
extern VOID*       g_pIGA3DDestoryt         ;
extern VOID*       g_pIGA3DGetMatrixData    ;
extern VOID*       g_pIGA3DRender           ;
extern VOID*       g_pSetFristDraw          ;
extern HINSTANCE   g_h3DCore                ;
extern DWORD       g_dw3DRanderPos          ;
extern VOID*       g_pSetUpdate             ;
enum RenderPos
{
	Per_SetTransform,	//设置矩阵前
	Post_SetTransform,	//设置矩阵后
	Per_Present,		//翻转前
	Post_Present		//翻转后
};
extern BOOL   Init3DCore();
extern VOID   UnInit3DCore();


// 当前的d3d对象数量
// extern  DWORD  g_dwD3DCount ;
// 当前的d3d设备数量
extern  DWORD		g_dwD3DDeviceCount;
extern  BOOL		g_bRightIP	;
extern	int			g_nDebug;
// 
// 
/*  emmoaw.iaf  函数指针 */


#endif