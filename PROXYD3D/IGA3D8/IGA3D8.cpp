// IGA3D.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "tchar.h"
#include "z_FileIni.h"
#include "3D_Core.h"
#include "IGA3D8.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

LPDIRECT3DDEVICE8 g_pDirect3DDevice8 = NULL;

DWORD IGA3DInit( LPDIRECT3DDEVICE8 pd3dDeviceOrig, LPCallBack3DFocus lpfocus, LPCallBackDeviceRefCount lpRefCount )
{

	g_IGA3DModule.m_pd3dDevice = pd3dDeviceOrig;
	g_pDirect3DDevice8 = pd3dDeviceOrig;
	int draw_pos = g_IGA3DModule.Init();
	g_IGA3DModule.m_iDrawPos = draw_pos;
	g_IGA3DModule.m_pFun3DFocus = lpfocus;
	g_IGA3DModule.m_pDeviceRefCount = lpRefCount;

	return draw_pos;
}

void IGA3DDestory()
{
	g_IGA3DModule.Destory();
}

void IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix )
{
	if( pMatrix == NULL )
	{
		return;
	}
	if( D3DTS_WORLD == State )
	{
		//OutputDebugString("D3DTS_WORLD == State");
		memcpy( &g_IGA3DModule.m_matWorld, pMatrix, sizeof(D3DXMATRIX) );
		g_IGA3DModule.m_bInitWorld = true;
	}

	if( (D3DTS_VIEW == State)  && (! g_IGA3DModule.m_bInitView ) )
	{
		memcpy( &g_IGA3DModule.m_matView, pMatrix, sizeof(D3DXMATRIX) );
		g_IGA3DModule.m_bInitView = true;
	}
	if( (D3DTS_PROJECTION == State) && ( !g_IGA3DModule.m_bInitProj ) )
	{
		/*
		《舞街区》的代码,这个游戏支持3D模型的显示、点击，并且支持裁剪，因此能够记录模型的显示时间（单位：秒）；在更新和游戏退出的时候，能够将显示数据保存到文件中。
		*/
		int game_id = g_IGA3DModule.m_enumGameID;

		if( game_id == IGA_WUJIEQU				//《舞街区》
			|| game_id == IGA_WUJIEQU_MLXY			//舞街区(马来西亚)
			|| game_id == IGA_WUJIEQU_XJP			//舞街区(新加坡)
			|| game_id == IGA_WUJIEQU_FLB)			//舞街区(菲律宾)
		{
			if((pMatrix->_22 / pMatrix->_11 - 1.0f) > 0.0f && pMatrix->_33 >= 1.0f)
			{
				memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
				g_IGA3DModule.m_bInitProj = true;
			}
		}

		/*
		《天尊》的代码,可以显示模型，但不能点击,不支持裁剪。
		注意：这个游戏显示3D模型会总是这挡在游戏界面上，并不和游戏内部模型在同一空间
		显示上会有偏差。
		*/
		else if( g_IGA3DModule.m_enumGameID == IGA_TIANZUN )
		{
			if( pMatrix->_44 == 0.0f )	
			{
				if((pMatrix->_22 / pMatrix->_11 - 1.0f) > 0.0f && fabs(pMatrix->_33) >= 1.0f)
				{
					memcpy(&g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DMATRIX));
					g_IGA3DModule.m_bInitProj = true;
				}
			}			
		}
		/*
		未知的游戏,这是默认的情况，为了那些带测试的游戏，对于需要具体对待的游戏，上面的代码已经做了具体解释。
		当然，以后经测试后的游戏，也需要按名称ID添加到上面的代码里，并把支持情况说明。
		*/
		else
		{
			memcpy(&g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DMATRIX));
			g_IGA3DModule.m_bInitProj = true;
		}
	}
}

void IGA3DRender()
{
	if( g_IGA3DModule.m_bFrist == FALSE )
	{
		return;
	}
	g_IGA3DModule.m_bFrist = FALSE;
	g_IGA3DModule.IGA3DInputState();
	g_IGA3DModule.PerIGA3DRender();
	g_IGA3DModule.RunScript();
	g_IGA3DModule.OnIGA3DRender();
	g_IGA3DModule.PostIGA3DRender();
}

void SetFristDraw()
{
	g_IGA3DModule.m_bFrist		= TRUE;
	g_IGA3DModule.m_bInitView	= false;
	g_IGA3DModule.m_bInitProj	= false;
	g_IGA3DModule.m_bInitWorld	= false;
}

void SetUpdate()
{
	g_IGA3DModule.m_bUpdate = true;
}

void GetModulePath( char fullPathName[MAX_PATH] )
{
	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *p;
	p = strrchr(TmpPath,'\\');
	if(!p)
		return ;
	*(p+1) = 0;
	char path[MAX_PATH];
	sprintf(path,"%s%s",TmpPath,fullPathName );
	strcpy(fullPathName, path);
}
