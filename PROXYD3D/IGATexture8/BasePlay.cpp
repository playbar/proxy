#include "stdafx.h"
#include "BasePlay.h"

CBasePlay*	CBasePlay::m_Instance = NULL;

CBasePlay* CBasePlay::GetPlayerInstance()
{
	if( m_Instance == NULL )
	{
		m_Instance = new CBackGroundPlayer();
	}
	return m_Instance;
}

//--------------------------------------------------------
CBackGroundPlayer::CBackGroundPlayer()
{
	m_bPlayIsEnd	= false;

	m_nMediaWidth	= 32;
	m_nMediaHeight  = 32;

	m_pd3dDevice	= NULL;
	m_pBackGroundBuff = NULL;

	ZeroMemory( &m_BackGroundBufDesc, sizeof(m_BackGroundBufDesc) );

	m_nPlayType = 0;

	m_X = 100;
	m_Y = 200;

	m_bDrawBuf = true;

	__x = 0;
	__y = 0;
	_bLB = true;
}

CBackGroundPlayer::~CBackGroundPlayer()
{
	if( m_pBackGroundBuff != NULL )
	{
		//m_pBackGroundBuff->Release;
		m_pBackGroundBuff = NULL;
	}

	m_VideoPlayer.Destroy();

	m_bPlayIsEnd	= false;

	m_nPlayType = 0;
}

bool CBackGroundPlayer::CheckDevice()
{
	HRESULT hr = m_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackGroundBuff );
	if( FAILED(hr) )
	{
		return false;
	}

	RECT	rc;
	rc.left = 0;
	rc.top  = 0;
	rc.right = 100;
	rc.bottom = 100;

	D3DLOCKED_RECT	LockRect;

	hr = m_pBackGroundBuff->LockRect( &LockRect, &rc, 0 );
	m_pBackGroundBuff->UnlockRect();
	m_pBackGroundBuff->Release();
	m_pBackGroundBuff = NULL;

	if( FAILED(hr) )
	{
		return false;
	}
	return true;
}

void CBackGroundPlayer::PlayAVIFile( LPDIRECT3DDEVICE8 pd3dDevice, char* AVIFileName )
{
	if( pd3dDevice == NULL )
	{
		m_bPlayIsEnd = false;
		return;
	}

	if( !m_bPlayIsEnd )
	{
		m_pd3dDevice = pd3dDevice;

		m_bDrawBuf = CheckDevice();

		HRESULT hr = S_OK;

		hr = m_VideoPlayer.CreateFromAVIFile( m_pd3dDevice, AVIFileName );
		if( FAILED(hr) )
		{
			OutputDebugString("CBackGroundPlayer: AVI文件打开失败!");
			return;
		}
        
		m_VideoPlayer.StartPlay();

		m_nPlayType = 1;	//AVI文件

		m_bPlayIsEnd = true;
	}
}

void CBackGroundPlayer::PlayFlashFile( LPDIRECT3DDEVICE8 pd3dDevice, char* SWFFileName )
{
	if( pd3dDevice == NULL )
	{
		m_bPlayIsEnd = false;
		return;
	}

	if( !m_bPlayIsEnd )
	{
		m_pd3dDevice = pd3dDevice;

		m_bDrawBuf = CheckDevice();

		HRESULT hr = S_OK;
		hr = m_FlashTexture.StartFlash( m_pd3dDevice, SWFFileName, 320, 240 );
		if( FAILED(hr) )
		{
			OutputDebugString("CBackGroundPlayer: SWF文件打开失败!");
			return;
		}
		m_FlashTexture.SetLoopPlay(FALSE);

		m_nPlayType = 2;	//SWF文件

		m_bPlayIsEnd = true;
	}
}

LPDIRECT3DTEXTURE8 CBackGroundPlayer::GetMediaInfor()
{
	if( m_nPlayType == 1 )
	{
		m_VideoPlayer.CheckForLoop();
		if( !m_VideoPlayer.m_bIsPlaying )
		{
			m_VideoPlayer.Destroy();
			return NULL;
		}

		m_nMediaWidth  = m_VideoPlayer.m_VideoWidth;
		m_nMediaHeight = m_VideoPlayer.m_VideoHeight;

		return m_VideoPlayer.GetTexture();
	}

	if( m_nPlayType == 2 )
	{
		m_FlashTexture.Update();
		if( !m_FlashTexture.IsPlaying() )
		{
			m_FlashTexture.Destory();
			return NULL;
		}

		m_nMediaWidth  = m_FlashTexture.m_nWidth;
		m_nMediaHeight = m_FlashTexture.m_nHeight;

		return m_FlashTexture.GetRenderTarget();

	}

	return NULL;
}

void CBackGroundPlayer::FrameMove()
{
	if( m_bPlayIsEnd )
	{
		GetCursorPos(&m_kCursor);
		ScreenToClient(GetActiveWindow(), &m_kCursor);

		//得到视屏信息
		LPDIRECT3DTEXTURE8 pTexture = GetMediaInfor();
		if( pTexture == NULL )
		{
			m_bPlayIsEnd = false;
			if( m_nPlayType == 1 )
			{
				m_VideoPlayer.Destroy();
			}
			else if( m_nPlayType == 2 ) 
			{
				m_FlashTexture.Destory();
			}

			//m_X = 100;
			//m_Y = 200;

			OutputDebugString( "CBackGroundPlayer: 播放结束");
			return;
		}

		if( m_bDrawBuf )
		{
			DrawToBkBuf( pTexture );
		}
		else
		{
			DrawToMesh( pTexture );
		}

	}
}

void CBackGroundPlayer::ComputeCursor()
{
	static int __x = 0;
	static int __y = 0;
	static bool _bLB = true;

	if( GetAsyncKeyState(VK_LBUTTON) )
	{		
		int _x = m_kCursor.x;
		int _y = m_kCursor.y;
		if( _x > m_LockRect.left && _x < m_LockRect.right && _y > m_LockRect.top && _y < m_LockRect.bottom )
		{	
			//在播放窗口内
			if( _bLB )
			{
				_bLB = false;
				__x = _x - m_LockRect.left; 
				__y = _y - m_LockRect.top;
			}
		}

		if( !_bLB )
		{
			RECT	rc1;
			rc1.left  = _x - __x;
			rc1.top   = _y - __y;
			rc1.right = rc1.left + m_nMediaWidth;
			rc1.bottom = rc1.top + m_nMediaHeight;

			if( rc1.left > 0 && rc1.top > 0 
				&& rc1.bottom < m_BackGroundBufDesc.Height
				&& rc1.right < m_BackGroundBufDesc.Width )
			{
				m_LockRect.bottom = rc1.bottom;
				m_LockRect.top = rc1.top;
				m_LockRect.left = rc1.left;
				m_LockRect.right = rc1.right;

				m_X = m_LockRect.left;
				m_Y = m_LockRect.top;
			}
			else
			{
				__x = _x - m_LockRect.left; 
				__y = _y - m_LockRect.top;
			}
		}
	}
	else
	{
		_bLB = true;
		__x = -1;
		__y = -1;
	}
}

void CBackGroundPlayer::DrawToBkBuf( LPDIRECT3DTEXTURE8 pTexture )
{
	//向后背缓冲区绘制
	if( m_pBackGroundBuff == NULL )
	{
		HRESULT hr = m_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackGroundBuff );
		if( FAILED(hr) )
		{
			return;
		}

		hr = m_pBackGroundBuff->GetDesc( &m_BackGroundBufDesc );
		if( FAILED(hr) )
		{
			OutputDebugString("CBackGroundPlayer: 获取后背缓冲区描述失败");
			return;
		}

		m_LockRect.left = m_X;
		m_LockRect.top  = m_Y;
		m_LockRect.right = m_LockRect.left + m_nMediaWidth;
		m_LockRect.bottom = m_LockRect.top + m_nMediaHeight;
		
		if( m_LockRect.right > m_BackGroundBufDesc.Width )
		{
			m_LockRect.right = m_BackGroundBufDesc.Width - 1;			
			m_LockRect.left = m_LockRect.right - m_nMediaWidth;
			m_X = m_LockRect.left;
		}

		if( m_LockRect.bottom > m_BackGroundBufDesc.Height )
		{
			m_LockRect.bottom = m_BackGroundBufDesc.Height - 1;
			m_LockRect.top = m_LockRect.bottom - m_nMediaHeight;
			m_Y = m_LockRect.top;
		}
	
	}

	//计算鼠标动作	
	ComputeCursor();

	//开始绘制
	D3DLOCKED_RECT	LockRect;
	D3DLOCKED_RECT	LockRectTex;
	
	HRESULT hr = m_pBackGroundBuff->LockRect( &LockRect, &m_LockRect, 0 );
	if( SUCCEEDED(hr) )
	{
		int pitch = LockRect.Pitch;

		hr = pTexture->LockRect( 0, &LockRectTex, NULL, 0 );	
		if( SUCCEEDED(hr) )
		{
			int pitchTex = LockRectTex.Pitch;

			DWORD* pBit = (DWORD*)LockRect.pBits;
			DWORD* pBitTex = (DWORD*)LockRectTex.pBits;
			for( int j = 0; j < m_nMediaHeight; j++ )
			{
				memcpy( pBit, pBitTex, m_nMediaWidth<<2 );
				//for( int i = 0; i < m_nMediaWidth; i++ )
				//{
				//	pBit[i] = pBitTex[i];
				//}
				pBit += (pitch>>2);
				pBitTex += (pitchTex>>2);
			}			
			pTexture->UnlockRect(0);	
		}
		else
		{
			D3DRECT rect = { 100, 100, 200, 200 };	
			m_pd3dDevice->Clear( 1, &rect, D3DCLEAR_TARGET, D3DXCOLOR(1,1,1,1), 1.0f, 0 );	
		}
		m_pBackGroundBuff->UnlockRect();
	}
	else
	{
		D3DRECT rect = { m_LockRect.left, m_LockRect.top, m_LockRect.right, m_LockRect.bottom };
		m_pd3dDevice->Clear( 1, &rect, D3DCLEAR_TARGET, D3DXCOLOR(1,1,1,1), 1.0f, 0 );
	}

	m_pBackGroundBuff->Release();
	m_pBackGroundBuff = NULL;

	if( m_kCursor.x > m_LockRect.left && m_kCursor.x < m_LockRect.right && m_kCursor.y > m_LockRect.top && m_kCursor.y < m_LockRect.bottom )
	{
		D3DRECT rect = { m_kCursor.x -5, m_kCursor.y -5, m_kCursor.x + 5, m_kCursor.y + 5 };
		m_pd3dDevice->Clear( 1, &rect, D3DCLEAR_TARGET, D3DXCOLOR(1,1,1,1), 1.0f, 0 );
	}

	if( m_nPlayType == 1 )
	{
		m_VideoPlayer.SetUpData();
	}
}

void CBackGroundPlayer::DrawToMesh( LPDIRECT3DTEXTURE8 pTexture )
{

}

void CBackGroundPlayer::Destory()
{
	m_Instance = NULL;
	if( m_bPlayIsEnd )
	{
		if( m_nPlayType == 1 )
		{
			m_VideoPlayer.Destroy();
			m_bPlayIsEnd = false;
		}

		if( m_nPlayType == 2 )
		{
			m_FlashTexture.Destory();
			m_bPlayIsEnd = false;
		}
	}
	delete this;
}