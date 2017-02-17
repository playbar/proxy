#include "ItemUI.h"
#include "UI.h"
#include "../Game.h"
#include "../GameDefine.h"



void CItemUI::Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	CUIStatic * pUIStatic = AddStatic(UI_GAME_ITEM_CONTROL_STATIC_BACK,x,y,256.f,256.f,true);
	pUIStatic->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_BACK]);
	pUIStatic->SetUV(0.f,0.f,1.f,1.f);
	pUIStatic->SetTextColor(0xffff0000);
	pUIStatic->SetColor(0xff000000);

	CUIButton * pUIButton = AddButton(UI_GAME_ITEM_CONTROL_STATIC_BUTTON,x + 226.f,y + 1.f,27.f,21.f,true);
	pUIButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_NORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ON],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_DOWN]);
	pUIButton->SetUV(0.f,0.f,20.f/32.f,19.f/32.f);
	pUIButton->SetTextColor(0xffff0000);
	pUIButton->SetColor(0xff000000);


	for (int i = 0; i < 16; i ++)
	{
		CUIStatic * pUIStatic = AddStatic(UI_GAME_ITEM_CONTROL_STATIC_ITEM0 + i,x + (float)(12 + (i%4) * 51 + (i%4) * 9),y + (float)(51 + i/4 * 39 + i/4 * 7),51.f,39.f,false);
		pUIStatic->SetUV(0.f,0.f,35.f/64.f,33.f/64.f);
		pUIStatic->SetTextColor(0xffff0000);
		pUIStatic->SetColor(0xff000000);
	}
	_tcscpy(m_tcMoney,L"10000");
	m_pUIStaticMoney = AddStatic(UI_GAME_ITEM_CONTROL_STATIC_MONEY,x + 100,y + 235,100,20,true);
	m_pUIStaticMoney->SetText(m_tcMoney);
	m_pUIStaticMoney->SetTextColor(0xffffffff);
}
bool CItemUI::Process(float x,float y)
{
	return CUIDialog::Process(x,y);
}
void CItemUI::Release()
{
	CUIDialog::Release();
}
void CItemUI::Render()
{
	CUIDialog::Render();
}
bool CItemUI::Event(ST_UI_MSG * pMsg)
{
	if (!m_bActivate)return false;
	switch(pMsg->msg)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case BUTTON_CLICK:
		{
			switch(pMsg->dwControlId)
			{
			case UI_GAME_ITEM_CONTROL_STATIC_BUTTON:
				m_bActivate = false;
				break;
			}
		}
		break;
	}
	return true;
}
bool CItemUI::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bActivate)
		return false;

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	m_pUIStaticMouseItem = GetStatic(UI_GAME_ITEM_CONTROL_STATIC_ITEM0 + m_iMouseItem);
	m_pUIStaticMouse = g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
	switch(msg)
	{
	case WM_RBUTTONDOWN:
		if (!APP_Tools::appInRect(x,y,(int)m_fX,(int)m_fY,(int)m_fWidth,(int)m_fHeight))
			break;
		if (m_bMouseItem)
		{
			m_bMouseItem = false;
			m_pUIStaticMouse->SetActivate(false);
		}
		else
		{
			for (int i = 0; i < 16; i ++)
			{	
				CUIStatic * p = GetStatic(UI_GAME_ITEM_CONTROL_STATIC_ITEM0 + i);
				RECT rect;
				p->GetRect(&rect);
				if (APP_Tools::appInRect(x,y,rect) && m_item[i].dwNUM > 0)
				{	
					TCHAR tcTemp[10];
					ITEM_INFO * pItem = GetItemInfo(m_item[i].dwID);
					if (g_pGame->m_pSkillManager->CreateSkill(pItem->dwSkillId))
					{
						m_item[i].dwNUM -= 1;
						APP_Tools::appIntToChar((int)m_item[i].dwNUM,tcTemp);
						p->SetText(tcTemp);
						if (m_item[i].dwNUM <= 0)
						{
							p->SetActivate(false);
							g_pUI->m_pGameUI->m_pMouseMessageUI->SetActivate(false);
						}
					}
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			static ITEM itemTemp;

			if (!APP_Tools::appInRect(x,y,(int)m_fX,(int)m_fY,(int)m_fWidth,(int)m_fHeight))
				break;

			for (int i = 0; i < 16; i++)
			{
				CUIStatic * p = GetStatic(UI_GAME_ITEM_CONTROL_STATIC_ITEM0 + i);
				RECT rect;
				p->GetRect( &rect );
				if (APP_Tools::appInRect(x,y,rect) && !m_bMouseItem)
				{
					CUIStatic * pStatic = GetStatic(UI_GAME_ITEM_CONTROL_STATIC_ITEM0 + i);
					TCHAR tcTemp[10];
					ITEM_INFO * pItemInfo;
					if (g_pUI->m_pGameUI->m_pFlxUI->m_bMouseItem)
					{
						if (m_item[i].dwID == g_pUI->m_pGameUI->m_pFlxUI->m_MouseItem.dwID && m_item[i].dwNUM > 0)
						{
							if (m_item[i].dwNUM + g_pUI->m_pGameUI->m_pFlxUI->m_MouseItem.dwNUM > 20)
							{
								if (!AddItem(m_item[i].dwID,g_pUI->m_pGameUI->m_pFlxUI->m_MouseItem.dwNUM))return false;
							}
							else
							{
								m_item[i].dwNUM += g_pUI->m_pGameUI->m_pFlxUI->m_MouseItem.dwNUM;
								pItemInfo = GetItemInfo(m_item[i].dwID);
								m_iMoney -= pItemInfo->dwMoney;
								APP_Tools::appIntToChar(m_iMoney,tcTemp);
								m_pUIStaticMoney->SetText(tcTemp);

								APP_Tools::appIntToChar(m_item[i].dwNUM,tcTemp);
								pStatic->SetText(tcTemp);
							}
							CUIStatic * pUIStatic = g_pUI->m_pGameUI->m_pFlxUI->GetStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM1 + g_pUI->m_pGameUI->m_pFlxUI->m_iItem);
							pUIStatic->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_FLX_STATIC_ITEM]);
							pUIStatic->SetTextColor(0x00000000);
							CUIStatic * pStaticMouse =g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
							pStaticMouse->SetActivate(false);
							g_pUI->m_pGameUI->m_pFlxUI->m_bMouseItem = false;
							g_pUI->m_pGameUI->m_pFlxUI->m_Item[g_pUI->m_pGameUI->m_pFlxUI->m_iItem].dwNUM = 0;
						}
						else if(m_item[i].dwNUM <= 0)
						{
							m_item[i].dwID = g_pUI->m_pGameUI->m_pFlxUI->m_MouseItem.dwID;
							m_item[i].dwNUM += g_pUI->m_pGameUI->m_pFlxUI->m_MouseItem.dwNUM;
							ITEM_INFO * pItemInfo = GetItemInfo(m_item[i].dwID);
							m_iMoney -= pItemInfo->dwMoney;
							APP_Tools::appIntToChar(m_iMoney,tcTemp);
							m_pUIStaticMoney->SetText(tcTemp);

							pStatic->SetTexture(m_pBaseUI->m_pTextures[pItemInfo->dwPicId]);
							APP_Tools::appIntToChar(m_item[i].dwNUM,tcTemp);
							pStatic->SetText(tcTemp);
							pStatic->SetActivate(true);

							CUIStatic * pStaticMouse =g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
							pStaticMouse->SetActivate(false);
							g_pUI->m_pGameUI->m_pFlxUI->m_bMouseItem = false;

							CUIStatic * pUIStatic = g_pUI->m_pGameUI->m_pFlxUI->GetStatic(UI_GAME_FLX_CONTROL_STATIC_ITEM1 + g_pUI->m_pGameUI->m_pFlxUI->m_iItem);
							pUIStatic->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_FLX_STATIC_ITEM]);
							pUIStatic->SetTextColor(0x00000000);

							g_pUI->m_pGameUI->m_pFlxUI->m_Item[g_pUI->m_pGameUI->m_pFlxUI->m_iItem].dwNUM = 0;
						}
					}
					else if (g_pUI->m_pGameUI->m_pShopUI->m_bMouseItem)
					{
						if (m_item[i].dwID == g_pUI->m_pGameUI->m_pShopUI->m_ShopMouseItem.dwID && m_item[i].dwNUM > 0)
						{
							if (m_item[i].dwNUM + g_pUI->m_pGameUI->m_pShopUI->m_ShopMouseItem.dwNum > 20)
							{
								if (!AddItem(m_item[i].dwID,g_pUI->m_pGameUI->m_pShopUI->m_ShopMouseItem.dwNum))return false;	
							}
							else
							{
								m_item[i].dwNUM += g_pUI->m_pGameUI->m_pShopUI->m_ShopMouseItem.dwNum;
								pItemInfo = GetItemInfo(m_item[i].dwID);
								m_iMoney -= pItemInfo->dwMoney;
								APP_Tools::appIntToChar(m_iMoney,tcTemp);
								m_pUIStaticMoney->SetText(tcTemp);

								APP_Tools::appIntToChar(m_item[i].dwNUM,tcTemp);
								pStatic->SetText(tcTemp);
							}
							CUIStatic * pStaticMouse =g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
							pStaticMouse->SetActivate(false);
							g_pUI->m_pGameUI->m_pShopUI->m_bMouseItem = false;
						}
						else if(m_item[i].dwNUM <= 0)
						{
							m_item[i].dwID = g_pUI->m_pGameUI->m_pShopUI->m_ShopMouseItem.dwID;
							m_item[i].dwNUM += g_pUI->m_pGameUI->m_pShopUI->m_ShopMouseItem.dwNum;
							ITEM_INFO * pItemInfo = GetItemInfo(m_item[i].dwID);
							m_iMoney -= pItemInfo->dwMoney;
							APP_Tools::appIntToChar(m_iMoney,tcTemp);
							m_pUIStaticMoney->SetText(tcTemp);

							pStatic->SetTexture(m_pBaseUI->m_pTextures[pItemInfo->dwPicId]);
							APP_Tools::appIntToChar(m_item[i].dwNUM,tcTemp);
							pStatic->SetText(tcTemp);
							pStatic->SetActivate(true);

							CUIStatic * pStaticMouse =g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
							pStaticMouse->SetActivate(false);
							g_pUI->m_pGameUI->m_pShopUI->m_bMouseItem = false;
						}
					}
					else if(m_item[i].dwNUM > 0)
					{
						ITEM_INFO * pItemInfo = GetItemInfo( m_item[i].dwID );
						if (pItemInfo)
						{
							m_pUIStaticMouse->SetTexture( g_pUI->m_pGameUI->m_pTextures[pItemInfo->dwPicId] );
							m_pUIStaticMouse->SetActivate(true);
							itemTemp.dwID = m_item[i].dwID;
							itemTemp.dwNUM = m_item[i].dwNUM;
							m_iMouseItem = i;
							m_bMouseItem = true;
						}
					}
				}
				else if (m_bMouseItem && APP_Tools::appInRect(x,y,rect))
				{
					if (i == m_iMouseItem);
					else if (m_item[i].dwNUM == 0)
					{
						m_item[m_iMouseItem].dwNUM = 0;
					}
					else if (m_item[i].dwID == m_item[m_iMouseItem].dwID && m_item[i].dwNUM < 20)
					{
						if (m_item[i].dwNUM + m_item[m_iMouseItem].dwNUM <= 20)
						{
							m_item[i].dwNUM = m_item[i].dwNUM + m_item[m_iMouseItem].dwNUM;
							m_item[m_iMouseItem].dwNUM = 0;
							itemTemp.dwNUM = m_item[i].dwNUM;
						}
						else
						{
							m_item[m_iMouseItem].dwNUM = m_item[i].dwNUM + m_item[m_iMouseItem].dwNUM - 20;
							m_item[i].dwNUM = 20;
							itemTemp.dwNUM = m_item[i].dwNUM;
						}
					}
					else
					{
						TCHAR tcTemp[10];
						m_item[m_iMouseItem].dwID = m_item[i].dwID;
						m_item[m_iMouseItem].dwNUM = m_item[i].dwNUM;
						m_pItemInfo  =  GetItemInfo(m_item[m_iMouseItem].dwID);
						m_pUIStaticMouseItem->SetTexture(m_pBaseUI->m_pTextures[m_pItemInfo->dwPicId]);
						APP_Tools::appIntToChar((int)m_item[m_iMouseItem].dwNUM,tcTemp);
						m_pUIStaticMouseItem->SetText(tcTemp);
					}

					TCHAR tcTemp[10];
					m_item[i].dwID = itemTemp.dwID;
					m_item[i].dwNUM = itemTemp.dwNUM;
					m_pItemInfo  =  GetItemInfo(m_item[i].dwID);
					p->SetTexture(m_pBaseUI->m_pTextures[m_pItemInfo->dwPicId]);
					APP_Tools::appIntToChar((int)m_item[i].dwNUM,tcTemp);
					p->SetText(tcTemp);
					p->SetActivate(true);
					APP_Tools::appIntToChar((int)m_item[m_iMouseItem].dwNUM,tcTemp);
					m_pUIStaticMouseItem->SetText(tcTemp);

					CUIStatic * pUIStatic = g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
					pUIStatic->SetActivate(false);
					if (m_item[m_iMouseItem].dwNUM == 0)
					{
						m_pUIStaticMouseItem->SetActivate(false);
					}
					m_bMouseItem = false;
				}
			}
			if (APP_Tools::appInRect((int)x,(int)y,(int)m_fX,(int)m_fY,224,22))
			{
				m_bSetPos = true;
				m_fXTemp = (float)x - m_fX;
				m_fYTemp = (float)y - m_fY;
			}
			return true;
		}
		break;
	case WM_LBUTTONUP:
		{
			m_bSetPos = false;
			if (!APP_Tools::appInRect(x,y,(int)m_fX,(int)m_fY,(int)m_fWidth,(int)m_fHeight))
			{
				if (m_bMouseItem)
				{	
					D3DXVECTOR3 tempPos;
					tempPos = *(g_pGame->m_pPlayerManager->GetUser()->GetPos());
					tempPos.x = tempPos.x + rand()%10;
					tempPos.z = tempPos.z + rand()%10;

					CreateItem(tempPos,D3DXVECTOR3(0.005f,0.005f,0.005f),0.f,m_item[m_iMouseItem].dwID,m_item[m_iMouseItem].dwNUM);

					m_item[m_iMouseItem].dwNUM = 0;
					m_pUIStaticMouseItem->SetActivate(false);
					m_pUIStaticMouse->SetActivate(false);

					m_bMouseItem = false;
				}
				break;
			}
			return true;
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (m_bSetPos)
			{
				SetPos((float)x,(float)y);
			}
		}
		break;
	}
	return false;
}
bool CItemUI::AddItem(DWORD dwId,DWORD dwNum)
{
	for (int i = 0; i < 16; i ++)
	{	
		TCHAR temp[10];
		CUIStatic * p = GetStatic(UI_GAME_ITEM_CONTROL_STATIC_ITEM0 + i);
		if (m_item[i].dwNUM == 0)
		{
			m_pItemInfo  =  GetItemInfo(dwId);
			p->SetTexture(m_pBaseUI->m_pTextures[m_pItemInfo->dwPicId]);

			m_item[i].dwID = dwId;
			m_item[i].dwNUM = dwNum;
			APP_Tools::appIntToChar((int)dwNum,temp);
			p->SetText(temp);
			p->SetActivate(true);
			return true;
		}
		else if (m_item[i].dwID == dwId && m_item[i].dwNUM + dwNum <= 20)
		{
			m_item[i].dwNUM += dwNum;
			APP_Tools::appIntToChar((int)m_item[i].dwNUM,temp);
			p->SetText(temp);
			return true;
		}
	}
	return false;
}
void CItemUI::CreateItem(D3DXVECTOR3 vpos,D3DXVECTOR3 vscaling,float fRotationY,int iType,int iNum)
{
	ITEM_INFO * pItemInfo = GetItemInfo((DWORD)iType);
	DWORD dKey = g_pGame->m_pItemManager->GetItemKey();
	g_pGame->m_pTerrain->LineCheck(vpos - D3DXVECTOR3(0.f,1000,0.f),vpos + D3DXVECTOR3(0.f,1000,0.f),vpos);
	CItem * pItem = new CItem(g_pGame->m_pItemManager->m_pStaticMesh[pItemInfo->dwMeshId],vpos,vscaling,fRotationY);
	pItem->m_iNum = iNum;
	pItem->m_iType = iType;
	g_pGame->m_pItemManager->m_pItemList->InsertItem(dKey,pItem);
}

void CItemUI::SetPos(float x,float y)
{
	m_fX = x - m_fXTemp;
	m_fY = y - m_fYTemp;

	CUIStatic * pUIStatic = GetStatic(UI_GAME_ITEM_CONTROL_STATIC_BACK);
	pUIStatic->SetXY(m_fX ,m_fY);

	CUIButton * pUIButton = GetButton(UI_GAME_ITEM_CONTROL_STATIC_BUTTON);
	pUIButton->SetXY(m_fX + 226.f,m_fY + 1.f);
	for (int i = 0; i < 16; i ++)
	{
		CUIStatic * pUIStatic = GetStatic(UI_GAME_ITEM_CONTROL_STATIC_ITEM0 + i);
		pUIStatic->SetXY(m_fX + (float)(12 + (i%4) * 51 + (i%4) * 9),m_fY + (float)(51 + i/4 * 39 + i/4 * 7));
	}
	m_pUIStaticMoney = GetStatic(UI_GAME_ITEM_CONTROL_STATIC_MONEY);
	m_pUIStaticMoney->SetXY(m_fX + 100,m_fY + 235);
}

