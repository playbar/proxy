
#pragma once

#include "UIDialog.h"

#define MAX_LOADING_LENGTH 500.f

enum
{
	UI_LOADING_CONTROL_STATIC_BACK = 0,
	UI_LOADING_CONTROL_STATIC_LOADING,
	
};


class CUILoading : public CUIDialog
{
public:
	CUILoading()
	{
		m_fLoading = 0.f;
		m_pTextures[0] = new CTexture;
		m_pTextures[0]->CreateFromFile( GetCommonTexturePath( UI_TEXTURE_ID::COMMON_TEX_LOADING_BACK ), false );
		m_pTextures[1] = new CTexture;
		m_pTextures[1]->CreateFromFile( GetCommonTexturePath( UI_TEXTURE_ID::COMMON_TEX_LOADING_LOADING ), false );
	}
	~CUILoading()
	{
		SAFE_DELETE( m_pTextures[0] );
		SAFE_DELETE( m_pTextures[1] );
	}

protected:

	float			m_fLoading;

	CTexture *  m_pTextures[2];

public:

	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );

	void Release();

	void Render();

	bool Process( float x, float y );

	bool WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

	bool Event( ST_UI_MSG * pMsg );


	void UpdateLoading( float fLoading );
	void SetLoading( float fLoading );
};