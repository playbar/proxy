#pragma  once

#include <d3d8.h>
#include <d3dx8.h>

typedef void(*LPIGAFUN)(int id);

typedef VOID(WINAPI *LPCallBack3DFocus)(DWORD,LPVOID);
typedef VOID(WINAPI *LPCallBackDeviceRefCount)(VOID*,long,LPVOID);

DWORD	IGA3DInit( LPDIRECT3DDEVICE8 pd3dDeviceOrig, LPCallBack3DFocus lpfocus = NULL, LPCallBackDeviceRefCount lpRefCount = NULL );
void	IGA3DDestory();
void	IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix );
DWORD	SetRenderPos();
void	IGA3DRender();
void	SetFristDraw();
void	SetUpdate();	//这个函数的作用是通知系统重新加载资源

void GetModulePath( char fullPathName[MAX_PATH] );
void GetCameraPos();

extern LPDIRECT3DDEVICE8 g_pDirect3DDevice8;