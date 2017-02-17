#include "stdafx.h"

#include "BasePlay.h"
#include "TexturePaly.h"
#include "IGATexture.h"

#define PATH_WORK  "InterAdtive\\EMMOA\\3D\\"
#define DNOTPALY	0

CIGATexture g_CIGATexture;

CIGATexture::CIGATexture()
{
	m_pD3DDevice	= NULL;
	m_pD3DTexture	= NULL;
}

CIGATexture::~CIGATexture()
{
	if (NULL != m_pD3DDevice )
	{
		m_pD3DDevice = NULL;
	}
}

HRESULT CIGATexture::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	HRESULT hr = S_OK;
	m_pD3DDevice = pd3dDevice;

	if ( ! m_mapTexLibrary.empty() )
	{
		m_mapTexLibrary.clear();
	}
	if ( ! m_listTexIDLibrary.empty() )
	{
		m_listTexIDLibrary.clear();
	}
	if ( ! m_setErrorID.empty() )
	{
		m_setErrorID.clear();
	}

	return hr;
}

void CIGATexture::LostDevice()
{
	for( m_mapIterator = m_mapTexLibrary.begin(); m_mapIterator != m_mapTexLibrary.end(); m_mapIterator++ )
	{
		DWORD	id = m_mapIterator->first;
		m_listTexIDLibrary.push_back(id);

		if( m_mapIterator->second != NULL )
		{
			m_mapIterator->second->Destroy();
			delete m_mapIterator->second;
			m_mapIterator->second = NULL;
		}
	}
	if ( ! m_mapTexLibrary.empty() )
	{
		m_mapTexLibrary.clear();
	}

}

void CIGATexture::ResumeDevice()
{
	for( m_listIterator = m_listTexIDLibrary.begin(); m_listIterator != m_listTexIDLibrary.end(); m_listIterator++ )
	{
		DWORD id = * m_listIterator;
		AddResource( id );	
	}
	//if ( !m_listTexIDLibrary.empty() )
	//{
	//	m_listTexIDLibrary.clear();
	//}
}

LPDIRECT3DTEXTURE8 CIGATexture::GetTexture( DWORD id, bool* bIsPlayEnd, float* w, float* h )
{
	OutputDebugString("Begin CIGATexture::GetTexture");
	if( id < 0 )
	{
		return NULL;
	}

	m_setIterator = m_setErrorID.find(id);
	if( m_setIterator != m_setErrorID.end() )
	{
		return NULL;
	}

	//在看看是否在当前库中
	m_mapIterator = m_mapTexLibrary.find( id );
	if( m_mapIterator == m_mapTexLibrary.end() )
	{
		AddResource( id );
		return NULL;
	}
	//否则绘制这个网格对象
	m_pD3DTexture = m_mapIterator->second->GetTexture();

	*bIsPlayEnd = m_mapIterator->second->m_bIsPlayEnd;

	if ( * bIsPlayEnd)
	{
		m_mapIterator->second->StartPlay();
	}
	
	//if( w != NULL && h != NULL )
	//{
	//	m_pNode->second->GetSize( w, h );
	//}

	OutputDebugString("End CIGATexture::GetTexture");
	return m_pD3DTexture;
}

void CIGATexture::AddResource( DWORD dwID )
{
	//OutputDebugString("CIGATexture::AddResource");
	char szTextureFileName[MAX_PATH];
	char szTexturePathFileName[MAX_PATH];
	char szIniFileName[MAX_PATH];
	char szFilePath[MAX_PATH];
	GetModulePath(szFilePath);
	sprintf(szFilePath, "%s%s", szFilePath, PATH_WORK );
	sprintf(szIniFileName, "%sdata.ini", szFilePath );
	OutputDebugString(szIniFileName);
	char szID[10];
	sprintf(szID, "%d", dwID );

	int iIsPaly = 0;
	iIsPaly = GetPrivateProfileInt(szID, "IsPaly", 0, szIniFileName);
	if (  DNOTPALY == iIsPaly)
	{
		return;
	}
	//OutputDebugString("VideoFileName");

	GetPrivateProfileString(szID,"VideoFileName","", szTextureFileName, _MAX_PATH, szIniFileName);

	if ( strlen(szTextureFileName) == 0)
	{
		return;
	}
	sprintf(szTexturePathFileName, "%s%s", szFilePath, szTextureFileName);

	CIGATextureBase*	pIGATexture = NULL;
	char szExtensionName[10];
	char *pchTmp = strrchr(szTexturePathFileName, '.');
	if (pchTmp == NULL)
	{
		return;
	}
	strcpy(szExtensionName, pchTmp + 1 );

	if (strcmp(szExtensionName, "swf") == 0)
	{
		pIGATexture = new CIGAFlashTexture();
	}
	else if ( (strcmp( szExtensionName , "avi") == 0) ||
		(strcmp( szExtensionName , "mpg") == 0) || 
		(strcmp( szExtensionName,  "rm")  == 0 ))
	{
		pIGATexture = new CIGAVideoTexture();
	}
	else if ( (strcmp( szExtensionName , "jpg") == 0) ||
		(strcmp( szExtensionName , "dds") == 0) || 
		(strcmp( szExtensionName,  "bmp")  == 0 ))
	{
		pIGATexture = new CIGAStaticTexture();
	}


	if( pIGATexture == NULL )
	{
		return;
	}
	HRESULT hr = pIGATexture->Init( m_pD3DDevice, szTexturePathFileName );
	if( FAILED(hr) )
	{
		delete pIGATexture;
		m_setErrorID.insert(dwID);
		return;
	}
	m_mapTexLibrary.insert( MAP_TEXTURE_NODE(dwID, pIGATexture) );

}

void CIGATexture::RemoveResource( DWORD id )
{

}

void CIGATexture::ReleaseResource()
{
	LostDevice();
	m_listTexIDLibrary.clear();
	m_setErrorID.clear();
}


void CIGATexture::Destroy()
{

}

void CIGATexture::StartPlay()
{

}

void CIGATexture::StopPlay( DWORD dwID)
{
	if( dwID < 0 )
	{
		return;
	}

	m_setIterator = m_setErrorID.find(dwID);
	if( m_setIterator != m_setErrorID.end() )
	{
		return;
	}

	//在看看是否在当前库中
	m_mapIterator = m_mapTexLibrary.find( dwID );
	if( m_mapIterator == m_mapTexLibrary.end() )
	{
		return;
	}
	//否则绘制这个网格对象
	m_mapIterator->second->StopPlay();

}

void CIGATexture::Pause()
{

}

//------------------------------------------------------------------------
//
CIGAFlashTexture::CIGAFlashTexture()
{
}

CIGAFlashTexture::~CIGAFlashTexture()
{

}

HRESULT CIGAFlashTexture::Init(LPDIRECT3DDEVICE8 pd3dDevice, char* name)
{

	strcpy( m_szName, name);
	m_pd3dDevice = pd3dDevice;
	if( m_FlashSprite.StartFlash( pd3dDevice, name, 256, 256 ) )
	{
		m_bIsPlayEnd =false;
		return S_OK;
	}
	OutputDebugString(name);
	return E_FAIL;
}

LPDIRECT3DTEXTURE8 CIGAFlashTexture::GetTexture()
{

	if ( ! m_FlashSprite.IsPlaying() )
	{
		m_bIsPlayEnd = true;
	}

	m_FlashSprite.Update();
	m_pTexture = m_FlashSprite.GetRenderTarget();
	if( m_pTexture == NULL )
	{
		return NULL;
	}
	return m_pTexture;
}

void CIGAFlashTexture::GetSize( float* w, float* h )
{
	if( w != NULL )
	{
		*w = m_FlashSprite.m_nWidth;
	}

	if( h != NULL )
	{
		*h = m_FlashSprite.m_nHeight;
	}
}

void  CIGAFlashTexture::StopPlay()
{ 
	if (m_FlashSprite.IsPlaying() )
	{
		m_FlashSprite.SetLoopPlay(true );
		m_FlashSprite.Pause();
	}
}

void CIGAFlashTexture::StartPlay()
{
	m_FlashSprite.GotoFrame( 0 );
	//m_FlashSprite.Destory();
	// m_FlashSprite.StartFlash( m_pd3dDevice, m_szName, 256, 256 );
	 m_bIsPlayEnd =false;
}

void CIGAFlashTexture::Destroy()
{
	m_FlashSprite.SetLoopPlay(true );
	m_FlashSprite.Destory();
	m_bIsPlayEnd =false;
}

//------------------------------------------------------
//视屏纹理
CIGAVideoTexture::CIGAVideoTexture()
{

}

CIGAVideoTexture::~CIGAVideoTexture()
{

}

HRESULT CIGAVideoTexture::Init( LPDIRECT3DDEVICE8 pd3dDevice, char* name )
{
	HRESULT hr = m_VideoTexture.CreateFromAVIFile( pd3dDevice, name );
	if( FAILED(hr) )
	{
		return E_FAIL;
	}

	return S_OK;
}

LPDIRECT3DTEXTURE8  CIGAVideoTexture::GetTexture()
{
	m_VideoTexture.CheckForLoop();
	m_pTexture = m_VideoTexture.m_pTexture;

	m_bIsPlayEnd = m_VideoTexture.m_bStartPlay;

	if( m_bIsPlayEnd )
	{
		//m_VideoTexture.StartPlay();
	}

	return m_pTexture;
}

void CIGAVideoTexture::GetSize( float* w, float* h )
{
	if( w != NULL )
	{
		*w = m_VideoTexture.m_VideoWidth;
	}

	if( h != NULL )
	{
		*h = m_VideoTexture.m_VideoHeight;
	}
}

void CIGAVideoTexture::StartPlay()
{
	m_VideoTexture.StartPlay();
}

void CIGAVideoTexture::StopPlay()
{
	m_VideoTexture.Destroy();
}

void CIGAVideoTexture::Pause()
{
	m_VideoTexture.Pause();
}

void CIGAVideoTexture::Destroy()
{
	m_VideoTexture.Destroy();
}

//------------------------------------------------------
//静态纹理
CIGAStaticTexture::CIGAStaticTexture()
{

}

CIGAStaticTexture::~CIGAStaticTexture()
{

}

HRESULT CIGAStaticTexture::Init( LPDIRECT3DDEVICE8 pd3dDevice, char* name )
{
	HRESULT hr = D3DXCreateTextureFromFile( pd3dDevice, name, &m_pTexture );
	if( FAILED(hr) )
	{
		m_pTexture = NULL;
		return E_FAIL;
	}
	return S_OK;
}

LPDIRECT3DTEXTURE8 CIGAStaticTexture::GetTexture()
{
	return NULL;
}

void CIGAStaticTexture::Destroy()
{
	if( m_pTexture != NULL )
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

bool GetModulePath( char * szPathName )
{
	if (NULL == szPathName )
	{
		return false;
	}
	GetModuleFileName(NULL,szPathName,MAX_PATH);
	char *p = NULL;
	p = strrchr(szPathName,'\\');
	if( NULL == p )
		return false;
	*(p+1) = 0;
	return true;

}