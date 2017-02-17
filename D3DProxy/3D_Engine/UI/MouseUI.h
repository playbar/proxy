
#pragma once

#include "UIDialog.h"


enum
{
	UI_GAME_MOUSE_CLICKDOWN_ITEM,
};

class CUIMouse : public CUIDialog
{
public:
	CUIMouse()
	{
		m_iLDown = -1;
		m_iLUp = 2;
		m_iRDown = -1;
		m_iRUp = 2;
	}
	~CUIMouse()
	{
	}
public:

	POINT	m_MousePos;

	POINT	m_LastPos;

	int		m_iLDown;
	int		m_iLUp;
	int		m_iRDown;
	int		m_iRUp;

public:

	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );

	void Release()
	{
		CUIDialog::Release();
	}

	void Render();

	bool Process( float x, float y );

	bool Event( ST_UI_MSG * pMsg );
};