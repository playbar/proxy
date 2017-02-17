#include "stdafx.h"

CFace::CFace()
{
	m_pd3dDevice = NULL;
	m_pVB		 = NULL;
	m_pTex		 = NULL;
}

CFace::~CFace()
{
	Destory();
}

HRESULT CFace::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	HRESULT hr = S_OK;

	if( pd3dDevice == NULL )
	{
		return E_FAIL;
	}

	m_pd3dDevice = pd3dDevice;
	
	//创建顶点缓冲区
    if( FAILED( m_pd3dDevice->CreateVertexBuffer( 4*sizeof(_vf),
                                                 0, D3D_FVF,
                                                 D3DPOOL_MANAGED, &m_pVB ) ) )
    {
        return E_FAIL;
    }

	//顶点数据
    _vf g_Vertices[] =
    {
		{ 10.0f,  10.0f,   0.5f, 1.0f, 0.0f, 0.0f },   
	    { 10.0f,  300.0f,  0.5f, 1.0f, 0.0f, 1.0f },	
	    { 300.0f, 10.0f,   0.5f, 1.0f, 1.0f, 0.0f },	
	    { 300.0f, 300.0f,  0.5f, 1.0f, 1.0f, 1.0f }
    };

    //填充顶点缓冲区
    _vf* pVertices;
    if( FAILED( m_pVB->Lock( 0, sizeof(g_Vertices), (BYTE**)&pVertices, 0 ) ) )
	{
        return E_FAIL;
	}
    memcpy( pVertices, g_Vertices, sizeof(g_Vertices) );
    m_pVB->Unlock();

	ZeroMemory( &m_Material, sizeof(m_Material) );
	m_Material.Diffuse.a = 1.0f;
	m_Material.Diffuse.r = m_Material.Diffuse.g = m_Material.Diffuse.b = 0.7f;
	m_Material.Ambient	= m_Material.Diffuse; 

	return hr;
}

HRESULT CFace::SetPosition( int x, int y, int size_x, int size_y )
{
	//顶点数据
    _vf g_Vertices[] =
    {
		{ 10.0f,  10.0f,   0.5f, 1.0f, 0.0f, 0.0f },   
	    { 10.0f,  300.0f,  0.5f, 1.0f, 0.0f, 1.0f },	
	    { 300.0f, 10.0f,   0.5f, 1.0f, 1.0f, 0.0f },	
	    { 300.0f, 300.0f,  0.5f, 1.0f, 1.0f, 1.0f }
    };
	
	g_Vertices[0].x = x*1.0f;
	g_Vertices[0].y = y*1.0f;

	g_Vertices[1].x = x*1.0f;
	g_Vertices[1].y = y*1.0f + size_y;

	g_Vertices[2].x = x*1.0f + size_x;
	g_Vertices[2].y = y*1.0f;

	g_Vertices[3].x = x*1.0f + size_x;
	g_Vertices[3].y = y*1.0f + size_y;


    //填充顶点缓冲区
    _vf* pVertices;
    if( FAILED( m_pVB->Lock( 0, sizeof(g_Vertices), (BYTE**)&pVertices, 0 ) ) )
	{
        return E_FAIL;
	}
    memcpy( pVertices, g_Vertices, sizeof(g_Vertices) );
    m_pVB->Unlock();
	
    return S_OK;
}

void CFace::Render()
{
	m_pd3dDevice->SetTexture( 0, m_pTex );
	m_pd3dDevice->SetMaterial(&m_Material);
	m_pd3dDevice->SetVertexShader(D3D_FVF);
	m_pd3dDevice->SetStreamSource(0, m_pVB,sizeof(_vf));
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,  2);
}

void CFace::Destory()
{
	if ( m_pVB )
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

void CFace::SetTexture( LPDIRECT3DTEXTURE8 pTexture )
{
	m_pTex = pTexture;
}

CFace* GetCFace()
{
	static CFace face;
	return &face;
}

//-------------------------------------------------------------------
//UI纹理库
CUITexture::CUITexture()
{
	m_pd3dDevice = NULL;
}

CUITexture::~CUITexture()
{
	Destory();
}

HRESULT CUITexture::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;

	m_pNode		= NULL;
	m_pTexID	= NULL;
	m_pErrorNode = NULL;

	//进行一次清空
	//OutputDebugString("CUITexture：纹理库的清空....");
	Lost();
	m_TexIDLibrary.clear();
	m_ErrorID.clear();

	char path[256];
	sprintf( path, "%s%s",PATH_WORK,"adv_res\\" );

	GetAbsPath( path );

	m_Pack.Init( "texUI", path );
	m_Pack.LoadDataRootFromFile();	//加载数据包目录

//#ifdef USE_IGA3D_TOOL
//	int i = 0;
//	while(1)
//	{
//		AddResource( i );
//
//		m_pErrorNode = m_ErrorID.find(i);
//		if( m_pErrorNode != m_ErrorID.end() )
//		{
//			GetAdTool()->SetMaxTexID(i);
//			break;
//		}
//		i++;	
//	}
//#endif

	return hr;
}

LPDIRECT3DTEXTURE8 CUITexture::GetTexture( DWORD id )
{
	//判断是否是不存在的纹理
	m_pErrorNode = m_ErrorID.find(id);
	if( m_pErrorNode != m_ErrorID.end() )
	{
		return NULL;
	}

	//在库中寻找
	m_pNode = m_UITexLibrary.find(id);
	if( m_pNode == m_UITexLibrary.end() )
	{
		AddResource( id );
		return NULL;
	}

	return m_pNode->second;
}

void CUITexture::Destory()
{
	Lost();
	m_TexIDLibrary.clear();
	m_ErrorID.clear();
}

void CUITexture::Lost()
{
	OutputDebugString("CUITexture::Lost()");
	for( m_pNode = m_UITexLibrary.begin(); m_pNode != m_UITexLibrary.end(); m_pNode++ )
	{
		DWORD id = m_pNode->first;
		m_TexIDLibrary.push_back(id);

		if( m_pNode->second != NULL )
		{
			m_pTex = m_pNode->second;
			//m_pTex->Release();
			m_pNode->second = NULL;
		}
	}
	m_UITexLibrary.clear();
}

void CUITexture::Resume()
{
	for( m_pTexID = m_TexIDLibrary.begin(); m_pTexID != m_TexIDLibrary.end(); m_pTexID++ )
	{
		DWORD	id = *m_pTexID;
		AddResource( id );	
	}
	m_TexIDLibrary.clear();
}

void CUITexture::AddResource( DWORD id )
{
	//根据id加载纹理资源
	char	fileName[256];
	
	sprintf( fileName, "texUI_%d.png", id );

	//从数据文件加载网格模型
	char	*tmp_data = NULL;
	DWORD	size     = 0x0;
	m_Pack.GetDataFromPack( fileName, tmp_data, &size );
	if( tmp_data == NULL || size == 0x0 )
	{
		if( tmp_data != NULL )
		{
			delete tmp_data;
			tmp_data = NULL;
		}
		OutputDebugString("CUITexture: 从数据包中获得纹理数据失败！");
		m_ErrorID.insert(id);
		return;
	}

	LPDIRECT3DTEXTURE8 pTex;
	if( FAILED( D3DXCreateTextureFromFileInMemory( m_pd3dDevice, tmp_data, size, &pTex ) ) )
    {
		if( tmp_data != NULL )
		{
			delete tmp_data;
		}

		m_ErrorID.insert(id);
		OutputDebugString("CUITexture: 没有找到纹理....");
		return;
    }
	delete tmp_data;
	tmp_data = NULL;

	m_UITexLibrary.insert( UI_TEX_NODE( id, pTex ) );
}

void CUITexture::ReMove( DWORD id )
{

}

CUITexture* GetUITexture()
{
	static CUITexture UI_Tex;
	return &UI_Tex;
}

//----------------------------------------------------------------
//UI
CUI::CUI()
{
	m_pd3dDevice = NULL;
}

CUI::~CUI()
{

}

HRESULT CUI::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	HRESULT hr = S_OK;
	
	if( pd3dDevice == NULL )
	{
		return E_FAIL;
	}

	m_pd3dDevice = pd3dDevice;

	GetCFace()->Init( m_pd3dDevice );
	
	GetUITexture()->Init( m_pd3dDevice );

	return hr;
}

void CUI::Render()
{
	//OutputDebugString("绘制UI...");
	GetCFace()->Render();

}

void CUI::Lost()
{

}

void CUI::Resume()
{
	GetCFace()->Init( m_pd3dDevice );
	GetUITexture()->Resume();
}

CUI* GetUI()
{
	static CUI ui;
	return &ui;
}

//---------------------------------------------------------------
//对话框
CUIDialog::CUIDialog()
{
	GetCFace()->SetPosition( 10, 120, 280, 440 );
}

CUIDialog::~CUIDialog()
{


}

DWORD CUIDialog::Render( POINT* pPoint, DWORD id )
{
	LPDIRECT3DTEXTURE8 pTex = GetUITexture()->GetTexture(id);
	//if( pTex == NULL )
	//{
	//	return 2;
	//}
	GetCFace()->SetTexture(pTex);

	GetCFace()->Render();

	if( GetIGA3DModule()->InputState[b_LBUTTON] )
	{
		if( pPoint->x > 10 && pPoint->x < 280 && pPoint->y > 120 && pPoint->y < 440 )
		{
			if( pPoint->x > 50 && pPoint->x < 240 && pPoint->y > 160 && pPoint->y < 400 )
			{
				return 1;	//确定
			}
			else
			{
				return 2;	//确定
			}
		}
	}

	return 0;	//没有任何事件的时候
}

DWORD ShowDlg( POINT* pPoint, DWORD id )
{
	static CUIDialog dlg;
	return dlg.Render( pPoint, id );
}