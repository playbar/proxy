#include "SkillUI.h"
#include "../Game.h"
#include "../Table/SkillInfoTable.h"

CSkillUI::~CSkillUI()
{
}

void CSkillBookUI::Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	CUIStatic * pUIStatic = AddStatic(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_BACK,x,y,width,height,true);
	pUIStatic->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILLBOOKUI_STATIC_BACK]);
	pUIStatic->SetUV(0.f,0.f,367.f/512.f,463.f/512.f);
	pUIStatic->SetColor(0xff000000);

	CUIButton * pUIButton = AddButton(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_BUTTON,x + 345.f,y + 19.f,20.f,19.f,true);
	pUIButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_NORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ON],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_DOWN]);
	pUIButton->SetUV(0.f,0.f,20.f/32.f,19.f/32.f);

	for (int i = 0; i < 12; i ++)
	{
		CUIStatic * pUIStaticSkill = AddStatic(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL + i ,34.f + x + i%2 * 166.f,93.f + y + i/2 * 53.8f,36.f,35.f,true);
		pUIStaticSkill->SetUV(0.f,0.f,33.f/64.f,33.f/64.f);
		pUIStaticSkill->SetColor(0xffff0000);

		CUIListBox * pUIListBoxMessage = AddListBox(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL + i + 12,74.f + x + i%2 * 166.f,91.5f + y + i/2 * 53.8f,100.f,40.f,true);
		pUIListBoxMessage->SetLineInfo(2,20);
	}

	CUIStatic * pUIStaticSkill1 = GetStatic(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL);
	m_Skill[0].dwId = 1;
	SKILL_INFO * pSkillInfo = GetSkillInfo(m_Skill[0].dwId);
	pUIStaticSkill1->SetTexture(m_pBaseUI->m_pTextures[pSkillInfo->dwPicID]);
	CUIListBox * pUIListBoxMessage1 = GetListBox(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL + 12);
	pUIListBoxMessage1->InsertLine(pSkillInfo->szName,0xff000000,DT_CENTER);
	pUIListBoxMessage1->InsertLine(pSkillInfo->szlevel,0xff000000,DT_CENTER);


	CUIStatic * pUIStaticSkill2 = GetStatic(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL + 1);
	m_Skill[1].dwId = 2;
	pSkillInfo = GetSkillInfo(m_Skill[1].dwId);
	pUIStaticSkill2->SetTexture(m_pBaseUI->m_pTextures[pSkillInfo->dwPicID]);
	CUIListBox * pUIListBoxMessage2 = GetListBox(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL + 12 + 1);
	pUIListBoxMessage2->InsertLine(pSkillInfo->szName,0XFF000000,DT_CENTER);
	pUIListBoxMessage2->InsertLine(pSkillInfo->szlevel,0XFF000000,DT_CENTER);

}
void CSkillUI::Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	CUIStatic * pUIStatic = AddStatic(UI_GAME_SKILL_CONTROL_STATIC_BACK,x,y,width,height,true);
	pUIStatic->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_STATIC_BACK]);
	pUIStatic->SetUV(0.f,0.f,1071.f/1088.f,78.f/612.f);
	pUIStatic->SetColor(0xff000000);


	m_pUIItemButton = AddButton(UI_GAME_SKILL_CONTROL_BUTTON_ITEMBUTTON,x + 947.f,y + 40.f,37.f,37.f,true);
	m_pUIItemButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_NORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_ON],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_DOWN]);
	m_pUIItemButton->SetUV(0.f,0.f,37.f/64.f,37.f/64.f);

	for (int i = 0; i < 12; i++ )
	{
		CUIStatic * pUIStaticSkill = AddStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + i,87.f + x + i*38.f + i * 8.f,y + 37.f,38.f,39.f,true);
		pUIStaticSkill->SetUV(0.f,0.f,35.f/64.f,33.f/64.f);
		pUIStaticSkill->SetColor(0xffff0000);

		CUIStatic * pUIStaticSkill2 = AddStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + i + 12,87.f + x + i*38.f + i * 8.f,y + 37.f,38.f,39.f,false);
		pUIStaticSkill2->SetUV(0.f,0.f,30.f/64.f,33.f/64.f);
		pUIStaticSkill2->SetColor(0xffff0000);
		pUIStaticSkill2->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_STATIC_ALPHASKILL]);
	}

	for (int i = 0; i < 7; i ++)
	{


		CUIButton * pButton = AddButton(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 31 + i,x + 685.f + i*2.25f + i*26.f,y + 40.f,26.f,34.f,true);
		pButton->InitTexture(NULL,NULL,NULL);
		pButton->SetUV(0.f,0.f,25.f/32.f,34.f/64.f);

		CUIStatic * pUI = AddStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 24 + i,x + 685.f + i*2.25f + i*26.f,y + 40.f,26.f,34.f,false);
		pUI->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_STATIC_BUTTONRECT]);
		pUI->SetUV(0.f,0.f,25.f/32.f,34.f/64.f);
	}

	m_pUISkillBookButton = GetButton(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 31 + 1);
	m_pUISkillBookButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKNORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKNORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKDOWN]);

	m_pUIFixButton = GetButton(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 31 + 2);
	m_pUIFixButton->InitTexture(NULL,NULL,m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_FIXDOWN]);
}

bool CSkillUI::Process(float x,float y)
{
	static bool bItemTemp = false;
	if (g_pUI->m_pGameUI->m_pItemUI->GetActivate())
	{
		m_pUIItemButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_DOWN],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_ON],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_DOWN]);
		bItemTemp = true;
	}
	else 
	{
		if(bItemTemp)
		{
			m_pUIItemButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_NORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_ON],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_DOWN]);
			bItemTemp = false;
		}
	}
	static bool bFixTemp = false;
	if (g_pUI->m_pGameUI->m_pFlxUI->GetActivate())
	{
		g_pUI->m_pGameUI->m_pShopUI->SetActivate(false);
		m_pUIFixButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_FIXDOWN],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_FIXDOWN],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_FIXDOWN]);
		bFixTemp = true;
	}
	else
	{
		if (bFixTemp)
		{
			m_pUIFixButton->InitTexture(NULL,NULL,m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_FIXDOWN]);
			bFixTemp = false;
		}
	}
	static bool bSkillBookTemp = false;
	if (g_pUI->m_pGameUI->m_pSkillBookUI->GetActivate())
	{
		g_pUI->m_pGameUI->m_pShopUI->SetActivate(false);
		bSkillBookTemp = true;
		m_pUISkillBookButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKDOWN],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKDOWN],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKDOWN]);
	}
	else
	{
		if (bSkillBookTemp)
		{
			bSkillBookTemp = false;
			m_pUISkillBookButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKNORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKNORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKDOWN]);
		}
	}
	return CUIDialog::Process(x,y);
}

void CSkillUI::Release()
{
	CUIDialog::Release();
}

void CSkillUI::Render()
{
	CUIDialog::Render();
}

bool CSkillUI::Event(ST_UI_MSG * pMsg)
{
	switch(pMsg->msg)
	{
	case BUTTON_CLICK:
		switch(pMsg->dwControlId)
		{
		case UI_GAME_SKILL_CONTROL_BUTTON_ITEMBUTTON:
			{
				if (g_pUI->m_pGameUI->m_pItemUI->GetActivate())
				{
					g_pUI->m_pGameUI->m_pItemUI->SetActivate(false);
				}
				else
				{
					g_pUI->m_pGameUI->m_pItemUI->SetActivate(true);
				}
			}
			break;
		case UI_GAME_SKILL_CONTROL_STATIC_SKILL + 33:
			if (g_pUI->m_pGameUI->m_pFlxUI->GetActivate())
			{
				g_pUI->m_pGameUI->m_pFlxUI->SetActivate(false);
			} 
			else
			{
				g_pUI->m_pGameUI->m_pFlxUI->SetActivate(true);
			}
			break;
		case UI_GAME_SKILL_CONTROL_STATIC_SKILL + 32:
			if (g_pUI->m_pGameUI->m_pSkillBookUI->GetActivate())
			{
				g_pUI->m_pGameUI->m_pSkillBookUI->SetActivate(false);
			} 
			else
			{
				g_pUI->m_pGameUI->m_pSkillBookUI->SetActivate(true);
			}
			break;
		}
		break;
	}
	return false;
}

bool CSkillUI::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bActivate) return false;
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	switch(msg)
	{
	case WM_MOUSEMOVE:
		if (APP_Tools::appInRect(x,y,(int)m_fX,(int)m_fY,(int)m_fWidth,(int)m_fHeight))
		{
			for (int i = 0; i < 12; i++)
			{
				RECT rect;
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + i);
				pUIStatic->GetRect(&rect);
				if (APP_Tools::appInRect(x,y,rect))
				{
					pUIStatic->SetActivate(true);
				}
				else
				{
					pUIStatic->SetActivate(false);
				}
				if (i < 7)
				{
					CUIStatic * pUIStatic1 = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 24 + i);
					pUIStatic1->GetRect(&rect);
					if (APP_Tools::appInRect(x,y,rect))
					{
						pUIStatic1->SetActivate(true);
					}
					else
					{
						pUIStatic1->SetActivate(false);
					}
				}
			}
			return true;
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if (APP_Tools::appInRect(x,y,(int)m_fX,(int)m_fY,(int)m_fWidth,(int)m_fHeight))
			{
				SKILL_INFO * pSkillInfo;
				if (g_pUI->m_pGameUI->m_pSkillBookUI->m_MouseSkill.dwId != 0)
				{
					for (int i = 0; i < 12; i++)
					{
						RECT rect;
						CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + i);
						pUIStatic->GetRect(&rect);
						if (APP_Tools::appInRect(x,y,rect))
						{
							CUIStatic * pUIStaticMouse = g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
							m_Skill[i].dwId = g_pUI->m_pGameUI->m_pSkillBookUI->m_MouseSkill.dwId;
							pSkillInfo = GetSkillInfo(m_Skill[i].dwId);
							if (pSkillInfo)
							{
								pUIStatic->SetTexture(m_pBaseUI->m_pTextures[pSkillInfo->dwPicID]);
								pUIStatic->SetActivate(true);
								g_pUI->m_pGameUI->m_pSkillBookUI->m_MouseSkill.dwId = 0;
								pUIStaticMouse->SetActivate(false);
							}

						}
					}
				}
				else //if (APP::appInRect(x,y,(int)m_fX,(int)m_fY,(int)m_fWidth,(int)m_fHeight))
				{
					for (int i = 0; i < 12; i++)
					{
						RECT rect;
						CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + i);
						pUIStatic->GetRect(&rect);
						if (APP_Tools::appInRect(x,y,rect))
						{
							SetSkill(i);
						}
					}
					return true;
				}
				return true;
			}
		}
		break;
	case WM_LBUTTONUP:
		{

		}
		break;
	case WM_RBUTTONDOWN:
		{
			for (int i = 0; i < 12; i++)
			{
				RECT rect;
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + i);
				pUIStatic->GetRect(&rect);
				if (APP_Tools::appInRect(x,y,rect) && m_Skill[i].dwId != NULL)
				{
					m_Skill[i].dwId = NULL;
					pUIStatic->SetActivate(false);
				}
			}
		}
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case '1':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 0);
				pUIStatic->SetActivate(true);

				SetSkill(0);
			}
			break;
		case '2':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 1);
				pUIStatic->SetActivate(true);

				SetSkill(1);
			}
			break;
		case '3':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 2);
				pUIStatic->SetActivate(true);

				SetSkill(2);
			}
			break;
		case '4':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 3);
				pUIStatic->SetActivate(true);

				SetSkill(3);
			}
			break;
		case '5':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 4);
				pUIStatic->SetActivate(true);

				SetSkill(4);
			}
			break;
		case '6':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 5);
				pUIStatic->SetActivate(true);

				SetSkill(5);
			}
			break;
		case '7':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 6);
				pUIStatic->SetActivate(true);

				SetSkill(6);
			}
			break;
		case '8':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 7);
				pUIStatic->SetActivate(true);

				SetSkill(7);
			}
			break;
		case '9':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 8);
				pUIStatic->SetActivate(true);

				SetSkill(8);
			}
			break;
		case '0':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 9);
				pUIStatic->SetActivate(true);

				SetSkill(9);
			}
			break;
		case VK_SUBTRACT:
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 10);
				pUIStatic->SetActivate(true);
			}
			break;
		case '=':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 11);
				pUIStatic->SetActivate(true);
			}
			break;
		}
		break;
	case WM_KEYUP:
		switch(wParam)
		{
		case '1':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12);
				pUIStatic->SetActivate(false);
			}
			break;
		case '2':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 1);
				pUIStatic->SetActivate(false);
			}
			break;
		case '3':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 2);
				pUIStatic->SetActivate(false);
			}
			break;
		case '4':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 3);
				pUIStatic->SetActivate(false);
			}
			break;
		case '5':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 4);
				pUIStatic->SetActivate(false);
			}
			break;
		case '6':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 5);
				pUIStatic->SetActivate(false);
			}
			break;
		case '7':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 6);
				pUIStatic->SetActivate(false);
			}
			break;
		case '8':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 7);
				pUIStatic->SetActivate(false);
			}
			break;
		case '9':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 8);
				pUIStatic->SetActivate(false);
			}
			break;
		case '0':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 9);
				pUIStatic->SetActivate(false);
			}
			break;
		case '-':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 10);
				pUIStatic->SetActivate(false);
			}
			break;
		case '=':
			{
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILL_CONTROL_STATIC_SKILL + 12 + 11);
				pUIStatic->SetActivate(false);
			}
			break;
		}
		break;
	}
	return false;
}
void CSkillUI::SetSkill(int iNum)
{
	if (m_Skill[iNum].dwId)
	{
		SKILL_INFO * pSkillInfo;
		pSkillInfo = GetSkillInfo(m_Skill[iNum].dwId);
		if (pSkillInfo)
		{
			g_pGame->m_pSkillManager->CreateSkill(pSkillInfo->dwID);
		}
	}
}

bool CSkillBookUI::Process(float x,float y)
{

	return CUIDialog::Process(x,y);
}

void CSkillBookUI::Release()
{
	CUIDialog::Release();
}

void CSkillBookUI::Render()
{
	CUIDialog::Render();
}

bool CSkillBookUI::Event(ST_UI_MSG * pMsg)
{
	switch(pMsg->msg)
	{
	case BUTTON_CLICK:
		switch(pMsg->dwControlId)
		{
		case UI_GAME_SKILLBOOKUI_CONTROL_STATIC_BUTTON:
			m_bActivate = false;
			break;
		}
		break;
	}
	return false;
}
bool CSkillBookUI::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bActivate) return false;
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	switch(msg)
	{
	case WM_RBUTTONDOWN:


		break;
	case WM_LBUTTONDOWN:
		{
			if (!APP_Tools::appInRect(x,y,(int)m_fX,(int)m_fY,(int)m_fWidth,(int)m_fHeight))
				break;
			CUIStatic * pUIStaticMouse = g_pUI->m_pMouseUI->GetStatic(UI_GAME_MOUSE_CLICKDOWN_ITEM);
			if (m_MouseSkill.dwId != 0)
			{
				m_MouseSkill.dwId = 0;
				pUIStaticMouse->SetActivate(false);
			}
			SKILL_INFO * pSkillInfo;
			for (int i = 0; i < 12; i++)
			{
				RECT rect;
				CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL + i);
				pUIStatic->GetRect(&rect);
				if (APP_Tools::appInRect(x,y,rect) && m_Skill[i].dwId && !g_pUI->m_pGameUI->m_pFlxUI->m_bMouseItem && !g_pUI->m_pGameUI->m_pItemUI->m_bMouseItem && !g_pUI->m_pGameUI->m_pShopUI->m_bMouseItem)
				{	
					m_MouseSkill.dwId = m_Skill[i].dwId;	
					pSkillInfo = GetSkillInfo(m_MouseSkill.dwId);
					pUIStaticMouse->SetTexture(m_pBaseUI->m_pTextures[pSkillInfo->dwPicID]);
					pUIStaticMouse->SetActivate(true);	
				}
			}
			if (APP_Tools::appInRect(x,y,(int)m_fX + 72,(int)m_fY + 17,273,22))
			{
				m_fXTemp = (float)x - m_fX;
				m_fYTemp = (float)y - m_fY;
				m_bSetPos = true;
			}
			return true;
		}
		break;
	case WM_LBUTTONUP:
		m_bSetPos = false;
		break;
	case WM_MOUSEMOVE:
		if (m_bSetPos)
		{
			SetPos((float)x,(float)y);
		}
		break;
	}

	return false;
}
void CSkillBookUI::SetPos(float x,float y)
{
	m_fX = x - m_fXTemp;
	m_fY = y - m_fYTemp;

	CUIStatic * pUIStatic = GetStatic(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_BACK);
	pUIStatic->SetXY(m_fX,m_fY);

	CUIButton * pUIButton = GetButton(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_BUTTON);
	pUIButton->SetXY(m_fX + 345.f,m_fY + 19.f);

	for (int i = 0; i < 12; i ++)
	{
		CUIStatic * pUIStaticSkill = GetStatic(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL + i);
		pUIStaticSkill->SetXY(34.f + m_fX + i%2 * 166.f,93.f + m_fY + i/2 * 53.8f);

		CUIListBox * pUIListBoxMessage = GetListBox(UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL + i + 12);
		pUIListBoxMessage->SetXY(74.f + m_fX + i%2 * 166.f,91.5f + m_fY + i/2 * 53.8f);
	}
}
