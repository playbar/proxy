#include "stdafx.h"

CIGA3DMessageManager* GetMessageManager()
{
	static CIGA3DMessageManager msg;
	return &msg;
}



//-------------------------------------
CIGA3D_UI_Element* GetUI()
{
	//static CIGA3D_UI_Element ui;
	//return &ui;
	return NULL;
}

CUI_Tex* GetUITexture()
{
	static CUI_Tex ui_tex;
	return &ui_tex;
}

IGA3DDialog* GetDialog()
{
	static IGA3DDialog dlg;
    return &dlg;
}

//这两个是按钮事件函数
void CallBackProcFun()
{
	int id = GetIGA3DModule()->m_PickID;
	if( id > 0 )
	{
		HWND hWnd = GetDesktopWindow();
		switch( id )
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
	GetIGA3DModule()->m_PickID = -1;
}

void CallBackProcFun1()
{
	GetIGA3DModule()->m_PickID = -1;
}

//-----------------------------------------------
//面片的实现
CIGA3D_UI_Element::CIGA3D_UI_Element()
{
	m_pd3dDevice = NULL;
	m_pVB		 = NULL;
	m_pTex		 = NULL;

	m_x			 = 0;
	m_y			 = 0;		
	m_width		 = 10;
	m_height	 = 10;

}

CIGA3D_UI_Element::~CIGA3D_UI_Element()
{
	Destory();
}

HRESULT CIGA3D_UI_Element::Init( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;
	
	//创建顶点缓冲区
	HRESULT hr = m_pd3dDevice->CreateVertexBuffer( 4*sizeof(_VUI), 0, _D3DFVF_VUI, D3DPOOL_MANAGED, &m_pVB, 0 );
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
	hr = m_pVB->Lock( 0, sizeof(vertices), (void**)&p, 0 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: 锁定顶点缓冲区时失败！ ");
		return E_FAIL;
	}
	memcpy( p, vertices, sizeof(vertices) );
	m_pVB->Unlock();

	return S_OK;
}

void CIGA3D_UI_Element::Render()
{
	m_pd3dDevice->SetTexture( 0, m_pTex );
	m_pd3dDevice->SetFVF( _D3DFVF_VUI );
	m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(_VUI) );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

void CIGA3D_UI_Element::Destory()
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
HRESULT CIGA3D_UI_Element::SetSize( int width, int height )
{
	void* p;
	HRESULT hr = m_pVB->Lock( 0, 0, (void**)&p, 0 );
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

//设置位置
HRESULT CIGA3D_UI_Element::SetPosition( int x, int y )
{
	void* p;
	HRESULT hr = m_pVB->Lock( 0, 0, (void**)&p, 0 );
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

	m_width		 = (int)pUI[2].x;
	m_height	 = (int)pUI[2].y;

	m_pVB->Unlock();

	return S_OK;


}

//设置使用的纹理
void CIGA3D_UI_Element::SetTexture( LPDIRECT3DTEXTURE9 pTex )
{
	m_pTex = pTex;
}

bool CIGA3D_UI_Element::IsInSide( int x, int y )
{
	if( x > m_x && x < m_width && y > m_y && y < m_height )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------
//UI纹理对象
CUI_Tex::CUI_Tex()
{
	m_pd3dDevice = NULL;
	m_pTex       = NULL;
}

CUI_Tex::~CUI_Tex()
{
	Destory();
}

HRESULT CUI_Tex::Init( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	//--------------------------------------------------
	char path[256];
	sprintf( path, "%s%s",PATH_WORK,"UI\\ui.dds" );
	GetAbsPath( path );

	HRESULT hr = D3DXCreateTextureFromFile( m_pd3dDevice, path, &m_pTex );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: UI纹理创建失败！");
		return hr;
	}

	//--------------------------------------------------
	sprintf( path, "%s%s",PATH_WORK,"UI\\ok.dds" );
	GetAbsPath( path );

	hr = D3DXCreateTextureFromFile( m_pd3dDevice, path, &m_pTex1 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: UI纹理创建失败！");
		return hr;
	}
	
	//--------------------------------------------------
	sprintf( path, "%s%s",PATH_WORK,"UI\\ok1.dds" );
	GetAbsPath( path );

	hr = D3DXCreateTextureFromFile( m_pd3dDevice, path, &m_pTex2 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: UI纹理创建失败！");
		return hr;
	}

	//--------------------------------------------------
	sprintf( path, "%s%s",PATH_WORK,"UI\\cancel.dds" );
	GetAbsPath( path );

	hr = D3DXCreateTextureFromFile( m_pd3dDevice, path, &m_pTex3 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: UI纹理创建失败！");
		return hr;
	}

	//--------------------------------------------------
	sprintf( path, "%s%s",PATH_WORK,"UI\\cancel1.dds" );
	GetAbsPath( path );

	hr = D3DXCreateTextureFromFile( m_pd3dDevice, path, &m_pTex4 );
	if( FAILED(hr) )
	{
		OutputDebugString("IGA3D: UI纹理创建失败！");
		return hr;
	}


    return S_OK;
}

LPDIRECT3DTEXTURE9 CUI_Tex::GetTexture( int id )
{
	if( id == 0 )
	{
		return m_pTex;
	}
	else if( id == 1 )
	{
		return m_pTex1;
	}
	else if( id == 2 )
	{
		return m_pTex2;
	}
	else if( id == 3 )
	{
		return m_pTex3;
	}
	else if( id == 4 )
	{
		return m_pTex4;
	}
	return NULL;

}

void CUI_Tex::Destory()
{
	if( m_pTex != NULL )
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	if( m_pTex1 != NULL )
	{
		m_pTex1->Release();
		m_pTex1 = NULL;
	}

	if( m_pTex2 != NULL )
	{
		m_pTex2->Release();
		m_pTex2 = NULL;
	}

	if( m_pTex3 != NULL )
	{
		m_pTex3->Release();
		m_pTex3 = NULL;
	}

	if( m_pTex4 != NULL )
	{
		m_pTex4->Release();
		m_pTex4 = NULL;
	}
}

//----------------------------------------------
CButtom::CButtom()
{
	m_pd3dDevice = NULL;
	m_pProcFun   = 0;
	m_TexID		 = 1;
}

CButtom::~CButtom()
{
	Destory();
}

HRESULT CButtom::Init( LPDIRECT3DDEVICE9 pd3dDevice, LPFUN pFun )
{
	m_pd3dDevice = pd3dDevice;
	m_pProcFun   = pFun;

	//
	m_Buttom.Init( m_pd3dDevice );
	m_Buttom.SetSize( 70, 30 );
    
	return S_OK;
}

void CButtom::Draw( int x, int y )
{  
	if( m_Buttom.IsInSide( x, y ) )
	{
		m_Buttom.SetTexture( GetUITexture()->GetTexture( m_TexID + 1 ) );
		SIGA3DMSG msg = GetIGA3DModule()->m_Msg;
		if( msg.key == IGA3D_MSG_RBUTTONDOWN && m_pProcFun )
		{
			m_pProcFun();
		}
	}
	else
	{
		m_Buttom.SetTexture( GetUITexture()->GetTexture( m_TexID ) );
	}

	m_Buttom.Render();
}

void CButtom::Destory()
{
	m_Buttom.Destory();
}

void CButtom::SetPosition( int x, int y )
{
	m_Buttom.SetPosition( x, y );
}

void CButtom::SetProcFun( LPFUN pFun )
{
	m_pProcFun = pFun;
}

void CButtom::SetTextureID( int tex_id )
{
	m_TexID = tex_id;
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

HRESULT IGA3DDialog::Init( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;

	//初始化面片、ui纹理
	GetUITexture()->Init(m_pd3dDevice);
	GetIGA3DFont()->Init(m_pd3dDevice);

	//......................
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
        
	m_Frame.SetTexture( GetUITexture()->GetTexture( 0 ) );
	m_Frame.Render();

	//............
	int x = GetMessageManager()->m_xCursor;
	int y = GetMessageManager()->m_yCursor; 
	m_ButtomOK.Draw( x, y );
	m_ButtomCancel.Draw( x, y );

	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, false );

}

void IGA3DDialog::Destory()
{
	GetIGA3DFont()->Destory();
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
	
	if( m_Msg.empty() )
	{
		return false;	//没有消息
	}
	
	//OutputDebugString("有一个消息...........");

	*msg = m_Msg.front();

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
	m_Msg.push(msg);
}

void CIGA3DMessageManager::RemoveMsg()
{
	m_Msg.pop();
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
	if( GetKeyState(VK_RBUTTON) < 0 )	//鼠标右键按下状态
	{
		flg1 = true;
		if( !flg )
		{
			flg = true;
			
			//发送一个鼠标按下状态
			SIGA3DMSG msg;
			msg.key = IGA3D_MSG_RBUTTONDOWN;
			msg.x   = m_xCursor;
			msg.y   = m_yCursor;
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
			msg.key = IGA3D_MSG_RBUTTONUP;
			msg.x   = m_xCursor;
			msg.y   = m_yCursor;
			SendIGA3DMessage( msg );

		}
	}
}
