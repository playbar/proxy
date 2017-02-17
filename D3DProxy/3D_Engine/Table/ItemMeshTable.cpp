
#include "ItemMeshTable.h"

ITEM_MESH_TABLE g_ItemMeshTable[] =
{
		L"Bin/obj/red.X",
		L"Bin/obj/blue.X",
		L"Bin/obj/bag.X",
		L"Bin/obj/obj1.x",
};


TCHAR * GetItemMeshPath( int id )
{
	if( id < 0 )
		return NULL;
	else
		return g_ItemMeshTable[id].szPath;
}