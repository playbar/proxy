#include "SystemUI.h"
#include "../GameDefine.h"
#include "../Table/ItemInfoTable.h"
#include "../Game.h"


void CSystemUI::Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	x = 250.f;
	y = 100.f;

	CUIStatic * pBack = AddStatic(UI_GAME_SYSTEM_CONTROL_STATIC_BACK,x,y,300.f,400.f,true);
	pBack->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BACK]);
	pBack->SetUV(0.f,0.f,300.f/512.f,400.f/512.f);
	pBack->SetColor(0xaa000000);

	CUIButton * pButton = AddButton(UI_GAME_SYSTEM_CONTROL_STATIC_BUTTON,x + 70.f,y + 30.f,160.f,24.f,true);
	pButton->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_NORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_ON],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_DOWN]);
	pButton->SetUV(0.f,0.f,160.f/256.f,24.f/256.f);
	pButton->SetColor(0xff000000);
	pButton->SetText(L"返回游戏");
	pButton->m_dwTextFlag = DT_CENTER | DT_VCENTER;
	pButton->SetTextColor(0xffffffff);

	CUIButton * pButton0 = AddButton(UI_GAME_SYSTEM_CONTROL_STATIC_BUTTON0,x + 70.f,y + 60.f,160.f,24.f,true);
	pButton0->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_NORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_ON],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_DOWN]);
	pButton0->SetUV(0.f,0.f,160.f/256.f,24.f/256.f);
	pButton0->SetColor(0xff000000);
	pButton0->SetText(L"返回人物菜单");
	pButton0->m_dwTextFlag = DT_CENTER | DT_VCENTER;
	pButton0->SetTextColor(0xffffffff);
	
	CUIButton * pButton1 = AddButton(UI_GAME_SYSTEM_CONTROL_STATIC_BUTTON1,x + 70.f,y + 90.f,160.f,24.f,true);
	pButton1->InitTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_NORMAL],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_ON],m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_SYSTEM_BUTTON_DOWN]);
	pButton1->SetUV(0.f,0.f,160.f/256.f,24.f/256.f);
	pButton1->SetColor(0xff000000);
	pButton1->SetText(L"退出游戏");
	pButton1->m_dwTextFlag = DT_CENTER | DT_VCENTER;
	pButton1->SetTextColor(0xffffffff);

}
bool CSystemUI::Process(float x,float y)
{
	return CUIDialog::Process(x,y);
}
void CSystemUI::Release()
{
	CUIDialog::Release();
}
void CSystemUI::Render()
{
	CUIDialog::Render();
}

bool CSystemUI::Event(ST_UI_MSG * pMsg)
{
	if (!m_bActivate)return false;
	switch(pMsg->msg)
	{
	case BUTTON_CLICK:
		switch(pMsg->dwControlId)
		{
		case UI_GAME_SYSTEM_CONTROL_STATIC_BUTTON:
			m_bActivate = false;
			break;
		case UI_GAME_SYSTEM_CONTROL_STATIC_BUTTON0:
			g_pGame->SetGameControl(GAME_RETURN_LOGIN);
			g_pUI->m_pLoadingUI->SetActivate(true);
			g_pUI->m_pLoadingUI->SetLoading(0.f);
			break;
		case UI_GAME_SYSTEM_CONTROL_STATIC_BUTTON1:
			::PostQuitMessage(0);
			break;
		}
		break;
	}
	return false;
}
bool CSystemUI::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bActivate)
		return false;

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	switch(msg)
	{
	case WM_MOUSEMOVE:
		return true;
		break;
	case WM_LBUTTONUP:
		{
			return true;
		}
		break;
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				m_bActivate = false;
				break;
			}

			return true;
		}
		break;
	case WM_KEYUP:
		return true;
		break;
	case WM_LBUTTONDOWN:
		if (APP_Tools::appInRect(x,y,(int)m_fX,(int)m_fY,(int)m_fWidth,(int)m_fHeight))
		{
			return true;
		}
		
		break;
	}
	return false;
}