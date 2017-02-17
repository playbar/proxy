#include "MouseMessageUI.h"
#include "../Game.h"
#include "FLXUI.h"
#include "../Table/SkillInfoTable.h"
#include "ShopUI.h"
#include "SkillUI.h"


void CMouseMessageUI::Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;


	CUIStatic * pBack = AddStatic(UI_GAME_MOUSEMESSAGE_CONTROL_STATIC_BACK,x,y,width,height,true);
	pBack->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_MOUSEMESSAGE_STATIC_BACK]);
	pBack->SetUV(0.f,0.f,1.f,91.f/128.f);
	pBack->SetColor(0xaa000000);
	pBack->SetTextColor(0xffffffff);


	CUIListBox * pMessage = AddListBox(UI_GAME_MOUSEMESSAGE_CONTROL_STATIC_MESSAGE,x,y,width,width,true);
	pMessage->SetLineInfo(10,20);
}
bool CMouseMessageUI::Process(float x,float y)
{
	static ITEM Item;
	static SKILL Skill;
	static SHOP_ITEM ShopItem;
	if (g_pUI->m_pGameUI->m_pFlxUI->GetActivate())
	{
		RECT rect;
		ITEM_INFO * pItemInfo;
		for (int i = 0; i < 3; i ++)
		{
			CUIStatic * pUIStatic = g_pUI->m_pGameUI->m_pFlxUI->GetStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM1 + i);
			pUIStatic->GetRect(&rect);
			if (APP_Tools::appInRect((int)x,(int)y,rect))
			{
				
				pItemInfo = GetItemInfo(g_pUI->m_pGameUI->m_pFlxUI->m_Item[i].dwID);
				if (pItemInfo && Item.dwID != g_pUI->m_pGameUI->m_pFlxUI->m_Item[i].dwID &&
					g_pUI->m_pGameUI->m_pFlxUI->m_Item[i].dwNUM > 0)
				{
					Item.dwID = g_pUI->m_pGameUI->m_pFlxUI->m_Item[i].dwID;
					SetMessage(pItemInfo,x,y);
				}
				return CUIDialog::Process(x,y);
			}
		}
	}
	if (g_pUI->m_pGameUI->m_pItemUI->GetActivate())
	{
		RECT rect;
		ITEM_INFO * pItemInfo;
		for (int i = 0; i < 16; i ++)
		{
			CUIStatic * pUIStatic = g_pUI->m_pGameUI->m_pItemUI->GetStatic(UI_GAME_ITEM_CONTROL_STATIC_ITEM0 + i);
			pUIStatic->GetRect(&rect);
			if (APP_Tools::appInRect((int)x,(int)y,rect))
			{
				pItemInfo = GetItemInfo(g_pUI->m_pGameUI->m_pItemUI->m_item[i].dwID);
				if (pItemInfo && Item.dwID != g_pUI->m_pGameUI->m_pItemUI->m_item[i].dwID &&
					g_pUI->m_pGameUI->m_pItemUI->m_item[i].dwNUM > 0)
				{
					Item.dwID = g_pUI->m_pGameUI->m_pItemUI->m_item[i].dwID;
					SetMessage(pItemInfo,x,y);
				}
				return CUIDialog::Process(x,y);
			}
		}
	}
	if (g_pUI->m_pGameUI->m_pShopUI->GetActivate())
	{
		RECT rect;
		ITEM_INFO * pItemInfo;
		for (int i = 0; i < 10; i ++)
		{
			CUIStatic * pUIStatic = g_pUI->m_pGameUI->m_pShopUI->GetStatic(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + i);
			pUIStatic->GetRect(&rect);
			if (APP_Tools::appInRect((int)x,(int)y,rect))
			{
				pItemInfo = GetItemInfo(g_pUI->m_pGameUI->m_pShopUI->m_ShopItem[i].dwID);
				if (pItemInfo && ShopItem.dwID != g_pUI->m_pGameUI->m_pShopUI->m_ShopItem[i].dwID &&
					g_pUI->m_pGameUI->m_pShopUI->m_ShopItem[i].dwNum > 0)
				{
					ShopItem.dwID = g_pUI->m_pGameUI->m_pShopUI->m_ShopItem[i].dwID;
					SetMessage(pItemInfo,x,y);
				}
				return CUIDialog::Process(x,y);
			}
		}	
	}
	if (g_pUI->m_pGameUI->m_pSkillBookUI->GetActivate())
	{
		RECT rect;
		SKILL_INFO * pItemInfo;
		for (int i = 0; i < 12; i ++)
		{
			CUIStatic * pUIStatic = g_pUI->m_pGameUI->m_pSkillBookUI->GetStatic(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL + i);
			pUIStatic->GetRect(&rect);
			if (APP_Tools::appInRect((int)x,(int)y,rect))
			{
				pItemInfo = GetSkillInfo(g_pUI->m_pGameUI->m_pSkillBookUI->m_Skill[i].dwId);
				if (pItemInfo && Skill.dwId != g_pUI->m_pGameUI->m_pSkillBookUI->m_Skill[i].dwId &&
					g_pUI->m_pGameUI->m_pSkillBookUI->m_Skill[i].dwId > 0)
				{
					Skill.dwId = g_pUI->m_pGameUI->m_pSkillBookUI->m_Skill[i].dwId;
					CUIStatic * pBack = GetStatic(UI_GAME_MOUSEMESSAGE_CONTROL_STATIC_BACK);
					int j = (int)_tcslen(pItemInfo->szInfo);
					if (j*20 < 140)
					{
						pBack->SetWH((float)j*20,20.f);
					}
					else
					{
						pBack->SetWH(140.f,(int)(j*20/140.f) * 20.f);
					}
					pBack->SetXY(x,y);
					pBack->SetText(pItemInfo->szInfo);
					pBack->m_dwTextFlag = DT_WORDBREAK | DT_CENTER;
					SetActivate(true);
				}
				return CUIDialog::Process(x,y);
			}
		}
	}
	if (g_pUI->m_pGameUI->m_pSkillUI->GetActivate())
	{
		RECT rect;
		SKILL_INFO * pItemInfo;
		for (int i = 0; i < 12; i ++)
		{
			CUIStatic * pUIStatic = g_pUI->m_pGameUI->m_pSkillUI->GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + i);
			pUIStatic->GetRect(&rect);
			if (APP_Tools::appInRect((int)x,(int)y,rect))
			{
				pItemInfo = GetSkillInfo(g_pUI->m_pGameUI->m_pSkillUI->m_Skill[i].dwId);
				if (pItemInfo && Skill.dwId != g_pUI->m_pGameUI->m_pSkillUI->m_Skill[i].dwId &&
					g_pUI->m_pGameUI->m_pSkillUI->m_Skill[i].dwId > 0)
				{
					Skill.dwId = g_pUI->m_pGameUI->m_pSkillUI->m_Skill[i].dwId;
					CUIStatic * pBack = GetStatic(UI_GAME_MOUSEMESSAGE_CONTROL_STATIC_BACK);
					int j = (int)_tcslen(pItemInfo->szInfo);
					if (j*20 < 140)
					{
						pBack->SetWH((float)j*20,20.f);
					}
					else
					{
						pBack->SetWH(140.f,(int)(j*20/140.f) * 20.f);
					}
					pBack->SetXY(x,y);
					pBack->SetText(pItemInfo->szInfo);
					pBack->m_dwTextFlag = DT_WORDBREAK | DT_CENTER;
					SetActivate(true);
				}
				return CUIDialog::Process(x,y);
			}
		}
	}
	SetActivate(false);
	Item.dwID = 99;
	Skill.dwId = 99;
	ShopItem.dwID = 99;
	return CUIDialog::Process(x,y);
}
void CMouseMessageUI::Release()
{
	CUIDialog::Release();
}
void CMouseMessageUI::Render()
{
	CUIDialog::Render();
}
bool CMouseMessageUI::Event(ST_UI_MSG * pMsg)
{
	return false;
}
bool CMouseMessageUI::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CMouseMessageUI::SetMessage(ITEM_INFO * pItemInfo,float x,float y)
{
	CUIStatic * pBack = GetStatic(UI_GAME_MOUSEMESSAGE_CONTROL_STATIC_BACK);
	int j = (int)_tcslen(pItemInfo->szInfo);
	if (j*20 < 140)
	{
		pBack->SetWH((float)j*20,20.f);
	}
	else
	{
		pBack->SetWH(140.f,(int)(j*20/140.f) * 20.f);
	}
	pBack->SetXY(x,y);
	pBack->SetText(pItemInfo->szInfo);
	pBack->m_dwTextFlag = DT_WORDBREAK | DT_CENTER;
	SetActivate(true);
}

