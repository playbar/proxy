#include "MiniMap.h"
#include "../Game.h"
	
void CMiniMap::Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI )
{
	m_dwId = dwId;
	m_bActivate = bActivate;
	m_pBaseUI = pBaseUI;
	m_fWidth = width;
	m_fHeight = height;
	m_fX = x;
	m_fY = y;

	CUIStatic * pUIMiniMap = AddStatic(UI_GAME_MINIMAP_CONTROL_STATIC_MAP,x + 43.f,y + 17.f,103.f,113.f,true);
	pUIMiniMap->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_MINIMAP_STATIC_MAP]);
	
	CUIStatic * pUIBack = AddStatic(UI_GAME_MINIMAP_CONTROL_STATIC_BACK,x,y,width,height,true);
	pUIBack->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_MINIMAP_STATIC_BACK]);
	pUIBack->SetUV(0.f,0.f,188.f/256.f,150.f/256.f);
	pUIBack->SetColor(0xff000000);

	CUIStatic * pUIRole = AddStatic(UI_GAME_MINIMAP_CONTROL_STATIC_ROLE,x + 76.f,y + 55.f,36.f,36.f,true);
	pUIRole->SetTexture(m_pBaseUI->m_pTextures[UI_TEXTURE_ID::GAMEUI_TEX_MINIMAP_STATIC_ROLE]);
	pUIRole->SetUV(0.f,0.f,1.f,1.f);
	pUIRole->SetColor(0xff000000);
	pUIRole->SetRotate(-D3DX_PI);
}

bool CMiniMap::Process(float x,float y)
{
	float fR = 0.f;
	CPlayer * pPlayer = g_pGame->m_pPlayerManager->GetUser();
	if (pPlayer)
	{
		pPlayer->GetPos(m_vPos);
		fR = pPlayer->GetRotationY();
	}
	CUIStatic * pUIMiniMap = GetStatic(UI_GAME_MINIMAP_CONTROL_STATIC_MAP);
	pUIMiniMap->SetUV((m_vPos.x - m_fView)/2540.f,(m_vPos.z + m_fView)/2540.f,(m_vPos.x + m_fView)/2540.f,(m_vPos.z - m_fView)/2540.f);
	CUIStatic * pUIRole = GetStatic(UI_GAME_MINIMAP_CONTROL_STATIC_ROLE);
	pUIRole->SetRotate(fR);
	pUIRole->m_fR = fR;
	return CUIDialog::Process(x,y);
}
void CMiniMap::Release()
{
	CUIDialog::Release();
}
void CMiniMap::Render()
{
	CUIDialog::Render();
}
bool CMiniMap::Event(ST_UI_MSG * pMsg)
{
	return false;
}
bool CMiniMap::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_SUBTRACT:
			if (m_fView > 45.f)
			{
				m_fView -= 10;
			}
			break;
		case VK_ADD:
			if (m_fView < 1024.f)
			{
				m_fView += 10;
			}
			break;
		}
		break;
	}
	return false;
}

