#include "stdafx.h"
#include "TexturePaly.h"
//-----------------------------------------------
//面片的实现

CTexture8Play g_CTexture8Play;


CIGA3DUIElement::CIGA3DUIElement()
{
	m_pd3dDevice = NULL;
	m_pVB		 = NULL;
	m_pTex		 = NULL;

	m_x			 = 0;
	m_y			 = 0;		
	m_iWidth		 = 10;
	m_iHeight	 = 10;

}

CIGA3DUIElement::~CIGA3DUIElement()
{
	Destory();
}

HRESULT CIGA3DUIElement::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;
	
	//创建顶点缓冲区
	HRESULT hr = m_pd3dDevice->CreateVertexBuffer( 4*sizeof(_VUI), 0, _D3DFVF_VUI, D3DPOOL_MANAGED, &m_pVB );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: 创建UI的顶点缓冲区失败！ ");
		return E_FAIL;
	}

	//填充区顶点缓冲区,默认的面片大小是10（像素） 
	_VUI  vertices[] = 
	{
		{ 0.0f,  10.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f },
		{ 0.0f,   0.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f },
		{ 10.0f, 10.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f },
		{ 10.0f,  0.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f }
	};
	void* p;
	hr = m_pVB->Lock( 0, sizeof(vertices), (BYTE**)&p, 0 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: 锁定顶点缓冲区时失败！ ");
		return E_FAIL;
	}
	memcpy( p, vertices, sizeof(vertices) );
	m_pVB->Unlock();

	return S_OK;
}

void CIGA3DUIElement::Render()
{
	m_pd3dDevice->SetTexture( 0, m_pTex );
	m_pd3dDevice->SetVertexShader( _D3DFVF_VUI );
	m_pd3dDevice->SetStreamSource( 0, m_pVB,sizeof(_VUI) );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

void CIGA3DUIElement::Destory()
{
	if( m_pVB != NULL )
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
	
	//纹理不再这里释放
	m_pTex = NULL;
}

//设置大小
HRESULT CIGA3DUIElement::SetSize( int width, int height )
{
	void* p;
	HRESULT hr = m_pVB->Lock( 0, 0, (BYTE**)&p, 0 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: 锁定顶点缓冲区时失败！ ");
		return E_FAIL;
	}

	_VUI* pUI = (_VUI*)p;

	D3DXVECTOR3 pos[4];
	for( int i = 0; i < 4; i++ )
	{
		pos[i] = D3DXVECTOR3( pUI[i].x, pUI[i].y, pUI[i].z );
	}

	pos[0].y = pos[1].y + height;
	pos[3].x = pos[1].x + width;

	pos[2].x = pos[3].x;
	pos[2].y = pos[0].y;

	for( int i = 0; i < 4; i++ )
	{
		pUI[i].x = pos[i].x;
		pUI[i].y = pos[i].y;
	}

	m_pVB->Unlock();


	return S_OK;

}

void CIGA3DUIElement::SetTexCoord( int w, int h )
{
	void* p;
	HRESULT hr = m_pVB->Lock( 0, 0, (BYTE**)&p, 0 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: 锁定顶点缓冲区时失败！ ");
		return;
	}

	_VUI* pUI = (_VUI*)p;

	if( w > 128 && h > 128 )
	{
		float u = 0.0f;
		float v = 0.0f;

		//u
		if( w > 128 && w <= 256 )
		{
			u = (float)w/256.0f;
		}
		else if( w > 256 && w <= 512 )
		{
			u = (float)w/512.0f;
		}
		else if( w > 512 && w <= 1024 )
		{
			u = (float)w/1024.0f;
		}

		//v
		if( h > 128 && h <= 256 )
		{
			v = (float)h/256.0f;
		}
		else if( h > 256 && h <= 512 )
		{
			v = (float)h/512.0f;
		}
		else if( h > 512 && h <= 1024 )
		{
			v = (float)h/1024.0f;
		}

		//pUI[0].u = u;
		pUI[0].v = v;

		//pUI[1].u = u;
		//pUI[1].v = v;

		pUI[2].u = u;
		pUI[2].v = v;

		pUI[3].u = u;
		//pUI[3].v = v;
	}

	m_pVB->Unlock();
}


//设置位置
HRESULT CIGA3DUIElement::SetPosition( int x, int y )
{
	void* p;
	HRESULT hr = m_pVB->Lock( 0, 0, (BYTE**)&p, 0 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: 锁定顶点缓冲区时失败！ ");
		return E_FAIL;
	}

	_VUI* pUI = (_VUI*)p;

	D3DXVECTOR3 Offset( (float)x,(float)y,0.0f );

	D3DXVECTOR3 pos[4];
	for( int i = 0; i < 4; i++ )
	{
		pos[i] = D3DXVECTOR3( pUI[i].x, pUI[i].y, pUI[i].z );
		pos[i] += Offset;
	}

	for( int i = 0; i < 4; i++ )
	{
		pUI[i].x = pos[i].x;
		pUI[i].y = pos[i].y;
	}
	
	m_x			 = (int)pUI[1].x;
	m_y			 = (int)pUI[1].y;

	m_iWidth		 = (int)pUI[2].x;
	m_iHeight	 = (int)pUI[2].y;

	m_pVB->Unlock();

	return S_OK;


}

//设置使用的纹理
void CIGA3DUIElement::SetTexture( LPDIRECT3DTEXTURE8 pTex )
{
	m_pTex = pTex;
}

bool CIGA3DUIElement::IsInSide( int x, int y )
{
	if( x > m_x && x < m_iWidth && y > m_y && y < m_iHeight )
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------------
CTexture8Play::CTexture8Play()
{
	//大小
	m_FrameX = 0;
	m_FrameY = 0;
	m_FrameWidth = 400;
	m_FrameHeight = 320;

	m_ScreenX = m_FrameX + 12;
	m_ScreenY = m_FrameY + 13;
	m_ScreenWidth = m_FrameWidth - 8 - m_ScreenX;
	m_ScreenHeight = m_FrameHeight - 10 - m_ScreenY;

	m_MinX = m_FrameX;
	m_MinY = m_FrameY;
	m_MinWidth = 64;
	m_MinHeight = 32;

	m_MinButtonX = 370;
	m_MinButtonY = 288;
    m_MinButtonWidth = 25;
	m_MinButtonHeight = 25; 

	m_PlayButtonX = 8;
	m_PlayButtonY = 288;
    m_PlayButtonWidth = 32;
	m_PlayButtonHeight = 32; 

	m_StopButtonX = 38;
	m_StopButtonY = 288;
    m_StopButtonWidth = 32;
	m_StopButtonHeight = 32; 

	m_bIsPlay = false;
	m_bIsMin  = true;
	m_bPause  = false;
}

CTexture8Play::~CTexture8Play()
{

}

HRESULT CTexture8Play::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	//
	m_MediaFrame.Init(m_pd3dDevice);
	m_MediaFrame.SetSize( m_FrameWidth, m_FrameHeight );

	m_MediaScreen.Init(m_pd3dDevice);
	m_MediaScreen.SetSize( m_ScreenWidth, m_ScreenHeight );
	m_MediaScreen.SetPosition( m_ScreenX, m_ScreenY );

	m_MinScreen.Init(m_pd3dDevice);
	m_MinScreen.SetSize(m_MinWidth,m_MinHeight);
	m_MinScreen.SetPosition(m_FrameX,m_FrameY);

	m_MinButton.Init(m_pd3dDevice);  //最小化窗口框架
	m_MinButton.SetSize( m_MinButtonWidth, m_MinButtonHeight );
	m_MinButton.SetPosition( m_MinButtonX, m_MinButtonY );

	m_PlayButton.Init(m_pd3dDevice);  //播放、暂停按钮
	m_PlayButton.SetSize( m_PlayButtonWidth, m_PlayButtonHeight );
	m_PlayButton.SetPosition( m_PlayButtonX, m_PlayButtonY );

	m_StopButton.Init(m_pd3dDevice);  //播放、暂停按钮
	m_StopButton.SetSize( m_StopButtonWidth, m_StopButtonHeight );
	m_StopButton.SetPosition( m_StopButtonX, m_StopButtonY );

	m_nFlag	= -1; //0：表示视屏， 1表示flash

	//加载框架纹理
	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\PLAY_WINDOW.dds", &m_pFrameTex );
	
	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\button.dds", &m_pMinTex );
	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\button1.dds", &m_pMinTex1 );

	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\MinButton.dds", &m_pMinButtonTex );
	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\MinButton1.dds", &m_pMinButtonTex1 );

	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\BackGround.dds", &m_pTexBackGround );
	
	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\PlayButton.dds", &m_pPlayButtonTex );
	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\PlayButton1.dds", &m_pPlayButtonTex1 );

	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\PauseButton.dds", &m_pPauseButtonTex );
	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\PauseButton1.dds", &m_pPauseButtonTex1 );

	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\StopButton.dds", &m_pStopButtonTex );
	D3DXCreateTextureFromFile( m_pd3dDevice, "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\StopButton1.dds", &m_pStopButtonTex1 );

	return S_OK;
}

void CTexture8Play::PlayAVIFile( char* fileName )
{
	if( !m_bIsPlay && !m_bIsMin )
	{
		if( SUCCEEDED( m_VideoTexture.CreateFromAVIFile( m_pd3dDevice, fileName ) ) )
		{
			m_PlayFileName = fileName;
			OutputDebugString(m_PlayFileName.c_str());
			long w = m_VideoTexture.m_VideoWidth;
			long h = m_VideoTexture.m_VideoHeight;
			m_MediaScreen.SetTexCoord( w, h );
			m_VideoTexture.StartPlay();
			m_bIsPlay = true;
			m_nFlag = 0;
		}
	}
}

void CTexture8Play::PlayFlashFile( char* fileName )
{
	if( !m_bIsPlay && !m_bIsMin )
	{
		if( SUCCEEDED( m_FlashTexture.StartFlash( m_pd3dDevice, fileName, 320, 240 ) ) )
		{
			long w = m_FlashTexture.m_nWidth;
			long h = m_FlashTexture.m_nHeight;
			//m_MediaFrame.SetTexCoord( w, h );
			m_MediaScreen.SetTexCoord( w, h );
			m_FlashTexture.SetLoopPlay(FALSE);
			m_bIsPlay = true;
			m_nFlag = 1;
		}
	}
}

void CTexture8Play::Stop()
{
	if( m_bIsPlay && !m_bIsMin )
	{
		if( m_nFlag == 0 )
		{
			m_VideoTexture.Destroy();
			m_MediaScreen.SetTexCoord( 256, 256 );
			m_bIsPlay = false;
			m_nFlag   = -1;
		}
		else if( m_nFlag == 1 )
		{
			m_FlashTexture.Destory();
			m_MediaScreen.SetTexCoord( 256, 256 );
			m_bIsPlay = false;
			m_nFlag   = -1;
		}
		m_bPause = false;
	}
}

void CTexture8Play::Pause()
{
	m_VideoTexture.PaustPlay();
}

void CTexture8Play::PlayNext()
{

}

void CTexture8Play::AddToPlayList( string name )
{

}

int CTexture8Play::ShowPlayer( UINT type )
{
	//----------------------------------------------------
	{
		POINT kCursor;
		GetCursorPos(&kCursor);
		ScreenToClient(GetActiveWindow(), &kCursor);

		if( m_bIsMin )
		{
			if( m_MinScreen.IsInSide( kCursor.x, kCursor.y ) )
			{
				m_MinScreen.SetTexture(m_pMinTex1);
				//OutputDebugString("在范围内....");
				if( GetAsyncKeyState(VK_LBUTTON) )
				{
					m_bIsMin = false;
				}
			}
			else
			{
				m_MinScreen.SetTexture(m_pMinTex);
				//OutputDebugString("在范围外....");
			}
			m_MinScreen.Render();
		}
		else
		{
			LPDIRECT3DTEXTURE8  pTex = NULL;
			if( m_bIsPlay )	//正在播放
			{
				if( m_nFlag == 0 )
				{
					pTex		= m_VideoTexture.m_pTexture;
					m_VideoTexture.CheckForLoop();
					m_bIsPlay	= m_VideoTexture.m_bIsPlaying;
				}
				else if( m_nFlag == 1 )
				{
					m_FlashTexture.Update();
					pTex		= m_FlashTexture.GetRenderTarget();
					m_bIsPlay	= m_FlashTexture.IsPlaying();
				}
			}
			else
			{
				//if( m_nFlag == 0 )
				//{
				//	m_VideoTexture.Destroy();
				//	m_MediaScreen.SetTexCoord( 256, 256 );
				//}
				//else if( m_nFlag == 1 )
				//{
				//	m_FlashTexture.Destory();
				//	m_MediaScreen.SetTexCoord( 256, 256 );
				//}
				m_nFlag = -1;
			}

		//	if( pTex == NULL )
		//	{
		//		m_MediaScreen.SetTexture(m_pTexBackGround);
		//	}
		//	else
		//	{
		//		m_MediaScreen.SetTexture(pTex);
		//	}
		//	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
		//	m_MediaScreen.Render();
		//	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
		//	
		//	//播放
		//	if( m_bPause )
		//	{
		//		m_PlayButton.SetTexture(m_pPauseButtonTex);
		//		if( m_PlayButton.IsInSide( kCursor.x, kCursor.y ) )
		//		{
		//			m_PlayButton.SetTexture(m_pPauseButtonTex1);
		//			if( GetAsyncKeyState(VK_LBUTTON) )
		//			{
		//				m_bPause = false;
		//			}
		//		}
		//	}
		//	else
		//	{
		//		m_PlayButton.SetTexture(m_pPlayButtonTex);
		//		if( m_PlayButton.IsInSide( kCursor.x, kCursor.y ) )
		//		{
		//			m_PlayButton.SetTexture(m_pPlayButtonTex1);
		//			if( !m_bIsPlay && GetAsyncKeyState(VK_LBUTTON))
		//			{
		//				PlayAVIFile( "F:\\特种部队\\specialforce\\InterAdtive\\EMMOA\\media\\at_52.avi" );
		//				m_bPause = true;
		//			}

		//			if(GetAsyncKeyState(VK_LBUTTON))
		//			{
		//				m_bPause = true;
		//			}
		//		}			
		//	}
		//	m_PlayButton.Render();

		//	//停止
		//	m_StopButton.SetTexture(m_pStopButtonTex);
		//	if( m_StopButton.IsInSide( kCursor.x, kCursor.y ) )
		//	{
		//		m_StopButton.SetTexture(m_pStopButtonTex1);
		//		if( GetAsyncKeyState(VK_LBUTTON) )
		//		{
		//			Stop();
		//		}
		//	}
		//	m_StopButton.Render();

		//	//......
		//	m_MediaFrame.SetTexture( m_pFrameTex );
		//	m_MediaFrame.Render();

		//	//......
		//	m_MinButton.SetTexture(m_pMinButtonTex);
		//	if( m_MinButton.IsInSide( kCursor.x, kCursor.y ) )
		//	{
		//		m_MinButton.SetTexture(m_pMinButtonTex1);
		//		if( GetAsyncKeyState(VK_LBUTTON) )
		//		{
		//			m_bIsMin = true;
		//		}
		//	}
		//	m_MinButton.Render();

		}
	}

	//--------------------------------------------------------

	int width = 0;
	int height = 0;
	LPDIRECT3DTEXTURE8 m_pTexture = NULL;
	if( m_nFlag == 0 )
	{
		width = m_VideoTexture.m_VideoWidth;
		height = m_VideoTexture.m_VideoHeight;
		m_pTexture = m_VideoTexture.m_pTexture;
	}
	else if( m_nFlag == 1 )
	{
		width = m_FlashTexture.m_nWidth;
		height = m_FlashTexture.m_nHeight;
		m_pTexture = m_FlashTexture.GetRenderTarget();
	}

	
	if( m_pTexture == NULL )
	{
		return 0;
	}

	LPDIRECT3DSURFACE8 m_pBackBuf;
	HRESULT hr = m_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuf );
	if( SUCCEEDED(hr) )
	{
		D3DLOCKED_RECT	LockRect;
		D3DLOCKED_RECT	LockRectTex;
		RECT	rc;
		rc.left = 10;
		rc.top  = 400;
		rc.right = 0 + width;
		rc.bottom = 400 + height;

		//D3DRECT rect = { 100, 100, 200, 200 };
		//m_pDevice->Clear( 1, &rect, D3DCLEAR_TARGET, D3DXCOLOR(1,1,1,1), 1.0f, 0 );
		
		hr = m_pTexture->LockRect( 0, &LockRectTex, NULL, 0 );
		int pitchTex = LockRectTex.Pitch;
		if( SUCCEEDED(hr) )
		{
			hr = m_pBackBuf->LockRect( &LockRect, &rc, 0 );
			int pitch = LockRect.Pitch;
			if( SUCCEEDED(hr) )
			{
				DWORD* pBit = (DWORD*)LockRect.pBits;
				DWORD* pBitTex = (DWORD*)LockRectTex.pBits;
				for( int j = 0; j < height; j++ )
				{
					memcpy( pBit, pBitTex, width<<2 );
					//for( int i = 0; i < width; i++ )
					//{
					//	pBit[i] = pBitTex[i];
					//}
					pBit += pitch>>2;
					pBitTex += pitchTex>>2;
				}
				
				m_pBackBuf->UnlockRect();	


			}
			m_pTexture->UnlockRect(0);
		}
		m_pBackBuf->Release();
	}

	////-------------------


	return 0;
}

void CTexture8Play::SetPos( int x, int y )
{
	
}

void CTexture8Play::Destroy()
{
	m_VideoTexture.Destroy();
	m_FlashTexture.Destory();
}

//CTexture8Play* GetTexPlay()
//{
//	static CTexture8Play tp;
//	return &tp;
//}