#include "StdAfx.h"
#include <WinSock2.h>
#include "z_fileini.h"
#include "Ad_3DFilter.h"
#include "GlobalAPI_Val.h"
#include "IGAWin.h"
#include "AdCoreApp.h"
#include "D3DHelpFun.h"

BOOL CheckIP()
{
	return g_bRightIP;
}

BOOL  g_bInUserLock = TRUE;

HWND  m_hWndDevice = NULL;		//设备对应的窗口句柄，把这个传给3D模块

////////////////////////////////////////////////////////////////
//D3D对象的若干处理函数
////////////////////////////////////////////////////////////////

IGA_Filter_IDirect3D::IGA_Filter_IDirect3D()
{
    m_dwRefCount = 0;
}

IGA_Filter_IDirect3D::~IGA_Filter_IDirect3D()
{
}

//------------------------------------------------------------
//卸载窗体模块、卸载3d模块、上传关注度  和 玩家信息
//------------------------------------------------------------
void  ReleaseAllResource()
{
	//
	//卸载窗体模块
	//
	IGAWIN::CloseFlash()       ;
	IGAWIN::CloseVideo()       ;
	IGAWIN::CloseActiveFlash() ;
	IGAWIN::UnInitWindow()     ;
	
	if(g_nDebug>1)
		LOG <<"D3D强制卸载窗体模块. " << "\r\n";
	
	//
	//卸载3d模块
	//
	if ( NULL != g_pIGA3DDestoryt)
	{
		(*(FUN_IGA3DDestory )g_pIGA3DDestoryt)();
		UnInit3DCore();	
		if(g_nDebug>1)
			LOG <<"D3D强制卸载3D模块. " << "\r\n";
	}	
	
	//
	//上传关注度  和 玩家信息 	
	//
	
	if(CheckIP())
	{
		if(g_nDebug>1)
		{
			LOG << "设备释放前上传关注度  和 玩家信息" << "\r\n";
			LOG << "ip 有效" << "\r\n";
		}
		
		KAdCoreApp* p = CreateADApp() ;
		p->UploadFocus()     ;
		p->UploadPlayInfo()  ;
	}
	else
	{
		if(g_nDebug>1)
			LOG << "上传关注: 未连接" << "\r\n";
	}

}

//------------------------------------------------------------
//完成在IGA_Filter_IDirect3DSurface::OnUnlockRect_Before中未进行的纹理层替换
//------------------------------------------------------------
BOOL CompleteMultiTextureReplace(IDirect3DDevice* pDevice)
{
	KAdCoreApp* pApp = CreateADApp();
	for ( KADManger::AD_MAP_IT  it = pApp->m_adManager.m_ads.begin(); it != pApp->m_adManager.m_ads.end(); it++)
	{
		if  ( it->second->type != AD_TEXTURE ) 
			continue;
		
		KTextureAD*  pTextureAD = (KTextureAD*)it->second;  
      	if(!pTextureAD)				
			continue;

		KTextureAD::DEV_MAP_IT  pDev =  pTextureAD->devs.find(pDevice);
		if ( pDev == pTextureAD->devs.end() ) 	
			continue;
		
		TextureADContent&   ad = pDev->second;
		if  ( ad.m_pMexture == NULL || ad.m_pADTexture == NULL || ad.bAllReplace)
			continue;

		
		DWORD  dwLevelCount = ((IDirect3DTexture*)(ad.m_pMexture))->GetLevelCount();
		
		for ( DWORD dwLevel = 0 ;dwLevel < dwLevelCount ;dwLevel++)
		{						
			if ( IGA_D3D_HELP::Texture_DoReplace((IDirect3DTexture*)(ad.m_pADTexture),(IDirect3DTexture*)(ad.m_pMexture),dwLevel) )
			{
				START_LOG;
				LOG << "完全替换成功  level = " << dwLevel   << " ID= " << pTextureAD->m_dwAdID;
				END_LOG;
			}
			else
			{
				ad.m_pMexture = NULL;
				
				START_LOG;
				LOG << "完全替换失败 level=" << dwLevel  << " ID= " << pTextureAD->m_dwAdID;
				END_LOG;
				
				//把该广告位设置到m_dwNeedReplaceTexture
				KADManger  & adManager = pApp->m_adManager;
				if (  adManager.m_dwNeedReplaceTexture.IsCreate(pDevice) )
				{ 
					adManager.m_dwNeedReplaceTexture.d_array [ pDevice ].insert(pTextureAD->m_dwAdID );					
					ad.bAllReplace  = TRUE;
					break;
				}
				
			}
			ad.bAllReplace  = TRUE;
		}					
	}
	return TRUE;
}
VOID    IGA_Filter_IDirect3D::OnRelease_After      (ULONG  ulRefCount)
{
	try
	{
		if(g_nDebug>1)
		{
			START_LOG;
			LOG << "IDirect3D::OnRelease:   count= " << ulRefCount<< "\r\n";
		}
	
		if ( 0 == ulRefCount )
		{	
			if(g_nDebug>1)
				END_LOG;
			
			return;
		}

		if ( g_dwD3DDeviceCount != 0 )
		{
			////卸载窗体模块、卸载3d模块、上传关注度  和 玩家信息
			ReleaseAllResource();

			g_dwD3DDeviceCount = 0;
		}

		if(g_nDebug>1)
			END_LOG;

	}catch(...)
	{
		START_LOG;
		LOG << "OnRelease_After Exception"  ;
		END_LOG;

		if(g_nDebug>0)
			OutputDebugString("OnRelease_After Exception");
	}

}
VOID    IGA_Filter_IDirect3D::OnCreateDevice_After (IDirect3DDevice*  pDevice,HWND hFocusWindow,IDirect3DDevice* pDeviceOrg)
{
	try
	{
	BOOL  bRet  = FALSE;

	if(g_nDebug>1)
	{
		START_LOG;
		LOG <<"IDirect3D::OnCreateDevice  [DEVICE]=" << (DWORD)pDevice   << "\r\n";
	}

	//
	//
	//
	IGA_D3D_HELP::SetDeviceRefCount(pDevice,0);		
	
	//
	//加载窗体模块
	//
	bRet = IGAWIN::InitWindow(CallBackPlayTimeFocus);
	if(g_nDebug>1)
		LOG <<"加载窗体模块 ：ret="   << (bRet ? "TRUE"  : "FALSE") << "\r\n";	

	//
	//加载本地XML中所有广告位，创建相应统计数据结构
	//
	KAdCoreApp* pApp = CreateADApp();
    pApp->LoadAD2Device(pDevice);   

	if(g_nDebug>1)
		LOG << "加载本地广告文件" << "\r\n";
	
	//
	//加载3d模块	
	//
	bRet = Init3DCore();
	if(g_nDebug>1)
		LOG <<"加载3d模块 ：ret="   << (bRet ? "TRUE"  : "FALSE") << "\r\n";

 	if ( NULL != g_pFunIGA3DInit)
 	{
		m_hWndDevice = hFocusWindow;
 		g_dw3DRanderPos = (*(FUN_IGA3DINIT)g_pFunIGA3DInit)(hFocusWindow,pDeviceOrg,pDevice,CallBack3DFocus,CallBackDeviceRefCount);
 	}	
	
	g_dwD3DDeviceCount++;

	if(g_nDebug>1)
		END_LOG;
	}catch(...)
	{
		START_LOG;
		LOG << "OnCreateDevice_After Exception"  ;
		END_LOG;
		
		if(g_nDebug>0)
			OutputDebugString("OnCreateDevice_After Exception");
	}
}

////////////////////////////////////////////////////////////////
//设备对象的若干处理函数
////////////////////////////////////////////////////////////////

IGA_Filter_IDirect3DDevice::IGA_Filter_IDirect3DDevice()
{

}
IGA_Filter_IDirect3DDevice::~IGA_Filter_IDirect3DDevice()
{

}

VOID IGA_Filter_IDirect3DDevice::OnRelease_After                 (IDirect3DDevice* pDevice,ULONG&  ulRefCount )
{	
	try
	{
 	IGA_D3D_HELP::DeviceReleaseLock(pDevice);

	int nRefCount = IGA_D3D_HELP::GetDeviceRefCount(pDevice);

	if(g_nDebug>1)
	{
		START_LOG;
		char szTmp[300];
		wsprintf(szTmp,"IDirect3DDevice::OnRelease   [DC]%d [RC]%d [DEVICE]%08X\r\n",
			ulRefCount,nRefCount,pDevice);
		LOG << szTmp ;
	}

	if ( nRefCount == ulRefCount )
	{	
		////卸载窗体模块、卸载3d模块、上传关注度  和 玩家信息
		ReleaseAllResource();

		//删除此设备创建的资源
		if(g_nDebug>1)
			LOG << "卸载广告文件" << "\r\n";

		//
		//调用m_adManager.Clear();
		//
		CreateADApp()->DeleteAdFromDevice(pDevice);

		g_dwD3DDeviceCount--;
		ulRefCount = 0;				
	}

	if(g_nDebug>1)
		END_LOG;

 	IGA_D3D_HELP::DeviceReleaseUnLock(pDevice);	
	
	}catch(...)
	{
		START_LOG;
		LOG << "OnRelease_After异常"  ;
		END_LOG;

		//
		//异常情况，把设备数据置零
		//
		g_dwD3DDeviceCount=0;
		ulRefCount = 0;	
		OutputDebugString("OnRelease_After Exception");
	}
}

//翻转前后
VOID IGA_Filter_IDirect3DDevice::OnPresent_Before                (IDirect3DDevice* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	if ( NULL !=g_pIGA3DRender && g_dw3DRanderPos == Per_Present)
	{
		(*(FUN_IGA3DRender)g_pIGA3DRender)();
	}
}
VOID IGA_Filter_IDirect3DDevice::OnPresent_After                 (IDirect3DDevice* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	if ( NULL != g_pSetFristDraw)
	{	
		(*(FUN_ISetFristDraw )g_pSetFristDraw)();
	}
}

//重设设备前后
VOID IGA_Filter_IDirect3DDevice::OnReset_Before                  (IDirect3DDevice* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if( g_pIGA3DDestoryt!= NULL )
	{
		(*(FUN_IGA3DDestory )g_pIGA3DDestoryt)();
		g_dw3DRanderPos = -1;
	}
}
VOID IGA_Filter_IDirect3DDevice::OnReset_After                   (IDirect3DDevice* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice* pOrgDevice)
{	
	if( NULL != g_pFunIGA3DInit )
	{
		g_dw3DRanderPos = (*(FUN_IGA3DINIT)g_pFunIGA3DInit)(m_hWndDevice,pOrgDevice,pDevice,CallBack3DFocus,CallBackDeviceRefCount);
	}
}

//设置转置矩阵前
VOID IGA_Filter_IDirect3DDevice::OnSetTransform_Before           (IDirect3DDevice* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{	
	if ( NULL != g_pIGA3DGetMatrixData)
	{		
		(*(FUN_IGA3DGetMatrixData)g_pIGA3DGetMatrixData)( State, pMatrix );
	}

	if(  NULL !=g_pIGA3DRender && Per_SetTransform == g_dw3DRanderPos)
	{
		if ( D3DTS_WORLD == State )
		{
			(*(FUN_IGA3DRender)g_pIGA3DRender)();
		}
	}
	
}

//设置转置矩阵后
VOID IGA_Filter_IDirect3DDevice::OnSetTransform_After           (IDirect3DDevice* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{	
	if( NULL !=g_pIGA3DRender && Per_SetTransform == g_dw3DRanderPos )
	{
		
		if ( D3DTS_WORLD == State)
		{
			(*(FUN_IGA3DRender)g_pIGA3DRender)();
		}
	}
}

VOID IGA_Filter_IDirect3DDevice::OnSetTexture_Before             (IDirect3DDevice* pDevice, DWORD Stage, IDirect3DBaseTexture* pTexture)
{
	if  ( NULL == pTexture)  
	{
		return;
	}
	
	try
	{
	KAdCoreApp* pApp = CreateADApp();

	//是否存在需要完全替换的纹理
    if  (  pApp->m_adManager.m_dwReplaceADTextureCount )
	{
		//完成在表面解锁前中未进行的纹理层替换
		CompleteMultiTextureReplace(pDevice);
		pApp->m_adManager.m_dwReplaceADTextureCount --;
	}
   
	//
	// 此设备下是否有需要进行动态替换的纹理
	//
	KADManger  & adManager = pApp->m_adManager;
	if ( ! adManager.m_dwNeedReplaceTexture.IsCreate(pDevice) )  
		return;
	
	//需要进行动态纹理替换的所有广告位ID，和此设备相关的
	KADManger::AD_WAIT_MATCH_ID&  waitID = adManager.m_dwNeedReplaceTexture.d_array[pDevice];
	for ( KADManger::AD_WAIT_MATCH_ID_IT it0 = waitID.begin() ; it0  != waitID.end(); it0++)
	{
		DWORD   dwADID = *it0;

		/*
		广告ID不能为零或者负值
		*/
		if  ( dwADID ==0)
		{
			START_LOG;
			LOG << __FILE__ << " " << __LINE__ << " ID:" << dwADID  ;
			END_LOG;
			//waitID.erase(it0);
			continue;
		}

		//
		//根据ID找纹理其他信息
		//
		KADManger::AD_MAP_IT  it = CreateADApp()->m_adManager.m_ads.find(dwADID);
		if  ( it == CreateADApp()->m_adManager.m_ads.end() )
		{
			START_LOG;
			LOG << __FILE__ << " " << __LINE__ << " ID:" << dwADID  ;
			END_LOG;
			//waitID.erase(it0);
			continue;
		}

		//
		//确认此纹理是否存在当前设备
		//
		KTextureAD*  pTextureAD = (KTextureAD*)it->second;        		
		KTextureAD::DEV_MAP_IT  pDev =  pTextureAD->devs.find(pDevice);
		if ( pDev == pTextureAD->devs.end() ) 
		{
			START_LOG;
			LOG << __FILE__ << " " << __LINE__ << " ID:" << dwADID  ;
			END_LOG;
			continue;
		}

		TextureADContent&   ad = pDev->second;			 
		
		IDirect3DTexture*  pMatchTex = (IDirect3DTexture*)ad.m_pMatchTexture ;
		IDirect3DTexture*  pADTex    = (IDirect3DTexture*)ad.m_pADTexture ;

		//
		//确认源目标纹理指针有效
		//
		if (  NULL == pMatchTex ||  NULL == pADTex )
		{
			START_LOG;
			LOG << __FILE__ << " " << __LINE__ << " ID:" << dwADID  ;
			END_LOG;
			//waitID.erase(it0);
			continue;
		}
		
		//
		//比较
		//
		g_bInUserLock = FALSE;
		if ( IGA_D3D_HELP::Texture_DoCompare(pMatchTex,(IDirect3DTexture*)pTexture,0))
		{	

			START_LOG;
            LOG << "REPLACE  id=" << pTextureAD->m_dwAdID << pTextureAD->m_MatchFileName;
            END_LOG;

			for ( int i =0; i < pTexture->GetLevelCount();i++ )
				IGA_D3D_HELP::Texture_DoReplace(pADTex,(IDirect3DTexture*)pTexture,i);					


			//
			//设置播放起始时间
			//
			pTextureAD->m_pFocus->dwStartPalyTime = ::GetTickCount();
			
			//设置曝光量
			pTextureAD->m_pFocus->dwShowCount++;
			
			ad.m_pMexture = pTexture;
			waitID.erase(it0);
			g_bInUserLock = TRUE;
			break;
		}
		g_bInUserLock = TRUE;					
	}

	}catch(...)
	{
 		START_LOG;
 		LOG << "OnSetTexture_Before Exception"  ;
 		END_LOG;
		OutputDebugString("OnSetTexture_Before");
	}

}

////////////////////////////////////////////////////////////////
//纹理COM对象的若干处理函数
////////////////////////////////////////////////////////////////

IGA_Filter_IDirect3DTexture::IGA_Filter_IDirect3DTexture(){}
IGA_Filter_IDirect3DTexture::~IGA_Filter_IDirect3DTexture(){}

VOID    IGA_Filter_IDirect3DTexture::OnRelease_After      (IDirect3DDevice*pDevice,IDirect3DTexture*  pTexture,ULONG  ulRefCount       )
{	
	//
	//还有引用，不处理
	//
	if ( ulRefCount != 0 ) 
		return ;

	try
	{
		//
		//如果正在加载纹理，不处理
		//
		if  (  CreateADApp()->m_adManager.IsLoadingTexture() ) 
			return;

		//查找加载的广告是否加载了此纹理
		KADManger::AD_MAP&  adMap = CreateADApp()->m_adManager.m_ads;    
		for (KADManger::AD_MAP_IT  it = adMap.begin() ; it != adMap.end() ;it++ )
		{
			//
			//只处理纹理类型
			//
			if  ( it->second->type != AD_TEXTURE) 
				continue;

			KTextureAD*  pTextureAD = (KTextureAD*)it->second;        
			KTextureAD::DEV_MAP_IT  pDev =  pTextureAD->devs.find(pDevice);

			//
			//没有找到此设备的数据
			//
			if ( pDev == pTextureAD->devs.end() )
				continue;

			//
			//比较纹理指针
			//
			if  ( pTexture != pDev->second.m_pMexture)
				continue;

			//
			//日志处理
			//
			if(g_nDebug>1)
			{
 				START_LOG;
                 LOG << _T("OnRelease_After   [ADID] = ") << pTextureAD->m_dwAdID    << _T("\r\n");
 				END_LOG;
			}

			//
			//计算加载时长
			//
			DWORD dwTime = ::GetTickCount();
			if(dwTime > it->second->m_pFocus->dwStartPalyTime)
				it->second->m_pFocus->dwPlayTime = dwTime - (it->second->m_pFocus->dwStartPalyTime); 
			else
				it->second->m_pFocus->dwPlayTime = 0xFFFFFFFF - (it->second->m_pFocus->dwStartPalyTime) + dwTime+1; 
			
			pDev->second.m_pMexture = NULL;

			//
			//此广告重新插入，等待进行替换
			//
			KAdCoreApp* pApp       = CreateADApp();
			KADManger  & adManager = pApp->m_adManager;
			adManager.m_dwNeedReplaceTexture.d_array[pDevice].insert(pTextureAD->m_dwAdID) ; 
		
	}

	}catch(...)
	{
		START_LOG;
		LOG << "OnRelease_After Exception"  ;
		END_LOG;

		if(g_nDebug>0)
			OutputDebugString("OnRelease_After");
	}
}

////////////////////////////////////////////////////////////////
//表面COM对象的若干处理函数
////////////////////////////////////////////////////////////////

IGA_Filter_IDirect3DSurface::IGA_Filter_IDirect3DSurface(){}
IGA_Filter_IDirect3DSurface::~IGA_Filter_IDirect3DSurface(){}
VOID    IGA_Filter_IDirect3DSurface::OnLockRect_After (IDirect3DSurface* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) 
{	
	try{

	if  (  !g_bInUserLock)  return;
    if  (  CreateADApp()->m_adManager.IsLoadingTexture() ) return;
	if  ( Flags & D3DLOCK_READONLY )   return;
	if  ( pRect != NULL)               return;	
	
	//
	//在表面解锁前需要这个表面的参数
	//
	SURFACE_PARAM   param={pLockedRect->Pitch, pLockedRect->pBits ,pRect,Flags};
	m_lockSurface.insert( CUR_SURFACE_MAP::value_type(pSurface,param));

	}catch(...)
	{
		START_LOG;
		LOG << "OnLockRect_After异常"  ;
		END_LOG;
		OutputDebugString("OnLockRect_After异常");
	}
}

VOID    IGA_Filter_IDirect3DSurface::OnUnlockRect_Before(IDirect3DDevice* pDevice,IDirect3DSurface* pSurface) 
{
	if  (  !g_bInUserLock)  return;
	static   BOOL   bReplacing = FALSE;
	if  ( bReplacing )   return;
	try
	{

	//是否正在替换中

	m_pTexture = NULL ;

	//查找是否有效的表面
	CUR_SURFACE_MAP_IT  it1 = m_lockSurface.find(pSurface);
	if  ( it1 == m_lockSurface.end() )	return;

    SURFACE_PARAM  param = (*it1).second;
	bReplacing = TRUE;	
	
	IDirect3DSurface*      pAdSurface    = NULL;
	IDirect3DSurface*      pMatchSurface = NULL;
	D3DLOCKED_RECT         LockedRect   ;
	D3DSURFACE_DESC        SurfaceDesc  ;
	D3DSURFACE_DESC        MatchSurfaceDesc ;

	//LOG << " 开始表面比较";
	KAdCoreApp*         pApp  =    CreateADApp();	
	KADManger::AD_MAP&  adMap = pApp->m_adManager.m_ads;  	


	for (KADManger::AD_MAP_IT  it = adMap.begin() ; it != adMap.end() ;it++ )
	{		
		if  ( it->second->type != AD_TEXTURE) continue;

		//查找广告图中是否有此设备的
		KTextureAD*  pTextureAD = (KTextureAD*)it->second;        		
		KTextureAD::DEV_MAP_IT  pDev =  pTextureAD->devs.find(pDevice);
        if ( pDev == pTextureAD->devs.end() )  continue;
		

		TextureADContent&   ad = pDev->second;
		//判断此广告是否已经被替换
	    if ( ad.m_pMexture != NULL )  continue;

		//判断是否存在有效的纹理		
		if (  NULL == ad.m_pADTexture ||  NULL == ad.m_pMatchTexture )  continue;
		IDirect3DTexture*  pMatchTex = (IDirect3DTexture*)ad.m_pMatchTexture ;
		IDirect3DTexture*  pADTex    = (IDirect3DTexture*)ad.m_pADTexture ;


		//取需要匹配的广告纹理表面
		
		DWORD  dwLevelCount = pMatchTex->GetLevelCount();
		dwLevelCount =1 ;

		for( DWORD  dwLevel = 0; dwLevel < dwLevelCount ; dwLevel++)
		{
			//取待匹配的纹理表面
			pMatchTex->GetSurfaceLevel(dwLevel,&pMatchSurface);	

			LockRectRaw(pMatchSurface,&LockedRect,NULL,D3DLOCK_READONLY);		

			//比较Pitch
			if ( LockedRect.Pitch   !=  param.Pitch ) 
			{
				UnlockRectRaw(pMatchSurface);
				pMatchSurface->Release();	
				continue;
			}
			
			//
			//比较格式
			//
			pMatchTex->GetLevelDesc(dwLevel,&MatchSurfaceDesc);
			HRESULT hr = pSurface->GetDesc(&SurfaceDesc);
			
			if ( SurfaceDesc.Format != MatchSurfaceDesc.Format )
			{
				UnlockRectRaw(pMatchSurface);
				pMatchSurface->Release();	
				continue;
			}

			//
			//宽、高
			//
            if ( SurfaceDesc.Height > MatchSurfaceDesc.Height 
				|| SurfaceDesc.Width > MatchSurfaceDesc.Width 
				||  SurfaceDesc.Width == 0 
				|| SurfaceDesc.Height == 0)
			{
				UnlockRectRaw(pMatchSurface);
		        pMatchSurface->Release();	
				break;
			}

			if ( SurfaceDesc.Height != MatchSurfaceDesc.Height || SurfaceDesc.Width != MatchSurfaceDesc.Width )
			{
				UnlockRectRaw(pMatchSurface);
				pMatchSurface->Release();	
				continue;
			}

			//
			//表面数据比较						
			//
            BOOL  bRet = IGA_D3D_HELP::Surface_DoCompare(param.pBits, LockedRect.pBits, param.Pitch, &SurfaceDesc,FALSE);
			if (! bRet )
			{
				UnlockRectRaw(pMatchSurface);
				pMatchSurface->Release();
				continue;
			}
			

			pADTex->GetSurfaceLevel(dwLevel,&pAdSurface);

			LockRectRaw(pAdSurface,&LockedRect,NULL,D3DLOCK_READONLY);																					
			
			bRet = IGA_D3D_HELP::Surface_DoReplace((CHAR*)(param.pBits),(CHAR*)(LockedRect.pBits),param.Pitch,&SurfaceDesc);							
			
			UnlockRectRaw(pAdSurface);
			pAdSurface->Release();

			
			if  ( !bRet )
			{
				START_LOG;
				LOG << "初始化替换失败"  << pTextureAD->m_dwAdID;
				END_LOG;

				UnlockRectRaw(pMatchSurface);
				pMatchSurface->Release();	
				break;
			}

								
			//保存当前表面指针
			ad.m_pSurface = pSurface;	

			//
			//获得表面对应的纹理
			//
			void *pContainer = NULL;
			IDirect3DTexture *pTexture = NULL;
			HRESULT hr1 = pSurface->GetContainer(__IID_IDirect3DTexture, &pContainer);
			if (SUCCEEDED(hr1) && pContainer)
			{
				pTexture = (IDirect3DTexture *)pContainer;
				pTexture->Release();
			}
			ad.m_pMexture = pTexture;
			ad.bAllReplace = FALSE;

			//
			//设置播放起始时间
			//
			pTextureAD->m_pFocus->dwStartPalyTime = ::GetTickCount();

			//设置曝光量
			pTextureAD->m_pFocus->dwShowCount++;


			UnlockRectRaw(pMatchSurface);
			pMatchSurface->Release();

			//完全替换的数量
			pApp->m_adManager.m_dwReplaceADTextureCount++;
        
			//去除动态替换
			KADManger::AD_WAIT_MATCH_ID&  waitID = pApp->m_adManager.m_dwNeedReplaceTexture.d_array[pDevice];
			waitID.erase(pTextureAD->m_dwAdID);

			START_LOG;
			LOG << "初始化替换成功"  << pTextureAD->m_dwAdID;
			END_LOG;
			break;
		}
	}		
	
	bReplacing = FALSE;
	m_lockSurface.erase(it1);

	}
	catch(...)
	{
		bReplacing = FALSE;
		START_LOG;
		LOG << "IGA_Filter_IDirect3DSurface::OnUnlockRect_Before异常"  ;
		END_LOG;
		OutputDebugString("OnUnlockRect_Before");
	}
}

