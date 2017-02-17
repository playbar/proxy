
#pragma once


#include "D3D/D3D.h"
#include "D3D/SkinMesh.h"
#include "item.h"


#define  PLAYER_SPEED 8
#define  MONSTER_NUM  15
#define  MONSTER_WALK_SPEED 4
#define  MONSTER_RUN_SPEED 8 

enum PLAYER_ACTION
{
	PLAYER_ATTACK = 0,
	PLAYER_WALK,
	PLAYER_IDLE,
	PLAYER_DIED,
	PLAYER_CATCH,
	PLAYER_BACK,
	PLAYER_PICKITEM,
};

class CPlayerMeshObj : public CSkinMeshObj
{
public:
	CPlayerMeshObj( D3DXVECTOR3 vPos, float fAngleY, float fScaling )
		:CSkinMeshObj( vPos, fAngleY, fScaling )
	{
	}
	~CPlayerMeshObj()
	{
	}
protected:



public:

	virtual void Render();
};


class CPlayer : public CPlayerMeshObj
{
public:
	CPlayer( D3DXVECTOR3 vPos, float fAngleY, float fScaling )
		:CPlayerMeshObj( vPos, fAngleY, fScaling )
	{
		m_iLevel = 1;
		m_iMaxHP = 100;
		m_iMaxMP = 100;
		m_iHP    = 100;
		m_iMP    = 100;
		m_iState = PLAYER_IDLE;
	}
	~CPlayer(){}

	void PickUpItem(CItem * pItem);

	int						m_iLevel;
	int						m_iMaxHP;
	int						m_iMaxMP;
	int						m_iHP;
	int						m_iMP;
	int						m_iState;
	DWORD                   m_dwKey;
	CItem *					m_pPickItem;
	D3DXVECTOR3				m_vPickDir;
protected:

	TCHAR					m_szName[20];
	int						m_iJob;
	int						m_iMapIndex;
	int						m_iSex;

	D3DXVECTOR3				m_vOldPos;

	D3DXVECTOR3				m_vStartPos;
	D3DXVECTOR3				m_vEndPos;
	D3DXVECTOR3				m_vCurPos;
	double					m_fStartTime;

public:


	void SetJob( int iJob ){ m_iJob = iJob; }
	int	 GetJob(){ return m_iJob; }

	void SetLevel( int iLevel ){ m_iLevel = iLevel; }
	int  GetLevel(){ return m_iLevel; }

	void SetName( TCHAR * szName ){ _tcscpy( m_szName, szName ); }
	TCHAR * GetName(){ return m_szName; }

	void SetMapIndex( int iMapIndex ){ m_iMapIndex = iMapIndex; }
	int  GetMapIndex(){ return m_iMapIndex; }

	void SetSex( int iSex ){ m_iSex = iSex; }
	int  GetSex(){ return m_iSex; }

	virtual void process();
};
enum MONSTER_ACTION
{
	MONSTER_ATTACK = 0,
	MONSTER_WALK,
	MONSTER_IDLE,
	MONSTER_DIED,
	MONSTER_CATCH,
	MONSTER_BACK,
	MONSTER_PICKITEM,
	MONSTER_DEAD,
};

class CMonster : public CPlayer
{
public:
	CMonster(D3DXVECTOR3 vPos, float fAngleY, float fScaling)
		:CPlayer( vPos, fAngleY, fScaling )
	{
		m_iState = MONSTER_IDLE;
		GetPos(m_vMonsterPos);
		m_dTime = APP_Tools::appGetTime();
		m_dEndTime = m_dTime;
		m_vMonsterDir = D3DXVECTOR3(1.f,0.f,0.f);
		m_fRadius = 20;
		m_fViewDis = 10;
		m_fAttDis = 2;
		m_iLevel = 1;
		m_iMaxHP = 100;
		m_iMaxMP = 200;
		m_iHP = 100;
		m_iMP = 200;
		m_dAttTime = m_dTime +1;
		//g_pGame->m_pTerrain->LineCheck(m_vPos - D3DXVECTOR3(0.f,1000,0.f),m_vPos + D3DXVECTOR3(0.f,1000.f,0.f),m_vPos);
	}
	~CMonster()
	{

	}
	void InitMonster()
	{

	}
	void Render()
	{
		CPlayer::Render();
	}
	void process();
	void Walk(double dTimePass);
	void CreateItem(D3DXVECTOR3 vpos,D3DXVECTOR3 vscaling,float fRotationY,int iType,int iNum);


	// 	int						m_iLevel;
	// 	int						m_iMaxHP;
	// 	int						m_iMaxMP;
	// 	int						m_iHP;
	// 	int						m_iMP;

protected:
	D3DXVECTOR3 m_vMonsterPos;
	D3DXVECTOR3 m_vMonsterNextPos;
	D3DXVECTOR3 m_vMonsterDir;
	double m_dTime;
	double m_dEndTime;

	float m_fRadius;
	float m_fViewDis;
	float m_fAttDis;
	double m_dAttTime;
private:
};

enum NPC_ACTION
{
	NPC_ATTACK = 0,
	NPC_WALK,
	NPC_IDLE,
	NPC_DIED,
	NPC_CATCH,
	NPC_BACK,
	NPC_PICKITEM,
};

class CNpc : public CPlayer
{
public:
	CNpc(D3DXVECTOR3 vPos, float fAngleY, float fScaling)
		:CPlayer( vPos, fAngleY, fScaling )
	{
		m_iState = NPC_IDLE;
		m_iLevel = 5;
		m_iMaxHP = 150;
		m_iMaxMP = 300;
		m_iHP = 150;
		m_iMP = 300;
	}
	~CNpc()
	{

	}


	void Render()
	{
		CPlayer::Render();
	}
	void process();
protected:
	D3DXVECTOR3 m_vNPCPos;
	D3DXVECTOR3 m_vNPCNextPos;
	D3DXVECTOR3 m_vNPCDir;

	double m_dTime;
	double m_dEndTime;

	float m_fRadius;
	float m_fViewDis;
	float m_fAttDis;
	double m_dAttTime;
private:
};