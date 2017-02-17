#include "PlayerUI.h"
#include "UI.h"
#include "../Game.h"
#include "../GameDefine.h"

void CPlayerUI::Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	CUIStatic * pUIStatic = AddStatic(UI_GAME_PLAYER_CONTROL_STATIC_BACK,0,0,200,64,true);
	pUIStatic->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_PLAYER_STATIC_BACK]);
	pUIStatic->SetUV(0.f,0.f,1.f,1.f);
	pUIStatic->SetColor(0xff000000);

	CUIStatic * pUIStaticPlayer = AddStatic(UI_GAME_PLAYER_CONTROL_STATIC_PLAYER,0,0,64,64,true);
	pUIStaticPlayer->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_PLAYER_STATIC_PLAYER]);
	pUIStaticPlayer->SetUV(0.f,0.f,1.f,1.f);
	pUIStaticPlayer->SetTextColor(0xffff0000);

	CUIStatic * pUIStaticHP = AddStatic(UI_GAME_PLAYER_CONTROL_STATIC_HP,64,35,136,8,true);
	pUIStaticHP->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_PLAYER_STATIC_HP]);
	pUIStaticHP->SetUV(0.f,0.f,1.f,1.f);
	pUIStaticHP->SetTextColor(0xffff0000);
	//pUIStaticHP->SetColor(0xff00000);

	CUIStatic * pUIStaticMP = AddStatic(UI_GAME_PLAYER_CONTROL_STATIC_MP,64,55,136,8,true);
	pUIStaticMP->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_PLAYER_STATIC_MP]);
	pUIStaticMP->SetUV(0.f,0.f,1.f,1.f);
	pUIStaticMP->SetTextColor(0xffff0000);
	
	CUIStatic * pUIStaticLV = AddStatic(UI_GAME_PLAYER_CONTROL_STATIC_LV,100,0,100,100,true);
	
	CUIStatic * p = AddStatic(UI_GAME_PLAYER_CONTROL_STATIC_P,132,20,100,20,true);

	CUIStatic * p1 = AddStatic(UI_GAME_PLAYER_CONTROL_STATIC_P1,132,40,100,20,true);
	
}

void CPlayerUI::Release()
{
	CUIDialog::Release();
}

void CPlayerUI::Render()
{
	CUIDialog::Render();
}

bool CPlayerUI::Process(float x,float y)
{
	
	CPlayer * pPlayer = g_pGame->m_pPlayerManager->GetUser();
	TCHAR tcPlayerlv[10];
	TCHAR tcPlayerhp[20];
	TCHAR tcPlayermp[20];
	TCHAR tcPlayerhpMax[20];
	TCHAR tcPlayermpMax[20];
	TCHAR tcP[10];
	if (pPlayer != NULL)
	{
		_tcscpy(tcPlayerlv,L"lv:");
		APP_Tools::appIntToChar(pPlayer->m_iLevel,m_tcPlayerLV);
		_tcscat(tcPlayerlv,m_tcPlayerLV);
       
		_tcscpy(tcP,L"/");
		APP_Tools::appIntToChar(pPlayer->m_iHP,tcPlayerhp);
		APP_Tools::appIntToChar(pPlayer->m_iMaxHP,tcPlayerhpMax);
		APP_Tools::appIntToChar(pPlayer->m_iMP,tcPlayermp);
		APP_Tools::appIntToChar(pPlayer->m_iMaxMP,tcPlayermpMax);
		_tcscat(tcPlayerhp,tcP);
		_tcscat(tcPlayerhp,tcPlayermpMax);
		_tcscat(tcPlayermp,tcP);
		_tcscat(tcPlayermp,tcPlayermpMax);
		

		CUIStatic * p = GetStatic(UI_GAME_PLAYER_CONTROL_STATIC_P);
		p->SetText(tcPlayerhp);
		p->SetTextColor(0xff000000);
		
		CUIStatic * p1 = GetStatic(UI_GAME_PLAYER_CONTROL_STATIC_P1);
		p1->SetText(tcPlayermp);
		p1->SetTextColor(0xff000000);

		CUIStatic * p2 = GetStatic(UI_GAME_PLAYER_CONTROL_STATIC_HP);
		p2->SetWH((float)pPlayer->m_iHP/pPlayer->m_iMaxHP * 136.f,8.f);

 		CUIStatic * p3 = GetStatic(UI_GAME_PLAYER_CONTROL_STATIC_MP);
 		p3->SetWH((float)pPlayer->m_iMP/pPlayer->m_iMaxMP * 136.f,8.f);

		CUIStatic * pUIStaticLV = GetStatic(UI_GAME_PLAYER_CONTROL_STATIC_LV);
		pUIStaticLV->SetText(tcPlayerlv);
		pUIStaticLV->SetTextColor(0xff000000);
	}

	return CUIDialog::Process(x,y);
}
bool CPlayerUI::Event(ST_UI_MSG * pMsg)
{
	return false;
}
bool CPlayerUI::WndProc(UINT msg, WPARAM wParam, LPARAM lParam )
{
	return false;
}