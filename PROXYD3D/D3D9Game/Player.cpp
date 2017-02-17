

#include "Game.h"
#include "Player.h"
#include "Table/ItemInfoTable.h"



void CPlayerMeshObj::Render()
{
	CSkinMeshObj::Render();
}

void CPlayer::PickUpItem(CItem * pItem)
{
	m_pPickItem = pItem;

	D3DXVECTOR3 vPlayerPos;
	D3DXVECTOR3 vItemPos;
	GetPos(vPlayerPos);
	pItem->GetPos(vItemPos);

	SetAction("get");
	D3DXVECTOR3 vPlayerDir =vItemPos - vPlayerPos;

	m_vPickDir = vPlayerDir;
	D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);
	float fR = acos(D3DXVec3Dot(&vPlayerDir,&D3DXVECTOR3(0.f,0.f,-1.f)));
	if (vPlayerDir.x > 0)
	{
		fR = 2 * D3DX_PI - fR;
	}
	SetRotationY(fR);
	D3DXVec3Normalize(&m_vPickDir,&m_vPickDir);
	m_iState = PLAYER_PICKITEM;
}
void CPlayer::process()
{
	static double time = APP_Tools::appGetTime();
	double time2 = APP_Tools::appGetTime() - time;
	switch(m_iState)
	{
	case PLAYER_IDLE:
		{
			float R = 0.05f;
			D3DXMATRIX matrixY;
			D3DXVECTOR3 vPlayerPos;
			D3DXVECTOR3 vPlayerDir = D3DXVECTOR3(0.f,0.f,-1.f);
			D3DXVECTOR3 vDes = g_pGame->m_pCamera->GetDesPos();
			D3DXVECTOR3 vDir = g_pGame->m_pCamera->GetPos() - vDes;

			CPlayer * p1 = g_pGame->m_pPlayerManager->GetUser();
			GetPos(vPlayerPos);
			SetAction("idle");
			D3DXMatrixRotationY(&matrixY,p1->GetRotationY());

			APP_Tools::appVec3Transform(vPlayerDir,matrixY);

			vDir.y = 0;
			if( g_pDInput->m_bKeyboard[DIK_W] & 0x80 )
			{
				SetAction("get");
				vPlayerPos += vPlayerDir * (float)(PLAYER_SPEED * time2);
				g_pGame->m_pTerrain->LineCheck(vPlayerPos - D3DXVECTOR3(0.f,1000,0.f),vPlayerPos + D3DXVECTOR3(0.f,1000.f,0.f),vPlayerPos);
			}
			else if( g_pDInput->m_bKeyboard[DIK_S] & 0x80 )
			{
				SetAction("get");
				vPlayerPos -=  vPlayerDir * (float)(PLAYER_SPEED * time2);
				g_pGame->m_pTerrain->LineCheck(vPlayerPos - D3DXVECTOR3(0.f,1000,0.f),vPlayerPos + D3DXVECTOR3(0.f,1000.f,0.f),vPlayerPos);
			}

			D3DXVec3Normalize( &vDir, &vDir );

			if( g_pDInput->m_bKeyboard[DIK_A] & 0x80 )
			{
				SetAction("walk");
				RotationY(-R);
			}
			else if( g_pDInput->m_bKeyboard[DIK_D] & 0x80 )
			{
				SetAction("walk");
				RotationY(R);
			}
			SetPos(vPlayerPos);
			g_pGame->m_pCamera->SetDesPos( vPlayerPos );

			g_pGame->m_pSkyBox->SetPos( vPlayerPos );

		}
		break;
	case PLAYER_PICKITEM:
		{
			D3DXVECTOR3 vItemPosTemp;
			m_pPickItem->GetPos(vItemPosTemp);
			float fLengthTemp = D3DXVec3Length(&(m_vPos - vItemPosTemp));
			if (g_pDInput->m_bKeyboard[DIK_A] || g_pDInput->m_bKeyboard[DIK_D] || g_pDInput->m_bKeyboard[DIK_S] || g_pDInput->m_bKeyboard[DIK_W])
			{
				m_iState = PLAYER_IDLE;
				break;
			}
			if (!g_pGame->m_pItemManager->m_pItemList->GetItem(m_pPickItem->m_dwKey))
			{
				SetAction("idle");
				m_iState = PLAYER_IDLE;
				break;
			}
			if (fLengthTemp < 40)
			{
				SetAction("idle");
				if (g_pGame->m_pItemManager->m_pItemList->GetItem(m_pPickItem->m_dwKey))
				{
					if (g_pUI->m_pGameUI->m_pItemUI->AddItem(rand()%2,rand()%20 + 1))
					{
						g_pGame->m_pItemManager->m_pItemList->GetItem(m_pPickItem->m_dwKey)->m_bPickUp = true;
					}
				}
				m_iState = PLAYER_IDLE;
			}
			m_vPos += m_vPickDir * (float)time2 * PLAYER_SPEED;
			g_pGame->m_pTerrain->LineCheck(m_vPos - D3DXVECTOR3(0.f,1000,0.f),m_vPos + D3DXVECTOR3(0.f,1000.f,0.f),m_vPos);
			g_pGame->m_pPlayerManager->GetUser()->SetPos(m_vPos);
			g_pGame->m_pCamera->SetDesPos(m_vPos);
		}
		break;
	}
	time = APP_Tools::appGetTime();

	D3DXVECTOR3 vPos;
	vPos  = g_pGame->m_pCamera->GetPos();

	g_pGame->m_pTerrain->LineCheck(vPos - D3DXVECTOR3(0.f,1000,0.f),vPos + D3DXVECTOR3(0.f,1000.f,0.f),vPos);
	if (g_pGame->m_pCamera->GetPos().y < vPos.y)
	{
		g_pGame->m_pCamera->SetPos(vPos);
	}
}

void CMonster::process()
{
	double dTimeNow = APP_Tools::appGetTime();
	double dTimePass = dTimeNow - m_dTime;
	D3DXVECTOR3 vTempDir = (*(g_pGame->m_pPlayerManager->GetUser()->GetPos()) - m_vPos);
	vTempDir.y = 0.f;
	float fDis = D3DXVec3Length(&vTempDir);

	if (fDis < m_fViewDis && fDis > m_fAttDis && m_iState != MONSTER_BACK)
	{
		m_iState = MONSTER_CATCH;
		m_vMonsterDir = *(g_pGame->m_pPlayerManager->GetUser()->GetPos()) - m_vPos;
		m_vMonsterDir.y = 0.f;
		D3DXVec3Normalize(&m_vMonsterDir,&m_vMonsterDir);
		m_fAngle = acos(D3DXVec3Dot(&m_vMonsterDir,&D3DXVECTOR3(1.f,0.f,0.f)));
		if (m_vMonsterDir.x > 0)
		{
			m_fAngle = 2 * D3DX_PI - m_fAngle;
		}
		m_dAttTime = dTimeNow + 1;
	}

	switch(m_iState)
	{
	case MONSTER_DEAD:
		SetAction("dead");
		break;
	case MONSTER_ATTACK:
		{
			m_vMonsterDir = *(g_pGame->m_pPlayerManager->GetUser()->GetPos()) - m_vPos;
			m_vMonsterDir.y = 0.f;
			D3DXVec3Normalize(&m_vMonsterDir,&m_vMonsterDir);
			m_fAngle = acos(D3DXVec3Dot(&m_vMonsterDir,&D3DXVECTOR3(1.f,0.f,0.f)));
			if (m_vMonsterDir.x > 0)
			{
				m_fAngle = 2 * D3DX_PI - m_fAngle;
			}
			SetAction("attack");
			if (fDis > m_fAttDis)
			{
				m_iState = MONSTER_CATCH;
			}
			if (dTimeNow > m_dAttTime)
			{
				g_pGame->m_pPlayerManager->GetUser()->m_iHP --;
				m_dAttTime = dTimeNow + 1;
			}
		}
		break;
	case MONSTER_IDLE:
		{
			SetAction("flay");
			if (dTimeNow >= m_dEndTime)
			{
				float fTempZ;
				m_iState = MONSTER_WALK;
				m_vMonsterNextPos.x = rand()%(2*(int)m_fRadius) - m_fRadius + m_vMonsterPos.x;

				fTempZ = (sqrt(pow(m_fRadius,2)-pow(m_vMonsterNextPos.x - m_vMonsterPos.x,2)) * 2) - 
					(sqrt(pow(m_fRadius,2)-pow(m_vMonsterNextPos.x - m_vMonsterPos.x,2)));

				if (fTempZ != 0)
				{
					m_vMonsterNextPos.z = rand()%(int)fTempZ + m_vMonsterPos.z;
				}
				else
				{
					m_vMonsterNextPos.z = m_vMonsterPos.z;
				}
				m_vMonsterNextPos.y = 10.f;
				m_vMonsterDir = m_vMonsterNextPos - m_vPos;
				m_vMonsterDir.y = 0.f;
				D3DXVec3Normalize(&m_vMonsterDir,&m_vMonsterDir);
				m_fAngle = acos(D3DXVec3Dot(&m_vMonsterDir,&D3DXVECTOR3(1.f,0.f,0.f)));
				if (m_vMonsterDir.x > 0)
				{
					m_fAngle = 2 * D3DX_PI - m_fAngle;
				}
			}
		}
		break;
	case MONSTER_WALK:
		{
			Walk(dTimePass);
		}
		break;
	case MONSTER_CATCH:
		{
			SetAction("flay");
			m_vPos += m_vMonsterDir *  (float)(MONSTER_WALK_SPEED * dTimePass);
			g_pGame->m_pTerrain->LineCheck(m_vPos - D3DXVECTOR3(0.f,1000,0.f),m_vPos + D3DXVECTOR3(0.f,1000.f,0.f),m_vPos);
			if (fDis < m_fAttDis)
			{
				m_iState = MONSTER_ATTACK;
			}
			vTempDir = m_vMonsterPos - m_vPos;
			vTempDir.y = 0.f;
			if (D3DXVec3Length(&vTempDir) > m_fRadius)
			{
				m_iState = MONSTER_BACK;

				m_vMonsterDir = m_vMonsterPos - m_vPos;
				m_vMonsterDir.y = 0.f;
				D3DXVec3Normalize(&m_vMonsterDir,&m_vMonsterDir);
				m_fAngle = acos(D3DXVec3Dot(&m_vMonsterDir,&D3DXVECTOR3(1.f,0.f,0.f)));
				if (m_vMonsterDir.x > 0)
				{
					m_fAngle = 2 * D3DX_PI - m_fAngle;
				}
			}
		}
		break;
	case MONSTER_BACK:
		{
			//Walk(dTimePass);
			SetAction("flay");
			m_vPos +=  m_vMonsterDir * (float)(MONSTER_WALK_SPEED * dTimePass);
			g_pGame->m_pTerrain->LineCheck(m_vPos - D3DXVECTOR3(0.f,1000,0.f),m_vPos + D3DXVECTOR3(0.f,1000.f,0.f),m_vPos);
			if (((m_vMonsterPos - m_vPos).x > 0 && m_vMonsterDir.x < 0) ||
				((m_vMonsterPos - m_vPos).x < 0 && m_vMonsterDir.x > 0)  )
			{
				m_iState = MONSTER_IDLE;
				m_dEndTime = APP_Tools::appGetTime() + rand()%10;
			}
		}
		break;
	case MONSTER_PICKITEM:
		{

		}
		break;
	}
	if (dTimePass != 0)
	{
		m_dTime = APP_Tools::appGetTime();
	}
}
void CMonster::Walk(double dTimePass)
{
	SetAction("flay");
	m_vPos +=  m_vMonsterDir * (float)(MONSTER_WALK_SPEED * dTimePass);
	g_pGame->m_pTerrain->LineCheck(m_vPos - D3DXVECTOR3(0.f,1000,0.f),m_vPos + D3DXVECTOR3(0.f,1000.f,0.f),m_vPos);
	if (((m_vMonsterNextPos - m_vPos).x > 0 && m_vMonsterDir.x < 0) ||
		((m_vMonsterNextPos - m_vPos).x < 0 && m_vMonsterDir.x > 0)  )
	{
		m_iState = MONSTER_IDLE;
		m_dEndTime = APP_Tools::appGetTime() + rand()%10;
	}
}
void CMonster::CreateItem(D3DXVECTOR3 vpos,D3DXVECTOR3 vscaling,float fRotationY,int iType,int iNum)
{
	ITEM_INFO * pItemInfo = GetItemInfo((DWORD)iType);
	DWORD dKey = g_pGame->m_pItemManager->GetItemKey();
	CItem * pItem = new CItem(g_pGame->m_pItemManager->m_pStaticMesh[pItemInfo->dwMeshId],vpos,vscaling,fRotationY);
	pItem->m_iType = iType;
	pItem->m_iNum = iNum;
	g_pGame->m_pItemManager->m_pItemList->InsertItem(dKey,pItem);
}


void CNpc ::process()
{

}