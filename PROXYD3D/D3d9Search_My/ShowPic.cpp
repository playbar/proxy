

#include "ToolFunction.h"
#include "DefineConst.h"
#include "ShowPic.h"

CShowPic				*g_pShowPic = NULL;			//自定义类，显示搜索图片

CShowPic::CShowPic(void)
:	m_spD3DDevice(NULL),
	m_spTextureSearch(NULL),
	m_spVBSearchPic(NULL),
	m_bMouseOver(false),
	m_bShowSearch( false ),
	m_bProxyMsg( false ),
	m_bIsMove(false),
	m_iCurrentFrame(0),
	m_bMouseBtnDown( false),
	m_nMouseD(0),
	m_nMouseO(0),
	m_nMouseL(0)
{
}

CShowPic::~CShowPic(void)
{
	//m_D3D9ManageTexture.D3D9TextureDestory();
	if ( m_spVBSearchPic != NULL )
	{
		m_spVBSearchPic->Release();
		m_spVBSearchPic = NULL;
	}
	if (m_spTextureSearch != NULL )
	{
		m_spTextureSearch->Release();
		m_spTextureSearch = NULL;
	}
	m_spD3DDevice = NULL;
	
}

bool FileExist(const char* path)
{
	return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES || 
		GetLastError() != ERROR_FILE_NOT_FOUND;
}

//绘制矩形
struct SVertex
{
	float x,y,z,rhw;
	float u,v;
	const static DWORD _fvf = D3DFVF_XYZRHW|D3DFVF_TEX1;
};

void CShowPic::DrawRect( )
{
	//定义顶点
	SVertex verteices[] = 
	{
		{ m_ixPos,				m_iyPos ,				0.0f, 1.0f,		0.0f, 0.0f },
		{ m_ixPos + m_iWidth,	m_iyPos,				0.0f, 1.0f,		1.0f, 0.0f },
		{ m_ixPos + m_iWidth,	m_iyPos + m_iHeight,	0.0f, 1.0f,		1.0f, 1.0f },
		{ m_ixPos,				m_iyPos + m_iHeight,	0.0f, 1.0f,		0.0f, 1.0f }
	};
	m_spD3DDevice->SetTexture( 0, m_spTextureSearch );
	m_spD3DDevice->SetFVF(SVertex::_fvf);
	m_spD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, verteices, sizeof(SVertex) );

}


HRESULT CShowPic::InitPic(IDirect3DDevice9 * pD3DDevice )
{
	//SHOWOUTPUTDEBUG(CShowPic::InitPic_begin);
	m_spD3DDevice = pD3DDevice;
	char	szConfigFile[_MAX_PATH];
	char	szModulePath[_MAX_PATH];
	char	szPicPathFileName[MAX_PATH];
	char	szVideoFileName[MAX_PATH];
	memset(szConfigFile, 0, MAX_PATH );
	memset(szModulePath, 0, MAX_PATH );
	memset(szPicPathFileName, 0, MAX_PATH );

	::GetModuleFileNameA(NULL, szModulePath, _MAX_PATH);
	char	*p	= strrchr(szModulePath, '\\');
	if(p == NULL)
		p= strrchr(szModulePath, '/');
	*++p	=0;

	//SHOWOUTPUTDEBUG(CShowPic::InitPic_1);

	sprintf(szConfigFile, "%s%s%s", szModulePath, ADINFO_EMMOA_RES_FLASH, "search.ini");
	GetPrivateProfileStringA("SearchButton", "TextureName" ,"texture.tga", szPicPathFileName, MAX_PATH, szConfigFile);
	//DWORD dwError = GetLastError();
	sprintf(m_szPicFileName, "%s%s%s", szModulePath, ADINFO_EMMOA_RES_FLASH, szPicPathFileName );

	GetPrivateProfileStringA("SearchButton", "VideoTexture", "video.swf", szVideoFileName, MAX_PATH, szConfigFile);
	sprintf(m_szVideoFileName,  "%s%s%s", szModulePath, ADINFO_EMMOA_RES_FLASH, szVideoFileName );


	m_bIsMove = GetPrivateProfileIntA("SearchButton","IsMove",0, szConfigFile);

	m_ixPos = GetPrivateProfileIntA("SearchButton", "x",200, szConfigFile);
	m_iyPos = GetPrivateProfileIntA("SearchButton", "y",0, szConfigFile);

	m_iWidth = GetPrivateProfileIntA("SearchButton", "width",100, szConfigFile);
	m_iHeight = GetPrivateProfileIntA("SearchButton","height",50, szConfigFile);

	//SHOWOUTPUTDEBUG(CShowPic::InitPic_2);
	//////////////////////////////////////////////////////////////////////////
	//初始化纹理列表
	char szList[MAX_CLIP_RECT_NUM];
	GetPrivateProfileStringA("SearchButton", "StateList_1", "1", szList, MAX_PATH, szConfigFile);
	char *needle=",";
	int i = 0;
	memset(m_nMouseLeave, 0, sizeof(UINT) * MAX_CLIP_RECT_NUM );
	const char* buf = strtok( szList, needle);
	while ( buf != NULL )
	{
		m_nMouseLeave[i ++ ]= atoi(buf);
		buf = strtok(NULL, needle );
	}

	memset( szList, 0, sizeof(szList) );
	i = 0;
	memset(m_nMouseOver, 0, sizeof(UINT) * MAX_CLIP_RECT_NUM );
	GetPrivateProfileStringA("SearchButton", "StateList_2", "1", szList, MAX_PATH, szConfigFile);
	buf = strtok( szList, needle);
	while ( buf != NULL )
	{
		m_nMouseOver[i ++ ]= atoi(buf);
		buf = strtok(NULL, needle );
	}

	memset( szList, 0, sizeof(szList) );
	i = 0;
	memset(m_nMouseDown, 0, sizeof(UINT) * MAX_CLIP_RECT_NUM );
	GetPrivateProfileStringA("SearchButton", "StateList_3", "1", szList, MAX_PATH, szConfigFile);
	buf = strtok( szList, needle);
	while ( buf != NULL )
	{
		m_nMouseDown[i ++ ]= atoi(buf);
		buf = strtok(NULL, needle );
	}

	//SHOWOUTPUTDEBUG(CShowPic::InitPic_3);
	//////////////////////////////////////////////////////////////////////////
	//初始化纹理矩阵列表
	memset(m_rectTexture, 0, sizeof(RECT) * MAX_CLIP_RECT_NUM );
	char szIndex[20];
	for (int index = 1; index <= MAX_CLIP_RECT_NUM; index ++ )
	{
		memset( szIndex, 0, 20);
		sprintf( szIndex, "sl_%d", index );
		m_rectTexture[index-1].left	= GetPrivateProfileIntA( szIndex, "RectLeft",	0,	szConfigFile );
		m_rectTexture[index-1].top	= GetPrivateProfileIntA( szIndex, "RectTop",	0,	szConfigFile );
		m_rectTexture[index-1].right	= GetPrivateProfileIntA( szIndex, "RectRight",	0,	szConfigFile );
		m_rectTexture[index-1].bottom = GetPrivateProfileIntA( szIndex, "RectBottom",	0,	szConfigFile );
		if ( (m_rectTexture[index-1].left ==0) &&
			 (m_rectTexture[index-1].top ==0) &&
			 (m_rectTexture[index-1].right ==0) &&
			 (m_rectTexture[index-1].bottom ==0) )
		{
			break;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////

	TCHAR szwPicFileName[ MAX_PATH * sizeof(TCHAR)];
	MultiByteToWideChar (CP_ACP, 0, m_szPicFileName, -1, szwPicFileName, MAX_PATH);  

	if ( ! FileExist(m_szPicFileName))
	{
		SHOWOUTPUTDEBUG(CShowPic::InitPic_FileName);
		return S_OK;
	}
	//SHOWOUTPUTDEBUG(CShowPic::InitPic_4);

	/*char szOutput[MAX_PATH ];
	sprintf(szOutput, "m_spD3DDevice = %08x,m_szPicFileName = %s", m_spD3DDevice, m_szPicFileName );
	OutputDebugStringA(szOutput );*/

	HRESULT hr = E_FAIL;
	
	try
	{
		if ( m_spTextureSearch != NULL )
		{
			m_spTextureSearch->Release();
			m_spTextureSearch = NULL;
		}
		D3DXCreateTextureFromFileA( m_spD3DDevice, m_szPicFileName, &m_spTextureSearch ) ;


		D3DSURFACE_DESC desc;
		if( SUCCEEDED( m_spTextureSearch->GetLevelDesc( 0, &desc ) ) )
		{
			m_fPicWidth = (float)desc.Width;
			m_fPicHeight = (float)desc.Height;
		}
		else
		{
			m_fPicHeight = 512;
			m_fPicWidth = 512;
		}

		//////////////////////////////////////////////////////////////////////////

		if ( m_spVBSearchPic != NULL )
		{
			m_spVBSearchPic->Release();
			m_spVBSearchPic = NULL;
		}
		m_spD3DDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX), 0, 
			D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT , &m_spVBSearchPic,NULL );
	}
	catch(...)
	{
		m_fPicHeight = 512;
		m_fPicWidth = 512;
		SHOWOUTPUTDEBUG(CShowPic::InitPic_Except_In);
	}
	
	
	//SHOWOUTPUTDEBUG(CShowPic::InitPic_5);

	m_fU1 = m_rectTexture[0].left / m_fPicWidth;
	m_fV1 = m_rectTexture[0].bottom / m_fPicHeight;

	m_fU2 = m_rectTexture[0].left / m_fPicWidth;
	m_fV2 = m_rectTexture[0].top / m_fPicHeight;

	m_fU3 = m_rectTexture[0].right / m_fPicWidth;
	m_fV3 = m_rectTexture[0].bottom / m_fPicHeight;

	m_fU4 = m_rectTexture[0].right / m_fPicWidth;
	m_fV4 = m_rectTexture[0].top / m_fPicHeight;

	//SHOWOUTPUTDEBUG(CShowPic::InitPic_End);
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
	m_spD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );

	//创建并设置投影矩阵

	D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
	//D3DXMatrixOrthoLH( &m_matProj, 1, 1, 1, 100);
	m_spD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );
	
	//left bottom 位置 
	D3DXVECTOR3 vWorldLB;
	ScreenPosToWorldPos(m_hGameWnd, m_matProj, m_matView, m_ixPos, m_iyPos + m_iHeight, vWorldLB );
	
	//right ， top 位置
	D3DXVECTOR3 vWorldRT;
	ScreenPosToWorldPos(m_hGameWnd, m_matProj, m_matView, m_ixPos + m_iWidth, m_iyPos, vWorldRT );

	float fWidth = 0.0f;
	float fHeight = 0.0f;
	fWidth =  abs(vWorldRT.x - vWorldLB.x) * 20.0f;
	fHeight = abs(vWorldRT.y - vWorldLB.y) * 20.0f;


	D3DXMatrixTranslation( &m_matWorld, vWorldLB.x *abs( m_vEyePt.z ), vWorldLB.y *abs( m_vEyePt.z) , 0);
	m_spD3DDevice->SetTransform( D3DTS_WORLD, &m_matWorld );

	m_vMin.x = vWorldLB.x * abs( m_vEyePt.z );
	m_vMin.y = vWorldLB.y * abs( m_vEyePt.z );
	m_vMin.z = 0.0f;

	m_vMax.x = vWorldLB.x * abs( m_vEyePt.z ) + fWidth;
	m_vMax.y = vWorldLB.y * abs( m_vEyePt.z ) + fHeight;
	m_vMax.z = 0.0f;

	///////////////////////////////////////////////////////////////////////////
	//static long s_time = GetTickCount();
	//if( GetTickCount() - s_time > 100 )
	//{
	//	int i = m_fPicWidth / 128;

	//	if ( ! m_bMouseOver )
	//	{
	//		if (m_iCurrentFrame > 30 )
	//		{
	//			m_iCurrentFrame = 0;
	//		}

	//		//////////////////////////////////////////////////////////////////////////
	//		m_fU1 = ( m_iCurrentFrame % i) * 128 / m_fPicWidth;
	//		m_fV1 = ((m_iCurrentFrame / i ) +1) * 64  / m_fPicHeight;

	//		m_fU2 = (m_iCurrentFrame % i) * 128 / m_fPicWidth;
	//		m_fV2 = (m_iCurrentFrame / i) * 64  / m_fPicHeight;

	//		m_fU3 = ( (m_iCurrentFrame % i )+ 1 ) * 128 / m_fPicWidth;
	//		m_fV3 = ((m_iCurrentFrame / i ) +1) * 64  / m_fPicHeight;

	//		m_fU4 = ((m_iCurrentFrame % i )+ 1 ) * 128 / m_fPicWidth;
	//		m_fV4 = ( m_iCurrentFrame / i) * 64  / m_fPicHeight;

	//		m_iCurrentFrame ++;
	//		s_time = GetTickCount();
	//	}
	//	else
	//	{
	//		m_fU1 = ( 30 % i) * 128 / m_fPicWidth;
	//		m_fV1 = ((30 / i ) +1) * 64  / m_fPicHeight;

	//		m_fU2 = (30 % i) * 128 / m_fPicWidth;
	//		m_fV2 = (30 / i) * 64  / m_fPicHeight;

	//		m_fU3 = ( (30 % i )+ 1 ) * 128 / m_fPicWidth;
	//		m_fV3 = ((30 / i ) +1) * 64  / m_fPicHeight;

	//		m_fU4 = ((30 % i )+ 1 ) * 128 / m_fPicWidth;
	//		m_fV4 = ( 30 / i) * 64  / m_fPicHeight;
	//	}
	//	if ( m_bMouseBtnDown )
	//	{
	//		m_fU1 = ( 31 % i) * 128 / m_fPicWidth;
	//		m_fV1 = ((31 / i ) +1) * 64  / m_fPicHeight;

	//		m_fU2 = (31 % i) * 128 / m_fPicWidth;
	//		m_fV2 = (31 / i) * 64  / m_fPicHeight;

	//		m_fU3 = ( (31 % i )+ 1 ) * 128 / m_fPicWidth;
	//		m_fV3 = ((31 / i ) +1) * 64  / m_fPicHeight;

	//		m_fU4 = ((31 % i )+ 1 ) * 128 / m_fPicWidth;
	//		m_fV4 = ( 31 / i) * 64  / m_fPicHeight;
	//	}
	//}

	//////////////////////////////////////////////////////////////////////////
	static long s_time = GetTickCount();
	if( GetTickCount() - s_time > 100 )
	{
		if ( ! m_bMouseOver )
		{
			UINT nCurrent = m_nMouseLeave[m_nMouseL] - 1;
			m_fU1 = m_rectTexture[nCurrent].left / m_fPicWidth;
			m_fV1 = m_rectTexture[nCurrent].bottom / m_fPicHeight;

			m_fU2 = m_rectTexture[nCurrent].left / m_fPicWidth;
			m_fV2 = m_rectTexture[nCurrent].top / m_fPicHeight;

			m_fU3 = m_rectTexture[nCurrent].right / m_fPicWidth;
			m_fV3 = m_rectTexture[nCurrent].bottom / m_fPicHeight;

			m_fU4 = m_rectTexture[nCurrent].right / m_fPicWidth;
			m_fV4 = m_rectTexture[nCurrent].top / m_fPicHeight;

			m_nMouseL ++;
			if ( m_nMouseLeave[m_nMouseL] ==0  )
			{
				m_nMouseL = 0;
			}
			/*char szOutput[500];
			sprintf(szOutput, " num = %d, u1= %f, v1=%f, u2 = %f, v2=%f, u3=%f, v3=%f,u4=%f,v4= %f", 
				m_nMouseL,m_fU1, m_fV1,m_fU2, m_fV2, m_fU3, m_fV3, m_fU4, m_fV4);
			OutputDebugStringA(szOutput);*/

		}
		else
		{
			UINT nCurrent = m_nMouseOver[m_nMouseO] -1 ;
			m_fU1 = m_rectTexture[nCurrent].left / m_fPicWidth;
			m_fV1 = m_rectTexture[nCurrent ].bottom / m_fPicHeight;

			m_fU2 = m_rectTexture[nCurrent ].left / m_fPicWidth;
			m_fV2 = m_rectTexture[nCurrent ].top / m_fPicHeight;

			m_fU3 = m_rectTexture[nCurrent ].right / m_fPicWidth;
			m_fV3 = m_rectTexture[nCurrent ].bottom / m_fPicHeight;

			m_fU4 = m_rectTexture[nCurrent ].right / m_fPicWidth;
			m_fV4 = m_rectTexture[nCurrent ].top / m_fPicHeight;

			m_nMouseO ++;
			if ( m_nMouseOver[m_nMouseO] ==0  )
			{
				m_nMouseO = 0;
			}
		}
		if ( m_bMouseBtnDown )
		{
			UINT nCurrent = m_nMouseDown[m_nMouseD] - 1;
			m_fU1 = m_rectTexture[nCurrent].left / m_fPicWidth;
			m_fV1 = m_rectTexture[nCurrent].bottom / m_fPicHeight;

			m_fU2 = m_rectTexture[nCurrent].left / m_fPicWidth;
			m_fV2 = m_rectTexture[nCurrent].top / m_fPicHeight;

			m_fU3 = m_rectTexture[nCurrent].right / m_fPicWidth;
			m_fV3 = m_rectTexture[nCurrent].bottom / m_fPicHeight;

			m_fU4 = m_rectTexture[nCurrent].right / m_fPicWidth;
			m_fV4 = m_rectTexture[nCurrent].top / m_fPicHeight;

			m_nMouseD ++;
			if ( m_nMouseDown[m_nMouseD] ==0  )
			{
				m_nMouseD = 0;
			}
		}
		s_time = GetTickCount();
	}

	//////////////////////////////////////////////////////////////////////////

	CUSTOMVERTEX customerVertices[4] =
	{
		{ 0,		0,			0.0f,  m_fU1,	m_fV1 },   
		{ 0,		fHeight,	0.0f,  m_fU2,	m_fV2 },	
		{ fWidth,	0,			0.0f,  m_fU3,	m_fV3 },	
		{ fWidth,	fHeight,	0.0f,  m_fU4,	m_fV4 }
	};

	char szOutput[256];
	sprintf( szOutput, "%08x", m_spD3DDevice );

	VOID* pVertices;
	m_spVBSearchPic->Lock( 0, sizeof(customerVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, customerVertices, sizeof(customerVertices) );
	m_spVBSearchPic->Unlock();
	//////////////////////////////////////////////////////////////////////////

	return hr;
}

HRESULT CShowPic::RenderPic()
{
	HRESULT hr = S_OK;
	m_spD3DDevice->SetRenderState(D3DRS_ZENABLE, false);

	//////////////////////////////////////////////////////////////////////////
	m_spD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   true );
	m_spD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_spD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_spD3DDevice->SetTexture( 0, m_spTextureSearch );
	m_spD3DDevice->SetStreamSource( 0, m_spVBSearchPic, 0, sizeof(CUSTOMVERTEX) );
	m_spD3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	m_spD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	
	return hr;
}

bool CShowPic::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool bresult = false;
	static bool			s_bMouseBtnDown = false;
	static bool			s_bMouseBtnUp = false;
	static bool			s_bMouseMove = false;

	RAY ray;
	
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		s_bMouseBtnDown = false;
		s_bMouseMove	= false;
		if ( m_bMouseOver  )
		{
			bresult = true;
			SetCursor( LoadCursor(NULL,MAKEINTRESOURCE(32649) ));
			if ( ! s_bMouseBtnUp )
			{
				m_bShowSearch = !m_bShowSearch;
				m_bMouseBtnDown = false;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		s_bMouseBtnDown = true;
		if ( m_bMouseOver )
		{
			m_bMouseBtnDown = true;
			s_bMouseMove = true;
			bresult = true;
			SetCursor( LoadCursor(NULL,MAKEINTRESOURCE(32649) ));
		}
		break;

	case WM_MOUSEMOVE:
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(m_hGameWnd, &mousePos);
		GetRay(m_hGameWnd, m_matProj, m_matView,m_vEyePt, mousePos.x, mousePos.y, &ray);
		if (RayCheck(m_vMin, m_vMax, 1, ray.vOrig, ray.vVector ))
		{
			m_bMouseOver = true;
			bresult = true;
			SetCursor( LoadCursor(NULL,MAKEINTRESOURCE(32649) ));
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			m_bMouseOver = false;
			m_bMouseBtnDown = false;
		}

		if ( m_bIsMove && s_bMouseBtnDown && s_bMouseMove )
		{
			s_bMouseBtnUp = true;
			m_ixPos = mousePos.x - m_iWidth / 2;
			m_iyPos = mousePos.y - m_iHeight / 2;
		}
		else
		{
			s_bMouseBtnUp = false;
		}
		break;
	case WM_KEYDOWN:
		switch ( wParam )
		{
		case VK_F2:
			//m_bShowSelf = !m_bShowSelf;
			break;
		}

		break;
	}

	if(m_bShowSearch)
	{
		m_bProxyMsg = true;
	}
	else
	{
		m_bProxyMsg = false;
	}

	//m_bShowSearch = true;

	return bresult;
}


void CShowPic::ScreenPosToWorldPos(const HWND hWnd,const D3DXMATRIXA16 &matProj,const D3DXMATRIXA16& matView,int x, int y, D3DXVECTOR3 &vWorldPos)
{
	RECT rect;
	GetClientRect( hWnd, &rect );
	//计算屏幕空间的坐标(-1,-1)～(1,1)
	D3DXVECTOR3 v;
	v.x =  ( 2 * x  / (float)rect.right - 1 ) / matProj._11;
	v.y =  -( 2 * y  / (float)rect.bottom - 1 ) / matProj._22;
	v.z =  1.0f;
	//获得视图变换矩阵的逆矩阵
	D3DXMATRIXA16 m;
	D3DXMatrixInverse( &m, NULL, &matView );

	//把上述坐标转换成世界坐标
	D3DXVECTOR3 vTmp = v;
	v.x = vTmp.x * m.m[0][0] + vTmp.y * m.m[1][0] + vTmp.z * m.m[2][0] ;
	v.y = vTmp.x * m.m[0][1] + vTmp.y * m.m[1][1] + vTmp.z * m.m[2][1] ;
	v.z = vTmp.x * m.m[0][2] + vTmp.y * m.m[1][2] + vTmp.z * m.m[2][2] ;
	vWorldPos = v;

}

void CShowPic::GetRay(HWND hWnd, D3DXMATRIXA16 &matProj, D3DXMATRIXA16& matView, D3DXVECTOR3 vPos, int x, int y, LPRAY ray )
{
	ray->vOrig = vPos;
	RECT rect;
	GetClientRect( hWnd, &rect );

	//计算屏幕空间的坐标(-1,-1)～(1,1)
	D3DXVECTOR3 v;
	v.x =  ( 2 * x  / (float)rect.right - 1 ) / matProj._11;
	v.y =  -( 2 * y  / (float)rect.bottom - 1 ) / matProj._22;
	v.z =  1.0f;
	//获得视图变换矩阵的逆矩阵
	D3DXMATRIXA16 m;
	D3DXMatrixInverse( &m, NULL, &matView );

	//把上述坐标转换成世界坐标
	D3DXVECTOR3 vTmp = v;
	v.x = vTmp.x * m.m[0][0] + vTmp.y * m.m[1][0] + vTmp.z * m.m[2][0] ;
	v.y = vTmp.x * m.m[0][1] + vTmp.y * m.m[1][1] + vTmp.z * m.m[2][1] ;
	v.z = vTmp.x * m.m[0][2] + vTmp.y * m.m[1][2] + vTmp.z * m.m[2][2] ;

	ray->vVector = v;

}


bool  CShowPic::RayCheck(D3DXVECTOR3 vMin,D3DXVECTOR3 vMax,float fScaling, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir)
{
	D3DXMATRIX  mMat;

	D3DXMatrixScaling(&mMat,fScaling,fScaling,fScaling);
	AppVec3Transform(vMin,mMat);
	AppVec3Transform(vMax,mMat);
	vMin = vMin;
	vMax = vMax;
	if (D3DXBoxBoundProbe(&vMin,&vMax,&vPos,&vDir))
		return true;
	else 
		return false;
}

//向量*变换矩阵
void CShowPic::AppVec3Transform( D3DXVECTOR3 & vPoint, D3DXMATRIX matTrans )
{
	D3DXVECTOR3 vTmp = vPoint;
	vPoint.x = vTmp.x * matTrans.m[0][0] + vTmp.y * matTrans.m[1][0] + vTmp.z * matTrans.m[2][0] + matTrans.m[3][0];
	vPoint.y = vTmp.x * matTrans.m[0][1] + vTmp.y * matTrans.m[1][1] + vTmp.z * matTrans.m[2][1] + matTrans.m[3][1];
	vPoint.z = vTmp.x * matTrans.m[0][2] + vTmp.y * matTrans.m[1][2] + vTmp.z * matTrans.m[2][2] + matTrans.m[3][2];
}
