#include "StdAfx.h"
#include "GlobalAPI_Val.h"




VOID*       g_pFunIGA3DInit          = NULL ;
VOID*       g_pIGA3DDestoryt         = NULL ;
VOID*       g_pIGA3DGetMatrixData    = NULL ;
VOID*       g_pIGA3DRender           = NULL ;
VOID*       g_pSetFristDraw          = NULL ;
VOID*       g_pSetUpdate             = NULL ;
HINSTANCE   g_h3DCore                = NULL ;
DWORD       g_dw3DRanderPos          ;

BOOL		g_bRightIP				 = FALSE;
int			g_nDebug				 =0;

BOOL   Init3DCore()
{
	if ( g_h3DCore == NULL )
	{			
		g_h3DCore                = ::LoadLibrary(_3D_CORE_NAME);
		if(g_h3DCore)
		{
			g_pFunIGA3DInit          = ::GetProcAddress(g_h3DCore, _3D_CORE_IGA3DINIT);
			g_pIGA3DDestoryt         = ::GetProcAddress(g_h3DCore, _3D_CORE_IGA3DDESTORY);
			g_pIGA3DGetMatrixData    = ::GetProcAddress(g_h3DCore, _3D_CORE_IGA3DGETMATRIXDATA);
			g_pIGA3DRender           = ::GetProcAddress(g_h3DCore, _3D_CORE_IGA3DRENDE);
			g_pSetFristDraw          = ::GetProcAddress(g_h3DCore, _3D_CORE_SETFRISTDRAW);
			g_pSetUpdate             = ::GetProcAddress(g_h3DCore, _3D_CORE_SETUPDATE);
			return TRUE;
		}
	}	
	return FALSE;
}
VOID   UnInit3DCore()
{
	if  ( NULL != g_h3DCore)
	{		
		g_pFunIGA3DInit       = NULL ; 
		g_pIGA3DDestoryt      = NULL ; 
		g_pIGA3DGetMatrixData = NULL ;
		g_pIGA3DRender        = NULL ;
		g_pSetFristDraw       = NULL ;
		g_pSetUpdate          = NULL ;
		::FreeLibrary(g_h3DCore);
		g_h3DCore             = NULL; 
	}
}

 
DWORD  g_dwD3DDeviceCount = 0 ;	//当前的d3d设备数量
