 
#include "ItemInfoTable.h"
#include "../GameDefine.h"
#include "../itemManager.h"

using namespace UI_TEXTURE_ID;


ITEM_INFO g_ItemInfoTable[MAX_ITEM] = 
{
	//{10001,,,},
	//{10002,,,},
	//...
	{0,L"小型HP药水",UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_0,ITEM_MESH_ID_RED,ITEM_HPMP,40,L"可恢复少量HP",3},
	{1,L"小型MP药水",UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_1,ITEM_MESH_ID_BLUE,ITEM_HPMP,35,L"可恢复少量MP",4},
	{2,L"粗布",		 UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_2,ITEM_MESH_ID_BAG,ITEM_MATERAIL,40,L"可以做布衣",0},
	{3,L"粗线",		 UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_3,ITEM_MESH_ID_BAG,ITEM_MATERAIL,20,L"缝衣服用的线",0},
	{4,L"粗皮",		 UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_4,ITEM_MESH_ID_BAG,ITEM_MATERAIL,40,L"可以做皮衣",0},
	{5,L"破烂的衣服",UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_5,ITEM_MESH_ID_BAG,ITEM_EQUIP,50,L"破烂的衣服,防御+3",0},
	{6,L"破烂的皮袄",UI_TEXTURE_ID::GAMEUI_TEX_ITEM_STATIC_6,ITEM_MESH_ID_BAG,ITEM_EQUIP,50,L"破烂的皮袄,防御+2",0},
};


ITEM_INFO * GetItemInfo( DWORD dwId )
{
	for( int i = 0; i < MAX_ITEM; ++i )
	{
		if( g_ItemInfoTable[i].dwId == dwId )
			return &g_ItemInfoTable[i];
	}
	return NULL;
}

ITEM_HPMP_INFO g_HPMPInfoTable[MAX_ITEM] = 
{
	{10001,50,0},
	{10002,0,50},
};

ITEM_HPMP_INFO * GetHPMPInfo( DWORD dwId )
{
	for( int i = 0; i < MAX_ITEM; ++i )
	{
		if( g_HPMPInfoTable[i].dwId == dwId )
			return &g_HPMPInfoTable[i];
	}
	return NULL;
}