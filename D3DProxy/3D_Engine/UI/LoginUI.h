

#pragma once


#include "LogUI.h"
#include "../D3D/SkinMesh.h"
#include "../D3D/StaticMesh.h"



class CLoginUI : public CBaseUI
{
public:
	CLoginUI()
	{
		for( int i = 0; i < MAX_LOGIN_DIALOG_NUM; i ++ )
		{
			m_pDialogs[i] = NULL;
		}
	}

	~CLoginUI(){}
	CUILog *				m_pLogUI;
protected:
	CUIDialog *				m_pDialogs[MAX_LOGIN_DIALOG_NUM];
	//POINT					m_MousePos;
public:

	bool Init();

	void Release();

	bool Process( float x, float y );

	void Render();

	bool WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

	bool Event( ST_UI_MSG * pMsg );

	CUIDialog * GetDialog( DWORD dwId )
	{
		return m_pDialogs[(int)dwId];
	}
};