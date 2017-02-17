#pragma once

#include "UIDialog.h"
#include "../Table/ItemInfoTable.h"

#define ITEMUI_X 0
#define ITEMUI_Y 0

enum
{
	UI_GAME_ITEM_CONTROL_STATIC_BACK = 0,
	UI_GAME_ITEM_CONTROL_STATIC_BUTTON,
	UI_GAME_ITEM_CONTROL_STATIC_MONEY,
	UI_GAME_ITEM_CONTROL_STATIC_ITEM0,
};

struct ITEM 
{
	DWORD dwID;
	DWORD dwNUM;
};
class CItemUI : public CUIDialog
{
public:
	CItemUI()
	{
		memset(m_item,0,sizeof(ITEM) * 16);
		m_iMouseItem = 0;
		m_bMouseItem = false;
		m_iMoney = 1000;
		m_pUIStaticMoney = NULL;
		m_pUIStaticMouse = NULL;
		m_pUIStaticMouseItem = NULL;
		m_pItemInfo = NULL;
		m_bSetPos = false;
	}
	~CItemUI()
	{
		delete m_pUIStaticMoney;
		delete m_pUIStaticMouse;
		delete m_pItemInfo;
		delete m_pUIStaticMouseItem;

		m_pUIStaticMoney = NULL;
		m_pUIStaticMouse = NULL;
		m_pUIStaticMouseItem = NULL;
		m_pItemInfo = NULL;
		
	}
		
	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );
	void Release();
	void Render();
	bool Process(float x,float y);
	bool WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool Event(ST_UI_MSG * pMsg);
	bool AddItem(DWORD dwId,DWORD dwNum);
	void CreateItem(D3DXVECTOR3 vpos,D3DXVECTOR3 vscaling,float fRotationY,int iType,int iNum);
	void SetPos(float x,float y);

	float m_fXTemp;
	float m_fYTemp;
	bool  m_bSetPos;

	int m_iMoney;
	int m_iMouseItem;
	bool m_bMouseItem;
	TCHAR m_tcMoney[20];
	ITEM m_item[16];
	ITEM_INFO * m_pItemInfo;
	CUIStatic * m_pUIStaticMouseItem;
	CUIStatic * m_pUIStaticMouse;
	CUIStatic * m_pUIStaticMoney;
protected:
private:
};