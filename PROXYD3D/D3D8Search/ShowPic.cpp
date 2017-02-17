#include "dxstdafx.h"
#include "ShowPic.h"
#include "ToolFunction.h"



CShowPic::CShowPic(void)
:	m_pD3DDevice(NULL),
	m_pTextureSearchOver(NULL),
	m_pVBSearchPic(NULL),
	m_bMouseOver(false),
	m_bShowSearch(false)
{
}

CShowPic::~CShowPic(void)
{
	if (NULL != m_pTextureSearchOver )
	{
		m_pTextureSearchOver->Release();
		m_pTextureSearchOver = NULL;
	}
	if (NULL != m_pVBSearchPic )
	{
		m_pVBSearchPic->Release();
		m_pVBSearchPic = NULL;
	}
	if ( NULL != m_pD3DDevice )
	{
		m_pD3DDevice->Release();
	}
}

HRESULT CShowPic::InitPic()
{
	char	szConfigFile[_MAX_PATH]	= { 0 };
	char	szModulePath[512]	= { 0 };
	//char	szPicPathFile[MAX_PATH];
	::GetModuleFileNameA(NULL, szModulePath, _MAX_PATH);
	char	*p	= strrchr(szModulePath, '\\');
	if(p == NULL)
		p= strrchr(szModulePath, '/');
	*++p	=0;

	sprintf(szConfigFile, "%s%s", szModulePath, "SEARCH.INI");
	GetPrivateProfileStringA("SHOWPIC", "PicFileName","", m_szPicFileName, MAX_PATH, szConfigFile);
	m_ixPos = GetPrivateProfileIntA("SHOWPIC", "ixPos",200, szConfigFile);
	m_iyPos = GetPrivateProfileIntA("SHOWPIC", "iyPos",100, szConfigFile);
	//sprintf(m_szPicFileName, "%s%s", szModulePath, szPicPathFile);

	HRESULT hr = S_FALSE;
	TCHAR szwPicFileName[ MAX_PATH * sizeof(TCHAR)];
	MultiByteToWideChar (CP_ACP, 0, m_szPicFileName, -1, szwPicFileName, MAX_PATH);  
	OutputDebugStringW( szwPicFileName );
	V_RETURN( D3DXCreateTextureFromFile( m_pD3DDevice, szwPicFileName, &m_pTextureSearchOver ) );
	CUSTOMVERTEX customerVertices[] =
	{
		{ 0,   0,  0.0f,  0.0f, 1.0f},   
		{ 0,   2,  0.0f,  0.0f, 0.0f},	
		{ 2,   0,  0.0f,  1.0f, 1.0f},	
		{ 2,   2,  0.0f,  1.0f, 0.0f }
	};

	V_RETURN( m_pD3DDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED, &m_pVBSearchPic,NULL ) );
	VOID* pVertices;
	V_RETURN( m_pVBSearchPic->Lock( 0, sizeof(customerVertices), (void**)&pVertices, 0 ) );
	memcpy( pVertices, customerVertices, sizeof(customerVertices) );
	m_pVBSearchPic->Unlock();
	return hr;
}

HRESULT CShowPic::InitTranslationMatrix()
{
	HRESULT hr = S_OK;

	//创建并设置观察矩阵
	//m_vEyePt( 0.0f, 0.0f, -20 );
	m_vEyePt.x = 0.0f;
	m_vEyePt.y = 0.0f;
	m_vEyePt.z = -20.0f;

	m_vLookatPt.x = m_vLookatPt.y = m_vLookatPt.z = 0.0f;

	m_vUpVec.x = 0.0f;
	m_vUpVec.y = 1.0f;
	m_vUpVec.z = 0.0f;

	D3DXMatrixLookAtLH( &m_matView, &m_vEyePt, &m_vLookatPt, &m_vUpVec );
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );

	//创建并设置投影矩阵

	D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );

	
	D3DXVECTOR3 vWorldPos;
	APP_Tools::ScreenPosToWorldPos(m_hGameWnd, m_matProj, m_matView, m_ixPos, m_iyPos, vWorldPos );

	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixTranslation( &m_matWorld, vWorldPos.x *abs( m_vEyePt.z ), vWorldPos.y *abs( m_vEyePt.z) , 0);
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &m_matWorld );


	m_vMin.x = vWorldPos.x * abs( m_vEyePt.z );
	m_vMin.y = vWorldPos.y * abs( m_vEyePt.z );
	m_vMin.z = 0.0f;

	m_vMax.x = vWorldPos.x * abs( m_vEyePt.z ) + 2;
	m_vMax.y = vWorldPos.y * abs( m_vEyePt.z ) + 2;
	m_vMax.z = 0.0f;

	return hr;
}

HRESULT CShowPic::RenderPic()
{
	HRESULT hr = S_OK;
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, false);
	m_pD3DDevice->SetTexture( 0, m_pTextureSearchOver );
	m_pD3DDevice->SetStreamSource( 0, m_pVBSearchPic, 0, sizeof(CUSTOMVERTEX) );
	m_pD3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	return hr;
}

LRESULT CShowPic::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//OutputDebugStringA("CShowPic::MsgProc");
	RAY ray;
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		if ( m_bMouseOver )
		{
			//OutputDebugStringA("CShowPic::MsgProc  WM_LBUTTONDOWN");
			m_bShowSearch = !m_bShowSearch;
			/*if ( !m_bShowSearch )
			{
				OutputDebugStringA("CShowPic::MsgProc  WM_LBUTTONDOWN true");
				m_bShowSearch = true;
			}
			else
			{
				OutputDebugStringA("CShowPic::MsgProc  WM_LBUTTONDOWN false");
				m_bShowSearch = false;
			}*/
			//m_bShowSearch = true;
			//SetCursor( LoadCursor(NULL,MAKEINTRESOURCE(32649) ));
		}
		break;
	case WM_MOUSEMOVE:

		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(m_hGameWnd, &mousePos);

		APP_Tools::GetRay(m_hGameWnd, m_matProj, m_matView,m_vEyePt, mousePos.x, mousePos.y, &ray);
		if (APP_Tools::RayCheck(m_vMin, m_vMax, 1, ray.vOrig, ray.vVector ))
		{
			m_bMouseOver = true;
			SetCursor( LoadCursor(NULL,MAKEINTRESOURCE(32649) ));
		}
		else
		{
			m_bMouseOver = false;
		}

		break;
	case WM_KEYUP:

		break;
	}
	return S_OK;
}
