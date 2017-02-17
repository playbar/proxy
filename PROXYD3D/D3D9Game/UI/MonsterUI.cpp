#include "MonsterUI.h"
#include "UI.h"
#include "../Game.h"
#include "../GameDefine.h"

void CMonsterUI::Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	CUIStatic * pUIStatic = AddStatic(UI_GAME_MONSTER_CONTROL_STATIC_BACK,x,y,200,64,true);
	pUIStatic->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_PLAYER_STATIC_BACK]);
	pUIStatic->SetUV(0.f,0.f,1.f,1.f);
	pUIStatic->SetColor(0xff000000);

	CUIStatic * pUIStaticPlayer = AddStatic(UI_GAME_MONSTER_CONTROL_STATIC_PLAYER,x,y,64,64,true);
	pUIStaticPlayer->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_PLAYER_STATIC_PLAYER]);
	pUIStaticPlayer->SetUV(0.f,0.f,1.f,1.f);
	pUIStaticPlayer->SetTextColor(0xffff0000);

	CUIStatic * pUIStaticHP = AddStatic(UI_GAME_MONSTER_CONTROL_STATIC_HP,64 + x,35 + y,136,8,true);
	pUIStaticHP->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_PLAYER_STATIC_HP]);
	pUIStaticHP->SetUV(0.f,0.f,1.f,1.f);
	pUIStaticHP->SetTextColor(0xffff0000);
	//pUIStaticHP->SetColor(0xff00000);

	CUIStatic * pUIStaticMP = AddStatic(UI_GAME_MONSTER_CONTROL_STATIC_MP,64 + x,55 + y,136,8,true);
	pUIStaticMP->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_PLAYER_STATIC_MP]);
	pUIStaticMP->SetUV(0.f,0.f,1.f,1.f);
	pUIStaticMP->SetTextColor(0xffff0000);

	CUIStatic * pUIStaticLV = AddStatic(UI_GAME_MONSTER_CONTROL_STATIC_LV,100 + x,0 + y,100,100,true);

	CUIStatic * p = AddStatic(UI_GAME_MONSTER_CONTROL_STATIC_P,132 + x,20 + y,100,20,true);

	CUIStatic * p1 = AddStatic(UI_GAME_MONSTER_CONTROL_STATIC_P1,132 + x,40 + y,100,20,true);
}

void CMonsterUI::Release()
{
	CUIDialog::Release();
}

void CMonsterUI::Render()
{
	CUIDialog::Render();
}

bool CMonsterUI::Process(float x,float y)
{
	
	TCHAR tcPlayerlv[50];
	TCHAR tcPlayerhp[50];
	TCHAR tcPlayermp[50];
	TCHAR tcPlayerhpMax[50];
	TCHAR tcPlayermpMax[50];
	TCHAR tcP[10];
	if (m_pMonster != NULL)
	{
		_tcscpy(tcPlayerlv,L"lv:");
		APP_Tools::appIntToChar(m_pMonster->m_iLevel,m_tcMonsterLV);
		_tcscat(tcPlayerlv,m_tcMonsterLV);

		_tcscpy(tcP,L"/");
		APP_Tools::appIntToChar(m_pMonster->m_iHP,tcPlayerhp);
		APP_Tools::appIntToChar(m_pMonster->m_iMaxHP,tcPlayerhpMax);
		APP_Tools::appIntToChar(m_pMonster->m_iMP,tcPlayermp);
		APP_Tools::appIntToChar(m_pMonster->m_iMaxMP,tcPlayermpMax);
		_tcscat(tcPlayerhp,tcP);
		_tcscat(tcPlayerhp,tcPlayerhpMax);
		_tcscat(tcPlayermp,tcP);
		_tcscat(tcPlayermp,tcPlayermpMax);

		if (m_pMonster->m_iHP <= 0)
		{
			CUIStatic * p = GetStatic(UI_GAME_MONSTER_CONTROL_STATIC_P);
			p->SetText(L"ËÀÍö");
			p->SetTextColor(0xff000000);

			CUIStatic * p2 = GetStatic(UI_GAME_MONSTER_CONTROL_STATIC_HP);
			p2->SetWH(0.f,8.f);
			return CUIDialog::Process(x,y);

		}

		CUIStatic * p = GetStatic(UI_GAME_MONSTER_CONTROL_STATIC_P);
		p->SetText(tcPlayerhp);
		p->SetTextColor(0xff000000);

		CUIStatic * p1 = GetStatic(UI_GAME_MONSTER_CONTROL_STATIC_P1);
		p1->SetText(tcPlayermp);
		p1->SetTextColor(0xff000000);

		CUIStatic * p2 = GetStatic(UI_GAME_MONSTER_CONTROL_STATIC_HP);
		p2->SetWH((float)m_pMonster->m_iHP/m_pMonster->m_iMaxHP * 136.f,8.f);

		CUIStatic * p3 = GetStatic(UI_GAME_MONSTER_CONTROL_STATIC_MP);
		p3->SetWH((float)m_pMonster->m_iMP/m_pMonster->m_iMaxMP * 136.f,8.f);

		CUIStatic * pUIStaticLV = GetStatic(UI_GAME_MONSTER_CONTROL_STATIC_LV);
		pUIStaticLV->SetText(tcPlayerlv);
		pUIStaticLV->SetTextColor(0xff000000);
	}

	return CUIDialog::Process(x,y);
}

bool CMonsterUI::Event(ST_UI_MSG * pMsg)
{
	return false;
}
bool CMonsterUI::WndProc(UINT msg, WPARAM wParam, LPARAM lParam )
{
	return false;
}

