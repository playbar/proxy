#pragma once

#include "UIDialog.h"
#include "../Table/ItemInfoTable.h"


enum
{
	UI_GAME_SHOP_CONTROL_STATIC_BACK = 0,
	UI_GAME_SHOP_CONTROL_STATIC_BUTTON,
	UI_GAME_SHOP_CONTROL_STATIC_SELL,
	UI_GAME_SHOP_CONTROL_STATIC_ITEM0,
};

struct SHOP_ITEM 
{
	DWORD dwID;
	DWORD dwNum;
};

class CShopUI : public CUIDialog
{
public:
	CShopUI()
	{
		m_bMouseItem = false;
	}
	~CShopUI()
	{

	}


	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );
	void Release();
	void Render();
	bool Process(float x,float y);
	bool WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool Event(ST_UI_MSG * pMsg);
	SHOP_ITEM m_ShopItem[10];
	SHOP_ITEM m_ShopMouseItem;
	bool m_bMouseItem;
protected:
private:
};