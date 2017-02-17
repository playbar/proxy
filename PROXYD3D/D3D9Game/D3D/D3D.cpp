

#include "D3D.h"

CMyD3D * g_pMyD3D;


bool CMyD3D::Init( HWND hWnd, WORD wWidth, WORD wHeight, bool bFullScreen )
{
	m_hWnd = hWnd;
	m_pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );
    if( g_pMyD3D == NULL )
		return false;

	//得到当前的显示模式
    D3DDISPLAYMODE d3ddm;
    if( FAILED( m_pD3D9->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
		return false;

    //建立一个存储设备设置的结构体 
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );

	RECT rect;
	GetClientRect( hWnd, &rect );
    d3dpp.Windowed = !bFullScreen;
	if( bFullScreen )
		d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	else
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = rect.right;
	d3dpp.BackBufferHeight = rect.bottom;
    d3dpp.BackBufferFormat = d3ddm.Format;//显示模式
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	//创建D3D设备
	//D3DCAPS9 d3dcaps;
	//m_pD3D9->GetDeviceCaps( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, &d3dcaps );

	/*if( d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{*/
		if( FAILED( m_pD3D9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
                                   D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &m_pDevice ) ) )
			return false;
	/*}
	else
	{
		if( FAILED( m_pD3D9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice ) ) )
			return false;
	}*/


	if( SUCCEEDED( m_pD3D9->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT5 ) ) )
		m_D3DFMT = D3DFMT_DXT5;
	else if( SUCCEEDED( m_pD3D9->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A1R5G5B5, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT5 ) ) )
		m_D3DFMT = D3DFMT_DXT1;
	else
		m_D3DFMT = D3DFMT_UNKNOWN;

	//设置渲染时的剔出模式
    m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	//m_pDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB( 255, 255, 255 ) );

	m_pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

	//设置填充模式
	m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID ); 

	m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,D3DTOP_BLENDTEXTUREALPHA );	//使用透明贴图

	m_pDevice->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );


	//font
	for( int i = 0; i < 10; i ++ )
	{
		m_pVFont[i] = new CVFont;
	}
	m_pVFont[0]->CreateVFont( 16, 16, L"隶书" );//D3DXCreateFont(m_pDevice,16,0,0,0,0,GB2312_CHARSET,0,0,0,L"隶书",&m_pFont[0]); 
	m_pVFont[1]->CreateVFont( 16, 16, L"宋体" );//D3DXCreateFont(m_pDevice,16,0,0,0,0,GB2312_CHARSET,0,0,0,L"宋体",&m_pFont[1]); 
	m_pVFont[2]->CreateVFont( 16, 16, L"Tahoma" );//D3DXCreateFont(m_pDevice,16,0,0,0,0,GB2312_CHARSET,0,0,0,L"Tahoma",&m_pFont[2]); 

	m_pVFont[3]->CreateVFont( 20, 20, L"隶书" );//D3DXCreateFont(m_pDevice,20,0,0,0,0,GB2312_CHARSET,0,0,0,L"隶书",&m_pFont[3]); 
	m_pVFont[4]->CreateVFont( 20, 20, L"宋体" );//D3DXCreateFont(m_pDevice,20,0,0,0,0,GB2312_CHARSET,0,0,0,L"宋体",&m_pFont[4]); 
	m_pVFont[5]->CreateVFont( 20, 20, L"Tahoma" );//D3DXCreateFont(m_pDevice,20,0,0,0,0,GB2312_CHARSET,0,0,0,L"Tahoma",&m_pFont[5]); 

	m_wScreenWidth = wWidth;
	m_wScreenHeight = wHeight;


	//////////////////////////////////////////////

	return true;
}


void CMyD3D::Release()
{
	SAFE_RELEASE( m_pDevice );
	
	for( int i = 0; i < 10; i ++ )
	{
		SAFE_DELETE( m_pVFont[i] );
	}
	SAFE_RELEASE( m_pD3D9 );
}


void CMyD3D::Render()
{
	Clear( RGB( 0, 0, 0 ) );
	m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	BeginScene();


	/*for( POS pos = m_pBaseList->GetFirstPos(); pos; )
	{
		CBase * p = m_pBaseList->GetNext( pos );

		if( p )
			p->Render();
	}*/
	if( NULL !=  m_pSky   )
		m_pSky->Render();
	if( NULL !=  m_pTerrain )
		m_pTerrain->Render();

	for( int i = 0; i < m_pBaseList.Num(); i ++ )
	{
		m_pBaseList[i]->Render();
	}
	
	EndScene();
}

void CMyD3D::SetFog( float start, float end, float density, DWORD color, WORD fogkey )
{
	m_fFogStart = start;
	m_fFogEnd = end;
	m_fFogDensity = density;
	m_wFogKey = fogkey;
	m_dwFogColor = color;

	//雾效
	D3DCAPS9 Caps;
	m_pDevice->GetDeviceCaps( &Caps );
	m_pDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );
	m_pDevice->SetRenderState( D3DRS_FOGCOLOR, m_dwFogColor );

	if( Caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE )
	{
		m_pDevice->SetRenderState( D3DRS_RANGEFOGENABLE, true );//发散雾化
	}

	switch( m_wFogKey )
	{
	case 0:
		m_pDevice->SetRenderState( D3DRS_FOGSTART, *(DWORD*)&m_fFogStart );
		m_pDevice->SetRenderState( D3DRS_FOGEND, *(DWORD*)&m_fFogEnd );
		m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_LINEAR );
		break;
	case 1:
		m_pDevice->SetRenderState( D3DRS_FOGDENSITY, *(DWORD*)&m_fFogDensity );
		m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_EXP );
		break;
	case 2:
		m_pDevice->SetRenderState( D3DRS_FOGDENSITY, *(DWORD*)&m_fFogDensity );
		m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_EXP2 );
		break;
	}
}

