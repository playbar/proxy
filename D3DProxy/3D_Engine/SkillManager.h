#pragma once


#include "D3D/Skill.h"
#include "PlayerManager.h"
#include "D3D/base/Texture.h"


typedef CHashList<DWORD,CSkill *> CHashSkill;

class CSkillList : public CHashSkill
{
public:
	CSkillList(DWORD dwMax) : CHashSkill(dwMax)
	{

	}
	~CSkillList()
	{

	}
	void InsertSkill(DWORD dwKey,CSkill * pSkill)
	{
		pSkill->m_dwKey = dwKey;
		Insert(dwKey,pSkill);
	}
	void RemoveSkill(DWORD dwKey)
	{
		delete GetSkill(dwKey);
		Remove(dwKey);
	}
	CSkill * GetSkill(DWORD dwKey)
	{
		POS pos = Find( dwKey );
		if( !pos )
			return NULL;
		return GetAt( pos );
	}
	void ClearAllSkill()
	{
		for( POS pos = GetFirstPos();pos; )
		{
			GetNext( pos );
			if( pos )
				RemoveAt( pos );
			else
				return;
			//
		}
	}
protected:
private:
};

class CSkillManager
{
public:
	CSkillManager()
	{
		m_pSkillList = new CSkillList(200);
		m_dwKey = 0;
		m_pPlalyer = NULL;
		m_pSkill = NULL;
		Init();
	}
	~CSkillManager()
	{
		for (int i = 0; i < m_pTex.Num(); i++)
		{
			SAFE_DELETE(m_pTex[i]);
		}
		delete m_pSkillList;
	}

	CSkillList * m_pSkillList;
	CSkill     * m_pSkill;
	CPlayer    * m_pPlalyer;
	
	Array<CTexture *> m_pTex;

	DWORD m_dwKey;
	DWORD GetKey()
	{
		m_dwKey ++;
		return m_dwKey;
	}
	void Init();

	bool CreateSkill(DWORD dwID);
	void Release();
	CSkill * GetSkill(DWORD dwKey)
	{
		return m_pSkillList->GetSkill(dwKey);
	}

	CSkill * RemoveSkill()
	{
		if (m_pSkillList->GetFirstPos())
		{
			CSkill * pSkill;
			for (POS pos = m_pSkillList->GetFirstPos(); pos;)
			{
				pSkill = m_pSkillList->GetNext(pos);
				if (pSkill->m_pBillBoard->m_wCurFrame >= pSkill->m_pBillBoard->m_wFrameNum - 1)
				{
					g_pMyD3D->RemoveFromWorld(m_pSkillList->GetSkill(pSkill->m_dwKey)->m_pBillBoard);
					m_pSkillList->RemoveSkill(pSkill->m_dwKey);
					return pSkill;
				}
			}
		}
		return NULL;
	}
protected:
private:
};