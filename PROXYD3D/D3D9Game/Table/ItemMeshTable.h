
#pragma once

#include <Windows.h>

struct ITEM_MESH_TABLE
{
	TCHAR szPath[50];
};

extern TCHAR * GetItemMeshPath( int id );