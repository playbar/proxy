#pragma once

#include "UIDialog.h"
#include "d3dx8.h"


enum
{
	UI_GAME_MINIMAP_CONTROL_STATIC_BACK = 0,
	UI_GAME_MINIMAP_CONTROL_STATIC_MAP,
	UI_GAME_MINIMAP_CONTROL_STATIC_ROLE,
};

class CMiniMap : public CUIDialog
{
public:
	CMiniMap()
	{
		m_fView = 70.f;
	}
	~CMiniMap()
	{
	}

	float m_fView;
	//CPlayerManager * m_pPlayerManager;
	D3DXVECTOR3 m_vPos;

	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );
	void Release();
	void Render();
	bool Process(float x,float y);
	bool WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool Event(UI_MSG * pMsg);
protected:
private:
};