/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : IGA3D9.h
*	IGA3D9.dll导入文件定义
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：
/************************************************************************/

#pragma  once

#include "d3d9.h"
#include "d3dx9.h"


extern int g_bDubug;		//	全局变量，控制是否显示调试信息，从emmoa.ini文件中读取

//typedef void(*LPIGAFUN)(int id);
typedef VOID(WINAPI *LPCallBack3DFocus)(DWORD,LPVOID);
typedef VOID(WINAPI *LPCallBackDeviceRefCount)(VOID*,long,LPVOID);


DWORD IGA3DInit(HWND hGameWnd, LPDIRECT3DDEVICE9 pd3dDeviceOrig, LPDIRECT3DDEVICE9 pd3dDeviceProxy, LPCallBack3DFocus lpfocus, LPCallBackDeviceRefCount lpRefCount );
void IGA3DDestory();
void IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix );
void IGA3DRender();
void SetFristDraw();
void SetUpdate();


void GetCameraPos();

