// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

// TODO: 在此处引用程序要求的附加头文件
#include "shellapi.h"

#include "dxut.h"
#include "dxutgui.h"
#pragma comment(lib,"..\\Lib\\DXGUI.lib")



//绘制矩形
struct SVertex
{
	float x,y,z,rhw;
	float u,v;
	const static DWORD _fvf = D3DFVF_XYZRHW|D3DFVF_TEX1;
};
void DrawRect( RECT& rc, LPDIRECT3DTEXTURE9 pTex = NULL );


class CMyButton
{
public:
	CMyButton()
	{
		m_Top		= 0;
		m_Bottom	= 1;
		m_Left		= 0;
		m_Right		= 1;
		m_bIsDown   = false;

		m_State = 0;

		for( int i = 0; i < 3; i++ )
		{
			m_pTexBuf[i] = NULL;		
		}
	}

public:
	void Set( int x, int y, int width, int height )
	{
		m_Top = y;
		m_Left = x;

		m_Right = m_Left + width;
		m_Bottom = m_Top + height;
	}

	void AddResource( UINT Index, LPDIRECT3DTEXTURE9 pTex )
	{
		if( Index < 3 )
		{
			m_pTexBuf[Index] = pTex;
		}
	}

	bool MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		//
		bool _bOldIsDown = m_bIsDown;

		//得到鼠标位置	
		int	x = (short)LOWORD(lParam);
		int	y = (short)HIWORD(lParam);
		bool bIsInRect = _IsInRect(x,y);
		switch( msg )
		{
		case WM_MOUSEMOVE:
			{
				if( bIsInRect )
				{
					m_State = 1;
				}
				else
				{
					m_State = 0;	
				}
			}
			break;

		case WM_LBUTTONDOWN:
			m_bIsDown = true;
			break;

		case WM_LBUTTONUP:
			m_bIsDown = false;
			break;
	
		}

		if( m_bIsDown && m_State == 1 )
		{
			m_State = 2;
			if(_bOldIsDown)
			{
				return false;
			}
			return true;
		}

		return false;
	}


	void Draw()
	{
		RECT rc;
		SetRect( &rc, m_Left, m_Top, m_Right, m_Bottom );
		DrawRect( rc, m_pTexBuf[m_State] );
	}

private:
	bool _IsInRect( int x, int y )
	{
		if( x > m_Left && x < m_Right && y > m_Top && y < m_Bottom )
		{
			return true;
		}
		return false;
	}

private:
	int m_Top;
	int m_Bottom;
	int m_Left;
	int m_Right;

	DWORD m_State;

	bool m_bIsDown;

	LPDIRECT3DTEXTURE9	m_pTexBuf[3];


};