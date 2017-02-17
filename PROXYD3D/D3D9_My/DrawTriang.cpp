
#include "stdafx.h"
#include "DrawTriang.h"

ID3DXSprite	*g_pSprite= NULL;
IDirect3DTexture9	*g_pTexture		= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB        = NULL;    //顶点缓冲区对象



void DrawRectMy( LPDIRECT3DDEVICE9 pd3dDevice, LPDIRECT3DTEXTURE9 pTex )
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//D3DRECT rec = {100,1,150,50};
	//pd3dDevice->Clear(1, &rec, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,0,0),0 ,0);


	DWORD dwFvF = 0;
	pd3dDevice->BeginScene();
	//pd3dDevice->GetFVF( &dwFvF );
	pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
	//pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
	//pd3dDevice->SetFVF( dwFvF );
	pd3dDevice->EndScene();


	//HRESULT	hr  = S_OK;
	//D3DRECT rec = {100,1,350,350};
	////	hr = pd3dDevice->Clear(1, &rec, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,255,255),0 ,0);
	//if(g_pSprite == NULL)
	//{
	//	hr = D3DXCreateTextureFromFile(pd3dDevice, "texture.jpg", &g_pTexture);
	//	hr = D3DXCreateSprite(pd3dDevice, &g_pSprite);
	//}

	//D3DXVECTOR2	Translation;
	//Translation.x	= 100;
	//Translation.y	= 1;
	//D3DXVECTOR2	Scaling;
	//Scaling.x	= 0.5f;
	//Scaling.y	= 0.5f;
	//D3DXMATRIX	Mat;
	//D3DXMatrixTransformation2D(&Mat, NULL, 0, &Scaling, NULL, 0, &Translation);

	////pd3dDevice->BeginScene();

	//g_pSprite->Begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE);

	//D3DXVECTOR3 vPos( 100.f, 1.f, 0.0f );

	////vPos.x /= 1.f;
	////vPos.y /= 1.f;

	//g_pSprite->SetTransform(&Mat);
	//g_pSprite->Draw(g_pTexture, (RECT*)&rec, NULL, &vPos, D3DXCOLOR(255, 255, 255, 255) );
	//g_pSprite->End();

	////pd3dDevice->EndScene();
	//return ;

	////////////////////////////////////////////////////////////////////////////

	////创建纹理对象
	//if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, "texture.jpg", &g_pTexture ) ) )
	//{
	//	return ;
	//}
	////顶点数据
	//CUSTOMVERTEX g_Vertices[] =
	//{
	//	{ -3,   -3,  0.0f,  0.0f, 1.0f},   
	//	{ -3,    3,  0.0f,  0.0f, 0.0f},	
	//	{  3,   -3,  0.0f,  1.0f, 1.0f},	
	//	{  3,    3,  0.0f,  1.0f, 0.0f }

	//};
	////创建顶点缓冲区
	//if( FAILED( pd3dDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
	//	0, D3DFVF_CUSTOMVERTEX,
	//	D3DPOOL_MANAGED, &g_pVB,NULL ) ) )
	//{
	//	return ;
	//}
	////填充顶点缓冲区
	//VOID* pVertices;
	//if( FAILED( g_pVB->Lock( 0, sizeof(g_Vertices), (void**)&pVertices, 0 ) ) )
	//	return ;
	//memcpy( pVertices, g_Vertices, sizeof(g_Vertices) );
	//g_pVB->Unlock();

	//pd3dDevice->SetRenderState(D3DRS_ZENABLE, false );
	//pd3dDevice->SetRenderState(D3DRS_LIGHTING, false );
	//pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false );
	////////////////////////////////////////////////////////////////////////////

	//D3DXMATRIXA16 matWorld;
	//D3DXMatrixIdentity( &matWorld );
	//pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	////创建并设置观察矩阵
	//D3DXVECTOR3 vEyePt( 0.0f, 0.0f, -30 );
	//D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	//D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	//D3DXMATRIXA16 matView;
	//D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	//pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	////创建并设置投影矩阵
	//D3DXMATRIXA16 matProj;
	//D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
	//pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	//

	////pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(45, 50, 170), 1.0f, 0 );

	////开始在后台缓冲区绘制图形
	//if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	//{
	//	pd3dDevice->SetTexture( 0, g_pTexture ); //设置纹理
	//	pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
	//	pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	//	pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	//	//结束在后台缓冲区绘制图形
	//	pd3dDevice->EndScene();
	//}

////////////////////////////////////////////////////////////////////////////
	//定义顶点
	//SVertex verteices[] = 
	//{
	//	{ 120,120,		0.0f, 1.0f,0xff00ff00,	0.0f, 0.0f },
	//	{ 250, 120,		0.0f, 1.0f,0xffff00ff,	1.0f, 0.0f },
	//	{ 250, 250,	0.0f, 1.0f,0xffffff00,	1.0f, 1.0f },
	//	{ 120, 250,	0.0f, 1.0f,0xffff00ff,	0.0f, 1.0f }
	//};

	//设置纹理
	//pd3dDevice->SetTexture( 0, pTex );

	//绘制
	//IDirect3DVertexDeclaration9 *pDecl = NULL;
	//pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl

	//pd3dDevice->SetFVF(SVertex::_fvf);
	//pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, verteices, sizeof(SVertex) );
	//pd3dDevice->SetVertexDeclaration( pDecl );
	//pDecl->Release();


}