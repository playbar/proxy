//=============================================================================
// Desc: 基于范围的雾化效果
//=============================================================================
#include <d3dx9.h>


//-----------------------------------------------------------------------------
// 全局变量
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D           = NULL; //Direct3D对象
LPDIRECT3DDEVICE9       g_pd3dDevice     = NULL; //Direct3D设备对象

LPD3DXMESH              g_pMesh          = NULL; //网格模型对象
D3DMATERIAL9*           g_pMeshMaterials = NULL; //网格模型材料
LPDIRECT3DTEXTURE9*     g_pMeshTextures  = NULL; //网格模型纹理
DWORD                   g_dwNumMaterials = 0L;   //网格模型子块数量

//----------------------------------------------------------------------------
// Desc: 
//----------------------------------------------------------------------------
inline FLOAT HeightField( FLOAT x, FLOAT z )
{
    FLOAT y = 0.0f;
    y += 10.0f * cosf( 0.051f*x + 0.0f ) * sinf( 0.055f*x + 0.0f );
    y += 10.0f * cosf( 0.053f*z + 0.0f ) * sinf( 0.057f*z + 0.0f );
    y += 2.0f * cosf( 0.101f*x + 0.0f ) * sinf( 0.105f*x + 0.0f );
    y += 2.0f * cosf( 0.103f*z + 0.0f ) * sinf( 0.107f*z + 0.0f );
    y += 2.0f * cosf( 0.251f*x + 0.0f ) * sinf( 0.255f*x + 0.0f );
    y += 2.0f * cosf( 0.253f*z + 0.0f ) * sinf( 0.257f*z + 0.0f );
    return y;
}


//-----------------------------------------------------------------------------
// Desc: 初始化Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
	//创建Direct3D对象, 该对象用来创建Direct3D设备对象
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

	//设置D3DPRESENT_PARAMETERS结构, 准备创建Direct3D设备对象
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    //创建Direct3D设备对象
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

	//检查是否支持基于范围的雾化
	D3DCAPS9  stCaps;
	g_pd3dDevice->GetDeviceCaps(&stCaps);
	if(!(stCaps.RasterCaps & D3DPRASTERCAPS_FOGRANGE))
		return E_FAIL;

    //设置环境光
    g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );
	//雾化设置
	g_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, true);
	g_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, 0xff0000ff);
	g_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	float fogStart   = 50;      //线性雾化开始位置
	float fogEnd     = 200;     //线性雾化结束位置	
	g_pd3dDevice->SetRenderState(D3DRS_FOGSTART,  *(DWORD*)&fogStart);
	g_pd3dDevice->SetRenderState(D3DRS_FOGEND,    *(DWORD*)&fogEnd);
	g_pd3dDevice->SetRenderState(D3DRS_RANGEFOGENABLE, true); //启用基于范围的物化

	//设置纹理过滤状态
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

    return S_OK;
}


//-----------------------------------------------------------------------------
// Desc: 从绝对路径中提取纹理文件名
//-----------------------------------------------------------------------------
void RemovePathFromFileName(LPSTR fullPath, LPWSTR fileName)
{
	//先将fullPath的类型变换为LPWSTR
	WCHAR wszBuf[MAX_PATH];
	MultiByteToWideChar( CP_ACP, 0, fullPath, -1, wszBuf, MAX_PATH );
	wszBuf[MAX_PATH-1] = L'\0';

	WCHAR* wszFullPath = wszBuf;

	//从绝对路径中提取文件名
	LPWSTR pch=wcsrchr(wszFullPath,'\\');
	if (pch)
		lstrcpy(fileName, ++pch);
	else
		lstrcpy(fileName, wszFullPath);
}


//-----------------------------------------------------------------------------
// Desc: 创建场景图形
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
    LPD3DXBUFFER pD3DXMtrlBuffer; //存储模型材料的缓冲区对象

    //从磁盘文件加载网格模型
    if( FAILED( D3DXLoadMeshFromX( L"seafloor.x", D3DXMESH_SYSTEMMEM, 
                                   g_pd3dDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials, 
                                   &g_pMesh ) ) )
    {
        MessageBox(NULL, L"Could not find seafloor.x", L"Fog", MB_OK);
        return E_FAIL;
    }

    //从网格模型中提取材料属性和纹理文件名 
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials]; //材料结构数组

    if( g_pMeshMaterials == NULL )
        return E_OUTOFMEMORY;
    g_pMeshTextures  = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
    if( g_pMeshTextures == NULL )
        return E_OUTOFMEMORY;

	//逐块提取网格模型材料属性和纹理文件名
    for( DWORD i=0; i<g_dwNumMaterials; i++ )
    {
        //复制材料属性
        g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
        g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;

        g_pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL && 
            strlen(d3dxMaterials[i].pTextureFilename) > 0 )
        {
			//获取纹理文件名
			WCHAR filename[256];
			RemovePathFromFileName(d3dxMaterials[i].pTextureFilename, filename);

            //创建纹理
            if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, filename, 
                                                  &g_pMeshTextures[i] ) ) )
            {
                MessageBox(NULL, L"Could not find texture map", L"Fog", MB_OK);
            }
			
        }
    }

	//释放在加载模型文件时创建的保存模型材质和纹理信息的缓冲区对象
    pD3DXMtrlBuffer->Release(); 

		//修改网格模型
	LPDIRECT3DVERTEXBUFFER9 pVB;
    if( SUCCEEDED( g_pMesh->GetVertexBuffer( &pVB ) ) )
    {
		struct VERTEX { FLOAT x,y,z,tu,tv;};
        VERTEX* pVertices;
        DWORD   dwNumVertices = g_pMesh->GetNumVertices();

        pVB->Lock( 0, 0, (void**)&pVertices, 0 );

        for( DWORD i=0; i<dwNumVertices; i++ )
            pVertices[i].y = HeightField( pVertices[i].x, pVertices[i].z );

        pVB->Unlock();
        pVB->Release();
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Desc: 释放创建对象
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	//释放网格模型材料
    if( g_pMeshMaterials != NULL ) 
        delete[] g_pMeshMaterials;

	//释放网格模型纹理
    if( g_pMeshTextures )
    {
        for( DWORD i = 0; i < g_dwNumMaterials; i++ )
        {
            if( g_pMeshTextures[i] )
                g_pMeshTextures[i]->Release();
        }
        delete[] g_pMeshTextures;
    }

	//释放网格模型对象
    if( g_pMesh != NULL )
        g_pMesh->Release();
    
    //释放Direct3D设备对象
    if( g_pd3dDevice != NULL) 
        g_pd3dDevice->Release();

	//释放Direct3D对象
    if( g_pD3D != NULL)
        g_pD3D->Release();
}

//-------------------------------------------------
// Desc: 设置变换矩阵
//--------------------------------------------------
VOID SetupMatrices()
{
    //创建并设置世界矩阵
    D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

    //创建并设置观察矩阵
    D3DXVECTOR3 vEyePt( 0.0f, 30.0f,-100.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    //创建并设置投影矩阵
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 200.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}


//-----------------------------------------------------------------------------
// Desc: 渲染图形
//-----------------------------------------------------------------------------
VOID Render()
{
	//清空后台缓冲区
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
                         D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
    
     //开始在后台缓冲区绘制图形
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        SetupMatrices();  //设置世界矩阵

        //逐块渲染网格模型
        for( DWORD i=0; i<g_dwNumMaterials; i++ )
        {
            //设置材料和纹理
            g_pd3dDevice->SetMaterial( &g_pMeshMaterials[i] );
            g_pd3dDevice->SetTexture( 0, g_pMeshTextures[i] );

			//渲染模型
            g_pMesh->DrawSubset( i );
        }

        //结束在后台缓冲区渲染图形
        g_pd3dDevice->EndScene();
    }

    //将在后台缓冲区绘制的图形提交到前台缓冲区显示
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}


//-----------------------------------------------------------------------------
// Desc: 消息处理
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static bool rangeFog = true;
	switch( msg )
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage( 0 );
		return 0;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case 65+'R'-'A':  //基于范围的雾化
			rangeFog = !rangeFog;
			g_pd3dDevice->SetRenderState(D3DRS_RANGEFOGENABLE, rangeFog);
			break;
		}
	}

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Desc: 程序入口
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    //注册窗口类
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"ClassName", NULL };
    RegisterClassEx( &wc );

    //创建窗口
    HWND hWnd = CreateWindow( L"ClassName", L"基于范围雾化", 
                              WS_OVERLAPPEDWINDOW, 200, 100, 600, 500,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

	//初始化Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    { 
		//创建场景图形
        if( SUCCEEDED( InitGeometry() ) )
        {
            //显示窗口
            ShowWindow( hWnd, SW_SHOWDEFAULT );
            UpdateWindow( hWnd );

            //进入消息循环
            MSG msg; 
            ZeroMemory( &msg, sizeof(msg) );
            while( msg.message!=WM_QUIT )
            {
                if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                else
				{
                    Render();  //渲染图形
				}
            }
        }
    }

    UnregisterClass( L"ClassName", wc.hInstance );
    return 0;
}



