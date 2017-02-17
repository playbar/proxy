

#pragma once

#include <windows.h>
#define MAX_ITEM 100

enum ITEM_TYPE
{
	ITEM_HPMP,
	ITEM_EQUIP,
	ITEM_NOMORL,
	ITEM_MISSION,
	ITEM_MATERAIL,
};

struct ITEM_INFO
{
	DWORD			dwId;
	TCHAR			szName[20];
	DWORD			dwPicId;
	DWORD			dwMeshId;
	DWORD			dwType;
	DWORD			dwMoney;
	TCHAR			szInfo[100];
	DWORD           dwSkillId;
};

ITEM_INFO * GetItemInfo( DWORD dwId );  


struct ITEM_HPMP_INFO
{
	DWORD			dwId;
	DWORD			dwHp;
	DWORD			dwMp;
};

ITEM_HPMP_INFO * GetHPMPInfo( DWORD dwId );

