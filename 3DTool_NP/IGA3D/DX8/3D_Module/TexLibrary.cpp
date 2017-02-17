#include "stdafx.h"

CTexLibrary* GetTexLibrary()
{
	static CTexLibrary TexLib;
	return &TexLib;
}

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

	m_TexLibrary.clear();
	m_pNode		= NULL;
	m_pTex		= NULL;

	m_TexIDLibrary.clear();
	m_pNodeID	= NULL;

	m_ErrorID.clear();
	m_pErrorNode = NULL;

	//OutputDebugString("加载纹理数据目录......");
	char path[256];
	sprintf( path, "%s%s",PATH_WORK,"adv_res\\" );

	GetAbsPath( path );

	m_Pack.Init("texture", path);
	m_Pack.LoadDataRootFromFile();	//加载数据包目录

#ifdef USE_IGA3D_TOOL
	int i = 0;
	while(1)
	{
		AddResource( i );

		m_pErrorNode = m_ErrorID.find(i);
		if( m_pErrorNode != m_ErrorID.end() )
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
	for( m_pNode = m_TexLibrary.begin(); m_pNode != m_TexLibrary.end(); m_pNode++ )
	{
		DWORD	id = m_pNode->first;
		m_TexIDLibrary.push_back(id);

		if( m_pNode->second != NULL )
		{
			delete m_pNode->second;
			m_pNode->second = NULL;
		}
	}

	m_TexLibrary.clear();
}

void CTexLibrary::Resume()
{
	for( m_pNodeID = m_TexIDLibrary.begin(); m_pNodeID != m_TexIDLibrary.end(); m_pNodeID++ )
	{
		DWORD	id = *m_pNodeID;
		AddResource( id );	
	}
	m_TexIDLibrary.clear();
}

LPDIRECT3DTEXTURE8 CTexLibrary::GetTexture( DWORD id )
{
	//先判断是不是没有的资源
	m_pErrorNode = m_ErrorID.find(id);
	if( m_pErrorNode != m_ErrorID.end() )
	{
		return NULL;
	}

	//在看看是否在当前库中
	m_pNode = m_TexLibrary.find( id );
	if( m_pNode == m_TexLibrary.end() )
	{
		//需要加载资源
		AddResource( id );
		return NULL;	//不绘制这个对象
	}

	//否则绘制这个网格对象
	m_pTex = m_pNode->second->GetTexture();
	
	return m_pTex;

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
		m_ErrorID.insert(id);
		return;
	}

	//把加载的网格放到MESH库中
	m_TexLibrary.insert( TEX_NODE(id, pTex) );
}

void CTexLibrary::ReMove( DWORD id )
{

}

void CTexLibrary::Destory()
{
	Lost();
	m_TexIDLibrary.clear();
	m_ErrorID.clear();
}
