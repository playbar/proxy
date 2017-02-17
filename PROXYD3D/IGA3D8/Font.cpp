//#include "stdafx.h"
//
//IGA3DFont* GetIGA3DFont()
//{
//	static IGA3DFont font;
//	return &font;
//}
//
////--------------------------------------------------
//IGA3DFont::IGA3DFont()
//{
//	m_pd3dDevice = NULL;
//	m_pFont		 = NULL;
//}
//
//IGA3DFont::~IGA3DFont()
//{
//	Destory();
//}
//
//HRESULT IGA3DFont::Init( LPDIRECT3DDEVICE8 pd3dDevice )
//{
//	m_pd3dDevice = pd3dDevice;
//
//	//--------------------------------
//	if(	FAILED( D3DXCreateFont( m_pd3dDevice, 0,0,0, 0,0,0,0,0,0, "Arial", &m_pFont ) ) )
//	{
//		OutputDebugString("×ÖÌå´´½¨Ê§°Ü£¡");
//		return E_FAIL;
//	}
//
//
//	return S_OK;
//}
//
//void IGA3DFont::ShowText( WCHAR* text, int x, int y )
//{
//	RECT	rc;
//	rc.top  = y;
//	rc.left = x;
//	rc.right = x + 600;
//	rc.bottom = y + 60;
//	m_pFont->DrawTextW( NULL, text, -1, &rc, DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_VCENTER, 0xffffffff );
//}
//
//void IGA3DFont::Destory()
//{
//	if( m_pFont != NULL )
//	{
//		m_pFont->Release();
//		m_pFont = NULL;
//	}
//}