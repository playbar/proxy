

#pragma once

#include "UIDialog.h"
#include "ItemUI.h"
#include "PlayerUI.h"
#include "MonsterUI.h"
#include "SystemUI.h"
#include "ShopUI.h"
#include "FLXUI.h"
#include "SkillUI.h"
#include "MouseMessageUI.h"
#include "MiniMap.h"

class CGameUI : public CBaseUI
{
public:
	CGameUI()
	{
		for( int i = 0; i < MAX_GAME_DIALOG_NUM; i ++ )
		{
			m_pDialogs[i] = NULL;
		}
	}
	~CGameUI()
	{
	}


	DWORD				m_dwFocusId;
	CItemUI   *			m_pItemUI;
	CPlayerUI *			m_pPlayerUI;
	CMonsterUI *		m_pMonsterUI;
	CSystemUI *			m_pSystemUI;
	CShopUI *			m_pShopUI;
	CFlxUI *			m_pFlxUI;
	CSkillUI *			m_pSkillUI;
	CSkillBookUI *      m_pSkillBookUI;
	CMouseMessageUI *   m_pMouseMessageUI;
	CMiniMap   *        m_pMiniMap;


private:
		
	CUIDialog *			m_pDialogs[MAX_GAME_DIALOG_NUM];

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