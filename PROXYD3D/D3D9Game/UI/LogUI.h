
#pragma once

#include "UIDialog.h"


enum
{
	UI_LOGIN_LOG_CONTROL_STATIC_BACK = 0,
	UI_LOGIN_LOG_CONTROL_STATIC_LOG,
	UI_LOGIN_LOG_CONTROL_EDITBOX_ID,
	UI_LOGIN_LOG_CONTROL_EDITBOX_PASSWORD,
	UI_LOGIN_LOG_CONTROL_BUTTON_LOG,
	UI_LOGIN_LOG_CONTROL_BUTTON_EXIT,
};


class CUILog : public CUIDialog
{
public:

	CUILog(){}
	~CUILog(){}


protected:

public:

	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );
	void Release();
	bool Process( float x, float y );
	void Render();
	bool WndProc( UINT msg, WPARAM wParam, LPARAM lParam );
	bool Event( ST_UI_MSG * pMsg );
	void OnCharId( WPARAM wParam );
	void OnCharPassword( WPARAM wParam );
	void OnKeyDownTab();
	void OnLButtonDownId();
	void OnLButtonDownPassword();
	void OnLogin();

};