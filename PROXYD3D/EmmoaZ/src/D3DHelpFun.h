#ifndef _D3DHELP_FUN_H
#define _D3DHELP_FUN_H

#ifdef  USING_D3D9
    #include <d3d9.h>
    #include <d3dx9.h>
	#define     _D3DX_LIB_NAME    _T("D3DX9_29.dll")
    typedef IDirect3D9          IDirect3D;
	typedef IDirect3DDevice9    IDirect3DDevice;
	typedef IDirect3DTexture9   IDirect3DTexture;
	typedef IDirect3DSurface9   IDirect3DSurface;
	typedef LPDIRECT3DDEVICE9   LPDIRECT3DDEVICE;
	typedef IDirect3DBaseTexture9  IDirect3DBaseTexture;
	
	const GUID __IID_IDirect3DTexture = {0xe4cdd575, 0x2866, 0x4f01, {0xb1, 0x2e, 0x7e, 0xec, 0xe1, 0xec, 0x93, 0x58}};
    
#else if USING_D3D8
	#include <d3dx8.h>
	#define     _D3DX_LIB_NAME    _T("D3DX8.dll")
	typedef IDirect3D8          IDirect3D;
	typedef IDirect3DDevice8    IDirect3DDevice;
	typedef IDirect3DTexture8   IDirect3DTexture;	
	typedef IDirect3DSurface8   IDirect3DSurface;
	typedef LPDIRECT3DDEVICE8   LPDIRECT3DDEVICE;
	typedef IDirect3DBaseTexture8  IDirect3DBaseTexture;

	
	const GUID __IID_IDirect3DTexture = {0xe4cdd575, 0x2866, 0x4f01, {0xb1, 0x2e, 0x7e, 0xec, 0xe1, 0xec, 0x93, 0x58}};
							
	
#endif


namespace   IGA_D3D_HELP
{
	//表面数据的比较替换
	BOOL Surface_DoCompare(const void *pSource, const void *pTarget, UINT nPicth, D3DSURFACE_DESC* pDesc,BOOL  bDebug = FALSE);
	BOOL Surface_DoReplace( CHAR *pTarget, const CHAR *pSource, UINT nPicth, D3DSURFACE_DESC* pDesc);

	//纹理比较替换
	BOOL Texture_DoCompare( IDirect3DTexture* pTexture1,IDirect3DTexture* pTexture2,UINT  uLevel);
	BOOL Texture_DoReplace( IDirect3DTexture* pTexture1,IDirect3DTexture* pTexture2,UINT  uLevel);

	//创建纹理对象
	BOOL CreateTexture(LPDIRECT3DDEVICE pDevice,LPCTSTR lpszFileName,IDirect3DTexture** ppTexture);
	BOOL CreateTexture(LPDIRECT3DDEVICE pDevice,LPCTSTR lpszFileName,IDirect3DTexture** ppTexture ,IDirect3DTexture*  pRefTexture);


	//保存纹理到文件
	BOOL  SaveTextureToFile(IDirect3DTexture* pTexture,LPCTSTR   lpszFileName);
	//保存表面到文件
	BOOL  SaveSurfaceToFile(IDirect3DSurface* pSurface,LPCTSTR   lpszFileName);


	//是否正在加载纹理
	BOOL  IsCreatingTexture();
	//初始化d3d扩展库
	BOOL  InitD3Dx();
	VOID  UnInitD3Dx();


	/*设备引用计数*/
	VOID  AddDeviceRefCount(LPDIRECT3DDEVICE pDevice, long  lCount);
	VOID  DelDeviceRefCount(LPDIRECT3DDEVICE pDevice );

	VOID  SetDeviceRefCount(LPDIRECT3DDEVICE pDevice, long  dwCount);	
	long  GetDeviceRefCount( LPDIRECT3DDEVICE pDevice);

	//改变设备引用的函数调用前后添加计算更改的引用计数
// 	DWORD BeginUpdate ( LPDIRECT3DDEVICE pDevice);
//     VOID  EndUpdate   ( LPDIRECT3DDEVICE pDevice,DWORD  dwCount );

	//设备 release是的锁和解锁
	VOID  DeviceReleaseLock    (LPDIRECT3DDEVICE pDevice);
	VOID  DeviceReleaseUnLock  (LPDIRECT3DDEVICE pDevice);
}

#endif