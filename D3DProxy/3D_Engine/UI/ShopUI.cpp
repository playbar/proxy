#include "ShopUI.h"
#include "../Game.h"


void CShopUI::Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	CUIStatic * pUIStatic = AddStatic(UI_GAME_SHOP_CONTROL_STATIC_BACK,x,y,362.f,500.f,true);
	pUIStatic->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SHOP_STATIC_BACK]);
	pUIStatic->SetUV(0.f,0.f,362.f/512.f,500.f/512.f);
	pUIStatic->SetTextColor(0xffff0000);
	pUIStatic->SetColor(0xff000000);

	CUIButton * pUIButton = AddButton(UI_GAME_SHOP_CONTROL_STATIC_BUTTON,x+341.f,y+15.f,20.f,19.f,true);
	pUIButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_NORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ON],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_DOWN]);
	pUIButton->SetUV(0.f,0.f,20.f/32.f,19.f/32.f);
	pUIButton->SetTextColor(0xffff0000);
	pUIButton->SetColor(0xff000000);

	for(int i = 0; i < 10; i ++)
	{
		CUIStatic * pUIStaticItem = AddStatic(UI_GAME_SHOP_CONTROL_STATIC_ITEM0+i,x + 21.f + i%2*173.f,y + 84.f + i%5*54.f,38.f,38.f,true);
		pUIStaticItem->SetUV(0.f,0.f,35.f/64.f,33.f/64.f);
		pUIStaticItem->SetTextColor(0xff000000);
		pUIStaticItem->SetColor(0xff000000);

		CUIListBox * pUIListBoxMessage = AddListBox(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + 10 + i,x + 68.f + i%2*173.f,y + 84.f + i%5*54.f,104.f,48.f,true);
		pUIListBoxMessage->SetLineInfo(2,30);
	}


 		CUIStatic * pUIStaticItem1 = GetStatic(UI_GAME_SHOP_CONTROL_STATIC_ITEM0);
		m_ShopItem[0].dwID = 0;
		m_ShopItem[0].dwNum = 1;
		ITEM_INFO * pItemInfo = GetItemInfo(m_ShopItem[0].dwID);
		pUIStaticItem1->SetTexture(m_pBaseUI->m_pTextures[pItemInfo->dwPicId]);
		CUIListBox * pUIListBoxMessage1 = GetListBox(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + 10);
		pUIListBoxMessage1->InsertLine(pItemInfo->szName,0xffffffff,DT_CENTER);
  		TCHAR tcMoney[20];
 		APP_Tools::appIntToChar(pItemInfo->dwMoney,tcMoney);
		TCHAR tcMoney1[10];
		_tcscpy(tcMoney1,L" ½ð±Ò");
		_tcscat(tcMoney,tcMoney1);
		pUIListBoxMessage1->InsertLine(tcMoney,0xffffffff,DT_CENTER);


  		CUIStatic * pUIStaticItem6 = GetStatic(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + 6);
		m_ShopItem[6].dwID = 1;
		m_ShopItem[6].dwNum = 5;
		pItemInfo = GetItemInfo(m_ShopItem[6].dwID);
  		pUIStaticItem6->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_1]);
		CUIListBox * pUIListBoxMessage6 = GetListBox(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + 6 + 10);
		pUIListBoxMessage6->InsertLine(pItemInfo->szName,0xffffffff,DT_CENTER);
		APP_Tools::appIntToChar(pItemInfo->dwMoney,tcMoney);
		_tcscat(tcMoney,tcMoney1);
		pUIListBoxMessage6->InsertLine(tcMoney,0xffffffff,DT_CENTER);

		
		CUIStatic * pUIStaticItem5 = GetStatic(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + 5);
		m_ShopItem[5].dwID = 3;
		m_ShopItem[5].dwNum = 2;
		pItemInfo = GetItemInfo(m_ShopItem[5].dwID);
		pUIStaticItem5->SetTexture(m_pBaseUI->m_pTextures[pItemInfo->dwPicId]);
		CUIListBox * pUIListBoxMessage5 = GetListBox(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + 5 + 10);
		pUIListBoxMessage5->InsertLine(pItemInfo->szName,0xffffffff,DT_CENTER);
		APP_Tools::appIntToChar(pItemInfo->dwMoney,tcMoney);
		_tcscat(tcMoney,tcMoney1);
		pUIListBoxMessage5->InsertLine(tcMoney,0xffffffff,DT_CENTER);

		CUIStatic * pUIStaticItem7 = GetStatic(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + 7);
		m_ShopItem[7].dwID = 4;
		m_ShopItem[7].dwNum = 1;
		pItemInfo = GetItemInfo(m_ShopItem[7].dwID);
		pUIStaticItem7->SetTexture(m_pBaseUI->m_pTextures[pItemInfo->dwPicId]);
		CUIListBox * pUIListBoxMessage7 = GetListBox(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + 7 + 10);
		pUIListBoxMessage7->InsertLine(pItemInfo->szName,0xffffffff,DT_CENTER);
		APP_Tools::appIntToChar(pItemInfo->dwMoney,tcMoney);
		_tcscat(tcMoney,tcMoney1);
		pUIListBoxMessage7->InsertLine(tcMoney,0xffffffff,DT_CENTER);

		CUIStatic * pUIStaticItem8 = GetStatic(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + 8);
		m_ShopItem[8].dwID = 2;
		m_ShopItem[8].dwNum = 1;
		pItemInfo = GetItemInfo(m_ShopItem[8].dwID);
		pUIStaticItem8->SetTexture(m_pBaseUI->m_pTextures[pItemInfo->dwPicId]);
		CUIListBox * pUIListBoxMessage8 = GetListBox(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + 8 + 10);
		pUIListBoxMessage8->InsertLine(pItemInfo->szName,0xffffffff,DT_CENTER);
		APP_Tools::appIntToChar(pItemInfo->dwMoney,tcMoney);
		_tcscat(tcMoney,tcMoney1);
		pUIListBoxMessage8->InsertLine(tcMoney,0xffffffff,DT_CENTER);
}

bool CShopUI::Process(float x,float y)
{
	return CUIDialog::Process(x,y);
}
void CShopUI::Release()
{
	CUIDialog::Release();
}
void CShopUI::Render()
{
	CUIDialog::Render();
}
bool CShopUI::Event(ST_UI_MSG * pMsg)
{
	switch(pMsg->msg)
	{
	case BUTTON_CLICK:
		switch(pMsg->dwControlId)
		{
		case UI_GAME_SHOP_CONTROL_STATIC_BUTTON:
			SetActivate(false);
			break;
		}
		break;
	}
	return false;
}
bool CShopUI::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bActivate)return false;
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	switch(msg)
	{
	case WM_RBUTTONDOWN:
		{
			if (m_bMouseItem)
			{
				m_bMouseItem = false;
				CUIStatic * pStaticMouse =g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
				pStaticMouse->SetActivate(false);
			}
			for (int i = 0; i < 10; i++)
			{
				ITEM_INFO * pItemInfo = GetItemInfo(m_ShopItem[i].dwID);
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + i);
				RECT rect;
				pUIStatic->GetRect(&rect);
				if (APP_Tools::appInRect(x,y,rect) && pItemInfo && g_pUI->m_pGameUI->m_pItemUI->m_iMoney >= (int)pItemInfo->dwMoney)
				{
					if (g_pUI->m_pGameUI->m_pItemUI->AddItem(m_ShopItem[i].dwID,1))
					{
						g_pUI->m_pGameUI->m_pItemUI->m_iMoney -=(int)(pItemInfo->dwMoney);
						APP_Tools::appIntToChar(g_pUI->m_pGameUI->m_pItemUI->m_iMoney,g_pUI->m_pGameUI->m_pItemUI->m_tcMoney);
						g_pUI->m_pGameUI->m_pItemUI->m_pUIStaticMoney->SetText(g_pUI->m_pGameUI->m_pItemUI->m_tcMoney);
					}
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
		}
		break;
	case WM_LBUTTONDOWN:
		if (APP_Tools::appInRect(x,y,(int)m_fX,(int)m_fY,(int)m_fWidth ,(int)m_fHeight))
		{
			if (g_pUI->m_pGameUI->m_pItemUI->m_bMouseItem)
			{
				ITEM_INFO * pItemInfo = GetItemInfo(g_pUI->m_pGameUI->m_pItemUI->m_item[g_pUI->m_pGameUI->m_pItemUI->m_iMouseItem].dwID);
				g_pUI->m_pGameUI->m_pItemUI->m_iMoney += (int)(pItemInfo->dwMoney)/2 * g_pUI->m_pGameUI->m_pItemUI->m_item[g_pUI->m_pGameUI->m_pItemUI->m_iMouseItem].dwNUM;

				g_pUI->m_pGameUI->m_pItemUI->m_item[g_pUI->m_pGameUI->m_pItemUI->m_iMouseItem].dwNUM = 0;
				g_pUI->m_pGameUI->m_pItemUI->m_bMouseItem = false;

				APP_Tools::appIntToChar(g_pUI->m_pGameUI->m_pItemUI->m_iMoney,g_pUI->m_pGameUI->m_pItemUI->m_tcMoney);
				g_pUI->m_pGameUI->m_pItemUI->m_pUIStaticMouseItem->SetActivate(false);
				g_pUI->m_pGameUI->m_pItemUI->m_pUIStaticMouse->SetActivate(false);
				g_pUI->m_pGameUI->m_pItemUI->m_pUIStaticMoney->SetText(g_pUI->m_pGameUI->m_pItemUI->m_tcMoney);
			}
			else
			{
				for (int i = 0; i < 10; i++)
				{
					ITEM_INFO * pItemInfo = GetItemInfo(m_ShopItem[i].dwID);
					CUIStatic * pUIStatic = GetStatic(UI_GAME_SHOP_CONTROL_STATIC_ITEM0 + i);
					RECT rect;
					pUIStatic->GetRect(&rect);
					if (APP_Tools::appInRect(x,y,rect) && pItemInfo && (int)pItemInfo->dwMoney <= g_pUI->m_pGameUI->m_pItemUI->m_iMoney)
					{
						CUIStatic * pStaticMouse =g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
						pStaticMouse->SetActivate(true);
						pStaticMouse->SetTexture(m_pBaseUI->m_pTextures[pItemInfo->dwPicId]);
						m_bMouseItem = true;
						m_ShopMouseItem.dwID = pItemInfo->dwId;
						m_ShopMouseItem.dwNum = 1;
					}
				}
			}
			return true;
		}
		break;
	}
	return false;
}


