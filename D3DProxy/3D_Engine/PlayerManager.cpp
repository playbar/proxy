
#include "PlayerManager.h"
#include "Game.h"


CPlayer * CPlayerManager::MonsterRayCheck()
{
	POINT point;
	g_pGame->GetMousePos(&point);
	RAY ray;
	g_pGame->m_pCamera->GetRay(point.x,point.y,&ray);
	CPlayer * pMonsterTemp = NULL;
	for (POS pos = m_pPlayerList->GetFirstPos();pos;)
	{
		CPlayer * pMonster = m_pPlayerList->GetNext(pos);
		if (pMonster == GetUser())
		{
			continue;
		}
		if (pMonster->RayCheck(ray.vOrig,ray.vVector))
		{
			pMonster->SetCustomRenderState(COLOR_SELECT_TEXTURE,ALPHA_SELECT_TEXTURE,ALPHABLEND_ONE_ONE,FILTER_LINEAR,CULL_CCW,true,false,0);
			pMonsterTemp = pMonster;
		}
		else if(pMonster != m_pMonster)
		{
			pMonster->SetCustomRenderState(COLOR_MODULATE_TEXTURE_CURRENT,ALPHA_SELECT_TEXTURE,ALPHABLEND_NONE,FILTER_LINEAR,CULL_CCW,true,false,0);
		}
	}
	return pMonsterTemp;
}
bool CPlayerManager::Init()
{
	m_pSkinMesh[0] = new CSkinMesh;
	m_pSkinMesh[0]->LoadFromXFile(L"Bin/Obj/hero_.X");
	m_pSkinMesh[1] = new CSkinMesh;
	m_pSkinMesh[1]->LoadFromXFile(L"Bin/Obj/22.X");
	return true;
}


void CPlayerManager::Release()
{
	delete m_pSkinMesh[0];
	delete m_pSkinMesh[1];
}