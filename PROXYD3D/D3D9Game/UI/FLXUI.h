 #pragma once
#include "UIDialog.h"
#include "../Table/FLXInfoTable.h"
#include "../Table/ItemInfoTable.h"
#include "ItemUI.h"

enum 
{
	UI_GAME_FLX_CONTROL_STATIC_BACK,
	UI_GAME_FLX_CONTROL_STATIC_ITEM1,
	UI_GAME_FLX_CONTROL_STATIC_ITEM2,
	UI_GAME_FLX_CONTROL_STATIC_ITEM3,
	UI_GAME_FLX_CONTROL_STATIC_BOTTON,
	UI_GAME_FLX_CONTROL_STATIC_LOAD,
	UI_GAME_FLX_CONTROL_STATIC_LOADING,
};

class CFlxUI : public CUIDialog
{
public:

	CFlxUI()
	{
		m_Item[0].dwNUM = 0;
		m_Item[1].dwNUM = 0;
		m_Item[2].dwNUM = 0;
		m_bMouseItem = false;
		m_bLoading = false;
	};
	~CFlxUI()
	{

	};

	int m_iItem;
	ITEM m_Item[3];
	ITEM m_MouseItem;
	bool m_bMouseItem;

	bool m_bLoading;
	void Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );
	void Release();
	void Render();
	bool Process(float x,float y);
	bool WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool Event(ST_UI_MSG * pMsg);
protected:
private:
};