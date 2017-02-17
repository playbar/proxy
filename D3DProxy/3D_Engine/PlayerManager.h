
#pragma once 




#include "Player.h"



typedef CHashList<DWORD,CPlayer *> CHashPlayer;

class CPlayerList : public CHashPlayer
{
public:
	CPlayerList( DWORD dwMax ) : CHashPlayer( dwMax )
	{
	}
	~CPlayerList()
	{
	}


protected:


public:


	void InsertPlayer( DWORD dwKey, CPlayer * p )
	{
		p->m_dwKey = dwKey;
		Insert( dwKey, p );
		g_pMyD3D->AddToWorld( p );
	}

	void RemovePlayer( DWORD dwKey )
	{
		delete GetPlayer(dwKey);
		Remove( dwKey );
	}

	CPlayer * GetPlayer( DWORD dwKey )
	{
		POS pos = Find( dwKey );
		if( !pos )
			return NULL;
		return GetAt( pos );
	}

	void ClearAllPlayer()
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

};

class CPlayerManager
{
public:
	CPlayerManager()
	{
		m_pPlayerList = new CPlayerList( 200 );
	}
	~CPlayerManager()
	{
		delete m_pPlayerList;
	}

public:

	CPlayerList	*		m_pPlayerList;

	CSkinMesh *			m_pSkinMesh[2];

	CPlayer * m_pMonster;
protected:

public:

	bool Init();

	void Release();


	CPlayer * GetPlayer( DWORD dwKey )
	{
		return m_pPlayerList->GetPlayer( dwKey );
	}

	CPlayer * GetUser()
	{
		POS pos = m_pPlayerList->GetFirstPos();
		if (!pos) return NULL;
		return m_pPlayerList->GetNext( pos );
	}

	CPlayer * MonsterRayCheck();

	CPlayer * RemovePlayer()
	{
		CPlayer * pPlayer;
		for (POS pos = m_pPlayerList->GetFirstPos(); pos;)
		{
			pPlayer = m_pPlayerList->GetNext(pos);
			if (pPlayer->m_iHP <= 0)
			{
				g_pMyD3D->RemoveFromWorld(pPlayer);
				m_pPlayerList->RemovePlayer(pPlayer->m_dwKey);
			}
		}
	}
};
