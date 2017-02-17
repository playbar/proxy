#include "../Game.h"
#include "GameUI.h"
#include "UI.h"


bool CGameUI::Init()
{
	/*
	m_pMenuUI = new CUIMenu;
	m_pDialogs[DIALOG_MENU] = m_pMenuUI;
	m_pDialogs[DIALOG_MENU]->Init(DIALOG_MENU,270,150,0,true);
	m_nActiveDialog = DIALOG_MENU;

	m_pSpeakUI = new CUISpeak;
	m_pDialogs[DIALOG_SPEAK] = m_pSpeakUI;
	m_pDialogs[DIALOG_SPEAK]->Init(DIALOG_SPEAK,183,400,PIC_SPEAK_DIALOG,false);

	m_pMiniInfoUI = new CUIMiniInfo;
	m_pDialogs[DIALOG_MINIINFO] = m_pMiniInfoUI;
	m_pDialogs[DIALOG_MINIINFO]->Init(DIALOG_MINIINFO,324,200,PIC_MINIINFO_DIALOG,false);

	m_pSystemMenuUI = new CUISystemMenu;
	m_pDialogs[DIALOG_SYSTEMMENU] = m_pSystemMenuUI;
	m_pDialogs[DIALOG_SYSTEMMENU]->Init(DIALOG_SYSTEMMENU,270,150,0,false);

	m_pFaceUI = new CUIFace;
	m_pDialogs[DIALOG_FACE] = m_pFaceUI;
	((CUIFace *)m_pDialogs[DIALOG_FACE])->Init();

	m_pSaveLoadUI = new CUISaveLoad;
	m_pDialogs[DIALOG_SAVELOAD] = m_pSaveLoadUI;
	m_pDialogs[DIALOG_SAVELOAD]->Init(DIALOG_SAVELOAD,307,200,PIC_SAVELOAD_DIALOG,false);

	m_pItemUI = new CUIItem;
	m_pDialogs[DIALOG_ITEM] = m_pItemUI;
	m_pDialogs[DIALOG_ITEM]->Init(DIALOG_ITEM,200,200,PIC_ITEM_DIALOG,false);

	m_pEquipUI = new CUIEquip;
	m_pDialogs[DIALOG_EQUIP]	= m_pEquipUI;
	m_pDialogs[DIALOG_EQUIP]->Init(DIALOG_EQUIP,380,200,PIC_EQUIP_DIALOG,false);



	*/


	//创建GameUI所需纹理
	static int i = 0;
	while( i < UI_TEXTURE_ID::MAX_GAMEUI_TEX_NUM )
	{
		CTexture * pTexture = new CTexture;
		pTexture->CreateFromFile( GetGameUITexturePath( i ), false );
		_ASSERT( pTexture );
		m_pTextures.AddItem( pTexture );

		++ i;
		g_pUI->m_pLoadingUI->UpdateLoading(100.f / UI_TEXTURE_ID::MAX_GAMEUI_TEX_NUM);
		return false;
	}
	RECT rect;
	::GetClientRect(g_pGame->m_hWND,&rect);


	//UI初始化
	//m_pMouseUI = new CUIMouse;
	//m_pDialogs[UI_GAME_DIALOG_MOUSE]	= m_pMouseUI;
	//m_pDialogs[UI_GAME_DIALOG_MOUSE]->Init( UI_GAME_DIALOG_MOUSE, true, this );
	
	m_pItemUI = new CItemUI;
	m_pDialogs[UI_GAME_DIALOG_ITEM] = m_pItemUI;
	m_pDialogs[UI_GAME_DIALOG_ITEM]->Init(UI_GAME_DIALOG_ITEM,500.f,300.f,256.f,256.f,false,this);
	m_pPlayerUI = new CPlayerUI;
	m_pDialogs[UI_GAME_DIALOG_PLAYER] = m_pPlayerUI;
	m_pDialogs[UI_GAME_DIALOG_PLAYER]->Init(UI_GAME_DIALOG_PLAYER,0.f,0.f,200.f,64.f,true,this);
	m_pMonsterUI = new CMonsterUI;
	m_pDialogs[UI_GAME_DIALOG_MONSTER] = m_pMonsterUI;
	m_pDialogs[UI_GAME_DIALOG_MONSTER]->Init(UI_GAME_DIALOG_MONSTER,230.f,0.f,200.f,64.f,false,this);
	m_pSystemUI = new CSystemUI;
	m_pDialogs[UI_GAME_DIALOG_SYSTEM] = m_pSystemUI;
	m_pDialogs[UI_GAME_DIALOG_SYSTEM]->Init(UI_GAME_DIALOG_SYSTEM,0.f,0.f,800.f,600.f,false,this);
	m_pShopUI = new CShopUI;
	m_pDialogs[UI_GAME_DIALOG_SHOP] = m_pShopUI;
	m_pDialogs[UI_GAME_DIALOG_SHOP]->Init(UI_GAME_DIALOG_SHOP,0.f,50.f,362.f,500.f,false,this);
	m_pFlxUI = new CFlxUI;
	m_pDialogs[UI_GAME_DIALOG_FLX] = m_pFlxUI;
	m_pDialogs[UI_GAME_DIALOG_FLX]->Init(UI_GAME_DIALOG_FLX,200.f,100.f,300.f,250.f,false,this);
	m_pSkillUI = new CSkillUI;
	m_pDialogs[UI_GAME_DIALOG_SKILL] = m_pSkillUI;
	m_pDialogs[UI_GAME_DIALOG_SKILL]->Init(UI_GAME_DIALOG_SKILL,rect.left - 23.f,rect.bottom - 78.f,1071,78.f,true,this);
	m_pSkillBookUI = new CSkillBookUI;
	m_pDialogs[UI_GAME_DIALOG_SKILLBOOK] = m_pSkillBookUI;
	m_pDialogs[UI_GAME_DIALOG_SKILLBOOK]->Init(UI_GAME_DIALOG_SKILLBOOK,100.f,100.f,367.f,463.f,false,this);
	m_pMouseMessageUI = new CMouseMessageUI;
	m_pDialogs[UI_GAME_DIALGO_MOUSEMESSAGE] = m_pMouseMessageUI;
	m_pDialogs[UI_GAME_DIALGO_MOUSEMESSAGE]->Init(UI_GAME_DIALGO_MOUSEMESSAGE,100.f,100.f,100.f,100.f,true,this);
	m_pMiniMap = new CMiniMap;
	m_pDialogs[UI_GAME_DIALGO_MINIMAP] = m_pMiniMap;
	m_pDialogs[UI_GAME_DIALGO_MINIMAP]->Init(UI_GAME_DIALGO_MINIMAP,835.f,0.f,188.f,150.f,true,this);


	i = 0;
	return true;
}

void CGameUI::Release()
{
	for( int i = 0; i < MAX_GAME_DIALOG_NUM; i ++ )
	{
		if( m_pDialogs[i] )
		{
			m_pDialogs[i]->Release();
			delete m_pDialogs[i];
			m_pDialogs[i] = NULL;
		}
	}
	for( int i = 0; i < UI_TEXTURE_ID::MAX_GAMEUI_TEX_NUM; i ++ )
	{
		SAFE_DELETE( m_pTextures[i] );
	}
}


bool CGameUI::Process( float x, float y )
{
	bool result = false;
	for( int i = 0;i < MAX_GAME_DIALOG_NUM; i ++ )
	{
		if( m_pDialogs[i] )
		{
			if( m_pDialogs[i]->Process( x, y ) )
				result = true;
		}
	}
	return result;
}

void CGameUI::Render()
{
	for( int i = 0; i < MAX_GAME_DIALOG_NUM; i ++ )
	{
		if( m_pDialogs[i] )
			m_pDialogs[i]->Render();
	}
}


bool CGameUI::WndProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
	for( int i = 0; i < MAX_GAME_DIALOG_NUM; i ++ )
	{
		if( m_pDialogs[i] )
		{
			if( m_pDialogs[i]->WndProc( msg, wParam, lParam ) )
				return true;
		}
	}
	return false;
}

bool CGameUI::Event( ST_UI_MSG * pMsg )
{
	for( int i = 0; i < MAX_GAME_DIALOG_NUM; i ++ )
	{
		if( m_pDialogs[i] && pMsg->dwDialogId == i)
		{
			if( m_pDialogs[i]->Event( pMsg ) )
				return true;
		}
	}
	return false;
}