#include "itemManager.h"
#include "Table/ItemMeshTable.h"
#include "Game.h"

CItem * CItemManager::ItemRayCheck()
{
	POINT point;
	g_pGame->GetMousePos(&point);
	RAY ray;
	g_pGame->m_pCamera->GetRay(point.x,point.y,&ray);
	for (POS pos = m_pItemList->GetFirstPos();pos;)
	{
		CItem * item = m_pItemList->GetNext(pos);
		if (item->RayCheck(ray.vOrig,ray.vVector))
		{
			item->SetCustomRenderState(COLOR_SELECT_TEXTURE,ALPHA_SELECT_TEXTURE,ALPHABLEND_ONE_ONE,FILTER_LINEAR,CULL_CCW,true,false,0);
			return item;
		}
		else
		{
			item->SetCustomRenderState(COLOR_SELECT_TEXTURE,ALPHA_SELECT_TEXTURE,ALPHABLEND_NONE,FILTER_LINEAR,CULL_CCW,true,false,0);
		}
	}
	return NULL;
}

bool CItemManager::Init()
{
	//m_pStaticMesh[0] = new CStaticMesh;
	//m_pStaticMesh->LoadMesh(L"Bin/Obj/hero_.X");
	for(int i = 0; i < ITEM_MESH_ID_NUM; ++ i)
	{
		m_pStaticMesh[i] = new CStaticMesh;
		m_pStaticMesh[i]->LoadMesh(GetItemMeshPath(i));
	}
	
	return true;
}
void CItemManager::Release()
{
	for(int i = 0; i < ITEM_MESH_ID_NUM; ++ i)
	{
		SAFE_DELETE(m_pStaticMesh[i]);
	}
}
void CItemMeshObj::Render()
{
	CStaticMeshObj::Render();
}