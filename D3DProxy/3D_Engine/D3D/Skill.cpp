#include "Skill.h"
#include "../Game.h"



CSkill::~CSkill()
{
	if (m_pBillBoard)
	{
		SAFE_DELETE(m_pBillBoard);
	}
}
CSkill::CSkill()
{
}
void CSkill1::InitSkill(float fWidth,float fHeight,float fFrame)
{
	m_pBillBoard = new CBillBoard(BILLBOARD_STAND,fWidth,fHeight,fFrame,D3DXVECTOR3(0.f,0.f,0.5f),D3DXVECTOR3(0.f,0.f,0.f));
	for (int i = 0; i < 16; i ++)
	{
		m_pBillBoard->AddTexture(g_pGame->m_pSkillManager->m_pTex[i]);
	}
	m_pBillBoard->SetCustomRenderState(COLOR_SELECT_TEXTURE,ALPHA_SELECT_TEXTURE,ALPHABLEND_SRCALPHA_INVSRCALPHA,FILTER_LINEAR,CULL_NONE,TRUE,FALSE,0);
}
void CSkill2::InitSkill(float fWidth,float fHeight,float fFrame)
{
	m_pBillBoard = new CBillBoard(BILLBOARD_STAND,fWidth,fHeight,fFrame,D3DXVECTOR3(0.f,0.f,0.5f),D3DXVECTOR3(0.f,0.f,0.f));
	for (int i = 16; i < 35; i++)
	{
		m_pBillBoard->AddTexture(g_pGame->m_pSkillManager->m_pTex[i]);
	}
	m_pBillBoard->SetCustomRenderState(COLOR_SELECT_TEXTURE,ALPHA_SELECT_TEXTURE,ALPHABLEND_SRCALPHA_INVSRCALPHA,FILTER_LINEAR,CULL_NONE,TRUE,FALSE,0);
}

bool CSkill::SetSkill(CPlayer * pPlayer,CPlayer * pMonster,float fX,float fY,float fZ,float fDis)
{
	if ( NULL == pPlayer|| NULL == pMonster)
		return false;
	D3DXVECTOR3 vMonsterPos;
	pMonster->GetPos(vMonsterPos);
	D3DXVECTOR3 vPlayerPos;
	pPlayer->GetPos(vPlayerPos);
	D3DXVECTOR3 vPlayerDis = vMonsterPos - vPlayerPos;
	float fPlayerDis = D3DXVec3Length(&vPlayerDis);
	if (fDis >= fPlayerDis)
	{
		m_pBillBoard->SetPos(D3DXVECTOR3(fX,fY,fZ) + vMonsterPos);
		g_pMyD3D->AddToWorld(m_pBillBoard);
	}
	return true;
}