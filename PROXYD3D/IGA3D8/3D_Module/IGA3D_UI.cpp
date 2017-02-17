#include "stdafx.h"
#include "define.h"
#include <shellapi.h>
#include "tchar.h"
#include "z_FileIni.h"
#include "3D_Core.h"
#include "IGA3D_UI.h"


CIGA3DMessageManager	g_CIGA3DMessageManager;
CUITexture				g_CUITexture;


void CallBackProcFun()
{
	int iPickID = g_IGA3DModule.m_PickID;
	if( iPickID > 0 )
	{
		HWND hWnd = GetDesktopWindow();
		switch( iPickID )
		{
		case 12:
		case 13:
			ShellExecute( hWnd, "open", "http://www.canon.com.cn/products/printer/selphy/index.html", NULL, NULL, SW_SHOW );
			break;

		case 8:
			ShellExecute( hWnd, "open", "http://www.pepsi.cn/", NULL, NULL, SW_SHOW );
			break;

		default:
			break;
		}	
	}
	g_IGA3DModule.m_PickID = -1;
}

void CallBackProcFun1()
{
	g_IGA3DModule.m_PickID = -1;
}

//-----------------------------------------------
//面片的实现
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
	void* pVBdata;
	hr = m_pVB->Lock( 0, sizeof(vertices), (BYTE**)&pVBdata, 0 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: 锁定顶点缓冲区时失败！ ");
		return E_FAIL;
	}
	memcpy( pVBdata, vertices, sizeof(vertices) );
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
	void* pVBData;
	HRESULT hr = m_pVB->Lock( 0, 0, (BYTE**)&pVBData, 0 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: 锁定顶点缓冲区时失败！ ");
		return E_FAIL;
	}

	_VUI* pUI = (_VUI*)pVBData;

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


CUITexture::CUITexture()
{
	m_pd3dDevice = NULL;
	m_pTexUI       = NULL;
}

CUITexture::~CUITexture()
{
	Destory();
}

HRESULT CUITexture::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	char szPath[256];
	sprintf( szPath, "%s%s",PATH_WORK,"UI\\ui.dds" );
	GetModulePath( szPath );

	HRESULT hr = D3DXCreateTextureFromFile( m_pd3dDevice, szPath, &m_pTexUI );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: UI纹理创建失败！");
		return hr;
	}

	//--------------------------------------------------
	sprintf( szPath, "%s%s",PATH_WORK,"UI\\ok.dds" );
	GetModulePath( szPath );

	hr = D3DXCreateTextureFromFile( m_pd3dDevice, szPath, &m_pTexOK );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: UI纹理创建失败！");
		return hr;
	}
	
	//--------------------------------------------------
	sprintf( szPath, "%s%s",PATH_WORK,"UI\\ok1.dds" );
	GetModulePath( szPath );

	hr = D3DXCreateTextureFromFile( m_pd3dDevice, szPath, &m_pTexOk1 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: UI纹理创建失败！");
		return hr;
	}

	//--------------------------------------------------
	sprintf( szPath, "%s%s",PATH_WORK,"UI\\cancel.dds" );
	GetModulePath( szPath );

	hr = D3DXCreateTextureFromFile( m_pd3dDevice, szPath, &m_pTexCancel );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: UI纹理创建失败！");
		return hr;
	}

	//--------------------------------------------------
	sprintf( szPath, "%s%s",PATH_WORK,"UI\\cancel1.dds" );
	GetModulePath( szPath );

	hr = D3DXCreateTextureFromFile( m_pd3dDevice, szPath, &m_pTexCancel1 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: UI纹理创建失败！");
		return hr;
	}

    return S_OK;
}

LPDIRECT3DTEXTURE8 CUITexture::GetTexture( int id )
{
	if( id == 0 )
	{
		return m_pTexUI;
	}
	else if( id == 1 )
	{
		return m_pTexOK;
	}
	else if( id == 2 )
	{
		return m_pTexOk1;
	}
	else if( id == 3 )
	{
		return m_pTexCancel;
	}
	else if( id == 4 )
	{
		return m_pTexCancel1;
	}
	return NULL;

}

void CUITexture::Destory()
{
	if( m_pTexUI != NULL )
	{
		m_pTexUI->Release();
		m_pTexUI = NULL;
	}

	if( m_pTexOK != NULL )
	{
		m_pTexOK->Release();
		m_pTexOK = NULL;
	}

	if( m_pTexOk1 != NULL )
	{
		m_pTexOk1->Release();
		m_pTexOk1 = NULL;
	}

	if( m_pTexCancel != NULL )
	{
		m_pTexCancel->Release();
		m_pTexCancel = NULL;
	}

	if( m_pTexCancel1 != NULL )
	{
		m_pTexCancel1->Release();
		m_pTexCancel1 = NULL;
	}
}

//----------------------------------------------
CIGAUIButton::CIGAUIButton()
{
	m_pd3dDevice = NULL;
	m_pProcFun   = 0;
	m_iTexID		 = 1;
}

CIGAUIButton::~CIGAUIButton()
{
	Destory();
}

HRESULT CIGAUIButton::Init( LPDIRECT3DDEVICE8 pd3dDevice, LPFUN pFun )
{
	m_pd3dDevice = pd3dDevice;
	m_pProcFun   = pFun;

	m_UIElement.Init( m_pd3dDevice );
	m_UIElement.SetSize( 70, 30 );
    
	return S_OK;
}

void CIGAUIButton::Draw( int x, int y )
{  
	if( m_UIElement.IsInSide( x, y ) )
	{
		m_UIElement.SetTexture( g_CUITexture.GetTexture( m_iTexID + 1 ) );
		SIGA3DMSG msg = g_IGA3DModule.m_Msg;
		if( msg.iKey == IGA3D_MSG_RBUTTONDOWN && m_pProcFun )
		{
			m_pProcFun();
		}
	}
	else
	{
		m_UIElement.SetTexture( g_CUITexture.GetTexture( m_iTexID ) );
	}

	m_UIElement.Render();
}

void CIGAUIButton::Destory()
{
	m_UIElement.Destory();
}

void CIGAUIButton::SetPosition( int x, int y )
{
	m_UIElement.SetPosition( x, y );
}

void CIGAUIButton::SetProcFun( LPFUN pFun )
{
	m_pProcFun = pFun;
}

void CIGAUIButton::SetTextureID( int tex_id )
{
	m_iTexID = tex_id;
}

//------------------------------------------------
//
IGA3DDialog::IGA3DDialog()
{
	m_pd3dDevice = NULL;
	m_pFrame	 = NULL;
}

IGA3DDialog::~IGA3DDialog()
{
	Destory();
}

HRESULT IGA3DDialog::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	g_CUITexture.Init(m_pd3dDevice);

	m_Frame.Init( m_pd3dDevice );
	m_Frame.SetSize( 250, 200 );
	m_Frame.SetPosition( 10, 400 );
	
	m_ButtomOK.Init( m_pd3dDevice );
	m_ButtomOK.SetPosition( 160,550 );
	m_ButtomOK.SetProcFun( CallBackProcFun );
	m_ButtomOK.SetTextureID( 1 );

	m_ButtomCancel.Init( m_pd3dDevice );
	m_ButtomCancel.SetPosition( 40,550 );
	m_ButtomCancel.SetProcFun( CallBackProcFun1 );
	m_ButtomCancel.SetTextureID( 3 );

    return S_OK;
}

void IGA3DDialog::Render()
{
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, true );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000 );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
        
	m_Frame.SetTexture( g_CUITexture.GetTexture( 0 ) );
	m_Frame.Render();

	//............
	int x = g_CIGA3DMessageManager.m_xCursor;
	int y = g_CIGA3DMessageManager.m_yCursor; 
	m_ButtomOK.Draw( x, y );
	m_ButtomCancel.Draw( x, y );

	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, false );

}

void IGA3DDialog::Destory()
{
	//GetIGA3DFont()->Destory();
	//GetUI()->Destory();
}

//-----------------------------------------------------------------
//消息管理器
CIGA3DMessageManager::CIGA3DMessageManager()
{
	m_xCursor = 0;
	m_yCursor = 0;

}

CIGA3DMessageManager::~CIGA3DMessageManager()
{

}

HRESULT CIGA3DMessageManager::Init()
{

	return S_OK;
}

bool CIGA3DMessageManager::GetIGA3DMessage( SIGA3DMSG* msg )
{
	
	if( m_queueMsg.empty() )
	{
		return false;	//没有消息
	}
	
	//OutputDebugString("有一个消息...........");

	*msg = m_queueMsg.front();

	return true;


	//if( GetKeyState(VK_RBUTTON) < 0 )	//鼠标右键按下状态
	//{
	//	msg->type = 0;
	//	msg->key  = IGA3D_MSG_RBUTTONDOWN;
	//	msg->x    = 0;
	//	msg->y    = 0;
	//}
	//else
	//{
	//	return false;
	//}

	//return true;
}

void CIGA3DMessageManager::SendIGA3DMessage( SIGA3DMSG msg )
{
	m_queueMsg.push(msg);
}

void CIGA3DMessageManager::RemoveMsg()
{
	m_queueMsg.pop();
}

void CIGA3DMessageManager::Destory()
{


}

void CIGA3DMessageManager::UpdataMessage()
{
	HWND hWnd = GetActiveWindow();
	POINT kCursor;
	GetCursorPos(&kCursor);
	ScreenToClient(hWnd, &kCursor);
	
	m_xCursor = kCursor.x;
	m_yCursor = kCursor.y;


	static bool flg = false;
	static bool flg1 = false;
	if( GetKeyState(VK_LBUTTON) < 0 )	//鼠标右键按下状态
	{
		flg1 = true;
		if( !flg )
		{
			flg = true;
			
			//发送一个鼠标按下状态
			SIGA3DMSG msg;
			msg.iKey = IGA3D_MSG_RBUTTONDOWN;
			msg.ixPoint   = m_xCursor;
			msg.iyPoint   = m_yCursor;
			SendIGA3DMessage( msg );
		}
	}
	else	//鼠标右键抬起状态
	{
		flg = false;
		if( flg1 )
		{
			flg1 = false;
			
			//发送一个鼠标抬起状态
			SIGA3DMSG msg;
			msg.iKey = IGA3D_MSG_RBUTTONUP;
			msg.ixPoint   = m_xCursor;
			msg.iyPoint   = m_yCursor;
			SendIGA3DMessage( msg );

		}
	}
}
