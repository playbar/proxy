#pragma once

#include "UIDialog.h"
#include "../Player.h"

enum
{
	UI_GAME_MONSTER_CONTROL_STATIC_BACK = 0,
	UI_GAME_MONSTER_CONTROL_STATIC_PLAYER,
	UI_GAME_MONSTER_CONTROL_STATIC_HP,
	UI_GAME_MONSTER_CONTROL_STATIC_MP,
	UI_GAME_MONSTER_CONTROL_STATIC_LV,
	UI_GAME_MONSTER_CONTROL_STATIC_P,
	UI_GAME_MONSTER_CONTROL_STATIC_P1,
};

class CMonsterUI : public CUIDialog
{
public:
	CMonsterUI()
	{
		m_pMonster = NULL;
	}
	~CMonsterUI()
	{

	}

	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );
	void Release();
	void Render();
	bool Process(float x,float y);
	bool WndProc(UINT msg, WPARAM wParam, LPARAM lParam );
	bool Event(ST_UI_MSG * pMsg);

	TCHAR m_tcMonsterLV[20];

	CPlayer * m_pMonster;

protected:
private:
};