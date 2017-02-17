#include "SkillManager.h"
#include "Game.h"



bool CSkillManager::CreateSkill(DWORD dwID)
{
	if (!dwID)return false;
	DWORD dwIDTemp = dwID;
	if (!m_pPlalyer)
	{
		m_pPlalyer = g_pGame->m_pPlayerManager->GetUser();
	}
	switch(dwIDTemp)
	{
	case 1:
		if (!g_pUI->m_pGameUI->m_pMonsterUI->m_pMonster || m_pPlalyer->m_iMP < 40 || g_pUI->m_pGameUI->m_pMonsterUI->m_pMonster->m_iHP <= 0)return false;
		m_pSkill = new CSkill1(4.f,5.f,0.5f/16.f,m_pPlalyer,g_pUI->m_pGameUI->m_pMonsterUI->m_pMonster);
		m_pPlalyer->m_iMP -= 40;
		g_pUI->m_pGameUI->m_pMonsterUI->m_pMonster->m_iHP -= rand()%15 + 70;
		break;
	case 2:
		m_pSkill = new CSkill2(3.f,3.f,0.05f,m_pPlalyer,m_pPlalyer);
		break;
	case 3:
		m_pPlalyer->m_iHP += 20;
		if (m_pPlalyer->m_iHP > m_pPlalyer->m_iMaxHP)
		{
			m_pPlalyer->m_iHP = m_pPlalyer->m_iMaxHP;
		}
		return true;
		break;
	case 4:
		m_pPlalyer->m_iMP += 20;
		if (m_pPlalyer->m_iMP > m_pPlalyer->m_iMaxMP)
		{
			m_pPlalyer->m_iMP = m_pPlalyer->m_iMaxMP;
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	if (g_pUI->m_pGameUI->m_pMonsterUI->m_pMonster->m_iHP <= 0)
	{
		g_pUI->m_pGameUI->m_pMonsterUI->m_pMonster->m_iState = MONSTER_DEAD;
		//CreateItem(g_pUI->m_pGameUI->m_pMonsterUI->m_pMonster->m_vPos,D3DXVECTOR3(0.005f,0.005f,0.005f),0.f,rand()%7,rand()%10);
	}
	m_pSkillList->InsertSkill(GetKey(),m_pSkill);
	return true;
}

void CSkillManager::Init()
{
	CTexture * m_pTexture;
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/1.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/2.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/3.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/4.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/5.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/6.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/7.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/8.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/9.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/10.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/11.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/12.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/13.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/14.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/15.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other2/16.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/1.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/2.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/3.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/4.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/5.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/6.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/7.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/8.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/9.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/10.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/11.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/12.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/13.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/14.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/15.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/16.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/17.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/18.tga",false);
	m_pTex.AddItem(m_pTexture);
	m_pTexture = new CTexture();
	m_pTexture->CreateFromFile(L"Bin/Other3/19.tga",false);
	m_pTex.AddItem(m_pTexture);
}