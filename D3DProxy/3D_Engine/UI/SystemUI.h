#pragma once

#include "UIDialog.h"


enum
{
	UI_GAME_SYSTEM_CONTROL_STATIC_BACK = 0,
	UI_GAME_SYSTEM_CONTROL_STATIC_BUTTON,
	UI_GAME_SYSTEM_CONTROL_STATIC_BUTTON0,
	UI_GAME_SYSTEM_CONTROL_STATIC_BUTTON1,

};

class CSystemUI : public CUIDialog
{
public:
	

	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );
	void Release();
	void Render();
	bool Process(float x,float y);
	bool WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool Event(ST_UI_MSG * pMsg);
protected:
private:
};