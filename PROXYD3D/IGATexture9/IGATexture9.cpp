/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : IGATexture9.cpp
*	功能描述：IGATextur9, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#include "stdafx.h"
#include "D3D9Texture.h"
#include "IGATexture9.h"
#include "GlobalFunction.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	return TRUE;
}


//HRESULT IGATextureInit( LPDIRECT3DDEVICE9 pd3dDevice )
//{
//	HRESULT hr = g_CIGATexture.Init(pd3dDevice);
//	if( FAILED(hr) )
//	{
//		//OutputDebugString("IGATextureInit");
//	}
//	return hr;
//}
//
//LPDIRECT3DTEXTURE9 IGAGetTexture( int id, bool* bIsPlayEnd , float* w, float* h )
//{
//	LPDIRECT3DTEXTURE9 pIGATex = g_CIGATexture.GetTexture(id, bIsPlayEnd, w, h );
//	if( pIGATex != NULL )
//	{
//		return pIGATex;
//	}
//
//	return NULL;
//}
//
//
//void IGATextureDestory()
//{
//	//GetTexPlay()->Stop();
//	//GetTexPlay()->Destroy();
//	//CBasePlay::GetPlayerInstance()->Destory();
//
//	g_CIGATexture.ReleaseResource();
//}
//
//int DrawPlay()
//{
//	//CBasePlay::GetPlayerInstance()->FrameMove();
//	return 0;//GetTexPlay()->ShowPlayer(0);
//}
//
//void PlayAVI( LPDIRECT3DDEVICE9 pd3dDevice, char* name )
//{
//	//CBasePlay::GetPlayerInstance()->PlayAVIFile( pd3dDevice, name );
//	//GetTexPlay()->PlayAVIFile(name);
//}
//
//void PlayFLASH( LPDIRECT3DDEVICE9 pd3dDevice, char* name )
//{
//	//CBasePlay::GetPlayerInstance()->PlayFlashFile( pd3dDevice, name );
//	//GetTexPlay()->PlayFlashFile(name);
//}
//
//void PausePlayer()
//{
//	//GetTexPlay()->Pause();
//}
//
//void StopPlayer( DWORD dwID)
//{
//	g_CIGATexture.StopPlay( dwID);
//	//g_CIGATexture.Destory();
//	//GetTexPlay()->Stop();
//}
//////////////////////////////////////////////////////////////////////////

HRESULT CD3D9ManageTexture::D3D9TextureInit(LPDIRECT3DDEVICE9 pd3dDevice,char * pszTextureFileName,int iWidth, int iHeight  )
{
	try
	{
		char szExtensionName[10];
		char *pchTmp = strrchr(pszTextureFileName, '.');

		if (pchTmp == NULL)
		{
			return S_FALSE;
		}
		strcpy(szExtensionName, pchTmp + 1 );

		if (stricmp(szExtensionName, "swf") == 0)
		{
			m_pCD3D9TextureBase = new CD3D9FlashTexture();
		}
		else if ( (stricmp( szExtensionName , "avi") == 0) ||
			(stricmp( szExtensionName , "mpg") == 0) || 
			(stricmp( szExtensionName,  "rm")  == 0 ))
		{
			m_pCD3D9TextureBase = new CD3D9VideoTexture();
		}
		else if ( (stricmp( szExtensionName , "jpg") == 0) ||
			(stricmp( szExtensionName , "dds") == 0) || 
			(stricmp( szExtensionName,  "bmp")  == 0 ))
		{
			m_pCD3D9TextureBase = new CIGAStaticTexture();
		}

		m_pCD3D9TextureBase->D3D9TextureInit(pd3dDevice, pszTextureFileName, iWidth, iHeight);

	}
	catch (...)
	{
		ExceptDispose(__FILE__, __LINE__ );
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CD3D9ManageTexture::D3D9ResetDevice(LPDIRECT3DDEVICE9 pd3dDevice)
{
	__try
	{
		if ( m_pCD3D9TextureBase == NULL )
		{
			return E_FAIL;
		}
		m_pCD3D9TextureBase->D3D9ResetDevice(pd3dDevice);

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
		return E_FAIL;
	}
	return S_OK;
};

HRESULT CD3D9ManageTexture::D3D9StartPlay()
{
	__try
	{
		if ( m_pCD3D9TextureBase == NULL )
		{
			return E_FAIL;
		}
		m_pCD3D9TextureBase->D3D9StartPlay();

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CD3D9ManageTexture::D3D9StopPlay()
{
	__try
	{
		if ( m_pCD3D9TextureBase == NULL )
		{
			return E_FAIL;
		}
		m_pCD3D9TextureBase->D3D9StopPlay();

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
		return E_FAIL;
	}
	return S_OK;
}

LPDIRECT3DTEXTURE9 CD3D9ManageTexture::D3D9GetTexture( )
{
	__try
	{
		if ( m_pCD3D9TextureBase == NULL )
		{
			return NULL;
		}
		m_pCD3D9TextureBase->D3D9GetTexture();

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
		return NULL;
	}

	return m_pCD3D9TextureBase->GetTexture();
}

HRESULT CD3D9ManageTexture::D3D9PausePlay()
{
	__try
	{
		if ( m_pCD3D9TextureBase == NULL )
		{
			return E_FAIL;
		}
		m_pCD3D9TextureBase->D3D9PausePlay();

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CD3D9ManageTexture::D3D9UnPausePlay()
{
	__try
	{
		if ( m_pCD3D9TextureBase == NULL )
		{
			return E_FAIL;
		}
		m_pCD3D9TextureBase->D3D9UnPausePlay();

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
		return E_FAIL;
	}
	return  S_OK;
}

HRESULT CD3D9ManageTexture::D3D9GotoFrame(int iFrame)
{
	__try
	{
		if ( m_pCD3D9TextureBase == NULL )
		{
			return E_FAIL;
		}
		m_pCD3D9TextureBase->D3D9GotoFrame( iFrame );

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CD3D9ManageTexture::D3D9GetCurrentFrame()
{
	__try
	{
		if ( m_pCD3D9TextureBase == NULL )
		{
			return E_FAIL;
		}
		m_pCD3D9TextureBase->D3D9GetCurrentFrame();
	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
		return E_FAIL;
	}
	return S_OK;
}
void CD3D9ManageTexture::D3D9TextureDestory()
{
	__try
	{
		if ( m_pCD3D9TextureBase == NULL )
		{
			return;
		}
		m_pCD3D9TextureBase->D3D9TextureDestory();
	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
	}
	return ;
}