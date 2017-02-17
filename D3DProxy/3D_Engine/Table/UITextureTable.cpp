
#include "UITextureTable.h"


UI_TEXTURE_TABLE g_pLoginUITextureTable[] = 
{
	L"Bin/Interface/login_back.bmp",
	L"Bin/Interface/login_log.tga",

	L"Bin/Interface/selchar_back.bmp",
	L"Bin/Interface/selchar_back2.tga",
	L"Bin/Interface/selchar_char_back1.bmp",
	L"Bin/Interface/selchar_char_back2.bmp",
	L"Bin/Interface/selchar_button_enter_normal.bmp",
	L"Bin/Interface/selchar_button_enter_on.bmp",
	L"Bin/Interface/selchar_button_enter_down.bmp",
	L"Bin/Interface/selchar_button_left_on.bmp",
	L"Bin/Interface/selchar_button_left_down.bmp",
	L"Bin/Interface/selchar_button_right_on.bmp",
	L"Bin/Interface/selchar_button_right_down.bmp",
};


UI_TEXTURE_TABLE g_pGameUITextureTable[] =
{
	L"Bin/Interface/ItemUi.tga",
	L"Bin/Interface/item_life_small.tga",
	L"Bin/Interface/item_mana_small.tga",
	L"Bin/Interface/item_piece.tga",
	L"Bin/Interface/item_clew.tga",
	L"Bin/Interface/item_leather.tga",
	L"Bin/Interface/item_clouth.tga",
	L"Bin/Interface/item_jacket.tga",

	L"Bin/Interface/PlayerInfo_Bak.tga",
	L"Bin/Interface/PlayerInfo_Player.tga",
	L"Bin/Interface/PlayerInfo_Life.tga",
	L"Bin/Interface/PlayerInfo_Mana.tga",

	L"Bin/Interface/SystemUIBack.tga",

	L"Bin/Interface/ButtonNormal.tga",
	L"Bin/Interface/ButtonDown.tga",
	L"Bin/Interface/ButtonOn.tga",

	L"Bin/Interface/button_normal.tga",
	L"Bin/Interface/button_down.tga",
	L"Bin/Interface/button_on.tga",

	L"Bin/Interface/ShopUI_Back.tga",

	L"Bin/Interface/FLX_ITEM.JPG",
	
	L"Bin/Interface/UI_Back.tga",
	L"Bin/Interface/AlphaSkill.tga",
	L"Bin/Interface/Fix_Loading.tga",
	L"Bin/Interface/Load.tga",

	L"Bin/Other2/ÖÎÓúÖ®¹â.tga",
	L"Bin/Interface/SkillBookUI_Back.tga",
	L"Bin/Interface/MouseMessage_Back.tga",
	L"Bin/Other3/»ð¿¹.tga",

	L"Bin/map/Map_Small.tga",
	L"Bin/map/Map_Big.tga",
	L"Bin/map/role.tga",

	L"Bin/Interface/ItemButtonNormal.tga",
	L"Bin/Interface/ItemButtonDown.tga",
	L"Bin/Interface/ItemButtonON.tga",

	L"Bin/Interface/UIRect.tga",

	L"Bin/Interface/SkillFixButton_Down.tga",
	L"Bin/Interface/SkillNULLButton_Down.tga",
	L"Bin/Interface/SkillBookButton_Down.tga",
	L"Bin/Interface/SkillBookButton_Normal.tga",

};

UI_TEXTURE_TABLE g_pCommonTextureTable[] =
{
	L"Bin/Interface/common_white.bmp",

	L"Bin/Interface/common_button_ok_normal.bmp",
	L"Bin/Interface/common_button_ok_on.bmp",
	L"Bin/Interface/common_button_ok_down.bmp",

	L"Bin/Interface/common_button_cancel_normal.bmp",
	L"Bin/Interface/common_button_cancel_on.bmp",
	L"Bin/Interface/common_button_cancel_down.bmp",

	L"Bin/Interface/common_loading_back.bmp",
	L"Bin/Interface/common_loading_loading.bmp",

	L"Bin/Interface/common_button_cancel_normal.bmp",
	L"Bin/Interface/common_button_cancel_down.bmp",
	L"Bin/Interface/common_button_cancel_on.bmp",
};


TCHAR * GetLoginUITexturePath( int id )
{
	if( id < 0 )
		return NULL;
	else
		return g_pLoginUITextureTable[id].szPath;
}

TCHAR * GetGameUITexturePath( int id )
{
	if( id < 0 )
		return NULL;
	else
		return g_pGameUITextureTable[id].szPath;
}

TCHAR * GetCommonTexturePath( int id )
{
	if( id < 0 )
		return NULL;
	else
		return g_pCommonTextureTable[id].szPath;
}