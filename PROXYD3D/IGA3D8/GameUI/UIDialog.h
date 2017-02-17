
#pragma once

#include <crtdbg.h>
#include "D3Dstruct.h"
#include "Texture.h"
#include "define.h"
#include "D2D.h"
#include "Font.h"


//基础UI类块
class CBaseUI
{
public:
	CBaseUI(){}
	~CBaseUI(){}
	Array<CTexture *>		m_pTextures;
protected:
public:
	virtual bool Init() = 0;
	virtual void Render() = 0;
	virtual bool Process( float x, float y ) = 0;
	virtual void Release() = 0;
	virtual bool WndProc( UINT msg, WPARAM wParam, LPARAM lParam ) = 0;
	virtual bool Event( UI_MSG * pMsg ) = 0;
};

class CUIDialog;


//控件基类
class CUIControl : public CBase2D
{
public:
	CUIControl()
	{
		m_pVFont =NULL;
		m_dwTextColor = 0xff000000;
		m_dwTextFlag = DT_WORDBREAK;
		m_szText[0] = 0;

		m_fBWidth = 0.f;
		m_fBHeight = 0.f;

		m_fStartU = 0.f;
		m_fStartV = 0.f;
		m_fEndU   = 1.f;
		m_fEndV	  = 1.f;
		
		m_dwColor = 0xff000000;
	}
	~CUIControl()
	{
	}

	DWORD			m_dwTextFlag;
	float 			m_fX;				//相对窗口坐标
	float 			m_fY;
protected:


	TCHAR			m_szText[50];
	DWORD			m_dwTextColor;
	float			m_fBWidth;
	float			m_fBHeight;
	CVFont *		m_pVFont;

	bool			m_bActivate;

	DWORD			m_dwId;
	


	float			m_fWidth;
	float			m_fHeight;

	float			m_fStartU;
	float			m_fStartV;
	float			m_fEndU;
	float			m_fEndV;

	DWORD			m_dwColor;

	CUIDialog *		m_pDialog;


public:

	//bool			m_bLDown;
	//bool			m_bRDown;


	virtual void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CUIDialog * pDialog )
	{
		m_dwId = dwId;
		m_fX = x;
		m_fY = y;
		m_fWidth = width;
		m_fHeight = height;
		m_bActivate = bActivate;
		m_pDialog = pDialog;

		UpdateData( m_dwColor, width, height, x, y );
		SetUV(0.f,0.f,1.f,1.f);
	}

	virtual void Release() = 0;

	virtual bool Process( float x, float y ) = 0;

	virtual void Render()
	{
		if( !m_bActivate )
			return;
		CBase2D::Render();
	}

	virtual bool WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
	{
		return false;
	}

	void ShowText();

	void SetText( TCHAR * szText )
	{
		strcpy( m_szText,szText );
	}

	void SetTextColor( DWORD dwColor )
	{
		m_dwTextColor = dwColor;
	}

	void SetFont( CVFont * pFont )
	{
		m_pVFont = pFont;
	}

	void SetBL( float width, float height )
	{
		m_fBWidth = width;
		m_fBHeight = height;
	}

	void SetActivate( bool activate )
	{
		m_bActivate = activate;
	}
	bool GetActivate()
	{
		return m_bActivate;
	}

	void SetXY( float x, float y )
	{
		m_fX = x;
		m_fY = y;
		UpdateData( m_dwColor, m_fWidth, m_fHeight, m_fX, m_fY );
	}

	void SetWH( float width, float height )
	{
		m_fWidth = width;
		m_fHeight = height;
		UpdateData( m_dwColor, m_fWidth, m_fHeight, m_fX, m_fY );
	}

	void SetXYWH( float x, float y, float width, float height )
	{
		m_fX = x;
		m_fY = y;
		m_fWidth = width;
		m_fHeight = height;
		UpdateData( m_dwColor, m_fWidth, m_fHeight, m_fX, m_fY );
	}

	DWORD GetId()
	{
		return m_dwId;
	}

	void GetRect( RECT * rect )
	{
		rect->left = (long)m_fX;
		rect->right = rect->left + (long)m_fWidth;
		rect->top = (long)m_fY;
		rect->bottom = rect->top + (long)m_fHeight;
	}

	void SetTexture( CTexture * tex )
	{
		UpdateTexture( tex );
	}

	TCHAR * GetText()
	{
		return m_szText;
	}

	void SetColor( DWORD color )
	{
		m_dwColor = color;
		UpdateColor( m_dwColor );
	}

	void SetUV( float fStartU, float fStartV, float fEndU, float fEndV )
	{
		m_fStartU = fStartU;
		m_fStartV = fStartV;
		m_fEndU   = fEndU;
		m_fEndV   = fEndV;
		UpdateUV( fStartU, fStartV, fEndU, fEndV );
	}
};


class CUIStatic;
class CUIButton;
class CUIEditBox;
class CUIListBox;



//对话框基类
class CUIDialog
{
public:
	CUIDialog(){}
	~CUIDialog(){}

	CBaseUI *			m_pBaseUI;

protected:


	Array<CUIControl *>		m_pControls;


	bool			m_bActivate;

	DWORD			m_dwId;

	float			m_fX;					//相对窗口坐标
	float			m_fY;
	float			m_fWidth;
	float			m_fHeight;

	float			m_fMoveX;
	float			m_fMoveY;
	float			m_fMoveWidth;
	float			m_fMoveHeight;


	/*bool			m_bLDown;
	bool			m_bRDown;*/

	bool			m_bMoving;
	float			m_fMoveStartX;
	float			m_fMoveStartY;


public:
	void GetXY(float &x, float &y)
	{
		x = m_fX; 
		y = m_fY;
	}

public:
	

	virtual void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI ) = 0;

	virtual void Release()
	{
		for( int i = 0; i < m_pControls.Num(); i ++ )
		{
			SAFE_RELEASE( m_pControls[i] );
			SAFE_DELETE( m_pControls[i] );
		}
	}

	virtual bool Process( float x, float y );

	virtual void Render();

	virtual bool WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
	{
		for( int i = 0; i < m_pControls.Num(); i ++ )
		{
			if( m_pControls[i]->WndProc( msg, wParam, lParam ) )
				return true;
		}
		return false;
	}

	virtual bool Event( UI_MSG * pMsg )
	{
		return false;
	}

	void SendMsg( UI_MSG * pMsg );


	CUIButton * AddButton( DWORD dwId, float x, float y, float width, float height, bool bActivate );

	CUIStatic * AddStatic( DWORD dwId, float x, float y, float width, float height, bool bActivate );

	CUIEditBox * AddEditBox( DWORD dwId, float x, float y, float width, float height, bool bActivate );

	CUIListBox * AddListBox( DWORD dwId, float x, float y, float widht, float height, bool bActivate );

	CUIControl * GetControl( DWORD dwId )
	{
		for( int i=0; i < m_pControls.Num(); i ++ )
		{
			if( m_pControls[i]->GetId() == dwId )
				return m_pControls[i];
		}
		return NULL;
	}

	CUIStatic * GetStatic( DWORD dwId )
	{
		return (CUIStatic *)GetControl( dwId );
	}
	CUIButton * GetButton( DWORD dwId )
	{
		return (CUIButton *)GetControl( dwId );
	}
	CUIEditBox * GetEditBox( DWORD dwId )
	{
		return (CUIEditBox *)GetControl( dwId );
	}
	CUIListBox * GetListBox( DWORD dwId )
	{
		return (CUIListBox *)GetControl( dwId );
	}

	void SetActivate( bool flag )
	{
		m_bActivate = flag;
	}
	bool GetActivate()
	{
		return m_bActivate;
	}

	DWORD GetId()
	{
		return m_dwId;
	}
};



//按钮
class CUIButton : public CUIControl
{
public:
	CUIButton()
	{
		m_bMouseFlag = BUTTON_MOUSE_NORMAL;
		m_pTexOn = NULL;
		m_pTexDown = NULL;
	}
	~CUIButton()
	{
	}


	BYTE		m_bMouseFlag;

protected:

	


	CTexture *			m_pTexNormal;
	CTexture *			m_pTexOn;
	CTexture *			m_pTexDown;


public:
	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CUIDialog * pDialog )
	{
		CUIControl::Init( dwId, x, y, width, height, bActivate, pDialog );
	}

	void Release()
	{
	}

	bool Process( float x, float y );

	void Render()
	{
		if( !m_bActivate )
			return;
		CUIControl::Render();
		ShowText();
	}

	void InitTexture( CTexture * pTexNormal, CTexture * pTexOn, CTexture * pTexDown )
	{
		m_pTexNormal = pTexNormal;
		m_pTexOn = pTexOn;
		m_pTexDown = pTexDown;
		SetTexture( m_pTexOn );
	}

	void SetNormal()
	{
		SetTexture( m_pTexNormal );
		m_bMouseFlag = BUTTON_MOUSE_NORMAL;
	}
	void SetOn()
	{
		SetTexture( m_pTexOn );
		m_bMouseFlag = BUTTON_MOUSE_ON;
	}
	void SetDown()
	{
		SetTexture( m_pTexDown );
		m_bMouseFlag = BUTTON_MOUSE_DOWN;
	}

};



//静态框
class CUIStatic : public CUIControl
{
public:
	CUIStatic()
	{
	}
	~CUIStatic()
	{
	}

protected:

	bool m_bCanMove;

	//LPDIRECT3DTEXTURE9			m_pScrTex;

	
public:
	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CUIDialog * pDialog )
	{
		CUIControl::Init( dwId, x, y, width, height, bActivate, pDialog );
	}

	void Release()
	{
	}

	bool Process( float x, float y );

	void Render()
	{
		if( !m_bActivate )
			return;
		CUIControl::Render();
		ShowText();
	}

};



//文本编辑框
class CUIEditBox : public CUIControl
{
public:
	CUIEditBox()
	{
		m_bFocus = false;
		m_fPassTime = 0.f;
		m_bPassword = false;
		m_iCharNum = 0;

		m_szPassword[0] = 0;
	}
	~CUIEditBox()
	{
	}


	bool			m_bFocus;
	bool			m_bPassword;

protected:

	DOUBLE			m_fPassTime;

	bool			m_bCursor;

	int				m_iCharNum;

	TCHAR			m_szPassword[50];

public:

	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CUIDialog * pDialog )
	{
		CUIControl::Init( dwId, x, y, width, height, bActivate, pDialog );
	}

	void Release()
	{
	}

	bool Process( float x, float y );

	void Render();

	void OnChar( WPARAM wParam );

};


class CUIScrollBar : public CUIControl
{
public:
	CUIScrollBar()
	{
	}
	~CUIScrollBar()
	{
	}

protected:

	CUIButton			m_ButtonUp;
	CUIButton			m_ButtonMid;
	CUIButton			m_ButtonDown;


	bool				m_bMidDown;


public:

	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CUIDialog * pDialog )
	{
		CUIControl::Init( dwId, x, y, width, height, bActivate, pDialog );
	}

	void Release()
	{
	}

	bool Process( float x, float y );

	void Render();

	bool WndProc( UINT msg, WPARAM wParam, LPARAM lParam );
};



class CUIListBox : public CUIControl
{
public:
	CUIListBox()
	{
		m_wCurLineNum = 0;
	}
	~CUIListBox()
	{
	}


protected:

	WORD							m_wLineNum;

	WORD							m_wCurLineNum;

	Array<CUIStatic *>				m_pLineStatic;
	Array<TCHAR *>					m_pLineText;

	float                           m_fHeight;
public:

	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CUIDialog * pDialog )
	{
		CUIControl::Init( dwId, x, y, width, height, bActivate, pDialog );
	}

	void Release()
	{
		for( int i = 0; i < m_pLineStatic.Num(); i ++ )
		{
			SAFE_RELEASE( m_pLineStatic[i] );
			SAFE_DELETE( m_pLineStatic[i] );
			SAFE_DELETE_ARRAY( m_pLineText[i] );
		}
	}

	void SetXY(float x,float y);
	void Render();

	bool Process( float x, float y );

	bool WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

	void SetLineInfo( WORD wLineNum, WORD height )
	{
		m_wLineNum = wLineNum;
		m_fHeight = height;
		for( int i = 0; i < wLineNum; i ++ )
		{
			CUIStatic * p = new CUIStatic;
			m_pLineStatic.AddItem( p );
			p->Init( i, m_fX + m_fBWidth, m_fY + m_fBHeight + height * i, m_fWidth - 2 * m_fBWidth, height, true, NULL );
			TCHAR * pText = new TCHAR[50];
			m_pLineText.AddItem( pText );
		}
		
	}

	void InsertLine( TCHAR * szText, DWORD dwColor, DWORD dwFormat )
	{
		float fWidth = m_fWidth - 2 * m_fBWidth;
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 0;
		rect.bottom = 0;
		int i = 0;
		while( szText[i] != 0 )
		{
			m_pVFont->m_pFont->DrawText( szText, i+1, &rect, DT_CALCRECT, 0xff000000 );
			if( rect.right - rect.left > fWidth )
			{
				TCHAR tmp[100];
				int j = 0;
				for(  j = 0; j < i; j ++ )
				{
					tmp[j] = szText[j];
				}
				tmp[j] = 0;

				_tcscpy( m_pLineText[m_wCurLineNum], tmp );
				m_pLineStatic[m_wCurLineNum]->SetTextColor( dwColor );
				m_pLineStatic[m_wCurLineNum]->m_dwTextFlag = dwFormat;
				++ m_wCurLineNum;
				rect.right = 0;
				szText = &szText[i];
				i = 0;
				continue;
			}

			++ i;
		}
		
		_tcscpy( m_pLineText[m_wCurLineNum], szText );
		m_pLineStatic[m_wCurLineNum]->SetTextColor( dwColor );
		m_pLineStatic[m_wCurLineNum]->m_dwTextFlag = dwFormat;
		++ m_wCurLineNum;
	}

	void Clear()
	{
		m_wCurLineNum = 0;
	}

};