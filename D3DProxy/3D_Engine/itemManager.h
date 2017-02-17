#pragma once


#include "D3D/D3D.h"
#include "D3D/StaticMesh.h"
#include "item.h"

typedef CHashList<DWORD,CItem *> CHashItem;

enum
{
	ITEM_MESH_ID_RED,
	ITEM_MESH_ID_BLUE,
	ITEM_MESH_ID_BAG,
	ITEM_MESH_ID_0,
	
	ITEM_MESH_ID_NUM,
};
class CItemList : public CHashItem
{
public:
	CItemList(DWORD dwMax):CHashItem(dwMax)
	{

	}
	~CItemList()
	{

	}
	void InsertItem(DWORD dwKey ,CItem * p)
	{
		p->m_dwKey = dwKey;
		Insert(dwKey,p);
		g_pMyD3D->AddToWorld(p);
	}
	void RemoveItem(DWORD dwKey)
	{
		Remove(dwKey);
	}
	CItem * GetItem(DWORD dwKey)
	{
		POS pos = Find(dwKey);
		if (!pos)
		{
			return NULL;
		}
		return GetAt(pos);
	}
	void ClearAllItem()
	{
		for (POS pos = GetFirstPos();pos;)
		{
			GetNext(pos);
			if (pos)
			{
				RemoveAt(pos);
			}
			else
				return;
		}
	}
protected:
private:
};
////////////////////////////////////////////////////////////
class CItemManager
{
public:
	CItemManager()
	{
		m_pItemList = new CItemList(200);
		m_dwItemKey = 0;
	}
	~CItemManager()
	{
		delete m_pItemList;
	}
	DWORD m_dwItemKey;
	CItemList * m_pItemList;
	CStaticMesh * m_pStaticMesh[ITEM_MESH_ID_NUM];
	DWORD GetItemKey()
	{
		return m_dwItemKey++;
	}
	bool Init();
	void Release();
	CItem * RemoveItem()
	{
		CItem * pItem;
		for (POS pos = m_pItemList->GetFirstPos();pos;)
		{
			pItem = m_pItemList->GetNext(pos);
			pItem->m_dTime = APP_Tools::appGetTime();
			if (pItem->m_dTime >pItem->m_dTimeEnd || pItem->m_bPickUp)
			{
				m_pItemList->RemoveItem(pItem->m_dwKey);
				g_pMyD3D->RemoveFromWorld(pItem);
				return pItem;
			}
		}
		return NULL;
	}
	CItem * GetItem(DWORD dwKey)
	{
		return m_pItemList->GetItem(dwKey);
	}

	CItem * ItemRayCheck();

protected:
private:
};