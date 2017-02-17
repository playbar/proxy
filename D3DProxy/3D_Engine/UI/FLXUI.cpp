#include "FLXUI.h"
#include "../Game.h"
#include "../GameDefine.h"

void CFlxUI::Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	CUIStatic * pUIStatic = AddStatic(UI_GAME_FLX_CONTROL_STATIC_BACK,x,y,width,height,true);
	pUIStatic->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BACK]);
	pUIStatic->SetUV(0.f,0.f,300.f/512.f,400.f/512.f);
	pUIStatic->SetColor(0xaa000000);

	CUIStatic * pUIStaticItem1 = AddStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM1,x + 25.f,y + 10.f,51.f,39.f,true);
	pUIStaticItem1->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_FLX_STATIC_ITEM]);
	pUIStaticItem1->SetUV(0.f,0.f,35.f/64.f,33.f/64.f);
	pUIStaticItem1->SetTextColor(0xffff0000);
	pUIStaticItem1->SetColor(0xff000000);

	CUIStatic * pUIStaticItem2 = AddStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM2,x + 200.f + 25.f,y + 10.f,51.f,39.f,true);
	pUIStaticItem2->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_FLX_STATIC_ITEM]);
	pUIStaticItem2->SetUV(0.f,0.f,35.f/64.f,33.f/64.f);
	pUIStaticItem2->SetTextColor(0xffff0000);
	pUIStaticItem2->SetColor(0xff000000);

	CUIStatic * pUIStaticItem3 = AddStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM3,x + 100.f + 25.f,y + 200.f,51.f,39.f,true);
	pUIStaticItem3->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_FLX_STATIC_ITEM]);
	pUIStaticItem3->SetUV(0.f,0.f,35.f/64.f,33.f/64.f);
	pUIStaticItem3->SetTextColor(0xffff0000);
	pUIStaticItem3->SetColor(0xff000000);

	CUIButton * pUIButton = AddButton(UI_GAME_FLX_CONTROL_STATIC_BOTTON,x + 68.f,y + 100.f,160.f,24.f,true);
	pUIButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_NORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_ON],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_DOWN]);
	pUIButton->SetUV(0.f,0.f,160.f/256.f,24.f/256.f);
	pUIButton->SetColor(0xff000000);
	pUIButton->SetText(L"ºÏ³É");
	pUIButton->m_dwTextFlag = DT_CENTER | DT_VCENTER;
	pUIButton->SetTextColor(0xffffffff);

	CUIStatic * pUIStaticLoad = AddStatic(UI_GAME_FLX_CONTROL_STATIC_LOAD,x + 45.f,y + 100.f,205.f,25.f,false);
	pUIStaticLoad->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_FLX_STATIC_LOAD]);
	pUIStaticLoad->SetUV(0.f,0.f,205.f/256.f,25.f/32.f);

	CUIStatic * pUIStaticLoading = AddStatic(UI_GAME_FLX_CONTROL_STATIC_LOADING,x + 52.f, y + 107.f,0.f,11.f,true);
	pUIStaticLoading->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_FLX_STATIC_LOADING]);
	pUIStaticLoading->SetUV(0.f,0.f,8.f/16.f,10.f/16.f);
}
bool CFlxUI::Process(float x,float y)
{
	if (m_bLoading)
	{
		static int iTemp = 0;
		CUIStatic * pUIStatic = GetStatic(UI_GAME_FLX_CONTROL_STATIC_LOADING);
		pUIStatic->SetWH((float)iTemp,11.f);
		iTemp ++;
		if (iTemp >= 193)
		{
			iTemp = 0;
			pUIStatic->SetWH(0.f,11.f);
			m_bLoading = false;
			CUIStatic * pStatic1 = GetStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM1);
			CUIStatic * pStatic2 = GetStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM2);
			TCHAR tcNumTemp[10];
			CUIStatic * pStatic = GetStatic(UI_GAME_FLX_CONTROL_STATIC_BOTTON);
			pStatic->SetActivate(true);
			CUIStatic * pStatic3 = GetStatic(UI_GAME_FLX_CONTROL_STATIC_LOAD);
			pStatic3->SetActivate(false);
			for (int i = 0; i < MAX_FLX_ITEM; i ++)
			{
				if (g_FlxItemInfo[i].FLX_ITEM1_ID == m_Item[0].dwID && g_FlxItemInfo[i].FLX_ITEM1_NUM <= m_Item[0].dwNUM &&
					g_FlxItemInfo[i].FLX_ITEM2_ID == m_Item[1].dwID && g_FlxItemInfo[i].FLX_ITEM2_NUM <= m_Item[1].dwNUM )

				{	
					m_Item[0].dwNUM -= g_FlxItemInfo[i].FLX_ITEM1_NUM;
					m_Item[1].dwNUM -= g_FlxItemInfo[i].FLX_ITEM2_NUM;
				}
				else if (g_FlxItemInfo[i].FLX_ITEM1_ID == m_Item[1].dwID && g_FlxItemInfo[i].FLX_ITEM1_NUM <= m_Item[1].dwNUM &&
					g_FlxItemInfo[i].FLX_ITEM2_ID == m_Item[0].dwID && g_FlxItemInfo[i].FLX_ITEM2_NUM <= m_Item[0].dwNUM)
				{
					m_Item[1].dwNUM -= g_FlxItemInfo[i].FLX_ITEM1_NUM;
					m_Item[0].dwNUM -= g_FlxItemInfo[i].FLX_ITEM2_NUM;
				}
				else continue;

				if (m_Item[0].dwNUM <= 0)
				{
					pStatic1->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_FLX_STATIC_ITEM]);
					pStatic1->SetTextColor(0x00000000);
				}
				if (m_Item[1].dwNUM <= 0)
				{
					pStatic2->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_FLX_STATIC_ITEM]);
					pStatic2->SetTextColor(0x00000000);
				}
				APP_Tools::appIntToChar(m_Item[0].dwNUM,tcNumTemp);
				pStatic1->SetText(tcNumTemp);
				APP_Tools::appIntToChar(m_Item[1].dwNUM,tcNumTemp);
				pStatic2->SetText(tcNumTemp);

				ITEM_INFO * pItemInfo = GetItemInfo(g_FlxItemInfo[i].FLX_ITEM3_ID);
				CUIStatic * pStatic = GetStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM3);
				pStatic->SetTexture(m_pBaseUI->m_pTextures[pItemInfo->dwPicId]);
				APP_Tools::appIntToChar(g_FlxItemInfo[i].FLX_ITEM3_NUM,tcNumTemp);
				pStatic->SetText(tcNumTemp);
				pStatic->SetTextColor(0xffff0000);
				m_Item[2].dwID = g_FlxItemInfo[i].FLX_ITEM3_ID;
				m_Item[2].dwNUM = g_FlxItemInfo[i].FLX_ITEM3_NUM;
			}

		}
	}
	
	return CUIDialog::Process(x,y);
}
void CFlxUI::Release()
{
	CUIDialog::Release();
}
void CFlxUI::Render()
{
	CUIDialog::Render();
}
bool CFlxUI::Event(ST_UI_MSG * pMsg)
{
	switch(pMsg->msg)
	{
	case BUTTON_CLICK:
		{
			switch(pMsg->dwControlId)
			{
			case UI_GAME_FLX_CONTROL_STATIC_BOTTON:
				{
					if (m_Item[0].dwNUM > 0 && m_Item[1].dwNUM > 0 )
					{
						if (m_Item[2].dwNUM <= 0)
						{
							for (int i = 0; i < MAX_FLX_ITEM; i++)
							{
								if (g_FlxItemInfo[i].FLX_ITEM1_ID == m_Item[0].dwID && g_FlxItemInfo[i].FLX_ITEM1_NUM <= m_Item[0].dwNUM &&
									g_FlxItemInfo[i].FLX_ITEM2_ID == m_Item[1].dwID && g_FlxItemInfo[i].FLX_ITEM2_NUM <= m_Item[1].dwNUM ||
									g_FlxItemInfo[i].FLX_ITEM1_ID == m_Item[1].dwID && g_FlxItemInfo[i].FLX_ITEM1_NUM <= m_Item[1].dwNUM &&
									g_FlxItemInfo[i].FLX_ITEM2_ID == m_Item[0].dwID && g_FlxItemInfo[i].FLX_ITEM2_NUM <= m_Item[0].dwNUM)
								{
									m_bLoading = true;
									CUIStatic * pStatic = GetStatic(UI_GAME_FLX_CONTROL_STATIC_BOTTON);
									pStatic->SetActivate(false);
									CUIStatic * pStatic1 = GetStatic(UI_GAME_FLX_CONTROL_STATIC_LOAD);
									pStatic1->SetActivate(true);
								}
							}
						}
						else if (g_pUI->m_pGameUI->m_pItemUI->AddItem(m_Item[2].dwID,m_Item[2].dwNUM))
						{
							CUIStatic * pStatic = GetStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM3);
							pStatic->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_FLX_STATIC_ITEM]);
							m_Item[2].dwNUM = 0;
							pStatic->SetTextColor(0x00000000);
						}
					}
				}
				break;
			}
		}
		break;
	}
	return false;
}
bool CFlxUI::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{


	if (!m_bActivate)return false;

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	switch(msg)
	{
	case WM_KEYDOWN:
		break;
	case WM_RBUTTONDOWN:
		{
			if (m_bMouseItem)
			{
				m_bMouseItem = false;
				CUIStatic * pUIStaticMouse = g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
				pUIStaticMouse->SetActivate(false);
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if (!APP_Tools::appInRect(x,y,(int)m_fX,(int)m_fY,(int)m_fWidth ,(int)m_fHeight))
				break;
			ITEM  Item = g_pUI->m_pGameUI->m_pItemUI->m_item[g_pUI->m_pGameUI->m_pItemUI->m_iMouseItem];
			TCHAR tcItemNum[10];
			RECT rect;
			if (g_pUI->m_pGameUI->m_pItemUI->m_bMouseItem)
			{
				for(int i = 0; i < 2; i ++)
				{
					CUIStatic * pStatic = GetStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM1 + i);
					pStatic->GetRect(&rect);
					if (APP_Tools::appInRect(x,y,rect) && m_Item[i].dwNUM <= 0)
					{
						ITEM_INFO * pItemInfo = GetItemInfo(Item.dwID);
						pStatic->SetTexture(m_pBaseUI->m_pTextures[pItemInfo->dwPicId]);
						m_Item[i].dwID = Item.dwID;
						m_Item[i].dwNUM = Item.dwNUM;

						APP_Tools::appIntToChar(Item.dwNUM,tcItemNum);
						pStatic->SetText(tcItemNum);
						pStatic->SetTextColor(0xffff0000);

						g_pUI->m_pGameUI->m_pItemUI->m_item[g_pUI->m_pGameUI->m_pItemUI->m_iMouseItem].dwNUM = 0;
						g_pUI->m_pGameUI->m_pItemUI->m_bMouseItem = false;
						g_pUI->m_pGameUI->m_pItemUI->m_pUIStaticMouseItem->SetActivate(false);
						g_pUI->m_pGameUI->m_pItemUI->m_pUIStaticMouse->SetActivate(false);
						m_iItem = i;
					}
				}
			}			
			else
			{
				for(int i = 0; i < 3; i ++)
				{
					CUIStatic * pStatic = GetStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM1 + i);
					pStatic->GetRect(&rect);
					if (APP_Tools::appInRect(x,y,rect) && m_Item[i].dwNUM > 0)
					{
						ITEM_INFO * pItemInfo = GetItemInfo(m_Item[i].dwID);
						m_MouseItem.dwID = m_Item[i].dwID;
						m_MouseItem.dwNUM = m_Item[i].dwNUM;
						m_bMouseItem = true;

						CUIStatic * pUIStaticMouse = g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
						pUIStaticMouse->SetTexture(m_pBaseUI->m_pTextures[pItemInfo->dwPicId]);
						pUIStaticMouse->SetActivate(true);
						m_iItem = i;
					}
				}
			}
			ST_UI_MSG * pMsg = new ST_UI_MSG;
			pMsg->dwDialogId = m_dwId;
			pMsg->lParam = lParam;
			pMsg->wParam = wParam;
			SendMsg(pMsg);

			return true;
		}
		break;
	}
	return false;

}
