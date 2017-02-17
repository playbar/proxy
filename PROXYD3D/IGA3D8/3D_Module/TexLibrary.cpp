#include "stdafx.h"
#include "define.h"

#include "3D_Core.h"
#include "tchar.h"
#include "z_FileIni.h"
#include "TexLibrary.h"

//CTexLibrary* GetTexLibrary()
//{
//	static CTexLibrary TexLib;
//	return &TexLib;
//}

//---------------------------------------------
//纹理类的实现
CTexture::CTexture()
{
	m_pTexture = NULL;
	m_pd3dDevice = NULL;
}

CTexture::~CTexture()
{
	if( m_pTexture != NULL )
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

HRESULT CTexture::Init( IDirect3DDevice8 *pd3dDevice, char* fileName, CPack* pPack  )
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;

	//从数据文件加载网格模型
	char	*tmp_data = NULL;
	DWORD	size     = 0x0;
	pPack->GetDataFromPack( fileName, tmp_data, &size );
	if( tmp_data == NULL || size == 0x0 )
	{
		if( tmp_data != NULL )
		{
			delete tmp_data;
			tmp_data = NULL;
		}
		OutputDebugString("从数据包中获得纹理数据失败！");
		return E_FAIL;
	}

	if( FAILED( D3DXCreateTextureFromFileInMemory( m_pd3dDevice, tmp_data, size,
													&m_pTexture ) ) )
	{
		if( tmp_data != NULL )
		{
			delete tmp_data;
		}

		m_pTexture = NULL;
		OutputDebugString("从内存中加载纹理数据失败！");
		return E_FAIL;
	}
	delete tmp_data;
	tmp_data = NULL;


	return hr;
}

LPDIRECT3DTEXTURE8 CTexture::GetTexture()
{
	return m_pTexture;
}

//------------------------------------------------------------------------------------

CTexLibrary::CTexLibrary()
{
	m_pd3dDevice = NULL;
}

CTexLibrary::~CTexLibrary()
{
	Destory();
}


HRESULT CTexLibrary::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;

	m_mapTexLibrary.clear();
	//m_pNode		= NULL;
	m_pDirect3DTexture		= NULL;

	m_listTexIDLibrary.clear();
	//m_pNodeID	= NULL;

	m_setErrorID.clear();
	//m_pErrorNode = NULL;

	//OutputDebugString("加载纹理数据目录......");
	char path[256];
	sprintf( path, "%s%s",PATH_WORK,"adv_res\\" );

	GetModulePath( path );

	m_Pack.Init("texture", path);
	m_Pack.LoadDataRootFromFile();	//加载数据包目录

#ifdef USE_IGA3D_TOOL
	int i = 0;
	while(1)
	{
		AddResource( i );

		m_setIterator = m_setErrorID.find(i);
		if( m_setIterator != m_setErrorID.end() )
		{
			GetAdTool()->SetMaxTexID(i);
			break;
		}
		i++;	
	}
#endif

	return hr;
}

void CTexLibrary::Lost()
{
	for( m_mapIterator = m_mapTexLibrary.begin(); m_mapIterator != m_mapTexLibrary.end(); m_mapIterator++ )
	{
		DWORD	id = m_mapIterator->first;
		m_listTexIDLibrary.push_back(id);

		if( m_mapIterator->second != NULL )
		{
			delete m_mapIterator->second;
			m_mapIterator->second = NULL;
		}
	}

	m_mapTexLibrary.clear();
}

void CTexLibrary::Resume()
{
	for( m_listIterator = m_listTexIDLibrary.begin(); m_listIterator != m_listTexIDLibrary.end(); m_listIterator++ )
	{
		DWORD	id = *m_listIterator;
		AddResource( id );	
	}
	m_listTexIDLibrary.clear();
}

LPDIRECT3DTEXTURE8 CTexLibrary::GetTexture( DWORD id )
{
	//先判断是不是没有的资源
	m_setIterator = m_setErrorID.find(id);
	if( m_setIterator != m_setErrorID.end() )
	{
		return NULL;
	}

	//在看看是否在当前库中
	m_mapIterator = m_mapTexLibrary.find( id );
	if( m_mapIterator == m_mapTexLibrary.end() )
	{
		//需要加载资源
		AddResource( id );
		return NULL;	//不绘制这个对象
	}

	//否则绘制这个网格对象
	m_pDirect3DTexture = m_mapIterator->second->GetTexture();
	
	return m_pDirect3DTexture;

}

void CTexLibrary::AddResource( DWORD id )
{
	//根据ID解析要加载的网格文件名称
	char	fileName[256];
	
	//加载这个资源
	CTexture*	pTex = new CTexture();
	if( pTex == NULL )
	{
		OutputDebugString("申请CMesh的空间出错！");
		return;
	}

	sprintf( fileName, "tex_%d.jpg", id );
	
	//加载纹理
	HRESULT hr = pTex->Init( m_pd3dDevice, fileName, &m_Pack );
	if( FAILED(hr) )
	{
		delete pTex;
		m_setErrorID.insert(id);
		return;
	}

	//把加载的网格放到MESH库中
	m_mapTexLibrary.insert( MAP_TEXTURE_NODE(id, pTex) );
}

void CTexLibrary::ReMove( DWORD id )
{

}

void CTexLibrary::Destory()
{
	Lost();
	m_listTexIDLibrary.clear();
	m_setErrorID.clear();
}
