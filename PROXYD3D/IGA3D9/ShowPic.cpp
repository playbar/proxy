#include "stdafx.h"
#include "stdio.h"
#include "GlobalFunction.h"
#include "ShowPic.h"

CShowPic::CShowPic(void)
:	m_spD3DDevice(NULL),
	m_spTextureSearch(NULL),
	m_spVBSearchPic(NULL),
	m_bMouseOver(false),
	m_bShowSearch(false)
	//m_image(NULL)
{
}

CShowPic::~CShowPic(void)
{
	if ( m_spVBSearchPic != NULL )
	{
		m_spVBSearchPic.Release();
	}
	if (m_spTextureSearch != NULL )
	{
		m_spTextureSearch.Release();
	}
	if ( m_spD3DDevice != NULL )
	{
		m_spD3DDevice.Release();
	}
}

HRESULT CShowPic::InitPic()
{
	const char szFlashPath[] = "InterAdtive\\EMMOA\\res\\flash\\";
	char	szConfigFile[_MAX_PATH]	= { 0 };
	char	szModulePath[_MAX_PATH]	= { 0 };
	char	szPicPathFileName[MAX_PATH];
	::GetModuleFileNameA(NULL, szModulePath, _MAX_PATH);
	char	*p	= strrchr(szModulePath, '\\');
	if(p == NULL)
		p= strrchr(szModulePath, '/');
	*++p	=0;

	sprintf(szConfigFile, "%s%s%s", szModulePath,szFlashPath, "Flash.ini");
	GetPrivateProfileStringA("comm", "Name","", szPicPathFileName, MAX_PATH, szConfigFile);
	if ( strlen(szPicPathFileName) == 0 )
	{
		return 0;
	}
	sprintf(m_szPicFileName, "%s%s%s", szModulePath, szFlashPath, szPicPathFileName );
	if( ! FileExist(m_szPicFileName ))
	{
		return 0;
	}

	char szExtensionName[10];
	char *pchTmp = strrchr( m_szPicFileName, '.');
	if (pchTmp == NULL)
	{
		return S_FALSE;
	}

	m_ixPos = GetPrivateProfileIntA("pos", "x0",200, szConfigFile);
	m_iyPos = GetPrivateProfileIntA("pos", "y0",0, szConfigFile);

	m_iWidth = GetPrivateProfileIntA("pos", "x1",200, szConfigFile);
	m_iHeight = GetPrivateProfileIntA("pos", "y1",150, szConfigFile);

	HRESULT hr = S_FALSE;

	CUSTOMVERTEX customerVertices[] =
	{
		{ m_ixPos ,				m_iyPos,			 0.0f, 1.0f,  0.0f, 0.0f},   
		{ m_ixPos,				m_iyPos + m_iHeight, 0.0f, 1.0f,  0.0f, 1.0f},	
		{ m_ixPos + m_iWidth,   m_iyPos,			 0.0f, 1.0f,  1.0f, 0.0f},	
		{ m_ixPos + m_iWidth,	m_iyPos + m_iHeight, 0.0f, 1.0f,  1.0f, 1.0f }
	};

	if ( m_spVBSearchPic != NULL )
	{
		m_spVBSearchPic.Release();
	}
	m_spD3DDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED, &m_spVBSearchPic,NULL );
	VOID* pVertices;
	m_spVBSearchPic->Lock( 0, sizeof(customerVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, customerVertices, sizeof(customerVertices) );
	m_spVBSearchPic->Unlock();

	//if ( m_image != NULL)
	//{
	//	delete m_image;
	//	m_image = NULL;
	//}
	//m_image = new CxImage("e:\\1.gif", 2);
	//m_image->SetRetreiveAllFrames(true);
	//m_image->SetFrame(m_image->GetNumFrames());
	////m_image->Load(filename, type);
	//m_image->Save("z:\\aa.gif", 2 );
	return hr;
}

HRESULT CShowPic::InitTranslationMatrix()
{
	HRESULT hr = S_OK;

	//创建并设置观察矩阵
	//m_vEyePt( 0.0f, 0.0f, -20 );
	m_vEyePt.x = 0.0f;
	m_vEyePt.y = 0.0f;
	m_vEyePt.z = -30.0f;

	m_vLookatPt.x = m_vLookatPt.y = m_vLookatPt.z = 0.0f;

	m_vUpVec.x = 0.0f;
	m_vUpVec.y = 1.0f;
	m_vUpVec.z = 0.0f;

	D3DXMatrixLookAtLH( &m_matView, &m_vEyePt, &m_vLookatPt, &m_vUpVec );
	m_spD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );

	//创建并设置投影矩阵

	D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
	m_spD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );

	
	D3DXVECTOR3 vWorldPos;
	ScreenPosToWorldPos(m_hGameWnd, m_matProj, m_matView, m_ixPos, m_iyPos, vWorldPos );

	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixTranslation( &m_matWorld, vWorldPos.x *abs( m_vEyePt.z ), vWorldPos.y *abs( m_vEyePt.z) , 0);
	m_spD3DDevice->SetTransform( D3DTS_WORLD, &m_matWorld );


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
	m_spD3DDevice->SetRenderState(D3DRS_ZENABLE, false);
	m_spD3DDevice->SetTexture( 0, m_spTextureSearch );
	m_spD3DDevice->SetStreamSource( 0, m_spVBSearchPic, 0, sizeof(CUSTOMVERTEX) );
	m_spD3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	m_spD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
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

		GetRay(m_hGameWnd, m_matProj, m_matView,m_vEyePt, mousePos.x, mousePos.y, &ray);
		if (RayCheck(m_vMin, m_vMax, 1, ray.vOrig, ray.vVector ))
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
